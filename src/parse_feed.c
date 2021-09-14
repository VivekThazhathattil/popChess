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

#include <parse_feed.h>

static struct json_object_element_s *
initJsonObjElement(struct json_value_s *rt) {
  assert(rt->type == json_type_object);
  struct json_object_s *object = (struct json_object_s *)rt->payload;
  struct json_object_element_s *a = object->start;
  return a;
}

static char *getKeyString(struct json_object_element_s *a) {
  struct json_string_s *a_name = a->name;
  return a_name->string;
}

static char *getValueString(struct json_object_element_s *a) {
  struct json_value_s *a_value = a->value;
  struct json_string_s *a_value_string =
      (struct json_string_s *)a_value->payload;
  return a_value_string->string;
}

static struct json_object_element_s *
valueToElement(struct json_object_element_s *a) {
  struct json_value_s *a_value = a->value;
  return json_value_as_object(a_value)->start;
}

static copyAllocateValues(char **key, struct json_object_element_s *value) {
  *key = (char *)malloc(sizeof(char) * strlen(getValueString(value)) + 1);
  strcpy(*key, getValueString(value));
}

static copyAllocateString(char **key, char *_string) {
  *key = (char *)malloc(sizeof(char) * strlen(_string) + 1);
  strcpy(*key, _string);
}

static struct json_object_element_s *
navigateTo(char *destination, struct json_object_element_s *elem) {
  while (elem != NULL) {
    //  printf("%s\n", getKeyString(elem));
    if (!strcmp(getKeyString(elem), destination))
      break;
    elem = elem->next;
  }
  return elem;
}

char *getLastJSON(char *feed) {
  /*    get the pointer to the last character of the char string.
      Move backwards until return character is met of argument char pointer is
     met. return the pointer to the character after the return character
  */

  char *lastCharPtr = strchr(feed, '\0');
  size_t totalStrLength = strlen(feed),
         currStrLength = 0; // + 1 to include '\0'
  size_t returnCount = 0;
  while (currStrLength < totalStrLength) {
    //        if(*lastCharPtr == '\n' && *lastCharPtr == '\r')
    if (*lastCharPtr ==
        '{') // really bad way to extract strings. It works for now
      if (*(lastCharPtr + 2) == 't')
        break;
    ++currStrLength;
    --lastCharPtr;
  }
  return (*lastCharPtr == '\n' && *lastCharPtr == '\r') ? lastCharPtr + 1
                                                        : lastCharPtr;
}

fen_data_t *getFenFromJson(char *feed) {
  fen_data_t *fen_data;
  // printf("%s Feed is %s\n", feed);

  struct json_value_s *root = json_parse(feed, strlen(feed));
  struct json_object_element_s *a = initJsonObjElement(root);

  char *aName = getKeyString(a);
  char *aValue = getValueString(a);
  // TODO: Rewrite this whole thing to be more legible and codewise efficient
  if (!strcmp(aName, "t")) {
    if (!strcmp(aValue, "fen")) {
      a = navigateTo("d", a);
      struct json_value_s *a_value = a->value;
      a = json_value_as_object(a_value)->start;
      fen_data = (fen_data_t *)malloc(sizeof(fen_data_t));

      a = navigateTo("fen", a);
      fen_data->fen = (char *)malloc(sizeof(char) * (strlen(getValueString(a)) + 1));
      strcpy(fen_data->fen, getValueString(a));

      a = navigateTo("lm", a); // last move
      fen_data->lastMove = (char *)malloc(sizeof(char) * (strlen(getValueString(a)) + 1));
      strcpy(fen_data->lastMove, getValueString(a));

      a = navigateTo("wc", a);
      //printf("Vivek\n");
      //printf("wc: %s\n",getValueString(a));
      //printf("Keviv\n");
      fen_data->whiteClock = (char *)malloc(sizeof(char) * (strlen(getValueString(a)) + 1));
      strcpy(fen_data->whiteClock, getValueString(a));

      a = navigateTo("bc", a);
      fen_data->blackClock = (char *)malloc(sizeof(char) * (strlen(getValueString(a)) + 1));
      strcpy(fen_data->blackClock, getValueString(a));

    } else if (!strcmp(aValue, "featured")) {
      a = a->next;
      struct json_value_s *a_value = a->value;
      a = json_value_as_object(a_value)->start;
      a = navigateTo("fen", a);
      if (a == NULL) {
        printf("Error: Cannot find PGN in JSON. Skipping");
      } else {
        fen_data = (fen_data_t *)malloc(sizeof(fen_data_t));
        fen_data->fen = (char *)malloc(sizeof(char) * (strlen(getValueString(a)) + 1));
        strcpy(fen_data->fen, getValueString(a));
        fen_data->lastMove = NULL;
        fen_data->whiteClock = NULL;
        fen_data->blackClock = NULL;
      }
    }
  }
  free(root);
  if(!fen_data->fen){
    free(fen_data);
    return NULL;
  }
  return fen_data;
}

