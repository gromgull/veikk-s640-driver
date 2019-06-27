// all files need gtk headers
#include <gtk/gtk.h>

// enums
enum setting { SET_ORIENTATION, SET_MAP_SCREEN, SET_MAP_PRESSURE };
enum orientation { ORI_DEFAULT, ORI_ROT_RIGHT, ORI_REVERSED, ORI_ROT_LEFT };

// structs
// struct veikk_option_panel is for every option page
struct veikk_option_panel {
  const gchar *title;
  GtkWidget *box, **widgets;
};

// globals variables
extern void (*customize_panel[])();

// global functions
void write_setting(enum setting, const char *);
