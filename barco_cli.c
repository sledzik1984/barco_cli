#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    if (argc < 3) {

       fprintf(stderr,"Barco Command Line Tool\nUsage: %s hostname\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

	unsigned char lamp_timer[6];

	lamp_timer[0]=0xfe;
	lamp_timer[1]=0x00;
	lamp_timer[2]=0x76;
	lamp_timer[3]=0x90;
	lamp_timer[4]=0x06;
	lamp_timer[5]=0xff;


    n = write(sockfd,lamp_timer,sizeof(lamp_timer));
    if (n < 0) 
         error("ERROR writing to socket");
	//printf("SENT: %02x\n",lamp_timer);

	bzero(lamp_timer,6);
    n = read(sockfd,lamp_timer,255);

	//Test długości lamp_timer w bajtach
	//int leng;
	//leng = sizeof(lamp_timer);
	//printf("%d\n",leng);

    if (n < 0) 
         error("ERROR reading from socket");


/*
    printf("RECV0: %02x\n",lamp_timer[0]);
    printf("RECV1: %02x\n",lamp_timer[1]);
    printf("RECV2: %02x\n",lamp_timer[2]);
    printf("RECV3: %02x\n",lamp_timer[3]);
    printf("RECV4: %02x\n",lamp_timer[4]);
    printf("RECV5: %02x\n",lamp_timer[5]);
    printf("RECV6: %02x\n",lamp_timer[6]);
    printf("RECV7: %02x\n",lamp_timer[7]);
    printf("RECV8: %02x\n",lamp_timer[8]);
    printf("RECV9: %02x\n",lamp_timer[9]);
    printf("RECV10: %02x\n",lamp_timer[10]);
    printf("RECV11: %02x\n",lamp_timer[11]);
    printf("RECV12: %02x\n",lamp_timer[12]);
    printf("RECV13: %02x\n",lamp_timer[13]);
    printf("RECV14: %02x\n",lamp_timer[14]);
    printf("RECV15: %02x\n",lamp_timer[15]);
    printf("RECV16: %02x\n",lamp_timer[16]);

*/
    
	unsigned char lamp_seconds[4];

	lamp_seconds[0]=lamp_timer[10];
	lamp_seconds[1]=lamp_timer[11];
	lamp_seconds[2]=lamp_timer[12];
	lamp_seconds[3]=lamp_timer[13];


	//printf("Lamp time: %d",lamp_seconds);



//4prog

char flampFameData[] = { 0x76, 0x90, lamp_seconds[0], lamp_seconds[1], lamp_seconds[2], lamp_seconds[3] };
//if (flampFameData[0] != LampDataMark) {
//    return error;
//}
//f (flampFameData[1] != LampCounterMark) {
//    return error;
//}
long unsigned lampLifetime = 0;
int i;
// read big endian value
for (i=2; i<6; ++i)
    lampLifetime = (lampLifetime << 8) | (unsigned char)flampFameData[i];
 
fprintf(stdout, "Lamp lifetime is: %lu seconds\n\n", lampLifetime);






close(sockfd);
    return 0;
}
