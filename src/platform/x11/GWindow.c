#include "GSPCore/GWindow.h"
#include "internal/def/GWindowDef.h"

#include <math.h>

#include "GSPCore/Graphics/GGI.h"

#include "internal/def/Graphics/GFrameDef.h"
#include "internal/def/Graphics/GShaderDef.h"
#include "internal/def/Graphics/GTextureDef.h"
#include "internal/def/Graphics/GVertexBufferDef.h"
#include "GSPCore/GVector.h"

#include "GSPCore/GLog.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>


#include <time.h>


// common statics
static GVector windowVector = NULL;

static Display* xDisplay = NULL;
static Window xRoot = None;
static XEvent xEvent;
static KeySym eventKey;
static char eventText[255];

//static Atom deleteMessage;

XVisualInfo* glxVisualInfo = NULL;
GLXFBConfig bestGlxFbc;

static int visual_attribs[] = {
    GLX_X_RENDERABLE    , True,
    GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
    GLX_RENDER_TYPE     , GLX_RGBA_BIT,
    GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
    GLX_RED_SIZE        , 8,
    GLX_GREEN_SIZE      , 8,
    GLX_BLUE_SIZE       , 8,
    GLX_ALPHA_SIZE      , 8,
    GLX_DEPTH_SIZE      , 24,
    GLX_STENCIL_SIZE    , 8,
    GLX_DOUBLEBUFFER    , True,
    None
};

clock_t start = 0;
clock_t end = 0;
double cpu_time_used;


#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

void TryMakeGlVisualInfo();
void TryMakeGlxWindow(Window* xWindow, GLXContext* context, GWindowInfo info);
bool isExtensionSupported(const char *extList, const char *extension);

bool ctxErrorOccurred = false;
int ctxErrorHandler( Display *dpy, XErrorEvent *ev) {
    ctxErrorOccurred = true;
    return 0;
}

// Vertex shader source code
const static char* vertexShaderSource = "               \
    #version 330 core                                   \
    layout (location = 0) in vec3 aPos;                 \
    layout (location = 1) in vec2 aTexCoord;            \
                                                        \
    out vec2 TexCoord;                                  \
                                                        \
    void main() {                                       \
        gl_Position = vec4(aPos, 1.0);                  \
        TexCoord = vec2(aTexCoord.x, aTexCoord.y)       \
    }                                                   \
";

// Fragment shader source code
const static char* fragmentShaderSource = "             \
    #version 330 core                                   \
    out vec4 FragColor;                                 \
                                                        \
    in vec2 TexCoord;                                   \
                                                        \
    uniform sampler2D textureSampler;                   \
                                                        \
    void main() {                                       \
        FragColor = texture(textureSampler, TexCoord);  \
    }                                                   \
";

// Define the vertices and texture coordinates of the quad
static float vertices[] = {
    // Positions          // Texture Coordinates
    1.0f,  1.0f, 0.0f,   1.0f, 0.0f, // Top Right
    1.0f, -1.0f, 0.0f,   1.0f, 1.0f, // Bottom Right
    -1.0f, -1.0f, 0.0f,   0.0f, 1.0f, // Bottom Left
    -1.0f,  1.0f, 0.0f,   0.0f, 0.0f  // Top Left 
};

static unsigned int indices[] = {
    0, 1, 3, // First Triangle
    1, 2, 3  // Second Triangle
};



// functions defined in this file
GWindowDef* TryGetWindow(Window xWindow);
void SetupShadersForWindow(GWindowDef* windowDef);

bool GWindowDef_Init() {
    return true;
}