int isNewGame(char *unparsedJson) {
  int flag = 0;
  struct json_value_s *root = json_parse(unparsedJson, strlen(unparsedJson));
  struct json_object_element_s *a = initJsonObjElement(root);

  char *aName = getKeyString(a);
  char *aValue = getValueString(a);
  // printf("%s %s\n", aName, aValue);
  if (!strcmp(aName, "t") && !strcmp(aValue, "featured"))
    flag = 1;
  free(root);
  return flag;
}

// typical JSON from the curl output is of this form:
/*{"t":"featured","d":{"id":"52Vg8VB0","orientation":"black","players":[{"color":"white","user":{"name":"donotcrymeariver","id":"donotcrymeariver"},"rating":2634},{"color":"black","user":{"name":"NguyenDucHoa","title":"GM","id":"nguyenduchoa"},"rating":2644}],"fen":"r1r3k1/p2n1pbp/b1pBp1p1/q2p4/3P4/P1N3P1/1P2PPBP/2RQR1K1"}}
{"t":"fen","d":{"fen":"r1r3k1/p2n1pbp/bqpBp1p1/3p4/3P4/P1N3P1/1P2PPBP/2RQR1K1
w","lm":"a5b6","wc":161,"bc":121}} */

uint fillGameInfo(lichess_data_t *destData, char *unparsedData) {
  uint errFlag = 1;
  struct json_value_s *root = json_parse(unparsedData, strlen(unparsedData));
  struct json_object_element_s *a = initJsonObjElement(root);

  if ((a = navigateTo("d", a)) == NULL) {
    printf("Error: No field:\'d\' found. Invalid JSON\n");
    return --errFlag;
  }
  // get into the inner JSON of key:'d' convert the value of 'd' into a json
  // element
  a = valueToElement(a);

  if ((a = navigateTo("players", a)) == NULL) {
    printf("Error: No field:\'players\' was found. Invalid JSON\n");
    return --errFlag;
  }
  struct json_array_s *array = json_value_as_array(a->value);
  assert(array->length == 2);

  struct json_array_element_s *firstArrayElement = array->start;
  struct json_value_s *whiteInfo = firstArrayElement->value;
  struct json_object_s *whiteData = (struct json_object_s *)whiteInfo->payload;
  struct json_object_element_s *whiteColor, *whiteUser, *whiteRating,
      *whiteUserName, *whiteUserTitle, *blackColor, *blackUser, *blackRating,
      *blackUserName, *blackUserTitle;

  whiteColor = navigateTo("color", whiteData->start);
  whiteUser = navigateTo("user", whiteData->start);
  whiteRating = navigateTo("rating", whiteData->start);

  whiteUserName =
      valueToElement(whiteUser); // TODO: check the correctness of this part
  whiteUserTitle = whiteUserName->next;
  // TODO: get the clocktime from fen adjoint data

  struct json_array_element_s *secondArrayElement = firstArrayElement->next;
  struct json_value_s *blackInfo = secondArrayElement->value;
  struct json_object_s *blackData =
      (struct json_object_s *)(blackInfo)->payload;

  blackColor = navigateTo("color", blackData->start);
  blackUser = navigateTo("user", blackData->start);
  blackRating = navigateTo("rating", blackData->start);

  blackUserName = valueToElement(blackUser);
  blackUserTitle = blackUserName->next;

  copyAllocateValues(&destData->white.name, whiteUserName);
  copyAllocateValues(&destData->white.rating, whiteRating);
  copyAllocateValues(&destData->white.title, whiteUserTitle);
  copyAllocateValues(&destData->black.name, blackUserName);
  copyAllocateValues(&destData->black.rating, blackRating);
  copyAllocateValues(&destData->black.title, blackUserTitle);

  return errFlag;
}

uint fillClockTimes(lichess_data_t *destData, char* wClock, char* bClock){
  // TODO: fix errflag usability
  uint errFlag = 1; 
  copyAllocateString(&destData->white.timeLeft, wClock);
  copyAllocateString(&destData->black.timeLeft, bClock);
  return errFlag;
}
