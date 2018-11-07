#ifndef _MAIN_H
#define _MAIN_H
#include <pthread.h>
#include <stdlib.h>

typedef struct THREAD_COTR{//命令行参数
	struct THREAD_COTR * this;
	char name[48]; //线程名字
	int tid;
	pthread_t app_thread_id;
	int (* thread_func)(void *);//线程函数
	void * para;//参数
	int run_status;//当前运行状态;
	int conf_status;//配置开关
	int dependent;//运行所依赖的线程
	int index;//线程私有索引
	pthread_mutex_t * mut[2];
	pthread_cond_t  * cond[2];
	int control[20];
}__attribute__ ((packed,aligned(1)))THREAD_COTR_S;




typedef struct APP{//命令行参数
	struct APP * self;
	THREAD_COTR_S app_thread[30];
}__attribute__ ((packed,aligned(4)))APP_S;
#endif