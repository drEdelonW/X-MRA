#pragma once
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

/*
 * ShellProcessGuard
 *  - Spawns a shell command in its own process group.
 *  - On destruction sends SIGTERM, waits a moment, then SIGKILL to the group.
 *  - Reaps every child to avoid zombies.
 *  - No STL, pure POSIX.
 */
class ShellProcessGuard {
public:
    explicit ShellProcessGuard(const char *cmd)
        : pid_(-1), pgid_(-1)
    {
        pid_ = fork();
        if (pid_ < 0) {                 /* fork failed */
            perror("fork");
            return;
        }

        if (pid_ == 0) {                /* child */
            setsid();                   /* new session; pgid == pid */
            execl("/bin/sh", "sh", "-c", cmd, (char *)0);
            perror("exec");
            _exit(127);                 /* exec unreachable => error */
        }

        /* parent */
        pgid_ = pid_;                   /* store process-group id */
        fprintf(stderr,
                "[PG] started PID %d (PGID %d)\n",
                pid_, pgid_);
    }

    /* non-copyable, movable */
    ShellProcessGuard(const ShellProcessGuard&)            = delete;
    ShellProcessGuard& operator=(const ShellProcessGuard&) = delete;
    ShellProcessGuard(ShellProcessGuard&& o) noexcept
        : pid_(o.pid_), pgid_(o.pgid_) { o.pid_ = o.pgid_ = -1; }
    ShellProcessGuard& operator=(ShellProcessGuard&& o) noexcept {
        if (this != &o) {
            terminate();
            pid_  = o.pid_;
            pgid_ = o.pgid_;
            o.pid_ = o.pgid_ = -1;
        }
        return *this;
    }

    ~ShellProcessGuard() { terminate(); }

private:
    pid_t pid_;
    pid_t pgid_;

    void terminate() {
        if (pgid_ <= 0) return;

        /* 1) graceful shutdown */
        kill(-pgid_, SIGTERM);          /* negative pgid => whole group */
        usleep(1000000);                /* 1 s grace period */

        /* 2) hard kill if still alive */
        kill(-pgid_, SIGKILL);

        /* 3) reap all children in the group */
        int status = 0;
        while (waitpid(-pgid_, &status, 0) > 0) {
            if (WIFEXITED(status))
                fprintf(stderr,
                        "[PG] member exited code %d\n",
                        WEXITSTATUS(status));
            else if (WIFSIGNALED(status))
                fprintf(stderr,
                        "[PG] member killed by sig %d\n",
                        WTERMSIG(status));
        }
        pgid_ = pid_ = -1;
    }
};
