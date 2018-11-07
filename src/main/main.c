#include <stdio.h>
//#include <stlib.h>
#include <main.h>

APP_S app;


#include <monitor.h>
int main(int argc,char ** argv)
{
	fs_monitor(&app);
	printf("ok let's start\n");
}