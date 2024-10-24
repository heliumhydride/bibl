#define _XOPEN_SOURCE   600
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include <menu.h>
#include <stdlib.h>
#include <string.h>
//#include <getopt.h>

#include "config.h"
#include "prettyprint.h"
//#include "util.h"

char* read_file_to_buf(FILE* fileptr) {
  char* buf;
  if(NULL == fileptr)
    return NULL;
  if(fseek(fileptr, 0L, SEEK_END) == 0) {
    ssize_t filesize = ftell(fileptr);
    if(filesize == -1) {
      return NULL;
    }
    buf = malloc(sizeof(char) * (filesize + 1));
    if(fseek(fileptr, 0L, SEEK_SET) != 0) {
      return NULL;
    }
    size_t newlen = fread(buf, sizeof(char), filesize, fileptr);
    if(ferror(fileptr) != 0) {
      return NULL;
    } else {
      buf[newlen++] = '\0';
    }
  }
  return buf;
}

void scroll_up(void) {
  // TODO scroll up onscreen text
}

void scroll_down(void) {
  // TODO scroll down onscreen text
}

void next_chapter(void) {
  // TODO load next chapter if we are not at the last one
}

void prev_chapter(void) {
  // TODO load prev chapter if we are not at the first one
}

void show_help(void) {
  attrset(A_BOLD);      printw("// command help //\n\n");
  attrset(A_UNDERLINE); printw("up arrow / k");
  attrset(A_NORMAL);    printw(": scroll text up\n");
  attrset(A_UNDERLINE); printw("down arrow / j");
  attrset(A_NORMAL);    printw(": scroll text down\n");
  attrset(A_UNDERLINE); printw("right arrow / l");
  attrset(A_NORMAL);    printw(": scroll text up\n");
  attrset(A_UNDERLINE); printw("left arrow / h");
  attrset(A_NORMAL);    printw(": scroll text up\n");
  attrset(A_UNDERLINE); printw("!");
  attrset(A_NORMAL);    printw(": control menu\n");
  attrset(A_UNDERLINE); printw("q/ESC");
  attrset(A_NORMAL);    printw(": quit\n");
  refresh();
  (void)getch();
}

int menu_loop(MENU* menu) {
  post_menu(menu);
  int item_count = 5;
  int ch;
  int quit = 0;
  int select = 0;
  while(!quit) {
    ch = getch();
    switch(ch) {
      case KEY_DOWN: // fallthrough
      case 'j':
        menu_driver(menu, REQ_DOWN_ITEM);
        if(select < item_count)
          select++;
        break;
      case KEY_UP: // fallthrough
      case 'k':
        menu_driver(menu, REQ_UP_ITEM);
        if(select > 0)
          select--;
        break;
      case 27: // ESC key
        select = -1;
        quit = 1;
        break;
      case '\r': case '\n': // ENTER key
        quit = 1;
        break;
      default:
        menu_driver(menu, ch);
        menu_driver(menu, REQ_NEXT_MATCH);
        break;
    }
    refresh();
  }
  for(int i = 0; i < item_count; i++)
    menu_driver(menu, REQ_UP_ITEM); // reset menu item selection

  unpost_menu(menu);
  return select;
}

