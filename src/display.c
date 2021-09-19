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

#include "display.h"
#include <stdio.h>

// TODO: remove global vars; use a struct ptr;
GtkWidget *wNameWidget, *wRatingWidget, *wTimeWidget;
GtkWidget *bNameWidget, *bRatingWidget, *bTimeWidget;
RsvgHandle *piece_images[2][6];
board_info_t *board_info;

static gboolean draw_callback(GtkWidget *, cairo_t *, board_info_t *);
static gboolean flip_board_callback(GtkWidget *, gpointer *);
static gboolean color_callback(GtkWidget *, gpointer *);
static gboolean coord_callback(GtkWidget *, gpointer *);

// bad practice: remove duplicates
void updateAllLabelTexts(lichess_data_t *liDat) {
  updateLabelTexts(wNameWidget, liDat->white.name);
  updateLabelTexts(bNameWidget, liDat->black.name);
  updateLabelTexts(wRatingWidget, liDat->white.rating);
  updateLabelTexts(bRatingWidget, liDat->black.rating);
}

static void formatIndividualTimeString(char *t, char *T){
  int intT = atoi(T), secs, mins;
  //int intBT = atoi(bT);
  secs = intT % 60; 
  mins = intT * 1.0 / 60;
  if(mins > 99){
    printf("Error: Time control too long\n");
    sprintf(t, "00:00");
  }
  else{
    sprintf(t, "%02d:%02d", mins, secs);
  }
}

static void formatTimeStrings(char *wt, char *bt, char *wT, char *bT){
  formatIndividualTimeString(wt, wT);
  formatIndividualTimeString(bt, bT);
}

void updateClockLabelTexts(char *wTime, char *bTime) {
  char wt[6], bt[6];
  formatTimeStrings(wt, bt, wTime, bTime);
  updateLabelTexts(wTimeWidget, wt);
  updateLabelTexts(bTimeWidget, bt);
}

void freePieceInfo(piece_info_t *piece_info) { free(piece_info); }

void freeBoardInfo(board_info_t *board_info) { free(board_info); }

void freeDisplayOutput(display_output_t *output) { free(output); }

display_output_t *displayControl(state_vars_t *states) {
  GtkWidget *canvas, *outVBox, *buttonArray, *board, *whitePlayerDetails,
      *blackPlayerDetails;
  display_output_t *output;
  button_array_t buttons;

  char *piece_svgs = "pieces/merida/";
  GError *err = NULL;
  load_svgs(piece_svgs, &err);

  canvas = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  outVBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  buttonArray = gtk_button_box_new(GTK_ORIENTATION_VERTICAL);
  gtk_button_box_set_layout(GTK_BUTTON_BOX(buttonArray), GTK_BUTTONBOX_EXPAND);

  board = gtk_drawing_area_new();
  gtk_widget_set_size_request(board, BOARD_SIZE_X, BOARD_SIZE_Y);
  board_info = (board_info_t *)malloc(sizeof(board_info_t));
  if (!board_info) {
    printf("\n malloc error: Cannot set board_info\n");
    return NULL;
  }
  board_info->widget = board;
  board_info->fenActive = 0;
  board_info->piece_info = (piece_info_t *)calloc(32, sizeof(piece_info_t));
  board_info->states = states;

  g_signal_connect(board_info->widget, "draw", G_CALLBACK(draw_callback),
                   board_info);

  wNameWidget = gtk_label_new("-");
  bNameWidget = gtk_label_new("-");
  wRatingWidget = gtk_label_new("-");
  bRatingWidget = gtk_label_new("-");
  wTimeWidget = gtk_label_new("-");
  bTimeWidget = gtk_label_new("-");

  whitePlayerDetails = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  blackPlayerDetails = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

  makePlayerDetails(whitePlayerDetails, wNameWidget, wRatingWidget, wTimeWidget,
                    NULL, NULL);
  makePlayerDetails(blackPlayerDetails, bNameWidget, bRatingWidget, bTimeWidget,
                    NULL, NULL);
  gtk_box_pack_end(GTK_BOX(canvas), whitePlayerDetails, 1, 1, 0);
  gtk_box_pack_end(GTK_BOX(canvas), board, 1, 1, 0);
  gtk_box_pack_end(GTK_BOX(canvas), blackPlayerDetails, 1, 1, 0);

  makeControlButtonsArray(buttonArray, &buttons, board_info);
  gtk_box_pack_start(GTK_BOX(outVBox), canvas, 1, 1, 0);
  gtk_box_pack_start(GTK_BOX(outVBox), buttonArray, 1, 1, 0);

  // makeCoordinates(coords);
  // makeArrows(arrows);

  output = (display_output_t *)malloc(sizeof(display_output_t));
  if (!output) {
    printf("\n Error in setting display output\n");
  } else {
    output->board_info = board_info;
    output->display = outVBox;
  }
  return output;
}

