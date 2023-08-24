/*
 *    Ygl: Run 2d-GL programs with standard X11 routines.
 *    (C) Fred Hucht 1993-96
 *    EMail: fred@thp.Uni-Duisburg.DE
 */

#include "header.h"

#define DWGC D,W->draw,W->gc
#define Coo Coord
#define Ico Icoord
#define Sco Scoord

/* Drawing */
void clear(void) { I("clear"); XFillRectangle(DWGC, 0, 0, W->xm, W->ym); F;}

/* Points */
void pnt2  (Coo x, Coo y) { I("pnt2" ); if(W->pm) XFillRectangle(DWGC, XS(x), YS(y), 1, 1);else XDrawPoint(DWGC, XS(x), YS(y)); F;}
void pnt2i (Ico x, Ico y) { I("pnt2i"); if(W->pm) XFillRectangle(DWGC, XS(x), YS(y), 1, 1);else XDrawPoint(DWGC, XS(x), YS(y)); F;}
void pnt2s (Sco x, Sco y) { I("pnt2s"); if(W->pm) XFillRectangle(DWGC, XS(x), YS(y), 1, 1);else XDrawPoint(DWGC, XS(x), YS(y)); F;}

/* Lines */
void move2 (Coo x, Coo y) { SCP( =x, =y);}
void move2i(Ico x, Ico y) { SCP( =x, =y);}
void move2s(Sco x, Sco y) { SCP( =x, =y);}

void rmv2  (Coo x, Coo y) { SCP(+=x,+=y);}
void rmv2i (Ico x, Ico y) { SCP(+=x,+=y);}
void rmv2s (Sco x, Sco y) { SCP(+=x,+=y);}

void draw2 (Coo x, Coo y) { I("draw2" ); XDrawLine(DWGC, X(W->xp), Y(W->yp), X(x), Y(y)); SCP( =x, =y); F;}
void draw2i(Ico x, Ico y) { I("draw2i"); XDrawLine(DWGC, X(W->xp), Y(W->yp), X(x), Y(y)); SCP( =x, =y); F;}
void draw2s(Sco x, Sco y) { I("draw2s"); XDrawLine(DWGC, X(W->xp), Y(W->yp), X(x), Y(y)); SCP( =x, =y); F;}

void rdr2  (Coo x, Coo y) { I("rdr2" ); XDrawLine(DWGC, X(W->xp), Y(W->yp), X(W->xp+x), Y(W->yp+y)); SCP(+=x,+=y); F;}
void rdr2i (Ico x, Ico y) { I("rdr2i"); XDrawLine(DWGC, X(W->xp), Y(W->yp), X(W->xp+x), Y(W->yp+y)); SCP(+=x,+=y); F;}
void rdr2s (Sco x, Sco y) { I("rdr2s"); XDrawLine(DWGC, X(W->xp), Y(W->yp), X(W->xp+x), Y(W->yp+y)); SCP(+=x,+=y); F;}

/* Arcs & Circles */
void arc   (Coo x, Coo y, Coo r, Angle s, Angle e) { I("arc" ); r=MAX(0,r); XDrawArc(DWGC, X(x-r), Y(y+r), XR(2*r), YR(2*r), (64*s)/10, (64*(e-s))/10); F;}
void arci  (Ico x, Ico y, Ico r, Angle s, Angle e) { I("arci"); r=MAX(0,r); XDrawArc(DWGC, X(x-r), Y(y+r), XR(2*r), YR(2*r), (64*s)/10, (64*(e-s))/10); F;}
void arcs  (Sco x, Sco y, Sco r, Angle s, Angle e) { I("arcs"); r=MAX(0,r); XDrawArc(DWGC, X(x-r), Y(y+r), XR(2*r), YR(2*r), (64*s)/10, (64*(e-s))/10); F;}

