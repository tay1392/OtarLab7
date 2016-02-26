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
#include <libgen.h> //this

void TableOfContents_Short(int filenum);
void PrintHelpPrompt();
void Append(int argc, char * OtarFname, char * argv[]);

int main(int argc, char * argv[])
{
  // otar_hdr_t hdr; 
  // int i;
  int fd = -1;
  int c;
  char * fname = argv[2];
  //  int number_of_files = argc - 3; /*-3 because ./otar , option(-a ,-v...) and the archive file*/

  while((c = getopt(argc, argv, "hvVt:a:")) != -1)
  {
    switch(c)
      {
      case 'h':
	PrintHelpPrompt();
	break;
      case 'v':
	printf("\n-**verbose processing**\n");
	break;
      case 'V':
	printf("\n**Otar version 1.0.**\n");
      case 't':
	printf("Short table of contents for otar archive file: %s\n", fname);
        fd = open(fname, O_RDONLY);
	TableOfContents_Short(fd);
	break;
      case 'a':
	 //call append for each file to be added
	Append(argc, fname, argv); 	  
      default:
	//abort();
	printf("wonkie option given\n");
       }
  }
  fprintf(stderr, "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
  return 0;
}

void PrintHelpPrompt()
{
      printf("Program: otar\n");
      printf("  options: adehtTvV\n\n");
      printf("    -a add member(s) from command line to archive\n");
      printf("    -d delete member(s) from command line\n");
      printf("    -e extract member(s) from archive to file, from command line\n");
      printf("    -h show the help text and exit\n");
      printf("    -t short table of contents\n");
      printf("    -T long table of contents\n");
      printf("    -v verbose processing\n");
      printf("    -V show version and exit\n\n");

      printf("otar: Version 1.0\n");
}

void TableOfContents_Short(int filenum)
{
  //example code from demo7-1.c
  int fd = filenum;
  ssize_t result;
  size_t file_name_len;
  otar_hdr_t hdr;
  char file_name_len_char[OTAR_FNAME_LEN_SIZE + 1];
  char file_name[OTAR_MAX_FILE_NAME_LEN + 1];
  char file_size_char[OTAR_FILE_SIZE + 1];
  size_t file_size;
  char * buffer = malloc(sizeof(OTAR_ID_LEN));  

  if(fd > 0)
    {
      read(fd, (void *) buffer, sizeof(buffer)); 
	   
      if (strcmp(buffer, "!<otar>\n") != 0)
      {
	  fprintf(stderr, "Error: not an Otar file - missing file header\n");
	  exit(EXIT_FAILURE);
      }	   

      while((result = read(fd, (void *) &hdr, sizeof(otar_hdr_t))) > 0)
	{
	  memset((void *) file_name_len_char, 0, sizeof(file_name_len_char));
          
	  //converting ascii  to int
	  strncpy(file_name_len_char, hdr.otar_fname_len, OTAR_FNAME_LEN_SIZE);
	  file_name_len = (size_t) atoi(file_name_len_char);
	  
	  memset((void *) file_name, 0, sizeof(file_name));
	  strncpy(file_name, hdr.otar_fname, file_name_len);
	  printf("\t%s\n", file_name);

	  memset((void *) file_size_char, 0, sizeof(file_size_char));
	  strncpy(file_size_char, hdr.otar_size, OTAR_FILE_SIZE);
	  file_size = (size_t) atoi(file_size_char);

	  lseek(fd, file_size, SEEK_CUR);
	}
    }
}

void Append(int argc, char * OtarFname, char * argv[])
{
  //numfiles = number of files to be appended
  //otarfname = archive file name 
  int fd = -1;
  int check_fd = -1;
  int res;
  otar_hdr_t hdr;
  //ssize_t NumWritten;
  int i = 0;
  char buffer[100];
  char temp[100];
  struct stat fd_stat;

  //open otar
  fd = open(OtarFname, O_RDWR | O_CREAT, 0644);
  check_fd = open(OtarFname, O_RDWR | O_CREAT, 0644);
  if(fd > 0)
    {
      fprintf(stderr, "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
      memset(buffer, 0, sizeof(buffer)); 
      res =  read(check_fd, (void *) temp, sizeof(temp));
      fprintf(stderr, "%s %s %d %d\n", __FILE__, __FUNCTION__, __LINE__, res);
      if(res == 0)
	{
	  fprintf(stderr, "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	  //empty file
	  //write otar id into file
	   write(fd, (void *) OTAR_ID, OTAR_ID_LEN);  
	  //use #define var in .h	  
	  fprintf(stderr, "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	}
      //else
//	{
          /* Read otar header tag */
          read(fd, (void *) buffer, OTAR_ID_LEN);
	  
          int read_res = -1;
	  fprintf(stderr, "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	  //read first id size bytes into buffer
	  //memset(buffer, 0, sizeof(buffer));
	  //read_res = read(fd, (void *) buffer, OTAR_ID_LEN);//buffer holds the otar ID
	  fprintf(stderr, "%s %s %d %d ++%s++\n", __FILE__, __FUNCTION__, __LINE__, read_res, buffer);
	  if(strcmp(buffer, OTAR_ID) == 0)
	    {
	      printf("\nstrcmp works\n");
	      //if zero then correct otar file
	      //if not zero, then throw error - invalid file type
	      
	      //seek to end of file
	       lseek(fd, 0, SEEK_END);
	      //loop thru command line
	       fprintf(stderr, "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	       for(i = 3; i < argc ; i++) //i = 3 b/c [0], [1], [2] holds ./otar, option, otar file
	       {  
		  //argv[i] holds first file to be added
		 printf("%s\n", argv[i]);
		 fprintf(stderr, "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	          //build otar header type

		 int newfd;
		 //open the file
	         newfd = open(argv[i], O_RDONLY);
	 
		 //fstat the file
		 fstat(newfd, &fd_stat);
h 
 		 //fill ouot hdr
		 memset(&hdr, ' ', sizeof(hdr));
		 memcpy(&hdr.otar_fname
			, basename(argv[i]), strlen(basename(argv[i])));

		 memset(buffer,0, sizeof(buffer));
		 //memcpy(buffer, hdr.otar_fname_len,OTAR_FNAME_LEN_SIZE);
		 sprintf(hdr.otar_fname_len, "%2d", (int) strlen(basename(argv[i])));
		 sprintf(hdr.otar_adate, "%ld", fd_stat.st_atime);
		 sprintf(hdr.otar_mdate, "%ld", fd_stat.st_mtime);
		 
		 sprintf(hdr.otar_uid, "%-d", fd_stat.st_uid);
		 sprintf(hdr.otar_gid, "%-d", fd_stat.st_gid);
		 //so.. adate, mdate, fname_len, otar_mode and otar_size
		 memcpy(hdr.otar_hdr_end, OTAR_HDR_END, strlen(OTAR_HDR_END));

		 sprintf(hdr.otar_mode, "%o", fd_stat.st_mode);
		 sprintf(hdr.otar_size, "%-ld", (long) fd_stat.st_size);
		 /*memcpy(&hdr.otar_fname_len, argv[i], strlen(argv[i]));
		 memcpy(&hdr.otar_adate, argv[i], strlen(argv[i]));
		 memcpy(&hdr.otar_mdate, argv[i], strlen(argv[i]));
		 memcpy(&hdr.otar_uid, argv[i], strlen(argv[i]));
		 memcpy(&hdr.otar_gid, argv[i], strlen(argv[i]));
		 memcpy(&hdr.otar_mode, argv[i], strlen(argv[i]));
		 memcpy(&hdr.otar_size, argv[i], strlen(argv[i]));
		 memcpy(&hdr.otar_hdr_end, argv[i], strlen(argv[i]));
		 */
		 
		

		 //write hdr to archive file
		 //write data to archive file

	         //write into file
	         //write data into file
		 //ssize_t W2;
		 write(fd, (void *) &hdr, sizeof(otar_hdr_t));
		 {
		   int r;
		   while ((r = read(newfd, buffer, sizeof(buffer))) > 0) {
		     write(fd, buffer, r);
		   }
		 }
		 close(newfd);
	       }
	    }
    }
  fprintf(stderr, "%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
}
