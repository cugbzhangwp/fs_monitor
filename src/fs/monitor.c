#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <linux/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <linux/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdlib.h>   
//#include <net/if.h>
#include <sys/types.h>  
#include <sys/socket.h>  
#include <asm/types.h>  
#include <linux/netlink.h>  
#include <linux/rtnetlink.h>
#include <stdlib.h>  
#include <stdio.h>  
#include <sys/ioctl.h>  
#include <linux/if.h>  
#include <string.h>
#include <dirent.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <linux/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <math.h>
#include <time.h>
#include <sys/inotify.h>//inotify_init inotify_add_watch....
#include <stdio.h>//printf
#include <string.h> //strcmp
#include <sys/inotify.h>//inotify_init inotify_add_watch....
#include <sys/select.h>//select timeval
#include <unistd.h>//close

#define BUF_LEN 512


#include <main.h>
#include <common.h>
#include <printcolor.h>


#define LOCAT() printf(GREEN"file:%s,function：%s,line:%d\n"NONE,__FILE__,__FUNCTION__,__LINE__)
#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )
#define ERR_EXIT(msg,flag)  {perror(msg);goto flag;}

typedef struct _UPDATE_HEAD
{
int pak_type;
int cmd;
int nextreadlen;
char symbol[8];
char checksum[2];
char md5[64];
char basepath[64];
char updatecmd[64];
char unuse[298];
}__attribute__ ((packed,aligned(1)))UPDATE_HEAD_S,*UPDATE_HEAD_T;

char CK[2] = {0,0};//校验码
void check_sum_B(char * Buffer,int N)
{

    int I=0;
    for(I=0;I<N;I++)
    {
    CK[0] = CK[0]  + Buffer[I];
    CK[1]  = CK[1]  + CK[0] ;
    }
    //Buffer[N]=CK_A;
    //Buffer[N+1]=CK_B;
}


inline int dochecksum(FILE * fp,int update_file_size)
{
    char readfilebuf[512];
    printf(GREEN"in dochecksum\n");
    CK[0]=CK[1]=0;
    int i_read,i_write;
    int readcount=0;
    FILE * fp_tmp;
    if((fp_tmp=fopen("/home/pi/update/updatetmp.tar.gz","wb"))!=NULL){
        
            while(!(readcount>=update_file_size)){
            i_read=fread(readfilebuf,1,512,fp);
            printf("i_read=%d\n",i_read);

            check_sum_B(readfilebuf,i_read);
            i_write=fwrite(readfilebuf,1,i_read,fp_tmp);
            readcount+=i_read;
            }
        }
    fclose(fp_tmp);
    return 0;
}


int doupdatebin()
{

    LOCAT();
    if(try_system("mv /home/pi/update/updatetmp.tar.gz /home/pi/update/update.tar.gz")==0){
        printf("do system success!\n");
        return 0;
        //try_system("mv /home/pi/update/updatetmp.tar.gz /home/pi/update/update.tar.gz");
    }
    else{
        printf("do system fail!\n");
        return -1;
    }
    if(try_system("dotarupdate")==0){
        printf("do system success!\n");
        //system("mv /home/pi/update/updatetmp.tar.gz /home/pi/update/update.tar.gz");
        //try_system("dotarupdate");
        return 0;
    }
    else{
        printf("do system fail!\n");
        return -2;
    }

    
    

}




















int update_anyname(char * file_url)//
{
    FILE * fp_test;
    UPDATE_HEAD_S abl_update_head;
    char checknum[2];
    char readbuf[512];
    char cmdstr[100];
    int i_read;
    UPDATE_HEAD_S head;
    int update_file_size=file_size(file_url);
    if((fp_test=fopen(file_url,"rb"))!=NULL){
        i_read=fread(&abl_update_head,1,sizeof(UPDATE_HEAD_S),fp_test);
        printf(BLUE"abl_update_head.symbol=%s\n",abl_update_head.symbol);
        printf(BLUE"abl_update_head.pak_type=%d\n",abl_update_head.pak_type);
        printf(BLUE"abl_update_head.cmd=%d\n",abl_update_head.cmd);
        printf(BLUE"update_file_size=%d\n",update_file_size);
        printf(BLUE"abl_update_head.nextreadlen=%d\n",abl_update_head.nextreadlen);//(strstr(abl_update_head.symbol,"abl-up")!=NULL)
        if((abl_update_head.pak_type==0)&&(abl_update_head.cmd==65540)&&(abl_update_head.nextreadlen==(update_file_size-sizeof(UPDATE_HEAD_S)))&&(strstr(abl_update_head.symbol,"abl-up")!=NULL)){
            dochecksum(fp_test,abl_update_head.nextreadlen);        //TODO DO UPDATE
            printf("real send filesize=CK_A=%X CK_B=%X\n",(unsigned char)CK[0],(unsigned char)CK[1]);
            if((abl_update_head.checksum[0]==CK[0])&&(abl_update_head.checksum[1]==CK[1])){
                printf(GREEN"CHECKSUM OKKKKKKKKKKKKKKKKKK\n");
                memset(cmdstr,0,100);
                snprintf(cmdstr,100,"rm %s",file_url);//\"DO\'T have SD!\"|/telluser
                system(cmdstr);//rm the orig file
                doupdatebin();//
                }
            exit(0);
            }
        printf(GREEN"IN UPDATE_ANYNAME FUNCTION\n");
        }
    fclose(fp_test);

}


