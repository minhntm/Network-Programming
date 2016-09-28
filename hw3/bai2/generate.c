#include <stdio.h>
int main(){
  int i, j, k=0;
  FILE *f = fopen("file", "w");
  for (i=0; i<10000; i++){
    for (j=0; j<20; j++){

	printf("%d\n", k++);
      fprintf(f, " %s", "abcdef ghikl 123");
    }
    fprintf(f, "\n");
  }
  fclose(f);
}
