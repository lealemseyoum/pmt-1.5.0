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

//Load the PMT Configuration File to make sure the image accessor was enabled.
#include "PmtConfig.h"

#ifdef PMT_IMAGE_ACCESSOR_REQUIRED
#ifdef PMT_EXAMPLES_USE_X

// Pmt Includes
#include "PmtAccessor.h"
#include "PmtMetadataT.h"

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 400

// This class definition
#include "CViewImage.h"

// The PMT code to read a file and get the data out.
extern int readFileAndView(char *filename, CViewImage * viewer);

//////////////////////////////////////////////////////////////////////////////
// Static Callbacks
//////////////////////////////////////////////////////////////////////////////
static void showError(Widget w, const char *errorMessage)
{
    Arg al[10];
    int ac = 0;
    XmString errorString = XmCvtCTToXmString((char*)errorMessage);
    ac=0;
    XtSetArg(al[ac],XmNmessageString,errorString); ac++;
    Widget ed = XmCreateErrorDialog(w,"ERROR",al, ac);
    Widget button = XtNameToWidget(ed,"Help");
    XtUnmanageChild(button);
    button = XtNameToWidget(ed,"Cancel");
    XtUnmanageChild(button);
    XtManageChild(ed);
}    

void CViewImage::openFileDialog(Widget w, 
                                   void * client_data, 
                                   void * call_data)
{
    CViewImage * myThis = (CViewImage*)client_data;
    XtManageChild(myThis->theFileSelectionBox);
    return;
}

void CViewImage::openFile(Widget w, 
                             void * client_data, 
                             void * call_data)
{
    bool shouldUnmanageBox = true;
    CViewImage * myThis = (CViewImage*)client_data;
    XmFileSelectionBoxCallbackStruct * data = 
                               (XmFileSelectionBoxCallbackStruct *)call_data;
    if (data->reason == XmCR_OK)
    {
        char * filename = XmCvtXmStringToCT(data->value);
        int error = readFileAndView(filename,myThis);
        if (error) 
        {
            char * errorMessage="Could not open file.\nPlease try again!";
            showError(w,errorMessage);
            shouldUnmanageBox = false;
        }
        XtFree(filename);
    }
    
    if (shouldUnmanageBox)
        XtUnmanageChild(myThis->theFileSelectionBox);

    return;
}

void CViewImage::exitApp(Widget w, 
                            void * client_data, 
                            void * call_data)
{
    exit(0);
}

//////////////////////////////////////////////////////////////////////////////
// Window Creation Routines
//////////////////////////////////////////////////////////////////////////////
void CViewImage::createMainWindowMenu(Widget top)
{
    Arg al[10];
    int ac = 0;
    
    Widget menu=XmCreateMenuBar(top,"menu",NULL,0);
    XtManageChild(menu);
    
    // Create the file menu
    ac=0;
    XtSetArg(al[ac],XmNmarginWidth,5); ac++;
    XtSetArg(al[ac],XmNmarginHeight,5); ac++;
    XtSetArg(al[ac],XmNspacing,5); ac++;
    Widget fileMenu = XmCreatePulldownMenu(menu,"file_menu",al, ac);
    
    ac=0;
    XtSetArg(al[ac],XmNmnemonic,'O'); ac++;
    XtSetArg(al[ac],XmNborderWidth,0); ac++;
    Widget open = XmCreatePushButton(fileMenu,"Open",NULL,0);
    XtManageChild(open);
    Widget sep = XmCreateSeparator(fileMenu,"sep",NULL,0);
    XtManageChild(sep);
    ac=0;
    XtSetArg(al[ac],XmNmnemonic,'x'); ac++;
    XtSetArg(al[ac],XmNborderWidth,0); ac++;
    Widget exit = XmCreatePushButton(fileMenu,"Exit",NULL,0);
    XtManageChild(exit);
    
    ac=0;
    XtSetArg(al[ac],XmNsubMenuId,fileMenu); ac++;
    XtSetArg(al[ac],XmNmnemonic,'F'); ac++;
    Widget rFileMenu = XmCreateCascadeButton(menu,"File",al,ac);
    XtManageChild(rFileMenu);
    
    XtAddCallback(open,XmNactivateCallback,openFileDialog,this);
    XtAddCallback(exit,XmNactivateCallback,exitApp,this);
    
    //create the about menu option
//     ac=0;
//     XtSetArg(al[ac],XmNmnemonic,'A'); ac++;
//     XtSetArg(al[ac],XmNmarginWidth,0); ac++;
//     XtSetArg(al[ac],XmNmarginHeight,0); ac++;
//     XtSetArg(al[ac],XmNborderWidth,0); ac++;
//     Widget rAboutMenu = XmCreatePushButton(menu,"About",al,ac);
//     XtManageChild(rAboutMenu);
   
}

