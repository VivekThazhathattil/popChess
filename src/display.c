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
static gboolean coord_callback(GtkWidget *, gpointer *);
static gboolean select_pieces_callback(GtkWidget *, gpointer *);
static gboolean select_color_callback(GtkWidget *, gpointer *);
static gboolean color_button_pressed_callback(GtkWidget *, gpointer *);
static gboolean exit_color_window_callback(GtkWidget *, gpointer *);
static gboolean copy_FEN_callback(GtkWidget *, gpointer *);
static gboolean copy_image_callback(GtkWidget *, gpointer *);
static gboolean show_undefended_pieces_callback(GtkWidget *, gpointer *);
static gboolean show_evaluation_bar_callback(GtkWidget *, gpointer *);
static gboolean select_mode_callback(GtkWidget *, gpointer *);

// bad practice: remove duplicates
void updateAllLabelTexts(lichess_data_t *liDat) {
  updateLabelTexts(wNameWidget, liDat->white.name);
  updateLabelTexts(bNameWidget, liDat->black.name);
  updateLabelTexts(wRatingWidget, liDat->white.rating);
  updateLabelTexts(bRatingWidget, liDat->black.rating);
}

static void formatIndividualTimeString(char *t, char *T) {
  int intT = atoi(T), secs, mins;
  // int intBT = atoi(bT);
  secs = intT % 60;
  mins = intT * 1.0 / 60;
  if (mins > 99) {
    printf("Error: Time control too long\n");
    sprintf(t, "00:00");
  } else {
    sprintf(t, "%02d:%02d", mins, secs);
  }
}

void colors_t_to_GdkRGBA(const colors_t* src, GdkRGBA *dest){
  dest->red = src->r;
  dest->green = src->g;
  dest->blue = src->b;
  dest->alpha = src->a;
  return;
}

void GdkRGBA_to_colors_t(const GdkRGBA *src, colors_t* dest){
  dest->r = src->red;
  dest->g = src->green;
  dest->b = src->blue;
  dest->a = src->alpha;
  return;
}

