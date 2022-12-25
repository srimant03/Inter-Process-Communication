#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
//#include <ipc.h>
#include <sys/types.h>
//#include <sys/wait.h>
//#include <semaphore.h>
//#include <pthread.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/socket.h>

int main(){
    int fd1;
    char *socket_path="socket";
    char arr1[50];
    int index[5];
    struct sockaddr_un addr1;
    fd1=socket(AF_UNIX,SOCK_STREAM,0);
    if (fd1==-1){
        printf("Error creating socket");
        exit(1);
    }
    else{
        memset(&addr1,0,sizeof(addr1));
        addr1.sun_family=AF_UNIX;
        if (socket_path=='\0'){
            *addr1.sun_path='\0';
            strncpy(addr1.sun_path+1,socket_path+1,sizeof(addr1.sun_path)-2);
        }
        else{
            strncpy(addr1.sun_path,socket_path,sizeof(addr1.sun_path)-1);
            unlink(socket_path);
        }
        
        int x=bind(fd1,(struct sockaddr*)&addr1,sizeof(addr1));
        if(x==-1){
            printf("Error binding");
            exit(1);
        }

        int y=listen(fd1,5);
        if (y==-1){
            printf("Error listening");
            exit(1);
        }
        int k=0;
        int l=0;
        fd1=accept(fd1,NULL,NULL);
        if (fd1==-1){
            printf("Error accepting");
            exit(1);
        }
        while(l<10){
            for(int i=0;i<5;i++){
                sleep(2);
                int ret=read(fd1,&arr1,5);
                if (ret==-1){
                    printf("Error reading");
                    exit(1);
                }
                printf("Strings received: ");
                printf("%s",arr1);
                printf("\n");
                printf("ID received: %d",k+i+1);
                printf("\n");
            }
            //printf("done");
            printf("\n");
            k=k+5;

            index[0]=k;
            int ret1=write(fd1,&index,5);
            if (ret1==-1){
                printf("Error writing");
                exit(1);
            }
            //printf("%c",index[0]);

            l++;
        }
        close(fd1);
    }
    return 0;
}