#include <assert.h>
#include <stdio.h>
#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>
#include <X11/Xlib.h>

cairo_write_func_t my_write(void *colsure, char *data, unsigned int length){
    return length == fwrite(data, 1, length, stdout) ? CAIRO_STATUS_SUCCESS: CAIRO_STATUS_WRITE_ERROR;
}

int main(int argc, char** argv) {
    Display *disp;
    Window root;
    cairo_surface_t *surface;
    int scr;
    /* The only checkpoint only concerns about the number of parameters, see "Usage" */
    if( argc < 2) {
        fprintf(stderr, 
                "Usage:\n"
                "    <%s> <display> [output file]\n"
                "        display The name of the display that you want to capture.\n"
                "                You can find it with command \"xdpyinfo\"\n"
                "        output  The picture you want to store. Figure will redirect\n"
                "                to stdout if you leave it blank.\n"
                , argv[0]);
    	return 1;
    }
    /* try to connect to display, exit if it's NULL */
    disp = XOpenDisplay( argv[1] );
    if( disp == NULL ){
    	fprintf(stderr, "Given display cannot be found, exiting: %s\n" , argv[1]);
    	return 1;    	
    }
    scr = DefaultScreen(disp);
    root = DefaultRootWindow(disp);
    /* get the root surface on given display */
    surface = cairo_xlib_surface_create(disp, root, DefaultVisual(disp, scr),
                                                    DisplayWidth(disp, scr), 
                                                    DisplayHeight(disp, scr));
    if (argc == 3)
        cairo_surface_write_to_png(surface, argv[2]);
    else
        cairo_surface_write_to_png_stream(surface, my_write, NULL); //(surface, argv[2]);
    /* free the memory*/

    cairo_surface_destroy(surface);
    /* return with success */
    return 0;
}
