/*
 *    Ygl: Run 2d-GL programs with standard X11 routines.
 *    (C) Fred Hucht 1993-96
 *    EMail: fred@thp.Uni-Duisburg.DE
 */

#ifndef	_YGL_INCLUDED_
#define _YGL_INCLUDED_

#include <sys/types.h>

#ifdef _AUX_SOURCE
# include <X11/Yglprefix.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
  
#define BLACK			0
#define WHITE			1
#define GREEN			2
#define YELLOW			3
#define BLUE			4
#define MAGENTA			5
#define CYAN			6
#define RED			7
  
  /* for queue */
#define BREAKKEY		1	
#define SETUPKEY		2	
#define LEFTCTRLKEY		3	
#define CAPSLOCKKEY		4	
#define RIGHTSHIFTKEY		5	
#define LEFTSHIFTKEY		6	
#define ESCKEY			7	
#define ONEKEY			8	
#define TABKEY			9	
#define QKEY			10	
#define AKEY			11	
#define SKEY			12	
#define NOSCRLKEY		13	
#define TWOKEY			14	
#define THREEKEY		15	
#define WKEY			16	
#define EKEY			17	
#define DKEY			18	
#define FKEY			19	
#define ZKEY			20	
#define XKEY			21	
#define FOURKEY			22	
#define FIVEKEY			23	
#define RKEY			24	
#define TKEY			25	
#define GKEY			26	
#define HKEY			27	
#define CKEY			28	
#define VKEY			29	
#define SIXKEY			30	
#define SEVENKEY		31	
#define YKEY			32	
#define UKEY			33	
#define JKEY			34	
#define KKEY			35	
#define BKEY			36	
#define NKEY			37	
#define EIGHTKEY		38	
#define NINEKEY			39	
#define IKEY			40	
#define OKEY			41	
#define LKEY			42	
#define SEMICOLONKEY		43	
#define MKEY			44	
#define COMMAKEY		45	
#define ZEROKEY			46	
#define MINUSKEY		47	
#define PKEY			48	
#define LEFTBRACKETKEY		49	
#define QUOTEKEY		50	
#define RETKEY			51	
#define PERIODKEY		52	
#define VIRGULEKEY		53	
#define EQUALKEY		54	
#define ACCENTGRAVEKEY		55	
#define RIGHTBRACKETKEY		56	
#define BACKSLASHKEY		57	
#define PAD1			58	
#define PAD0			59	
#define LINEFEEDKEY		60	
#define BACKSPACEKEY		61	
#define DELKEY			62	
#define PAD4			63	
#define PAD2			64	
#define PAD3			65	
#define PADPERIOD		66	
#define PAD7			67	
#define PAD8			68	
#define PAD5			69	
#define PAD6			70	
#define PADPF2			71	
#define PADPF1			72	
#define LEFTARROWKEY		73	
#define DOWNARROWKEY		74	
#define PAD9			75	
#define PADMINUS		76	
#define PADCOMMA		77	
#define PADPF4			78	
#define PADPF3			79	
#define RIGHTARROWKEY		80	
#define UPARROWKEY		81	
#define PADENTER		82	
#define SPACEKEY		83	
#define LEFTALTKEY	 	143
#define	RIGHTALTKEY	 	144
#define	RIGHTCTRLKEY	 	145
#define	F1KEY	 		146
#define	F2KEY	 		147
#define	F3KEY	 		148
#define	F4KEY	 		149
#define	F5KEY	 		150
#define	F6KEY	 		151
#define	F7KEY	 		152
#define	F8KEY	 		153
#define	F9KEY	 		154
#define	F10KEY			155
#define	F11KEY			156
#define	F12KEY			157
#define	PRINTSCREENKEY		158
#define	SCROLLLOCKKEY		159
#define	PAUSEKEY		160
#define	INSERTKEY		161
#define	HOMEKEY			162
#define	PAGEUPKEY	 	163
#define	ENDKEY			164
#define	PAGEDOWNKEY		165
#define	NUMLOCKKEY		166
#define	PADVIRGULEKEY	 	167
#define PADASTERKEY	 	168
#define PADPLUSKEY	 	169
  
#define LEFTMOUSE		103
#define MIDDLEMOUSE		102
#define RIGHTMOUSE		101
#define MENUBUTTON		101
  
#define MOUSEX			266
#define MOUSEY			267
  
#define KEYBD			513
#define REDRAW			528
#define INPUTCHANGE		534
#define WINCLOSE		537
#define WINFREEZE		539
#define WINTHAW			540
#define WINQUIT			542
#define DEPTHCHANGE		543
  
  /* for readsource(): */
