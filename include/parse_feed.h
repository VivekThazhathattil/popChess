#ifndef PARSE_FEED_H_
#define PARSE_FEED_H_

#include <assert.h>
#include <json.h>
#include <stdio.h> // for DEBUG
#include <string.h>
#include <utils.h>

struct json_value_s *json_parse(const void *src, size_t src_size);
char *getLastJSON(char *feed);
char *getFenFromJson(char *feed);
int isNewGame(char *unparsedJson);
uint fillGameInfo(lichess_data_t *liData, char *unparsedData);

#endif // include guard