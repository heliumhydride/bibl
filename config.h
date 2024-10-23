#ifndef CONFIG_H
#define CONFIG_H

#define VERSION "v0.01a"

static const int h_margin = 7; // horizontal character margin (==space between screen border and window border)
static const int v_margin = 5; // same but vertical
static const int v_padding = 1; // horizontal character padding (==space between window border and text)
static const int h_padding = 2; // same but vertical

static const char* default_translation = "en_nkjv";
static const char* bible_directory = "/usr/share/bibl/"; // On Windows, could be set to "C:/Program Files/bibl/share/bibl" ?

#endif
