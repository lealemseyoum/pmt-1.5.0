/* 
 * The contents of this file are subject to the IBM Common Public
 * License Version 0.5 (the "License"); you may not use this file 
 * except in compliance with the License. You may obtain a copy of 
 * the License at http://oss.software.ibm.com/developerworks/
 * opensource/license-cpl.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Initial Developer of the Original Code is Eastman
 * Kodak Company. Portions created by Kodak are
 * Copyright (C) 2003 Eastman Kodak Company.
 *
 * Original Author:
 * Sam Fryer  samuel.fryer@kodak.com
 *
 * Contributor(s):
 *
 * Date: July, 2003
*/

#ifndef CVIEWIMAGE_H
#define CVIEWIMAGE_H

// System includes
#include <X11/Xlib.h> // Every Xlib program must include this
#include <X11/Xutil.h> // Every Xlib program must include this
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>  // We're using Motif!
#include <Xm/XmAll.h>// We're not going to individually include everything...

// PMT includes
#include "PmtImageRect.h"

class CViewImage { 
public:

    CViewImage();

    ~CViewImage();
    
    void setImage(PmtImageRect *im) { buffer = im; };
    
    // The call to display the image
    int display();
    void start();
    
    //void setText(const char * theMetadata);
    void addMetadataItem( const char * metadataName,
                          const char * metadataValue,
                          const char * tip );
    
    /////////////////////////////////////////////////////////////////
    // Call back methods
    /////////////////////////////////////////////////////////////////
    static void openFileDialog(Widget w, void * client_data, void * call_data);
    static void openFile(Widget w, void * client_data, void * call_data);
    static void exitApp(Widget w, void * client_data, void * call_data);

private:

    // The XDisplay.
    // Note: this is a void* to prevent applications from having to include
    // all of the X11 includes necessary 
    // (same with all the parameter passing below)
    void *theDisplay;
    
    // The actual input image data
    PmtImageRect * buffer;
    XtAppContext app;
    Widget theDrawingArea,theFileSelectionBox,toplevel;
    Widget theNameRC,theValueRC;

    Pixmap thePixmap;

    // The displaying image's width and height (not the input image's)    
    unsigned int windowWidth;
    unsigned int windowHeight;
    
#define DATAVIEW_MAX_WIDTH 400
#define DATAVIEW_MAX_HEIGHT 400

    // X display and imaging routines:

    // Setup the X window and handle events (blocking call)
    int DisplayImage();

    // Actually setup the X window 
    unsigned long WindowSetup(void *incomingVisual);
    
    // Get the visual information
    void * getVisual();
    
    // converts the data to the required format and depth
    char * convertBufferToVisualDepth(void * incomingVisual);
    
    // creates the XImage (may perform scaling).
    void * createImage(void * incomingVisual,
                       unsigned int mag, 
                       long &offset_x, 
                       long &offset_y,
                       void * old_image = NULL);
                       
    int createMainWindow(Widget toplevel);
    void createMainWindowMenu(Widget top);
    
    char * insertNewlines( const char * tip );
    
    
};

#endif


