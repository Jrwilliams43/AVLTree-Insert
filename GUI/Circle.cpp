#include "Circle.h"

Circle::Circle(Color* clr, int r, CSC2110::String* t)
{
   color = clr;
   radius = r;
   text = t;
}

Circle::~Circle()
{
   delete color;
   //delete text;
}

void Circle::draw(Cairo::RefPtr<Cairo::Context> cr, int x, int y)
{
   const char* text_ch = text->getText();

   cr->save();
      cr->arc(x, y, radius, 0.0, PI2); //full circle
      cr->set_source_rgba(color->getRed(), color->getGreen(), color->getBlue(), 1.0);    //opaque
      cr->fill();  //draw the interior of the circle

      //draw the exterior outline of the circle
      cr->arc(x, y, radius, 0.0, PI2); 
      cr->set_source_rgba(0, 0, 0, 1.0);    //black outline
      cr->set_line_width(5);  //outline thickness 
      cr->stroke();  //outline it

      //the text to be displayed in the circle
      Glib::RefPtr<Pango::Layout> pangoLayout = Pango::Layout::create (cr);
      cr->move_to(x - 10, y - 12);
      Pango::FontDescription verdana_9 = Pango::FontDescription("verdana 9"); 
      pangoLayout->set_font_description(verdana_9);
      pangoLayout->set_text(text_ch);
      pangoLayout->update_from_cairo_context(cr); //gets cairo cursor position
      pangoLayout->add_to_cairo_context(cr); //adds text to cairos stack of stuff to be drawn
      cr->set_source_rgba(0, 0, 0, 1.0); //opaque, black
      cr->fill();

   cr->restore();
}