static void formatTimeStrings(char *wt, char *bt, char *wT, char *bT) {
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

void clean_rsvg(void) {
  for (uint i = 0; i < 2; ++i) {
    for (uint j = 0; j < 6; ++j) {
      if (piece_images[i][j] != NULL)
        g_object_unref(piece_images[i][j]);
    }
  }
  return;
}

static void set_SVG(char *fileName) {
  char filePath[50];
  sprintf(filePath, "pieces/lichess/%s/", fileName);
  GError *err = NULL;
  load_svgs(filePath, &err);
}

display_output_t *displayControl(state_vars_t *states) {
  GtkWidget *canvas, *outVBox, *buttonArray, *board, *whitePlayerDetails,
      *blackPlayerDetails;
  display_output_t *output;
  button_array_t buttons;

  set_SVG("alpha");

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
  assignColors(&(board_info->darkSquareColor), 240, 217, 181, 255); //default colors
  assignColors(&(board_info->lightSquareColor), 181, 136, 99, 255);

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
  gtk_box_pack_end(GTK_BOX(canvas), whitePlayerDetails, 1, 1, 10);
  gtk_box_pack_end(GTK_BOX(canvas), board, 1, 1, 0);
  gtk_box_pack_end(GTK_BOX(canvas), blackPlayerDetails, 1, 1, 10);

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
  gchar *markupString = g_markup_printf_escaped("<span font_desc = \"Sans  Bold 11\"> %s </span>",text);
  gtk_label_set_markup(GTK_LABEL(label), (text) == NULL
                                          ? "..."
                                          : markupString);
  g_free(markupString);
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

static GtkWidget *gtkImageButton(char *imageLocation, char *tooltipText) {
  GtkWidget *image = gtk_image_new_from_file(imageLocation);
  GtkWidget *button = gtk_button_new();
  gtk_widget_set_tooltip_text(button, tooltipText);
  gtk_button_set_image(GTK_BUTTON(button), image);
  return button;
}

void makeControlButtonsArray(GtkWidget *array, button_array_t *btns,
                             board_info_t *data) {
  btns->flipBoard = gtkImageButton("res/flip.png", "Flip board");
  btns->selectMode =
      gtkImageButton("res/mode.png", "Rapid/blitz/bullet/Top Rated Mode");
  btns->selectColor = gtkImageButton("res/color.png", "Select square colors");
  btns->selectPieces = gtkImageButton("res/piece.png", "Select piece type");
  btns->copyFEN = gtkImageButton("res/fencopy.png", "Copy current FEN");
  btns->copyImage =
      gtkImageButton("res/imgcopy.png", "Copy image to clipboard");
  btns->showUndefendedPieces =
      gtkImageButton("res/undefended.png", "Show/hide undefended pieces");
  btns->showEvaluationBar =
      gtkImageButton("res/evalbar.png", "Show/hide Evalutation bar");
  btns->showCoords =
      gtkImageButton("res/coords.png", "Show/hide board coordinates");

  g_signal_connect(GTK_BUTTON(btns->flipBoard), "clicked",
                   G_CALLBACK(flip_board_callback), data);
  g_signal_connect(GTK_BUTTON(btns->selectMode), "clicked",
                   G_CALLBACK(select_mode_callback), data);
  g_signal_connect(GTK_BUTTON(btns->selectColor), "clicked",
                   G_CALLBACK(select_color_callback), data);
  g_signal_connect(GTK_BUTTON(btns->selectPieces), "clicked",
                   G_CALLBACK(select_pieces_callback), data);
  g_signal_connect(GTK_BUTTON(btns->copyFEN), "clicked",
                   G_CALLBACK(copy_FEN_callback), data);
  g_signal_connect(GTK_BUTTON(btns->copyImage), "clicked",
                   G_CALLBACK(copy_image_callback), data);
  g_signal_connect(GTK_BUTTON(btns->showUndefendedPieces), "clicked",
                   G_CALLBACK(show_undefended_pieces_callback), data);
  g_signal_connect(GTK_BUTTON(btns->showEvaluationBar), "clicked",
                   G_CALLBACK(show_evaluation_bar_callback), data);
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

void assignColors(colors_t *color, const double r, const double g,
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

static void drawBoard(cairo_t *cr, GtkWidget *data, state_vars_t *statesVar, colors_t *darkColor, colors_t *lightColor) {
  colors_t darkSquares, lightSquares;
  darkSquares = *darkColor;
  lightSquares = *lightColor;
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
  if (pieces[0].totalCount > 32)
    return;
  double ss = DEFAULT_SQUARE_SIZE;
  double scale = 0.0048 * DEFAULT_BOARD_SIZE / ss;
  RsvgHandle *piece_image;
  RsvgRectangle viewport;
  for (uint i = 0; i < pieces[0].totalCount; ++i) {
    cairo_scale(cr, scale, scale);
    cairo_scale(cr, 1 / scale, 1 / scale);
    piece_image = piece_images[(pieces[i].color == 'b' ? 0 : 1)]
                              [getPieceType(pieces[i].type)];
    if(isFlipped){
      viewport.x = (7 - pieces[i].x)*ss;
      viewport.y = (pieces[i].y)*ss;
    }
    else{
      viewport.x = (pieces[i].x)*ss;
      viewport.y = (7 - pieces[i].y)*ss;
    }
    viewport.width = ss;
    viewport.height = ss;
    rsvg_handle_render_document(piece_image, cr, &viewport, NULL);
  }
}

static void highlightLastMove(cairo_t *cr, char *lastMove, uint isFlipped) {
  uint x1Coord, y1Coord, x2Coord, y2Coord;
  if (!getCoordinatesFromMove(lastMove, &x1Coord, &y1Coord, &x2Coord,
                              &y2Coord)) {
    printf("Error: Last move \'%s\' not recognized.\n", lastMove);
    return;
  }
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
  drawBoard(cr, board, data->states, &(data->darkSquareColor), &(data->lightSquareColor));

  // draw the chess pieces
  if (fenActive == 1) {
    if (data->lastMove) {
      highlightLastMove(cr, data->lastMove, data->states->flip);
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

static gboolean coord_callback(GtkWidget *CoordButton, gpointer *data) {
  IGNORE(CoordButton);
  board_info_t *dat = (board_info_t *)data;
  dat->states->coords = (dat->states->coords) ? FALSE : TRUE;
  gtk_widget_queue_draw(dat->widget);
  return FALSE;
}

static gboolean select_pieces_callback(GtkWidget *PiecesButton,
                                       gpointer *data) {
  IGNORE(PiecesButton);
  board_info_t *dat = (board_info_t *)data;
  dat->states->pieces = dat->states->pieces + 1;
  if (dat->states->pieces >= LAST_NULL) {
    dat->states->pieces = ALPHA;
  }
  //printf("%d\n", dat->states->pieces);

  switch (dat->states->pieces) {
  case ALPHA:
    set_SVG("alpha");
    break;
  case CALIFORNIA:
    set_SVG("california");
    break;
  case CARDINAL:
    set_SVG("cardinal");
    break;
  case CBURNETT:
    set_SVG("cburnett");
    break;
  case CHESS7:
    set_SVG("chess7");
    break;
  case CHESSNUT:
    set_SVG("chessnut");
    break;
  case COMPANION:
    set_SVG("companion");
    break;
  case DUBROVNY:
    set_SVG("dubrovny");
    break;
  case FANTASY:
    set_SVG("fantasy");
    break;
  case FRESCA:
    set_SVG("fresca");
    break;
  case GIOCO:
    set_SVG("gioco");
    break;
  case GOVERNOR:
    set_SVG("governor");
    break;
  case HORSEY:
    set_SVG("horsey");
    break;
  case ICPIECES:
    set_SVG("icpieces");
    break;
  case KOSAL:
    set_SVG("kosal");
    break;
  case LEIPZIG:
    set_SVG("leipzig");
    break;
  case LETTER:
    set_SVG("letter");
    break;
  case LIBRA:
    set_SVG("libra");
    break;
  case LIST:
    set_SVG("list");
    break;
  case MAESTRO:
    set_SVG("maestro");
    break;
  case MERIDA:
    set_SVG("merida");
    break;
  case MONO:
    set_SVG("mono");
    break;
  case PIROUETTI:
    set_SVG("pirouetti");
    break;
  case PIXEL:
    set_SVG("pixel");
    break;
  case REILLYCRAIG:
    set_SVG("reillycraig");
    break;
  case RIOHACHA:
    set_SVG("riohacha");
    break;
  case SHAPES:
    set_SVG("shapes");
    break;
  case SPATIAL:
    set_SVG("spatial");
    break;
  case STAUNTY:
    set_SVG("staunty");
    break;
  case TATIANA:
    set_SVG("tatiana");
    break;
  default:
    set_SVG("horsey");
  }

  gtk_widget_queue_draw(dat->widget);
  return FALSE;
}

static GtkWidget* setColorWindowProps(GtkWidget *win){
  gtk_window_set_default_size(GTK_WINDOW(win), 400, 200);
  gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);
  return win;
}

static void populateHContainer(colors_t *squareColor, GtkWidget *hContainer, const char *labelText){
  GdkRGBA *color = (GdkRGBA *)malloc(sizeof(GdkRGBA));
  colors_t_to_GdkRGBA(squareColor, color);
  GtkWidget *vContainer, *colorButton, *label;
  vContainer = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
  colorButton = gtk_color_button_new_with_rgba(color);
  g_signal_connect(colorButton, "color-set", G_CALLBACK(color_button_pressed_callback), squareColor);
  label = gtk_label_new(labelText);
  gtk_box_pack_start(GTK_BOX(vContainer), label, 1, 1, 0);
  gtk_box_pack_start(GTK_BOX(vContainer), colorButton, 1, 1, 0);
  gtk_box_pack_start(GTK_BOX(hContainer), vContainer, 1, 1, 0);

  gdk_rgba_free(color);
  return;
}

static GtkWidget *showColorWindow(gpointer *data){
  GtkWidget *colorWindow;
  colorWindow = gtk_window_new(GTK_WINDOW_POPUP);
  setColorWindowProps(colorWindow);
  board_info_t *dat = (board_info_t *)data;
  GtkWidget *hContainer, *exitButton;
  hContainer = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
  exitButton = gtk_button_new_with_label("Exit");
  g_signal_connect(GTK_BUTTON(exitButton), "clicked",
                   G_CALLBACK(exit_color_window_callback), colorWindow);
  populateHContainer(&(dat->lightSquareColor), hContainer,"Dark square color");
  populateHContainer(&(dat->darkSquareColor), hContainer, "Light square color");
  gtk_box_pack_start(GTK_BOX(hContainer), exitButton, 1,1,0);
  gtk_container_add(GTK_CONTAINER(colorWindow), hContainer);
  gtk_widget_show_all(colorWindow);
  return colorWindow;
}

static gboolean select_color_callback(GtkWidget *ColorButton,
                                       gpointer *data) {
  IGNORE(ColorButton);
  GtkWidget *win;
  if( (win = showColorWindow(data)) == NULL){
    return TRUE;
  }
  return FALSE;
}

static gboolean color_button_pressed_callback(GtkWidget *ColorButton, gpointer *data){
  colors_t *destColor= (colors_t *)data;
  GdkRGBA *srcColor = (GdkRGBA *)malloc(sizeof(GdkRGBA));
  gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(ColorButton), srcColor);
  GdkRGBA_to_colors_t(srcColor, destColor);
  gtk_widget_queue_draw(board_info->widget);
  //gdk_rgba_free(srcColor);
  return FALSE;
}

static gboolean exit_color_window_callback(GtkWidget *ExitButton, gpointer *window){
  IGNORE(ExitButton);
  if(window != NULL){
    g_object_unref(window);
    gtk_widget_destroy(GTK_WIDGET(window));
  }
  return FALSE;
}

static gboolean copy_FEN_callback(GtkWidget *CopyFENButton, gpointer *data){
  IGNORE(data);
  IGNORE(CopyFENButton);
  return FALSE;
}
static gboolean copy_image_callback(GtkWidget *CopyImageButton, gpointer *data){
  IGNORE(data);
  IGNORE(CopyImageButton);
  return FALSE;
}
static gboolean show_undefended_pieces_callback(GtkWidget *UndefPiecesButton, gpointer *data){
  IGNORE(data);
  IGNORE(UndefPiecesButton);
  return FALSE;
}
static gboolean show_evaluation_bar_callback(GtkWidget *EvalBarButton, gpointer *data){
  IGNORE(data);
  IGNORE(EvalBarButton);
  return FALSE;
}
static gboolean select_mode_callback(GtkWidget *SelectModeButton, gpointer *data){
  IGNORE(data);
  IGNORE(SelectModeButton);
  return FALSE;
}

void load_svgs(char *dir, GError **err) {
  uint len = strlen(dir) + 8; // e.g.: "w_k.svg\0"
  char str[len];
  char piece_letters[] = "pnbrqk";
  char side_letters[] = "bw";

  clean_rsvg();
  for (uint i = 0; i < 2; i++) {
    char side = side_letters[i];
    for (uint j = 0; piece_letters[j] != '\0'; j++) {
      sprintf(str, "%s%c_%c.svg", dir, side, piece_letters[j]);

      piece_images[i][j] = rsvg_handle_new_from_file(str, err);
      if (*err != NULL) {
        printf("Error encountered while loading the SVGs.\n");
        return;
      }
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
