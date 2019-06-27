#include <stdlib.h>
#include "config.h"

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
  rad1 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(rad0), "Rotated right");
  rad2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(rad0), "Reversed (left-handed)");
  rad3 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(rad0), "Rotated left");

  gtk_grid_attach(GTK_GRID(grid), rad0, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), rad1, 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), rad2, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), rad3, 1, 1, 1, 1);

  // TODO: read from sysfs orientation parameter, set with existing value

  g_signal_connect(rad0, "toggled", G_CALLBACK(orientation_radio_callback), (gpointer) ORI_DEFAULT);
  g_signal_connect(rad1, "toggled", G_CALLBACK(orientation_radio_callback), (gpointer) ORI_ROT_RIGHT);
  g_signal_connect(rad2, "toggled", G_CALLBACK(orientation_radio_callback), (gpointer) ORI_REVERSED);
  g_signal_connect(rad3, "toggled", G_CALLBACK(orientation_radio_callback), (gpointer) ORI_ROT_LEFT);

  // create a grid pane of the four orientations
  // TODO: add images / graphics
  panel->widgets = (GtkWidget *[6]) { label, grid, rad0, rad1, rad2, rad3 };
}
// screen mapping panel
void customize_panel_2(struct veikk_option_panel *panel) {
  panel->title = "Screen mapping";
}
// pressure mapping panel
void customize_panel_3(struct veikk_option_panel *panel) {
  panel->title = "Pressure mapping";
}
void (*customize_panel[])() = {
  customize_panel_1,
  customize_panel_2,
  customize_panel_3
};
