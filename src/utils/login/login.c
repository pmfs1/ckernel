//
// Sign on user
//
#include <os.h>
#include <string.h>

#define NAMESIZE 32

int main(int argc, char *argv[]) {
    struct utsname sys;
    char username[NAMESIZE];
    char password[NAMESIZE];
    char *p;
    char ch;
    int rc;
    char enter;
    struct passwd *pwd;
    char *shell;

    // Get system information
    memset(&sys, 0, sizeof(struct utsname));
    uname(&sys);

    // Determine console newline character
    enter = (gettib()->proc->term->type == TERM_CONSOLE) ? '\r' : '\n';

    // Print banner
    write(fdout, sys.sysname, strlen(sys.sysname));
    write(fdout, " version ", 9);
    write(fdout, sys.release, strlen(sys.release));
    write(fdout, " (", 2);
    write(fdout, sys.version, strlen(sys.version));
    write(fdout, ")\r\n", 3);

    for (;;) {
        // Clear credentials
        memset(username, 0, NAMESIZE);
        memset(password, 0, NAMESIZE);

        // Get user name
        if (argc > 1) {
            strncpy(username, argv[1], NAMESIZE);
        } else {
            while (!*username) {
                if (*sys.nodename) {
                    write(fdout, sys.nodename, strlen(sys.nodename));
                    write(fdout, " ", 1);
                }

                write(fdout, "login: ", 7);
                p = username;
                for (;;) {
                    rc = read(fdin, &ch, 1);
                    if (rc < 0) return 0;
                    if (ch == 0x03) return 0;

                    if (ch == '\r' || ch == '\n') {
                        if (ch == enter) {
                            break;
                        } else {
                            continue;
                        }
                    }

                    if (ch > ' ') {
                        write(fdout, &ch, 1);
                        if (p < username + NAMESIZE - 1) *p++ = ch;
                    }
                }
                *p = 0;
                write(fdout, "\r\n", 2);
            }
        }

        // Lookup user in user database
        pwd = getpwnam(username);
        if (pwd && !*pwd->pw_passwd) break;

        // Get password
        write(fdout, "Password: ", 10);
        p = password;
        for (;;) {
            rc = read(fdin, &ch, 1);
            if (rc < 0) return 0;
            if (ch == 0x03) return 0;

            if (ch == '\r' || ch == '\n') {
                if (ch == enter) {
                    break;
                } else {
                    continue;
                }
            }

            if (p < password + NAMESIZE - 1) *p++ = ch;
        }
        *p = 0;
        write(fdout, "\r\n", 2);

        // Check password
        if (pwd) {
            p = crypt(password, pwd->pw_passwd);
            if (p && strcmp(p, pwd->pw_passwd) == 0) {
                memset(password, 0, NAMESIZE);
                break;
            }
        }

        // Login failed
        sleep(2);
        write(fdout, "Login incorrect\r\n", 17);
    }

    // Switch user and group id
    if (initgroups(pwd->pw_name, pwd->pw_gid) < 0 || setgid(pwd->pw_gid) < 0 || setuid(pwd->pw_uid) < 0) {
        write(fdout, "Permission denied\r\n", 19);
        return 0;
    }

    // Launch shell
    shell = pwd->pw_shell;
    if (!shell || !*shell) shell = "/bin/sh";
    if (spawn(P_WAIT, NULL, shell, NULL, NULL) < 0) {
        write(fdout, "No shell\r\n", 10);
        sleep(2);
    }

    return 0;
}
