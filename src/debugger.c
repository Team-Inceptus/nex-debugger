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
    } else if (strncmp(command, "ss", 2) == 0) {
        // Single step.
        command += 2;
        while (*command == ' ') {
            ++command;
        }

        // Get how many steps we want in string form.
        char n_steps_str[STEP_MAX_DIGITS];
        strncpy(n_steps_str, command, STEP_MAX_DIGITS);
        n_steps_str[STEP_MAX_DIGITS - 1] = '\0';

        // Get n steps in integer form.
        int n_steps = atoi(n_steps_str);

        // Save original instruction pointer.
        uint64_t orig_rip = 0;
        get_instruction_pointer(child_pid, &orig_rip);

        // Step n times.
        for (uint32_t i = 0; i < n_steps; ++i) {
            single_step(child_pid);
        }

        // Let the user know how far they stepped.
        uint64_t new_rip = 0;
        get_instruction_pointer(child_pid, &new_rip);
        printf("Stepped RIP from 0x%X to 0x%X\n", orig_rip, new_rip);
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
