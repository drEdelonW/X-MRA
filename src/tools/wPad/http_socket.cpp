#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "terminal_tools.h"   // WARNING/LOG

#define NL  "\r\n"
#define BUF_SIZE 4096
#define HTTP_PL(name) name, sizeof(name) - 1

/*───────────────────────── static HTML page */
static const char kHTML[] = R"(
<!DOCTYPE html>
<html>
  <head><meta charset='utf-8'><title>Counter</title></head>
  <body>
    <h3>Clicks: <span id='cnt'>-1</span></h3>
    <button id='btn'>ADD</button>
    <script>
      let cnt = 13;
      const span = document.getElementById('cnt');
      document.getElementById('btn').onclick = () => {
        span.textContent = ++cnt;
        fetch('/ping', {
          method: 'POST',
          headers: {'Content-Type': 'text/plain'},
          body: String(cnt)
        });
      };
    </script>
  </body>
</html>
)";


static const char k404[] = "Not Found\n";

/*───────────────────────── send_all */
static int send_all(int fd, const char *buf, size_t len) {
    while (len) {
        ssize_t sent = send(fd, buf, len, 0);
        if (sent <= 0) {
            perror("send"); return -1;
        }
        buf += sent;
        len -= sent;
    }
    return 0;
}

/*───────────────────────── init listener */
static int init_server(uint16_t port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return -1;
    }
    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in sa = {
        .sin_family = AF_INET,
        .sin_port   = htons(port),
        .sin_addr   = {
            .s_addr = INADDR_ANY
        }
    };
    if (bind(sock, (sockaddr *)&sa, sizeof(sa))) {
        perror("bind"); close(sock);
        return -1;
    }
    if (listen(sock, 8)) {
        perror("listen"); close(sock);
        return -1;
    }

    WARNING("Listening on %d...\n", port);
    return sock;
}

/*───────────────────────── compose & send response */
static int reply(
    int fd, const char *body, size_t blen,
    const char *status, const char *ctype, bool keep
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

static const char *find_body(const char *req) {
    const char *p = strstr(req, NL NL);
    return
        p ?
            p + 4 : nullptr;
}

/*───────────────────────── main loop */
int sMain() {
    uint16_t port =
        (geteuid() == 0) ?
            80 : 8080;
    int srv_fd = init_server(port);
    if (srv_fd < 0)
        return 1;

    char buf[BUF_SIZE];

    for (;;) {
        sockaddr_in cli{};
        socklen_t l = sizeof(cli);
        int cli_fd = accept(srv_fd, (sockaddr *)&cli, &l);
        if (cli_fd < 0) {
            perror("accept");
            continue;
        }
        WARNING(
            "Client %s:%d connected\n",
            inet_ntoa(cli.sin_addr),
            ntohs(cli.sin_port)
        );

        bool keep = true;
        while (keep) {
            ssize_t rcvd = recv(cli_fd, buf, sizeof(buf) - 1, 0);
            if (rcvd <= 0) {
                perror("recv");
                break;
            }
            buf[rcvd] = '\0';

            if (strcasestr(buf, "Connection: close"))
                keep = false;

            /* -------- route -------- */
            if (!strncmp(buf, "POST /ping", 10)) {
                size_t need = get_content_length(buf);
                const char *body = find_body(buf);
                size_t have =
                    body ?
                        ((buf + rcvd) - body) : 0;
                while (
                    (have < need) &&
                    (need < BUF_SIZE)
                ) {
                    ssize_t add = recv(cli_fd, buf + rcvd, sizeof(buf) - 1 - rcvd, 0);
                    if (add <= 0) {
                        perror("recv body");
                        break;
                    }
                    rcvd += add;
                    buf[rcvd] = '\0';
                    body = find_body(buf);
                    have = (buf + rcvd) - body;
                }
                uint32_t val =
                    body ?
                        strtoul(body, nullptr, 10) : 0;
                WARNING("[PING] value from client = %u\n", val);
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
    }
    close(srv_fd);
    return 0;
}