#define SRC_AUTO		0
#define SRC_FRONT		1
#define SRC_BACK		2
  
  /* for getdisplaymode(): */
#define DMRGB			0L
#define DMSINGLE		1L
#define DMDOUBLE		2L
#define DMRGBDOUBLE		5L
  
  /* for getgdesc(): */
#define GD_XPMAX 		1L
#define GD_YPMAX 		2L
  
  /* for setpup() */
#define PUP_NONE		0
#define PUP_GREY		1
  
  /* for logicop() */
#define LO_ZERO			0x0
#define LO_AND			0x1
#define LO_ANDR			0x2
#define LO_SRC			0x3
#define LO_ANDI			0x4
#define LO_DST			0x5
#define LO_XOR			0x6
#define LO_OR			0x7
#define LO_NOR			0x8
#define LO_XNOR			0x9
#define LO_NDST			0xa
#define LO_ORR			0xb
#define LO_NSRC			0xc
#define LO_ORI			0xd
#define LO_NAND			0xe
#define LO_ONE			0xf
#define LO_MIN			0x10
#define LO_MAX			0x11
#define LO_AVG			0x12
#define LO_DMS			0x13
#define LO_SMD			0x14
#define LO_SUM			0x15

  /* Types */
  typedef char			Int8;
  typedef unsigned char		Uint8;
  typedef short			Int16;
  typedef unsigned short	Uint16;
  typedef long			Int32;
  typedef unsigned long		Uint32;
  typedef float			Float32;
  typedef double		Float64;
  typedef char			Char8;
  typedef char			Void;
  
  typedef Uint8			Byte;
  typedef Uint8			RGBvalue;
  
  typedef Uint16		Colorindex;
  typedef Uint16		Device;
  
  typedef Int16			Angle;
  typedef Int16			Scoord;
  typedef Int16			Screencoord;
  
  typedef Int32			Icoord;
  
  typedef Float32		Coord;
  
  typedef Float32		Matrix[4][4];
  
  /********************* draw.c */
  extern void  clear		( void );
  
  /* Points */
  extern void  pnt2 		(  Coord,  Coord );
  extern void  pnt2i		( Icoord, Icoord );
  extern void  pnt2s		( Scoord, Scoord );
  
  /* Lines */
  extern void  move2 		(  Coord,  Coord );
  extern void  move2i		( Icoord, Icoord );
  extern void  move2s		( Scoord, Scoord );
  
  extern void  rmv2 		(  Coord,  Coord );
  extern void  rmv2i		( Icoord, Icoord );
  extern void  rmv2s		( Scoord, Scoord );
  
  extern void  draw2 		(  Coord,  Coord );
  extern void  draw2i		( Icoord, Icoord );
  extern void  draw2s		( Scoord, Scoord );
  
  extern void  rdr2 		(  Coord,  Coord );
  extern void  rdr2i		( Icoord, Icoord );
  extern void  rdr2s		( Scoord, Scoord );
  
  /* Arcs & Circles */
  extern void  arc 		(  Coord,  Coord,  Coord, Angle, Angle );
  extern void  arci		( Icoord, Icoord, Icoord, Angle, Angle );
  extern void  arcs		( Scoord, Scoord, Scoord, Angle, Angle );
  
  extern void  arcf 		(  Coord,  Coord,  Coord, Angle, Angle );
  extern void  arcfi		( Icoord, Icoord, Icoord, Angle, Angle );
  extern void  arcfs		( Scoord, Scoord, Scoord, Angle, Angle );
  
  extern void  circ 		(  Coord,  Coord,  Coord );
  extern void  circi		( Icoord, Icoord, Icoord );
  extern void  circs		( Scoord, Scoord, Scoord );
  
  extern void  circf 		(  Coord,  Coord,  Coord );
  extern void  circfi		( Icoord, Icoord, Icoord );
  extern void  circfs		( Scoord, Scoord, Scoord );
  
  /* Rects & Boxes */
  extern void  rect 		(  Coord,  Coord,  Coord,  Coord );
  extern void  recti		( Icoord, Icoord, Icoord, Icoord );
  extern void  rects		( Scoord, Scoord, Scoord, Scoord );
  
  extern void  rectf 		(  Coord,  Coord,  Coord,  Coord );
  extern void  rectfi		( Icoord, Icoord, Icoord, Icoord );
  extern void  rectfs		( Scoord, Scoord, Scoord, Scoord );
  
  extern void  sbox 		(  Coord,  Coord,  Coord,  Coord );
  extern void  sboxi		( Icoord, Icoord, Icoord, Icoord );
  extern void  sboxs		( Scoord, Scoord, Scoord, Scoord );
  
  extern void  sboxf 		(  Coord,  Coord,  Coord,  Coord );
  extern void  sboxfi		( Icoord, Icoord, Icoord, Icoord );
  extern void  sboxfs		( Scoord, Scoord, Scoord, Scoord );
  
  /* Filled Polygons */
  extern void  concave		( Int32 );
  
  extern void  pmv2 		(  Coord,  Coord );
  extern void  pmv2i		( Icoord, Icoord );
  extern void  pmv2s		( Scoord, Scoord );
  
  extern void  rpmv2 		(  Coord,  Coord );
  extern void  rpmv2i		( Icoord, Icoord );
  extern void  rpmv2s		( Scoord, Scoord );
  
  extern void  pdr2 		(  Coord,  Coord );
  extern void  pdr2i		( Icoord, Icoord );
  extern void  pdr2s		( Scoord, Scoord );
  
  extern void  rpdr2 		(  Coord,  Coord );
  extern void  rpdr2i		( Icoord, Icoord );
  extern void  rpdr2s		( Scoord, Scoord );
  
  extern void  pclos		( void );
  
  extern void  poly2 		( Int32,  Coord[][2] );
  extern void  poly2i		( Int32, Icoord[][2] );
  extern void  poly2s		( Int32, Scoord[][2] );
  
  extern void  polf2 		( Int32,  Coord[][2] );
  extern void  polf2i		( Int32, Icoord[][2] );
  extern void  polf2s		( Int32, Scoord[][2] );
  
  /* Vertex graphics */
  extern void  bgnpoint		( void );
  extern void  bgnline		( void );
  extern void  bgnclosedline	( void );
  extern void  bgnpolygon	( void );
  
  extern void  endpoint		( void );
  extern void  endline		( void );
  extern void  endclosedline	( void );
  extern void  endpolygon	( void );
  
  extern void  v2s		( Int16[2] );
  extern void  v2i		( Int32[2] );
  extern void  v2f		( Float32[2] );
  extern void  v2d		( Float64[2] );
  
  /* Text */
  extern void  cmov2 		(  Coord,  Coord );
  extern void  cmov2i		( Icoord, Icoord );
  extern void  cmov2s		( Scoord, Scoord );
  
  extern void  getcpos		( Screencoord *, Screencoord * );
  
  /* Extensions: Routines not in gl by MiSt (michael@hal6000.thp.Uni-Duisburg.DE) */
  extern void  arcx 		(  Coord,  Coord,  Coord,  Coord, Angle, Angle );
  extern void  arcxi		( Icoord, Icoord, Icoord, Icoord, Angle, Angle );
  extern void  arcxs		( Scoord, Scoord, Scoord, Scoord, Angle, Angle );
  
  extern void  arcxf 		(  Coord,  Coord,  Coord,  Coord, Angle, Angle );
  extern void  arcxfi		( Icoord, Icoord, Icoord, Icoord, Angle, Angle );
  extern void  arcxfs		( Scoord, Scoord, Scoord, Scoord, Angle, Angle );
  
  /********************* queue.c */
  extern void  tie		( Device, Device, Device );
  extern void  qdevice		( Device );
  extern void  unqdevice	( Device );
  extern void  qreset		( void );
  extern Int32 qtest		( void );
  extern Int32 qread		( Int16 * );
  extern void  qenter		( Int16, Int16 );

  /********************* misc.c */
  extern void  singlebuffer 	( void );
  extern void  doublebuffer 	( void );
  extern void  swapbuffers 	( void );
  extern void  frontbuffer	( Int32 );
  extern void  backbuffer	( Int32 );
  
  extern void  gflush		( void );
  
