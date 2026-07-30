#include <stdint.h>
#include <stdlib.h>     // strtoul
#include <string.h>     // strcasestr, strstr
#include <unistd.h>     // geteuid, close, ssize_t

#include "joystick.hpp"
#include "socket_ctrl.hpp"   // init_server(), waitConnection(), send_all()
#include "common_tools.h"   // WARNING/LOG/ERROR macros
#include "shellProcGuard.hpp"

#define NL  "\r\n"
#define HEAD_END  NL NL
// #define BUF_SIZE 81920
#define BUF_SIZE 4096
#define HTTP_PL(name) name, sizeof(name) - 1

#pragma pack(push, 1)
struct RawPacket {
    float       slider[6];
    uint16_t    flags;
};
#pragma pack(pop)


/*───────────────────────── static HTML page */
static const char kHTML[] =
#include "page.html"
;

static const char k404[] = "Not Found\n";

static const uint8_t Net2Host[16] = {
    BUTTON_CROSS,          /* 0  */
    BUTTON_ROUND,          /* 1  */
    BUTTON_SQUARE,         /* 2  */
    BUTTON_TRIANGLE,       /* 3  */
    BUTTON_L1,             /* 4  */
    BUTTON_R1,             /* 5  */
    BUTTON_MUTE,           /* 6  placeholder (L2)         */
    BUTTON_MUTE,           /* 7  placeholder (R2)         */
    BUTTON_SHARE,          /* 8  */
    BUTTON_MENU,           /* 9  */
    BUTTON_L3,             /* 10 */
    BUTTON_R3,             /* 11 */
    BUTTON_DPAD_UP,        /* 12 */
    BUTTON_DPAD_DOWN,      /* 13 */
    BUTTON_DPAD_LEFT,      /* 14 */
    BUTTON_DPAD_RIGHT      /* 15 */
};

/* Convert network packet -> host-side bitmask (enum positions)             */
static inline uint16_t unpackButtons(uint16_t pkt) {
    uint16_t hostMask = 0x0000;
    uint16_t netMask = ntohs(pkt);

    while (netMask) {
        uint8_t bit = __builtin_ctz(netMask);
        netMask &= netMask - 1;

        hostMask |= (uint16_t)(1u << Net2Host[bit]);
    }
    return hostMask;
}

/*───────────────────────── compose & send response */
int reply(int fd,
    cStringRO body, size_t blen,
    cStringRO status,
    cStringRO ctype,
    bool keep
) {
    char head[256];
    int hd_len = snprintf(
        head, sizeof(head),
        "HTTP/1.1 %s" NL
        "Content-Type: %s" NL
        "Content-Length: %zu" NL
        "Connection: %s" NL
        "%s" NL NL,
        status,
        ctype,
        blen,
        (keep ?
            "keep-alive" : "close"),
        (keep ?
            "Keep-Alive: timeout=5, max=50" : "")
    );

    return (
        send_all(fd, head, hd_len) ||
        (blen ?
            send_all(fd, body, blen) : 0)
        );
}

/*───────────────────────── helpers */
size_t get_content_length(cStringRO req) {
    cStringRO h = strcasestr(req, "Content-Length:");
    return
        (h ?
            strtoul(h + 15, nullptr, 10) : 0);
}

cStringRO find_payLoad(cStringRO req) {
    cStringRO p = strstr(req, HEAD_END);
    return
        (p ?
            p + 4 : nullptr);
}

