/*
 *    Ygl: Run 2d-GL programs with standard X11 routines.
 *    (C) Fred Hucht 1993-96
 *    EMail: fred@thp.Uni-Duisburg.DE
 */

#include "header.h"

static Ulong YglButtonMask = 0;

static int   KeyFlag   = False;
static int   KeybdFlag = False;

static int   change_map(Device dev, int on_off);
static void  set_YglEventMask(void);
static Bool  more_redraw(Display*, XEvent*, char *);
static int   check_more_redraw(Int32 wid);
static Int32 q_next(int parent, Int16 *val);

/* XChangeActivePointerGrab only understands these: */
#define XCAPG_MASK (ButtonPressMask|ButtonReleaseMask|\
		    EnterWindowMask|LeaveWindowMask|\
		    Button1MotionMask|Button2MotionMask|\
		    Button3MotionMask|Button4MotionMask|\
		    Button5MotionMask|PointerMotionHintMask|\
		    PointerMotionMask|ButtonMotionMask|KeymapStateMask)

static int change_map(Device dev, int on_off) {
  int code, found = False;
  for(code = 0; code < KEYMAP_LEN; code++) /* several codes may return same keysym (eg MacX) */
    if(dev == (Ygl.keymap[code] & (KEYMAP_BIT-1))) { /* found */
      found = True;
      if(on_off) Ygl.keymap[code] |=  KEYMAP_BIT;
      else       Ygl.keymap[code] &= ~KEYMAP_BIT;
#ifdef DEBUG
      fprintf(stderr, "change_map: dev = %d, code = %d, new Ygl.keymap[code] = %d\n", dev, code, Ygl.keymap[code]);
#endif
    }
  return found;
}

static void set_YglEventMask(void) {
  int i;
  
  /* To change event mask even when a button is pressed: */
  XChangeActivePointerGrab(D, Ygl.EventMask & XCAPG_MASK, None, CurrentTime);
  
#ifdef DEBUG
  fprintf(stderr, "set_YglEventMask: EventMask = 0x%x, KeyFlag = %d\n",
	  Ygl.EventMask, KeyFlag);
#endif
  
  for(i = 1; i < Ygl.NextWindow; i++) {
    YglWindow *w = &Ygl.Windows[i];
    if(w->main != 0) {
      XSetWMProtocols(D, w->main, &Ygl.wm_dw, Ygl.wm_dw_flag ? 1 : 0);
      XSelectInput(D, w->main, Ygl.EventMask);
#ifdef RGBWIN
      if(w->rgb) /* Expose events don't propagate */
	XSelectInput(D, w->win,  Ygl.EventMask &  ExposureMask);
#endif
    }
  }
}

static int YglTie[3][2] = {{0,0},{0,0},{0,0}}; /* ties to left, middle, right button */

void tie(Device button, Device val1, Device val2) {
  const char * MyName = "tie";
  int tieb;
  
  switch(button) {
  case LEFTMOUSE:   tieb = 0; break;
  case MIDDLEMOUSE: tieb = 1; break;
  case RIGHTMOUSE:  tieb = 2; break;
  default:
    Yprintf(MyName, "first argument is not a button: %d.\n", button); return;
  }

  switch(val1) {
  case MOUSEX: break;
  case MOUSEY: break;
  default:
    Yprintf(MyName, "second argument is not a valuator: %d.\n", val1); return;
  }
  
  switch(val2) {
  case MOUSEX: break;
  case MOUSEY: break;
  default:
    Yprintf(MyName, "third argument is not a valuator: %d.\n", val2); return;
  }
  
  YglTie[tieb][0] = val1;
  YglTie[tieb][1] = val2;
}  

void qdevice(Device dev) {
  const char * MyName = "qdevice";
  
  I(MyName);
  switch(dev) {
  case WINCLOSE:    
    Yprintf(MyName, "device WINCLOSE not longer supported. Using WINQUIT instead...\n");
  case WINQUIT:      /* This is the event GL sends for WM_DELETE_WINDOW... */
                     Ygl.wm_dw_flag = True;              break;
  case REDRAW:       Ygl.EventMask |= RedrawMask;        break;
  case INPUTCHANGE:  Ygl.EventMask |= EnterLeaveMask;    break;
  case MOUSEX:      
  case MOUSEY:       Ygl.EventMask |= PointerMotionMask; break;
  case LEFTMOUSE:    YglButtonMask |= Button1Mask;       break;
  case MIDDLEMOUSE:  YglButtonMask |= Button2Mask;       break;
  case RIGHTMOUSE:   YglButtonMask |= Button3Mask;       break;
  case KEYBD:        KeybdFlag   = True;                 break;
    
  default:          
    if(change_map(dev, True)) {
      KeyFlag = True;
    } else {
      Yprintf(MyName, "unknown device: %d.\n", dev); return;
    }
    break;
  }
  
  if(YglButtonMask) 
    Ygl.EventMask |= ButtonPressMask|ButtonReleaseMask;
  
  if(KeybdFlag || KeyFlag)
    Ygl.EventMask |= KeyPressMask|KeyReleaseMask;
  
  set_YglEventMask();
}    

