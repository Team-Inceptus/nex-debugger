#ifndef REGS_H
#define REGS_H

#include <stdint.h>
#include <sys/user.h>
#include <unistd.h>

uint8_t get_instruction_pointer(pid_t pid, uint64_t* rip);


#endif
