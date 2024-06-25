#include <gtk/gtk.h>
#include <math.h>
#include <time.h>

#define UPDATE_INTERVAL 5000
//gcc -o analog_clock analog_clock.c -lm `pkg-config --cflags --libs gtk+-3.0`
gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data);
gboolean update_clock(gpointer user_data);
void draw_clock(cairo_t *cr, int width, int height);

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Analog Clock");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
    gtk_widget_set_app_paintable(window, TRUE);
    //gtk_widget_override_background_color(window, GTK_STATE_FLAG_NORMAL, &(GdkRGBA){1.0, 1.0, 0.0, 1.0}); // Fundo amarelo

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(window, "draw", G_CALLBACK(on_draw_event), NULL);

    g_timeout_add(UPDATE_INTERVAL, update_clock, window);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
    GtkAllocation allocation;
    gtk_widget_get_allocation(widget, &allocation);
    int width = allocation.width;
    int height = allocation.height;
    
    draw_clock(cr, width, height);

    return FALSE;
}

gboolean update_clock(gpointer user_data) {
    gtk_widget_queue_draw(GTK_WIDGET(user_data));
    return TRUE; // Keep the timer running
}

void draw_clock(cairo_t *cr, int width, int height) {
    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    int cx = width / 2;
    int cy = height / 2;
    int radius = (width < height ? width : height) / 2 - 10;
    cairo_set_source_rgb(cr, 255, 255, 0);
    cairo_rectangle(cr,0,0,width,height);
    cairo_fill(cr);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 2.0);

    // Draw clock circle
    cairo_arc(cr, cx, cy, radius, 0, 2 * M_PI);
    cairo_stroke(cr);

    // Draw hour hand
    double hour_angle = (local->tm_hour % 12 + local->tm_min / 60.0) * (2 * M_PI / 12) - M_PI / 2;
    cairo_set_source_rgb(cr, 0, 0, 0); 
    cairo_move_to(cr, cx, cy);
    cairo_line_to(cr, cx + radius * 0.5 * cos(hour_angle), cy + radius * 0.5 * sin(hour_angle));
    cairo_stroke(cr);

    // Draw minute hand
    double minute_angle = (local->tm_min + local->tm_sec / 60.0) * (2 * M_PI / 60) - M_PI / 2;
    cairo_set_source_rgb(cr, 0, 0, 0); 
    cairo_move_to(cr, cx, cy);
    cairo_line_to(cr, cx + radius * 0.8 * cos(minute_angle), cy + radius * 0.8 * sin(minute_angle));
    cairo_stroke(cr);

    // Draw second hand
    double second_angle = local->tm_sec * (2 * M_PI / 60) - M_PI / 2;
    cairo_set_source_rgb(cr, 0, 0, 0); 
    cairo_move_to(cr, cx, cy);
    cairo_line_to(cr, cx + radius * 0.9 * cos(second_angle), cy + radius * 0.9 * sin(second_angle));
    cairo_stroke(cr);
}

