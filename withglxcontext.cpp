/* withglxcontext
 *
 * Create a simple glX context, and execute the argument.
 * Destoy the context when the child exits.
 *
 * compile: c++ -o withglxcontext withglxcontext.cpp -lGL -lX11
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <GL/glx.h>

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

int main (int argc, char ** argv)
{
  printf("%s\n", argv[1]);
  Display *display = XOpenDisplay(0);
  if ( !display )
  {
    printf( "Failed to open X display\n" );
    exit(1);
  }

  GLint att[] = { GLX_RGBA, None };

  XVisualInfo *vi = 0;
  vi = glXChooseVisual(display, 0, att);

  if(!vi)
  {
      printf("Failed to get visual info\n");
      exit(1);
  }

  GLXContext ctx = 0;
  ctx = glXCreateContext( display, vi,  0, True );

  if ( !ctx )
  {
    printf( "Failed to create an OpenGL context\n" );
    exit(1);
  }

  // Verifying that context is a direct context
  if ( ! glXIsDirect ( display, ctx ) )
  {
    printf( "Indirect GLX rendering context obtained\n" );
  }
  else
  {
    printf( "Direct GLX rendering context obtained\n" );
  }

  int child = fork();
  if(child < 0)
  {
      exit(1);
  }
  else if(child == 0)
  {
    // child
    execvp(argv[1], argv+1);
    _exit(0);
  }
  else
  {
    // parent
    wait(0);
    glXMakeCurrent( display, 0, 0 );
    glXDestroyContext( display, ctx );
    printf("Context destroyed\n");
    XCloseDisplay( display );
    exit(0);
  }
}

