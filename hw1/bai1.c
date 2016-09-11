#include <stdio.h>
#include <string.h>

typedef struct secondCharData{
  char* react;
  char* protect;
  char* contain;
} SecondCharData;

char* getMaterial(char firstChar);
SecondCharData* getSecondCharData(int secondChar, int color);
char* getEvacuation(char thirdChar);
char* getReact(int react);
char* getProtect(int protect);
char* getContain(int contain);
void printNotify(char* material,
                 SecondCharData* secondCharData,
                 char* evacuation);
void printError();

int main(){
  char hazCode[4];
  char *material;
  char *evacuation;
  SecondCharData *secondCharData;
  printf("Enter HAZCHEM code: \n");
  printf("> ");
  gets(hazCode);
  if (strlen(hazCode) > 3){
    printError();
    return 1;
  }


  material = getMaterial(hazCode[0]);
  evacuation = getEvacuation(hazCode[2]);
  if (material == NULL || evacuation == NULL){
    printError();
    return 1;
  }


  if (hazCode[1] == 'S' || hazCode[1] == 'T' ||
      hazCode[1] == 'Y' || hazCode[1] == 'Z'){
    printf("Is the %c reverse coloured?\n", hazCode[1]);
    printf("> ");
    char reverse[10];
    gets(reverse);

    if (strcmp(reverse, "yes") == 0){
      secondCharData = getSecondCharData(hazCode[1], 1);
    } else {
      secondCharData = getSecondCharData(hazCode[1], 0);
    }
  } else {
    secondCharData = getSecondCharData(hazCode[1], 2);
  }

  if (secondCharData == NULL){
    printError();
    return 1;
  }

  printNotify(material, secondCharData, evacuation);
  return 0;

}

void printError(){
  printf("Error HAZCHEM code!!!\n");
}

void printNotify(char* material,
                 SecondCharData* secondCharData,
                 char* evacuation){

  printf("\n\n\n***Emergency action advice***\n");
  printf("Material: %s\n", material);
  printf("Reactivity: %s\n", secondCharData->react);
  printf("Protection: %s\n", secondCharData->protect);
  printf("Containment: %s\n", secondCharData->contain);
  printf("Evacuation: %s\n", evacuation);
  printf("*********************************\n\n\n");
}

char *getMaterial(char firstChar){
  switch(firstChar){
    case '1': return "Jets";
    case '2': return "Fog";
    case '3': return "Foam";
    case '4': return "Dry agent";
    default: return NULL;
  }
}

SecondCharData* getSecondCharData(int secondChar, int color){
  /**
  color = 2 if don't know color
  color = 0 if reverse coloured
  color = 1 if don't reverse coloured
  */

  SecondCharData* data =
        (SecondCharData*)malloc(sizeof(SecondCharData));

  switch(secondChar){
    case 'P': data->react = getReact(1);
              data->protect = getProtect(1);
              data->contain = getContain(1);
              break;
    case 'R': data->react = getReact(0);
              data->protect = getProtect(1);
              data->contain = getContain(1);
              break;
    case 'S': data->react = getReact(1);
              data->contain = getContain(1);
              if (color){
                data->protect = getProtect(3);
              } else {
                data->protect = getProtect(2);
              }
              break;
    case 'T': data->react = getReact(0);
              data->contain = getContain(1);
              if (color){
                data->protect = getProtect(3);
              } else {
                data->protect = getProtect(2);
              }
              break;
    case 'W': data->react = getReact(1);
              data->contain = getContain(2);
              data->protect = getProtect(1);

              break;
    case 'X': data->react = getReact(0);
              data->contain = getContain(2);
              data->protect = getProtect(1);

              break;
    case 'Y': data->react = getReact(1);
              data->contain = getContain(2);
              if (color){
                data->protect = getProtect(3);
              } else {
                data->protect = getProtect(2);
              }
              break;
    case 'Z': data->react = getReact(1);
              data->contain = getContain(2);
              if (color){
                data->protect = getProtect(3);
              } else {
                data->protect = getProtect(2);
              }
              break;
    default: free(data);
              data = NULL;
  }
  return data;
}

char* getEvacuation(char thirdChar){
  if (thirdChar == 'E'){
    return "consider evacuation";
  } else if (thirdChar == '\0'){
    return "do not consider evacuation";
  } else {
    return NULL;
  }
}

char* getReact(int react){
  if (react == 1){
    return "can be violently reactive";
  } else {
    return "not be violently reactive";
  }
}

char* getProtect(int protect){
  if (protect == 1){
    return "full protection";
  } else if (protect == 2){
    return "breathing apparatus, protective gloves";
  } else if (protect == 3){
    return "breathing apparatus, protective gloves for fire only";
  }
}

char* getContain(int contain){
  if (contain == 1){
    return "may be diluted and washed down the drain";
  } else {
    return "do not washed down the drain";
  }
}