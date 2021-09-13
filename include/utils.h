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

#ifndef UTILS_H_
#define UTILS_H_

#define IGNORE(x) (void)(x)

typedef unsigned int uint;

typedef struct _pair_t {
  uint x;
  uint y;
} sizes_t;

typedef struct PlayerAttributes {
  char *name, *title, *rating, *timeLeft;
} player_attributes_t;

typedef struct LichessData {
  player_attributes_t white, black;
  char *lastMove;
} lichess_data_t;

typedef struct PieceInfo {
  uint x, y;
  uint id;
  uint totalCount; // this is unnecessary, but convenient.
  char type, color;
} piece_info_t;

typedef struct Colors {
  double r, g, b;
} colors_t;

typedef struct FenData{
    char *fen;
    char *lastMove;
    char *whiteClock;
    char *blackClock; 
} fen_data_t;


#endif // include guard
