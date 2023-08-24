/*
 *    Ygl: Run 2d-GL programs with standard X11 routines.
 *    (C) Fred Hucht 1993-96
 *    EMail: fred@thp.Uni-Duisburg.DE
 */

#include "header.h"

#define VERSION "3.1"

static const char *WhatString[2]= {
  "@(#)Ygl V" VERSION " by Fred Hucht (C) 1993-96",
  "@(#)EMail: fred@thp.Uni-Duisburg.DE"
};

static void change_gc(Int32, Ulong, XGCValues *);
static void set_scales(YglWindow *w);

static void change_gc(Int32 id, Ulong mask, XGCValues *values) {
  YglWindow *w = &Ygl.Windows[id];
  if(w->rgb || Ygl.GC) { /* RGB windows have only one GC */
    XChangeGC(D, w->gc, mask, values);
  } else {
    int i;
    for(i = 0; i < CMapSize; i++) {
      XChangeGC(D, w->gclist[i], mask, values);
    }
  }
  XChangeGC(D, w->chargc, mask, values); /* Added 960130 */
}

void singlebuffer(void) {
  const char * MyName = "singlebuffer";
  I(MyName);
  W->dbuf = False;	/* Window is not in dbuf mode */
#ifdef RGBWIN
  W->draw = W->win;
#else
  W->draw = W->main;
#endif
}

#define YglUpdateAction MultibufferUpdateActionUndefined
#define YglUpdateHint   MultibufferUpdateHintFrequent

#ifdef MULTIBUFLIB

static int HasMBuf = True;

void doublebuffer(void) {
  const char * MyName = "doublebuffer";
  I(MyName);
  if(HasMBuf && W->dispbuf == -1) { /* If not inited */
#ifdef RGBWIN
    if(2 != XmbufCreateBuffers(D, W->win, 2, YglUpdateAction, YglUpdateHint, W->mbuf))
#else
    if(2 != XmbufCreateBuffers(D, W->main, 2, YglUpdateAction, YglUpdateHint, W->mbuf))
#endif
    {
      Yprintf(MyName, "unable to create 2 buffers.\n");
      HasMBuf = False;
    } else {
      W->dbuf = True;			/* Window is in dbuf mode */
      W->dispbuf = 0;			/* Displayed buffer */
      W->draw = W->mbuf[1 - W->dispbuf];/* draw to backbuffer */
    }
  }
}

void swapbuffers(void) {
  const char * MyName = "swapbuffers";
  I(MyName);
  if(HasMBuf) { /* MULTI-BUFFER extension in X-Server? */
    
    if(!W->dbuf) {
      Yprintf(MyName, "window %d is not in doublebuffer mode.\n", winget());
      return;
    }
    
    W->draw = W->mbuf[W->dispbuf];
    W->dispbuf = 1 - W->dispbuf;
    XmbufDisplayBuffers(D, 1, W->mbuf + W->dispbuf, 0, 0);
  }
  XFlush(D); /* XmbufDisplayBuffers() seems not to flush */
}

void frontbuffer(Int32 bool) {
  const char * MyName = "frontbuffer";
  I(MyName);
  if(HasMBuf) { /* MULTI-BUFFER extension in X-Server? */
    
    if(!W->dbuf) {
      /* Ignore if not in doublebuffer mode */
      /* Yprintf(MyName, "window %d is not in doublebuffer mode.\n", winget()); */
      return;
    }
    
    if(bool) W->draw = W->mbuf[    W->dispbuf];
    else     W->draw = W->mbuf[1 - W->dispbuf];
  }
  XFlush(D);
}

void backbuffer(Int32 bool) {
  const char * MyName = "backbuffer";
  I(MyName);
  frontbuffer(!bool);
}

#else /* MULTIBUFLIB */

void doublebuffer(void) {
  const char * MyName = "doublebuffer";
  I(MyName);
  Yprintf(MyName, "Ygl is not configured to use doublebuffering.\n");
}

void swapbuffers(void) {
  const char * MyName = "swapbuffers";
  I(MyName);
  XFlush(D);
}

void frontbuffer(Int32 bool) {
  const char * MyName = "frontbuffer";
  I(MyName);
}

void backbuffer(Int32 bool) {
  const char * MyName = "backbuffer";
  I(MyName);
}

#endif /* MULTIBUFLIB */

void gflush(void) {
  const char * MyName = "gflush";
  I(MyName);
  XFlush(D);
}

