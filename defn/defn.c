#include "pthread.h"
#include "wordapi.h"

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
bool thread_running = false;

void *defn_thread_fn(void *arg) {
  char *str = (char *)arg;
  char *defn = get_defn(str);
  printf("definition: %s\n", defn);
  free(defn);

  pthread_mutex_lock(&mutex);
  thread_running = false;
  pthread_mutex_unlock(&mutex);
  return NULL;
}

int main(void) {
  char input[INPUT_STR_MAX + 1];
  char word[INPUT_STR_MAX + 1];
  pthread_t thread;

  while (1) {
    if (scanf("%" STRINGIFY(INPUT_STR_MAX) "s", input) != 1) {
      fprintf(stderr, "failed to read from stdin\n");
      return EXIT_FAILURE;
    }
    pthread_mutex_lock(&mutex);
    if (!thread_running) {
      strcpy(word, input);
      thread_running = true; // remove this to create new threads each time
      pthread_create(&thread, NULL, defn_thread_fn, &word[0]);
      pthread_detach(thread);
    } else {
      printf("currently fetching a word, please wait!\n");
    }
    pthread_mutex_unlock(&mutex);
  }
  return EXIT_SUCCESS;
}
