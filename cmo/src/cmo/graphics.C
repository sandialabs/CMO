#ifndef __cplusplus
#define __cplusplus
#endif


#include "X11/Ygl.h"    
#include "graphics.H"

using namespace std;

 
 NKGraphicPanel::NKGraphicPanel()
 {
 };

 NKGraphicPanel::NKGraphicPanel(int sizeX, int sizeY, double rangeX, double rangeY) 
 {
  _sizeX= sizeX;
  _sizeY= sizeY;
  _rangeX = rangeX;
  _rangeY = rangeY;

 };

 NKGraphicPanel::~NKGraphicPanel() {};


 double NKGraphicPanel::NKGPGetXRange()
 {
  return _rangeX;
 }

 double NKGraphicPanel::NKGPGetYRange()
 {
  return _rangeY;
 }

 void NKGraphicPanel::NKGPOpenMainWindow(char * title)
 {
   Int32 win; 
   minsize(_sizeX,_sizeY);
   win = winopen(title);
   ortho2(0.0,_rangeX,0.0,_rangeY);
   loadXfont(2,"-*-times-medium-r-*-*-*-140-*-*-*-*-iso8859-1"); 
   font(2);
   color(BLACK);
   clear();
   sleep(0);
 }   

 
 
 
 void NKGraphicPanel::NKGPPrint(int x,int y,int c, char * text)
 {

   cmov2i(x,y);
   color(c);
   charstr(text);
 } 
 
 



 void NKGraphicPanel::NKGPUpdateGraphics()
 {
   Int32 dev;
   Int16 val;
   
   while (qtest()) {
      dev = qread(&val);
      if (dev=REDRAW) {
         color(BLACK);
         clear();
         sleep(0);
      }   
   }      
 }


 void NKGraphicPanel::NKGPFilledRectangle(int x1,int y1, int x2, int y2, int c) {
   color(c);
   sboxf(x1,y1,x2,y2);

 }  


 void NKGraphicPanel::NKGPCircle(int x, int y, int r, int c) {

   color (c);
   circf(x, y, r);
 }


 void NKGraphicPanel::NKGPArc(int x, int y, int r, int startAngle, int endAngle, int c)
 {
  color(c);
  arc(x,y,r,startAngle,endAngle);
 }


 void NKGraphicPanel::NKGPLine(int x1,int y1,int x2,int y2,int c) {

 Int16 vector[2];
 
 color(c);
 bgnline();
    vector[0]=x1;
    vector[1]=y1;
    v2s(vector);
    
    vector[0]=x2;
    vector[1]=y2;
    v2s(vector);
 endline();

 }

 int NKGraphicPanel::NKGPRedraw()
 {
   Int32 dev;
   Int16 val;
   int ret=0;
   
   while (qtest()) {
      dev= qread(&val);
      ret=ret || (dev==REDRAW);
   }
   return(ret);
 }    

 void NKGraphicPanel::NKGPClearWindow(){

   color(BLACK);
   clear();
   sleep(0);

 }  

 void NKGraphicPanel::NKGPRefresh(){
   sleep(0);      
 }   






 
