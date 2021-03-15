#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define clear() printf("\033[H\033[J") 
#define MAX 5
char *q[MAX];
int total_commands=0;
char q1[64]="";
char q2[64];
char q3[64];
char q4[64];
char q5[64];
//strcpy(q[0],q1);
/*qarray[1]=q2;
qarray[2]=q3;
qarray[3]=q4;
qarray[4]=q5;*/
int front=0;
int rear=-1;
int count_q=0;
/*bool isFull(){
  //queue is full then yes
  return count_q==MAX;
}
bool isEmpty(){
  //queue is empty then yes
  return count_q==0;
}
void delete(){
  //deletes element if queue is full
   char data = qarray[front++];
	
   if(front == MAX) {
      front = 0;
   }
	
   count_q--;
    
}*/
void insert(char dat[]) {
  if (total_commands%5==0){
    strcpy(q1,dat);
    q[0]=q1;
    front=1;
  }
   else if (total_commands%5==1){
    strcpy(q2,dat);
    q[1]=q2;
    front=2;
  }
  else if (total_commands%5==2){
    strcpy(q3,dat);
    q[2]=q3;
    front=3;
  }
  else if (total_commands%5==3){
    strcpy(q4,dat);
    q[3]=q4;
    front=4;
  }
  else {
    strcpy(q5,dat);
    q[4]=q5;
    front=0;
  }
    /*printf("%s\n",data );
   //it inserts queue if queue is full then deletes the first added element and adds it;
   if(!isFull()) {
	
      if(rear == MAX-1) {
         rear = -1;            
      }       
      
      qarray[++rear] = data;
      printf("%s\n",qarray[rear] );
      count_q++;
   }
   else{
		 delete();
		 insert( data);
	 }*/
   count_q++;
}

char *read_line() {
  char *line = NULL;
  ssize_t bufsize = 0;
  getline(&line, &bufsize, stdin);
  return line;
}

char **split_line(char *line) {
    int i=0,j=128; 
    char **temp = malloc(j);  
    char *temp2 = strtok(line," \n"); 
    while(temp2!=NULL) { 
        temp[i] = temp2;
        i++; 
        if(i>=128) {
          j=j+128;
          temp = realloc(temp,j);
        }
        temp2 = strtok(NULL,"\n"); 
    } 
    temp[i]=NULL;   
    return temp;
} 

char **split_l(char *line) {
    int i=0,j=128; 
    char **temp = malloc(j);  
    char *temp2 = strtok(line,"\"\n"); 
    while(temp2!=NULL) { 
        temp[i] = temp2;
        i++; 
        if(i>=128) {
          j=j+128;
          temp = realloc(temp,j);
        }
        temp2 = strtok(NULL,"\"\n"); 
    } 
    temp[i]=NULL;   
    return temp;
}

int main(int argc, char **argv) {
  clear();
  char *line;
  char **args;
  int status;

  do
  {
    char cwd[1024]; 
    getcwd(cwd, sizeof(cwd)); 
    printf("MTL458:%s", cwd);
    printf("$");
    char s1[]="";
    line = read_line();
    
    args = split_line(line);
    char concate_line[1024]="";
    int i=0;
    while(args[i]!=NULL){
      strcat(concate_line,args[i]);
      i++;
    }  

    if (args[0] == NULL) {
      status = 1; // An empty command was entered.
    }
    else if(strcmp(args[0], "cd")==0){
      
      strcat(s1,args[0]);
      if (args[1] == NULL) {
      fprintf(stderr, "expected argument to \"cd\"\n");
      }
			else {
        strcat(s1," ");
        strcat(s1,args[1]);
        char **token=split_l(args[1]);
				char s2[]="";
				int i=0;
				while(token[i]!=NULL){
					strcat(s2,token[i]);
					i++;
				}
        if (chdir(s2) != 0) {
          perror("ERROR: ");
        }
      }
      insert(concate_line);
      total_commands++;
      status = 1;
    }
    else if(strcmp(args[0],"^C")==0){
      clear();
      sleep(1);
      status=0;
    }
    else if(strcmp(args[0], "exit")==0){
      status = 0;
    }
    else if(strcmp(args[0],"history")==0){
      insert("history");
      
      if(count_q>=5){
        for(int i=0;i<5;i++){
          printf("%s \n",q[(front+i)%5]);
        }
      }
      else{
        printf("only %i commands in history \n",count_q);
        for(int i=0;i<count_q;i++){
          printf("%s \n",q[i]);
        }
      }
      if(total_commands%5==0){
        
      }
      total_commands++;
      status=1;
    }
    else{
      pid_t pid,wpid;
      int stat;
      pid = fork();
      if(pid==0){
        if(execvp(args[0],args)== -1){
          perror("ERROR :");
        }
        exit(EXIT_FAILURE);
      }
      else if(pid<0){
        perror("error :");
      }
      else{
        do{
          char s1[]="";
          strcat(s1,args[0]);
          if (args[1]!=NULL){
            strcat(s1," ");
            strcat(s1,args[1]);
          }
          insert(concate_line);
          total_commands++;
          wpid=waitpid(pid,&stat,WUNTRACED);
        }while(!(WIFEXITED(stat)) && !(WIFSIGNALED(stat)));
      }
      status =1;
    }
    
    //printf("%s\n", qarray[rear]);
  }while(status);
}
