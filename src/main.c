#include "control.h"

int main(int argc, char *argv[]) {
  if (run(argc, argv) == 0)
    printf("Error encountered while running the program. Exiting...");
  return 0;
}
