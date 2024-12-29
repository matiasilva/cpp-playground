#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_STR_MAX 39
#define DEFN_STR_MAX 500
#define WORD_API_URL "https://api.dictionaryapi.dev/api/v2/entries/en/"
#define SEARCH_STR "definition\":\""

typedef struct {
  size_t size;
  char *response;
} word_api_data;

char *get_defn(char *word);
