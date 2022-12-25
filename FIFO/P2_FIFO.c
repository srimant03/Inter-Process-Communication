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
//#include <sys/un.h>
#include <sys/stat.h>
#include <time.h>

int main(){
    
    mkfifo("fifo",0666);
    int fd;
    char arr1[50];
    int index[5];
    int k=0;
    int l=0;
    while(l<10){
        fd=open("fifo",O_RDONLY);
        if (fd==1){
            printf("Error opening file");
            exit(1);
        }
        else{
            for (int i=0;i<5;i++){
                sleep(2);
                int ret=read(fd,arr1,5);
                if (ret==-1){
                  printf("Error writing to file");
                  exit(1);
                }
                printf("Strings received: ");
                printf("%s",arr1);
                printf("\n");
                printf("ID Received: %d",k+i+1);
                printf("\n");
            }
          
            //printf("Print Done");
            printf("\n");
            k=k+5;
        }
        close(fd);

        index[0]=k;
        //printf("%d",index[0]);
        //printf("\n");
        fd=open("fifo",O_WRONLY);
        if (fd==-1){
            printf("Error opening file");
            exit(1);
        }
        else{
            int ret1=write(fd,index,5);
            if (ret1==-1){
                printf("Error writing to file");
                exit(1);
            }
            //printf("%d",index[0]);
        }
        close(fd);
        l=l+1;
    }
    return 0;
}