/*
 *    Ygl: Run 2d-GL programs with standard X11 routines.
 *    (C) Fred Hucht 1993-96
 *    EMail: fred@thp.Uni-Duisburg.DE
 */

#include "header.h"

static int   rect_read_errh(Display *, XErrorEvent *);
static Int32 rect_read(const char*, Screencoord, Screencoord, Screencoord, Screencoord, int, void *);
static void rect_write(const char*, Screencoord, Screencoord, Screencoord, Screencoord, int, void *);
static Ulong emuColorsInv(Ulong x);
static void create_dither(int r, int g, int b);
static void set_color(const char *caller, int r, int g, int b);

#define YGL_COLORSINV(x) (Ygl.PCM ? CMapSize-1-(x) : Ygl.EmulateCmap ? emuColorsInv(x) : Ygl.ColorsInv[x])

static Ulong emuColorsInv(Ulong x) {
  Ulong i;
  for(i = 0; i < CMapSize && Ygl.Colors[i] != x; i++);
  return i < CMapSize ? i : 0;
}

void mapcolor(Colorindex ind, Int16 r, Int16 g, Int16 b) {
  XColor xc;
  const char * MyName = "mapcolor";
  I(MyName);
  if(W->rgb) { Yprintf(MyName, "not in CMap mode.\n"); return; }
  if(ind >= CMapSize) {
    Yprintf(MyName, "can't map cell %d (must be between 0 and %d).\n", ind, CMapSize-1);
    return;
  }
  xc.red   = r << 8;
  xc.green = g << 8;
  xc.blue  = b << 8;
  if(!Ygl.PCM) { /* use default colormap */
    if (Ygl.Colors[ind]) XFreeColors(D, Ygl.CCmap, Ygl.Colors + ind, 1, 0);

    if(XAllocColor(D, Ygl.CCmap, &xc)) {
      Ygl.Colors[ind] = xc.pixel;
      if(!Ygl.EmulateCmap)
	Ygl.ColorsInv[xc.pixel] = ind;
    } else {
      Yprintf(MyName, "can't allocate color ( %d, %d, %d) for cell %d.\n", r, g, b, ind);
    }
  }
  else { /* use private colormap */
    xc.pixel = CMapSize - 1 - ind;
    xc.flags = DoRGB;
    XStoreColor(D, Ygl.CCmap, &xc);
  }
#ifdef DEBUG
  fprintf(stderr, "mapcolor: index = %d, xc = %d (%d,%d,%d)\n", ind, xc.pixel, xc.red, xc.green, xc.blue);
#endif
  if(!Ygl.GC) { /* if GC list in CMap windows */
    int i;
    YglWindow *w = &Ygl.Windows[1]; /* first window */
    for(i = 1; i < Ygl.NextWindow; i++, w++) {
      if(w->main != 0 && !w->rgb) {
	XSetForeground(D, w->gclist[ind], ind);
      }
    }
  }
  F;
}

static const char *DI[] = { 	/* index vector for dither matrices */
  "\000", 			/* DI[sz][c] >> 3 : x-coordinate */
  "\000", 			/* DI[sz][c] &  7 : y-coordinate */
  				/* 2x2  ***+-0--1-*/
  "\000\011"				/*0| 0  3 */
  "\001\010", 				/*1| 2  1 */
    				/* 3x3  ***+-0--1--2-*/
  "\000\011\012"			/*0| 0  5  3 */
  "\020\001\010"			/*1| 4  1  7 */
  "\022\021\002", 			/*2| 8  2  6 */
    				/* 4x4  ***+-0--1--2--3-*/
  "\000\022\002\020"			/*0| 0  8  3 11 */
  "\011\033\013\031"			/*1|12  4 15  7 */
  "\010\032\012\030"			/*2| 2 10  1  9 */
  "\021\003\023\001", 			/*3|14  6 13  5 */
    				/* 5x5  ***+-0--1--2--3--4-*/
  "\000\012\024\031\043"		/*0| 0  5 10 15 20 */
  "\010\022\034\041\003"		/*1|13 18 23  3  8 */
  "\020\032\044\001\013"		/*2|21  1  6 11 16 */
  "\030\042\004\011\023"		/*3| 9 14 19 24  4 */
  "\040\002\014\021\033", 		/*4|17 22  2  7 12 */
				/* 6x6  ***+-0--1--2--3--4--5-*/
  "\000\033\002\035\004\031" 		/*0| 0 18 16 34  9 27 */
  "\011\044\013\040\015\042" 		/*1|33  6 24  5 23 15 */
  "\022\055\024\051\020\053" 		/*2| 2 20 12 30 11 29 */
  "\010\043\012\045\014\041" 		/*3|35  8 26  1 19 17 */
  "\021\054\023\050\025\052" 		/*4| 4 22 14 32  7 25 */
  "\032\005\034\001\030\003" 		/*5|31 10 28  3 21 13 */
};