int CViewImage::createMainWindow(Widget toplevel)
{


    Arg al[20];
    int ac = 0;

    // resize the main window 
    XtSetArg(al[ac],XmNwidth,DEFAULT_WINDOW_WIDTH+60); ac++;
    XtSetArg(al[ac],XmNheight,DEFAULT_WINDOW_HEIGHT+60); ac++;
    XtSetValues(toplevel,al,ac);

    Widget mainWindow = XtVaCreateManagedWidget("mainWindow",
                                         xmMainWindowWidgetClass,toplevel,
                                         XmNrecomputeSize,true,
                                         NULL);
                                
    createMainWindowMenu(mainWindow);
    

    ac=0;
    XtSetArg(al[ac],XmNnumColumns,2); ac++;
    XtSetArg(al[ac],XmNpacking,XmPACK_COLUMN); ac++;
    XtSetArg(al[ac],XmNwidth,DEFAULT_WINDOW_WIDTH+30); ac++;
    XtSetArg(al[ac],XmNheight,DEFAULT_WINDOW_HEIGHT+20); ac++;
    XtSetArg(al[ac],XmNentryAlignment,XmALIGNMENT_CENTER); ac++;
    XtSetArg(al[ac],XmNentryVerticalAlignment,XmALIGNMENT_CENTER); ac++;
    XtSetArg(al[ac],XmNrecomputeSize,true); ac++;
    Widget rc2 = XtCreateManagedWidget("rc2",xmRowColumnWidgetClass,
                                       mainWindow,al,ac);
    ac=0;
    XtSetArg(al[ac],XmNwidth,DEFAULT_WINDOW_WIDTH/2+10); ac++;
    XtSetArg(al[ac],XmNheight,DEFAULT_WINDOW_HEIGHT+10); ac++;
    XtSetArg(al[ac],XmNlabelType,XmPIXMAP); ac++;
    XtSetArg(al[ac],XmNbackground,0x00000000); ac++;
    XtSetArg(al[ac],XmNalignment,XmALIGNMENT_CENTER); ac++;
    theDrawingArea = XtCreateManagedWidget("DA",xmLabelWidgetClass,
                                       rc2,al,ac);
    ac=0;
    XtSetArg(al[ac],XmNscrollingPolicy,XmAUTOMATIC); ac++;
    XtSetArg(al[ac],XmNwidth,DEFAULT_WINDOW_WIDTH/2); ac++;
    XtSetArg(al[ac],XmNheight,DEFAULT_WINDOW_HEIGHT+5); ac++;
    XtSetArg(al[ac],XmNscrolledWindowMarginWidth,0); ac++;
    XtSetArg(al[ac],XmNscrolledWindowMarginHeight,0); ac++;
    XtSetArg(al[ac],XmNspacing,0); ac++;
    XtSetArg(al[ac],XmNmarginWidth,0); ac++;
    XtSetArg(al[ac],XmNmarginHeight,0); ac++;
    XtSetArg(al[ac],XmNrecomputeSize,true); ac++;
    Widget scrolledWindow = XmCreateScrolledWindow(rc2,"scrolledWindow",al,ac);
    XtManageChild(scrolledWindow);
    
    ac=0;
    XtSetArg(al[ac],XmNmarginWidth,0); ac++;
    XtSetArg(al[ac],XmNmarginHeight,0); ac++;
    XtSetArg(al[ac],XmNspacing,0); ac++;
    XtSetArg(al[ac],XmNorientation,XmHORIZONTAL); ac++;
    Widget rc3 = XtCreateManagedWidget("rc3",xmRowColumnWidgetClass,
                                       scrolledWindow,al,ac);

    ac=0;
    XtSetArg(al[ac],XmNmarginWidth,0); ac++;
    XtSetArg(al[ac],XmNmarginHeight,0); ac++;
    theNameRC = XtCreateManagedWidget("rc4",xmRowColumnWidgetClass,
                                       rc3,al,ac);
    ac=0;
    XtSetArg(al[ac],XmNmarginWidth,0); ac++;
    XtSetArg(al[ac],XmNmarginHeight,0); ac++;
    theValueRC = XtCreateManagedWidget("rc4",xmRowColumnWidgetClass,
                                       rc3,al,ac);

    ac=0;
    XtSetArg(al[ac],XmNwidth,DEFAULT_WINDOW_WIDTH/2); ac++;
    Widget shell = XmCreateDialogShell(mainWindow,
                                       "Select an Image File",al,ac);
    theFileSelectionBox = XmCreateFileSelectionBox(shell,
                                                   "FileSelectionBox",
                                                   al,ac);
    Widget button = XtNameToWidget(theFileSelectionBox,"Help");
    XtUnmanageChild(button);
    XtAddCallback(theFileSelectionBox,XmNokCallback,openFile,this);
    XtAddCallback(theFileSelectionBox,XmNcancelCallback,openFile,this);

                                                            
    return 0;
}

