#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <poll.h>

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

typedef struct a{
  char user[20];
  char pass[20];
  int status;
} User;

void load_data(User* u, int* size){
  FILE *f = fopen("account.txt", "r");
  if (f == NULL){
    printf("FILE NOT FOUND!\n");
    exit(-1);
  }
  char s[30];
  while (fscanf(f, "%s", s)!=EOF){
    // puts(s);
    (*size) += 1;
    strcpy(u[*size - 1].user, s);
    // puts(s);
    fscanf(f, "%s", s);
    strcpy(u[*size - 1].pass, s);
    u[*size - 1].status = 0;
  }

  fclose(f);
}

int main(){

  int listenfd, connfd; /* file descriptors */
  char recv_data[1024];
  int bytes_sent, bytes_received;
  struct sockaddr_in server; /* server's address information */
  struct sockaddr_in client; /* client's address information */
  int rv;
  struct pollfd ufds[MAX_ELEMENT];
  int i;

  User u[100];
  int size=0;

  load_data(u, &size);


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
  server.sin_port = htons(PORT);   /* Remember htons() from "Conversions" section? =) */
  server.sin_addr.s_addr = htonl(INADDR_ANY);  /* INADDR_ANY puts your IP address automatically */

  if(bind(listenfd,(struct sockaddr*)&server,sizeof(server))==-1){ /* calls bind() */
    printf("bind() error\n");
    exit(-1);
  }

  if(listen(listenfd,BACKLOG) == -1){  /* calls listen() */
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
      printf("poll() time out. END\n");
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
        int k;


        bytes_received = recv(ufds[i].fd,recv_data,1024,0);

        if (bytes_received < 0){
          printf("\nError!Can not receive data from client!");
          close(ufds[i].fd);
          close_conn(ufds,&maxfd);
          break;
        } else {
          recv_data[bytes_received] = '\0';
        }

        if (strcmp(recv_data, "user")==0){

          bytes_sent = send(ufds[i].fd,"ready",5,0);
          if (bytes_sent < 0){
            printf("\nError!Can not sent data to client!");
            close(ufds[i].fd);
            close_conn(ufds,&maxfd);
            break;
          }

          bytes_received = recv(ufds[i].fd,recv_data,1024,0);

          if (bytes_received < 0){
            printf("\nError!Can not receive data from client!");
            close(ufds[i].fd);
            close_conn(ufds,&maxfd);
            break;
          } else {
            recv_data[bytes_received] = '\0';
          }

          int check = 0;
          for (k=0;k<size; k++){
            if (strcmp(u[k].user, recv_data)==0){
              check = 1;
              break;
            }
          }

          recv_data[0] = (check == 0) ? '0' : '1';

          bytes_sent = send(ufds[i].fd,recv_data,1,0);
          if (bytes_sent < 0){
            printf("\nError!Can not sent data to client!");
            close(ufds[i].fd);
            close_conn(ufds,&maxfd);
            break;
          }

        } else if (strcmp(recv_data, "pass")==0){

          k = 1;

          bytes_sent = send(ufds[i].fd,"ready",5,0);
          if (bytes_sent < 0){
            printf("\nError!Can not sent data to client!");
            close(ufds[i].fd);
            close_conn(ufds,&maxfd);
            break;
          }

          bytes_received = recv(ufds[i].fd,recv_data,1024,0);

          if (bytes_received < 0){
            printf("\nError!Can not receive data from client!");
            close(ufds[i].fd);
            close_conn(ufds,&maxfd);
            break;
          } else {
            recv_data[bytes_received] = '\0';
          }

          for (k=0;k<size; k++){
            if (strcmp(u[k].user, recv_data)==0){
              break;
            }
          }

          int uid;
          uid=k;

          bytes_sent = send(ufds[i].fd,"ready",5,0);
          if (bytes_sent < 0){
            printf("\nError!Can not sent data to client!");
            close(ufds[i].fd);
            close_conn(ufds,&maxfd);
            break;
          }

          bytes_received = recv(ufds[i].fd,recv_data,1024,0);

          if (bytes_received < 0){
            printf("\nError!Can not receive data from client!");
            close(ufds[i].fd);
            close_conn(ufds,&maxfd);
            break;
          } else {
            recv_data[bytes_received] = '\0';
          }

          if (strcmp(u[uid].pass, recv_data)==0){
            printf("User %s login.\n", u[uid].user);
            u[uid].status = 1;

            recv_data[0]='1';
            bytes_sent = send(ufds[i].fd,recv_data,1,0);
            if (bytes_sent < 0){
              printf("\nError!Can not sent data to client!");
              close(ufds[i].fd);
              close_conn(ufds,&maxfd);
              break;
            }
            break;
          } else {
            recv_data[0] = '0';
            bytes_sent = send(ufds[i].fd,recv_data,1,0);
            if (bytes_sent < 0){
              printf("\nError!Can not sent data to client!");
              close(ufds[i].fd);
              close_conn(ufds,&maxfd);
              break;
            }
            if (k==5){
              puts("Login Fail!");
              close(ufds[i].fd);
              close_conn(ufds,&maxfd);
              break;
            } else {
              k++;
            }
          }

        } else if (strcmp(recv_data, "q")==0 || strcmp(recv_data, "Q")==0){

            printf("logout.\n");
            close(ufds[i].fd);
            close_conn(ufds,&maxfd);

        }
      }
    }
  }
  close(listenfd);
  return 1;
}
