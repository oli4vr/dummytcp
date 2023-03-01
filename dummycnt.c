/* dummycnt.c
 * by Olivier Van Rompuy
 * https://github.com/oli4vr
 *
 * Dummy tcp client
 * Connect to a dummy tcp service and stream zero bytes uninterrupted.
 * */
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/time.h>
#include <inttypes.h>
#include "tcpd.h"

#define BUF_SIZE 1500


int main(int argc,char ** argv)
{
 int s,n;
 struct sockaddr_in sin;
 char buf[BUF_SIZE]={0};

 if (argc<3) {
  fprintf(stderr,"Dummy TCP Client\nby Olivier Van Rompuy\nSyntax : %s IP_Address TCP_Port\n",argv[0]);
  return -1;
 }

 s=socket(AF_INET,SOCK_STREAM,0);
 if (s==-1) {
  fprintf(stderr,"Error opening socket!\n");
  return -2;
 }

 sin.sin_addr.s_addr = inet_addr(argv[1]);
 sin.sin_family = AF_INET;
 sin.sin_port = htons( atoi(argv[2]) );

 if (connect(s ,(struct sockaddr *)&sin,sizeof(sin))<0) {
  fprintf(stderr,"Error connection failed!\n");
  return -3;
 }

 while (1) {
  for(n=0;n<10000;n++) {
  send(s,buf,BUF_SIZE,0);
  }
  usleep(10);
 }
 close(s);

 return 0;
}
