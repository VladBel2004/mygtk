#include "gtk/gtk.h"
#include "gdk/gdk.h"
#include "glib.h"
#include "gio/gio.h"
#include "glib-object.h"

#define USR_LABEL_BUF_SIZE 10

static gchar *global_str = NULL;

void usr_click_button( GtkButton *butt, gpointer user_data )
{
        gdouble val = *(gdouble*)user_data;
        gchar *buf = (gchar*)g_malloc( 10 );
        gchar *past_str = global_str;

        buf = g_ascii_dtostr( buf, 10, val );
        /*g_print( "Bug: %s\n", buf );*/
        global_str = g_strjoin( NULL, global_str, buf, NULL );
        g_free( past_str );
}

void usr_get_buf( GtkButton *butt, gpointer user_data )
{
        GtkWidget *entry = GTK_WIDGET(user_data);
        gtk_entry_set_text( GTK_ENTRY(entry), global_str );
}

void usr_clear_buf( GtkButton *butt, gpointer user_data )
{
        GtkWidget *entry = GTK_WIDGET( user_data );
        g_free( global_str );
        global_str = (gchar*)g_malloc( 1 );
        global_str[0]='\0';
        gtk_entry_set_text( GTK_ENTRY(entry), global_str );
}

void usr_active_app( GtkApplication *app, gpointer user_data )
{
        GtkWidget *window = gtk_application_window_new( app );
        GtkWidget *grid = gtk_grid_new();        

        GtkWidget *box = gtk_box_new( GTK_ORIENTATION_VERTICAL, 10 );

        GtkWidget *entry = gtk_entry_new();
        GtkWidget *button = NULL;

        GtkEntryBuffer *buf = gtk_entry_buffer_new( "Some text", -1 );
        GdkGeometry geometry = {
                .min_width = 300,
                .min_height = 300,
                .max_width = 300,
                .max_height = 300,
                .base_width = -1,
                .base_height = -1,
                .width_inc = 1,
                .height_inc = 1,
        };
        
        gchar *lable_buf = (gchar*)g_malloc( USR_LABEL_BUF_SIZE );
        gint count = 9;
        
        gtk_window_set_title( GTK_WINDOW( window ), "MyGTK3" );
        gtk_window_set_default_size( GTK_WINDOW(window), 300, 300 );
        gtk_window_set_geometry_hints( GTK_WINDOW( window ), NULL, &geometry,
                        ( GDK_HINT_MIN_SIZE | GDK_HINT_RESIZE_INC ) );

        button = gtk_button_new_with_label( "Test" );

        gtk_box_pack_end( GTK_BOX(box), button, TRUE, TRUE, 10 );


        gtk_box_pack_end( GTK_BOX(box), grid, TRUE, TRUE, 10 );

        gtk_container_add( GTK_CONTAINER(window), box );

        gtk_widget_set_halign( grid, GTK_ALIGN_CENTER );
        gtk_widget_set_valign( grid, GTK_ALIGN_FILL );
        
        gtk_entry_set_buffer( GTK_ENTRY( entry ), buf );
        g_object_set( G_OBJECT(entry), "editable", FALSE, NULL );

        gtk_grid_attach( GTK_GRID( grid ), entry, 0, 0, 3, 1 );

        for( gint y = 0; y < 3; y++ )
        {
                for( gint x = 3; x > 0; x-- )
                {
                        button = gtk_button_new_with_label( g_ascii_dtostr( lable_buf, USR_LABEL_BUF_SIZE, (gdouble)( count-- ) ) );
                        gtk_grid_attach( GTK_GRID( grid ), button, x-1, y+1, 1, 1 );

                        gdouble *val = (gdouble*)g_malloc( sizeof(gdouble) );
                        *val = count+1;
                        g_signal_connect( button, "clicked", G_CALLBACK(usr_click_button), val );
                }
        }

        button = gtk_button_new_with_label( "Get buffer." );
        gtk_grid_attach( GTK_GRID( grid ), button, 0, 4, 1, 1 );
        g_signal_connect( button, "clicked", G_CALLBACK( usr_get_buf ), (gpointer)entry );

        button = gtk_button_new_with_label( "Clear buf." );
        gtk_grid_attach( GTK_GRID( grid ), button, 1, 4, 1, 1 );
        g_signal_connect( button, "clicked", G_CALLBACK( usr_clear_buf ), (gpointer)entry );

        gtk_widget_show_all( window );
}

int main( int argc, char**argv )
{
        global_str = (gchar*)g_malloc(1);
        global_str[0] = '\0';

        GtkApplication *app = gtk_application_new( "org.gtk.mygtk", G_APPLICATION_FLAGS_NONE );
        g_signal_connect( app, "activate", G_CALLBACK(usr_active_app), NULL );
        g_application_set_default( G_APPLICATION(app) );
        g_application_run( G_APPLICATION(app), argc, argv );
        g_object_unref( app );

        return 0;
}