void CViewImage::start()
{
    XtRealizeWidget(toplevel);
    XFlush(XtDisplay(toplevel));
    XtAppMainLoop(app);
}

void CViewImage::addMetadataItem( const char * metadataName, 
                                  const char * metadataValue,
                                  const char * tip )
{
    Arg al[15];
    int ac=0;
    char * newtip = insertNewlines(tip);
    XmString mdtip = XmStringCreateLtoR(newtip, XmSTRING_DEFAULT_CHARSET);
    XtSetArg(al[ac],XmNvalue,metadataName); ac++;
#if defined ( XmVERSION ) && ( XmVERSION > 1 )
#if defined ( XmREVISION ) && ( XmREVISION > 1 )
    XtSetArg(al[ac],XmNtoolTipString,mdtip); ac++;
#endif
#endif
    XtSetArg(al[ac],XmNeditable,false); ac++;
    XtSetArg(al[ac],XmNmarginWidth,0); ac++;
    XtSetArg(al[ac],XmNmarginHeight,0); ac++;
    XtSetArg(al[ac],XmNborderWidth,0); ac++;
    XtSetArg(al[ac],XmNcolumns,45); ac++;
    XtSetArg(al[ac],XmNspacing,0); ac++;
    XtSetArg(al[ac],XmNshadowThickness,0); ac++;
    XtCreateManagedWidget("metadataName", xmTextWidgetClass,
                                          theNameRC,al,ac);
    ac=0;
    XmString mdv = XmStringCreateSimple((char *)metadataValue);
    XtSetArg(al[ac],XmNlabelString,mdv); ac++;
    XtCreateManagedWidget("metadataValue", xmLabelGadgetClass,
                                           theValueRC,al,ac);
}

char * CViewImage::insertNewlines( const char * tip )
{
    SPmtString newtip(tip);
    unsigned int i = 0;
    unsigned int newBoundary = 50;
    while (i < newtip.length())
    {
        if (i > newBoundary)
        {
            if (newtip[i] == ' ')
            {
                newBoundary = i + 50;
                newtip[i] = '\n';
            }
        }
        i++;
    }
    
    char * newchartip = (char*)calloc(newtip.length() + 2,1);
    memcpy(newchartip,newtip.c_str(),newtip.length());
    
    return newchartip;
}

//////////////////////////////////////////////////////////////////////////////

CViewImage::CViewImage()
     : theDisplay(NULL),buffer(NULL)
{
    windowWidth = DATAVIEW_MAX_WIDTH;
    windowHeight = DATAVIEW_MAX_HEIGHT;
    thePixmap = 0;
    int argc = 1;
    char * argv[] = {"DataView"};
    char * defaults[] = 
    {
        "?.toolTipEnable: True",
        "?.toolTipPostDelay: 1000",
        "?.toolTipPostDuration: 10000",
        "?.TipShell.TipLabel.background: yellow",
        NULL
    };
    
    toplevel = XtVaAppInitialize(&app,"DataView",NULL,0,&argc,argv,
                                 defaults,NULL);
    

    XtRealizeWidget(toplevel);
    
    createMainWindow(toplevel);
}

//////////////////////////////////////////////////////////////////////////////

    
//////////////////////////////////////////////////////////////////////////////

