/****************
 Taylor Caldwell
 Lab 7 
 February 23, 2016
*****************/

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h>
#include "otar_sb.h"

int main(int argc, char * argv[])
{
 
  int c;
  while((c = getopt(argc, argv, "hvV")) != -1)
  {
    switch(c)
      {
      case 'h':
        printf("\n----HELP DESK----\n");
	printf("-a add member(s) from command line to archive\n");
        printf("-d delete member(s) from command line\n");
	printf("-e extract member(s) from archive to file, from command line\n");
	printf("-h show the help text and exit\n");
	printf("-t short table of contents\n");
        printf("-T long table of contents\n");
	printf("-v verbose processing\n");
	printf("-V show version and exit\n\n");
	break;
      case 'v':
	printf("\n-**verbose processing**\n");
	break;
      case 'V':
	printf("\n**Otar version 1.0.**\n");
      default:
	abort();
       }
  }
  return 0;
}
