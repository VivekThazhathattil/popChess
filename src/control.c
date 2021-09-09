#include "control.h"

lichess_data_t lichessData;

int run(int argc, char *argv[]) {
  gtk_init(&argc, &argv);

  initCurl();

  GtkWidget *window, *displayOutput; //*playerName, *playerRating, *playerClock;

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  displayOutput = displayControl();

  g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);

  gtk_container_add(GTK_CONTAINER(window), displayOutput);
  if (setWindowProps(window) != 1) {
    printf("Setting window props failed. Exiting...");
    exit(0);
  }
  showWindow(window);
  return 1;
}

void triggerFENReceived(char *fenFeed) {
  char *lastJSON = getLastJSON(fenFeed);
  char* fen = getFenFromJson(lastJSON);
  printf("%s\n", fen);
  //free(fen);
  //  if(isNewGame(lastJSON)){
  //    latest_data = getNewGameData(lastJSON);
  //    updateCanvasDetails();
  //  }
  //  else{
  //    updateWithLatestData();
  //    triggerPieceChange();
  //  }
  //
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
