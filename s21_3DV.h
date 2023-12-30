
#ifndef _3D_VUVER_H_
#define _3D_VUVER_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_OF_ARRAYS 3
#define OK 0
#define SOME_ERROR 1

typedef struct facets {
  int *vertexes;
  int numbers_of_vertexes_in_facets;
} polygon_t;

typedef struct data {
  unsigned int count_of_vertexes;
  unsigned int count_of_facets;
  double *vertex;
  polygon_t *polygons;
} t_data;

int count(char *filename, t_data *datas);
int input_polygons(char *str, t_data *datas, int poly_i);
int pars(char *filename, t_data *datas);
void move_matrix(double x, double y, double z, t_data *datas);
void remove_struct(t_data *datas);

#endif
