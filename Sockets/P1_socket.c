#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
//#include <ipc.h>
#include <sys/types.h>
//#include <sys/wait.h>
//#include <semaphore.h>
//#include <pthread.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <time.h>

int main(){
    srand(time(0));
    int size=50;
    char array[50][5];
    for (int i=0;i<50;i++){
        //generate a random string of length 5
        for (int j=0;j<5;j++){
            //randome character from a to z
            array[i][j] = (rand() % 26) + 97;
        }
        array[i][5] = '\0';
    }

    for (int i=0;i<50;i++){
        printf("%d ",i+1);
        for (int j=0;j<5;j++){
            printf("%c",array[i][j]);
        }
        printf("\n");
    }
    //create a socket
    int fd;
    char *socket_path = "socket";
    int read_index[5];
    struct sockaddr_un addr;
    fd=socket(AF_UNIX,SOCK_STREAM,0);
    if (fd==-1){
        printf("Error creating socket");
        exit(1);
    }
    else{
        memset(&addr,0,sizeof(addr));
        addr.sun_family=AF_UNIX;
        if(*socket_path=='\0'){
            *addr.sun_path='\0';
            strncpy(addr.sun_path+1,socket_path+1,sizeof(addr.sun_path)-2);
        }
        else{
            strncpy(addr.sun_path,socket_path,sizeof(addr.sun_path)-1);
            //unlink(socket_path);
        }
        int x=connect(fd,(struct sockaddr*)&addr,sizeof(addr));
        if (x==-1){
            printf("Error connecting");
            exit(1);
        }
        else{
            int initial=0;
            int final=initial+5;
            double time_acknowledge=0;
            while(1){
                printf("Sending 5 strings...\n");
                for(int i=initial;i<final;i++){
                    int ret=write(fd,array[i],5);
                    sleep(1);
                    if(ret==-1){
                        printf("Error writing to file");
                        exit(1);
                    }
                }
                struct timespec start, finish;
                clock_gettime(CLOCK_REALTIME, &start);

                int ret1=read(fd,read_index,5);
                clock_gettime(CLOCK_REALTIME, &finish);
                double elapsed = (finish.tv_sec - start.tv_sec);
                elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
                time_acknowledge+=elapsed;

                if (ret1==-1){
                    printf("Error reading from file");
                    exit(1);
                }
                printf("Highest ID received is %d",read_index[0]);
                printf("\n");
                int x=(read_index[0]);
                if (x==50){
                    break;
                }
                else{
                    initial=final;
                    final=initial+5;
                }
            }
            printf("Time taken to receive acknowledgement is %f",time_acknowledge);
        }
    }
    return 0;
}