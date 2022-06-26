#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <debugger.h>


int main(int argc, char** argv) {
	if (argc < 2) {
		printf("nexdebug: Too few arguments!\n");
		exit(0);
	}

	const char* program = argv[1];
	pid_t pid = fork();

	if (pid == 0) {
		// We are the child process, execute program to be debugged.
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		execl(program, program, NULL);
	} else {
		// We are the parent process, execute debugging code.
		debug_proc(pid);
	}
}
