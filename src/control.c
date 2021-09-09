#include <control.h>

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
  destroyLichessData();
  return 1;
}

void triggerFENReceived(char *fenFeed) {
  char *lastJSON = getLastJSON(fenFeed);
  char *fen = getFenFromJson(lastJSON);
  printf("%s\n", fen);

  if (isNewGame(lastJSON)) {
    if (!fillGameInfo(&lichessData, lastJSON)) {
      printf("Error filling the game info\n");
    }
    printf("TEST:\n");
    printf("White name: %s, Black name: %s\n", lichessData.white.name,
           lichessData.black.name);
    printf("White rating: %s, Black rating: %s\n", lichessData.white.rating,
           lichessData.black.rating);
    printf("White title: %s, Black title: %s\n", lichessData.white.title,
           lichessData.black.title);

    // bad practice: remove duplicates
    updateAllLabelTexts(&lichessData);
  }

  //  else{
  //    updateWithLatestData();
  //    triggerPieceChange();
  //  }
  //
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