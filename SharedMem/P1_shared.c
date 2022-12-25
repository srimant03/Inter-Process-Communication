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
#include <sys/shm.h>

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
    //shared memory
    int shmid;
    key_t key;
    char *shm;

    key=1234;
    shmid = shmget(key,1024,0644|IPC_CREAT);

    shm = shmat(shmid,NULL,0);
    char*s=(char *) shm;
    *s='\0';
    int initial=0;
    int final=initial+5;
    double time_ack=0;
    while(final<=50){
        printf("Sending 5 strings...");
        printf("\n");
        for (int i=initial;i<final;i++){
            for (int j=0;j<5;j++){
                *s=array[i][j];
                s++;
            }
            sleep(1);
        }
        struct timespec start, end;
        clock_gettime(CLOCK_REALTIME, &start);
        
        //read acknowledgement from P2
        int shmid2;
        key_t key2;
        char *shm2;

        key2=1235;
        shmid2 = shmget(key2,1024,0644|IPC_CREAT);

        shm2 = shmat(shmid2,NULL,0);
        char *s2=(char *) shm2;
        *s2='\0';
        clock_gettime(CLOCK_REALTIME, &end);
        double time_taken;
        time_taken = (end.tv_sec - start.tv_sec) * 1e9;
        time_taken = (time_taken + (end.tv_nsec - start.tv_nsec)) * 1e-9;
        time_ack=time_ack+time_taken;
        printf("Highest ID Received: %d",final);
        printf("\n");
        initial=final;
        final=initial+5;
    }
    printf("Total time taken for acknowledgement: %f",time_ack);
    //detach from shared memory
    shmdt(shm);
    //shmctl(shmid,IPC_RMID,NULL);
}