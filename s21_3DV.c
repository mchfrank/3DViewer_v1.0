#include "s21_3DV.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void remove_struct(t_data *datas) {
  if (datas->vertex != NULL) {
    free(datas->vertex);
    printf("освободил массив вершин\n");
  }
  if (datas->polygons != NULL) {
    for (unsigned int i = 0; i < datas->count_of_facets + 1; i++) {
      free(datas->polygons[i].vertexes);
      printf("освободил массив вершин текуущего полигона\n");
    }
    free(datas->polygons);
    printf("освободил массив полигонов\n");
  }
}

int input_polygons(char *str, t_data *datas, int poly_i) {
  int flag = 0, f = 0, error = 0;

  for (int i = 2; str[i] != '\0'; i++) {
    if (flag && strchr(" ", str[i])) flag = 0;
    if (strchr("-1234567890/", str[i]) && !flag) {
      datas->polygons[poly_i].numbers_of_vertexes_in_facets++;
      flag = 1;
    }
  }

  datas->polygons[poly_i].vertexes =
      calloc(datas->polygons[poly_i].numbers_of_vertexes_in_facets,
             sizeof(datas->polygons[poly_i].vertexes));
  if (!datas->polygons[poly_i].vertexes) {
    error = 1;
  } else {
    flag = 0;
    for (int i = 2; str[i] != '\0'; i++) {
      if (flag && strchr(" ", str[i])) flag = 0;
      if (strchr("-1234567890", str[i]) && !flag) {
        int index = atof(str + i);
        if (strchr("-", str[i])) {
          index = 0;
          error = 2;
        }
        datas->polygons[poly_i].vertexes[f] = index;
        flag = 1;
        f++;
      }
    }
  }
  return error;
}

int count(char *filename, t_data *datas) {
  int error = 0;
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    error = 1;
  } else {
    datas->count_of_vertexes = 0;
    datas->count_of_facets = 0;
    char str[1025] = "";
    while (fgets(str, 1024, file) != NULL) {
      if (str[0] == 'v' && str[1] == ' ') datas->count_of_vertexes++;
      if (str[0] == 'f' && str[1] == ' ') datas->count_of_facets++;
    }
  }
  fclose(file);
  return error;
}

int pars(char *filename, t_data *datas) {
  int error = count(filename, datas);
  if (datas->count_of_vertexes == 0 && !error) error = 3;
  if (!error) {
    datas->vertex =
        calloc(datas->count_of_vertexes * 3 + 3, sizeof(datas->vertex));
    datas->polygons = calloc(datas->count_of_facets + 1, sizeof(polygon_t));
    FILE *file = fopen(filename, "r");
    if (file == NULL || !datas->polygons || error) {
      error = 1;
    } else {
      int polygon_i = 1;
      int i = 3;
      char str[1025] = "";
      while (fgets(str, 1024, file) != NULL && error != 1) {
        if (str[0] == 'v' && str[1] == ' ') {
          int flag = 0;
          for (int k = 2; str[k] != '\0'; k++) {
            if (flag && strchr(" ", str[k])) flag = 0;
            if (strchr("-1234567890", str[k]) && !flag) {
              datas->vertex[i] = atof(str + k);
              flag = 1;
              i++;
            }
          }
        }
        if (str[0] == 'f' && str[1] == ' ') {
          error = input_polygons(str, datas, polygon_i);
          polygon_i++;
        }
      }
    }
  }
  return error;
}

void move_matrix(double x, double y, double z, t_data *datas) {
  int f = 0;
  for (unsigned int i = 3; i <= datas->count_of_vertexes * 3 + 2; i++) {
    if (f == 0) {
      datas->vertex[i] += x;
      f++;
    } else if (f == 1) {
      datas->vertex[i] += y;
      f++;
    } else {
      datas->vertex[i] += z;
      f = 0;
    }
  }
}