CViewImage::~CViewImage()
{
    if (buffer) delete buffer;
    
    // free all the x stuff????
}

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////

int CViewImage::display()
{
    int return_value = 0;
       
    theDisplay = XtDisplay(theDrawingArea);
    
    if (buffer)
    {
        unsigned int width = buffer->width();
        unsigned int height = buffer->height();

        //Determine the scaling factor (done very simply here...)
        float step = 1;
        if (width > DATAVIEW_MAX_WIDTH) 
            step = ceil(((float)width)/DATAVIEW_MAX_WIDTH);
        float jstep = 1;
        if (height > DATAVIEW_MAX_HEIGHT) 
            jstep = ceil(((float)height)/DATAVIEW_MAX_HEIGHT);
        if (jstep > step ) step = jstep;

        if (width < DATAVIEW_MAX_WIDTH)
            windowWidth = width;
        else
            windowWidth = (unsigned int)ceil(width/step);
        if (height < DATAVIEW_MAX_HEIGHT)
            windowHeight = height;
        else
            windowHeight = (unsigned int)ceil(height/step);

        return_value = this->DisplayImage();
    }
               
    return return_value; 
}


//////////////////////////////////////////////////////////////////////////////

// We're assuming that the data in the buffer is pixel interleaved RGB with
// one byte per band.  If this is not the case ...
char * CViewImage::convertBufferToVisualDepth(void * incomingVisual)
{
    Display *display = (Display*)this->theDisplay;
    Visual *visual = (Visual *)incomingVisual;
    unsigned int depth = DefaultDepth(display,DefaultScreen(display));
    unsigned int depth_in_bytes = (depth/8);
    if ((depth_in_bytes % 2)&&(depth_in_bytes!=1)) depth_in_bytes++;
    unsigned long nPixels = buffer->width()*buffer->height();
    unsigned char *new_data = (unsigned char*)calloc(nPixels*depth_in_bytes,1);
    unsigned long i=0;
    unsigned char * image = (unsigned char*)buffer->getImageBuffer();
    unsigned int red_bits=0,red_place=0;
    unsigned int green_bits=0,green_place=0;
    unsigned int blue_bits=0,blue_place=0;
    unsigned int demask = 1;

    // Determine bit size and placement based on the visual masks
    for(i=0;i<depth;i++)
    {
        red_bits += (visual->red_mask & demask)?1:0;
        if (!red_bits) red_place++;
        green_bits += (visual->green_mask & demask)?1:0;
        if (!green_bits) green_place++;
        blue_bits += (visual->blue_mask & demask)?1:0;
        if (!blue_bits) blue_place++;
        demask = demask << 1;
    }
    
    int red_shift=8-red_bits,green_shift=8-green_bits,blue_shift=8-blue_bits;
    if (red_shift < 0) red_shift = 0;
    if (green_shift < 0) green_shift = 0;
    if (blue_shift < 0) blue_shift = 0;

// Helpful Debug Info    
//     cout << buffer->dataType(0).name()<<":"<<buffer->dataType(0).size() << endl;
//     cout << "bands = " << buffer->bands() << endl;
//     cout << "pixelStride = " << buffer->getPixelStrideByte() << endl;
//     cout << "nPixels = " << nPixels << endl;
//     cout << "depth = " << depth <<" : " << depth_in_bytes << " bytes"<< endl;
//     cout << "bits per mask : "<<red_bits<<":"<<green_bits<<":"<<blue_bits<<endl;
//     cout << "place per mask : "<<red_place<<":"<<green_place<<":"<<blue_place<<endl;
//     cout << "bits_per_rgb: " << visual->bits_per_rgb << endl;
//     cout << "map_entries: " << visual->map_entries << endl;
//     printf( "red_mask:   0x%08X\n" ,visual->red_mask);
//     printf( "green_mask: 0x%08X\n" ,visual->green_mask);
//     printf( "blue_mask:  0x%08X\n" ,visual->blue_mask);

#define IMAGE_TRANSFER_FOR_LOOP(TYPE) \
{\
TYPE tmp_data = (TYPE)new_data; \
for (i = 0; i<nPixels; i++) \
{ \
    unsigned char *pixel = image;image = image+3; \
    *tmp_data = (((*pixel)>>red_shift)<<red_place) |  \
                (((*(pixel+1))>>green_shift)<<green_place) | \
                (((*(pixel+2))>>blue_shift)<<blue_place); \
    tmp_data++; \
}\
}
    switch (depth_in_bytes)
    {
        case 4:
            IMAGE_TRANSFER_FOR_LOOP(unsigned int *);
            break;
            
        default:
        case 2:
            IMAGE_TRANSFER_FOR_LOOP(unsigned short *);
            break;
        case 1:
            IMAGE_TRANSFER_FOR_LOOP(unsigned char *);
            break;
    }

#undef IMAGE_TRANSFER_FOR_LOOP
        
    return (char*)new_data;
}
    