static void create_dither(int r, int g, int b) {
  YglWindow *w = W;
  int i, c;
  int sz  = Ygl.DSZ, szq = sz * sz;
  int ri, gi, bi, rx, gx, bx, ry, gy, by, red, green, blue;
  static int init = 0;
  static int rn, gn, bn;
  
  if(init == 0) {		/* Initialize static vars */
    init = 1;
    rn = ((256 >> Ygl.rb) - 1) * szq + 1; /* Number of dither patterns red   */
    gn = ((256 >> Ygl.gb) - 1) * szq + 1; /* Number of dither patterns green */
    bn = ((256 >> Ygl.bb) - 1) * szq + 1; /* Number of dither patterns blue  */
#ifdef DEBUG
    fprintf(stderr, "create_dither: sz = %d, {rn, gn, bn} = {%d,%d,%d}\n",
	    sz, rn, gn, bn);
#endif
  }
  
  /* Index of dither pat., base color, # elements with color rx + 1 */
  ri = (r * rn) >> 8; rx = ri / szq; ry = ri % szq;  
  gi = (g * gn) >> 8; gx = gi / szq; gy = gi % szq;
  bi = (b * bn) >> 8; bx = bi / szq; by = bi % szq;
  
  red   = (ri << 8) / rn;
  green = (gi << 8) / gn;
  blue  = (bi << 8) / bn;
  
  if(red != w->red || green != w->green || blue != w->blue) { /*Already set?*/
    /* No, we must change pattern/color */
    Ulong mask;
    XGCValues values;
    
    w->red = red; w->green = green; w->blue = blue;
    if(ry + gy + by == 0) {
      /* We hit pure color, don't dither but set foreground in w->gc */
      mask = GCFillStyle | GCForeground;
      values.fill_style = FillSolid;
      values.foreground =
	((rx << Ygl.rs) & Ygl.RMask) +
	((gx << Ygl.gs) & Ygl.GMask) +
	((bx << Ygl.bs) & Ygl.BMask);
    } else {
      /* Dither. */
      mask = GCTile | GCFillStyle;
      values.fill_style  = FillTiled;
      values.tile        = w->pm;
      
      for(c = szq; c > 0; c--) {
	if(c == ry) rx++;	/* Go to next color */
	if(c == gy) gx++;
	if(c == by) bx++;
	
	i = DI[sz][c-1];	/* Next position in dither matrix */
	
	XPutPixel(w->pmi, i >> 3, i & 7, /* Simplified RGB_TO_RGBVisual */
		  ((rx << Ygl.rs) & Ygl.RMask) +
		  ((gx << Ygl.gs) & Ygl.GMask) +
		  ((bx << Ygl.bs) & Ygl.BMask));
      }
      
      XPutImage(D, w->pm, w->pmgc, w->pmi, 0, 0, 0, 0, sz, sz);
    }
    XChangeGC(D, w->gc, mask, &values);
  }
}

/* converts 24 bit RGB values ( 0xBBGGRR ) to value appropiate for RGBVisual */
#define RGB24_TO_RGBVisual(rgb24) ( \
	((((rgb24) >> (Ygl.rb +  0)) << Ygl.rs) & Ygl.RMask) \
      + ((((rgb24) >> (Ygl.gb +  8)) << Ygl.gs) & Ygl.GMask) \
      + ((((rgb24) >> (Ygl.bb + 16)) << Ygl.bs) & Ygl.BMask) )

