/*Author:PXunL
 *email:dd.pangxie@gmail.com
 *main window of this shell
 *file:mainWindow.c
 *time:2012-3-6
*/


#include <gtk/gtk.h>

void destroy(GtkWidget *widget, gpointer data);


int main(int argc, char *argv[])
{
	GtkWidget *main_window;
	/*GtkWidget *button;*/
	/*GtkWidget *exit_area;*/
	
	gtk_init(&argc, &argv);

	main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_signal_connect(GTK_OBJECT(main_window), "destroy", G_CALLBACK(destroy), NULL);
	gtk_window_set_title(GTK_WINDOW(main_window), "Myshell");
	gtk_window_set_default_size(GTK_WINDOW(main_window), 700, 500);
	
	gtk_widget_show_all(main_window);
	gtk_main();
	

	return 0;
}


//close the main window and exit program
void destroy(GtkWidget *widget, gpointer data)
{
	g_print("\nended...");
	gtk_main_quit();
}
