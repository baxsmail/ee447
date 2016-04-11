#ifndef USER_API_H
#define USER_API_H
#include "hype.h"
#include "syscall.h"

int read (unsigned int device type, int id, char *buf, int len);
int write (unsigned int device type, int id, char *buf, int len);
int new_process (char *executable);
int new_thread (pfv_t entrypoint);
void yield();
void Uerror( char* str);

#endif