#define RGBVisual_TO_RGB24(pixel) ( \
	((((pixel) & Ygl.RMask) >> Ygl.rs) << (Ygl.rb +  0)) \
      + ((((pixel) & Ygl.GMask) >> Ygl.gs) << (Ygl.gb +  8)) \
      + ((((pixel) & Ygl.BMask) >> Ygl.bs) << (Ygl.bb + 16)) )

#define RGB_TO_RGBVisual(r,g,b) ( \
	((((r) >> Ygl.rb) << Ygl.rs) & Ygl.RMask) \
      + ((((g) >> Ygl.gb) << Ygl.gs) & Ygl.GMask) \
      + ((((b) >> Ygl.bb) << Ygl.bs) & Ygl.BMask) )

static void set_color(const char *caller, int r, int g, int b) {
  I(caller);
  if(!W->rgb) { Yprintf(caller, "not in RGB mode.\n"); return; }
  if(W->pm) {
    create_dither(r, g, b);
  } else {
    XSetForeground(D, W->gc, RGB_TO_RGBVisual(r, g, b));
  }
}

void RGBcolor(Int16 r, Int16 g, Int16 b) {
  const char * MyName = "RGBcolor";
  set_color(MyName, r, g, b);
}

void cpack(Uint32 rgb) {
  set_color("cpack", 
	    (rgb >>  0) & 0xFF, (rgb >>  8) & 0xFF, (rgb >> 16) & 0xFF);
}

void c3s(Int16 c[3]) {
  set_color("c3s", c[0], c[1], c[2]);
}

void c3i(Int32 c[3]) {
  set_color("c3i", c[0], c[1], c[2]);
}

void c3f(Float32 c[3]) {
  float f = 255.9999999;
  set_color("c3f", (int)(f * c[0]), (int)(f * c[1]), (int)(f * c[2]));
}

#if 0 /* This version is veeeryyy slooowwww !!! */
void RGBcolor (Int16 r, Int16 g, Int16 b) {
  XColor xc;
  const char * MyName = "RGBcolor";
  I(MyName);
  if(!W->rgb) { Yprintf(MyName, "not in RGB mode.\n"); return; }
  xc.red   = r << 8;
  xc.green = g << 8;
  xc.blue  = b << 8;
  if(XAllocColor(D, Ygl.RCmap, &xc))
    XSetForeground(D, W->gc, xc.pixel);
  else
    Yprintf(MyName, "can't allocate color ( %d, %d, %d).\n", r, g, b);
}
#endif

Int32 getcolor(void) {
  const char * MyName = "getcolor";
  I(MyName);
  if(W->rgb) { Yprintf(MyName, "not in CMap mode.\n"); return 0; }
  return W->color;
}

void getmcolor (Colorindex ind, Int16 *r, Int16 *g, Int16 *b) {
  XColor xc;
  const char * MyName = "getmcolor";
  I(MyName);
  if(W->rgb) { Yprintf(MyName, "not in CMap mode.\n"); return; }
  xc.pixel = YGL_COLORS(ind);
  XQueryColor(D, Ygl.CCmap, &xc);
  *r = xc.red   >> 8;
  *g = xc.green >> 8;
  *b = xc.blue  >> 8;
}

void getmcolors (Colorindex ind1, Colorindex ind2, Int16 *r, Int16 *g, Int16 *b) {
  XColor *xc;
  Colorindex i;
  int n = ind2 - ind1 + 1;
  const char * MyName = "getmcolors";
  I(MyName);
  if(W->rgb) {
    Yprintf(MyName, "not in CMap mode.\n");
    return;
  }

  if(n < 0) {
    Yprintf(MyName, "2nd argument < 1st argument.\n");
    return;
  }
  
  if(NULL == (xc = (XColor*) malloc(n * sizeof(XColor)))) {
    Yprintf(MyName, "can't allocate memory.\n");
    exit(1);
  }

  for(i = 0; i < n; i++) xc[i].pixel = YGL_COLORS(i);
  XQueryColors(D, Ygl.CCmap, xc, n);
  for(i = 0; i < n; i++) {
    r[i] = xc[i].red   >> 8;
    g[i] = xc[i].green >> 8;
    b[i] = xc[i].blue  >> 8;
  }
  free(xc);
}