GWindow GWindow_Init(GWindowInfo info) {

    if (xDisplay == NULL) {
        // either first time opening a window, or x11 broken.
        xDisplay = XOpenDisplay(NULL);
        xRoot = DefaultRootWindow(xDisplay);
    }

    if (xDisplay == NULL) {
        DEBUG_LOG(FAIL, "Failed to connect to an X11 display server!");
        return NULL;
    }

    if (xRoot == 0) {
        XCloseDisplay(xDisplay);
        DEBUG_LOG(FAIL, "Failed to connect to an X11 root window!");
        return NULL;
    }


    if (glxVisualInfo == NULL) {
        TryMakeGlVisualInfo();
    }

    GLXContext context = NULL;
    Window xWindow = None;

    if (glxVisualInfo != NULL) {
        TryMakeGlxWindow(&xWindow, &context, info);
 
        glXMakeCurrent(xDisplay, xWindow, context);

        if (glewInit() != GLEW_OK) {
            DEBUG_LOG(FAIL, "Failed to initialize GLEW");
            return NULL;
        }


    }

    if (xWindow == None) {
        xWindow = XCreateSimpleWindow(xDisplay, xRoot, 0, 0, info.width, info.height, 0, 0, 0);
    }

    if (xWindow == None) {
        DEBUG_LOG(FAIL, "Failed to create an X11 window");
        return 0;
    }

    XStoreName(xDisplay, xWindow, info.title);

    Atom deleteMessage = XInternAtom(xDisplay, "WM_DELETE_WINDOW", False);

    XSetWMProtocols(xDisplay, xWindow, &deleteMessage, 1);
    XSelectInput(xDisplay, xWindow, ExposureMask | KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);
    XMapWindow(xDisplay, xWindow);



    GWindowDef* window =  calloc(1, sizeof(GWindowDef));

    if (window == NULL) {
        return NULL;
    }

            SetupShadersForWindow(window);

    window->xDeleteAtom = (uintptr_t)deleteMessage;
    window->title = info.title;
    window->rawHandle = (void*)xWindow;
    window->width = info.width;
    window->height = info.height;
    window->glContext = (uintptr_t)context;

    if (windowVector == NULL) {
        windowVector = GVector_Init();
    }

    if (windowVector != NULL) {
        GVector_Add(windowVector, (GVectorItem)window);
        DEBUG_LOG(INFO, "Allocated GWindow at %lu", window);
        return window;
    } else {
        DEBUG_LOG(FAIL, "Failed to allocate a window vector.");
        return NULL;
    }

}

void GWindow_SetUserData(GWindow window, void* userData) {
    if (window == NULL) {
        return NULL;
    }

    ((GWindowDef*)window)->userData = userData;

}

void GWindow_SetDrawDelegate(GWindow window, GWindowDrawDelegate drawDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->drawDelegate = drawDelegate;
    }
}

void GWindow_SetCloseDelegate(GWindow window, GWindowCloseDelegate closeDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->closeDelegate = closeDelegate;
    }
}
void GWindow_SetWillCloseDelegate(GWindow window, GWindowWillCloseDelegate willCloseDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->willCloseDelegate = willCloseDelegate;
    }
}

void GWindow_SetResizeDelegate(GWindow window, GWindowResizeDelegate resizeDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->resizeDelegate = resizeDelegate;
    }
}

void GWindow_SetPointerMoveDelegate(GWindow window, GWindowPointerMoveDelegate pointerMoveDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->pointerMoveDelegate = pointerMoveDelegate;
    }
}

void GWindow_SetButtonDownDelegate(GWindow window, GWindowButtonDownDelegate buttonDownDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->buttonDownDelegate = buttonDownDelegate;
    }
}

void GWindow_SetButtonUpDelegate(GWindow window, GWindowButtonUpDelegate buttonUpDelegate) {
    if (GVector_Contains(windowVector, window)) {
        ((GWindowDef*)window)->buttonUpDelegate = buttonUpDelegate;
    }
}

void GWindow_Close(GWindow window) {
    if (GVector_Contains(windowVector, window)) {
        glXMakeCurrent(xDisplay, 0, 0);
        glXDestroyContext(xDisplay, (GLXContext)((GWindowDef*)window)->glContext);
        XDestroyWindow(xDisplay, (XID)((GWindowDef*)window)->rawHandle);

        GVector_Remove(windowVector, (GVectorItem)window);

        DEBUG_LOG(INFO, "Closed GWindow at %lu", (uintptr_t)window);

    }
}

void GWindow_Redraw(GWindow window) {
    if (GVector_Contains(windowVector, window)) {

        GWindowDef* windowDef = (GWindowDef*)window;
    
        windowDef->redrawFlag = true;
    }
}

