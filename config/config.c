#include "config.h"

// write to a setting
void write_setting(enum setting setting, const char *setting_text) {
  FILE *sysfs_param_handle = NULL;
  
  switch(setting) {
    case SET_ORIENTATION:
      sysfs_param_handle = fopen("/sys/module/veikk/parameters/orientation",
                                 "w");
      break;
    case SET_MAP_SCREEN:
      sysfs_param_handle = fopen("/sys/module/veikk/parameters/map_screen",
                                 "w");
      break;
    default:
      return g_print("Write to other settings not implemented yet.\n");
  }

  if(sysfs_param_handle == NULL) {
    g_print("Error: could not open file. Make sure to run as root.\n");
  } else {
    fprintf(sysfs_param_handle, "%s", setting_text);
    fclose(sysfs_param_handle);
    g_print("Setting written: %s.\n", setting_text);
  }
}

// set up gtk application
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

    gtk_stack_add_titled(GTK_STACK(stack), panels[i].box,
                         panels[i].title, panels[i].title);
  }

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
