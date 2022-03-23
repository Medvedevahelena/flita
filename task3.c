#include <stdio.h>
#include <stdlib.h>

#define HEIGHT 13 
/*число вершин везвде одинаковое (5 шт.),
поэтому WIDTH не использую.*/

int main(){ 
  FILE *input, *output, *result;
    
  int c, top_num = 1, top_start = 0, top_stop = 0;
  int arr1[5][HEIGHT + 1];
  int arr2[HEIGHT + 1][5];
  
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
        if(c == '0' || c == '1'){
          arr1[i][j] = c;
          c = fgetc(input);
        }else{
          if(c == ' '){
            c = fgetc(input);
            j--;
            }
        }
      } 
    }
  }

  //транспонируем матрицу
  for(int i = 0; i <= HEIGHT; i++){
    for(int j = 0; j <= 4; j++){
      arr2[i][j] = arr1[j][i];
    }
  }

  //записываем матрицу во вспомогательный файл
  for(int i = 0; i <= HEIGHT; i++){
    for(int j = 0; j <= 4; j++){
      if(arr2[i][j] == '0' || arr2[i][j] == '1'){
        fputc(arr2[i][j], output);
      } 
    }
    fputc('\n', output);
  }
  
  fclose(input);
  fclose(output);

  output = fopen("output.txt", "r");
  result = fopen("result.gv", "w");
  
  fprintf(result, "%s\n", "graph graphname{");

  //составляем файл-инструкцию.
  c = fgetc(output);
  while(c != EOF){
    
    if(top_num == 6){
      if(top_start != 0){
        top_stop = top_start;
        fprintf(result, "%d%s%d%s\n", top_start, " -- ", top_stop, ";");
        top_start = top_stop = 0;
        top_num = 1;
      }else{
        top_num = 1;
      }
    }
    
    if(c == '1'){
      if(top_start != 0){
        top_stop = top_num;
        fprintf(result, "%d%s%d%s\n", top_start, " -- ", top_stop, ";");
        top_start = top_stop = 0;
        c = fgetc(output);
        top_num++;
      }else{
        top_start = top_num;
        c = fgetc(output);
        top_num++;
      }
    }else{
      if(c == '0'){
        c = fgetc(output);
        top_num++; 
      }else{
        c = fgetc(output);
      }
    }
  }

  fprintf(result, "%s", "}");
  
  fclose(output);
  fclose(result);

  system("dot result.gv -Tpng -o graf.png");
  system("graf.png"); 

  return 0;
  }
