#define _GNU_SOURCE

#include<stdio.h>
#include <sched.h>
#include <sys/mman.h>
#include <sys/utsname.h>
#include <wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

const char *HOSTNAME = "GoServer";

int childFunc(void *arg) {

    if (chroot("./rootfs") == -1) {
        perror("Child: error with chroot");
        return -1;
    }

    if (sethostname(HOSTNAME, strlen(HOSTNAME)) == -1) {
        perror("Child: Can not set host name");
    }

    struct utsname uts;
    if (uname(&uts) == -1) {
        perror("uname");
        return -1;
    }
    printf("Child: hostname=%s\n", uts.nodename);
    execl(arg, arg, NULL);
    perror(arg);
    return -1;
}

int main(int argc, char **argv) {

    if (argc < 2) {
        printf("usage: %s <program>\n", argv[0]);
        return -1;
    }

    const int STACK_SIZE = 1024 * 1024;
    char *stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
    if (stack == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    char *stackTop = stack + STACK_SIZE;

    int childPid;
    if ((childPid = clone(childFunc, stackTop, CLONE_NEWNS | CLONE_NEWUTS | SIGCHLD, argv[1])) == -1) {
        perror("Parent: can not clone");
        return -1;
    }
    printf("Parent: my pid=%d\n", getpid());
    printf("Parent: after clone child have pid=%d\n", childPid);

    struct utsname uts;
    if (uname(&uts) == -1) {
        perror(argv[0]);
        return -1;
    }
    printf("Parent: hostname=%s\n", uts.nodename);
    int status;
    if (waitpid(childPid, &status, 0) == -1) {
        perror(argv[0]);
        return -1;
    }
    if (WIFEXITED(status)) {
        printf("Parent: My child with id=%d terminated with code %d\n", childPid, WEXITSTATUS(status));
    }
    if (WIFSIGNALED(status)) {
        printf("Parent: My child with id=%d terminated with signal %d\n", childPid, WTERMSIG(status));
    }
    return 0;
}
