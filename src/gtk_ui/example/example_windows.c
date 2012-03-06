#include <gtk/gtk.h>

void hello(GtkWidget *widget,gpointer data)
{
	g_print("Hello Ubuntu!\n");
}


gint delete_event(GtkWidget *widget,GdkEvent *event,gpointer data)
{
	g_print ("delete event occurred\n");
	return(TRUE);
}


void destroy(GtkWidget *widget,gpointer data)
{
	gtk_main_quit();
}


int main( int argc, char *argv[] )
{
	GtkWidget *window;
	GtkWidget *button;
	gtk_init (&argc, &argv);
	window=gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_signal_connect (GTK_OBJECT(window),"delete_event",GTK_SIGNAL_FUNC(delete_event),NULL);
	gtk_signal_connect (GTK_OBJECT (window), "destroy",GTK_SIGNAL_FUNC (destroy), NULL);
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);
	button = gtk_button_new_with_label ("Hello Ubuntu!");
	gtk_signal_connect (GTK_OBJECT (button), "clicked",GTK_SIGNAL_FUNC (hello), NULL);
	gtk_signal_connect_object (GTK_OBJECT (button), "clicked",GTK_SIGNAL_FUNC (gtk_widget_destroy),GTK_OBJECT (window));
	gtk_container_add (GTK_CONTAINER (window), button);
	gtk_widget_show (button);
	gtk_widget_show (window);   /*显示一个窗口*/
	gtk_main();   /*进入主循环*/
	return(0);
}
