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
  int bytes_sent,bytes_received;

  client_sock=socket(AF_INET,SOCK_STREAM,0);

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(5500);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if(connect(client_sock,(struct sockaddr*)&server_addr,sizeof(struct sockaddr))!=0){
    printf("\nError!Can not connect to sever!Client exit imediately! ");
    return 0;
  }

  char user[30];
  char pass[30];


  while (1){
    printf("Nhap username:\n> ");
    gets(user);

    bytes_sent = send(client_sock,user,strlen(user),0);
    if(bytes_sent == -1){
        printf("\nError!Cannot send data to sever!\n");
        close(client_sock);
        exit(-1);
    }

    bytes_received = recv(client_sock,buff,1024,0);
    if(bytes_received == -1){
      printf("\nError!Cannot receive data from sever!\n");
      close(client_sock);
      exit(-1);
    }

    if (buff[0] == '0'){
      printf("Wrong username!\n\n");
    } else {
      break;
    }
  }

  int i=1;
  while (1){
    printf("Nhap password:\n> ");
    gets(pass);

    bytes_sent = send(client_sock,pass,strlen(pass),0);
    if(bytes_sent == -1){
        printf("\nError!Cannot send data to sever!\n");
        close(client_sock);
        exit(-1);
    }

    bytes_received = recv(client_sock,buff,1024,0);
    if(bytes_received == -1){
      printf("\nError!Cannot receive data from sever!\n");
      close(client_sock);
      exit(-1);
    }

    if (buff[0] == '0'){
      printf("Wrong password!\n");
      if (i==5){
        printf("Login Fail!\nQuiting....\n");
        close(client_sock);
        exit(-1);
      } else {
        i++;
      }
    } else {
      printf("Welcome!\n");
      break;
    }
  }

  char q[2];
  while (1){
    printf("Nhap q/Q de dang xuat:\n> ");
    gets(q);

    bytes_sent = send(client_sock,q,1,0);
    if(bytes_sent == -1){
        printf("\nError!Cannot send data to sever!\n");
        close(client_sock);
        exit(-1);
    }

    if (strcmp(q, "q")==0 || strcmp(q, "Q")==0){
      printf("Good bye!\n");
      break;
    }
  }



  close(client_sock);
  return 0;
}