//int main(int argc, char* argv[]) {
int main(void) {
  initscr();
  keypad(stdscr, TRUE); // we need to take in UP,DOWN,LEFT,RIGHT keys and vim-like hjkl keys to scroll
  nonl(); raw(); // modify char input
  curs_set(0);
  noecho();

  int scr_x, scr_y; // screen size variables
  getmaxyx(stdscr, scr_y, scr_x);

  int height = scr_y - (2*v_margin);
  int width = scr_x - (2*h_margin);
  WINDOW* main_window    = newwin(height, width, v_margin, h_margin);
  WINDOW* reading_window = derwin(main_window, height-(v_padding*2), width-(2+h_padding*2), v_padding+1, h_padding+1);

  char* bible_buffer;
  bible_buffer = "Genesis\n\n  1.1 In the beginning God created the heavens and the earth.\n  1.2 Now the earth was formless and empty, darkness was over the surface of the deep, and the Spirit of God was hovering over the waters.\n  1.3 And God said, \"Let there be light,\" and there was light.\n  1.4 God saw that the light was good, and he separated the light from the darkness.\n  1.5 God called the light \"day\", and the darkness he called \"night\". And there was evening, and there was morning--the first day.\n  1.6 And God said, \"Let there be a vault between the waters to separate water from water.\"\n  1.7 So God made the vault and separated the water under the vault from the water above it. And it was so.\n  1.8 God called the vault \"sky.\" And there was evening, and there was morning--the second day.\n  1.9 And God said, \"Let the water under the sky be gathered to one place, and let dry ground appear.\" And it was so.\n  1.10 God called the dry ground \"land\", and the gathered waters he called \"seas\". And God saw that it was good.\n  1.11 Then God said, \"Let the land produce vegetation: seed-bearing plants and trees on the land that bear fruit with seed in it, according to their various kinds.\" And it was so.\n  1.12 The land produced vegetation: plants bearing seed according to their kinds and trees bearing fruit with seed in it according to their kinds. And God saw that it was good.\n  1.13 And there was evening, and there was morning--the third day.\n  1.14 And God said, \"Let there be lights in the vault of the sky to separate the day from the night, and let them serve as signs to mark sacred times, and days and years,\n  1.15 and let them be lights in the vault of the sky to give light on the earth.\" And it was so.\n  1.16 God made two great lights--the greater light to govern the day and the lesser light to govern the night. He also made the stars.\n  1.17 God set them in the vault of the sky to give light on the earth,\n  1.18 to govern the day and the night, and to separate light from darkness. And God saw that it was good.\n  1.19 And there was evening, and there was morning--the fourth day.\n  1.20 And God said, \"Let the water teem with living creatures, and let birds fly above the earth across the vault of the sky.\"\n  1.21 So God created the great creatures of the sea and every living thing with which the water teems and that moves about in it, according to their kinds, and every winged bird according to its kind. And God saw that it was good.\n  1.22 God blessed them and said, \"Be fruitful and increase in number and fill the water in the seas, and let the birds increase on the earth.\"\n  1.23 And there was evening, and there was morning--the fifth day.\n  1.24 And God said, \"Let the land produce living creatures according to their kinds: the livestock, the creatures that move along the ground, and the wild animals, each according to its kind.\" And it was so.\n  1.25 God made the wild animals according to their kinds, the livestock according to their kinds, and all the creatures that move along the ground according to their kinds. And God saw that it was good.\n";
  // TODO load this from a file


  ITEM* menuitems[5];
  MENU* menu;
  menuitems[0] = new_item("Exit", "q/ESC");
  menuitems[1] = new_item("Search", "CTRL-F");
  menuitems[2] = new_item("Help", "");
  menuitems[3] = new_item("About", "");
  menuitems[4] = NULL;
  menu = new_menu((ITEM**)menuitems);

  int quit = 0;
  int ch;
  while(!quit) {
    mvprintw(scr_y-1, 0, "bibl " VERSION);
    move(scr_y-1, (int)scr_x/2-10);
    box(main_window, 0, 0);
    wprintw(reading_window, bible_buffer); // TODO word wrapping maybe ?
    refresh();
    wrefresh(reading_window);
    wrefresh(main_window);

  // TODO load appropriate language bible into a buffer using read_file_to_buf function

    ch = getch();
    switch(ch) {
      case KEY_UP: // fallthrough
      case 'k':
        scroll_up(); // TODO implement
        break;
      case KEY_DOWN: // fallthrough
      case '\r': case '\n': // double fallthrough; for the ENTER key (because KEY_ENTER failed) (test on MS/Windows)
      case 'j':
        scroll_down(); // TODO implement
        break;
      case KEY_RIGHT: // fallthrough
      case 'l':
        next_chapter(); // TODO implement
        break;
      case KEY_LEFT: // fallthrough
      case 'h':
        prev_chapter(); // TODO implement
        break;
      case '!':
        switch(menu_loop(menu)) {
          case 0: // menu item == Exit
            quit = 1;
            break;
          case 1: // menu item == Search
            // TODO implement searching in buffer and autoscroll
            break;
          case 2: // menu item == Help
            show_help();
            break;
          case 3: // menu item == About
            attrset(A_BOLD);   printw("// about bibl //\n\n");
            attrset(A_NORMAL); printw("  Bible reading app for your terminal\n  version " VERSION);
            refresh();
            (void)getch();
            break;
        }
        break;
      case 'q': // fallthrough
      case 27:  // ESC key
        quit = 1;
        break;
      case 'c' & 037: // CTRL+C
        printw("Press ");
        attrset(A_BOLD); printw("q");
        attrset(A_NORMAL); printw(" to quit");
        (void)getch();
        break;
    }
    clear();
    wclear(reading_window);
    wclear(main_window);
  }

  // cleanup ncurses
  unpost_menu(menu);
  clear();
  wclear(reading_window);
  wrefresh(reading_window);
  delwin(reading_window);
  endwin();

  return 0;
}
