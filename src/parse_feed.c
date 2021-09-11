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

char *getFenFromJson(char *feed) {
  char *fen;
  // printf("%s Feed is %s\n", feed);

  struct json_value_s *root = json_parse(feed, strlen(feed));
  struct json_object_element_s *a = initJsonObjElement(root);

  char *aName = getKeyString(a);
  char *aValue = getValueString(a);
  // TODO: Rewrite this whole thing to be more legible and codewise efficient
  if (!strcmp(aName, "t")) {
    if (!strcmp(aValue, "fen")) {
      a = a->next;
      struct json_value_s *a_value = a->value;
      a = json_value_as_object(a_value)->start;
      fen = (char *)malloc(sizeof(char) * strlen(getValueString(a)));
      strcpy(fen, getValueString(a));
    } else if (!strcmp(aValue, "featured")) {
      a = a->next;
      struct json_value_s *a_value = a->value;
      a = json_value_as_object(a_value)->start;
      a = a->next->next->next;
      fen = (char *)malloc(sizeof(char) * strlen(getValueString(a)));
      strcpy(fen, getValueString(a));
    }
  }
  free(root);
  return (fen) ? fen : "failed";
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

uint fillGameInfo(lichess_data_t *destData, char *unparsedData) {
  uint errFlag = 1;
  struct json_value_s *root = json_parse(unparsedData, strlen(unparsedData));
  struct json_object_element_s *a = initJsonObjElement(root);
  a = valueToElement(
      a->next);      // takes an element. returns its value as an element.
  a = a->next->next; // id->orientation->players
  struct json_array_s *array = json_value_as_array(a->value);
  assert(array->length == 2);
  struct json_array_element_s *firstArrayElement = array->start;
  struct json_value_s *whiteInfo = firstArrayElement->value;
  struct json_object_s *whiteData = (struct json_object_s *)whiteInfo->payload;
  struct json_object_element_s *whiteColor, *whiteUser, *whiteRating,
      *whiteUserName, *whiteUserTitle, *blackColor, *blackUser, *blackRating,
      *blackUserName, *blackUserTitle;
  whiteColor = whiteData->start;
  whiteUser = whiteColor->next;
  whiteRating = whiteUser->next;
  whiteUserName = valueToElement(whiteUser);
  whiteUserTitle = whiteUserName->next;
  // TODO: get the clocktime from fen adjoint data

  struct json_array_element_s *secondArrayElement = firstArrayElement->next;
  struct json_value_s *blackInfo = secondArrayElement->value;
  struct json_object_s *blackData =
      (struct json_object_s *)(blackInfo)->payload;
  blackColor = blackData->start;
  blackUser = blackColor->next;
  blackRating = blackUser->next;
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
