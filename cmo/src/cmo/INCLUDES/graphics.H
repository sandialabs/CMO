#include "X11/Ygl.h"
#ifndef _graphics
#define _graphics 1
#endif



class NKGraphicPanel {

private:

 int _sizeX;
 int _sizeY;
 double _rangeX;
 double _rangeY;

public:

 NKGraphicPanel();

 NKGraphicPanel(int sizeX, int sizeY, double rangeX, double rangeY);

 ~NKGraphicPanel();

 void NKGPOpenMainWindow(char * title);

 void NKGPPrint(int x,int y,int c, char * text);
 
 void NKGPUpdateGraphics();

 void NKGPFilledRectangle(int x1,int y1, int x2, int y2, int c);

 void NKGPCircle(int x, int y, int r, int c);

 void NKGPArc(int x, int y, int r, int startAngle, int endAngle, int c);

 void NKGPLine(int x1,int y1,int x2,int y2,int c);

 int NKGPRedraw();

 void NKGPClearWindow();

 void NKGPRefresh();

 double NKGPGetXRange();
  
 double NKGPGetYRange();



};
  
