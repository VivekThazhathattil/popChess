## Issues:

* ~~Fix mouse out of focus, drawing area redraw~~
* ~~Fix blanking pieces issue~~
* ability to adjust size
* ~~recursively travel the JSON to get the required fields. Current method is too rigid. Can lead to problems when patreons are playing.~~
* sometimes pieces are displayed as their tiny versions. Don't know why it happens
* Need some mechanism in place to recover from broken network connection
* Fix escape button exit seg fault
* fix bad svgs and cairo panic errors

* occurence of "Slice: assertion failed: sinfo->n_allocated > 0" error observed once

## enhancements:

* pop effect
* ~game clock~
* let the clock tick down on its own
* Make the clock actually look like a clock
* select bullet, blitz, rapid, and other modes
* select a user to follow
* broadcasts
* sounds
* increase the font size of all text labels
* ~~button to flip board~~
* button to enable/disable sound
* text entry field to follow a player's games
* drop down menu to select which top mode to watch (rapid/bullet/ultrabullet ...)
* ~~colorwheel to choose the board colors~~
* ~~highlight last move squares~~
* button to enable/disable arrows
* Try to make it a transparent widget if possible
* ~~Change pieces~~
* Integrate a chess-engine to evaluate the current position
* Create bar display similar to lichess and chess.com
* make a status icon in the system tray which when hovered shows the ongoing game in a tiny window

## IMPORTANT:
* Write unit tests
* routine check for memory leaks
