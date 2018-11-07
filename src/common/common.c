#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <string.h>
#include <sys/vfs.h>
#include <getopt.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <printcolor.h>
char msb2lsb(char ch)
{
	int ifor;
	unsigned char hcdata[8];
	for(ifor=0;ifor<8;ifor++)
	{
		hcdata[ifor]=((ch>>ifor)&0x1)<<(7-ifor);
	}
	return hcdata[0]|hcdata[1]|hcdata[2]|hcdata[3]|hcdata[4]|hcdata[5]|hcdata[6]|hcdata[7];
}



#define MONTH_PER_YEAR   12   // 一年12月
#define YEAR_MONTH_DAY   20   // 年月日缓存大小
#define HOUR_MINUTES_SEC 20   // 时分秒缓存大小

void GetCompileTime(char * compile_t)
{
  const char year_month[MONTH_PER_YEAR][4] =
  { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  char compile_date[YEAR_MONTH_DAY] = {0}, compile_time[HOUR_MINUTES_SEC] = {0}, i;
  char str_month[4] = {0};
  int year, month, day, hour, minutes, seconds;

  sprintf(compile_date, "%s", __DATE__); // "Aug 23 2016"
  sprintf(compile_time, "%s", __TIME__); // "10:59:19"

  sscanf(compile_date, "%s %d %d", str_month, &day, &year);
  sscanf(compile_time, "%d:%d:%d", &hour, &minutes, &seconds);

  for(i = 0; i < MONTH_PER_YEAR; ++i)
  {
    if(strncmp(str_month, year_month[i], 3) == 0)
    {
      month = i + 1;
      break;
    }
  }

  snprintf(compile_t,20,"%d-%d-%d %d:%d:%d\n", year, month, day, hour, minutes, seconds);
}


/*
int writedbgfile(char * buf,unsigned int m, FILE * fp)
{
	int realwrite,nwrite,writetimes;
	realwrite=nwrite=writetimes=0;
	do{//写文件部分
	ioctl(fdio, GPIO_IOC_N_SD_LED1_ON,1);
	nwrite=fwrite(buffer_write+realwrite, 1,needwritebyte-realwrite,fp);
	writetimes++;
	realwrite+=nwrite;
	ioctl(fdio, GPIO_IOC_N_SD_LED1_OFF,1);
	}while(realwrite!=needwritebyte);
	return realwrite;
}
*/


int try_system(const char * cmd)
{
    pid_t status;
    printf("you want me docmd %s\n",cmd);
    status = system(cmd);

    if (-1 == status)
    {
        printf("system error!");
        return -1;
    }
    else
    {
        printf("exit status value = [0x%x]\n", status);

        if (WIFEXITED(status))
        {
            if (0 == WEXITSTATUS(status))
            {
              printf("run shell script successfully.\n");
              return 0;
            }
            else
            {
                printf("run shell script fail, script exit code: %d\n", WEXITSTATUS(status));
                return -2;
            }
        }
        else
        {
            printf("exit status = [%d]\n", WEXITSTATUS(status));
            return -3;
        }
    }
    return -1;
}


int mywrite(char * buffer_write,int needwritebyte,FILE * fp)
{

  int realwrite,nwrite,writetimes;
  realwrite=nwrite=writetimes=0;
  do{//写文件部分
  //ioctl(fdio, GPIO_IOC_N_SD_LED1_ON,1);
  nwrite=fwrite(buffer_write+realwrite, 1,needwritebyte-realwrite,fp);
  writetimes++;
  realwrite+=nwrite;
  //ioctl(fdio, GPIO_IOC_N_SD_LED1_OFF,1);
  }while(realwrite!=needwritebyte);
  return realwrite;
}



//i_state=fread(&para,sizeof(para_s),1,fp_para);

int myread(char * buffer_write,int needwritebyte,FILE * fp)
{

  int realwrite,nwrite,writetimes;
  realwrite=nwrite=writetimes=0;
  do{//写文件部分
  //ioctl(fdio, GPIO_IOC_N_SD_LED1_ON,1);
  nwrite=fread(buffer_write+realwrite, 1,needwritebyte-realwrite,fp);
  writetimes++;
  realwrite+=nwrite;
  //ioctl(fdio, GPIO_IOC_N_SD_LED1_OFF,1);
  }while(realwrite!=needwritebyte);
  printf(GREEN"inside %s line %d\n"NONE, __FUNCTION__,__LINE__);
  printf("needwritebyte=%d,realwrite=%d",needwritebyte,realwrite);
  return realwrite;
}







int is_dir_exist(const char * pathname)
{
  //printf(PURPLE"IN is_dir_exist\n"NONE);
  if(pathname==NULL)
    return -1;
  if(opendir(pathname)==NULL)
    return -1;
  //printf(PURPLE"IN is_dir_exist end\n"NONE);
  return 0;
}


int file_size(char* filename)  
{  
  struct stat statbuf;  
  stat(filename,&statbuf);  
  int size=statbuf.st_size;  
  return size;  
} 

int is_file_exist(const char * file)
{
  if (access(file, F_OK) == 0){ //如果文件存在
    return 0;
  }
  else{
    return -1;
  }
}




int read_m_send_para()
{

}



 
int getid()
{
  // 获取当前进程的PID
  pid_t pid = getpid();
  printf("pid = %d\n", pid);
  // 获取当前进程的PPID
  pid_t ppid = getppid();
  printf("ppid = %d\n", ppid);
  return 0;
}






extern int add_file_list(char * fileurl);
















int myscandir(char *basePath,int isrecursion,void (*callbackfun)(int,char*))
{
      char base[1000];
    char file_url[1000];
  struct dirent **namelist;
  int n;
  int ic=0;
  n = scandir(basePath,&namelist,0,alphasort);
  if(n < 0)
  { 
      printf("scandir return \n");
  }
  else
  {
      int index=0;
      while(index < n)
      {
          //printf("d_ino：%ld  d_off:%ld d_name: %s\n", namelist[index]->d_ino,namelist[index]->d_off,namelist[index]->d_name);
          snprintf(file_url,1000,"%s/%s",basePath,namelist[index]->d_name);
          (*callbackfun)(ic,file_url);
          ic++;
          free(namelist[index]);
          index++;
      }
      free(namelist);
  }
  return 0;
}





int readFileList(char *basePath,int isrecursion,void (*callbackfun)(int,char*))
{
    DIR *dir;
    struct dirent *ptr;
    char base[1000];
    char file_url[1000];
    if ((dir=opendir(basePath)) == NULL)
    {
        perror("Open dir error...");
        exit(1);
    }
    int ic=0;
    while ((ptr=readdir(dir)) != NULL)
    {
      if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
          continue;
      else if(ptr->d_type == 8){///file
          ic++;    
          //printf("d_name:%s/%s\n",basePath,ptr->d_name);
          snprintf(file_url,1000,"%s/%s",basePath,ptr->d_name);
          (*callbackfun)(ic,file_url);
      }
      else if(ptr->d_type == 10){///link file
      }    
      else if((ptr->d_type == 4)&&isrecursion){//dir
            memset(base,'\0',sizeof(base));
            strcpy(base,basePath);
            strcat(base,"/");
            strcat(base,ptr->d_name);
            readFileList(base,isrecursion,callbackfun);//递归调用
        }
    }
    closedir(dir);
    return 1;
}

int listfile_callback(int type,char * url)
{
  ;
}





int listfiles(char *basePath,int isrecursion)
{
    DIR *dir;
    void (*callbackfunp)(int,char*)=NULL;
    callbackfunp=&listfile_callback;
    ///get the current absoulte path
    //memset(basePath,'\0',sizeof(basePath));
    //getcwd(basePath, 999);
    printf("the current dir is : %s\n",basePath);
    //readFileList(basePath,isrecursion,callbackfunp);
    myscandir(basePath,isrecursion,callbackfunp);
    return 0;
}



