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
static cairo_surface_t *surface = NULL;

static void clearSurface(cairo_t *cr) {
  cairo_t *surface;
  cr = cairo_create(surface);
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);
  cairo_destroy(cr);
}

static gboolean draw_callback(GtkWidget *drawing_area, cairo_t *cr,
                              board_info_t *data);

// bad practice: remove duplicates
void updateAllLabelTexts(lichess_data_t *liDat) {
  updateLabelTexts(wNameWidget, liDat->white.name);
  updateLabelTexts(bNameWidget, liDat->black.name);
  updateLabelTexts(wRatingWidget, liDat->white.rating);
  updateLabelTexts(bRatingWidget, liDat->black.rating);
}

void updateClockLabelTexts(char* wTime, char* bTime){
  updateLabelTexts(wTimeWidget, wTime);
  updateLabelTexts(bTimeWidget, bTime);
}

void freePieceInfo(piece_info_t *piece_info) { free(piece_info); }

void freeBoardInfo(board_info_t *board_info) { free(board_info); }

void freeDisplayOutput(display_output_t *output) { free(output); }

GtkWidget *displayControl() {
  GtkWidget *canvas, *arrows, *buttonsArray, *coords, *board, *pieces,
      *whitePlayerDetails, *blackPlayerDetails;
  cairo_t *cr;
  display_output_t *output;

  char *piece_svgs = "pieces/merida/";
  GError *err = NULL;
  load_svgs(piece_svgs, &err);
  canvas = gtk_vbox_new(0, 0);
  board = gtk_drawing_area_new();
  gtk_widget_set_size_request(board, BOARD_SIZE_X, BOARD_SIZE_Y);
  board_info = (board_info_t *)malloc(sizeof(board_info_t));
  if (!board_info) {
    printf("\n malloc error: Cannot set board_info\n");
    return canvas;
  }
  board_info->widget = board;
  board_info->fenActive = 0;
  board_info->piece_info = (piece_info_t *)calloc(32, sizeof(piece_info_t));
  g_signal_connect(board_info->widget, "draw", G_CALLBACK(draw_callback),
                   board_info);
  makeBoard(board);

  wNameWidget = gtk_label_new("-");
  bNameWidget = gtk_label_new("-");
  wRatingWidget = gtk_label_new("-");
  bRatingWidget = gtk_label_new("-");
  wTimeWidget = gtk_label_new("-");
  bTimeWidget = gtk_label_new("-");

  whitePlayerDetails = gtk_hbox_new(0, 0);
  blackPlayerDetails = gtk_hbox_new(0, 0);

  makePlayerDetails(whitePlayerDetails, wNameWidget, wRatingWidget, wTimeWidget,
                    NULL, NULL, NULL);
  makePlayerDetails(blackPlayerDetails, bNameWidget, bRatingWidget, bTimeWidget,
                    NULL, NULL, NULL);
  gtk_box_pack_end(GTK_BOX(canvas), whitePlayerDetails, 1, 1, 0);
  gtk_box_pack_end(GTK_BOX(canvas), board, 1, 1, 0);
  gtk_box_pack_end(GTK_BOX(canvas), blackPlayerDetails, 1, 1, 0);

  char piecesDir[] = "pieces/merida/";
  // makePieces(pieces, piecesDir);
  // gtk_box_pack_end(GTK_BOX(canvas), pieces, 1, 1, 0);
  makeCoordinates(coords);
  makeArrows(arrows);
  makeControlButtonsArray(buttonsArray);

  output = (display_output_t *)malloc(sizeof(display_output_t));
  if (!output) {
    printf("\n Error in setting display output\n");
  } else {
    output->board_info = board_info;
    output->canvas = canvas;
  }
  return output;
}

void makeBoard(GtkWidget *board) {}

void makePieces(GtkWidget *pieces, char *piecesDir) {
  GError *error = NULL;
  GdkPixbuf *pix;
  if ((pix = gdk_pixbuf_new_from_file_at_scale(piecesDir, 200, 200, TRUE,
                                               &error)) == NULL) {
    g_printerr("Error loading file: #%d %s\n", error->code, error->message);
    g_error_free(error);
    exit(1);
  }
  pieces = gtk_image_new_from_pixbuf(pix);
}

