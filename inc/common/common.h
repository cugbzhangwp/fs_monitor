#ifndef _COMMONFUN_H
#define _COMMONFUN_H


extern char msb2lsb(char ch);
extern void GetCompileTime(char * compile_t);
extern int mywrite(char * buffer_write,int needwritebyte,FILE * fp);
extern int myread(char * buffer_write,int needwritebyte,FILE * fp);
extern int is_dir_exist(const char * pathname);
extern int try_system(char * cmd);

#endif