//////////////////////////////////////////////////////////////////////////////

unsigned long CViewImage::WindowSetup(void * incomingVisual)
{
    Display *display = (Display*)this->theDisplay;
    Visual *visual = (Visual *)incomingVisual;
    unsigned int depth = DefaultDepth(display,DefaultScreen(display));
    
    // Get the background and border color (BLACK)
    int blackColor = BlackPixel(display,DefaultScreen(display));

    // Get the parent window
    Window parent = DefaultRootWindow(display);

    // Create a colormap we can use based on the visual.  Note that this
    // sets the colormap for the parent window ... and thus (potentially)
    // makes everything else on the screen look bad -- usually only on 
    // 8-bit screens.
    Colormap colormap = XCreateColormap(display, parent, visual, AllocNone);
    
    // Set the window attributes
    XSetWindowAttributes attrs;
    attrs.background_pixel = blackColor;
    attrs.border_pixel=blackColor;
    attrs.colormap = colormap;
        
    // create the window
    Window w = XCreateWindow(display, 
                    parent,  
                    0, 0, windowWidth, windowHeight, 
                    0, depth, InputOutput,visual,
                    CWBackPixel | CWBorderPixel | CWColormap,
                    &attrs);

    // We want to get MapNotify events and Mouse ButtonRelease events
    // (although this enables just about all events -- could be fixed 
    // for only the events we really want...)
    XSelectInput(display, w, 0x01FFFFFF);

    // "Map" the window (that is, make it appear on the screen)
    XMapWindow(display, w);

    // Wait for the MapNotify event
    for(;;) 
    {
      XEvent e;
      XNextEvent(display, &e);
      if (e.type == MapNotify)
        break;
    }    
    
    return w;
}

//////////////////////////////////////////////////////////////////////////////
void * CViewImage::createImage(void * incomingVisual,
                                    unsigned int mag, 
                                    long &offset_x, 
                                    long &offset_y,
                                    void * old_image)
{
    Display *display = (Display*)this->theDisplay;
    Visual *visual = (Visual *)incomingVisual;
    unsigned int depth = DefaultDepth(display,DefaultScreen(display));
    XImage *image = (XImage *)old_image;
    int depth_in_bytes = (depth/8);
    if (depth_in_bytes % 2) depth_in_bytes++;

    // Convert the buffer to something we can use
    char *image_data = convertBufferToVisualDepth(visual);
    
    if (image_data)
    {
        // take out the mag>1 check 'cause we need to process the data anyways
        unsigned long nPixels = windowWidth*windowHeight;
        char *new_data = (char*)malloc(nPixels*depth_in_bytes);

        if (new_data)
        {
            // set our boundaries in image coordinates
            unsigned long w = buffer->width()/mag;
            unsigned long h = buffer->height()/mag;
            unsigned long x = offset_x - w/2;
            unsigned long y = offset_y - h/2;

            // make sure our boundaries are within image limits
            if (x < 0 ) x = 0;
            if (x > buffer->width() - w) x = buffer->width() - w;
            if (y < 0 ) y = 0;
            if (y > buffer->height() - h) y = buffer->height() - h;


            // Determine our scaling for the window
            // Note: we only have to consider one direction here,
            // as we made sure that the other was the same earlier.
            double scale = (double)w / (double)windowWidth;
            
            unsigned int i,j;
            int k;
            char * row, * col;
            char * tmp_new_data = new_data; 
            char * tmp_image_data = image_data + 
                                    y*buffer->width()*depth_in_bytes;

            for (j=0;j<windowHeight;j++)
            {
                row = tmp_image_data + 
                      (unsigned long)(j*scale)*buffer->width()*depth_in_bytes;
                for (i=0; i < windowWidth ; i++)
                {
                    col = row + (unsigned long)(i*scale + x) * depth_in_bytes;
                    for (k=0; k < depth_in_bytes; k++)
                    {
                        *tmp_new_data = *col;
                        ++tmp_new_data;
                        ++col;
                    }
                }
            }


            // reset the image offsets
            offset_x = x + w/2;
            offset_y = y + h/2;


            free(image_data);
            image_data = new_data;
        } 

        if (image != NULL)
        {
            XDestroyImage(image);
        }
        
        image = XCreateImage(display,visual,depth,
                            ZPixmap, 0, 
                            image_data,
                            windowWidth, windowHeight,
                            depth_in_bytes*8,0);
    }

    return image;
}

