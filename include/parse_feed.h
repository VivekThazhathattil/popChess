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

#ifndef __PARSE_FEED_H__
#define __PARSE_FEED_H__

#include <assert.h>
#include <json.h>
#include <stdio.h> // for DEBUG
#include <string.h>
#include <utils.h>

struct json_value_s *json_parse(const void *src, size_t src_size);
char *getLastJSON(char *feed);
fen_data_t *getFenFromJson(char *feed);
int isNewGame(char *unparsedJson);
uint fillGameInfo(lichess_data_t *liData, char *unparsedData);
uint fillClockTimes(lichess_data_t *LichessData, char *wClock, char *bClock);
uint fillLastMove(lichess_data_t *destData, char *lastMove);

#endif // include guard
