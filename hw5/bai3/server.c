#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <ctype.h>
#define PORT 5500
#define BACKLOG 20

typedef struct a{
  char user[20];
  char pass[20];
  int status;
} User;

void sig_chld(int signo){
  pid_t pid;
  int stat;
  while((pid=waitpid(-1,&stat,WNOHANG))>0)
    printf("[ForkingServer] Child %d terminated\n",pid);
}

void load_data(User* u, int* size){
  FILE *f = fopen("account.txt", "r");
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
  int listen_sock, conn_sock; /* file descriptors */
  char recv_data[1024];
  int bytes_sent, bytes_received;
  struct sockaddr_in server; /* server's address information */
  struct sockaddr_in client; /* client's address information */
  pid_t pid;
  int sin_size;

  User u[100];
  int size=0;

  load_data(u, &size);
  int i;
  int uid;
  // for (i=0; i<size; i++){
  //   printf("%s - %s\n", u[i].user, u[i].pass);
  // }

  if ((listen_sock=socket(AF_INET, SOCK_STREAM, 0)) == -1 ){  /* calls socket() */
    printf("socket() error\n");
    exit(-1);
  }
  bzero(&server, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);   /* Remember htons() from "Conversions" section? =) */
  server.sin_addr.s_addr = htonl(INADDR_ANY);  /* INADDR_ANY puts your IP address automatically */

  if(bind(listen_sock,(struct sockaddr*)&server,sizeof(server))==-1){ /* calls bind() */
    printf("bind() error\n");
    exit(-1);
  }

  if(listen(listen_sock,BACKLOG) == -1){  /* calls listen() */
    printf("listen() error\n");
    exit(-1);
  }

  while(1){
    sin_size=sizeof(struct sockaddr_in);
    if ((conn_sock = accept(listen_sock,(struct sockaddr *)&client,&sin_size))==-1){ /* calls accept() */
      printf("accept() error\n");
      exit(-1);
    }

    if((pid=fork())==0){
      close(listen_sock);

      printf("You got a connection from %s\n",inet_ntoa(client.sin_addr) ); /* prints client's IP */

      while(1){
        bytes_received = recv(conn_sock,recv_data,1024,0);

        if (bytes_received < 0){
          printf("\nError!Can not receive data from client!");
          exit(-1);
        } else {
          recv_data[bytes_received] = '\0';
        }


        int check = 0;
        for (i=0;i<size; i++){
          if (strcmp(u[i].user, recv_data)==0){
            check = 1;
            break;
          }
        }

        recv_data[0] = (check == 0) ? '0' : '1';

        bytes_sent = send(conn_sock,recv_data,1,0);
        if (bytes_sent < 0){
          printf("\nError!Can not sent data to client!");
          exit(-1);
        }

        if (check == 1){
          break;
        }
      }

      uid = i;

      i = 1;
      while(1){
        bytes_received = recv(conn_sock,recv_data,1024,0);

        if (bytes_received < 0){
          printf("\nError!Can not receive data from client!");
          exit(-1);
        } else {
          recv_data[bytes_received] = '\0';
        }

        if (strcmp(u[uid].pass, recv_data)==0){
          printf("User %s login.\n", u[uid].user);
          u[uid].status = 1;

          recv_data[0]='1';
          bytes_sent = send(conn_sock,recv_data,1,0);
          if (bytes_sent < 0){
            printf("\nError!Can not sent data to client!");
            exit(-1);
          }
          break;
        } else {
          recv_data[0] = '0';
          bytes_sent = send(conn_sock,recv_data,1,0);
          if (bytes_sent < 0){
            printf("\nError!Can not sent data to client!");
            exit(-1);
          }
          if (i==5){
            puts("Login Fail!");
            break;
          } else {
            i++;
          }
        }
      }

      while (1){
        bytes_received = recv(conn_sock,recv_data,1024,0);
          if (bytes_received < 0){
            printf("\nError!Can not receive data from client!");
            exit(-1);
          } else {
            recv_data[bytes_received] = '\0';
          }

        if (strcmp(recv_data, "q")==0 || strcmp(recv_data, "Q")==0){
          u[uid].status = 0;
          printf("%s logout.\n", u[uid].user);
          break;
        }
      }

      exit(0);
    }
    puts("signal");
    signal(SIGCHLD,sig_chld);
    close(conn_sock);
  }
  close(listen_sock);
  return 0;
}