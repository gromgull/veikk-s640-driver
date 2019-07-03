#include "config.h"

static gboolean select_window_button_press(GtkWidget *window,
                                           GdkEventButton *event,
                                           struct select_data *select_data) {
  if(select_data->started) {
    return TRUE;
  }

  select_data->started = TRUE;
  select_data->rect.x = event->x_root;
  select_data->rect.y = event->y_root;

  return TRUE;
}

static gboolean select_window_button_release(GtkWidget *window,
                                             GdkEventButton *event,
                                             struct select_data *select_data) {
  if(!select_data->started) {
    return TRUE;
  }
  
  select_data->rect = (GdkRectangle) {
    .x = MIN(select_data->rect.x, event->x_root),
    .y = MIN(select_data->rect.y, event->y_root),
    .width = ABS(select_data->rect.x - event->x_root),
    .height = ABS(select_data->rect.y - event->y_root)
  };

  // TODO: handle height/width of 0
  gtk_main_quit();

  return TRUE;
}

static gboolean select_window_motion_notify(GtkWidget *window,
                                            GdkEventMotion *event,
                                            struct select_data *select_data) {
  GdkRectangle rect;

  if(!select_data->started) {
    return TRUE;
  }

  rect = (GdkRectangle) {
    .x = MIN(select_data->rect.x, event->x_root),
    .y = MIN(select_data->rect.y, event->y_root),
    .width = ABS(select_data->rect.x - event->x_root),
    .height = ABS(select_data->rect.y - event->y_root)
  };

  // TODO: handle width, height <= 0 like in gnome-screenshot?
  
  gtk_window_move(GTK_WINDOW(window), rect.x, rect.y);
  gtk_window_resize(GTK_WINDOW(window), rect.width, rect.height);

  // TODO: app-paintable stuff?
}

static gboolean select_window_draw(GtkWidget *window,
                                   cairo_t *cr,
                                   gpointer unused) {
  GtkStyleContext *style;

  style = gtk_widget_get_style_context(window);

  if(gtk_widget_get_app_paintable(window)) {
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
    cairo_set_source_rgba(cr, 0, 0, 0, 0);

    cairo_paint(cr);

    gtk_style_context_save(style);
    gtk_style_context_add_class(style, GTK_STYLE_CLASS_RUBBERBAND);

    gtk_render_background(style, cr, 0, 0,
        gtk_widget_get_allocated_width(window),
        gtk_widget_get_allocated_height(window));

    gtk_style_context_restore(style);
  }

  return TRUE;
}

// guided by https://gitlab.gnome.org/GNOME/gnome-screenshot project
// screen-shot-area-selection.c file
void select_area(struct select_data *select_data) {
  GtkWidget *window;
  GdkScreen *screen;
  GdkVisual *visual;
  GdkDisplay *display;
  GdkCursor *cursor;
  GdkDeviceManager *manager;
  GdkDevice *pointer;
  GdkGrabStatus grab_status;

  // now for testing TODO: later change to other, non-arbitrary values
  select_data->rect = (GdkRectangle) {
    .x = 100,
    .y = 100,
    .width = 100,
    .height = 100
  };
  select_data->started = FALSE;

  screen = gdk_screen_get_default();
  visual = gdk_screen_get_rgba_visual(screen);

  window = gtk_window_new(GTK_WINDOW_POPUP);
  select_data->window = window;
  if(gdk_screen_is_composited(screen) && visual) {
    gtk_widget_set_visual(window, visual);
    gtk_widget_set_app_paintable(window, TRUE);
  }

  g_signal_connect(window, "draw", G_CALLBACK(select_window_draw), NULL);
  g_signal_connect(window, "motion-notify-event", 
                   G_CALLBACK(select_window_motion_notify), select_data);
  g_signal_connect(window, "button-press-event", 
                   G_CALLBACK(select_window_button_press), select_data);
  g_signal_connect(window, "button-release-event", 
                   G_CALLBACK(select_window_button_release), select_data);

  // TODO: change to other, non-arbitrary values
  gtk_window_move(GTK_WINDOW(window), 100, 100);
  gtk_window_resize(GTK_WINDOW(window), 100, 100);
  gtk_widget_show(window);

  // grab pointer
  display = gtk_widget_get_display(select_data->window);
  cursor = gdk_cursor_new_for_display(display, GDK_CROSSHAIR);
  manager = gdk_display_get_device_manager(display);
  pointer = gdk_device_manager_get_client_pointer(manager);

  grab_status = gdk_device_grab(pointer,
                                gtk_widget_get_window(select_data->window),
                                GDK_OWNERSHIP_NONE, FALSE,
                                GDK_POINTER_MOTION_MASK | GDK_BUTTON_PRESS_MASK
                                | GDK_BUTTON_RELEASE_MASK,
                                cursor, GDK_CURRENT_TIME);

  if(grab_status != GDK_GRAB_SUCCESS) {
    g_object_unref(cursor);
    g_print("gdk grab failure");
    // TODO: abort
    return;
  }

  // don't need to grab keyboard as well

  gtk_main();

  gdk_device_ungrab(pointer, GDK_CURRENT_TIME);
  gtk_widget_destroy(select_data->window);
  g_object_unref(cursor);
  gdk_flush();
}
