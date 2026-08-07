#include "socket_ctrl.hpp"
#include "terminal_tools.h"   // WARNING/LOG


/*───────────────────────── init listener */
int init_server(uint16_t port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        ERROR("socket");
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
        close(sock);
        ERROR("bind");
        return -1;
    }
    if (listen(sock, 8)) {
        close(sock);
        ERROR("listen");
        return -1;
    }

    WARNING("Listening on %d...\n", port);
    return sock;
}

int waitConnection(int srv_fd) {
    sockaddr_in cli{};
    socklen_t l = sizeof(cli);
    int cli_fd = accept(srv_fd, (sockaddr *)&cli, &l);
    if (cli_fd < 0) {
        ERROR("accept");
        return cli_fd;
    }

    WARNING(
        "Client %s:%d connected\n",
        inet_ntoa(cli.sin_addr),
        ntohs(cli.sin_port)
    );
    return cli_fd;
}

/*───────────────────────── send_all */
int send_all(int fd, cStrRO buf, size_t len) {
    while (len) {
        ssize_t sent = send(fd, buf, len, 0);
        if (sent <= 0) {
            ERROR("send");
            return -1;
        }
        buf += sent;
        len -= sent;
    }
    return 0;
}