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


#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )
#define ERR_EXIT(msg,flag)  {perror(msg);goto flag;}



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
                        // add_file_list(url);
                        // printf_file_list();
                        printf( "The file %s was created.\n", event->name );
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