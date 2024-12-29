#include "wordapi.h"

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)

int main(void) {
  char *str = malloc(INPUT_STR_MAX);
  scanf("%" STRINGIFY(INPUT_STR_MAX) "s", str);
  char *defn = get_defn(str);
  printf("definition: %s\n", defn);
  free(str);
  free(defn);
}
