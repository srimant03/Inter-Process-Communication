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
    //create an array of 50 words
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
    //created a named pipe
    mkfifo("fifo",0666);
    int fd;
    char arr1[50];
    int read_index[5];
    int initial=0;
    int final=initial+5;
    int j=0;
    double time_acknowledge=0;
    while(j<10){
        fd=open("fifo",O_WRONLY);
        if (fd==-1){
            printf("Error opening file");
            exit(1);
        }
        else{
            printf("Sending 5 strings...\n");
            for (int i=initial;i<final;i++){
                int ret=write(fd,array[i],5);
                if (ret==-1){
                  printf("Error writing to file");
                  exit(1);
                }
                sleep(1);
            }
        }
        close(fd);
        struct timespec start, end;
        clock_gettime(CLOCK_REALTIME, &start);

        fd=open("fifo",O_RDONLY);
        if (fd==-1){
            printf("Error opening file");
            exit(1);
        }
        else{
            int ret1=read(fd,read_index,5);
            clock_gettime(CLOCK_REALTIME, &end);
            double elapsed = (end.tv_sec - start.tv_sec);
            elapsed += (end.tv_nsec - start.tv_nsec) / 1000000000.0;
            time_acknowledge=time_acknowledge+elapsed;
            
            if (ret1==-1){
                printf("Error writing to file");
                exit(1);
            }
            //printf("%c",read_index[0]);
            //int indx=atoi(read_index);
            printf("Highest ID sent back is %d", read_index[0]);
            printf("\n");
            printf("\n");
        }
        close(fd);

        j=j+1;
        /*if (strcmp(read_index,"50")==0){
            break;
        }*/
        
        initial=final;
        final=initial+5;
    }
    printf("Total time to acknowledge is %f",time_acknowledge);
}