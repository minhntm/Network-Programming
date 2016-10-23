#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <poll.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORT 5500
#define BACKLOG 20
#define MAX_ELEMENT 50

void close_conn(struct pollfd input[],int* maxfd){
  int i,j;
  for(i =0;i<*maxfd;i++){
    if(input[i].fd == -1){
      for(j = i;j <*maxfd;j++){
        input[j].fd = input[j+1].fd;
      }
      *maxfd -=1;
    }
  }
}

void upperStr(char* str){
  int i;
  for (i=0; i<strlen(str); i++){
    str[i] = toupper(str[i]);
  }
}

int main(){

  int listenfd, connfd;
  char recv_data[1024];
  int bytes_sent, bytes_received;
  struct sockaddr_in server; /* server's address information */
  struct sockaddr_in client; /* client's address information */
  int sin_size=sizeof(struct sockaddr_in);
  int rv;
  int clients[50];
  struct pollfd ufds[MAX_ELEMENT];

  int i;

  if ((listenfd=socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  /* calls socket() */
    printf("socket() error\n");
    exit(-1);
  }

  for(i = 0; i< MAX_ELEMENT;i++){
    ufds[i].fd = -1;
    ufds[i].events = POLLIN;
  }

  ufds[0].fd = listenfd;
  ufds[0].events = POLLIN;

  int maxfd = 1;

  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);
  server.sin_addr.s_addr = htonl(INADDR_ANY);


  if(bind(listenfd,(struct sockaddr*)&server,sizeof(server))==-1){
    printf("bind() error\n");
    exit(-1);
  }

  if(listen(listenfd,BACKLOG) == -1){
    printf("listen() error\n");
    exit(-1);
  }

  while(1){
    //time out = 10 min
    rv = poll(ufds,maxfd, 10*60*1000);
    if(rv < 0){
      perror("poll() failed");
      break;
    }
    if(rv ==0){
      printf("poll() time out.\n");
      break;
    }
    if(ufds[0].revents & POLLIN){
      if((connfd = accept(listenfd,NULL,NULL)) == -1){
        printf("accept() error\n" );
        exit(-1);
      }
      printf("New incoming connection - %d\n", connfd);
      if(maxfd == MAX_ELEMENT){
        printf("FULL\n");
        break;
      }
      ufds[maxfd].fd = connfd;
      ufds[maxfd].events = POLLIN;
      maxfd++;
    }

    for (i=1 ; i < maxfd; i++){
      if(ufds[i].revents & POLLIN){

        printf("You got a connection from %s\n",inet_ntoa(client.sin_addr) ); /* prints client's IP */

        FILE *f = fopen("file_server.txt", "w");

        puts("Receiving file...");
        while (1){
          bytes_received = recv(ufds[i].fd,recv_data,1024,0);

          if (bytes_received < 0){
            printf("\nError!Can not receive data from client!");
            close(ufds[i].fd);
            close_conn(ufds,&maxfd);
            break;
          } else if (bytes_received == 1){
            printf("Finish convert file!\n");
            break;
          } else {
            recv_data[bytes_received] = '\0';
            upperStr(recv_data);
            fprintf(f, "%s", recv_data);
          }

          bytes_sent = send(ufds[i].fd,recv_data,strlen(recv_data),0);
          if (bytes_sent < 0){
            printf("\nError!Can not sent data to client!");
            close(ufds[i].fd);
            close_conn(ufds,&maxfd);
            break;
          }
        }
        fclose(f);

        puts("Da nhan xong");

        sin_size=sizeof(client);
        f = fopen("file_server.txt", "r");

        puts("Sending file...");

        while (fgets(recv_data, 1024, f)!=NULL){
          bytes_sent = send(ufds[i].fd,recv_data,strlen(recv_data),0);
          if (bytes_sent < 0){
            printf("\nError!Can not sent data to client!");
            close(ufds[i].fd);
            close_conn(ufds,&maxfd);
            break;
          }
          bytes_received = recv(ufds[i].fd,recv_data,1024,0);
          // puts(recv_data);
        }

        //send 0 bytes to finish send file process
        bytes_sent = send(ufds[i].fd,recv_data,1,0);
        if (bytes_sent < 0){
          printf("\nError!Can not sent data to client!");
          close(ufds[i].fd);
          close_conn(ufds,&maxfd);
          break;
        }

        puts("Finish! Close connection!");
        puts("-------------------------\n");

        fclose(f);


        close(ufds[i].fd);
        close_conn(ufds,&maxfd);


      }
    }

  }
  close(listenfd);
  return 1;
}