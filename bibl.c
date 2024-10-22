#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include <stdlib.h>
//#include <string.h>
//#include <getopt.h>

#include "prettyprint.h"
//#include "util.h"

int main(int argc, char* argv[]) {
  initscr();
  keypad(stdscr, TRUE); // we need to take in UP,DOWN,LEFT,RIGHT keys and vim-like hjkl keys to scroll
  nonl(); cbreak(); // modify char input

  char* homedir = getenv("HOME");
  if(homedir == NULL) {
    print_error("%s: could not get home dir from environment", argv[0]);
    return 2;
  }

  // 2 lines below are curses testing, not part of the actual program
  printw(homedir);
  getch();

  // TODO check if $HOME/.config exists: if not create directory
  // TODO check if $HOME/.config/bibl.cfg exists: if not go into first time setup

  /* TODO first time setup
   * ask user for default language
   * ask user for screen margins
   * ask user for single screen or dual screen view
  */

  // TODO error if could not write $HOME/.config/bibl.cfg

  // TODO load appropriate language bible

  /* TODO keybinds:
   * right arrow/h -- scroll upwards
   * down arrow/j  -- scroll downwards
   * up arrow/k    -- scroll right
   * left arrow/l  -- scroll upwards
   * !             -- control menu
  */

  endwin();
  return 0;
}