void unqdevice(Device dev) {
  int i;
  const char * MyName = "unqdevice";
  
  I(MyName);
  switch(dev) {
  case WINCLOSE:    
    Yprintf(MyName, "device WINCLOSE not longer supported. Using WINQUIT instead...\n");
  case WINQUIT:      /* This is the event GL sends for WM_DELETE_WINDOW... */
                     Ygl.wm_dw_flag = False;              break;
  case REDRAW:       Ygl.EventMask &= ~RedrawMask;        break;
  case INPUTCHANGE:  Ygl.EventMask &= ~EnterLeaveMask;    break;
  case MOUSEX:      
  case MOUSEY:       Ygl.EventMask &= ~PointerMotionMask; break;
  case LEFTMOUSE:    YglButtonMask &= ~Button1Mask;       break;
  case MIDDLEMOUSE:  YglButtonMask &= ~Button2Mask;       break;
  case RIGHTMOUSE:   YglButtonMask &= ~Button3Mask;       break;
  case KEYBD:        KeybdFlag   = False;                 break;

  default:
    if(!change_map(dev, False)) {
      Yprintf(MyName, "unknown device: %d.\n", dev); return;
    }
    break;
  }
  
  if (!YglButtonMask)
    Ygl.EventMask &= ~(ButtonPressMask|ButtonReleaseMask);

  KeyFlag = False;
  for(i = 0; i < KEYMAP_LEN; i++) KeyFlag = KeyFlag || (Ygl.keymap[i] & KEYMAP_BIT);
  
  if(!(KeybdFlag || KeyFlag))
    Ygl.EventMask &= ~(KeyPressMask|KeyReleaseMask);
  
  set_YglEventMask();
}    

static Int16 mousex = -1;
static Int16 mousey = -1;
static Int16 mousexchanged = False;
static Int16 mouseychanged = False;
static Int16 keyevent = 0;

void qreset(void) {
  const char * MyName = "qreset";
  
  I(MyName);
  XSync(D, True);
  /* Don't remove saved mouse/key events, else we may never get 
   * MOUSEY/ANYKEY events. */
  /*
    mousex = -1; mousey = -1;
    mousexchanged = False;
    mouseychanged = False;
    keyevent = 0;
    */
}

struct r_info {
  Window main;
#ifdef RGBWIN
  Window win;
#endif
  int    num;
  int    ret;
};

static Bool more_redraw(Display *dpy, XEvent *e, char *rc) {
  struct r_info *r = (struct r_info*) rc;
  int ret;
  if((e->type == Expose || e->type == ConfigureNotify) &&
     (e->xany.window == r->main IF_RGBWIN(|| e->xany.window == r->win))) {
    r->ret = True; /* found Expose or ConfigureNotify event for same window */
  }
  ret = r->ret || --r->num == 0; /* return True if found or queue empty */
#ifdef DEBUG
#ifdef RGBWIN
  fprintf(stderr, 
	  "more_redraw: type=%s, r={main=0x%x,win=0x%x,num=%d,ret=%d}, ret=%d\n",
	  Ygl.XEventNames[e->type], r->main, r->win, r->num, r->ret, ret);
#else
  fprintf(stderr, 
	  "more_redraw: type=%s, r={main=0x%x,num=%d,ret=%d}, ret=%d\n",
	  Ygl.XEventNames[e->type], r->main, r->num, r->ret, ret);
#endif
#endif
  return ret;
}

static int check_more_redraw(Int32 wid) {
  XEvent p;
  struct r_info info;
  info.main = Ygl.Windows[wid].main;
#ifdef RGBWIN
  info.win  = Ygl.Windows[wid].win;
#endif
  info.num  = XPending(D);
  info.ret  = False;
  if(info.num) {
#ifdef DEBUG
    fprintf(stderr, "check_more_redraw: Calling XPeekIfEvent()\n");
#endif
    XPeekIfEvent(D, &p, more_redraw, (char*)&info);
  }
  return info.ret;
}

