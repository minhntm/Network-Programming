#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int checkInput(char* name){
  int i;
  for (i=0; i<strlen(name); i++){
    if ((name[i] < 48 || name[i] > 57) && name[i]!='.')
      return 0;
  }
  return 1;
}

main(){
  char input[30];
  printf("Nhap dia chi IP hoac ten trang web: \n");
  printf("> ");
  gets(input);

  if (checkInput(input) == 1){
    //input is IP
    struct hostent *a;
    struct in_addr **b;
    struct in_addr ipv4;
    struct in6_addr ipv6;
    struct in_addr *add;

    int i;
    if (inet_pton(AF_INET, input, &ipv4)){
      a = gethostbyaddr(&ipv4, sizeof(ipv4), AF_INET);
      if (a!=NULL)
        printf("Web: %s\n", a->h_name);
      else
        printf("Wrong input!\n");
    } else {
      printf("Wrong input!\n");
      return;
    }

  } else {
    struct hostent *a;
    struct in_addr **b;
    int i;

    a = gethostbyname(input);
    if (a == NULL){
      printf("Wrong domain!\n");
      return;
    }

    b = (struct in_addr**)a->h_addr_list;
    printf("IP: %s\n", inet_ntoa(*b[0]));
    for (i=1; b[i]!=NULL; i++){
      printf("IP: %s\n", inet_ntoa(*b[i]));
    }
  }

  return 0;
}

