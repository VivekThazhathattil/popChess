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

#ifndef __UTILS_H__
#define __UTILS_H__

#define IGNORE(x) (void)(x)

typedef unsigned int uint;

enum MODES { BULLET, BLITZ, RAPID, TOP_RATED };

enum PIECES {
  ALPHA,
  CALIFORNIA,
  CARDINAL,
  CBURNETT,
  CHESS7,
  CHESSNUT,
  COMPANION,
  DUBROVNY,
  FANTASY,
  FRESCA,
  GIOCO,
  GOVERNOR,
  HORSEY,
  ICPIECES,
  KOSAL,
  LEIPZIG,
  LETTER,
  LIBRA,
  LIST,
  MAESTRO,
  MERIDA,
  MONO,
  PIROUETTI,
  PIXEL,
  REILLYCRAIG,
  RIOHACHA,
  SHAPES,
  SPATIAL,
  STAUNTY,
  TATIANA,
  LAST_NULL // doesn't correspond to anything. It just exists for checking if
            // enum's end has reached
};

typedef struct StateVars {
  /*  flip: flip board
      mode: bullet/blitz/rapid/ultrabullet/top-rated (enum-ed )
      undefPieces: undefended pieces
      evalBar: show evaluation bar
      coords: show board coordinates
      pieces: type of piece
  */
  uint flip, mode, undefPieces, evalBar, coords, pieces;
} state_vars_t;

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
  double r, g, b, a;
} colors_t;

typedef struct FenData {
  char *fen;
  char *lastMove;
  char *whiteClock;
  char *blackClock;
} fen_data_t;

#endif // include guard