void updateLabelTexts(GtkWidget *label, char *text) {
  gtk_label_set_text(GTK_LABEL(label), (text == NULL) ? "-" : text);
}
void makePlayerDetails(GtkWidget *playerDetails, GtkWidget *name,
                       GtkWidget *rating, GtkWidget *time, char *nameStr,
                       char *ratingStr) {
  //  name = (nameStr == NULL) ? gtk_label_new("Name") : nameStr;
  //  rating = (ratingStr == NULL) ? gtk_label_new("Rating") : ratingStr;
  //  time = gtk_label_new("Time"); // TODO: fix this as well, along with
  //  ratingStr
  updateLabelTexts(name, nameStr);
  updateLabelTexts(rating, ratingStr);
  updateLabelTexts(time, NULL);
  gtk_box_pack_start(GTK_BOX(playerDetails), name, 1, 1, 0);
  gtk_box_pack_start(GTK_BOX(playerDetails), rating, 1, 1, 0);
  gtk_box_pack_start(GTK_BOX(playerDetails), time, 1, 1, 0);
}

// void makeCoordinates(GtkWidget *coords) {}
//
// void makeArrows(GtkWidget *arrows) {}

static GtkWidget *gtkImageButton(char *imageLocation) {
  GtkWidget *image = gtk_image_new_from_file(imageLocation);
  GtkWidget *button = gtk_button_new();
  gtk_button_set_image(GTK_BUTTON(button), image);
  return button;
}

void makeControlButtonsArray(GtkWidget *array, button_array_t *btns,
                             board_info_t *data) {
  btns->flipBoard = gtkImageButton("res/flip.png");
  btns->selectMode = gtkImageButton("res/mode.png");
  btns->selectColor = gtkImageButton("res/color.png");
  btns->selectPieces = gtkImageButton("res/piece.png");
  btns->copyFEN = gtkImageButton("res/fencopy.png");
  btns->copyImage = gtkImageButton("res/imgcopy.png");
  btns->showUndefendedPieces = gtkImageButton("res/undefended.png");
  btns->showEvaluationBar = gtkImageButton("res/evalbar.png");
  btns->showCoords = gtkImageButton("res/coords.png");

  g_signal_connect(GTK_BUTTON(btns->flipBoard), "clicked",
                   G_CALLBACK(flip_board_callback), data);
  g_signal_connect(GTK_BUTTON(btns->selectColor), "clicked",
                   G_CALLBACK(color_callback), data);
  g_signal_connect(GTK_BUTTON(btns->showCoords), "clicked",
                   G_CALLBACK(coord_callback), data);

  gtk_box_pack_start(GTK_BOX(array), btns->flipBoard, 1, 0, 0);
  gtk_box_pack_start(GTK_BOX(array), btns->selectMode, 1, 0, 0);
  gtk_box_pack_start(GTK_BOX(array), btns->selectColor, 1, 0, 0);
  gtk_box_pack_start(GTK_BOX(array), btns->selectPieces, 1, 0, 0);
  gtk_box_pack_start(GTK_BOX(array), btns->copyFEN, 1, 0, 0);
  gtk_box_pack_start(GTK_BOX(array), btns->copyImage, 1, 0, 0);
  gtk_box_pack_start(GTK_BOX(array), btns->showUndefendedPieces, 1, 0, 0);
  gtk_box_pack_start(GTK_BOX(array), btns->showEvaluationBar, 1, 0, 0);
  gtk_box_pack_start(GTK_BOX(array), btns->showCoords, 1, 0, 0);
}

static void assignColors(colors_t *color, const double r, const double g,
                         const double b, const double a) {
  // colors assumed to be in (r,g,b) format : (0,0,0) -> (255, 255, 255)
  color->r = r / 255;
  color->g = g / 255;
  color->b = b / 255;
  color->a = a / 255;
}

