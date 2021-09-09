#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <board.h>
#include <gtk/gtk.h>
#include <utils.h>
#ifndef RSVG_CAIRO_H_
#include <librsvg/rsvg.h>
#include <librsvg/rsvg-cairo.h>
#endif

GtkWidget *displayControl();
void makeBoard(GtkWidget *);
void makePieces(GtkWidget *, char *);
void updateCanvas(GtkWidget *, char *, char *, char *, char *, char *, char *);
void makePlayerDetails(GtkWidget *, GtkWidget *, GtkWidget *, GtkWidget *,
                       char *, char *, char *);
void makeCoordinates(GtkWidget *);
void makeArrows(GtkWidget *);
void makeControlButtonsArray(GtkWidget *);
void updateAllLabelTexts(lichess_data_t *);
void updateLabelTexts(GtkWidget *, char *);
void load_svgs(char *, GError **);
void showPieces(piece_info_t *);

#endif // include guard
