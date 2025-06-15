#include <stdint.h>
#include <stdlib.h>     // strtoul
#include <string.h>     // strcasestr, strstr
#include <unistd.h>     // geteuid, close, ssize_t

#include "socket_ctrl.hpp"   // init_server(), waitConnection(), send_all()
#include "terminal_tools.h"   // WARNING/LOG/ERROR macros
#include "shellProcGuard.hpp"
#define NL  "\r\n"
#define HEAD_END  NL NL
// #define BUF_SIZE 81920
#define BUF_SIZE 4096
#define HTTP_PL(name) name, sizeof(name) - 1

#pragma pack(push, 1)
struct RawPacket {
    float slider[6];
    uint16_t flags;
};
#pragma pack(pop)


/*───────────────────────── static HTML page */
static const char kHTML[] =
#include "page.html"
;

static const char k404[] = "Not Found\n";

/*───────────────────────── compose & send response */
int reply(int fd,
    const char *body, size_t blen,
    const char *status,
    const char *ctype,
    bool keep
) {
    char head[256];
    int hd_len = snprintf(head, sizeof(head),
        "HTTP/1.1 %s" NL
        "Content-Type: %s" NL
        "Content-Length: %zu" NL
        "Connection: %s" NL
        "%s" NL NL,
        status, ctype, blen,
        keep ? "keep-alive" : "close",
        keep ? "Keep-Alive: timeout=5, max=50" : "");

    return (
        send_all(fd, head, hd_len) ||
        (blen ?
            send_all(fd, body, blen) : 0
        )
    );
}

/*───────────────────────── helpers */
static size_t get_content_length(const char *req) {
    const char *h = strcasestr(req, "Content-Length:");
    return
        h ?
            strtoul(h + 15, nullptr, 10) : 0;
}

static const char *find_payLoad(const char *req) {
    const char *p = strstr(req, HEAD_END);
    return
        p ?
            p + 4 : nullptr;
}

/*───────────────────────── main loop */
int sMain() {

    int srv_fd = -1;
    if (
            (srv_fd = init_server(
                (geteuid() == 0) ? // is user root?
                    80 : 8080
            )) < 0
        )
        return 1;

    char buf[BUF_SIZE];
    bool web_run = true;

    while (web_run) {
        int cli_fd = -1;
        if ( (cli_fd = waitConnection(srv_fd)) >= 0 ) {
            // ShellProcessGuard cam("killall motion ; motion -c ./cam.conf");
            sleep(3);
            bool keep = true;
            while (keep) {
                ssize_t rcvd = recv(cli_fd, buf, sizeof(buf) - 1, 0);
                if (rcvd <= 0) {
                    ERROR("recv");
                    break;
                }
                buf[rcvd] = '\0';
                if (strcasestr(buf, "Connection: close"))
                    keep = false;

                /* -------- route -------- */
                if (!strncmp(buf, "POST /ping", 10)) {
                    size_t need = get_content_length(buf);
                    const char *body = find_payLoad(buf);
                    size_t have =
                        body ?
                            ((buf + rcvd) - body) : 0;
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
                        body ?
                            strtoul(body, nullptr, 10) : 0;
                    WARNING("[PING] value from client = %u\n", val);

                    reply(cli_fd, "", 0, "200 OK", "text/plain", keep);

                } else if (!strncmp(buf, "POST /raw", 9)) {
                    size_t need = get_content_length(buf);
                    const char *body = find_payLoad(buf);
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
                        printf("head size %d\n", (int)(body - buf));
                        have = (buf + rcvd) - body;
                    }

                    if (body &&
                        (have >= sizeof(RawPacket))
                    ) {
                        RawPacket pkt;
                        memcpy(&pkt, body, sizeof(pkt));
                        WARNING("[RAW] [%.3f][%.3f][%.3f][%.3f][%.3f][%.3f] [0x%04X]",
                            pkt.slider[0], pkt.slider[1], pkt.slider[2],
                            pkt.slider[3], pkt.slider[4], pkt.slider[5],
                            pkt.flags
                        );
                    } else {
                        WARNING("[RAW] incomplete or missing payload\n");
                    }

                    reply(cli_fd, "", 0, "200 OK", "text/plain", keep);

                } else if (!strncmp(buf, "GET /favicon", 12)) {
                    reply(cli_fd, HTTP_PL(k404), "404 Not Found", "text/plain", keep);
                } else if (!strncmp(buf, "GET /", 5)) {
                    reply(cli_fd, HTTP_PL(kHTML), "200 OK", "text/html", keep);
                } else {
                    reply(cli_fd, HTTP_PL(k404), "404 Not Found", "text/plain", keep);
                }
                if (!keep) break;
            }
            close(cli_fd);
            web_run = false;
        }
    }
    close(srv_fd);
    return 0;
}