static void showFileNames(colors_t *squareColor, cairo_t *cr, uint i, uint j,
                          sizes_t ss, uint isFlipped) {
  cairo_save(cr);
  cairo_set_source_rgb(cr, squareColor->r, squareColor->g, squareColor->b);
  cairo_set_font_size(cr, 18);
  char fileName = (isFlipped) ? 'a' + (7 - i) : 'a' + i;
  cairo_move_to(cr, (i + 0.05) * ss.x, (j + 0.95) * ss.y);
  cairo_show_text(cr, &fileName);
  cairo_restore(cr);
}

static void showRankNumbers(colors_t *squareColor, cairo_t *cr, uint i, uint j,
                            sizes_t ss, uint isFlipped) {
  cairo_save(cr);
  cairo_set_source_rgb(cr, squareColor->r, squareColor->g, squareColor->b);
  cairo_set_font_size(cr, 18);
  char fileName = (isFlipped) ? '1' + j : '1' + (7 - j);
  cairo_move_to(cr, (i + 0.85) * ss.x, (j + 0.95) * ss.y);
  cairo_show_text(cr, &fileName);
  cairo_restore(cr);
}

static void drawBoard(cairo_t *cr, GtkWidget *data, state_vars_t *statesVar) {
  colors_t darkSquares, lightSquares;
  assignColors(statesVar->flip ? &lightSquares : &darkSquares, 240, 217, 181,
               255);
  assignColors(statesVar->flip ? &darkSquares : &lightSquares, 181, 136, 99,
               255);
  sizes_t ss = getSquareSizes(data);
  for (uint i = 0; i < NUM_SQUARES_X; ++i) {
    for (uint j = 0; j < NUM_SQUARES_Y; ++j) {
      if ((i + j) % 2 == 0) {
        cairo_set_source_rgb(cr, darkSquares.r, darkSquares.g, darkSquares.b);
        cairo_rectangle(cr, i * ss.x, j * ss.y, ss.x, ss.y);
        cairo_fill(cr);
        if (statesVar->coords) {
          if (j == 7)
            showFileNames(&lightSquares, cr, i, j, ss, statesVar->flip);
          if (i == 7)
            showRankNumbers(&lightSquares, cr, i, j, ss, statesVar->flip);
        }
      } else {
        cairo_set_source_rgb(cr, lightSquares.r, lightSquares.g,
                             lightSquares.b);
        cairo_rectangle(cr, i * ss.x, j * ss.y, ss.x, ss.y);
        cairo_fill(cr);
        if (statesVar->coords) {
          if (j == 7)
            showFileNames(&darkSquares, cr, i, j, ss, statesVar->flip);
          if (i == 7)
            showRankNumbers(&darkSquares, cr, i, j, ss, statesVar->flip);
        }
      }
    }
  }
}

static void drawPieces(cairo_t *cr, piece_info_t *pieces, uint isFlipped) {
  // printf("drawPieces?: %d\n", pieces[0].totalCount);
  if (pieces[0].totalCount > 32)
    return;
  double ss = DEFAULT_SQUARE_SIZE;
  double scale = 0.0048 * DEFAULT_BOARD_SIZE / ss;
  RsvgHandle *piece_image;
  for (uint i = 0; i < pieces[0].totalCount; ++i) {
    // printf("PINF: %d, %d \n", pieces[i].x, pieces[i].y);
    if (isFlipped)
      cairo_translate(cr, (7 - pieces[i].x) * ss, (pieces[i].y) * ss);
    else
      cairo_translate(cr, pieces[i].x * ss, (7 - pieces[i].y) * ss);
    cairo_scale(cr, scale, scale);
    // printf("PINF: %c, %c \n", pieces[i].color, pieces[i].type);
    piece_image = piece_images[(pieces[i].color == 'b' ? 0 : 1)]
                              [getPieceType(pieces[i].type)];
    rsvg_handle_render_cairo(piece_image, cr);
    cairo_scale(cr, 1 / scale, 1 / scale);
    if (isFlipped)
      cairo_translate(cr, -(7 - pieces[i].x) * ss, -(pieces[i].y) * ss);
    else
      cairo_translate(cr, -pieces[i].x * ss, -(7 - pieces[i].y) * ss);
  }
}

