#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>

// global vars (reachable by all threads)
pthread_mutex_t locky;
pthread_mutex_t lockx;
pthread_mutex_t lockz;
int readcount;
int linenum; // line number
int sum; // sum of x,y,z
int fd;
char * myfifo; // pipe path


//int str cevrim
char *intstr(int number)
{
    int count=0;
    int numbertmp=number;
    while(numbertmp>0)
    {
        int digit= numbertmp%10;
        numbertmp/=10;
        count++;
    }
    char * value= (char * ) calloc (count,sizeof(unsigned char)  );
    numbertmp=number;
    count--;
    while(numbertmp>0)
    {
        int digit= numbertmp%10;
        numbertmp/=10;
        char x=(char) 48+digit;
        *(value+count)=x;
        count--;
    }

    return value;
}


int readNumber(char  *selectedChar,int linenum){ // write x/y/z and line number to pipe then read the nubmer in that line
    char *character=(char*)calloc(10, sizeof(char));
 //char *sstring=(char*)calloc(10, sizeof(char)); // char line number
    char *sstring="";
    char *srecievedNumber=(char*)calloc(10, sizeof(char));
    int recievedNumber=0;
    character=selectedChar;

    char * linec=intstr(linenum);
if((sstring = malloc(strlen(character)+strlen("-")+1+strlen(linec)+1)) != NULL){
    sstring[0] = '\0';   // ensures the memory is an empty string
    strcat(sstring,character);
    strcat(sstring,"-");
    strcat(sstring,linec);
} else {
    printf("concat failed!\n");
    // exit(0);
}

    printf("Send %s\n",sstring );


        fd = open(myfifo, O_WRONLY);//this side : write only
        write(fd, sstring, sizeof(sstring));
        close(fd);
        //sleep(2);
        fd = open(myfifo, O_RDONLY); // this side read only
        read(fd, srecievedNumber, sizeof(srecievedNumber));
        close(fd);
        if(strcmp(srecievedNumber,"END")==0){ // if end of file: end the program
          printf("File end has reached!\nLast sum= %d\n",sum);
          printf("END\n");
          exit(0);
        }
        recievedNumber= atoi (srecievedNumber);
        printf("Recieved Number: %d\n", recievedNumber);
        sum+=recievedNumber;
 //linenum++;

}
void *readx() //$x thread
{
  pthread_mutex_lock(&lockx);// lock xthread so no one can reach critical section
  readNumber("x",linenum); // read that line from file by named pipe (x=val)
  linenum=linenum+1;// go to the next line
  pthread_mutex_unlock(&locky);

}
void *ready()//$y thread
{
  pthread_mutex_lock(&locky);
  readNumber("y",linenum);
  linenum=linenum+1;
  pthread_mutex_unlock(&lockz);
}

void *readz()//$z thread
{
  pthread_mutex_lock(&lockz);
  readNumber("z",linenum);
  linenum=linenum+1;
  printf("x + y + z = %d\n",sum);
  sum=0;
  pthread_mutex_unlock(&lockx);
}



int main(){
  //initialize mutex locks:
  pthread_mutex_init(&locky, NULL);
  pthread_mutex_init(&lockz, NULL);
  pthread_mutex_init(&lockx, NULL);
  pthread_mutex_lock(&locky); // lock y
  pthread_mutex_lock(&lockz); //lock z (start with x)
  //define threads
  pthread_t xthread;
  pthread_t zthread;
  pthread_t ythread;
  void* status;
  char sreadcount[80]=""; // read counter (string from user)
  printf("Enter read counter (must be a multiple of 3): ");
  do{
  getcount: fgets(sreadcount, 80, stdin);
  readcount=atoi(sreadcount);
  }while(readcount%3!=0 ); // reinter if it is not a mult of 3
  printf("\n");

  linenum=1;// start from 1st line
  sum=0;
  //_______ define pipe________
  myfifo = "/tmp/myfifo";
  mkfifo(myfifo, 0666);
  //loop as read counter
  //must be less or equal to file lines, if not exit without error.
  for (int i=0;i< readcount/3;i++){
  // create threads and call their functions
  pthread_create(&xthread,NULL,readx,NULL);
  pthread_create(&zthread,NULL,readz,NULL);
  pthread_create(&ythread,NULL,ready,NULL);
  // join (wait) threads
  pthread_join(xthread,&status);
  pthread_join(zthread,&status);
  pthread_join(ythread,&status);
  }
  // destroy mutex locks
  pthread_mutex_destroy(&locky); 
  pthread_mutex_destroy(&lockz); 
  pthread_mutex_destroy(&lockx); 
  return 0;
}

