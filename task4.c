#include <stdio.h>
#include <string.h>

#define HEIGHT 13 
/* число вершин везде одинаковое (5 шт.),
поэтому WIDTH не инициализирую */

int main(){ 
  FILE *input, *output, *result, *output1;
    
  int c, top_num = 1, top_start = 0, top_stop = 0, loops = 0;
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

  //транспонируем матрицу (на самом деле этого можно было отдельно не делать)
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
  
  for(int i = 1; i <= 5; i++){
    fprintf(result, "%d%s\n", i, ";");
  }

  //составляем файл-инструкцию
  c = fgetc(output);
  while(c != EOF){
    
    if(top_num == 6){
      if(top_start != 0){
        top_stop = top_start;
        fprintf(result, "%d%s%d%s\n", top_start, " -- ", top_stop, ";");
        top_start = top_stop = 0;
        top_num = 1;
        loops++;
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

  //определяем связный ли граф

  //определяем, есть ли в графе свободные вершины (вершина является свободной, даже если имеет петлю, и это мы учитываем)
  
  input = fopen("input.txt", "r");

  int single_top = 0, marker = 0;
  int c1 = 0, c2 = 0;
  int arr_loops[5] = {0,0,0,0,0}, arr_edges[5] = {0,0,0,0,0};

  //для каждой вершины определяем, сколько у неё петель, а сколько нормальных (здесь рёбра ещё могут повторяться) рёбер, и на основе этого вычисляем количество свободных вершин графа

    for(int j = 0; j <= HEIGHT; j++){
      for(int i = 0; i <= 4; i++){
        if(arr1[i][j] == '1'){
          if(c1 == 0){
            c1 = i;
            marker++;
          }else{
            c2 = i;
            marker++;
          }
        }
      }
      if(marker == 1){
        arr_loops[c1]++;
        c1 = 0;
      }if(marker == 2){
        arr_edges[c1]++;
        arr_edges[c2]++;
        c1 = c2 = 0;
      }
      marker = 0;
  }

/*
  for(int i = 0; i <= 4; i++){
    printf("%d ", arr_loops[i]);
    printf("%d ", arr_edges[i]);
    printf("%s", "\n");
  }
*/

  //определяем количесво свободных вершин
    for(int i = 0; i <= 4; i++){
      if((arr_loops[i] != 0 && arr_edges[i] == 0) || (arr_loops[i] == 0 && arr_edges[i] == 0)){
        single_top++;
      }
    }

  output = fopen("output.txt", "r");
  output1 = fopen("output1.txt", "w");

  //printf("%d\n", single_top);

  if(single_top == 0){

    //удаляем повторяющиеся рёбра
    int flag = 0;
    for(int i = 0; i <= HEIGHT; i++){
      for(int k = 1; k <= HEIGHT - i + 1; k++){
        for(int j = 0; j <= 4; j++){
          if(arr2[i][j] == arr2[i + k][j]){
            flag++;
          }
        }
        if(flag == 5){
          for(int j = 0; j <= 4; j++){
            arr2[i + k][j] = '0';
          }
        } 
        flag = 0;
      }
    }
    
    //записываем эту свеженькую матрицу во вспомогательный файл
    for(int i = 0; i <= HEIGHT; i++){
      for(int j = 0; j <= 4; j++){
        if(arr2[i][j] == '0' || arr2[i][j] == '1'){
          fputc(arr2[i][j], output1);
        } 
      }
      fputc('\n', output1);
    }

    //теперь из файла output1 удалим строки типа "00000" и "00100", т.е. не будем считать петли и рёбра, которых не существует;работаем не с файлом, а с матрицей, а потом её записываем в файл

    fclose(output1);
    output1 = fopen("output1.txt", "w");

    int marker_new = 0;

    //записываем эту матрицу во вспомогательный файл
    for(int i = 0; i <= HEIGHT; i++){
      for(int j = 0; j <= 4; j++){
        if(arr2[i][j] == '0'){
          marker_new++;
        }
      }
      if(marker_new < 4){
        for(int j = 0; j <= 4; j++){
          if(arr2[i][j] == '1'){
            fputc(arr2[i][j], output1);
          }
          }
      }
      marker_new = 0;
      fputc('\n', output1);
    }
    
    fclose(output1);
    output1 = fopen("output1.txt", "r");

    //теперь считаем, сколько адекватных (не повторяющихся и не являющихся петлями) рёбер у нашего графа
    int flag_new = 0, tops = 0;

    c = fgetc(output1);
    while(c != EOF){
      if(c == '1'){
        flag_new++;
      }
      c = fgetc(output1);
    }

    fclose(output1);
    //printf("%d\n", flag_new/2);

    //вычисляем число вершин графа, у которого нет свободных вершин (хотя и так уже понятно, что их 5)
    input = fopen("input.txt", "r"); 
    c = fgetc(input);
    while(c != EOF){
      if(c == '\n'){
        tops++;
      }
      c = fgetc(input);
    }

    fclose(input);
    //printf("%d\n", tops);

    //проверяем граф на связность
    if(flag_new/2 < (tops - 1)){
      printf("Disconnected graph.\n");
    }else{
      printf("Connected graph.\n");
    }
  }else{
    printf("Disconnected graph.\n");
  }
  
  return 0;
}