//////////////////////////////////////////////////////////////////////////////

void *CViewImage::getVisual()
{
    int nItemsInList=0;
    Display *display = (Display*)this->theDisplay;
    Visual *ret_val = DefaultVisual(display,DefaultScreen(display));
    
    if (ret_val->red_mask == 0)
    {
        XVisualInfo vinfo_template;
        vinfo_template.c_class = TrueColor;
        XVisualInfo *vinfo = XGetVisualInfo(display, 
                                    VisualClassMask,
                                    &vinfo_template, 
                                    &nItemsInList);
        if (vinfo)
        {
            ret_val = vinfo->visual;
// Helpful Debug Info    
//         cout << "=======================================================\n";
//         cout << "depth: " << vinfo->depth << endl;
//         cout << "bits_per_rgb: " << vinfo->visual->bits_per_rgb << endl;
//         cout << "map_entries: " << vinfo->visual->map_entries << endl;
//         printf( "red_mask:   0x%08X\n" ,vinfo->visual->red_mask);
//         printf( "green_mask: 0x%08X\n" ,vinfo->visual->green_mask);
//         printf( "blue_mask:  0x%08X\n" ,vinfo->visual->blue_mask);
//         cout << "=======================================================\n";
        }
    }
    return ret_val;
}
    
//////////////////////////////////////////////////////////////////////////////

int CViewImage::DisplayImage()
{
    int ret_val = 0;
    long offset_x = windowWidth/2;
    long offset_y = windowHeight/2;
    bool weCreatedDisplay = false;
    
    // First, let's make sure we have a valid buffer...
    if ((buffer->width()<1)||(buffer->height()<1))
    {
        return -1;
    }
    
    // Open the display
    if (this->theDisplay == NULL)
    {
        this->theDisplay = XOpenDisplay(NULL);
        weCreatedDisplay = true;
    }
    
    // If we couldn't open the display stop here.
    if (this->theDisplay == NULL)
    {
        return -1;
    }
    
    // do our typecast here once and for all...
    Display *display = (Display*)this->theDisplay;
      
    // Get the visual
    Visual *visual = (Visual*)this->getVisual();
    if (!visual) 
    {
        if (weCreatedDisplay)
            XCloseDisplay(display);
        return -1;
    }    
    
    // Create the image structure
    XImage *image  = (XImage *)this->createImage(visual,1,offset_x,offset_y);

    if (image)
    {                        

        unsigned int depth = DefaultDepth(display,DefaultScreen(display));

        Arg al[10];
        int ac = 0;
        Window w = XtWindow(theDrawingArea);
        GC gc = XtGetGC(theDrawingArea, 0, NULL);

        if (thePixmap) XFreePixmap(display,thePixmap);
        thePixmap = XCreatePixmap(display, w,windowWidth, windowHeight,depth);
        // Create the request to put the image in the window
        XPutImage(display, thePixmap, gc, image, 
                    0,0,0,0,windowWidth, windowHeight);

        
        ac = 0;
        XtSetArg(al[ac],XmNlabelPixmap,thePixmap); ac++;
        XtSetValues(theDrawingArea,al,ac);


        // Send the request to the server
        XFlush(display);

// XEvent event;
// event.type = ResizeRequest;
// event.xresizerequest.display = display;
// event.xresizerequest.window = w;
// event.xresizerequest.width = windowWidth;
// event.xresizerequest.height = windowHeight+1;
// if (!XtDispatchEvent(&event)) cout << "Could not dispatch event!\n";

        ret_val = 0;
    } 
    else
        ret_val = 1;

    return ret_val;
}

#endif
#endif