void GWindowDef_Poll() {


    if (!XPending(xDisplay)) { 
        return;
    }

    XNextEvent(xDisplay, &xEvent);

    GWindowDef* windowDef = TryGetWindow(xEvent.xany.window);

    if (windowDef == NULL) {
        //DEBUG_LOG(FAIL, "Event recieved for unknown window!");
        return;
    }

    if (xEvent.type == Expose || windowDef->redrawFlag) {
        //printf("EXPOSE\n");

            

            glXMakeCurrent(xDisplay, xEvent.xany.window, (GLXContext)windowDef->glContext);
            /*glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT);*/

            //glViewport(0,0,windowDef->width, windowDef->height);

            GFrameInfo frameInfo = { 
                windowDef->width,
                windowDef->height
            };

            printf("%d %d\n", windowDef->width, windowDef->height);

            GFrame frame = GFrame_Alloc(frameInfo);

            GColor color = { 1.0, 1.0, 0.0, 1.0};
            GRect rect = { 0.0, 0.0, 800.0, 600.0};
            GFrame_Fill(frame, rect, color );


            if (windowDef->drawDelegate != NULL) {
                (windowDef->drawDelegate)(windowDef->userData, (GWindow)windowDef, frame);

                
            }

            
                

            
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0,0,windowDef->width, windowDef->height);

             // Use shader program
            glUseProgram(((GShaderDef*)windowDef->shader)->glShaderProgram);
            glBindVertexArray(((GVertexBufferDef*)windowDef->vertexBuffer)->glVertexArrayBuffer);

            glBindTexture(GL_TEXTURE_2D, ((GFrameDef*)frame)->glBuffer);

            // Draw quad
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            GFrame_Free(frame);

            

            


            glXSwapBuffers(xDisplay, xEvent.xany.window);

            
        windowDef->redrawFlag = false;
    }

    switch(xEvent.type) {
        case ConfigureNotify:
            
            int newWidth = xEvent.xconfigure.width;
            int newHeight = xEvent.xconfigure.height;

            if (windowDef->width != newWidth || windowDef->height != newHeight) {

                GWindowSize newSize = { newWidth, newHeight };

                windowDef->width = newWidth;
                windowDef->height = newHeight;
                
                if (windowDef->resizeDelegate != NULL) {
                    (windowDef->resizeDelegate)(windowDef->userData, windowDef, newSize);
                }
            }
            
            break;
        case MotionNotify:

            end = clock();
            cpu_time_used = ((double) (end - start));

            printf("Mouse event took %f us to arrive.\n", cpu_time_used);

   
            if (cpu_time_used > 1000000.0/144.0) {
                GWindowPoint motionLocation = {xEvent.xmotion.x, xEvent.xmotion.y};
                if (windowDef->pointerMoveDelegate != NULL) {
                    (windowDef->pointerMoveDelegate)(windowDef->userData, windowDef, motionLocation);
                }

                start = clock();

            } 
            

            break;
        case ButtonPress:
        
            GWindowPoint buttonDownLocation = {xEvent.xbutton.x, xEvent.xbutton.y};
            if (windowDef->buttonDownDelegate != NULL) {
                (windowDef->buttonDownDelegate)(windowDef->userData, windowDef, buttonDownLocation, (uint8_t)xEvent.xbutton.button);
            }

            break;
        case ButtonRelease:

            GWindowPoint buttonUpLocation = {xEvent.xbutton.x, xEvent.xbutton.y};
            if (windowDef->buttonUpDelegate != NULL) {
                (windowDef->buttonUpDelegate)(windowDef->userData, windowDef, buttonUpLocation, (uint8_t)xEvent.xbutton.button);
            }

            break;
        case KeyPress:
            // Handle keyboard events
            XLookupString(&xEvent.xkey, eventText, sizeof(eventText), &eventKey, 0);
            printf("Key pressed: %s\n", eventText);
            break;

        case ClientMessage:
            
            if (xEvent.xclient.data.l[0] == (Atom)windowDef->xDeleteAtom) {

                if (windowDef->willCloseDelegate != NULL) {

                    if ((windowDef->willCloseDelegate)(windowDef->userData, (GWindow)windowDef)) {
                        if (windowDef->closeDelegate != NULL) {
                            (windowDef->closeDelegate)(windowDef->userData, (GWindow)windowDef);
                        }

                        GWindow_Close((GWindow)windowDef);
                    }
                } else {
                    
                    if (windowDef->closeDelegate != NULL) {
                        (windowDef->closeDelegate)(windowDef->userData, (GWindow)windowDef);
                    }

                    GWindow_Close((GWindow)windowDef);
                }
            }

            break;
    }
}

size_t GWindowDef_NumberOfOpenWindows() {
    return GVector_Size(windowVector);
}

GWindowDef* TryGetWindow(Window xWindow) {

    for (int i = 0; i < GVector_Size(windowVector); i++) {
        
        GVectorItem item = GVector_Get(windowVector, i);
        if (item == NULL) {
            return NULL;
        }

        GWindowDef* window = (GWindowDef*)item;
        if (window->rawHandle == (GVectorItem)xWindow) {
            return (GWindowDef*)GVector_Get(windowVector, i);
        }
    }
        
    return NULL;
}

