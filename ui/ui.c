#include <gtk/gtk.h>

// Variables
GtkWidget* window;
GtkWidget* image;
GtkWidget* buttonCancel;
GtkWidget* dialog;

char* filename = "";
char* filename1 = "";
char* filenameFinal = "";
char cmd[4096];


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

    // Connect signals
    gtk_builder_connect_signals(builder, NULL);


    // Gets the widgets.
    window = GTK_WIDGET(gtk_builder_get_object(builder, "main-window"));
    image = GTK_WIDGET(gtk_builder_get_object(builder, "main-picture"));
    buttonCancel = GTK_WIDGET(gtk_builder_get_object(builder, "button-cancel"));


    // Connects signal handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);


    filename = "../pictures/01-image_easy.jpeg";
    gtk_widget_set_sensitive(GTK_WIDGET(buttonCancel), FALSE);


    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}



// Function with button_load-image
void click_load_image(GtkButton* b)
{
    gtk_widget_set_sensitive(GTK_WIDGET(buttonCancel), FALSE);
    gint result;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    dialog = gtk_file_chooser_dialog_new("Open the image of a sudoku",
            GTK_WINDOW(window), action, ("Cancel"), GTK_RESPONSE_CANCEL,
            ("Open"), GTK_RESPONSE_ACCEPT, NULL);
    result = gtk_dialog_run(GTK_DIALOG (dialog));
    if (result == GTK_RESPONSE_ACCEPT) {
        GtkFileChooser* chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);
    }

    gtk_widget_destroy(dialog);
    if (result == GTK_RESPONSE_ACCEPT)
        gtk_image_set_from_file(GTK_IMAGE(image), filename);
    filename1 = "";
}

// Function with button-grayscale
void click_grayscale(GtkButton* b)
{
    gtk_widget_set_sensitive(GTK_WIDGET(buttonCancel), TRUE);
    if (strlen(filename1) == 0){
        sprintf(cmd, "../picture-processing/pre-processing/grayscale-image/"
            "grayscale %s", filename);
    } else {
        sprintf(cmd, "../picture-processing/pre-processing/grayscale-image/"
            "grayscale %s", filename1);
    }
    system(cmd);
    filename1 = "gray-image.jpeg";
    gtk_image_set_from_file(GTK_IMAGE(image), filename1);
}

// Function with button_rotation
void click_rotation(GtkButton* b)
{
    g_print("Rotation button clicked");
}

// Function with button_start
void click_median(GtkButton* b)
{
    gtk_widget_set_sensitive(GTK_WIDGET(buttonCancel), TRUE);
    if (strlen(filename1) == 0){
        sprintf(cmd, "../picture-processing/pre-processing/median-filter/"
            "medianFilter %s", filename);
    } else {
        sprintf(cmd, "../picture-processing/pre-processing/median-filter/"
            "medianFilter %s", filename1);
    }
    system(cmd);
    filename1 = "median-picture.jpeg";
    gtk_image_set_from_file(GTK_IMAGE(image), filename1);
}

// Function with button_cancel
void click_solver(GtkButton* b)
{
    g_print("Solver button clicked");
}

// Function with button_cancel
void click_start(GtkButton* b)
{
    g_print("Start button clicked");
}

// Function with button_cancel:
void click_cancel(GtkButton* b)
{
    gtk_image_set_from_file(GTK_IMAGE(image), filename);
    gtk_widget_set_sensitive(GTK_WIDGET(buttonCancel), FALSE);
    filename1 = "";
}

// Function with button_save
void click_save(GtkButton* b)
{
    GtkFileChooser *chooser;
    gint result;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;
    dialog = gtk_file_chooser_dialog_new("Save sudoku as...", GTK_WINDOW(window),
            action, ("Cancel"), GTK_RESPONSE_CANCEL, ("Save"),
            GTK_RESPONSE_ACCEPT, NULL);
    chooser = GTK_FILE_CHOOSER(dialog);

    gtk_file_chooser_set_do_overwrite_confirmation(chooser, TRUE);
    gtk_file_chooser_set_current_name(chooser, ("sudoku_result.jpeg"));

    result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_ACCEPT) {
        filenameFinal = gtk_file_chooser_get_filename(chooser);
        if (strlen(filename1) == 0)
            sprintf(cmd, "cp %s %s", filename, filenameFinal);
        else
            sprintf(cmd, "cp %s %s", filename1, filenameFinal);
        system(cmd);
        g_free(filenameFinal);
    }

    gtk_widget_destroy(dialog);
}

// Function with button_quit
void quit_app(GtkButton* b)
{
    gtk_main_quit();
}

