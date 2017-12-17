/*#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
 
void nonblock(int sockfd)
{
    int opts;
    opts = fcntl(sockfd, F_GETFL);
    if(opts < 0)
    {
        fprintf(stderr, "fcntl(F_GETFL) failedn");
    }
    opts = (opts | O_NONBLOCK);
    if(fcntl(sockfd, F_SETFL, opts) < 0) 
    {
        fprintf(stderr, "fcntl(F_SETFL) failedn");
    }
}
 
int main(int argc, char *argv[])
{
    int BUFLEN = 2000;
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    int i;
 
    char buffer[BUFLEN];
    if (argc < 3) {
       fprintf(stderr,"usage: %s hostname_or_ip portn", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR creating socket");
        exit(1);
    }
    //get the address info by either host name or IP address
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such hostn");
        exit(1);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)  {
        perror("ERROR connecting");
        exit(1);
    }
    printf("connection establishedn");
    //nonblock(sockfd);    //uncomment this line if we want to make the socket non-block
    for (i = 0; i < 5; ++i) {
        printf("Please enter the message: ");
        bzero(buffer,BUFLEN);
        fgets(buffer,BUFLEN,stdin);
        n = write(sockfd,buffer,strlen(buffer));
        printf("sent: %s", buffer);
        if (n < 0) {
             perror("ERROR writing to socket");
        }
        bzero(buffer,BUFLEN);
        n = read(sockfd,buffer,BUFLEN);
        if (n < 0) {
             perror("ERROR reading from socket");
        }
        printf("received: %s",buffer);
    }
    close(sockfd);
    return 0;
}*/

#include <winsock2.h>
  
int run_tcp_client()
{
    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;
 
    WSAStartup(MAKEWORD(2,0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);
 
    addr.sin_addr.s_addr = inet_addr("192.168.0.8"); // replace the ip with your futur server ip address. If server AND client are running on the same computer, you can use the local ip 127.0.0.1
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);
 
    connect(server, (SOCKADDR *)&addr, sizeof(addr));
    //cout << "Connected to server!" << endl;
 
    char buffer[1024]={'h', 'e', 'l', 'l', 'o', '.'};
    send(server, buffer, sizeof(buffer), 0);
    //cout << "Message sent!" << endl;
 
    closesocket(server);
    WSACleanup();
    //cout << "Socket closed." << endl << endl;
}
