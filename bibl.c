#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
//#include <getopt.h>

#include "prettyprint.h"
//#include "util.h"

int main(int argc, char* argv[]) {
  initscr();
  keypad(stdscr, TRUE); // we need to take in UP,DOWN,LEFT,RIGHT keys and vim-like hjkl keys to scroll
  nonl(); cbreak(); // modify char input

  char* confdir;
  #ifndef _WIN32 // Unix
  confdir = getenv("HOME");
  #endif
  #ifdef _WIN32
  confdir = getenv("AppData");
  #endif
  if(confdir == NULL) {
    print_error("%s: could not get home dir/appdata dir from environment", argv[0]);
    return 2;
  }

  #ifndef _WIN32 //Unix
  strcat(confdir, "/.config");
  #endif

  if(!opendir(confdir)) { // if $HOME/.config does not exist
    #ifndef _WIN32 // Unix
    if(mkdir(confdir, 0700) == -1)
    #endif
    #ifdef _WIN32
    if(mkdir(confdir) == -1)
    #endif
    {
      endwin();
      print_error("creating dir \"%s\" failed\n", confdir);
      return 1;
    }
    printw("created non existent config directory \"%s\"\n", confdir);
  }

  int first_time_setup = 0;
  char cfg_filename[255] = {}; // ISO C forbids empty init? TODO replace {} with something more c99 compliant
  strcat(cfg_filename, confdir);
  strcat(cfg_filename, "/bibl.cfg");

  FILE* fileptr = fopen(cfg_filename, "wx");
  if(fileptr == NULL) // if our config file doesn't exist
    first_time_setup = 1;

  /* TODO first time setup
   * ask user for default language
   * ask user for screen margins
   * ask user for single screen or dual screen view
  */

  // if(first_time_setup)

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