Display *getXdpy(void) {
  const char * MyName = "getXdpy";
  I(MyName);
  return D;
}

Window getXwid(void) {
  /* Return the main window. Usable for move/resize, map/unmap,
   * event stuff */
  const char * MyName = "getXwid";
  I(MyName);
  return W->main;
}

Window getXdid(void) {
  /* Return the drawable. Usable for drawing. */
  const char * MyName = "getXdid";
  I(MyName);
  return W->draw;
}

GC getXgc(void) {
  const char * MyName = "getXgc";
  I(MyName);
  return W->gc;
}

void wintitle(Char8 *Title) {
  const char * MyName = "wintitle";
  I(MyName);
  XStoreName(D, W->main, Title);
}

void winset(Int32 wid) {
  const char * MyName = "winset";
  I(MyName);
  if(wid > 0 && wid <= Ygl.NextWindow && Ygl.Windows[wid].main != 0) {
    W = &Ygl.Windows[Ygl.ActiveWindow = wid];
  } else {
    Yprintf(MyName, "invalid window id: %d\n", wid);
  }
}

Int32 winget(void) {
  const char * MyName = "winget";
  I(MyName);
  return Ygl.ActiveWindow;
}

Int32 getplanes(void) {
  const char * MyName = "getplanes";
  I(MyName);
  return W->rgb ? Ygl.RV.depth : Ygl.EmulateCmap ? EMULATE_CMAP_DEPTH : Ygl.CV.depth;
}

Int32 getvaluator(Device dev) {
  Window junkwin;
  int rx, ry, cx, cy;
  Uint mask;
  Int32 r = -1;
  const char * MyName = "getvaluator";
  I(MyName);
  XQueryPointer(D, W->main, &junkwin, &junkwin, &rx, &ry, &cx, &cy, &mask);
#ifdef DEBUG
  fprintf(stderr, "getvaluator: root = (%d,%d), child = (%d,%d) mask=0x%x\n", rx, ry, cx, cy, mask);
#endif
  switch(dev) {
  case MOUSEX: r = rx; break;
  case MOUSEY: r = YglScreenHeight - ry - 1; break;
  default:     Yprintf(MyName, "unknown device: %d.\n", dev); break;
  }
  return r;
}

Int32 getbutton(Device dev) {
  Window junkwin;
  int junk;
  Uint mask, bmask;
  Int32 r = -1;
  int i, j, code;
  char keys[32];
  const char * MyName = "getbutton";
  I(MyName);
  switch(dev) {
  case   LEFTMOUSE: bmask = Button1Mask; goto query;
  case MIDDLEMOUSE: bmask = Button2Mask; goto query;
  case  RIGHTMOUSE: bmask = Button3Mask; goto query;
  query:
    XQueryPointer(D, W->main, &junkwin, &junkwin, 
		  &junk, &junk, &junk, &junk, &mask);
#ifdef DEBUG
    fprintf(stderr, "getbutton: mask = 0x%x\n", mask);
#endif
    r = 0 != (mask & bmask);
    break;
  default:
    XQueryKeymap(D, keys);
    for(i = code = 0; i < 32; i++) for(j = 1; j < 256; j <<= 1, code++) {
      if(dev == Ygl.keymap[code] & (KEYMAP_BIT-1)) { /* dev found */
	r = 0 != (keys[i] & j);
#ifdef DEBUG
	fprintf(stderr, "getbutton: key %d %s, device = %d\n",
		code,
		r ? "pressed" : "released",
		Ygl.keymap[code] & (KEYMAP_BIT-1));
#endif
      }
    }
    if(r == -1) {
      Yprintf(MyName, "unknown device: %d.\n", dev);
    }
    break;
  }
  return r;
}

Int32 gversion(Char8 *v) { 
  static const char *version = "Ygl:X11-" VERSION;
  int i = 0, r = 0;
  
  while('\0' != (*(v+i) = *(version+i))) i++;
  
  if(D == NULL) {
    Display *dpy = XOpenDisplay(NULL);
    if(dpy == NULL) r = -1;
    else XCloseDisplay(dpy);
  }
  return r;
}

static void set_scales(YglWindow *w) {
  w->xf = (w->vw - 1) / (w->or - w->ol);
  w->yf = (w->vh - 1) / (w->ot - w->ob);
  w->xo = w->ol - w->vl / w->xf;
  w->yo = w->ob - w->vb / w->yf;
}