/*───────────────────────── main loop */
void Web_GCHandler() {

    int srv_fd = -1;
    if (
        (srv_fd = init_server(
            ((geteuid() == 0) ? // is user root?
                80 : 8080)
        )) < 0
        )   return;

    char buf[BUF_SIZE];

    while (!jQuit) {
        int cli_fd = -1;
        if ((cli_fd = waitConnection(srv_fd)) >= 0) {
            // ShellProcessGuard cam("killall motion ; motion -c ./cam.conf");
            sleep(3);
            bool keep = true;
            while (keep) {
                ssize_t rcvd = recv(cli_fd, buf, sizeof(buf) - 1, 0);
                if (rcvd <= 0) {
                    ERROR("recv");
                    close(cli_fd);
                    while (!((cli_fd = waitConnection(srv_fd)) >= 0)) {}
                    continue;
                }
                buf[rcvd] = '\0';
                if (strcasestr(buf, "Connection: close"))
                    keep = false;

                /* -------- route -------- */
                if (!strncmp(buf, "POST /ping", 10)) {
                    size_t need = get_content_length(buf);
                    cStringRO body = find_payLoad(buf);
                    size_t have =
                        (body ?
                            ((buf + rcvd) - body) : 0);
                    while (
                        (have < need) &&
                        (need < BUF_SIZE)
                        ) {
                        ssize_t add = recv(cli_fd, buf + rcvd, sizeof(buf) - 1 - rcvd, 0);
                        if (add <= 0) {
                            ERROR("recv body");
                            break;
                        }
                        rcvd += add;
                        buf[rcvd] = '\0';
                        body = find_payLoad(buf);
                        have = (buf + rcvd) - body;
                    }

                    uint32_t val =
                        (body ?
                            strtoul(body, nullptr, 10) : 0);
                    WARNING("[PING] value from client = %u\n", val);

                    reply(cli_fd, "", 0, "200 OK", "text/plain", keep);

                }
                else if (!strncmp(buf, "POST /raw", 9)) {
                    size_t need = get_content_length(buf);
                    cStringRO body = find_payLoad(buf);
                    size_t have =
                        body ?
                        (buf + rcvd) - body : 0;
                    while (
                        (have < need) &&
                        (need < BUF_SIZE)
                        ) {
                        ssize_t add = recv(cli_fd, buf + rcvd, sizeof(buf) - 1 - rcvd, 0);
                        if (add <= 0) {
                            ERROR("recv raw body");
                            break;
                        }
                        rcvd += add;
                        buf[rcvd] = '\0';
                        body = find_payLoad(buf);
                        // printf("head size %d\n", (int)(body - buf));
                        have = (buf + rcvd) - body;
                    }

                    if (body &&
                        (have >= sizeof(RawPacket))
                        ) {
                        RawPacket pkt;
                        memcpy(&pkt, body, sizeof(pkt));
                        // pkt.flags = ntohs(pkt.flags);
                        WARNING(
                            "[RAW] [%.3f][%.3f][%.3f][%.3f][%.3f][%.3f] ",
                            // WORD_TO_BINARY_PATTERN,
                            pkt.slider[0], pkt.slider[1], pkt.slider[2],
                            pkt.slider[3], pkt.slider[4], pkt.slider[5]
                            // pkt.flags
                            // WORD_TO_BINARY(pkt.flags)
                        );
#if 0
                        gp.left.x = (pkt.slider[0] - 0.5f) * 2;
                        gp.left.y = (pkt.slider[1] - 0.5f) * 2;
                        gp.left.z = pkt.slider[4];
                        gp.right.x = (pkt.slider[2] - 0.5f) * 2;
                        gp.right.y = (pkt.slider[3] - 0.5f) * 2;
                        gp.right.z = pkt.slider[5];
                        gp.btns = unpackButtons(pkt.flags);
#else
                        GamePad tgp = {
                            .left = {
                                (pkt.slider[0] - 0.5f) * 2,
                                (pkt.slider[1] - 0.5f) * 2,
                                pkt.slider[4]
                            },
                            .right = {
                                (pkt.slider[2] - 0.5f) * 2,
                                (pkt.slider[3] - 0.5f) * 2,
                                pkt.slider[5]
                            },
                            .btns = unpackButtons(pkt.flags)
                        };
                        gp = tgp;
#endif
                    }
                    else    WARNING("[RAW] incomplete or missing payload\n");

                    reply(cli_fd, "", 0, "200 OK", "text/plain", keep);

                }
                else if (!strncmp(buf, "GET /favicon", 12)) reply(cli_fd, HTTP_PL(k404), "404 Not Found", "text/plain", keep);
                else if (!strncmp(buf, "GET /", 5))         reply(cli_fd, HTTP_PL(kHTML), "200 OK", "text/html", keep);
                else                                        reply(cli_fd, HTTP_PL(k404), "404 Not Found", "text/plain", keep);

                if (!keep) break;
            }
            close(cli_fd);
            jQuit = true;
        }
    }
    close(srv_fd);
    return;
}