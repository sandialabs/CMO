/*
 *    Ygl: Run 2d-GL programs with standard X11 routines.
 *    (C) Fred Hucht 1993-96
 *    EMail: fred@thp.Uni-Duisburg.DE
 */

#ifdef YGL_PREFIX
# include <X11/Yglprefix.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <gl/gl.h>
#include <gl/device.h>
#include <X11/Xlib.h>

#ifndef TRUE
# define TRUE (0==0)
# define FALSE (!TRUE)
#endif
#define BYTE(x, n) (((x)>>(n)) & 0xFF)

extern int errno;
static int broken_pipe;

static void pipe_error(int sig_no) {
  broken_pipe = TRUE;
}

static int check_error(const char *fname, void (*oldsp)(int), int err) {
  if(broken_pipe) {
    fprintf(stderr, "gl2ppm: Broken pipe while writing to '%s'.\n", fname+1);
  } else {
    if(err) {
      perror("gl2ppm: ");
    } else {
      return FALSE; /* No error */
    }
  }
  signal(SIGPIPE, oldsp);
  return TRUE;
}

int gl2ppm(const char *fname) {
  /*
   * Read the active [Y]gl-window and write a ppm file
   * If fname[0] == '|', use popen.
   * return value:
   *  1 success
   *  0 not all bytes written or can't open file
   *
   * Fred Hucht 1992/95
   */
  int cells, size, r;
  Int16 *R, *G, *B;
  Int32 *buf, dx, dy, dxdy;
  register Int32 ix, iy;
  FILE *tn = NULL;
  Int32 DisplayMode = getdisplaymode(), CMapMode, DBufMode;
  void (*oldsigpipe)(int);
  
  CMapMode = DisplayMode == DMSINGLE || DisplayMode == DMDOUBLE;  /* cmap mode? */
  DBufMode = DisplayMode == DMDOUBLE || DisplayMode == DMRGBDOUBLE;
  
  getsize (&dx, &dy);		/* Get size of GL window */
  dxdy = dx*dy;
  
  if(NULL == (buf = (Int32*) calloc(dxdy, sizeof (Int32)))) {
    fprintf(stderr, "gl2ppm: can't allocate memory.\n");
    exit(1);
  }
  
#ifdef DEBUG
  fprintf(stderr, "gl2ppm: reading %d Pixels.\n", dxdy);
#endif
  if(DBufMode) readsource(SRC_FRONT);
  size = lrectread(0, 0, dx-1, dy-1, buf);
  if(DBufMode) readsource(SRC_AUTO);
#ifdef DEBUG
  fprintf(stderr, "gl2ppm: read %d of %d Pixels.\n", size, dxdy);
#endif
  
#if defined(DEBUG) || defined(JOKE)
  if(DBufMode) frontbuffer(TRUE);
  clear(); sleep(1); lrectwrite (0, 0, dx-1, dy-1, buf); /* Little joke... */
  if(DBufMode) frontbuffer(FALSE);
#endif
  
  if(CMapMode) {		/* cmap mode */  
    cells = 1 << getplanes();	/* Number of colormap cells */
    R = (Int16*) malloc(sizeof (Int16) * cells);
    G = (Int16*) malloc(sizeof (Int16) * cells);
    B = (Int16*) malloc(sizeof (Int16) * cells);
    if(R == NULL || G == NULL || B == NULL) {
      fprintf(stderr, "gl2ppm: Can't allocate memory.\n");
      exit(1);
    }
    getmcolors(0, cells - 1, R, G, B);	/* Load colormap. */
  }
  
  broken_pipe = FALSE;
  oldsigpipe = signal(SIGPIPE, pipe_error);
  
  tn = (fname[0] != '|') ? fopen(fname, "w") : (FILE*) popen(fname+1, "w");
  
  if(tn == NULL) {
    perror("gl2ppm: cannot open file");
    signal(SIGPIPE, oldsigpipe);
    return FALSE;
  } else {
    errno = 0; /* Don't ask me. Bug in some versions of AIX3 */
  }
#ifdef DEBUG
  if(fname[0] == '|') {
    fprintf(stderr, "gl2ppm: piping through '%s'. errno = %d\n",
	    fname, errno);
  } else {
    fprintf(stderr, "gl2ppm: writing ppm-file '%s'. errno = %d\n",
	    fname, errno);
  }
#endif
  
  r = 0 > fprintf(tn,"P6\n%d %d\n255\n",dx,dy); /* Write ppm-Header. */
  if(check_error(fname, oldsigpipe, r)) return FALSE;
  
  if(CMapMode) {
    for (iy = dy-1; iy >= 0; iy--) for (ix = 0; ix < dx; ix++) {
      Int32 b = buf[iy * dx + ix] & (cells-1);
      r =  EOF == putc((char)R[b],tn) /* Write RGB bytes */
	|| EOF == putc((char)G[b],tn)
	|| EOF == putc((char)B[b],tn);
      if(check_error(fname, oldsigpipe, r)) return FALSE;
    }
  } else {
    /* Do brightness correction */
    Int32 white = 0xFFFFFF, maxr, maxg, maxb;
    if(DBufMode) {
      frontbuffer(TRUE);
      readsource(SRC_FRONT);
    }
    lrectwrite(0, 0, 0, 0, &white); /* Set white pixel at (0,0) */
    lrectread (0, 0, 0, 0, &white); /* Read real colors */
#ifdef DEBUG
    sleep(1);
#endif
    lrectwrite(0, 0, 0, 0, &buf[0]);   /* Restore pixel at (0,0) */
    if(DBufMode) {
      frontbuffer(FALSE);
      readsource(SRC_AUTO);
    }
    
    maxr = BYTE(white, 0);
    maxg = BYTE(white, 8);
    maxb = BYTE(white,16);
#ifdef DEBUG
    fprintf(stderr, "gl2ppm: RGB white = (%d,%d,%d)\n",
	    maxr, maxg, maxb);
#endif
    
    for (iy = dy-1; iy >= 0; iy--) for (ix = 0; ix < dx; ix++) {
      Int32 b = buf[iy * dx + ix]; /* Write RGB bytes */
      r =  EOF == putc((char) (0xFF * BYTE(b, 0) / maxr), tn)
	|| EOF == putc((char) (0xFF * BYTE(b, 8) / maxg), tn)
	|| EOF == putc((char) (0xFF * BYTE(b,16) / maxb), tn);
      if(check_error(fname, oldsigpipe, r)) return FALSE;
    }
  }
  if(fname[0] != '|') fclose(tn);
  else                pclose(tn);
  free(buf);
  if(CMapMode) {
    free(R);
    free(G);
    free(B);
  }
#ifdef DEBUG
  fprintf(stderr, "gl2ppm: ready.\n");
#endif
  signal(SIGPIPE, oldsigpipe);
  return size == dxdy;
}