#define QREAD 1
#define QTEST 2

static Int32 q_next(int parent, Int16 *val) {
  XEvent e;
  Int32 dev = -1, dev2;
  int tieb;
  char key;
  char *caller = parent == QTEST ? "qtest" : "qread";
  
  I(caller);
  
  while(dev == -1) {
    if(mousexchanged) {		/* Hack for mouse events */
      dev  = MOUSEX;
      *val = mousex;
      if(parent == QREAD) mousexchanged = False;
    } else if(mouseychanged) {	/* Hack for mouse events */
      dev  = MOUSEY;
      *val = YglScreenHeight - mousey - 1;
      if(parent == QREAD) mouseychanged = False;
    } else if(keyevent > 0) {	/* Hack for reporting both KEYBD and ANYKEY device */
      dev  = keyevent;
      *val = 1; /* Must be 1 (KeyPress) */
      if(parent == QREAD) keyevent = 0;
    } else if(parent == QTEST && XPending(D) == 0) {
      dev = 0;
    } else {
      
      if(parent == QREAD) XNextEvent(D, &e);
      else                XPeekEvent(D, &e);
      
#ifdef DEBUG
      fprintf(stderr, 
	      "%s: e.type = %s for window 0x%x, XPending returns %d\n",
	      caller, Ygl.XEventNames[e.type], e.xany.window, XPending(D));
#endif
      
      switch(e.type) {
	Int32 wid;
      case ConfigureNotify:
      case Expose:
#ifdef RGBWIN
	wid = Ygl.x2gl_wid(e.xany.window, X2GL_WIN);
	if(wid == 0) wid = Ygl.x2gl_wid(e.xany.window, X2GL_MAIN);
#else
	wid = Ygl.x2gl_wid(e.xany.window, X2GL_MAIN);
#endif
	
	if(wid == 0) Yprintf(caller, "REDRAW received for unknown window\n");
	
	if(parent == QTEST) {
	  dev = REDRAW;
	} else if(e.type == Expose && e.xexpose.count != 0) { /* ignore */
#ifdef DEBUG
	  fprintf(stderr,
		  "%s: e.xexpose.count = %d\n",
		  caller, e.xexpose.count);
#endif
	} else if(check_more_redraw(wid)) {
	  /* more Expose events for the same window in queue? */
#ifdef DEBUG
	  fprintf(stderr,
		  "More Expose or ConfigureNotify events for same window in queue, ignoring this one...\n",
		  caller);
#endif
	} else {
	  dev  = REDRAW;
	  *val = wid;
	}
	break;
	
      case EnterNotify:
	dev  = INPUTCHANGE;
	*val = Ygl.x2gl_wid(e.xany.window, X2GL_MAIN);
	break;
	
      case LeaveNotify:
	dev  = INPUTCHANGE;
	*val = 0;
	break;
	
      case MotionNotify:
#ifdef DEBUG
	fprintf(stderr, "%s: e.xmotion.x_root = %d, e.xmotion.y_root = %d\n",
		/**/ caller, e.xmotion.x_root,      e.xmotion.y_root);
#endif
	if(parent == QTEST) {
	  if(mousex != e.xmotion.x_root)
	    dev = MOUSEX;	/* if both values have changed, report x first... */
	  else
	    dev = MOUSEY;
	} else {
	  if(mousey != e.xmotion.y_root) {
	    mousey = e.xmotion.y_root;
	    mouseychanged = True;
	  }
	  if(mousex != e.xmotion.x_root) {
	    dev  = MOUSEX;
	    *val = mousex = e.xmotion.x_root;
	  } 
	}
	break;
	
      case KeyPress:
      case KeyRelease: /* Note: KeyReleases are not reported as KEYBD events */
#ifdef DEBUG
	fprintf(stderr, "%s: e.xkey.keycode = %d, Ygl.keymap[%d] = %d\n",
		caller, e.xkey.keycode, e.xkey.keycode, Ygl.keymap[e.xkey.keycode]);
#endif
	if(KeybdFlag &&
	   e.type == KeyPress &&
	   1 == XLookupString(&e.xkey, &key, 1, NULL, NULL)) {
	  /* return code */
	  dev  = KEYBD;
	  *val = (Int16)key;
	}
	if(KeyFlag &&
	   (dev2 = Ygl.keymap[e.xkey.keycode]) & KEYMAP_BIT ) {
	  /* Individual keys requested? */
	  dev2 &= (KEYMAP_BIT-1);
	  if(dev == KEYBD) {	/* first report KEYBD event */
	    keyevent = dev2;
	  } else {
	    dev  = dev2;
	    *val = e.type == KeyPress ? 1 : 0;
	  }
	}
      	break;
	
      case ButtonPress:
      case ButtonRelease:
	switch(e.xbutton.button) {
	case Button1:
	  if(YglButtonMask & Button1Mask) {
	    dev = LEFTMOUSE;
	    if(parent == QREAD) tieb = 0;
	  }
	  break;
	case Button2:
	  if(YglButtonMask & Button2Mask) {
	    dev = MIDDLEMOUSE;
	    if(parent == QREAD) tieb = 1;
	  }
	  break;
	case Button3:
	  if(YglButtonMask & Button3Mask) {
	    dev = RIGHTMOUSE;
	    if(parent == QREAD) tieb = 2;
	  }
	  break;
	default:
	  Yprintf(caller, "unknown button: %d.\n", e.xbutton.button);
	  break;
	}
	
	if(parent == QREAD && dev != -1) {
	  *val = e.type == ButtonPress ? 1 : 0;
	  
	  switch(YglTie[tieb][0]) {
	  case MOUSEX: mousexchanged = True; mousex = e.xbutton.x_root; break;
	  case MOUSEY: mouseychanged = True; mousey = e.xbutton.y_root; break;
	  }
	  switch(YglTie[tieb][1]) {
	  case MOUSEX: mousexchanged = True; mousex = e.xbutton.x_root; break;
	  case MOUSEY: mouseychanged = True; mousey = e.xbutton.y_root; break;
	  }
	}
	break;
	
      case ClientMessage: 
#ifdef DEBUG
	fprintf(stderr, "%s: e.xclient.message_type = %d\n",
		caller, e.xclient.message_type);
#endif
	dev  = WINQUIT;
	*val = Ygl.x2gl_wid(e.xany.window, X2GL_MAIN);
	break;
	
      case CirculateNotify:
      case CreateNotify:
      case DestroyNotify:
      case GravityNotify:
      case ReparentNotify:
      case MapNotify:
      case UnmapNotify:
	/* Ignore these Events... */
	break;
      case MappingNotify:
	/* We should reload Ygl.keymap[] here, but who cares... */
	break;
      default:
	Yprintf(caller,
		"unknown event (type=%d) for window %d(0x%x) received.\n",
		e.type,	Ygl.x2gl_wid(e.xany.window, X2GL_MAIN), e.xany.window);
	break;
      } /* end switch */
      
      if(dev == -1 && parent == QTEST) {
#ifdef DEBUG
	fprintf(stderr, "qtest: eating event: e.type = %d(%s)\n",
		e.type, Ygl.XEventNames[e.type]);
#endif
	XNextEvent(D, &e); /* read and ignore unreported events so they don't block the queue */
      }
      
      if(dev != -1 && parent == QREAD) Ygl.lastreadevent = e;
    }
  }
#ifdef DEBUG
  if(dev != 0) fprintf(stderr, "%s: returning dev = %d, *val = %d\n",
		       caller, dev, *val);
#endif
  return dev;
}

