#include <stdio.h>

#define HEIGHT 13 
/* число вершин везде одинаковое (5 шт.),
поэтому WIDTH не инициализирую */

int main(){ 
  FILE *input, *output, *result;
    
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

  /* Кусочек теории: Связный граф — граф, между любой парой вершин которого существует как минимум один путь. 
  Следствие: Любой граф на n вершинах, имеющий меньше, чем n−1 ребро, обязательно является несвязным. В случае, 
  когда количество ребер в графе больше или равно n−1, граф может быть как связным, так и несвязным. 
    Пусть n>1. Рассмотрим несвязный граф, в котором одна вершина ни с чем не соединена, а остальные соединены попарно. 
  Тогда в графе (n−1)(n−2)/2 рёбер, и он не связен. Если количество рёбер увеличить на единицу, то их получится (n−1)(n−2)/2+1, 
  и здесь уже связность графа гарантирована. Поэтому, если граф имеет больше рёбер, чем (n−1)(n−2)/2, то он является связным.
  */

  //определяем связный ли граф
  int a, tops = 1, edges = 0;
  input = fopen("input.txt", "r");

  c = fgetc(input);
  while(c != '\n'){
    if(c == '0' || c == '1'){
      edges++;
    }
    c = fgetc(input);
  }
  while(c != EOF){
    if(c == '\n'){
      tops++;
    }
    c = fgetc(input);
  }
  
  fclose(input);

  a = (tops - 1)*(tops - 2)/2;
  
  if(a < edges - loops){
    printf("Connected graph.\n"); //граф связный
  }else{
    printf("Disconnected graph.\n"); //граф не связный
  }
  
  return 0;
}
