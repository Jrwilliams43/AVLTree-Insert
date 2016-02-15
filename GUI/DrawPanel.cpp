#include "DrawPanel.h"
#include "Color.h"
#include "Rect.h"

//#include <gdk/gdkkeysyms.h>

#include <iostream>
using namespace std;

DrawPanel::DrawPanel(int w, int h, Drawable* d)
{
   drawable = d;
   width = w;
   height = h;

   add_events(Gdk::BUTTON_PRESS_MASK);
   this->signal_button_press_event().connect(sigc::mem_fun(*this, &DrawPanel::on_button_press_event));
}

DrawPanel::~DrawPanel()
{
   delete drawable;
}

bool DrawPanel::on_expose_event(GdkEventExpose* event)
{
   render();
   return true;
}

bool DrawPanel::on_button_press_event(GdkEventButton* event)
{
   GdkEventType type = event->type;
   if (type == GDK_2BUTTON_PRESS || type == GDK_3BUTTON_PRESS) return false;  //ignore double click or triple click events

   int x = (int) event->x;
   int y = (int) event->y;

   drawable->mouseClicked(x, y);
   render();  //update screen after the mouse click has been processed

   return true;
}

void DrawPanel::drawBackground(Cairo::RefPtr<Cairo::Context> cr)
{
   Color white(1.0, 1.0, 1.0);
   Rect rect(&white, width, height);  
   rect.draw(cr, width/2, height/2);
}

void DrawPanel::render()
{
   Glib::RefPtr<Gdk::Window> w = get_window();
   const Gdk::Rectangle rect = Gdk::Rectangle(0, 0, width, height);
   const Gdk::Region region = Gdk::Region(rect);

   Glib::RefPtr<Gdk::GC> gc = get_style()->get_black_gc();

w->begin_paint_region(region);  //double buffering!!
   Cairo::RefPtr<Cairo::Context> cr = w->create_cairo_context();

   drawBackground(cr);
   drawable->draw(w, gc, cr, width, height);
w->end_paint();
}