void ortho2(Coord left, Coord right, Coord bottom, Coord top) {
  YglWindow *w = W;
  const char * MyName = "ortho2";
  I(MyName);
  if(left == right || bottom == top) {
    Yprintf(MyName, "x-range or y-range is empty.\n");
    return;
  }
  w->ol = left;
  w->or = right;
  w->ob = bottom;
  w->ot = top;
  
  set_scales(w);
}

void viewport(Screencoord left,   Screencoord right,
	      Screencoord bottom, Screencoord top) {
  YglWindow *w = W;
  XRectangle rect;
  const char * MyName = "viewport";
  I(MyName);
  w->vl = left;
  w->vr = right;
  w->vb = bottom;
  w->vt = top;
  w->vw = ABS(right - left) + 1;
  w->vh = ABS(top - bottom) + 1;
  
  set_scales(w);
  
  if(left > 0 || right < w->xm - 1 || bottom > 0 || top < w->ym - 1) {
    /* Clip window */
#ifdef DEBUG
    fprintf(stderr, "viewport: Clipping window.\n");
#endif
    w->clipped = True;
    rect.x = MIN(left, right);
    rect.y = MIN(bottom, w->ym - top);
    rect.width  = w->vw;
    rect.height = w->vh;
    
    if(w->rgb || Ygl.GC) {
      XSetClipRectangles(D, w->gc, 0, 0, &rect, 1, Unsorted);
    } else {
      int i;
      for(i = 0; i < CMapSize; i++) {
	XSetClipRectangles(D, w->gclist[i], 0, 0, &rect, 1, Unsorted);
      }
    }
    XSetClipRectangles(D, w->chargc, 0, 0, &rect, 1, Unsorted);
  } else if(w->clipped) {
    /* Unclip window */
    Ulong      mask;
    XGCValues  values;
    values.clip_mask = None;
    mask = GCClipMask;
    change_gc(Ygl.ActiveWindow, mask, &values);
    w->clipped = False;
#ifdef DEBUG
    fprintf(stderr, "viewport: Unclipping window.\n");
#endif
  }
}

void getviewport(Screencoord*left, Screencoord*right, Screencoord*bottom, Screencoord*top) {
  YglWindow *w = W;
  const char * MyName = "getviewport";
  I(MyName);
  *left   = w->vl;
  *right  = w->vr;
  *bottom = w->vb;
  *top    = w->vt;
}

void reshapeviewport(void) {
  YglWindow *w = W;
  Int32 x, y;
  const char * MyName = "reshapeviewport";
  I(MyName);
  
  getsize(&x, &y);
  w->xm = x;
  w->ym = y;
  
  viewport(0, x - 1, 0, y - 1);
#ifdef RGBWIN
  if(w->rgb) {
    /* Ignore ExposeEvent */
    XSelectInput(D, w->win,  NoEventMask);
    XResizeWindow(D, w->win, x, y);
    XSelectInput(D, w->win,  Ygl.EventMask &  ExposureMask);
  }
#endif
}

void winpop(void) {
  const char * MyName = "winpop";
  I(MyName);
  XRaiseWindow(D, W->main);
}

void winpush(void) {
  const char * MyName = "winpush";
  I(MyName);
  XLowerWindow(D, W->main);
}

void linewidth(Int16 w) {
  Ulong      mask;
  XGCValues  values;
  const char * MyName = "linewidth";
  I(MyName);
  if(w == W->linewidth) return; /* already set */
  W->linewidth = values.line_width = w;
  mask = GCLineWidth;
  change_gc(Ygl.ActiveWindow, mask, &values);
}

Int32 getlwidth(void) {
  const char * MyName = "getlwidth";
  I(MyName);
  return W->linewidth;
}

Int32 getdisplaymode(void) {
  const char * MyName = "getdisplaymode";
  I(MyName);
  if(W->rgb)
    if(W->dbuf) return DMRGBDOUBLE;
    else        return DMRGB;
  else
    if(W->dbuf) return DMDOUBLE;
    else        return DMSINGLE;
}

void setbell(Char8 t) {
  XKeyboardControl xkbc;
  const char * MyName = "setbell";
  I(MyName);
  switch(t) {
  case 0: xkbc.bell_duration = 0;   break;
  case 1: xkbc.bell_duration = 100; break;
  case 2: xkbc.bell_duration = 400; break;
  default:
    Yprintf(MyName, "invalid value: %d.\n", t);
    return; break;
  }
  XChangeKeyboardControl(D, KBBellDuration, &xkbc);
  F;
}

