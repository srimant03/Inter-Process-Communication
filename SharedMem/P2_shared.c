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
    //read from shared memory
    int shmid;
    key_t key;
    char *shm;
    
    key=1234;
    shmid = shmget(key,1024,0644|IPC_CREAT);
    shm = shmat(shmid,NULL,0);

    char *s=(char *) shm;
    char array[50][5];

    int k=0;
    while(k<50){
        for (int i=0;i<5;i++){
            sleep(1);
            for (int j=0;j<5;j++){
                array[i][j]=*s;
                s++;
            }
        }
        for (int i=0;i<5;i++){
            printf("Strings Received: ");
            for (int j=0;j<5;j++){
                printf("%c",array[i][j]);
            }
            printf("\n");
            printf("ID Received: %d",k+i+1);
            printf("\n");
        }
        printf("\n");
        k=k+5;
    
        //shmdt(shm);
        //shmctl(shmid,IPC_RMID,NULL);

        //send acknoledgement using shared memory
        int shmid2;
        key_t key2;
        char *shm2;

        key2=1235;
        shmid2 = shmget(key2,1024,0644|IPC_CREAT);

        shm2 = shmat(shmid2,NULL,0);
        char *s2=(char *) shm2;
        *s2='\0';
    }
    shmdt(shm);
    //shmctl(shmid,IPC_RMID,NULL);

    return 0;
}