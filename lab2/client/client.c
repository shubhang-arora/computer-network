/* CSD 304 Computer Networks, Fall 2016
   Lab 2, client
   Team member : Wagish Sharma
				 Sahil Sharma
				 Shubhang Aroa 
				 Pulkit gairola
to run type " ./client 0.0.0.0 filename.something 
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
  
  FILE *fp;
  struct hostent *hp;
  struct sockaddr_in sin;
  char *host;
  char buf[BUF_SIZE];
  int s,i=1;
  int len;
  ssize_t rcv;
  socklen_t addrlen;
	/* to check arg 3 */
  if ((argc==2)||(argc == 3)) {
    host = argv[1];
  }
  else {
    fprintf(stderr, "usage: client serverIP [download_filename(optional)]\n");
    exit(1);
  }

  if(argc == 3) {
    fp = fopen(argv[2], "w");
    if (fp == NULL) {
      fprintf(stderr, "Error opening output file\n");
      exit(1);
    }
  }

  /* translate host name into peer's IP address */
  hp = gethostbyname(host);
  if (!hp) {
    fprintf(stderr, "client: unknown host  %s\n", host);
    exit(1);
  }
  else
    printf("Host %s found at !\n", argv[1]);

  /* build address data structure */
  memset((char *)&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  memcpy((char *)&sin.sin_addr, hp->h_addr, hp->h_length);
  sin.sin_port = htons(SERVER_PORT);
  

  /* to create new socket */
  if ((s = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("client: socket");
    exit(1);
  }
  
  printf("Client will get data from to %s:%d.\n", argv[1], SERVER_PORT);
  printf("To play the music, pipe the downlaod file to a player, e.g., ALSA, SOX, VLC: cat recvd_file.wav | vlc -\n"); 
  
  
  

  /* send message to server */  
  fgets(buf, sizeof(buf), stdin);
  buf[BUF_SIZE-1] = '\0';
  len = strlen(buf) + 1;
  if (sendto(s, buf, len, 0, (struct sockaddr *)&sin, sizeof(sin)) < 0) { 
    perror("Client: sendto()"); 
    return 0; 
  }
    
  //fputs(buf, stdout);
  
  addrlen = sizeof(sin);

 	while((rcv = recvfrom(s, buf, BUF_SIZE, 0, (struct sockaddr *)&sin, &addrlen) )>0) {
 		 printf("Loop : %d Received Bytes : %zu \n",i,rcv);
     i++;
     if(strcmp("BYE",buf)==0){
        printf("File Recieved \n");
        printf("Client Closing \n");
        fclose(fp);
        exit(0);
     }
     if(fwrite(buf,1,rcv,fp)<0){
        perror("Error Writing to File");
     }
 		
    fflush(fp);
	   
 	}
  
}
