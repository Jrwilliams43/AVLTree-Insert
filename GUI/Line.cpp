#include "Line.h"

Line::Line(Color* clr, double l_w)
{
   color = clr;
   line_width = l_w;
}

Line::~Line()
{
   delete color;
}

void Line::draw(Cairo::RefPtr<Cairo::Context> cr, int x1, int y1, int x2, int y2)
{
   cr->save();
      cr->set_line_width(line_width);
      cr->set_source_rgba(color->getRed(), color->getGreen(), color->getBlue(), 1.0);
      cr->move_to(x1, y1);
      cr->line_to(x2, y2);
      cr->stroke();
   cr->restore(); 
}

