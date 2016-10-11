#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <string.h>

int main(){
  int client_sock;
  char buff[1024];
  struct sockaddr_in server_addr;
  int bytes_sent,bytes_received, sin_size;
  int total_bytes_sent = 0;
  char file_name[30];

  client_sock=socket(AF_INET,SOCK_DGRAM,0);

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(5551);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  sin_size = sizeof(struct sockaddr);

  printf("\nNhap ten file: \n> ");
  gets(file_name);

  FILE *sent_file = fopen(file_name, "r");
  FILE *receive_file = fopen("receive_file_client.txt", "w");

  if (sent_file == NULL){
    printf("File Not Found!\n");
    close(client_sock);
    exit(-1);
  }

  bytes_sent = sendto(client_sock,buff,strlen(buff),1024, (struct sockaddr *) &server_addr, sin_size);
  if(bytes_sent == -1){
    printf("\nError!Cannot send data to sever!\n");
    close(client_sock);
    exit(-1);
  }

  puts("Sending...");
  while (fgets(buff, 1024, sent_file)!=NULL){
    bytes_sent = sendto(client_sock,buff,strlen(buff),0, (struct sockaddr *) &server_addr, sin_size);
    if(bytes_sent == -1){
      printf("\nError!Cannot send data to sever!\n");
      close(client_sock);
      exit(-1);
    }

    total_bytes_sent += bytes_sent;
  }

  fclose(sent_file);

  //send 0 bytes to finish send file process
  bytes_sent = sendto(client_sock,buff,0,0, (struct sockaddr *) &server_addr, sin_size);
  if(bytes_sent == -1){
    printf("\nError!Cannot send data to sever!\n");
    close(client_sock);
    exit(-1);
  }

  puts("Loading...");

  while (1){
    bytes_received = recvfrom(client_sock,buff,1024,0, (struct sockaddr *) &server_addr, &sin_size);
    if(bytes_received == -1){
      printf("\nError!Cannot receive data from sever!\n");
      close(client_sock);
      exit(-1);
    } else if (bytes_received == 0){
      break;
    }
    buff[bytes_received] = '\0';
    fprintf(receive_file, "%s", buff);
  }

  fclose(receive_file);

  printf("Total bytes receive: %d\n", total_bytes_sent);
  printf("File's converted: receive_file_client.txt\n");
  close(client_sock);
  return 0;
}
