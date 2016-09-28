#include <stdio.h>          /* These are the usual header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORT 5500   /* Port that will be opened */
#define BACKLOG 2   /* Number of allowed connections */

void converStr(char* str){
  int i;
  for (i=0; i<strlen(str); i++){
    str[i] = toupper(str[i]);
  }
}

int main()
{

  int listen_sock, conn_sock; /* file descriptors */
  char recv_data[1024];
  int bytes_sent, bytes_received;
  struct sockaddr_in server; /* server's address information */
  struct sockaddr_in client; /* client's address information */

  int sin_size;

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

    printf("You got a connection from %s\n",inet_ntoa(client.sin_addr) ); /* prints client's IP */

    while (1){
      bytes_received = recv(conn_sock,recv_data,1024,0); //blocking
      if (bytes_received < 0){
        printf("\nError!Can not receive data from client!");
        close(conn_sock);
      } else if (bytes_received == 0){
        close(conn_sock);
        break;
      } else {
        recv_data[bytes_received] = '\0';
        converStr(recv_data);
        // printf("%s", recv_data);
      }

      bytes_sent = send(conn_sock,recv_data,1024,0);
      if (bytes_sent < 0){
        printf("\nError!Can not sent data to client!");
        close(conn_sock);
        continue;
      }
    }
  }

  close(listen_sock);
  return 0;
}