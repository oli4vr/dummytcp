/* dummysrv.c
 * by Olivier Van Rompuy
 * https://github.com/oli4vr
 *
 * Dummy TCP server
 * 
 * Accepts incoming tcp connections (multithreaded), receives data like 
 * a good bottomless pit and measures the incoming bandwidth.
 *
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
#include <time.h>
#include <inttypes.h>
#include "tcpd.h"

#define BUF_SIZE 1500

uint64_t counter=0;
uint64_t lastt=0;

// Return timestamp in microseconds
uint64_t getusecs() {
 uint64_t tmp;
 struct timeval tv;
 gettimeofday(&tv,NULL);
 tmp=tv.tv_sec;
 tmp*=1000000;
 tmp+=tv.tv_usec;
 return tmp;
}


void * tcpd_handle(void *p) {
 struct timeval tv;
 unsigned char out[4]={0};
 unsigned char buf[BUF_SIZE]={0};
 int rc,l;
 tv.tv_sec=5;
 tv.tv_usec=0;

 tcpcc *m=(tcpcc*)p;

 if (p==NULL) return NULL;

 pthread_detach(pthread_self());
 l=recv(m->sock, buf, BUF_SIZE, 0);
 while (l>0) {
  counter+=l;
  buf[l]=0;
  l=recv(m->sock, buf, BUF_SIZE, 0);
 }
 close(m->sock);
 free(m);
 m=NULL;
}

void main(int argc, char ** argv) {
 tcpd t;
 pthread_t thr;
 uint64_t curt;
 float dift;
 float calc_thr;

 if (argc<2) {
  t.port=30333;
 } else {
  t.port=atoi(argv[1]);
 }

 if (t.port<1) {
  fprintf(stderr,"Dummy TCP Server\nby Olivier Van Rompuy\nSyntax : %s TCP_PORT\nDefault port = 30333\n",argv[0]);
  exit(1);
 }

 t.data=NULL;
 t.hand=tcpd_handle;
 pthread_create(&thr, NULL, tcpd_daemon, (void*) &t);
 pthread_detach(thr);

 while (1) {
  usleep(500);
  curt=getusecs();
  dift=curt-lastt;
  if (dift>1000000) {
   calc_thr=(float)counter/(dift/1000);
   lastt=curt;
   counter=0;
   printf("Incoming Througput = %12.0f KByte/s\n",calc_thr);
  }
 }
}
