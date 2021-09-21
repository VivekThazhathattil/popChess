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

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <board.h>
#include <gtk/gtk.h>
#include <gtk/gtkbutton.h>
#include <stdlib.h>
#include <utils.h>
#ifndef RSVG_CAIRO_H_
#include <librsvg/rsvg.h>
#endif

typedef struct boardInfo {
  GtkWidget *widget;
  int fenActive;
  piece_info_t *piece_info;
  char *wClock, *bClock;
  char *lastMove;
  state_vars_t *states;
  colors_t darkSquareColor, lightSquareColor;
} board_info_t;

typedef struct displayOutput {
  GtkWidget *display;
  board_info_t *board_info;
} display_output_t;

typedef struct ButtonArray {
  GtkWidget *flipBoard, *selectMode, *selectColor, *selectPieces, *copyFEN;
  GtkWidget *showUndefendedPieces, *showEvaluationBar, *copyImage, *showCoords;
} button_array_t;

display_output_t *displayControl(state_vars_t *);
void updateCanvas(GtkWidget *, char *, char *, char *, char *, char *, char *);
void makePlayerDetails(GtkWidget *, GtkWidget *, GtkWidget *, GtkWidget *,
                       char *, char *);
// void makeCoordinates(GtkWidget *);
// void makeArrows(GtkWidget *);
void makeControlButtonsArray(GtkWidget *, button_array_t *, board_info_t *);
void updateAllLabelTexts(lichess_data_t *);
void updateClockLabelTexts(char *, char *);
void updateLabelTexts(GtkWidget *, char *);
void load_svgs(char *, GError **);
void showPieces(piece_info_t *, lichess_data_t *);
void freeBoardInfo(board_info_t *);
void freeDisplayOutput(display_output_t *);
void freePieceInfo(piece_info_t *);
void setFenInactive();
void clean_rsvg(void);
void assignColors(colors_t *color, const double, const double, const double, const double);
void colors_t_to_GdkRGBA(const colors_t* , GdkRGBA *);
void GdkRGBA_to_colors_t(const GdkRGBA *, colors_t* );

#endif // include guard
