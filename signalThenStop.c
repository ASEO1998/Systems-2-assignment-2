#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<signal.h>

const int NUM_CLOSE_DOWN_SECS = 10;

int main (int argc,
	char* argv[]
	)
{
  // YOUR CODE HERE
  if(argv[1] == NULL){
    exit(EXIT_FAILURE);
  }
  int timeStamp;
  
  sscanf(argv[1], "%d", &timeStamp);
  sleep(timeStamp);
  kill(getppid(),SIGUSR1);
  sleep(NUM_CLOSE_DOWN_SECS);
  
  kill(getppid(),SIGCHLD);
  
  return(EXIT_SUCCESS);
}
