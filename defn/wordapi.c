#include "wordapi.h"
#include "curl/curl.h"
#include <stddef.h>

size_t word_api_cb(char *buffer, size_t size, size_t nmemb, void *datap) {
  size_t realsize = size * nmemb;
  word_api_data *data = (word_api_data *)datap;

  /* resize our buffer by the bytes to be collected and for \0 */
  char *ptr = realloc(data->response, data->size + realsize + 1);
  if (!ptr)
    return 0;

  /* append data to the end of malloc'ed memory */
  data->response = ptr;
  memcpy(&(data->response[data->size]), buffer, realsize);
  data->size += realsize;
  data->response[data->size] = '\0';

  return realsize;
}

void strskip(char *big, char *little, char skip) {
  char *src = little;
  char *dst = big;
  char c;
  while ((c = *src++) != '\0') {
    if (c != skip) {
      *dst++ = c;
    }
  }
}

char *get_defn(char *word) {
  CURL *curl = curl_easy_init();
  if (!curl) {
    fprintf(stderr, "libcurl init failed :(\n");
    return NULL;
  }
  /* create URL string */
  size_t url_size = strlen(WORD_API_URL) + INPUT_STR_MAX;
  char *url_str = malloc(url_size + 1);
  if (!url_str) {
    fprintf(stderr, "out of memory for URL");
  }
  strcpy(url_str, WORD_API_URL);
  strcat(url_str, word);
  /* perform API request */
  word_api_data data = {.size = 0, .response = NULL};
  curl_easy_setopt(curl, CURLOPT_URL, url_str);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, word_api_cb);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&data);
  CURLcode result = curl_easy_perform(curl);
  if (result != CURLE_OK) {
    fprintf(stderr, "libcurl failed to perform %s\n",
            curl_easy_strerror(result));
  }
  free(url_str);
  curl_easy_cleanup(curl);
  /* dodgily parse JSON */
  char *defn_start = strstr(data.response, SEARCH_STR);
  if (!defn_start) {
    fprintf(stderr, "no definition found for word %s\n", word);
    return NULL;
  }
  defn_start += strlen(SEARCH_STR);
  /* string artifact discovery */
  char *defn_end = defn_start;
  char c;
  uint32_t backslash_cnt = 0;
  while (defn_end - defn_start < DEFN_STR_MAX) {
    c = *defn_end++;
    if (c == '"') {
      if ((defn_end - defn_start == 1) || *(defn_end - 2) != '\\') {
        break;
      }
    } else if (c == '\\') {
      backslash_cnt++;
    }
  };
  size_t defn_size = defn_end - defn_start; /* includes null terminator */
  char *defn_str = malloc(defn_size);
  memcpy(defn_str, defn_start, defn_size - 1);
  defn_str[defn_size - 1] = '\0';
  /* remove any backslashes */
  defn_size -= backslash_cnt;
  char *defn_str_clean = malloc(defn_size);
  strskip(defn_str_clean, defn_str, '\\');
  defn_str_clean[defn_size - 1] = '\0';

  free(defn_str);
  free(data.response);
  return defn_str_clean;
};
