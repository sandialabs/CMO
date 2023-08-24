/*
 *    Ygl: Run 2d-GL programs with standard X11 routines.
 *    (C) Fred Hucht 1993-96
 *    EMail: fred@thp.Uni-Duisburg.DE
 */

#include "config.h"

#ifdef YGL_PREFIX	/* If all routines should get the prefix ygl_ to avoid naming conflicts */
# include <X11/Yglprefix.h> /* must be included first (don't ask why...) */
#endif

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>  	/* fprintf */
#include <stdlib.h> 	/* malloc  */
#include <stdarg.h> 	/* defpup(...), warn() */

#ifdef MULTIBUF		/* we have both include file and library */
# include <X11/extensions/multibuf.h>
# define MULTIBUFLIB
#else			/* we don't have the include file, but maybe the library (HP/UX 8.0) */
# define Multibuffer 				XID
# define MultibufferUpdateActionUndefined	0
# define MultibufferUpdateActionBackground	1
# define MultibufferUpdateActionUntouched	2
# define MultibufferUpdateActionCopied		3
# define MultibufferUpdateHintFrequent		0
# define MultibufferUpdateHintIntermittent	1
# define MultibufferUpdateHintStatic		2
#endif

#include <X11/Ygl.h>

#ifdef AUTOFLUSH
# include <signal.h>
#endif

#define Ygl _Ygl_Control_	/* Alias for readability. _Ygl_Control_ is the only visible external of libYgl.a */

#define D 			Ygl.display
#define W 			Ygl.active

#define YglScreen		DefaultScreen(D)
#define YglScreenHeight  	DisplayHeight(D, YglScreen)
#define YglScreenWidth	  	 DisplayWidth(D, YglScreen)

#define KEYMAP_LEN 256		/* For init_keymap() etc. */
#define KEYMAP_BIT 0x1000	/* Is key qdevice()d? */

typedef unsigned long Ulong;
typedef unsigned int  Uint;

typedef struct YglWindow_ {
  Window	top;		/* top-level window of window, used for colormap installation */
  Window  	main;		/* main window */
#ifdef RGBWIN
  Window	win;		/* == main when in cmap mode, sub of main when in RGBmode */
#endif
  Window	draw; 		/* drawable, only differ from win when doublebuffering */
  GC		gc, *gclist, chargc;
  int		font;		/* window's font id */
  int		linewidth; 	/* active linewidth */
  Colorindex	color;		/* active color in CMap mode */
  double  	xf, yf; 	/* scaling factor for macros XR(), YR()*/
  double  	xo, yo; 	/* offset to coords for macros X(), Y()*/
  int     	xm, ym; 	/* width, height of window */
  double  	xp, yp; 	/* graphics cursor position */
  double       	xc, yc; 	/* text cursor position */
  int		vl, vr, vb, vt;	/* viewport left, right, bottom, top */
  int		vw, vh;		/* viewport width, height */
  double	ol, or, ob, ot;	/* ortho2   left, right, bottom, top */
  Window	xpid;		/* real X Parent window ID (excl. WM) */
  Int32		pid;    	/* parent id (swinopen),
				 * 0  if window opened with winopen,
				 * -1 if window converted with winX */
  Multibuffer 	mbuf[2];	/* the two doublebuffers */
  int	 	dispbuf;	/* mbuf[dispbuf] is displayed */
  Pixmap	pm;		/* Pixmap for dithering in RGBmode */
  GC		pmgc;		/* GC for it */
  XImage	*pmi;		/* XImage for dithering */
  unsigned char	red,green,blue;	/* Dithered RGBcolors for gRGBcolor */
  unsigned char vmode;		/* vertex mode of window */
  /*------------------------------ Bitfields: */
  Uint   	rgb:1;    	/* True if window in RGBmode */
  Uint		mapped:1;    	/* True if window is mapped */
  Uint		dbuf:1;		/* True if window in doublebuffer mode */
  Uint		clipped:1;	/* True if clipping is activated (viewport) */
} YglWindow;

typedef struct YglFont_ {
  XFontStruct 	*fs;
  Int32 	id;
} YglFont;

/* Global symbols */