bool isExtensionSupported(const char *extList, const char *extension) {
  const char *start;
  const char *where, *terminator;
  
  /* Extension names should not have spaces. */
  where = strchr(extension, ' ');
  if (where || *extension == '\0')
    return false;

  /* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
  for (start=extList;;) {
    where = strstr(start, extension);

    if (!where)
      break;

    terminator = where + strlen(extension);

    if ( where == start || *(where - 1) == ' ' )
      if ( *terminator == ' ' || *terminator == '\0' )
        return true;

    start = terminator;
  }

  return false;
}

void TryMakeGlVisualInfo() {
    int glx_major, glx_minor;

    // FBConfigs were added in GLX version 1.3.
    if (!glXQueryVersion(xDisplay, &glx_major, &glx_minor) || 
    ((glx_major == 1) && (glx_minor < 3)) || (glx_major < 1)) {
        DEBUG_LOG(FAIL, "Invalid GLX Version!");
        return;
    }

    int fbcount;
    GLXFBConfig* fbc = glXChooseFBConfig(xDisplay, DefaultScreen(xDisplay), visual_attribs, &fbcount);
    if (!fbc) {
        DEBUG_LOG(FAIL, "Failed to retrieve a GLX framebuffer config!");
        return;
    }

    int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;

    int i;
    for (i=0; i<fbcount; ++i) {

        XVisualInfo* vi = glXGetVisualFromFBConfig( xDisplay, fbc[i] );

        if (vi) {
            int samp_buf, samples;
            glXGetFBConfigAttrib( xDisplay, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf );
            glXGetFBConfigAttrib( xDisplay, fbc[i], GLX_SAMPLES       , &samples  );

            if (best_fbc < 0 || samp_buf && samples > best_num_samp) {
                best_fbc = i, best_num_samp = samples;
            }
                
            if ( worst_fbc < 0 || !samp_buf || samples < worst_num_samp ) {
                worst_fbc = i, worst_num_samp = samples;
            }
        }

        // free visual info from glXGetVisualFromFBConfig()
        XFree(vi);
    }

    bestGlxFbc = fbc[ best_fbc ];

    // free FBConfig list allocated by glXChooseFBConfig()
    XFree(fbc);

    // Get a visual
    glxVisualInfo = glXGetVisualFromFBConfig(xDisplay, bestGlxFbc);
}

void TryMakeGlxWindow(Window* xWindow, GLXContext* context, GWindowInfo info) {
    XSetWindowAttributes swa;
    Colormap cmap;
    swa.colormap = cmap = XCreateColormap(xDisplay, xRoot, glxVisualInfo->visual, AllocNone);
    swa.background_pixmap = None ;
    swa.border_pixel      = 0;
    swa.event_mask        = StructureNotifyMask;

    *xWindow = XCreateWindow(xDisplay, xRoot, 0, 0, info.width, info.height, 0, 
    glxVisualInfo->depth, InputOutput, glxVisualInfo->visual, CWBorderPixel|CWColormap|CWEventMask, &swa);

    XMapWindow(xDisplay, *xWindow);

    // Get the default screen's GLX extension list
    const char *glxExts = glXQueryExtensionsString(xDisplay, DefaultScreen(xDisplay));

    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB((const GLubyte*)"glXCreateContextAttribsARB");

    
    ctxErrorOccurred = false;
    int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&ctxErrorHandler);


    if (!isExtensionSupported( glxExts, "GLX_ARB_create_context") || !glXCreateContextAttribsARB) {
        DEBUG_LOG(FAIL, "GLX context creation error");
        return;
    } else {
        int context_attribs[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
            GLX_CONTEXT_MINOR_VERSION_ARB, 3,
            None
        };

        *context = glXCreateContextAttribsARB(xDisplay, bestGlxFbc, 0, True, context_attribs);

        XSync( xDisplay, False );
        if ( !ctxErrorOccurred && *context ) {
            DEBUG_LOG(INFO, "Created GL 3.3 context");
        } else {
            return;
        }

    }

    // Sync to ensure any errors generated are processed.
    XSync( xDisplay, False );

    // Restore the original error handler
    XSetErrorHandler( oldHandler );
}

void SetupShadersForWindow(GWindowDef* windowDef) {

    windowDef->vertexBuffer = GVertexBuffer_Alloc(vertices, 20, indices, 6);

    windowDef->shader = GShader_AllocSystemShader(TEXTURE);
    
    DEBUG_LOG(INFO, "Set up OpenGL successfully");

        GTextProvider textProvider = GTextProvider_Alloc("Ubuntu Mono", 20.0);

    windowDef->texture = GTextProvider_Render(textProvider, "hello world from GSPCore!");

    GTextProvider_Free(textProvider);

    //windowDef->texture = GTexture_AllocFromFile("./gsp_logo.png");
}