void arcf  (Coo x, Coo y, Coo r, Angle s, Angle e) { I("arcf" ); r=MAX(0,r); XFillArc(DWGC, X(x-r), Y(y+r), XR(2*r), YR(2*r), (64*s)/10, (64*(e-s))/10); F;}
void arcfi (Ico x, Ico y, Ico r, Angle s, Angle e) { I("arcfi"); r=MAX(0,r); XFillArc(DWGC, X(x-r), Y(y+r), XR(2*r), YR(2*r), (64*s)/10, (64*(e-s))/10); F;}
void arcfs (Sco x, Sco y, Sco r, Angle s, Angle e) { I("arcfs"); r=MAX(0,r); XFillArc(DWGC, X(x-r), Y(y+r), XR(2*r), YR(2*r), (64*s)/10, (64*(e-s))/10); F;}

void circ  (Coo x, Coo y, Coo r)                   { I("circ" ); r=MAX(0,r); XDrawArc(DWGC, X(x-r), Y(y+r), XR(2*r), YR(2*r), 0, 64*360); F;}
void circi (Ico x, Ico y, Ico r)                   { I("circi"); r=MAX(0,r); XDrawArc(DWGC, X(x-r), Y(y+r), XR(2*r), YR(2*r), 0, 64*360); F;}
void circs (Sco x, Sco y, Sco r)                   { I("circs"); r=MAX(0,r); XDrawArc(DWGC, X(x-r), Y(y+r), XR(2*r), YR(2*r), 0, 64*360); F;}

void circf (Coo x, Coo y, Coo r)                   { I("circf" ); r=MAX(0,r); XFillArc(DWGC, X(x-r), Y(y+r), XR(2*r), YR(2*r), 0, 64*360); F;}
void circfi(Ico x, Ico y, Ico r)                   { I("circfi"); r=MAX(0,r); XFillArc(DWGC, X(x-r), Y(y+r), XR(2*r), YR(2*r), 0, 64*360); F;}
void circfs(Sco x, Sco y, Sco r)                   { I("circfs"); r=MAX(0,r); XFillArc(DWGC, X(x-r), Y(y+r), XR(2*r), YR(2*r), 0, 64*360); F;}

/* Rects & Boxes */
void rect  (Coo x1, Coo y1, Coo x2, Coo y2) {
  int x, y;
  Uint w, h;
  I("rect" );
  x = X(MIN(x1,x2));
  y = Y(MAX(y1,y2));
  w = XR(ABS(x2-x1));
  h = YR(ABS(y2-y1));
  XDrawRectangle(DWGC, x, y, w, h);
  F;
}

void sbox  (Coo x1, Coo y1, Coo x2, Coo y2) { I("sbox" ); XDrawRectangle(DWGC, X(MIN(x1,x2)), Y(MAX(y1,y2)), XR(ABS(x2-x1)), YR(ABS(y2-y1))); F;}
void recti (Ico x1, Ico y1, Ico x2, Ico y2) { I("recti"); XDrawRectangle(DWGC, X(MIN(x1,x2)), Y(MAX(y1,y2)), XR(ABS(x2-x1)), YR(ABS(y2-y1))); F;}
void sboxi (Ico x1, Ico y1, Ico x2, Ico y2) { I("sboxi"); XDrawRectangle(DWGC, X(MIN(x1,x2)), Y(MAX(y1,y2)), XR(ABS(x2-x1)), YR(ABS(y2-y1))); F;}
void rects (Sco x1, Sco y1, Sco x2, Sco y2) { I("rects"); XDrawRectangle(DWGC, X(MIN(x1,x2)), Y(MAX(y1,y2)), XR(ABS(x2-x1)), YR(ABS(y2-y1))); F;}
void sboxs (Sco x1, Sco y1, Sco x2, Sco y2) { I("sboxs"); XDrawRectangle(DWGC, X(MIN(x1,x2)), Y(MAX(y1,y2)), XR(ABS(x2-x1)), YR(ABS(y2-y1))); F;}

