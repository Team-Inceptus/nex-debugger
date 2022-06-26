#include <regs.h>
#include <stdlib.h>
#include <sys/ptrace.h>


// MUST FREE REGS YOURSELF AFTER IT RETURNS.
static struct user_regs_struct* get_regs(pid_t pid) {
    struct user_regs_struct* regs = malloc(sizeof(struct user_regs_struct));
    if (ptrace(PTRACE_GETREGS, pid, NULL, regs)) {
        free(regs);
        return NULL;
    }

    return regs;
}


uint8_t get_instruction_pointer(pid_t pid, uint64_t* rip) {
    struct user_regs_struct* regs = get_regs(pid);

    if (regs == NULL) {
        return 1;
    }

    *rip = regs->rip;
    free(regs);
    return 0;
}
