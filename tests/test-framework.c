
#include "test-framework.h"

#include <stdio.h>
#include <stdlib.h>

test_suite_t **test_suites;
int test_suite_count;
int current_test_passed;
int test_failed_line;

void init_test_framework()
{
  test_suite_count = 0;
  test_suites = malloc(1024 * sizeof(test_suite_t*));
}

void register_test(test_suite_t *suite)
{
  test_suites[test_suite_count++] = suite;
}

void run_all_tests()
{
  int i, j;

  for (i = 0; i < test_suite_count; ++i)
  {
    printf("Starting test suite %s\n", test_suites[i]->name);

    for (j = 0; test_suites[i]->tests[j] != NULL; ++j)
    {
      test_case_t *current_test = test_suites[i]->tests[j];
      printf("Test %s\n", current_test->name);
      current_test_passed = 1;
      current_test->callback();
      if (!current_test_passed)
      {
        printf("  Test failed %s on line %d\n", current_test->name, test_failed_line);
      }
      else
      {
        printf("  Test passed\n");
      }
    }
  }
}

int test_assert(int cond, int line)
{
  current_test_passed = (cond != 0);

  test_failed_line = line;

  return cond;
}