#include <stdio.h>
#include <string.h>
#include "linkedlist.h"
#include "manager.h"
int main(){
  int choose;
  do {
    displayMenu();
    scanf("%d", &choose);
    switch (choose){
    case 1:
      createSubject();
      break;
    case 2:
      insertStudent();
      break;
    case 3:
      deleteStudent();
      break;
    case 4:
      searchStudent();
      break;
    case 5:
      summary();
      break;
    default:
      break;
    }

  } while (choose >= 1 && choose <= 5);
}