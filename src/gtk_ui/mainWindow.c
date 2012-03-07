/*Author:PXunL
 *email:dd.pangxie@gmail.com
 *main window of this shell
 *file:mainWindow.c
 *time:2012-3-6
*/


#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

void destroy(GtkWidget *widget, gpointer data);


int main(int argc, char *argv[])
{
	GtkWidget *main_window;
	/*GtkWidget *button;*/
	/*GtkWidget *exit_area;*/
	GtkWidget *menu_vbox;
	GtkWidget *menubar;
	GtkWidget *filemenu;
	GtkWidget *new;
	GtkWidget *file;
	GtkWidget *quit;
	GtkWidget *sep;
	GtkWidget *option;
	GtkWidget *edit;
	GtkWidget *help;

	GtkAccelGroup *accel_group = NULL;
	
	gtk_init(&argc, &argv);

	//Create main window
	main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	
	//bing the "destroy event" signal to close the main window
	gtk_signal_connect(GTK_OBJECT(main_window), "destroy", 
			G_CALLBACK(destroy), NULL);
	gtk_window_set_title(GTK_WINDOW(main_window), "Myshell");
	gtk_window_set_default_size(GTK_WINDOW(main_window), 730, 533);

	//vbox for menubar
	menu_vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(main_window), menu_vbox);

	menubar = gtk_menu_bar_new();
	filemenu = gtk_menu_new();

	//create accelrators
	accel_group = gtk_accel_group_new();
	gtk_window_add_accel_group(GTK_WINDOW(main_window), accel_group);

	//add menu into menubar
	file = gtk_menu_item_new_with_mnemonic("_File");
	edit = gtk_menu_item_new_with_mnemonic("_Edit");
	option = gtk_menu_item_new_with_mnemonic("_Option");
	help = gtk_menu_item_new_with_mnemonic("_Help");
	new = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW, NULL);
	/*open = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, NULL);*/
	quit = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, accel_group);

	//separator the menu
	sep = gtk_separator_menu_item_new();

	//add CTRL+N to create a new window
	/*gtk_widget_add_accelerator(new, "avtivate", accel_group,*/
			/*GDK_n, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);*/

	//add CTRL+Q to close this window
	gtk_widget_add_accelerator(quit, "activate", accel_group,
		GDK_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	//add submenu to filemenu
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);

	//show menu
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), new);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), quit);
	gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), sep);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), edit);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), option);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), help);
	gtk_box_pack_start(GTK_BOX(menu_vbox), menubar, FALSE, FALSE, 3);


	// bing the signal quit to close the window
	g_signal_connect(G_OBJECT(quit), "activate", 
			G_CALLBACK(destroy), NULL);

	//show all windows
	gtk_widget_show_all(main_window);
	gtk_main();
	
	return 0;
}


//close the main window and exit program
void destroy(GtkWidget *widget, gpointer data)
{
	g_print("\nended...\n");
	gtk_main_quit();
}
