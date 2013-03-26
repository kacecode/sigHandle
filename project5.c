#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>


void parentAction( int childpid );
void childAction( );
void parentSigHandle( int sigid );
void childSigHandle( int sigid );

void main()
{
	//fork
	//figure out if parent or child
		//0 => childAction()
		//-1 => error
		//else => parentAction()
}

// parentAction tree
// takes pid of child for reference
// no conditions
void parentAction( int childpid )
{
}

// parentSigHandle
// bound to parentAction signal handling events
void parentSigHandle( int sigid )
{
  switch ( sigid )
  {
  case SIGUSR1:
    printf( "*** Parent SIGUSR1 handler - Received 'task started' signal from child ***\n\n" );
    break;
  case SIGUSR2:
    printf( "*** Parent SIGUSR2 handler - Received 'task completed' signal from child ***\n\n" );
    break;
  default:
    printf( "PARENT: Weird stuff in the pipe - received signal id %d\n", sigid );
  }
}

// ChildAction
// Handles procedural actions of a child process
void childAction( )
{
	struct sigaction actions;
	sigset_t set;
	
	// Setup signalling
	sigemptyset( &actions.sa_mask );
	actions.sa_flags = 0;
	actions.sa_handler = childSigHandle;
	
	sigaction( SIGUSR1, &actions, NULL);
	sigaction( SIGUSR2, &actions, NULL);
	// announce 
	printf( "CHILD %d: Running! Parent id is %d.\n\n", getpid(), getppid() ); 
    
	// suspend to wait for work start 
	sigfillset( &set ); 
	sigdelset( &set, SIGUSR1 ); 
	sigsuspend( &set ); 
    
	printf( "CHILD: Sending signal I am starting work\n\n" ); 
	sleep(3); 
	kill( getppid(), SIGUSR1 );
    
	printf( "Child does work...\n\n" );
	
	sigdelset( &set, SIGUSR2 ); 
	sigsuspend( &set ); 
	
	printf( "CHILD: Work is done.\n\n" ); 
	
	printf( "CHILD %d: Finished.\n\n", getpid() ); 
	kill( getppid(), SIGUSR2 );
	
	exit(0);	
}

// ChildSigHandle
// Should be bound to childAction signal handling event
void childSigHandle( int sigid )
{
	switch(sigid)
	{
	case SIGUSR1:
		printf( "*** Child SIGUSR1 handler - Received 'task start' signal from parent ***\n\n");
		break;
	case SIGUSR2:
		printf( "*** Child SIGUSR2 handler - Received 'task complete verification' signal from parent ***\n\n");
		break;
	default:
		printf( "CHILD: Unexpected child signal id %d\n", sigid);
	}
}