Int32 qtest(void) { 
  Int16 val;
  return q_next(QTEST, &val);
}

Int32 qread(Int16 *val) {
  return q_next(QREAD, val);
}

void qenter(Int16 dev, Int16 val) {
  XEvent e;
  const char * MyName = "qenter";
  
  I(MyName);
  switch(dev) {
  case REDRAW:
    e.type = Expose;
#ifdef RGBWIN /* send to .win, not .main */
    e.xexpose.window = (val > 0 && val < Ygl.NextWindow) ? Ygl.Windows[val].win : 0;
#else
    e.xexpose.window = (val > 0 && val < Ygl.NextWindow) ? Ygl.Windows[val].main : 0;
#endif
    e.xexpose.count = 0;
    break;
  case INPUTCHANGE:  Yprintf(MyName, "unsupported device: INPUTCHANGE.\n") ; return; break;
    /* e.type = EnterNotify; e.xcrossing.window = val; break; */
  case KEYBD:        Yprintf(MyName, "unsupported device: KEYBD.\n")       ; return; break;
  case MENUBUTTON:   Yprintf(MyName, "unsupported device: MENUBUTTON.\n")  ; return; break;
  default:           Yprintf(MyName, "unknown device: %d.\n", dev)         ; return; break;
  }
  XPutBackEvent(D, &e);
}
