#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include <panel.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
//#include <getopt.h>

#include "prettyprint.h"
//#include "util.h"

//int main(int argc, char* argv[]) {
int main(void) {
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
    print_error("could not get home dir/appdata dir from environment");
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
    refresh();
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
   * left arrow/h  -- previous chapter
   * right arrow/l -- next chapter
   * up arrow/k    -- scroll right
   * down arrow/j  -- scroll downwards
   * !             -- control menu (mostly config editor)
   * q             -- quit
  */

  int scr_x, scr_y; // screen size variables
  getmaxyx(stdscr, scr_y, scr_x);

  // TODO the four variables below will be configurable in the configuration file
  int h_margin = 7; // horizontal character margin (==space between screen border and window border)
  int v_margin = 5; // same but vertical
  int v_padding = 1; // horizontal character padding (==space between window border and text)
  int h_padding = 2; // same but vertical

  int height = scr_y - (2*v_margin);
  int width = scr_x - (2*h_margin);
  WINDOW* main_window    = newwin(height, width, v_margin, h_margin);
  WINDOW* reading_window = derwin(main_window, height-(v_padding*2), width-(2+h_padding*2), v_padding+1, h_padding+1);

  mvprintw(scr_y-1, 0, "bibl v0.01a");
  box(main_window, 0, 0);

  wprintw(reading_window, "Genesis\n\n  1.1 In the beginning God created the heavens and the earth.\n  1.2 Now the earth was formless and empty, darkness was over the surface of the deep, and the Spirit of God was hovering over the waters.\n  1.3 And God said, \"Let there be light,\" and there was light.\n  1.4 God saw that the light was good, and he separated the light from the darkness.\n  1.5 God called the light \"day\", and the darkness he called \"night\". And there was evening, and there was morning--the first day.\n  1.6 And God said, \"Let there be a vault between the waters to separate water from water.\"\n  1.7 So God made the vault and separated the water under the vault from the water above it. And it was so.\n  1.8 God called the vault \"sky.\" And there was evening, and there was morning--the second day.\n  1.9 And God said, \"Let the water under the sky be gathered to one place, and let dry ground appear.\" And it was so.\n  1.10 God called the dry ground \"land\", and the gathered waters he called \"seas\". And God saw that it was good.\n  1.11 Then God said, \"Let the land produce vegetation: seed-bearing plants and trees on the land that bear fruit with seed in it, according to their various kinds.\" And it was so.\n  1.12 The land produced vegetation: plants bearing seed according to their kinds and trees bearing fruit with seed in it according to their kinds. And God saw that it was good.\n  1.13 And there was evening, and there was morning--the third day.\n  1.14 And God said, \"Let there be lights in the vault of the sky to separate the day from the night, and let them serve as signs to mark sacred times, and days and years,\n  1.15 and let them be lights in the vault of the sky to give light on the earth.\" And it was so.\n  1.16 God made two great lights--the greater light to govern the day and the lesser light to govern the night. He also made the stars.\n  1.17 God set them in the vault of the sky to give light on the earth,\n  1.18 to govern the day and the night, and to separate light from darkness. And God saw that it was good.\n  1.19 And there was evening, and there was morning--the fourth day.\n  1.20 And God said, \"Let the water teem with living creatures, and let birds fly above the earth across the vault of the sky.\"\n  1.21 So God created the great creatures of the sea and every living thing with which the water teems and that moves about in it, according to their kinds, and every winged bird according to its kind. And God saw that it was good.\n  1.22 God blessed them and said, \"Be fruitful and increase in number and fill the water in the seas, and let the birds increase on the earth.\"\n  1.23 And there was evening, and there was morning--the fifth day.\n  1.24 And God said, \"Let the land produce living creatures according to their kinds: the livestock, the creatures that move along the ground, and the wild animals, each according to its kind.\" And it was so.\n  1.25 God made the wild animals according to their kinds, the livestock according to their kinds, and all the creatures that move along the ground according to their kinds. And God saw that it was good.\n"); // TODO word wrapping maybe ?
  refresh();
  wrefresh(reading_window);
  wrefresh(main_window);
  getch();

  wclear(reading_window);
  wrefresh(reading_window);
  delwin(reading_window);
  endwin();

  return 0;
}
