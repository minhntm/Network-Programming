#ifndef manager_h
#define manager_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"
#define printSplit printf("\n------------------------------------\n");

typedef struct {
  char subjectID[10];
  char subjectName[30];
  int midTerm;
  int finallTerm;
  char termID[10];
  int numberStudent;
} Subject;

void displayMenu();
void createSubject();
void getSubjectAndTermID(char* subjectID, char* termID);
void getFileName(char* subjectID, char* termID, char* fileName);
void saveSubject(Subject subj, char *fileName);
void saveStudent(Student* student, char* fileName);
void continueThisFunction(void (*f)(void));
void insertStudent();
int loadSubjectInfor(char* fileName, Subject* subj);
int loadStudentList(studentList, fileName);
void split(char* s,char* splitStr, char* result);
void deleteStudent();
void getStudentData(char* s, Student* student);
void searchStudent();
void printStudentScore(Student* student);
void summary();
/*-----------------------------------------------------------------*/
void displayMenu(){
  printSplit;
  printf("Learning Management System\n");
  printf("1. Add a new score board\n");
  printf("2. Add score\n");
  printf("3. Remove score\n");
  printf("4. Search score\n");
  printf("5. Display score board and score report\n");
  printf("> Your choice (1-5, other to quit):\n");
  printf("> ");
}
/*-----------------------------------------------------------------*/
void createSubject(){
  Subject subj;

  printSplit;
  while (getchar()!='\n');
  getSubjectAndTermID(subj.subjectID, subj.termID);

  printf("Subject name: ");
  gets(subj.subjectName);

  printf("He so giua ki: ");
  scanf("%d", &subj.midTerm);

  printf("He so cuoi ki: ");
  scanf("%d", &subj.finallTerm);

  printf("So luong sinh vien: ");
  scanf("%d", &subj.numberStudent);
  while (getchar()!='\n');

  char fileName[30];
  getFileName(subj.subjectID, subj.termID, fileName);
  saveSubject(subj, fileName);
  continueThisFunction(createSubject);
}
/*-----------------------------------------------------------------*/
void getSubjectAndTermID(char* subjectID, char* termID){
  printf("Subject ID: ");
  gets(subjectID);
  printf("Term ID: ");
  gets(termID);
}
/*-----------------------------------------------------------------*/
void getFileName(char* subjectID, char* termID, char *fileName){
  strcpy(fileName, subjectID);
  strcat(fileName, "_");
  strcat(fileName, termID);
  strcat(fileName, ".txt");
}
/*-----------------------------------------------------------------*/
void saveSubject(Subject subj, char *fileName){
  FILE *f = fopen(fileName, "w");
  fprintf(f, "SubjectID|%s\n", subj.subjectID);
  fprintf(f, "Subject|%s\n", subj.subjectName);
  fprintf(f, "F|%d|%d\n", subj.midTerm, subj.finallTerm);
  fprintf(f, "Semester|%s\n", subj.termID);
  fprintf(f, "StudentCount|%d\n", subj.numberStudent);
  fclose(f);
}
/*-----------------------------------------------------------------*/
void continueThisFunction(void (*f)(void)){
  char ans;

  printSplit;
  printf("Continue This Function? (Y/N):\n> ");

  scanf("%c", &ans);
  if (ans == 'y' || ans == 'Y'){
    f();
  }
}
/*-----------------------------------------------------------------*/
void insertStudent(){
  char subjectID[10];
  char termID[10];
  Student student;
  Subject subj;
  char fileName[20];

  printSplit;
  while (getchar()!='\n');
  getSubjectAndTermID(subjectID, termID);
  getFileName(subjectID, termID, fileName);
  if (loadSubjectInfor(fileName, &subj) == 1){
    continueThisFunction(insertStudent);
    return;
  }

  printf("Student ID: ");
  gets(student.id);
  printf("Ten: ");
  gets(student.firstName);
  printf("Ho va dem: ");
  gets(student.lastName);
  printf("Diem giua ki: ");
  scanf("%f", &student.midTermScore);
  printf("Diem cuoi ki: ");
  scanf("%f", &student.finallTermScore);

  float score;
  score = (student.midTermScore*subj.midTerm+student.finallTermScore*subj.finallTerm)/(subj.midTerm+subj.finallTerm);
  if (score < 4.0){
    student.charScore = 'F';
  } else if (score < 5.5){
    student.charScore = 'D';
  } else if (score < 7.0){
    student.charScore = 'C';
  } else if (score < 8.5){
    student.charScore = 'B';
  } else {
    student.charScore = 'A';
  }


  saveStudent(&student, fileName);
  while (getchar()!='\n');
  continueThisFunction(insertStudent);
}
/*-----------------------------------------------------------------*/
int loadSubjectInfor(char* fileName, Subject* subj){
  char s[50];
  char temp[10];
  const char splitStr[]= "|";

  FILE *f = fopen(fileName, "r");

  if (f == NULL){
    printf("File Not Found!");
    return 1;
  }

  fgets(s, 50, f);
  split(s, splitStr, subj->subjectID);
  fgets(s, 50, f);
  split(s, splitStr, subj->subjectName);
  fgets(s, 50, f);

  char* str = NULL;
  str = strtok(s, splitStr);
  str = strtok(NULL, splitStr);
  subj->midTerm = atoi(str);
  str = strtok(NULL, splitStr);
  subj->finallTerm = atoi(str);

  fgets(s, 50, f);
  split(s, splitStr, subj->termID);
  fgets(s, 50, f);
  split(s, splitStr, temp);
  subj->numberStudent = atoi(temp);
  fclose(f);
  return 0;
}
/*-----------------------------------------------------------------*/
void split(char* s,char* splitStr, char* result){
  char* str = NULL;
  str = strtok(s, splitStr);
  str = strtok(NULL, splitStr);
  str[strlen(str)-1] = '\0';
  strcpy(result, str);
}
/*-----------------------------------------------------------------*/
void saveStudent(Student* student, char* fileName){
  FILE *f = fopen(fileName, "a");
  fseek(f, 0, SEEK_END);
  fprintf(f, "S|%s|%s|%s|%-.1f|%-.1f|%c|\n",
          student->id, student->lastName, student->firstName,
          student->midTermScore, student->finallTermScore, student->charScore);
  fclose(f);
}
/*-----------------------------------------------------------------*/
void deleteStudent(){
  char subjectID[10];
  char termID[10];
  char studentID[10];
  Node *studentList = NULL;
  Subject subj;
  char fileName[20];

  printSplit;
  while (getchar()!='\n');
  getSubjectAndTermID(subjectID, termID);
  getFileName(subjectID, termID, fileName);
  if (loadSubjectInfor(fileName, &subj) == 1){
    continueThisFunction(deleteStudent);
    return;
  }

  loadStudentList(&studentList, fileName);

  printf("Student ID: ");
  gets(studentID);

  delNode(&studentList, studentID);
  saveSubject(subj, fileName);

  Node *p;
  for (p=studentList; p!= NULL; p=p->next){
    saveStudent(p->data, fileName);
  }
  freeList(&studentList);
  continueThisFunction(deleteStudent);
}
/*-----------------------------------------------------------------*/
int loadStudentList(Node** studentList, char* fileName){
  FILE *f = fopen(fileName, "r");
  Student student;
  char s[50];

  if (f == NULL){
    printf("File Not Found!\n");
    return 1;
  }

  do {
    fgets(s, 50, f);
    if (s[0] == 'S' && s[1] == '|'){
      break;
    }
  } while(1);

  do {
    getStudentData(s, &student);
    addSort(studentList, makeNode(student));
  } while (fgets(s, 50, f)!=NULL);
  return 0;
}
/*-----------------------------------------------------------------*/
void getStudentData(char* s, Student* student){
  const char splitStr[] = "|";
  char* str = NULL;

  str = strtok(s, splitStr);
  str = strtok(NULL, splitStr);
  strcpy(student->id, str);
  str = strtok(NULL, splitStr);
  strcpy(student->lastName, str);
  str = strtok(NULL, splitStr);
  strcpy(student->firstName, str);
  str = strtok(NULL, splitStr);
  student->midTermScore = atof(str);
  str = strtok(NULL, splitStr);
  student->finallTermScore = atof(str);
  str = strtok(NULL, splitStr);

  int i;
  for (i=0; i<strlen(str); i++){
    if (str[i]!=' '){
      student->charScore = str[i];
      break;
    }
  }
}
/*-----------------------------------------------------------------*/
void searchStudent(){
  char subjectID[10];
  char termID[10];
  char studentID[10];
  Node *studentList = NULL;
  char fileName[20];

  printSplit;
  while (getchar()!='\n');
  getSubjectAndTermID(subjectID, termID);
  getFileName(subjectID, termID, fileName);

  if (loadStudentList(&studentList, fileName) == 1){
    continueThisFunction(searchStudent);
    return;
  }

  printf("Student ID: ");
  gets(studentID);

  Node* node;
  node = searchNode(studentList, studentID);
  if (node == NULL){
    printf("Student Not Found!");
  } else {
    printStudentScore(node->data);
  }

  freeList(&studentList);
  continueThisFunction(searchStudent);
}
/*-----------------------------------------------------------------*/
void printStudentScore(Student* student){
  printf("%-10s%-25s%-8s%-5.1f%-5.1f%-5c\n",
         student->id, student->lastName, student->firstName,
         student->midTermScore, student->finallTermScore, student->charScore);
}
/*-----------------------------------------------------------------*/
void summary(){
  char subjectID[10];
  char termID[10];
  Node *studentList = NULL;
  Subject subj;
  char fileName[20];
  char reportFileName[20];
  int numAScore = 0;
  int numBScore = 0;
  int numCScore = 0;
  int numDScore = 0;
  int numFScore = 0;

  printSplit;
  while (getchar()!='\n');
  getSubjectAndTermID(subjectID, termID);
  getFileName(subjectID, termID, fileName);
  loadSubjectInfor(fileName, &subj);

  strcpy(reportFileName, subjectID);
  strcat(reportFileName, "_");
  strcat(reportFileName, termID);
  strcat(reportFileName, "_");
  strcat(reportFileName, "rp");
  strcat(reportFileName, ".txt");

  if (loadStudentList(&studentList, fileName) == 1){
    continueThisFunction(summary);
    return;
  }

  Node* p;
  Node* highestMark = studentList;
  Node* lowestMark = studentList;
  int count = 0;
  float sum = 0;
  for (p=studentList; p!= NULL; p=p->next){
    count++;
    float score = (p->data->midTermScore*subj.midTerm+p->data->finallTermScore*subj.finallTerm)/(subj.midTerm+subj.finallTerm);
    sum+=score;

    if (score > (highestMark->data->midTermScore*subj.midTerm+highestMark->data->finallTermScore*subj.finallTerm)/(subj.midTerm+subj.finallTerm)){
      highestMark = p;
    }

    if (score < (lowestMark->data->midTermScore*subj.midTerm+lowestMark->data->finallTermScore*subj.finallTerm)/(subj.midTerm+subj.finallTerm)){
      lowestMark = p;
    }

    switch (p->data->charScore){
      case 'A': numAScore++;
        break;
      case 'B': numBScore++;
        break;
      case 'C': numCScore++;
        break;
      case 'D': numDScore++;
        break;
      case 'F': numFScore++;
        break;
    }

  }

  FILE *f = fopen(reportFileName, "w");
  fprintf(f, "The student with the highest mark is: %s %s\n", highestMark->data->lastName, highestMark->data->firstName);
  fprintf(f, "The student with the lowest mark is: %s %s\n", lowestMark->data->lastName, lowestMark->data->firstName);
  fprintf(f, "The average mark is: %-.2f\n", sum/count);
  fprintf(f, "A histogram of the subject %s is: \n", subj.subjectID);

  int i;
  fprintf(f, "A:");
  for (i=0; i<numAScore; i++){
    fprintf(f, "*");
  }
  fprintf(f, "\n");

  fprintf(f, "B:");
  for (i=0; i<numBScore; i++){
    fprintf(f, "*");
  }
  fprintf(f, "\n");

  fprintf(f, "C:");
  for (i=0; i<numCScore; i++){
    fprintf(f, "*");
  }
  fprintf(f, "\n");

  fprintf(f, "D:");
  for (i=0; i<numDScore; i++){
    fprintf(f, "*");
  }
  fprintf(f, "\n");

  fprintf(f, "F:");
  for (i=0; i<numFScore; i++){
    fprintf(f, "*");
  }
  fprintf(f, "\n");
  fclose(f);


  for (p=studentList; p!= NULL; p=p->next){
    printStudentScore(p->data);
  }


  freeList(&studentList);
  continueThisFunction(summary);
}
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/



#endif