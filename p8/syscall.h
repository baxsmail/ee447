#ifndef SYSCALL_H
#define SYSCALL_H


typedef int (*Syscall) (struct Interrupt_State * state);

#define SUCCESS 0
#define FAILED -1

#define REGS_0
#define REGS_1
#define REGS_2
#define REGS_3
#define REGS_4

#define DEF_SYSCALL( funcName, num, retType, params, argDefs, regs)	\
retType name params	{	\
	int syscallNum = (num);		\
	int retVal;				\
	argDefs		\
	/* some assembly */		\
	/* mov r0 to retVal */	\
	return retVal;			\
}

enum {
    Sys_Null,
    Sys_WriteLED,
    Sys_ReadLED,
    Sys_WriteSD,
    Sys_ReadSD,
    Sys_WriteClock,
    Sys_ReadClock,
    Sys_NewProc,
    Sys_NewThread,
    Sys_Exit,
    Sys_Max
};

const Syscall g_syscallTable[] ;

#endif


/*
DEF_SYSCALL(Spawn_Program, SYS_SPAWN, int,
                (const char *program, const char *command,
                 int background), const char *arg0 = program;
            size_t arg1 = strlen(program);
            const char *arg2 = command;
            size_t arg3 = strlen(command);
            int arg4 = background;
            , SYSCALL_REGS_5)
*/

