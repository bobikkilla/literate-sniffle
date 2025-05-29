#include "test.h"

int main() {
  Suite *suite;
  SRunner *sr;
  suite = string_suite();
  sr = srunner_create(suite);

  srunner_set_fork_status(sr, CK_NOFORK);

  srunner_run_all(sr, CK_NORMAL);
  srunner_free(sr);
  return 0;
}