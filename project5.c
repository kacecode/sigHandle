#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

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
	// set signal options
	struct sigaction SA;
	sigset_t SS;
  
	sigemptyset( &SA.sa_mask );
	SA.sa_flags = 0;
	SA.sa_handler = parentSigHandle;
	// finish setting signal options
  
	// bind handlers
	sigaction( SIGUSR1, &SA, NULL );
	sigaction( SIGUSR2, &SA, NULL );
	
	// Print Project Header
	printf( "\nProject 5\nKacey Cole\nMichael Pugina\nCS3060-001\n\n" );
	
	printf( "PARENT %d: Spawning child %d\n\n", getpid(), childpid );
	sleep(3);
	printf( "PARENT: Child to start work.\n\n" );
	kill( childpid, SIGUSR1 );  // send signal 1
	
	// suspend until given SIGUSR1
	sigfillset( &SS );
	sigdelset( &SS, SIGUSR1 );
	sigsuspend( &SS );
  
	// sleep to give child time
	sleep(3);
	
	// send signal 2
	kill( childpid, SIGUSR2 );
	printf( "PARENT: Told child to notify of task completion\n\n" );
  
	// suspend until given SIGUSR2
	sigdelset( &SS, SIGUSR2 );
	sigsuspend( &SS );
  
	printf( "PARENT %d: Finished.\n\n", getpid() );
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