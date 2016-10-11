#include <stdio.h>          /* These are the usual header files */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>

#define PORT 5551  /* Port that will be opened */

void upperStr(char* str){
  int i;
  for (i=0; i<strlen(str); i++){
    str[i] = toupper(str[i]);
  }
}

int main()
{

  int server_sock; /* file descriptors */
  char recv_data[1024];
  int bytes_sent, bytes_received;
  struct sockaddr_in server; /* server's address information */
  struct sockaddr_in client; /* client's address information */
  int sin_size;

  FILE *f;

  if ((server_sock=socket(AF_INET, SOCK_DGRAM, 0)) == -1 ){  /* calls socket() */
    printf("socket() error\n");
    exit(-1);
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(PORT);   /* Remember htons() from "Conversions" section? =) */
  server.sin_addr.s_addr = INADDR_ANY;  /* INADDR_ANY puts your IP address automatically */
  bzero(&(server.sin_zero),8); /* zero the rest of the structure */


  if(bind(server_sock,(struct sockaddr*)&server,sizeof(struct sockaddr))==-1){ /* calls bind() */
    printf("bind() error\n");
    exit(-1);
  }


  while(1){
    sin_size=sizeof(struct sockaddr_in);

    bytes_received = recvfrom(server_sock,recv_data,1024,0, (struct sockaddr *) &client, &sin_size);

    if (bytes_received < 0){
      printf("\nError!Can not receive data from client!");
      continue;
    } else {
      printf("You got a connection!\n");
    }

    f = fopen("file_server.txt", "w");

    puts("Receiving file...");
    while (1){
      bytes_received = recvfrom(server_sock,recv_data,1024,0, (struct sockaddr *) &client, &sin_size);

      if (bytes_received < 0){
        printf("\nError!Can not receive data from client!");
      } else if (bytes_received == 0){
        printf("Finish convert file!\n");
        break;
      } else {
        recv_data[bytes_received] = '\0';
        upperStr(recv_data);
        fprintf(f, "%s", recv_data);
      }
    }
    fclose(f);

    sin_size=sizeof(client);
    f = fopen("file_server.txt", "r");

    puts("Sending file...");

    while (fgets(recv_data, 500, f)!=NULL){
      bytes_sent = sendto(server_sock,recv_data,strlen(recv_data),0,(struct sockaddr *) &client, sin_size ); /* send to the client welcome message */
      if (bytes_sent < 0){
        printf("\nError!Can not sent data to client!");
        break;
      }
    }

    //send 0 bytes to finish send file process
    bytes_sent = sendto(server_sock,recv_data,0,0,(struct sockaddr *) &client, sin_size ); /* send to the client welcome message */
      if (bytes_sent < 0){
        printf("\nError!Can not sent data to client!");
        break;
      }

    puts("Finish! Close connection!");
    puts("-------------------------\n");

    fclose(f);

  }

  close(server_sock);
  return 0;
}
