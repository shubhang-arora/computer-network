/* CSD 304 Computer Networks, Fall 2016
   Lab 2, server
   Team:  Wagish Sharma
		  Sahil Sharma
	      Shubhang Aroa 
		  Pulkit gairola
to run type " ./server 0.0.0.0 filename.something 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SERVER_PORT 6001
#define BUF_SIZE 40480


int main(int argc, char * argv[]){
  struct sockaddr_in sin;
  struct sockaddr_storage client_addr;
  char clientIP[INET_ADDRSTRLEN]; /* For IPv4 addresses */
  socklen_t client_addr_len;
  char buf[BUF_SIZE];
  int len;
  int s;
  char *host;
  struct hostent *hp;
  ssize_t bytesSent, bytesRead, totalFileSize;
  int i = 1;
  struct timespec tim, tim2;
  tim.tv_sec = 0;
  tim.tv_nsec = 50000;
  


  // Declarations for file(s) to be sent 
     FILE *fp;

  /* For inserting delays, use nanosleep()
     struct timespec ... */ 
 

  /* To get filename from commandline */
    
   
  /* Create a socket */
  if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("server: socket");
    exit(1);
  }
 
 
  /* build address data structure and bind to all local addresses*/
  memset((char *)&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
 
  /* If socket IP address specified, bind to it. */
  if(argc == 2) {
    host = argv[1];
    hp = gethostbyname(host);
    if (!hp) {
      fprintf(stderr, "server: unknown host %s\n", host);
      exit(1);
    }
    memcpy((char *)&sin.sin_addr, hp->h_addr, hp->h_length);
  }
  /* Else bind to 0.0.0.0 */
  else
    sin.sin_addr.s_addr = INADDR_ANY;
  
  sin.sin_port = htons(SERVER_PORT);
  
  if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0) {
    perror("server: bind");
    exit(1);
  }
  else{
    /* Add code to parse IPv6 addresses */
    inet_ntop(AF_INET, &(sin.sin_addr), clientIP, INET_ADDRSTRLEN);
    printf("Server is listening at address %s:%d\n", clientIP, SERVER_PORT);
  }
  
  printf("Client needs to send \"GET\" to receive the file %s\n", argv[1]);
    
  
  client_addr_len = sizeof(client_addr);
  
  /* Receive messages from clients*/
  while(len = recvfrom(s, buf, sizeof(buf), 0,
		       (struct sockaddr *)&client_addr, &client_addr_len)){
  	i=1;
  	if(argc == 3) {
	    fp = fopen(argv[2], "r");
	    if (fp == NULL) {
	      fprintf(stderr, "Error opening file\n");
	      exit(1);
	    }
  	}
    
    inet_ntop(client_addr.ss_family,
	      &(((struct sockaddr_in *)&client_addr)->sin_addr),
	      clientIP, INET_ADDRSTRLEN);
    
    printf("Server got message from %s: %s [%d bytes]\n", clientIP, buf, len);
    totalFileSize=0;
    if(strcmp(buf,"GET\n")==0){
      printf("GET True\n");
      while((bytesRead = fread(buf,1,BUF_SIZE,fp)) > 0 ){
      	
      	if((bytesSent = sendto(s, buf, bytesRead, 0, (struct sockaddr*)&client_addr, client_addr_len) ) < bytesRead){
      		perror("unable to Send");
      		return -1;
      	}
      	totalFileSize += bytesSent;
      	printf("Loop: %d Bytes Read: %zu , Bytes Sent: %zu , Total Bytes sent : %zu \n",i,bytesRead,bytesSent,totalFileSize);
      	i++;
      	nanosleep(&tim,&tim2);
      }

    }
    //puts("FILE SENT !");
    fclose(fp);
  /* Send BYE to signal termination */
  	strcpy(buf, "BYE");
  //puts("In the end");
  	sendto(s, buf, sizeof(buf), 0, 
	 (struct sockaddr*)&client_addr, client_addr_len);
  	printf("File Successfully Sent!");
  }
  memset(buf, 0, sizeof(buf));
}