#ifdef _XLIB_H_ 		/* Declare if <X11/Xlib.h> is included */
  extern Display *getXdpy 	( void );
  extern Window   getXwid 	( void );
  extern Window   getXdid 	( void );
  extern GC       getXgc 	( void );
#endif /* _XLIB_H_ */
  
  extern void  wintitle		( Char8 * );
  extern void  winset		( Int32 );
  extern Int32 winget		( void );
  extern Int32 getplanes	( void );
  extern Int32 getvaluator 	( Device );
  extern Int32 getbutton	( Device );
  extern Int32 gversion		( Char8[12] );
  
  extern void  ortho2		( Coord, Coord, Coord, Coord );
  extern void  viewport		( Screencoord left, Screencoord right, Screencoord bottom, Screencoord top );
  extern void  getviewport	( Screencoord*left, Screencoord*right, Screencoord*bottom, Screencoord*top );
  extern void  reshapeviewport 	( void );
  
  extern void  winpop		( void );
  extern void  winpush		( void );
  
  extern void  linewidth	( Int16 );
  extern Int32 getlwidth	( void );
  extern Int32 getdisplaymode	( void );
  
  extern void  setbell		( Char8 );
  extern void  ringbell		( void );
  
  extern Int32 getgdesc		( Int32 );
  
  extern void  foreground	( void );
  
  extern void  logicop		( Int32 );  
  
  extern void  getmatrix	( Matrix );
  
  /********************* font.c */
  extern void  loadXfont	( Int32 , Char8 * );
  extern void  font		( Int16 );
  extern Int32 getfont		( void );
  extern void  getfontencoding 	( Char8 * );
  extern Int32 getheight		( void );
  extern Int32 getdescender 	( void );
  extern Int32 strwidth		( Char8 * );
  extern void  charstr		( Char8 * );
  
  /********************* color.c */
  extern void  mapcolor		( Colorindex, Int16, Int16, Int16 );
  extern void  RGBcolor		( Int16, Int16, Int16 );
  extern void  cpack		( Uint32 );
  
  extern void  c3s		( Int16[3] );
  extern void  c3i		( Int32[3] );
  extern void  c3f		( Float32[3] );
  
  extern Int32 getcolor		( void );
  extern void  getmcolor	( Colorindex, Int16 *, Int16 *, Int16 * );
  extern void  getmcolors	( Colorindex, Colorindex, Int16 *, Int16 *, Int16 * );
  extern void  gRGBcolor	( Int16 *, Int16 *, Int16 * );
  
  extern void  color		( Colorindex );
  extern void  readsource	( Int32 );
  
  extern Int32 crectread	( Screencoord, Screencoord, Screencoord, Screencoord, Uint8 * );
  extern Int32 rectread		( Screencoord, Screencoord, Screencoord, Screencoord, Int16 * );
  extern Int32 lrectread	( Screencoord, Screencoord, Screencoord, Screencoord, Int32 * );
  
  extern void  crectwrite	( Screencoord, Screencoord, Screencoord, Screencoord, Uint8 * );
  extern void  rectwrite	( Screencoord, Screencoord, Screencoord, Screencoord, Int16 * );
  extern void  lrectwrite	( Screencoord, Screencoord, Screencoord, Screencoord, Int32 * );
  
  extern void  rectcopy		( Screencoord, Screencoord, Screencoord, Screencoord, Screencoord, Screencoord );
  
  extern Int32 readpixels	( Int16, Colorindex[] );
  extern void  writepixels	( Int16, Colorindex[] );
  extern Int32 readRGB		( Int16, RGBvalue[], RGBvalue[], RGBvalue[] );
  extern void  writeRGB		( Int16, RGBvalue[], RGBvalue[], RGBvalue[] );
  
  /********************* menu.c */
  extern void  addtopup		( Int32, Char8 *, ... );
  extern Int32 defpup		( Char8 *, ... );
  extern Int32 dopup		( Int32 );
  extern void  freepup		( Int32 );
  extern Int32 newpup		( void );
  extern void  setpup		( Int32, Int32, Int32 );
  
  /********************* ygl.c */
  /* Contraints */
  extern void  minsize		( Int32, Int32 );
  extern void  maxsize		( Int32, Int32 );
  extern void  prefsize		( Int32, Int32 );
  extern void  prefposition 	( Int32, Int32, Int32, Int32 );
  extern void  stepunit		( Int32, Int32 );
  extern void  keepaspect	( Int32, Int32 );
  extern void  noport		( void );
  extern void  noborder		( void );
  
  extern void  ginit		( void );
  extern void  winconstraints 	( void );
  extern Int32 winopen		( Char8 * );
  extern Int32 swinopen		( Int32 );
  
  extern void  winposition 	( Int32, Int32, Int32, Int32 );
  extern void  winmove		( Int32, Int32 );
  
  extern void  getsize		( Int32 *, Int32 * );
  extern void  getorigin	( Int32 *, Int32 * );
  
  extern void  RGBmode		( void );
  extern void  cmode		( void );
  
  extern void  gconfig		( void );
  extern void  winclose		( Int32 );
  extern void  gexit		( void );
  
#ifdef _XLIB_H_ 		/* Declare if <X11/Xlib.h> is included */
  extern Int32    winX 		( Display *, Window );
#endif /* _XLIB_H_ */
  
  /* gl2ppm.c */
  extern int   gl2ppm		( const char * );
  
#ifdef __cplusplus
}
#endif

#endif /* _YGL_INCLUDED_ */
