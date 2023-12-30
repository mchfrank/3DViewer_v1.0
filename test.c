#include <check.h>

#include "s21_3DV.h"
#define ACCURACY 1e-4

START_TEST(model_load_test_1) {
  t_data data = {0, 0, NULL, NULL};
  char file[20] = "test_empty.obj";
  pars(file, &data);
  t_data objTest = {0, 0, NULL, NULL};
  ck_assert_int_eq(data.count_of_facets, objTest.count_of_facets);
  ck_assert_int_eq(data.count_of_vertexes, objTest.count_of_vertexes);
  ck_assert_ptr_null(data.polygons);
  ck_assert_ptr_null(data.vertex);
  remove_struct(&data);
}

START_TEST(model_load_test_2) {
  t_data data = {0, 0, NULL, NULL};
  char file[20] = "test_trash.obj";
  pars(file, &data);
  t_data objTest = {0, 0, NULL, NULL};
  ck_assert_int_eq(data.count_of_facets, objTest.count_of_facets);
  ck_assert_int_eq(data.count_of_vertexes, objTest.count_of_vertexes);
  ck_assert_ptr_null(data.polygons);
  ck_assert_ptr_null(data.vertex);
  remove_struct(&data);
}

START_TEST(model_load_test_3) {
  t_data data = {0, 0, NULL, NULL};
  char file[20] = "cube.obj";
  pars(file, &data);
  t_data objTest = {8, 12, NULL, NULL};
  ck_assert_int_eq(data.count_of_facets, objTest.count_of_facets);
  ck_assert_int_eq(data.count_of_vertexes, objTest.count_of_vertexes);
  ck_assert_ptr_nonnull(data.polygons);
  ck_assert_ptr_nonnull(data.vertex);
  remove_struct(&data);
}

START_TEST(model_load_test_4) {
  t_data data = {0, 0, NULL, NULL};
  char file[20] = "pusto.obj";
  int result = pars(file, &data);
  ck_assert_int_eq(result, 1);
  remove_struct(&data);
}

START_TEST(error_test_1) {
  t_data data = {0, 0, NULL, NULL};
  char file[20] = "fff.obj";
  int result = pars(file, &data);
  ck_assert_int_eq(result, 2);
  remove_struct(&data);
}
START_TEST(move_test_1) {
  t_data data = {0, 0, NULL, NULL};
  char file[20] = "fff.obj";
  int result = pars(file, &data);
  ck_assert_int_eq(result, 2);
  move_matrix(1, 1, 1, &data);
  ck_assert_double_eq(data.vertex[3], 6);
  ck_assert_double_eq(data.vertex[4], 2);
  ck_assert_double_eq(data.vertex[5], 8);
  ck_assert_double_eq(data.vertex[6], 14);
  ck_assert_double_eq(data.vertex[7], 6);
  ck_assert_double_eq(data.vertex[8], 2);
  remove_struct(&data);
}

int main() {
  Suite *s1 = suite_create("3D Viewer Tests");
  TCase *tc1_1 = tcase_create("3D Viewer Tests");
  SRunner *sr = srunner_create(s1);
  double result = 0;

  suite_add_tcase(s1, tc1_1);

  tcase_add_test(tc1_1, model_load_test_1);
  tcase_add_test(tc1_1, model_load_test_2);
  tcase_add_test(tc1_1, model_load_test_3);
  tcase_add_test(tc1_1, model_load_test_4);
  tcase_add_test(tc1_1, error_test_1);
  tcase_add_test(tc1_1, move_test_1);

  srunner_run_all(sr, CK_ENV);
  result = srunner_ntests_failed(sr);
  srunner_free(sr);
  return result == 0 ? 0 : 1;
}