static void highlightLastMove(cairo_t *cr, char *lastMove, uint isFlipped) {
  uint x1Coord, y1Coord, x2Coord, y2Coord;
  if (!getCoordinatesFromMove(lastMove, &x1Coord, &y1Coord, &x2Coord,
                              &y2Coord)) {
    printf("Error: Last move \'%s\' not recognized.\n", lastMove);
    return;
  }

  // printf("%d, %d, %d, %d", x1Coord, y1Coord, x2Coord, y2Coord);
  colors_t squareColor;
  assignColors(&squareColor, 255, 165, 0, 150);
  uint ss = DEFAULT_SQUARE_SIZE;
  cairo_set_source_rgba(cr, squareColor.r, squareColor.g, squareColor.b,
                        squareColor.a);
  if (isFlipped)
    cairo_rectangle(cr, (7 - x1Coord) * ss, (y1Coord)*ss, ss, ss);
  else
    cairo_rectangle(cr, x1Coord * ss, (7 - y1Coord) * ss, ss, ss);
  cairo_fill(cr);
  if (isFlipped)
    cairo_rectangle(cr, (7 - x2Coord) * ss, (y2Coord)*ss, ss, ss);
  else
    cairo_rectangle(cr, x2Coord * ss, (7 - y2Coord) * ss, ss, ss);
  cairo_fill(cr);
}

static gboolean draw_callback(GtkWidget *drawing_area, cairo_t *cr,
                              board_info_t *data) {
  IGNORE(drawing_area);
  GtkWidget *board = (GtkWidget *)data->widget;
  int fenActive = data->fenActive;
  // draw the chess board
  drawBoard(cr, board, data->states);

  // draw the chess pieces
  if (fenActive == 1) {
    if (data->lastMove) {
      highlightLastMove(cr, data->lastMove, data->states->flip);
      // printf("callback?: %d\n", board_info->piece_info[0].totalCount);
      if (data->wClock && data->bClock)
        updateClockLabelTexts(data->wClock, data->bClock);
    }
    piece_info_t *pieces = (piece_info_t *)data->piece_info;
    drawPieces(cr, pieces, data->states->flip);
  }

  return FALSE;
}

static gboolean flip_board_callback(GtkWidget *FlipBoardButton,
                                    gpointer *data) {
  IGNORE(FlipBoardButton);
  board_info_t *dat = (board_info_t *)data;
  dat->states->flip = (dat->states->flip) ? FALSE : TRUE;
  gtk_widget_queue_draw(dat->widget);
  return FALSE;
}

static gboolean color_callback(GtkWidget *ColorButton, gpointer *data) {
  IGNORE(ColorButton);
  IGNORE(data);
  //  GtkColorChooserDialog *a = gtk_color_chooser_dialog_new();
  //  IGNORE(a);
  return FALSE;
}

static gboolean coord_callback(GtkWidget *CoordButton, gpointer *data) {
  IGNORE(CoordButton);
  board_info_t *dat = (board_info_t *)data;
  dat->states->coords = (dat->states->coords) ? FALSE : TRUE;
  gtk_widget_queue_draw(dat->widget);
  return FALSE;
}

void load_svgs(char *dir, GError **err) {
  uint len = strlen(dir) + 8; // e.g.: "w_k.svg\0"
  char str[len];
  char piece_letters[] = "pnbrqk";
  char side_letters[] = "bw";

  for (uint i = 0; i < 2; i++) {
    char side = side_letters[i];

    for (uint j = 0; piece_letters[j] != '\0'; j++) {
      sprintf(str, "%s%c_%c.svg", dir, side, piece_letters[j]);

      piece_images[i][j] = rsvg_handle_new_from_file(str, err);
      if (*err != NULL)
        return;
    }
  }
}

void showPieces(piece_info_t *pieceInfo, lichess_data_t *liDat) {
  //  board_info->piece_info = pieceInfo;
  memcpy(board_info->piece_info, pieceInfo,
         pieceInfo[0].totalCount * sizeof(piece_info_t));
  // printf("showPieces: %d\n", board_info->piece_info[0].totalCount);
  board_info->fenActive = 1;
  if (liDat->white.timeLeft && liDat->black.timeLeft) {
    board_info->wClock = liDat->white.timeLeft;
    board_info->bClock = liDat->black.timeLeft;
  } else {
    board_info->wClock = NULL;
    board_info->bClock = NULL;
  }
  if (liDat->lastMove)
    board_info->lastMove = liDat->lastMove;
  else
    board_info->lastMove = NULL;
  gtk_widget_queue_draw(board_info->widget);
}

void setFenInactive() { board_info->fenActive = 0; }