void gRGBcolor(Int16 *r, Int16 *g, Int16 *b) {
  XGCValues ret;
  const char * MyName = "gRGBcolor";
  I(MyName);
  if(!W->rgb) {
    Yprintf(MyName, "not in RGB mode.\n");
    return;
  }
  if(W->pm) {
    *r = W->red;
    *g = W->green;
    *b = W->blue;
  } else {
    if(XGetGCValues(D, W->gc, GCForeground, &ret)) {
      *r = ((ret.foreground & Ygl.RMask) >> Ygl.rs) << Ygl.rb;
      *g = ((ret.foreground & Ygl.GMask) >> Ygl.gs) << Ygl.gb;
      *b = ((ret.foreground & Ygl.BMask) >> Ygl.bs) << Ygl.bb;
    } else {
      Yprintf(MyName, "cannot get GCValues.\n");
    }
  }
#ifdef DEBUG
  fprintf(stderr, "gRGBcolor: foreground = %d, (r,g,b) = (%d,%d,%d).\n", ret.foreground,*r,*g,*b);
#endif
}

void color(Colorindex ind) {
  const char * MyName = "color";
  I(MyName);
  if(W->rgb)
    Yprintf(MyName, "not in CMap mode.\n");
  else if(ind >= CMapSize)
    Yprintf(MyName, "invalid color %d (must be between 0 and %d).\n", ind, CMapSize-1);
  else {
    W->color = ind;
    if(Ygl.GC) /* single GC */
      XSetForeground(D, W->gc, YGL_COLORS(ind));
    else
      W->gc = W->gclist[YGL_COLORS(ind)];
  }
}

#ifndef YGL_PREFIX
void Xcolor(Colorindex ind) {
  const char * MyName = "Xcolor";
  I(MyName);
  if(W->rgb)
    Yprintf(MyName, "not in CMap mode.\n");
  else if(ind >= CMapSize)
    Yprintf(MyName, "invalid color %d (must be between 0 and %d).\n", ind, CMapSize-1);
  else {
    W->color = YGL_COLORSINV(ind);
    if(Ygl.GC) /* single GC */
      XSetForeground(D, W->gc, ind);
    else
      W->gc = W->gclist[ind];
  }
}

void getmXcolor (Colorindex ind, Int16 *r, Int16 *g, Int16 *b) {
  XColor xc;
  const char * MyName = "getmXcolor";
  I(MyName);
  if(W->rgb)
    Yprintf(MyName, "not in CMap mode.\n");
  else {
    xc.pixel = ind;
    XQueryColor(D, Ygl.CCmap, &xc);
    *r = xc.red   >> 8;
    *g = xc.green >> 8;
    *b = xc.blue  >> 8;
  }
}
#endif

/* block pixel transfer routines */
static int rect_read_front = True;

void readsource(Int32 source) {
  const char * MyName = "readsource";
  I(MyName);
  switch(source) {
  case SRC_AUTO: 
  case SRC_BACK: 
    rect_read_front = False;
    break;
  case SRC_FRONT: 
    rect_read_front = True;
    break;
  default:
    Yprintf(MyName, "unknown mode %d.\n", source);
    break;
  }
}

static int rect_read_err;
static int rect_read_errh(Display *dpy, XErrorEvent *error) {
  rect_read_err = error->error_code;
  return 0;
}

