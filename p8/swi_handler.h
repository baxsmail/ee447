#ifndef SWI_HANDLER_H
#define SWI_HANDLER_H
int swi_handler( char * buf, int bufsize, int device_ID, int function_ID );
int nondev_handler( char * buf, int bufsize, int device_ID, int function_ID );
int dev_handler( char * buf, int bufsize, int device_ID, int function_ID );
#endif
