// This is a pretty simple program, so no need for
// pre-processing directives or anything; this file is
// included once by every source file
#include <gtk/gtk.h>

// enums
enum setting { SET_ORIENTATION, SET_MAP_SCREEN, SET_MAP_PRESSURE };
enum orientation { ORI_DEFAULT, ORI_ROT_RIGHT, ORI_REVERSED, ORI_ROT_LEFT };

// structs
struct veikk_option_panel {
  const gchar *title;
  GtkWidget *box;
};
struct select_data {
  GdkRectangle rect;
  gboolean started;
  GtkWidget *window;
};

// globals variables
extern void (*customize_panel[])();

// global functions
void write_setting(enum setting, const char *);
void select_area(struct select_data *);
