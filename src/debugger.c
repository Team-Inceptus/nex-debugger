#include <debugger.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>

static int wait_status;

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
