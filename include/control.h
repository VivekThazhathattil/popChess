#ifndef CONTROL_H_
#define CONTROL_H_

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>

#include <board.h>
#include <display.h>
#include <get_feed.h>
#include <parse_feed.h>
#include <utils.h>
#include <json.h>

struct PlayerAttributes{
    char* name, title, rating, timeLeft;
};
typedef struct PlayerAttributes player_attributes_t;

struct LichessData{
    player_attributes_t white, black;
};
typedef struct LichessData lichess_data_t;

int run(int argc, char *args[]);
int setWindowProps(GtkWidget *win);
void showWindow(GtkWidget *win);
void triggerFENReceived(char *fenFeed);

#endif // include guard