typedef struct YglControl_ {
  Display   	*display;
  YglWindow 	*Windows, *active;
  int		ActiveWindow, NextWindow;
  /*------------------------------ Bitfields: */
#ifdef AUTOFLUSH
  Uint		flush:1;
#endif
  Uint		wm_dw_flag:1;
  Uint		stat:1;		/* CMap visual static? */
  Uint		PCM:1;		/* use private colormap? */
  Uint		BS:1;		/* YGL_BS (backing store ) set */
  Uint		GC:1;		/* Use single GC in CMap windows (slower) */
  Uint		EmulateCmap:1;	/* cmapmode uses TrueColor or DirectColor visual */
  /*------------------------------ End of Bitfields */
  YglFont	*Fonts;
  int		LastFont;
  Ulong		EventMask;
  Atom		wm_dw;
  
  int		FT;		/* autoflush time in msec */
  int		V[2];		/* User requested visuals */
  Window	PWID;		/* Parent window ID */
  int		DSZ;		/* size of RGBmode dither matrix */
  
  Colormap	CCmap, RCmap;
  
  Ulong		*Colors;
  Ulong		*ColorsInv;	/* for lrectread */
  
  int		rs, gs, bs;	/* bits to shift */
  int  		rb, gb, bb;	/* width of mask */
#ifdef DEBUG
  char 		**XEventNames;
#endif
  int		(*noinit)(const char *);
  int		(*warn)(const char *name, const char *format, ...);
  Int32 	(*x2gl_wid)(Window win, int which);
  int		(*XRscale)(double x);
  int		(*YRscale)(double x);
  int		(*Xscale )(double x);
  int		(*Yscale )(double x);
  void		(*await_windowevent)(Window win, long mask, int type);
  /*------------------------------ End of initialized values (see ygl.c) */
  long  	keymap[KEYMAP_LEN]; /* For getbutton() and queueing of keys */
  XEvent	lastreadevent;	/* not inited, For dopup() */
  XVisualInfo	CV, RV;		/* not inited, must be last */
} YglControl;

extern YglControl Ygl;

#define EMULATE_CMAP_DEPTH 8	/* no comment */

#define CMapSize   Ygl.CV.colormap_size

#define RMask      RV.visual->red_mask
#define GMask      RV.visual->green_mask
#define BMask      RV.visual->blue_mask

#define EnterLeaveMask 	(EnterWindowMask|LeaveWindowMask)
#define RedrawMask 	(ExposureMask|StructureNotifyMask)
#define DoRGB 	   	(DoRed|DoGreen|DoBlue)

#define VertexNone	0 /* Vertex modes */
#define VertexPoint	1
#define VertexLine	2
#define VertexCLine	3
#define VertexPoly	4

#define YglVisual()	((W->rgb) ? Ygl.RV.visual : Ygl.CV.visual)
#define YglDepth()	((W->rgb) ? Ygl.RV.depth  : Ygl.CV.depth )
#define YglColormap()	((W->rgb) ? Ygl.RCmap     : Ygl.CCmap    )

#define YGL_COLORS(x) 	 (Ygl.PCM ? CMapSize-1-(x) : Ygl.Colors[x])

#define Yprintf		Ygl.warn

#if DEBUG > 1
# define I(name) if(fprintf(stderr, "Entering %s\n", name) && D == NULL) Ygl.noinit(name)
#else
# define I(name) if(D == NULL) Ygl.noinit(name)
#endif

#ifdef AUTOFLUSH
# ifdef DEBUG
#  define F if(Ygl.flush) { Ygl.flush = False; fprintf(stderr, "Autoflushing.\n"); XFlush(D); }
# else
#  define F if(Ygl.flush) { Ygl.flush = False; XFlush(D); }
# endif
#else /*AUTOFLUSH*/
# define F
#endif /*AUTOFLUSH*/

#define XR(x) 	Ygl.XRscale(x)		/* rescaled x, rounded to next int */
#define YR(y) 	Ygl.YRscale(y)		/* rescaled y, dito */
#define X(x)    Ygl.Xscale(x)		/* rescaled and shifted x */
#define Y(y) 	Ygl.Yscale(y)		/* rescaled and shifted and mirrored y */

#define XS(x) 	X(W->xp = (x))		/* return X(x) and set cursor position */
#define YS(y) 	Y(W->yp = (y))		/* return Y(y) and set cursor position */

#define SCP(gx,gy) {W->xp gx;W->yp gy;}	/* Set cursor positions. Ex.: SCP(=x,=y), SCP(+=x,+=y); */

#define MIN(x,y) ((x)<(y) ?   (x) :(y))
#define MAX(x,y) ((x)>(y) ?   (x) :(y))
#define ABS(x)   ((x)< 0  ? (-(x)):(x))

/* #if defined(DEBUG) && !defined(malloc) */
#ifdef DEBUG
# undef malloc
# undef calloc
# undef realloc
static char *debug_malloc_p;
# define malloc(x)      (debug_malloc_p = malloc(x   ), fprintf(stderr, "malloc: x=%d r=0x%x\n",            x, debug_malloc_p), debug_malloc_p)
# define calloc(x, y)   (debug_malloc_p = calloc(x, y), fprintf(stderr, "calloc: x=%d y=%d r=0x%x\n",    x, y, debug_malloc_p), debug_malloc_p)
# define realloc(p, x) (debug_malloc_p = realloc(p, x), fprintf(stderr, "realloc: p=0x%x x=%d r=0x%x\n", p, x, debug_malloc_p), debug_malloc_p)
# define free(p) (fprintf(stderr, "free: p=0x%x\n", p), free(p))
#endif

#define X2GL_TOP	0
#define X2GL_MAIN	1
#define X2GL_WIN	2
#define X2GL_DRAW	3