void updateLabelTexts(GtkWidget *label, char *text) {
  gtk_label_set_text(GTK_LABEL(label), (text == NULL) ? "-" : text);
}
void makePlayerDetails(GtkWidget *playerDetails, GtkWidget *name,
                       GtkWidget *rating, GtkWidget *time, char *nameStr,
                       char *titleStr, char *ratingStr) {
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

void makeCoordinates(GtkWidget *coords) {}

void makeArrows(GtkWidget *arrows) {}

void makeControlButtonsArray(GtkWidget *buttonsArray) {}

static void assignColors(colors_t *color, const double r, const double g, const double b, const double a) {
  // colors assumed to be in (r,g,b) format : (0,0,0) -> (255, 255, 255)
  color->r = r / 255;
  color->g = g / 255;
  color->b = b / 255;
  color->a = a /255;
}

static void drawBoard(cairo_t *cr, GtkWidget *data) {
  colors_t darkSquares, lightSquares;
  assignColors(&darkSquares, 240, 217, 181, 255);
  assignColors(&lightSquares, 181, 136, 99, 255);
  sizes_t ss = getSquareSizes(data);
  for (uint i = 0; i < NUM_SQUARES_Y; ++i) {
    for (uint j = 0; j < NUM_SQUARES_X; ++j) {
      if ((i + j) % 2 == 0) {
        cairo_set_source_rgb(cr, darkSquares.r, darkSquares.g, darkSquares.b);
        cairo_rectangle(cr, i * ss.x, j * ss.y, ss.x, ss.y);
        cairo_fill(cr);
      } else {
        cairo_set_source_rgb(cr, lightSquares.r, lightSquares.g,
                             lightSquares.b);
        cairo_rectangle(cr, i * ss.x, j * ss.y, ss.x, ss.y);
        cairo_fill(cr);
      }
    }
  }
}

static void drawPieces(cairo_t *cr, piece_info_t *pieces) {
  // printf("drawPieces?: %d\n", pieces[0].totalCount);
  if (pieces[0].totalCount > 32)
    return;
  double ss = DEFAULT_SQUARE_SIZE;
  double scale = 0.0048 * DEFAULT_BOARD_SIZE / ss;
  RsvgHandle *piece_image;
  for (uint i = 0; i < pieces[0].totalCount; ++i) {
    // printf("PINF: %d, %d \n", pieces[i].x, pieces[i].y);
    cairo_translate(cr, pieces[i].x * ss, (7 - pieces[i].y) * ss);
    cairo_scale(cr, scale, scale);
    // printf("PINF: %c, %c \n", pieces[i].color, pieces[i].type);
    piece_image = piece_images[(pieces[i].color == 'b' ? 0 : 1)]
                              [getPieceType(pieces[i].type)];
    rsvg_handle_render_cairo(piece_image, cr);
    cairo_scale(cr, 1 / scale, 1 / scale);
    cairo_translate(cr, -pieces[i].x * ss, -(7 - pieces[i].y) * ss);
  }
}

static void highlightLastMove(cairo_t *cr, char* lastMove){ 
  uint x1Coord, y1Coord, x2Coord, y2Coord;
  if(!getCoordinatesFromMove(lastMove, &x1Coord, &y1Coord, &x2Coord, &y2Coord)){
    printf("Error: Last move \'%s\' not recognized.\n", lastMove);
    return;
  }

  //printf("%d, %d, %d, %d", x1Coord, y1Coord, x2Coord, y2Coord);
  colors_t squareColor;
  assignColors(&squareColor, 255, 165, 0, 150);
  uint ss = DEFAULT_SQUARE_SIZE;
  cairo_set_source_rgba(cr, squareColor.r, squareColor.g, squareColor.b, squareColor.a);
  cairo_rectangle(cr, x1Coord * ss, (7 - y1Coord) * ss, ss, ss);
  cairo_fill(cr);
  cairo_rectangle(cr, x2Coord * ss, (7 - y2Coord) * ss, ss, ss);
  cairo_fill(cr);
}

static gboolean draw_callback(GtkWidget *drawing_area, cairo_t *cr,
                              board_info_t *data) {
  GtkWidget *board = (GtkWidget *)data->widget;
  int fenActive = data->fenActive;
  // draw the chess board
  drawBoard(cr, board);

  // draw the chess pieces
  if (fenActive == 1) {
    if(data->lastMove){
      highlightLastMove(cr, data->lastMove);
    piece_info_t *pieces = (piece_info_t *)data->piece_info;
    // printf("callback?: %d\n", board_info->piece_info[0].totalCount);
    drawPieces(cr, pieces);
    if(data->wClock && data->bClock)
      updateClockLabelTexts (data->wClock, data->bClock);
    }
  }

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
  if(liDat->white.timeLeft && liDat->black.timeLeft){
    board_info->wClock = liDat->white.timeLeft;
    board_info->bClock = liDat->black.timeLeft;
  }
  else{
    board_info->wClock = NULL;
    board_info->bClock = NULL;
  }
  if(liDat->lastMove)
    board_info->lastMove = liDat->lastMove;
  else
    board_info->lastMove = NULL;  
  gtk_widget_queue_draw(board_info->widget);
}

void setFenInactive() { board_info->fenActive = 0; }