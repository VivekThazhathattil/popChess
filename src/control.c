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

#include <control.h>

lichess_data_t lichessData;

int run(GtkApplication *app, int argc, char *argv[]) {
  gtk_init(&argc, &argv);
  initCurl();

  GtkWidget *window; //*playerName, *playerRating, *playerClock;
  display_output_t *display_output;

  window = gtk_application_window_new(app);
  if((display_output = displayControl()) == NULL){
    return 0;
  }

  g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);

  gtk_container_add(GTK_CONTAINER(window), display_output->canvas);
  if (setWindowProps(window) != 1) {
    printf("Setting window props failed. Exiting...");
    exit(0);
  }
  showWindow(window);

  freePieceInfo(display_output->board_info->piece_info);
  freeBoardInfo(display_output->board_info);
  freeDisplayOutput(display_output);
  destroyLichessData();
  return 1;
}

void triggerFENReceived(char *fenFeed) {
  char *lastJSON = getLastJSON(fenFeed);
  if (isNewGame(lastJSON)) {
    if (!fillGameInfo(&lichessData, lastJSON)) {
      printf("Error filling the game info\n");
    }
    //    printf("TEST:\n");
    //    printf("White name: %s, Black name: %s\n", lichessData.white.name,
    //           lichessData.black.name);
    //    printf("White rating: %s, Black rating: %s\n",
    //    lichessData.white.rating,
    //           lichessData.black.rating);
    //    printf("White title: %s, Black title: %s\n", lichessData.white.title,
    //           lichessData.black.title);
    //
    updateAllLabelTexts(&lichessData);
  }

  char *fen = getFenFromJson(lastJSON);
  if(strcmp(fen, "failed")){ // fen =/= "failed"
    piece_info_t pieceInfo[32];
    getPiecePositions(fen, pieceInfo);
    // printf("%s\n", fen);
    showPieces(pieceInfo);
  }
  else
    setFenInactive();
  free(fen);
}

int setWindowProps(GtkWidget *win) {
  gtk_widget_set_size_request(win, 300, 300);
  gtk_window_set_title(GTK_WINDOW(win), "popChess");
  gtk_window_set_decorated(GTK_WINDOW(win), FALSE);
  return 1;
}

void showWindow(GtkWidget *win) {
  gtk_widget_show_all(win);
  gtk_main();
  return;
}

void destroyLichessData() {
  free(lichessData.white.name);
  free(lichessData.white.title);
  free(lichessData.white.rating);
  free(lichessData.white.timeLeft);
  free(lichessData.black.name);
  free(lichessData.black.title);
  free(lichessData.black.rating);
  free(lichessData.black.timeLeft);
}
