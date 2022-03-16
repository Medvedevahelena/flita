#include <stdio.h>

#define WIDTH 11
#define HEIGHT 50

int main(){ 
  FILE *input, *output, *result;
    
  int c, from = 0, top = 1, last;
  int arr1[55][55];
  int arr2[55][55];
  
  input = fopen("input.txt", "r");
  output = fopen("output.txt", "w");

  //записываем матрицу в двумерный массив
  c = fgetc(input);
  for(int i = 0; c != EOF; i++){
    for(int j = 0; c != EOF; j++){
      if(c == '\n'){
        c = fgetc(input);
        break;
      }else{
        if(c != ' '){
          arr1[i][j] = c;
          c = fgetc(input);
        }else{
            c = fgetc(input);
        }
      } 
    }
  }

  //транспонируем матрицу
  for(int i = 0; i <= HEIGHT; i++){
    for(int j = 0; j <= WIDTH; j++){
      arr2[i][j] = arr1[j][i];
    }
  }

  //записываем матрицу во вспомогательный файл
  for(int i = 0; i <= HEIGHT; i++){
    for(int j = 0; j <= WIDTH; j++){
      fputc(arr2[i][j], output);
    } 
    fputc('\n', output);
  }
  
  fclose(input);
  fclose(output);

  output = fopen("output.txt", "r");
  result = fopen("result.txt", "w");
  
  fprintf(result, "%s\n", "digraph test{");

  //формируем файл для передачи в графический редактор
  c = fgetc(output);
  while(c != EOF){
    last = c;
    if(c == '0' || c == '1'){
      if(c == '1' && from == 0){
        from = top;
        top++;
        c = fgetc(output);
    }else{
      if(c == '1' && from != 0){
        if(top > from + 4){ //обрабатываем петли
          top = from;
          fprintf(result, "%d%s%d%s\n", from, "->", top, ";");
          from = 0;
          top = 0;
          c = fgetc(output);
        }else{
          fprintf(result, "%d%s%d%s\n", from, "->", top, ";");
          from = 0;
          top = top - 4;
          c = fgetc(output);
        }
      }else{
        if(c == '0'){
          top++;
          c = fgetc(output);
        }
      } 
    }
    }else{
      c = fgetc(output);
    }
  }

  //обрабатываем последний символ
  if(from != 0){
    fprintf(result, "%d%s%d%s\n", from, "->", from, ";");
  }
  fprintf(result, "%s", "}");
  
  return 0;
}
