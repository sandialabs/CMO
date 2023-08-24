/*
 *    Ygl: Run 2d-GL programs with standard X11 routines.
 *    (C) Fred Hucht 1993-96
 *    EMail: fred@thp.Uni-Duisburg.DE
 */

#define AUTOFLUSH	/* define to generate code for flushing the
			   server via timer */ 

#define COVERSLEEP	/* define to cover the system commands sleep
			   and usleep with own versions that flushes
			   Xlibs output buffer */ 

/*#define RGBWIN*/

#ifdef RGBWIN
# define IF_RGBWIN(x) x
#else
# define IF_RGBWIN(x)
#endif
