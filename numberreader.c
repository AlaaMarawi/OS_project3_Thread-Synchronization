
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


//str int cevrim
int strint( char* charnums)
{
   int number=0;
   int index=0;
   while(charnums[index])
   {
    if(('0'<=charnums[index]) && (charnums[index]<='9'))
    {

        if(!number)
            number= ( (int) charnums[index]) - 48;
        else
        {
            number = (number *= 10) + ((int) charnums[index] - 48);
        }
        index++;
    }
    else
    {
        number=-1;
        printf("\nGecersiz islem");
        break;
    }
}
return number;
}



int main(void)
{
    printf("** numberreader.c\n");
    //________file________
    FILE * fp;
    char * line = NULL; // read value of the line
    size_t len = 0;
    ssize_t fread;
    //__________pipe________
    int fd1;
    char * myfifo = "/tmp/myfifo"; //pipe path
    mkfifo(myfifo, 0666); // make pipe

    char srecievedVal[80]; //string recieved value from pipe
    char *character=(char*)calloc(10, sizeof(char)); //revieved character x,y,z
    char *charLinenum=(char*)calloc(80, sizeof(char)); // line number as string
    int linenum=1; // line number
    while(1){
        //sleep(2); 
        line=NULL; // free line to be ready to recieve new value
        fd1 = open(myfifo,O_RDONLY); // open the pipe as read only side
        read(fd1, srecievedVal, 80); // read from pipe to srecievedVal
        close(fd1);
        printf("recieved: |%s|\n", srecievedVal);
        character = strtok(srecievedVal, "-"); 
        printf("recieved character:%s\n",character );
        charLinenum=(strtok(NULL, " "));
        //printf("recieved line:%s\n",charLinenum );
        //charLinenum=srecievedVal[1];
        //strcpy(charLinenum,srecievedVal[1]);
        //printf("char line %c|\n",charLinenum );
        //linenum=strint(charLinenum);
        linenum= atoi(charLinenum);
        printf("recieved line: %d\n", linenum);


    // open and read file :
        if (linenum==0) break;
        fp = fopen("numbers.txt", "r");
        printf("File opened\n");
        if (fp == NULL){
            printf("file is empty\n");
            exit(EXIT_FAILURE);
        }

    // read specific line :
        //linenum=1;
        for(int i=0;i<linenum;i++){
            if((fread = getline(&line, &len, fp)) != -1){
            //printf("line:%s\n", line); //line value
            }else{
                printf("File End Reached!\nReturn value E\n");
                line="END";
                break;
            }
        }
        fclose(fp);
        printf("%d. %s = %s\n",linenum,character,line);
        //write line value to pipe
        fd1 = open(myfifo,O_WRONLY);
        write(fd1, line, strlen(line)+1);
        close(fd1);



    //if (line)
        //free(line);
    //printf("line free %s\n",line); //line empty
        printf("------------------------\n");
    }
    exit(EXIT_SUCCESS);
    return 0;
}



