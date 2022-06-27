#include <debugger.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regs.h>

// Max digits the step count can be.
#define STEP_MAX_DIGITS 8


static int wait_status;


static int single_step(pid_t pid) {
    int status;
    int ret = ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL);

    if (ret > 0) {
        return ret;
    }

    waitpid(pid, &status, 0);
    return status;
}


static void exec_command(const char* command, pid_t child_pid) {
    if (strncmp(command, "continue", 8) == 0) {
        // Start execution of child process.
        ptrace(PTRACE_CONT, child_pid, NULL, NULL);
    }
}


void debug_proc(pid_t pid) {
    int options = 0;
    waitpid(pid, &wait_status, options);

    char buffer[50];

    while (1) {
        printf("nexdebug> ");
        fgets(buffer, sizeof(buffer), stdin);
        exec_command(buffer, pid);
    }
}