void rectf (Coo x1, Coo y1, Coo x2, Coo y2) { I("rectf" ); XFillRectangle(DWGC, X(MIN(x1,x2)), Y(MAX(y1,y2)), 1+XR(ABS(x2-x1)), 1+YR(ABS(y2-y1))); F;}
void sboxf (Coo x1, Coo y1, Coo x2, Coo y2) { I("sboxf" ); XFillRectangle(DWGC, X(MIN(x1,x2)), Y(MAX(y1,y2)), 1+XR(ABS(x2-x1)), 1+YR(ABS(y2-y1))); F;}
void rectfi(Ico x1, Ico y1, Ico x2, Ico y2) { I("rectfi"); XFillRectangle(DWGC, X(MIN(x1,x2)), Y(MAX(y1,y2)), 1+XR(ABS(x2-x1)), 1+YR(ABS(y2-y1))); F;}
void sboxfi(Ico x1, Ico y1, Ico x2, Ico y2) { I("sboxfi"); XFillRectangle(DWGC, X(MIN(x1,x2)), Y(MAX(y1,y2)), 1+XR(ABS(x2-x1)), 1+YR(ABS(y2-y1))); F;}
void rectfs(Sco x1, Sco y1, Sco x2, Sco y2) { I("rectfs"); XFillRectangle(DWGC, X(MIN(x1,x2)), Y(MAX(y1,y2)), 1+XR(ABS(x2-x1)), 1+YR(ABS(y2-y1))); F;}
void sboxfs(Sco x1, Sco y1, Sco x2, Sco y2) { I("sboxfs"); XFillRectangle(DWGC, X(MIN(x1,x2)), Y(MAX(y1,y2)), 1+XR(ABS(x2-x1)), 1+YR(ABS(y2-y1))); F;}

static Int32 PMode = Convex;
static Ulong q_len = 0, q_max = 0;
static XPoint *queue = NULL;

static void q_add(const char *caller, int x, int y) {
  if(q_max <= q_len) {
    q_max += 0x1000;
    if(queue == NULL)	/* Make new queue */
      queue = (XPoint*)malloc(q_max * sizeof(XPoint));
    else		/* Resize queue */
      queue = (XPoint*)realloc(queue, q_max * sizeof(XPoint));
    if(queue == NULL) {
      Yprintf(caller, "out of memory.\n");
      exit(1);
    }
  }
  /* Add point */
  queue[q_len].x = x;
  queue[q_len].y = y;
  q_len++;
}

void concave(Int32 bool) { PMode = bool ? Complex : Convex;}

void  pmv2 (Coo x, Coo y) { const char *MyName = "pmv2" ; I(MyName); SCP( =x,  =y); q_len = 0; q_add(MyName, X(x), Y(y));}
void  pmv2i(Ico x, Ico y) { const char *MyName = "pmv2i"; I(MyName); SCP( =x,  =y); q_len = 0; q_add(MyName, X(x), Y(y));}
void  pmv2s(Sco x, Sco y) { const char *MyName = "pmv2s"; I(MyName); SCP( =x,  =y); q_len = 0; q_add(MyName, X(x), Y(y));}

void rpmv2 (Coo x, Coo y) { const char *MyName = "rpmv2" ; I(MyName); SCP(+=x, +=y); q_len = 0; q_add(MyName, X(x), Y(y));}
void rpmv2i(Ico x, Ico y) { const char *MyName = "rpmv2i"; I(MyName); SCP(+=x, +=y); q_len = 0; q_add(MyName, X(x), Y(y));}
void rpmv2s(Sco x, Sco y) { const char *MyName = "rpmv2s"; I(MyName); SCP(+=x, +=y); q_len = 0; q_add(MyName, X(x), Y(y));}

void  pdr2 (Coo x, Coo y) { const char *MyName = "pdr2" ; I(MyName); q_add(MyName, X(x), Y(y));}
void  pdr2i(Ico x, Ico y) { const char *MyName = "pdr2i"; I(MyName); q_add(MyName, X(x), Y(y));}
void  pdr2s(Sco x, Sco y) { const char *MyName = "pdr2s"; I(MyName); q_add(MyName, X(x), Y(y));}