int cp_update_file(char * sourceurl)
{
    LOCAT();
    char cmd[1000];
    char * dest="/home/pi/";
    snprintf(cmd,900,"cp %s %s",sourceurl,dest);
    printf("cmd=%s\n",cmd);
    if(try_system(cmd)==0){
        printf("do system success!\n");
        return 0;
    }
    else{
        printf("do system fail!\n");
        return -1;
    }
    return -100;
}


void callbackframe(char *fileurl)
{
   LOCAT();
  if((strstr(fileurl,"update.bin")!=NULL)&&1/*(strstr(fileurl,".iws")!=NULL)*/){
   update_anyname(fileurl);
    //add_file_list(fileurl);
    //cp_update_file(fileurl);

  }

}












int fs_monitor(APP_S * app)
{
    for(;;){
        filenevt(app,"/home/pi/userdata/data"); //监测文件夹变化 
    }
    return 0;
    
}





int filenevt(APP_S * app,char * path) //监测文件夹变化
{
    int length, i = 0;
    int fd;
    int wd;
    char buffer[BUF_LEN];
    char url[200];

    if((fd = inotify_init()) < 0)
        ERR_EXIT("inotify_init",inotify_init_err);

    if( (wd = inotify_add_watch( fd, path,   IN_CREATE | IN_DELETE ) ) < 0)
        ERR_EXIT("inofity_add_watch", inotify_add_watch_err);
    
    fd_set rfd;
    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 10000;//10millsecond
    while(1)
    {
        int retval;
        FD_ZERO(&rfd);
        FD_SET(fd, &rfd);
        retval = select(fd + 1, &rfd, NULL, NULL, &tv);
        if(retval == 0) continue;
        else if(retval == -1)
            ERR_EXIT("select",select_err);

        // retval > 0
        length = read( fd, buffer, BUF_LEN );  
        if(length < 0)
            ERR_EXIT("read",read_err);

        //length >= 0
        int i = 0;
        while ( i < length ) 
        {
            struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];
            if ( event->len ) 
            {
                if ( event->mask & IN_CREATE ) 
                {
                    if ( event->mask & IN_ISDIR ) {
                        printf( "The directory %s was created.\n", event->name );       
                    }
                    else{
                        snprintf(url,200,"%s/%s",path,event->name);
                        //snprintf(file_url,1000,"%s/%s",basePath,namelist[index]->d_name);
                        // add_file_list(url);
                        // printf_file_list();
                        LOCAT();
                        sleep(5);
                        callbackframe(url);

                        printf( "zwp The file %s was created.\n", event->name );
                    }
                    if(strcmp(event->name,"kill") == 0){
                        ERR_EXIT("success exit",success_exit);
                    }

                }
                else if ( event->mask & IN_DELETE ) 
                {
                    if ( event->mask & IN_ISDIR ) {
                        printf( "The directory %s was deleted.\n", event->name );       
                    }
                    else{
                        // del_file_list(url);
                        // printf_file_list();
                        printf( "The file %s was deleted.\n", event->name );
                    }
                }
                else if ( event->mask & IN_MODIFY ) 
                {
                    if ( event->mask & IN_ISDIR ){
                        printf( "The directory %s was modified.\n", event->name );
                    }
                    else{
                        printf( "The file %s was modified.\n", event->name );
                    }
                }
            }else
            {
                //TODO
                //when only a file(not directory) is specified by add watch function, event->len's value may be zero, we can handle it here
            }
            i += EVENT_SIZE + event->len;
        }
    }
success_exit:
    ( void ) inotify_rm_watch( fd, wd );
    ( void ) close( fd );
    return 0;

read_err:
select_err:
inotify_add_watch_err:
    ( void ) inotify_rm_watch( fd, wd );
inotify_init_err:
    ( void ) close( fd );

    return -1;
}