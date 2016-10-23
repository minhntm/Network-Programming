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
void sig_chld(int signo){
  pid_t pid;
  int stat;
  while((pid=waitpid(-1,&stat,WNOHANG))>0)
    printf("[ForkingServer] Child %d terminated\n",pid);
}
void upperStr(char* str){
  int i;
  for (i=0; i<strlen(str); i++){
    str[i] = toupper(str[i]);
  }
}
int main(){
  int listen_sock, conn_sock; /* file descriptors */
  char recv_data[1024];
  int bytes_sent, bytes_received;
  struct sockaddr_in server; /* server's address information */
  struct sockaddr_in client; /* client's address information */
  pid_t pid;
  int sin_size;
  FILE* fr;
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

      FILE *f = fopen("file_server.txt", "w");

      puts("Receiving file...");
      while (1){
        bytes_received = recv(conn_sock,recv_data,1024,0);

        if (bytes_received < 0){
          printf("\nError!Can not receive data from client!");
        } else if (bytes_received == 1){
          printf("Finish convert file!\n");
          break;
        } else {
          recv_data[bytes_received] = '\0';
          upperStr(recv_data);
          fprintf(f, "%s", recv_data);
        }

        bytes_sent = send(conn_sock,recv_data,strlen(recv_data),0);
        if (bytes_sent < 0){
          printf("\nError!Can not sent data to client!");
          break;
        }
      }
      fclose(f);

      puts("Da nhan xong");

      sin_size=sizeof(client);
      f = fopen("file_server.txt", "r");

      puts("Sending file...");

      while (fgets(recv_data, 1024, f)!=NULL){
        bytes_sent = send(conn_sock,recv_data,strlen(recv_data),0);
        if (bytes_sent < 0){
          printf("\nError!Can not sent data to client!");
          break;
        }
        bytes_received = recv(conn_sock,recv_data,1024,0);
        // puts(recv_data);
      }

      //send 0 bytes to finish send file process
      bytes_sent = send(conn_sock,recv_data,1,0);
      if (bytes_sent < 0){
        printf("\nError!Can not sent data to client!");
        break;
      }

      puts("Finish! Close connection!");
      puts("-------------------------\n");

      fclose(f);


      exit(0);
    }
    puts("signal");
    signal(SIGCHLD,sig_chld);
    close(conn_sock);
  }
  close(listen_sock);
  return 0;
}