void rpdr2 (Coo x, Coo y) { const char *MyName = "rpdr2" ; I(MyName); q_add(MyName, queue[q_len-1].x + X(x), queue[q_len-1].y - Y(y));}
void rpdr2i(Ico x, Ico y) { const char *MyName = "rpdr2i"; I(MyName); q_add(MyName, queue[q_len-1].x + X(x), queue[q_len-1].y - Y(y));}
void rpdr2s(Sco x, Sco y) { const char *MyName = "rpdr2s"; I(MyName); q_add(MyName, queue[q_len-1].x + X(x), queue[q_len-1].y - Y(y));}

void pclos(void) { const char *MyName = "pclos";I(MyName); if(q_len) XFillPolygon(DWGC, queue, q_len, PMode, CoordModeOrigin); F;}

#define MKP(n, p) { Int32 i; q_len = 0; for(i = 0; i < n; i++) { q_add(MyName, X(p[i][0]), Y(p[i][1]));}}

void poly2 (Int32 n, Coo p[][2]) { const char *MyName = "poly2" ; I(MyName); MKP(n, p); q_add(MyName, XS(p[0][0]), YS(p[0][1])); XDrawLines(DWGC, queue, q_len, CoordModeOrigin); F;}
void poly2i(Int32 n, Ico p[][2]) { const char *MyName = "poly2i"; I(MyName); MKP(n, p); q_add(MyName, XS(p[0][0]), YS(p[0][1])); XDrawLines(DWGC, queue, q_len, CoordModeOrigin); F;}
void poly2s(Int32 n, Sco p[][2]) { const char *MyName = "poly2s"; I(MyName); MKP(n, p); q_add(MyName, XS(p[0][0]), YS(p[0][1])); XDrawLines(DWGC, queue, q_len, CoordModeOrigin); F;}

void polf2 (Int32 n, Coo p[][2]) { const char *MyName = "polf2" ; I(MyName); MKP(n, p); SCP(=p[0][0],=p[0][1]); XFillPolygon(DWGC, queue, q_len, PMode, CoordModeOrigin); F;}
void polf2i(Int32 n, Ico p[][2]) { const char *MyName = "polf2i"; I(MyName); MKP(n, p); SCP(=p[0][0],=p[0][1]); XFillPolygon(DWGC, queue, q_len, PMode, CoordModeOrigin); F;}
void polf2s(Int32 n, Sco p[][2]) { const char *MyName = "polf2s"; I(MyName); MKP(n, p); SCP(=p[0][0],=p[0][1]); XFillPolygon(DWGC, queue, q_len, PMode, CoordModeOrigin); F;}

/* Vertex graphics */
void bgnpoint     (void) { const char *MyName = "bgnpoint";      I(MyName); W->vmode = VertexPoint; q_len = 0;}
void bgnline      (void) { const char *MyName = "bgnline";       I(MyName); W->vmode = VertexLine;  q_len = 0;}
void bgnclosedline(void) { const char *MyName = "bgnclosedline"; I(MyName); W->vmode = VertexCLine; q_len = 0;}
void bgnpolygon   (void) { const char *MyName = "bgnpolygon";    I(MyName); W->vmode = VertexPoly;  q_len = 0;}

void endpoint     (void) { const char *MyName = "endpoint"     ; if(W->vmode != VertexPoint) {Yprintf(MyName, "missing bgnpoint\n"     ); exit(1);} if(q_len) {                                        XDrawPoints (DWGC, queue, q_len,        CoordModeOrigin); F;}}
void endline      (void) { const char *MyName = "endline"      ; if(W->vmode != VertexLine ) {Yprintf(MyName, "missing bgnline\n"      ); exit(1);} if(q_len) {                                        XDrawLines  (DWGC, queue, q_len,        CoordModeOrigin); F;}}
void endclosedline(void) { const char *MyName = "endclosedline"; if(W->vmode != VertexCLine) {Yprintf(MyName, "missing bgnclosedline\n"); exit(1);} if(q_len) { q_add(MyName, queue[0].x, queue[0].y); XDrawLines  (DWGC, queue, q_len,        CoordModeOrigin); F;}}
void endpolygon   (void) { const char *MyName = "endpolygon"   ; if(W->vmode != VertexPoly ) {Yprintf(MyName, "missing bgnpolygon\n"   ); exit(1);} if(q_len) {                                        XFillPolygon(DWGC, queue, q_len, PMode, CoordModeOrigin); F;}}

