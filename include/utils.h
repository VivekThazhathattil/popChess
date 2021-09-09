#ifndef UTILS_H_
#define UTILS_H_

#define IGNORE(x) (void)(x)

typedef unsigned int uint;
struct _pair_t {
  uint x;
  uint y;
};
typedef struct _pair_t sizes_t;

struct PlayerAttributes {
  char *name, *title, *rating, *timeLeft;
};
typedef struct PlayerAttributes player_attributes_t;

struct LichessData {
  player_attributes_t white, black;
};
typedef struct LichessData lichess_data_t;

struct PieceInfo {
  uint x, y;
  uint id;
  uint totalCount; // this is unnecessary, but convenient.
  char type, color;
};
typedef struct PieceInfo piece_info_t;

#endif // include guard