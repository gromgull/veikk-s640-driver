#include "config.h"

// TODO: clean this up: use XML templates (?) and/or more descriptive names
// TODO: fix up indenting (is 79 char max, but looks ugly and inconsistent)

/**
 * PANEL 1
 */

// radio button callback
static void orientation_radio_callback(GtkRadioButton *rad, gpointer user_data) {
  char orientation_text[2];
  // casting from void pointer -- is this safe?
  enum orientation orientation = (enum orientation) user_data;

  // return if toggling off
  if(!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(rad))) {
    return;
  }

  // deal with changing orientation: write to sysfs parameter
  switch(orientation) {
    case ORI_DEFAULT:
      g_print("Default orientation selected.\n");
      break;
    case ORI_ROT_RIGHT:
      g_print("Right orientation selected.\n");
      break;
    case ORI_REVERSED:
      g_print("Reversed orientation selected.\n");
      break;
    case ORI_ROT_LEFT:
      g_print("Left orientation selected.\n");
      break;
    default:
      return g_print("Error: Orientation not found.\n");
  }

  sprintf(orientation_text, "%d", (gint) orientation);
  write_setting(SET_ORIENTATION, orientation_text);
}

// orientation panel
void customize_panel_1(struct veikk_option_panel *panel) {
  GtkWidget *label, *grid,
            *rad0, *rad1, *rad2, *rad3;

  panel->title = "Orientation";

  label = gtk_label_new(panel->title);
  gtk_box_pack_start(GTK_BOX(panel->box), label, 0, 0, 0);

  grid = gtk_grid_new();
  gtk_box_pack_start(GTK_BOX(panel->box), grid, 0, 0, 0);

  // TODO: make this DRY-er
  rad0 = gtk_radio_button_new_with_label(NULL, "Default");
  rad1 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(rad0),
                                                     "Rotated right");
  rad2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(rad0),
                                                     "Reversed (left-handed)");
  rad3 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(rad0),
                                                     "Rotated left");

  gtk_grid_attach(GTK_GRID(grid), rad0, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), rad1, 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), rad2, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), rad3, 1, 1, 1, 1);

  // TODO: read from sysfs orientation parameter, set with existing value

  g_signal_connect(rad0, "toggled", G_CALLBACK(orientation_radio_callback),
                   (gpointer) ORI_DEFAULT);
  g_signal_connect(rad1, "toggled", G_CALLBACK(orientation_radio_callback),
                   (gpointer) ORI_ROT_RIGHT);
  g_signal_connect(rad2, "toggled", G_CALLBACK(orientation_radio_callback), 
                   (gpointer) ORI_REVERSED);
  g_signal_connect(rad3, "toggled", G_CALLBACK(orientation_radio_callback), 
                   (gpointer) ORI_ROT_LEFT);

  // TODO: add images / graphics
}

/**
 * PANEL 2: screen mapping panel
 */

void choose_screen_area_callback(GtkWidget *button,
                                 gpointer unused) {
  gchar *screen_map_text;
  struct select_data select_data;

  // choose the area on the screen
  select_area(&select_data);

  // TODO: make some sort of confirmation

  sprintf(screen_map_text, "%d %d %d %d",
          select_data.rect.x,
          select_data.rect.y,
          select_data.rect.width,
          select_data.rect.height);
  write_setting(SET_MAP_SCREEN, screen_map_text);
}

// TODO: move this back inside
static void customize_panel_2(struct veikk_option_panel *panel) {
  GtkWidget *label, *fullscreen_map_check_button, *map_box, *map_box_label,
            *grid, **grid_parameter_labels, **grid_parameter_values, *button;

  // label
  panel->title = "Screen mapping";
  label = gtk_label_new(panel->title);
  gtk_box_pack_start(GTK_BOX(panel->box), label, 0, 0, 0);

  // check for map full screen (default)
  fullscreen_map_check_button =
    gtk_check_button_new_with_label("Map to entire screen (default)");
  gtk_box_pack_start(GTK_BOX(panel->box), fullscreen_map_check_button,
                     0, 0, 0);

  // box to hold all parameters and features for non-fullscreen mapping
  map_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
  gtk_box_pack_start(GTK_BOX(panel->box), map_box, 0, 0, 0);

  map_box_label = gtk_label_new("Map to custom screen area");
  gtk_box_pack_start(GTK_BOX(map_box), map_box_label, 0, 0, 0);

  grid = gtk_grid_new();
  gtk_box_pack_start(GTK_BOX(map_box), grid, 0, 0, 0);

  grid_parameter_labels = (GtkWidget *[14]) {
    // col 1
    gtk_label_new("Screen"),
    gtk_label_new("Drawing area"),

    // col 2
    gtk_label_new("Width"),
    gtk_label_new("Height"),
    gtk_label_new("Start x"),
    gtk_label_new("Start y"),
    gtk_label_new("Width"),
    gtk_label_new("Height"),

    // col 3
    gtk_label_new("w"),
    gtk_label_new("h"),
    gtk_label_new("tx"),
    gtk_label_new("ty"),
    gtk_label_new("tw"),
    gtk_label_new("th")
  };
  grid_parameter_values = grid_parameter_labels + 8;

  // col 1
  // TODO: turn this into some loops
  gtk_grid_attach(GTK_GRID(grid), grid_parameter_labels[0], 0, 0, 1, 2);
  gtk_grid_attach(GTK_GRID(grid), grid_parameter_labels[1], 0, 2, 1, 4);

  // col 2
  gtk_grid_attach(GTK_GRID(grid), grid_parameter_labels[2], 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), grid_parameter_labels[3], 1, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), grid_parameter_labels[4], 1, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), grid_parameter_labels[5], 1, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), grid_parameter_labels[6], 1, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), grid_parameter_labels[7], 1, 5, 1, 1);

  // col 3
  gtk_grid_attach(GTK_GRID(grid), grid_parameter_labels[8], 2, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), grid_parameter_labels[9], 2, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), grid_parameter_labels[10], 2, 2, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), grid_parameter_labels[11], 2, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), grid_parameter_labels[12], 2, 4, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), grid_parameter_labels[13], 2, 5, 1, 1);

  // button to select a new area
  button = gtk_button_new_with_label("Choose screen area");
  gtk_box_pack_start(GTK_BOX(map_box), button, 0, 0, 0);
  g_signal_connect(button, "clicked", choose_screen_area_callback, NULL);

  // callback for check button (select between default / non default)
//  g_signal_connect(fullscreen_map_check_button, "toggled",
//                   fullscreen_map_check_button_toggled, map_box);

  // TODO: load from settings file
}

/**
 * PANEL 3
 */

// pressure mapping panel
void customize_panel_3(struct veikk_option_panel *panel) {
  panel->title = "Pressure mapping";
}
void (*customize_panel[])() = {
  customize_panel_1,
  customize_panel_2,
  customize_panel_3
};
