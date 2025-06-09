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

#define BUF_SIZE 4096

/*───────────────────────── send_all */
static int send_all(int fd, const char *buf, size_t len) {
    while (len) {
        ssize_t n = send(fd, buf, len, 0);
        if (n <= 0) { perror("send"); return -1; }
        buf += n; len -= n;
    }
    return 0;
}

/*───────────────────────── init listener */
static int init_server(uint16_t port) {
    int s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) { perror("socket"); return -1; }
    int opt = 1; setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in sa{}; sa.sin_family = AF_INET; sa.sin_addr.s_addr = INADDR_ANY; sa.sin_port = htons(port);
    if (bind(s, (sockaddr *)&sa, sizeof(sa))) { perror("bind"); close(s); return -1; }
    if (listen(s, 8)) { perror("listen"); close(s); return -1; }

    WARNING("Listening on %d...\n", port);
    return s;
}

/*───────────────────────── static HTML page */
static const char kHTML[] =
"<!DOCTYPE html><html><head><meta charset='utf-8'><title>Counter</title></head>"
"<body><h3>Clicks: <span id='cnt'>0</span></h3>"
"<button id='btn'>ADD</button>"
"<script>\n"
"let cnt=150; const span=document.getElementById('cnt');\n"
"document.getElementById('btn').onclick=()=>{\n"
"  span.textContent=++cnt;\n"
"  fetch('/ping',{method:'POST',headers:{'Content-Type':'text/plain'},body:String(cnt)});\n"
"};\n"
"</script></body></html>\n";

static const char k404[] = "Not Found\n";

/*───────────────────────── compose & send response */
static int reply(int fd, const char *body, size_t blen,
                 const char *status, const char *ctype, bool keep) {
    char head[256];
    int n = snprintf(head, sizeof(head),
        "HTTP/1.1 %s\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: %s\r\n"
        "%s\r\n\r\n",
        status, ctype, blen,
        keep ? "keep-alive" : "close",
        keep ? "Keep-Alive: timeout=5, max=50" : "");

    return send_all(fd, head, n) || (blen ? send_all(fd, body, blen) : 0);
}

/*───────────────────────── helpers */
static size_t get_content_length(const char *req) {
    const char *h = strcasestr(req, "Content-Length:");
    return h ? strtoul(h + 15, nullptr, 10) : 0;
}

static const char *find_body(const char *req) {
    const char *p = strstr(req, "\r\n\r\n");
    return p ? p + 4 : nullptr;
}

/*───────────────────────── main loop */
int sMain() {
    uint16_t port = geteuid() == 0 ? 80 : 8080;
    int sfd = init_server(port);
    if (sfd < 0) return 1;

    char buf[BUF_SIZE];

    for (;;) {
        sockaddr_in cli{}; socklen_t l = sizeof(cli);
        int cfd = accept(sfd, (sockaddr *)&cli, &l);
        if (cfd < 0) { perror("accept"); continue; }
        WARNING("Client %s:%d connected\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));

        bool keep = true;
        while (keep) {
            ssize_t r = recv(cfd, buf, sizeof(buf) - 1, 0);
            if (r <= 0) { perror("recv"); break; }
            buf[r] = '\0';

            if (strcasestr(buf, "Connection: close")) keep = false;

            /* -------- route -------- */
            if (!strncmp(buf, "POST /ping", 10)) {
                size_t need = get_content_length(buf);
                const char *body = find_body(buf);
                size_t have = body ? (buf + r) - body : 0;
                while (have < need && need < BUF_SIZE) {
                    ssize_t add = recv(cfd, buf + r, sizeof(buf) - 1 - r, 0);
                    if (add <= 0) { perror("recv body"); break; }
                    r += add; buf[r] = '\0';
                    body = find_body(buf); have = (buf + r) - body;
                }
                uint32_t val = body ? strtoul(body, nullptr, 10) : 0;
                WARNING("[PING] value from client = %u\n", val);
                reply(cfd, "", 0, "200 OK", "text/plain", keep);
            }
            else if (!strncmp(buf, "GET /favicon", 12)) {
                reply(cfd, k404, sizeof(k404) - 1, "404 Not Found", "text/plain", keep);
            }
            else if (!strncmp(buf, "GET /", 5)) {
                reply(cfd, kHTML, sizeof(kHTML) - 1, "200 OK", "text/html", keep);
            }
            else {
                reply(cfd, k404, sizeof(k404) - 1, "404 Not Found", "text/plain", keep);
            }
            if (!keep) break;
        }
        close(cfd);
    }
    close(sfd);
    return 0;
}