#include "Rect.h"

Rect::Rect(Color* clr, int w, int h)
{
   color = clr;
   width = w;
   height = h;
}

//Rect is not the owner of color, so shouldn't delete color
Rect::~Rect()
{
}

void Rect::draw(Cairo::RefPtr<Cairo::Context> cr, int x, int y)
{
   //draw a Rectangle centered on (x, y)
   int w_2 = width/2;
   int h_2 = height/2;

   cr->save();

      cr->rectangle(x - w_2, y - h_2, width, height); 
      cr->set_source_rgba(color->getRed(), color->getGreen(), color->getBlue(), 1.0);    //opaque
      cr->fill();

   cr->restore();  
}

