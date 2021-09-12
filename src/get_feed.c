/*
 *    This file is part of popChess.
 *
 *    popChess is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    popChess is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with popChess.  If not, see <https://www.gnu.org/licenses/>.
 */

// curl https://lichess.org/api/tv/feed
#include "get_feed.h"

void initCurl() {
  pthread_t id;
  int j = 1;
  pthread_create(&id, NULL, curlHandler, &j);
}
void curlHandler() {
  CURL *curl_handle;
  CURLcode res;

  struct mem_struct_t chunk;
  chunk.memory = malloc(1);
  chunk.size = 0;

  char *website_addr = "https://lichess.org/api/tv/feed";

  curl_handle = curl_easy_init();
  if (curl_handle) {
    curl_easy_setopt(curl_handle, CURLOPT_URL, website_addr);
    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    res = curl_easy_perform(curl_handle);

    if (res != CURLE_OK) {
      fprintf(stderr, "error: %s\n", curl_easy_strerror(res));
    } else {
      printf("Size: %lu\n", (unsigned long)chunk.size);
      printf("Data: %s\n", chunk.memory);
    }
    curl_easy_cleanup(curl_handle);
    free(chunk.memory);
  }
  return 0;
}

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb,
                           void *userp) {
  size_t realsize = size * nmemb;
  struct mem_struct_t *mem = (struct mem_struct_t *)userp;

  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if (ptr == NULL) {
    printf("error: not enough memory\n");
    return 0;
  }

  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  triggerFENReceived(mem->memory);

  return realsize;
}
