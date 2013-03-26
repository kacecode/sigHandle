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