static Int32 rect_read(const char *caller,
		       Screencoord x1, Screencoord y1,
		       Screencoord x2, Screencoord y2,
		       int size, void *data) {
  XImage *XI;
  register long x, y;
  register Int32 *data32 = (Int32*) data;
  register Int16 *data16 = (Int16*) data;
  register Uint8 *data8  = (Uint8*) data;
  Screencoord width  = x2 - x1 + 1;
  Screencoord height = y2 - y1 + 1;
  int (*old_handler)(Display*, XErrorEvent*);
  
  rect_read_err = 0; /* reset */
  old_handler = XSetErrorHandler(rect_read_errh);
  
  XI = XGetImage(D, 
#ifdef RGBWIN
		 rect_read_front ? W->win : W->draw,
#else
		 rect_read_front ? W->main : W->draw,
#endif
		 x1, W->ym - 1 - y2,
		 width, height,
		 (1 << YglDepth())-1,
		 ZPixmap);
  
  XSync (D, False);
  XSetErrorHandler(old_handler); /* reset */
  
#ifdef DEBUG
  fprintf(stderr, "rect_read: (w, h) = (%d, %d), XI->(w, h) = (%d, %d), err = %d\n",
	  width, height, XI->width, XI->height, rect_read_err);
#endif
  if(rect_read_err == BadMatch) {
    Yprintf(caller, "window not completely visible.\n");
    return 0;
  }
  
  for(y = XI->height-1; y >= 0; y--) {
#if DEBUG > 1
    fprintf(stderr, "y = %d\n", y);
#endif
    for(x = 0; x < XI->width; x++) {
      Ulong pixel;
      long val;
      
      pixel = XGetPixel(XI, x, y);
      
      if(W->rgb) {
	val = RGBVisual_TO_RGB24(pixel);
      } else {
	val = YGL_COLORSINV(pixel);
      }
      
      switch(size) {
      case 1: *(data8++)  = val; break;
      case 2: *(data16++) = val; break;
      case 4: *(data32++) = val; break;
      }
#if DEBUG > 2
      fprintf(stderr, "%x,%x ", pixel, val);
#endif
    }
#if DEBUG > 2
    fputc(13, stderr);
#endif
  }
  x = XI->width * XI->height;
#ifdef DEBUG
  fprintf(stderr, "loop end.");
#endif
  XDestroyImage(XI);
  F;
  return x;
}

static void rect_write(const char *caller,
		       Screencoord x1, Screencoord y1,
		       Screencoord x2, Screencoord y2,
		       int size, void *data) {
  XImage *XI;
  register long x, y;
  register Int32 *data32 = (Int32*) data;
  register Int16 *data16 = (Int16*) data;
  register Uint8 *data8  = (Uint8*) data;
  Screencoord width  = x2 - x1 + 1;
  Screencoord height = y2 - y1 + 1;
  
  XI = XCreateImage(D,
		    YglVisual(),
		    YglDepth(),
		    ZPixmap,	/* Format */
		    0,		/* Offset */
		    NULL,	/* Data */
		    width,	/* Width */
		    height,	/* Height */
		    8, 		/* BitmapPad */
		    0);		/* BytesPerLine */

  if(NULL == (XI->data = (char*) malloc(XI->bytes_per_line * height))) {
    Yprintf(caller, "can't allocate memory.\n");
    exit(1);
  }
  
  for(y = height - 1; y >= 0; y--) for(x = 0; x < width; x++) {
    Ulong pixel;
    long val;
    switch(size) {
    case 1: val = *(data8++);  break;
    case 2: val = *(data16++); break;
    case 4: val = *(data32++); break;
    }
    if(W->rgb) {
      pixel = RGB24_TO_RGBVisual(val);
      XPutPixel(XI, x, y, pixel); 
      /* *(d + y * XI->bytes_per_scanline + x) = pixel; */
    } else { /* CMap */
      if(val < 0 || val >= CMapSize) {
	Yprintf(caller, "invalid color %d at position (%d,%d) in CMap mode (must be between 0 and %d).\n",
		val, x, y, CMapSize-1);
      } else {
	XPutPixel(XI, x, y, YGL_COLORS(val));
      }
    }
  }
  
  XPutImage(D, W->draw, W->gc, XI, 0, 0,
	    x1, W->ym - 1 - y2,
	    width, height);
  
  XDestroyImage(XI);
  F;
}

Int32 lrectread(Screencoord x1, Screencoord y1,	Screencoord x2, Screencoord y2,	Int32 *data) { const char * MyName = "lrectread"; I(MyName); return rect_read(MyName, x1,y1,x2,y2, 4, data);}
Int32  rectread(Screencoord x1, Screencoord y1,	Screencoord x2, Screencoord y2,	Int16 *data) { const char * MyName = "rectread" ; I(MyName); return rect_read(MyName, x1,y1,x2,y2, 2, data);}
Int32 crectread(Screencoord x1, Screencoord y1,	Screencoord x2, Screencoord y2,	Uint8 *data) { const char * MyName = "crectread"; I(MyName); return rect_read(MyName, x1,y1,x2,y2, 1, data);}