void v2s(Int16   v[2]) { const char *MyName = "v2s"; if(W->vmode == VertexNone) { Yprintf(MyName, "not in vertex mode\n"); exit(1);} q_add(MyName, X(v[0]), Y(v[1])); F;}
void v2i(Int32   v[2]) { const char *MyName = "v2i"; if(W->vmode == VertexNone) { Yprintf(MyName, "not in vertex mode\n"); exit(1);} q_add(MyName, X(v[0]), Y(v[1])); F;}
void v2f(Float32 v[2]) { const char *MyName = "v2f"; if(W->vmode == VertexNone) { Yprintf(MyName, "not in vertex mode\n"); exit(1);} q_add(MyName, X(v[0]), Y(v[1])); F;}
void v2d(Float64 v[2]) { const char *MyName = "v2d"; if(W->vmode == VertexNone) { Yprintf(MyName, "not in vertex mode\n"); exit(1);} q_add(MyName, X(v[0]), Y(v[1])); F;}

/* Text */
void cmov2 (Coo x, Coo y) { W->xc = x; W->yc = y;}
void cmov2s(Sco x, Sco y) { W->xc = x; W->yc = y;}
void cmov2i(Ico x, Ico y) { W->xc = x; W->yc = y;}

void getcpos(Screencoord *x, Screencoord *y) {
  /* Note: Due to a bug in GL getcpos() returns coordinates
   * relative to the screen, not to the window. We emulate this. :-((
   */
  const char *MyName = "getcpos";
  Int32 xo, yo;
  I(MyName);
  getorigin(&xo, &yo);
  *x = xo + XR(W->xc);
  *y = yo + YR(W->yc);
#ifdef DEBUG
  fprintf(stderr, "getcpos: *x = %d, *y = %d\n", *x, *y);
#endif
}

/* Extensions: Routines from X not in gl. Contributed by MiSt (michael@hal6000.thp.Uni-Duisburg.DE) */
void arcx  (Coo x,Coo y,Coo rx,Coo ry,Angle s,Angle e) { I("arcx" ); XDrawArc(DWGC, X(x-rx), Y(y+ry), XR(2*MAX(0,rx)), YR(2*MAX(0,ry)), (64*s)/10, (64*(e-s))/10); F;}
void arcxi (Ico x,Ico y,Ico rx,Ico ry,Angle s,Angle e) { I("arcxi"); XDrawArc(DWGC, X(x-rx), Y(y+ry), XR(2*MAX(0,rx)), YR(2*MAX(0,ry)), (64*s)/10, (64*(e-s))/10); F;}
void arcxs (Sco x,Sco y,Sco rx,Sco ry,Angle s,Angle e) { I("arcxs"); XDrawArc(DWGC, X(x-rx), Y(y+ry), XR(2*MAX(0,rx)), YR(2*MAX(0,ry)), (64*s)/10, (64*(e-s))/10); F;}

void arcxf (Coo x,Coo y,Coo rx,Coo ry,Angle s,Angle e) { I("arcxf" ); XFillArc(DWGC, X(x-rx), Y(y+ry), XR(2*MAX(0,rx)), YR(2*MAX(0,ry)), (64*s)/10, (64*(e-s))/10); F;}
void arcxfi(Ico x,Ico y,Ico rx,Ico ry,Angle s,Angle e) { I("arcxfi"); XFillArc(DWGC, X(x-rx), Y(y+ry), XR(2*MAX(0,rx)), YR(2*MAX(0,ry)), (64*s)/10, (64*(e-s))/10); F;}
void arcxfs(Sco x,Sco y,Sco rx,Sco ry,Angle s,Angle e) { I("arcxfs"); XFillArc(DWGC, X(x-rx), Y(y+ry), XR(2*MAX(0,rx)), YR(2*MAX(0,ry)), (64*s)/10, (64*(e-s))/10); F;}
