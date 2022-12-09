#include <gtk/gtk.h>

// Variables
GtkWidget* window;
GtkWidget* image;

// Main function.
int main (int argc, char *argv[])
{
    // Initializes GTK.
    gtk_init(NULL, NULL);

    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "ui.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    gtk_builder_connect_signals(builder, NULL);


    // Gets the widgets.
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main-window"));
    image = GTK_WIDGET(gtk_builder_get_object(builder, "main-picture"));


    // Connects signal handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);


    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}


// Function with button_quit
void quit_app(GtkButton* b)
{
    gtk_main_quit();
}

// Function with button_load-image
void click_load_image(GtkButton* b)
{
    GtkWidget* dialog;
    gint result;
    char* filename;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    dialog = gtk_file_chooser_dialog_new("Open File", GTK_WINDOW(window),
            action, ("Cancel"), GTK_RESPONSE_CANCEL, ("Open"),
            GTK_RESPONSE_ACCEPT, NULL);
    result = gtk_dialog_run(GTK_DIALOG (dialog));
    if (result == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser* chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);
    }

    gtk_widget_destroy(dialog);
    if (result == GTK_RESPONSE_ACCEPT)
        gtk_image_set_from_file(GTK_IMAGE(image), filename);
}