void lrectwrite(Screencoord x1, Screencoord y1, Screencoord x2, Screencoord y2, Int32 *data) { const char * MyName = "lrectwrite"; I(MyName); rect_write(MyName, x1,y1,x2,y2, 4, data);}
void  rectwrite(Screencoord x1, Screencoord y1,	Screencoord x2, Screencoord y2,	Int16 *data) { const char * MyName = "rectwrite" ; I(MyName); rect_write(MyName, x1,y1,x2,y2, 2, data);}
void crectwrite(Screencoord x1, Screencoord y1,	Screencoord x2, Screencoord y2,	Uint8 *data) { const char * MyName = "crectwrite"; I(MyName); rect_write(MyName, x1,y1,x2,y2, 1, data);}

void rectcopy(Screencoord x1, Screencoord y1,
	      Screencoord x2, Screencoord y2,
	      Screencoord xn, Screencoord yn) {
  const char * MyName = "rectcopy";
  I(MyName);
  XCopyArea(D, W->draw, W->draw, W->gc,
	    x1, W->ym - 1 - y2,
	    x2 - x1 + 1,
	    y2 - y1 + 1,
	    xn, W->ym - 1 - yn - (y2-y1));
  F;
}

Int32 readpixels(Int16 n, Colorindex data[]) {
  Screencoord x, y;
  Int32 r;
  const char * MyName = "readpixels";
  I(MyName);
  if(W->rgb) {
    Yprintf(MyName, "not in CMap mode.\n");
    exit(1);
  }
  x = X(W->xc);
  y = Y(W->yc);
  r = rect_read(MyName, x, y, x + n, y + 1, 2, data);
  W->xc += n / W->xf;
  return r;
}

void writepixels(Int16 n, Colorindex data[]) {
  Screencoord x, y;
  const char * MyName = "writepixels";
  I(MyName);
  if(W->rgb) {
    Yprintf(MyName, "not in CMap mode.\n");
    exit(1);
  }
  x = X(W->xc);
  y = Y(W->yc);
  rect_write(MyName, x, y, x + n, y + 1, 2, data);
  W->xc += n / W->xf;
}

Int32 readRGB(Int16 n, RGBvalue r[], RGBvalue g[], RGBvalue b[]) {
  Screencoord x, y;
  Int32 ret, *data, i;
  const char * MyName = "readRGB";
  I(MyName);
  if(!W->rgb) {
    Yprintf(MyName, "not in RGB mode.\n");
    exit(1);
  }
  x = X(W->xc);
  y = Y(W->yc);
  W->xc += n / W->xf;
  if(NULL == (data = (Int32*)malloc(n * sizeof(Int32)))) {
    Yprintf(MyName, "out of memory.\n");
    exit(1);
  }
  ret = rect_read(MyName, x, y, x + n, y + 1, 4, data);
  for(i = 0; i < n; i++) {
    r[i] = (data[i] >>  0) & 0xFF;
    g[i] = (data[i] >>  8) & 0xFF;
    b[i] = (data[i] >> 16) & 0xFF;
  }
  free(data);
  return ret;
}

void writeRGB(Int16 n, RGBvalue r[], RGBvalue g[], RGBvalue b[]) {
  Screencoord x, y;
  Int32 *data, i;
  const char * MyName = "writeRGB";
  I(MyName);
  if(!W->rgb) {
    Yprintf(MyName, "not in RGB mode.\n");
    exit(1);
  }
  x = X(W->xc);
  y = Y(W->yc);
  W->xc += n / W->xf;
  if(NULL == (data = (Int32*)malloc(n * sizeof(Int32)))) {
    Yprintf(MyName, "out of memory.\n");
    exit(1);
  }
  for(i = 0; i < n; i++) {
    data[i] = (b[i] << 16) + (g[i] << 8) + r[i];
  }
  rect_write(MyName, x, y, x + n, y + 1, 4, data);  
  free(data);
}
