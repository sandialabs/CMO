C     -*- fortran -*-
C     
C     Ygl: Run 2d-GL programs with standard X11 routines.
C     (C) Fred Hucht 1993-96
C     EMail: fred@thp.Uni-Duisburg.DE
C
      integer*4 BLACK
      integer*4 WHITE
      integer*4 GREEN
      integer*4 YELLOW
      integer*4 BLUE
      integer*4 MAGENT
      integer*4 CYAN
      integer*4 RED
      integer*4 BREAKK
      integer*4 SETUPK
      integer*4 LEFTCT
      integer*4 CAPSLO
      integer*4 RIGHTS
      integer*4 LEFTSH
      integer*4 ESCKEY
      integer*4 ONEKEY
      integer*4 TABKEY
      integer*4 QKEY
      integer*4 AKEY
      integer*4 SKEY
      integer*4 NOSCRL
      integer*4 TWOKEY
      integer*4 THREEK
      integer*4 WKEY
      integer*4 EKEY
      integer*4 DKEY
      integer*4 FKEY
      integer*4 ZKEY
      integer*4 XKEY
      integer*4 FOURKE
      integer*4 FIVEKE
      integer*4 RKEY
      integer*4 TKEY
      integer*4 GKEY
      integer*4 HKEY
      integer*4 CKEY
      integer*4 VKEY
      integer*4 SIXKEY
      integer*4 SEVENK
      integer*4 YKEY
      integer*4 UKEY
      integer*4 JKEY
      integer*4 KKEY
      integer*4 BKEY
      integer*4 NKEY
      integer*4 EIGHTK
      integer*4 NINEKE
      integer*4 IKEY
      integer*4 OKEY
      integer*4 LKEY
      integer*4 SEMICO
      integer*4 MKEY
      integer*4 COMMAK
      integer*4 ZEROKE
      integer*4 MINUSK
      integer*4 PKEY
      integer*4 LEFTBR
      integer*4 QUOTEK
      integer*4 RETKEY
      integer*4 PERIOD
      integer*4 VIRGUL
      integer*4 EQUALK
      integer*4 ACCENT
      integer*4 RIGHTB
      integer*4 BACKSL
      integer*4 PAD1
      integer*4 PAD0
      integer*4 LINEFE
      integer*4 BACKSP
      integer*4 DELKEY
      integer*4 PAD4
      integer*4 PAD2
      integer*4 PAD3
      integer*4 PADPER
      integer*4 PAD7
      integer*4 PAD8
      integer*4 PAD5
      integer*4 PAD6
      integer*4 PADPF2
      integer*4 PADPF1
      integer*4 LEFTAR
      integer*4 DOWNAR
      integer*4 PAD9
      integer*4 PADMIN
      integer*4 PADCOM
      integer*4 PADPF4
      integer*4 PADPF3
      integer*4 RIGHTA
      integer*4 UPARRO
      integer*4 PADENT
      integer*4 SPACEK
      integer*4 LEFTAL
      integer*4 RGHTAL
      integer*4 RIGHTC
      integer*4 F1KEY
      integer*4 F2KEY
      integer*4 F3KEY
      integer*4 F4KEY
      integer*4 F5KEY
      integer*4 F6KEY
      integer*4 F7KEY
      integer*4 F8KEY
      integer*4 F9KEY
      integer*4 F10KEY
      integer*4 F11KEY
      integer*4 F12KEY
      integer*4 PRINTS
      integer*4 SCROLL
      integer*4 PAUSEK
      integer*4 INSERT
      integer*4 HOMEKE
      integer*4 PAGEUP
      integer*4 ENDKEY
      integer*4 PAGEDO
      integer*4 NUMLOC
      integer*4 PADVIR
      integer*4 PADAST
      integer*4 PADPLU
      integer*4 LEFTMO
      integer*4 MIDDLE
      integer*4 RIGHTM
      integer*4 MENUBU
      integer*4 MOUSEX
      integer*4 MOUSEY
      integer*4 KEYBD
      integer*4 REDRAW
      integer*4 INPUTC
      integer*4 WINCLO
      integer*4 WINFRE
      integer*4 WINTHA
      integer*4 WINQUI
      integer*4 DEPTHC
      integer*4 SRCAUT
      integer*4 SRCFRO
      integer*4 SRCBAC
      integer*4 DMRGB
      integer*4 DMSING
      integer*4 DMDOUB
      integer*4 DMRGBD
      integer*4 GDXPMA
      integer*4 GDYPMA
      integer*4 PUPNON
      integer*4 PUPGRE

      integer*4   LOZERO
      parameter ( LOZERO = 0 ) 	
      integer*4   LOAND
      parameter ( LOAND = 1 ) 	
      integer*4   LOANDR
      parameter ( LOANDR = 2 ) 	
      integer*4   LOSRC
      parameter ( LOSRC = 3 ) 	
      integer*4   LOANDI
      parameter ( LOANDI = 4 ) 	
      integer*4   LODST
      parameter ( LODST = 5 ) 	
      integer*4   LOXOR
      parameter ( LOXOR = 6 ) 	
      integer*4   LOOR
      parameter ( LOOR = 7 ) 	
      integer*4   LONOR
      parameter ( LONOR = 8 ) 	
      integer*4   LOXNOR
      parameter ( LOXNOR = 9 ) 	
      integer*4   LONDST
      parameter ( LONDST = 10 ) 	
      integer*4   LOORR
      parameter ( LOORR = 11 ) 	
      integer*4   LONSRC
      parameter ( LONSRC = 12 ) 	
      integer*4   LOORI
      parameter ( LOORI = 13 ) 	
      integer*4   LONAND
      parameter ( LONAND = 14 ) 	
      integer*4   LOONE
      parameter ( LOONE = 15 ) 	
      integer*4   LOMIN
      parameter ( LOMIN = 16 ) 	
      integer*4   LOMAX
      parameter ( LOMAX = 17 ) 	
      integer*4   LOAVG
      parameter ( LOAVG = 18 ) 	
      integer*4   LODMS
      parameter ( LODMS = 19 ) 	
      integer*4   LOSMD
      parameter ( LOSMD = 20 ) 	
      integer*4   LOSUM
      parameter ( LOSUM = 21 ) 	
      
      parameter ( BLACK  = 0 )
      parameter ( WHITE  = 1 )
      parameter ( GREEN  = 2 )
      parameter ( YELLOW = 3 )
      parameter ( BLUE   = 4 )
      parameter ( MAGENT = 5 )
      parameter ( CYAN   = 6 )
      parameter ( RED    = 7 )
      parameter ( BREAKK = 1 )
      parameter ( SETUPK = 2 )
      parameter ( LEFTCT = 3 )
      parameter ( CAPSLO = 4 )
      parameter ( RIGHTS = 5 )
      parameter ( LEFTSH = 6 )
      parameter ( ESCKEY = 7 )
      parameter ( ONEKEY = 8 )
      parameter ( TABKEY = 9 )
      parameter ( QKEY   = 10 )
      parameter ( AKEY   = 11 )
      parameter ( SKEY   = 12 )
      parameter ( NOSCRL = 13 )
      parameter ( TWOKEY = 14 )
      parameter ( THREEK = 15 )
      parameter ( WKEY   = 16 )
      parameter ( EKEY   = 17 )
      parameter ( DKEY   = 18 )
      parameter ( FKEY   = 19 )
      parameter ( ZKEY   = 20 )
      parameter ( XKEY   = 21 )
      parameter ( FOURKE = 22 )
      parameter ( FIVEKE = 23 )
      parameter ( RKEY   = 24 )
      parameter ( TKEY   = 25 )
      parameter ( GKEY   = 26 )
      parameter ( HKEY   = 27 )
      parameter ( CKEY   = 28 )
      parameter ( VKEY   = 29 )
      parameter ( SIXKEY = 30 )
      parameter ( SEVENK = 31 )
      parameter ( YKEY   = 32 )
      parameter ( UKEY   = 33 )
      parameter ( JKEY   = 34 )
      parameter ( KKEY   = 35 )
      parameter ( BKEY   = 36 )
      parameter ( NKEY   = 37 )
      parameter ( EIGHTK = 38 )
      parameter ( NINEKE = 39 )
      parameter ( IKEY   = 40 )
      parameter ( OKEY   = 41 )
      parameter ( LKEY   = 42 )
      parameter ( SEMICO = 43 )
      parameter ( MKEY   = 44 )
      parameter ( COMMAK = 45 )
      parameter ( ZEROKE = 46 )
      parameter ( MINUSK = 47 )
      parameter ( PKEY   = 48 )
      parameter ( LEFTBR = 49 )
      parameter ( QUOTEK = 50 )
      parameter ( RETKEY = 51 )
      parameter ( PERIOD = 52 )
      parameter ( VIRGUL = 53 )
      parameter ( EQUALK = 54 )
      parameter ( ACCENT = 55 )
      parameter ( RIGHTB = 56 )
      parameter ( BACKSL = 57 )
      parameter ( PAD1   = 58 )
      parameter ( PAD0   = 59 )
      parameter ( LINEFE = 60 )
      parameter ( BACKSP = 61 )
      parameter ( DELKEY = 62 )
      parameter ( PAD4   = 63 )
      parameter ( PAD2   = 64 )
      parameter ( PAD3   = 65 )
      parameter ( PADPER = 66 )
      parameter ( PAD7   = 67 )
      parameter ( PAD8   = 68 )
      parameter ( PAD5   = 69 )
      parameter ( PAD6   = 70 )
      parameter ( PADPF2 = 71 )
      parameter ( PADPF1 = 72 )
      parameter ( LEFTAR = 73 )
      parameter ( DOWNAR = 74 )
      parameter ( PAD9   = 75 )
      parameter ( PADMIN = 76 )
      parameter ( PADCOM = 77 )
      parameter ( PADPF4 = 78 )
      parameter ( PADPF3 = 79 )
      parameter ( RIGHTA = 80 )
      parameter ( UPARRO = 81 )
      parameter ( PADENT = 82 )
      parameter ( SPACEK = 83 )
      parameter ( LEFTAL = 143 )
      parameter ( RGHTAL = 144 )
      parameter ( RIGHTC = 145 )
      parameter ( F1KEY  = 146 )
      parameter ( F2KEY  = 147 )
      parameter ( F3KEY  = 148 )
      parameter ( F4KEY  = 149 )
      parameter ( F5KEY  = 150 )
      parameter ( F6KEY  = 151 )
      parameter ( F7KEY  = 152 )
      parameter ( F8KEY  = 153 )
      parameter ( F9KEY  = 154 )
      parameter ( F10KEY = 155 )
      parameter ( F11KEY = 156 )
      parameter ( F12KEY = 157 )
      parameter ( PRINTS = 158 )
      parameter ( SCROLL = 159 )
      parameter ( PAUSEK = 160 )
      parameter ( INSERT = 161 )
      parameter ( HOMEKE = 162 )
      parameter ( PAGEUP = 163 )
      parameter ( ENDKEY = 164 )
      parameter ( PAGEDO = 165 )
      parameter ( NUMLOC = 166 )
      parameter ( PADVIR = 167 )
      parameter ( PADAST = 168 )
      parameter ( PADPLU = 169 )
      parameter ( LEFTMO = 103 )
      parameter ( MIDDLE = 102 )
      parameter ( RIGHTM = 101 )
      parameter ( MENUBU = 101 )
      parameter ( MOUSEX = 266 )
      parameter ( MOUSEY = 267 )
      parameter ( KEYBD  = 513 )
      parameter ( REDRAW = 528 )
      parameter ( INPUTC = 534 )
      parameter ( WINCLO = 537 )
      parameter ( WINFRE = 539 )
      parameter ( WINTHA = 540 )
      parameter ( WINQUI = 542 )
      parameter ( DEPTHC = 543 )
      parameter ( SRCAUT = 0 )
      parameter ( SRCFRO = 1 )
      parameter ( SRCBAC = 2 )
      parameter ( DMRGB  = 0 )
      parameter ( DMSING = 1 )
      parameter ( DMDOUB = 2 )
      parameter ( DMRGBD = 5 )
      parameter ( GDXPMA = 1 )
      parameter ( GDYPMA = 2 )
      parameter ( PUPNON = 0 )
      parameter ( PUPGRE = 1 )

      integer*4 getbut
      integer*4 getcol
      integer*4 getdes
      integer*4 getdis
      integer*4 getfon
      integer*4 getgde
      integer*4 gethei
      integer*4 getlwi
      integer*4 getpla
      integer*4 getval
      integer*4 gversi
      integer*4 crectr
      integer*4 rectre
      integer*4 lrectr
      integer*4 readpi
      integer*4 readrg
      integer*4 qread 
      integer*4 qtest 
      integer*4 strwid
      integer*4 winget
      integer*4 winope
      integer*4 swinop
C     getXdpy and getXwid are >6 chars long
      integer*4 getxdpy
      integer*4 getxwid
      integer*4 getxdid
      integer*4 getxgc
      integer*4 winx
      integer*4 gl2ppm
      integer*4 newpup
      integer*4 dopup
 
      external getbut
      external getcol
      external getdes
      external getdis
      external getfon
      external getgde
      external gethei
      external getlwi
      external getpla
      external getval
      external gversi
      external crectr
      external rectre
      external lrectr
      external readpi
      external readrg
      external qread 
      external qtest 
      external strwid
      external winget 
      external winope
      external swinop
      external getxdpy
      external getxwid
      external getxdid
      external getxgc
      external winx
      external gl2ppm
      external newpup
      external dopup
      
