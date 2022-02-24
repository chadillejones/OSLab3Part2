#include  <stdio.h>
#include  <stdlib.h>
#include  <sys/types.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>


int randomNum(int range);
void  PoorStudentProcess(int []);
void  OldDadProcess(int []);

int  main(int  argc, char *argv[])
{
  srand(time(NULL));
     int    ShmID;
     int    *ShmPTR;
     pid_t  pid;
     int    status;

//      if (argc != 5) {
//           printf("Use: %s #1 #2 #3 #4\n", argv[0]);
//           exit(1);
//      }

     ShmID = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0666);
     if (ShmID < 0) {
          printf("*** shmget error (server) ***\n");
          exit(1);
     }
     //printf("Server has received a shared memory of four integers...\n");

     ShmPTR = (int *) shmat(ShmID, NULL, 0);
     if (*ShmPTR == -1) {
          printf("*** shmat error (server) ***\n");
          exit(1);
     }
     printf("Server has attached the shared memory...\n");

     ShmPTR[0] = 0;
     ShmPTR[1] = 0;
     printf("Server is about to fork a child process...\n");
     pid = fork();
     
     if (pid < 0) {
          printf("*** fork error (server) ***\n");
          exit(1);
     }
     else if (pid == 0) {
       //Child Process
       
       for(int x = 0; x<25; x++){
         sleep(randomNum(6));
         PoorStudentProcess(ShmPTR);

       }
     }
  
     // Parent Process
     
     for(int i = 0; i<25; i++){  
       sleep(randomNum(6));
       OldDadProcess(ShmPTR);       
     }
}

int randomNum(int range){
    int num;
    num = rand() % range;
    return num;
}

void  PoorStudentProcess(int  SharedMem[]) {
     int account = SharedMem[0];
     while(SharedMem[1] != 1 ) {
       ;
     }   
     int balance = randomNum(51);
     printf("Poor Student needs $%d\n", balance);
     if(balance <= account) {
       account = account - balance;
       printf("Poor Student: Withdraws $%d / Balance = $%d\n", balance, account);
     }
     else {
       printf("Poor Student: Not Enough Cash ($%d)\n", account );
     }
     SharedMem[0] = account;
     SharedMem[1] = 0;
}

void  OldDadProcess(int  SharedMem[])
{
     int account = SharedMem[0];
     while(SharedMem[1] != 0 ) {
       ;
     }
     if(account <=100) {
       int balance = randomNum(101);
       if(balance % 2 == 0) {
         account = account + balance;
         printf("Dear old Dad: Deposits $%d / Balance = $%d\n", balance, account);
         
       }
       else{
         printf("Dear old Dad: Doesn't have any money to give. balance = %d \n", balance);
       }
       SharedMem[0] = account;
       SharedMem[1] = 1;
     }
     else {
       printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", account);
       SharedMem[0] = account;
       SharedMem[1] = 1;
     }
}