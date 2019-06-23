#include <gtk/gtk.h>

struct veikk_option_panel {
  const gchar *title;
  GtkWidget *box, *label;
};

static void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *window,
            *layout_box,
            *sidebar,
            *stack,
            *label1;
  gint i, panels_count;
  struct veikk_option_panel panels[] = {
    { .title = "Orientation" },
    { .title = "Screen Mapping" },
    { .title = "Pressure Mapping" }
  };
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
    panels[i].box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_stack_add_titled(GTK_STACK(stack), panels[i].box, panels[i].title, panels[i].title);

    panels[i].label = gtk_label_new(panels[i].title);
    gtk_box_pack_start(GTK_BOX(panels[i].box), panels[i].label, 0, 0, 0);
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