void ringbell(void) {
  const char * MyName = "ringbell";
  I(MyName);
  XBell(D, 0);
  F;
}

Int32 getgdesc(Int32 what) {
  const char * MyName = "getgdesc";
  Int32 r = -1;
  Display *dpy;
  
  if(D == NULL) {
    if ((dpy = XOpenDisplay(NULL)) == NULL) {
      Yprintf(MyName, "can\'t open display \"%s\".\n", XDisplayName(NULL));
      exit(1);
    }
  } else {
    dpy = D;
  }
  
  switch(what) {
  case GD_XPMAX: r = DisplayWidth (dpy, DefaultScreen(dpy)); break;
  case GD_YPMAX: r = DisplayHeight(dpy, DefaultScreen(dpy)); break;
  default:
    Yprintf(MyName, "unsupported or unknown argument: %d.\n", what);
    break;
  }

  if(D == NULL) {
    XCloseDisplay(dpy);
  }

  return r;
}

void foreground(void) {
  /* Do nothing */
}

void logicop(Int32 op) {
  int xop;
  Ulong      mask;
  XGCValues  values;
  const char * MyName = "logicop";
  I(MyName);
  switch(op) {
  case LO_ZERO: xop = GXclear; 		break;
  case LO_AND:  xop = GXand;   		break;
  case LO_ANDR: xop = GXandReverse; 	break;
  case LO_SRC:  xop = GXcopy;		break;
  case LO_ANDI: xop = GXandInverted;	break;
  case LO_DST:  xop = GXnoop;		break;
  case LO_XOR:  xop = GXxor;		break;
  case LO_OR:   xop = GXor;		break;
  case LO_NOR:  xop = GXnor;		break;
  case LO_XNOR: xop = GXequiv;		break;
  case LO_NDST: xop = GXinvert;		break;
  case LO_ORR:  xop = GXorReverse;	break;
  case LO_NSRC: xop = GXcopyInverted;	break;
  case LO_ORI:  xop = GXorInverted;	break;
  case LO_NAND: xop = GXnand;		break;
  case LO_ONE:  xop = GXset;		break;
    
  case LO_MIN:
  case LO_MAX:
  case LO_AVG:
  case LO_DMS:
  case LO_SMD:
  case LO_SUM:
    Yprintf(MyName, "unsupported argument: %d.\n", op);
    break;
  default:
    Yprintf(MyName, "unknown argument: %d.\n", op);
    break;
  }
  values.function = xop;
  mask = GCFunction;
  change_gc(Ygl.ActiveWindow, mask, &values);
}

void getmatrix(Matrix m) {
  /* We only have orthographic views, so... */
  YglWindow *w = W;
  const char * MyName = "getmatrix";
  I(MyName);
  memset(m, 0, sizeof(Matrix));
  m[0][0] =  2.0 / (w->or - w->ol);
  m[1][1] =  2.0 / (w->ot - w->ob);
  m[2][2] = -1.0;
  m[3][0] = -(w->or + w->ol)/(w->or - w->ol);
  m[3][1] = -(w->ot + w->ob)/(w->ot - w->ob);
  m[3][3] =  1.0;
  return;
}

#ifdef COVERSLEEP

#if defined (__linux) /* Any others? */
/* Under Linux, usleep is "void usleep(unsigned long)" */
# define USLEEP_RET_TYPE void
# define USLEEP_RET_VAL
# define USLEEP_ARG_TYPE unsigned long
#else
/* On all (?) other platforms, usleep is "int usleep(unsigned int)" */
# define USLEEP_RET_TYPE int
# define USLEEP_RET_VAL  0
# define USLEEP_ARG_TYPE unsigned int
#endif

USLEEP_RET_TYPE usleep(USLEEP_ARG_TYPE Useconds) {
  struct timeval tmout;
  if(D != NULL) XFlush(D);
  tmout.tv_usec = Useconds % 1000000;
  tmout.tv_sec  = Useconds / 1000000;
  (void) select(0, NULL, NULL, NULL, &tmout);
  return USLEEP_RET_VAL;
}

Uint sleep(Uint seconds) {
  struct timeval tmout;
  if(D != NULL) XFlush(D);
  tmout.tv_usec = 0;
  tmout.tv_sec  = seconds;
  (void) select(0, NULL, NULL, NULL, &tmout);
  return 0;
}
#endif
