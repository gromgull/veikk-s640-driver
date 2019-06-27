#include <gtk/gtk.h>
#include <stdlib.h>

// struct to add another panel
struct veikk_option_panel {
  const gchar *title;
  GtkWidget *box, **widgets;
};

// orientation panel
static void customize_panel_1(struct veikk_option_panel *panel) {
  GtkWidget *label, *grid,
            *rad0, *rad1, *rad2, *rad3;

  panel->title = "Orientation";

  label = gtk_label_new(panel->title);
  gtk_box_pack_start(GTK_BOX(panel->box), label, 0, 0, 0);

  grid = gtk_grid_new();
  gtk_box_pack_start(GTK_BOX(panel->box), grid, 0, 0, 0);

  rad0 = gtk_radio_button_new_with_label(NULL, "Default");
  rad1 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(rad0), "Orientation 2");
  rad2 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(rad0), "Orientation 3");
  rad3 = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(rad0), "Orientation 4");

  gtk_grid_attach(GTK_GRID(grid), rad0, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), rad1, 1, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), rad2, 0, 1, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), rad3, 1, 1, 1, 1);

  // create a grid pane of the four orientations
  // TODO: add images / graphics
  panel->widgets = (GtkWidget *[6]) { label, grid, rad0, rad1, rad2, rad3 };
}
// screen mapping panel
static void customize_panel_2(struct veikk_option_panel *panel) {
  panel->title = "Screen mapping";
}
// pressure mapping panel
static void customize_panel_3(struct veikk_option_panel *panel) {
  panel->title = "Pressure mapping";
}
void (*customize_panel[])() = {
  customize_panel_1,
  customize_panel_2,
  customize_panel_3
};

static void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *window,
            *layout_box,
            *sidebar,
            *stack;
  gint i, panels_count;
  struct veikk_option_panel panels[3];
  panels_count = sizeof panels / sizeof (struct veikk_option_panel);
  
  // window and layout box as general layout
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Window");
  gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);

  layout_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  gtk_container_add(GTK_CONTAINER(window), layout_box);

  // sidebar widget automatically adds headers, makes switching easy
  sidebar = gtk_stack_sidebar_new();
  gtk_box_pack_start(GTK_BOX(layout_box), sidebar, 0, 0, 0);

  // create stack of option panels
  stack = gtk_stack_new();
  gtk_stack_sidebar_set_stack(GTK_STACK_SIDEBAR(sidebar), GTK_STACK(stack));
  gtk_box_pack_start(GTK_BOX(layout_box), stack, 0, 0, 0);

  // add options
  for(i = 0; i < panels_count; i++) {
    // create box and customize panel
    panels[i].box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    customize_panel[i](&panels[i]);

    // add panel to stack
    gtk_stack_add_titled(GTK_STACK(stack), panels[i].box, panels[i].title, panels[i].title);
  }

  // show
  gtk_widget_show_all(window);
}

// main to set up GTK application (main details in activate fn)
int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
