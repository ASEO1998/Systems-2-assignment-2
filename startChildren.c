/*
 *Compile with:
 *$ g++ startChildren.c -o startChildren
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <sys/types.h>
#include <signal.h>

#define CHILD_PROGRAM_NAME "signalThenStop"

const int NUM_CHILDREN = 4;

const int NUM_STR_LEN = 16;

int numFinishedChildren = 0;

pid_t childPidArray[NUM_CHILDREN];

void sigUsr1Handler (int  sigNum,
		   siginfo_t*  infoPtr,
		   void* dataPtr
		   )
{
  int i;
  
  // YOUR CODE HERE
  
  
  for (i = 0; i < NUM_CHILDREN; i++){
    
    if (childPidArray[i] ==  infoPtr -> si_pid ){
      printf("Child %d has signaled that its about to stop\n",i);
    }
  }
}


void sigChildHandler (int sigNum )
{
  int status;
  pid_t pid;
  int i;

  //  pid=waitpid(-1,&status,WNOHANG);
  //    for (i = 0; i < NUM_CHILDREN; i++){

      //      if(childPidArray[i] == pid){
	//        printf("Child %d has stopped. \n", i);
	//      numFinishedChildren ++;
      //    }
      //}
  
 
    while( (pid=waitpid(-1,&status,WNOHANG)) > 0){
      if (WIFEXITED(status) != 0){
      
       for (i = 0; i < NUM_CHILDREN; i++){

    	if(childPidArray[i] == pid){
    	  printf("Child %d has stopped. \n", i);
    	  numFinishedChildren ++;
    	}
       }
     }
    
    else{
     printf("%d crashed!\n",pid);
      for (i = 0; i < NUM_CHILDREN; i++){

    	if(childPidArray[i] == pid){
    	  printf("Child %d has stopped. \n", i);
    	  numFinishedChildren ++;
    	}
     }
     }
     }
  // YOUR CODE HERE
}


int main ()
{
  //  I.  Application validity check:

  //  II.  Do main work:
  //  II.A.  Reset random number generator:
  srand(getpid());

  //  II.B.  Install signal handlers:
  struct sigaction act0;
  struct sigaction act1;

  // YOUR CODE HERE
  act0.sa_sigaction = sigUsr1Handler;
  act1.sa_handler = sigChildHandler;
  
  act0.sa_flags = SA_SIGINFO | SA_RESTART ;
  act1.sa_flags = SA_NOCLDSTOP | SA_RESTART;
  
  sigaction(SIGUSR1,&act0,NULL);
  sigaction(SIGCHLD,&act1,NULL);
  

  //  II.C.  Start child processes:
  int index;
  char numStr[NUM_STR_LEN];

  for  (index = 0;  index < NUM_CHILDREN;  index++)
    {
      snprintf(numStr,NUM_STR_LEN,"%d",20 + rand() % 40);

      printf("Child %d: %s seconds.\n",index,numStr);

      // YOUR CODE HERE
      
      
      if ( fork() == 0)
	{
	  
	  childPidArray[index] = getpid();
	  //execl("./signalThenStop",CHILD_PROGRAM_NAME,numStr,NULL);
	  execl(CHILD_PROGRAM_NAME,CHILD_PROGRAM_NAME,numStr,NULL);
	  exit(0);
	}

      else{
	//printf("Parent pid of %d is in index number %d\n",getpid(),index );
	childPidArray[index] = getpid() + index + 1;
      }
      
    }

  

  //  II.D.  Wait for children to finish:
  int timeCount = 0;

  while  (numFinishedChildren < NUM_CHILDREN)
    {
      sleep(1);
      timeCount++;
      printf("%d\n",timeCount);
    }

  //  III.  Finished:
  return(EXIT_SUCCESS);
}
