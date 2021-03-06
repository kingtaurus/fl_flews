#ifndef FLTK_1_0_COMPAT
#define FLTK_1_0_COMPAT
#endif

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <stdio.h>
#include <stdlib.h>
#include "FL/Fl_Value_Input_Spin.H"
#include <FL/fl_draw.H>


static char hack_o_rama;

void Fl_Value_Input_Spin::input_cb(Fl_Widget*, void* v) {
  Fl_Value_Input_Spin& t = *(Fl_Value_Input_Spin*)v;
  double nv;
  if (t.step()>=1.0) nv = strtol(t.input.value(), nullptr, 0);
  else nv = strtod(t.input.value(), nullptr);
  hack_o_rama = 1;
  t.handle_push();
  t.handle_drag(nv);
  t.handle_release();
  hack_o_rama = 0;
}

void Fl_Value_Input_Spin::draw() {

  int sxx = x(), syy = y(), sww = w(), shh = h(); 
  sxx += sww - buttonssize(); sww = buttonssize();
  Fl_Boxtype box1 = (Fl_Boxtype)(box()&-2);
  int border_size=Fl::box_dx(box());

  if (damage()&~FL_DAMAGE_CHILD) input.clear_damage(FL_DAMAGE_ALL);
  input.box(box());
  input.color(FL_WHITE, selection_color());
  input.doDraw();
  input.clear_damage();
  sxx+=border_size;
  syy+=border_size;
  sww-=border_size*2;  
  shh-=border_size*2;

  if (!box1) box1 = (Fl_Boxtype)(box()&-2);

  if((indrag || mouseobj) && deltadir!=0) {
     if(deltadir>0) {
       draw_box(down(box1),sxx,syy,sww,shh/2,color());
       draw_box(box1,sxx,syy+shh/2,sww,shh/2,color());
     } else {
       draw_box(box1,sxx,syy,sww,shh/2,color());
       draw_box(down(box1),sxx,syy+shh/2,sww,shh/2,color());
     }
  } else {
   draw_box(box1,sxx,syy,sww,shh/2,color());
   draw_box(box1,sxx,syy+shh/2,sww,shh/2,color());
  }
  sxx+=border_size;
  syy+=border_size;
  sww-=border_size*2;  
  shh-=border_size*2;
  if (active_r()) {
        fl_color(labelcolor());  
  } else {
        fl_color(labelcolor() | 8);
  }    
  int w1 = (sww-1)|1; // use odd sizes only
  int X = sxx+w1/2;
  int W = w1/3;
  int h1 = shh/2-border_size-2;
  int Y= syy;
  fl_polygon(X, Y, X+W,Y+h1 , X-W, Y+h1);
  Y=syy+shh/2+border_size+1+h1; 
  fl_polygon(X, Y, X-W, Y-h1, X+W, Y-h1); 
  clear_damage();  
}
 

void Fl_Value_Input_Spin::resize(int X, int Y, int W, int H) {
   input.resize(X,Y,W,H);
   Fl_Valuator::resize(X,Y,W,H);
}

void Fl_Value_Input_Spin::value_damage() {
  if (hack_o_rama) return;
  char buf[128];
  format(buf);
  input.value(buf);
  input.mark(input.position()); // turn off selection highlight
}

void Fl_Value_Input_Spin::repeat_callback(void* v) {
  Fl_Value_Input_Spin* b = (Fl_Value_Input_Spin*)v;
  if (b->mouseobj) {
    Fl::add_timeout(.1, repeat_callback, b);
    b->increment_cb();
  }
}

void Fl_Value_Input_Spin::increment_cb() {
  if (!mouseobj) return;
  delta+=deltadir;
  double v;
  switch (drag) {
    case 3: v = increment(value(), deltadir*100); break;
    case 2: v = increment(value(), deltadir*10); break;
    default:v = increment(value(), deltadir); break;
  }
  v = round(v);
  handle_drag(soft()?softclamp(v):clamp(v));
}

int Fl_Value_Input_Spin::handle(int event) {
  double v;
  int olddelta;
  int mx = Fl::event_x();
  int my = Fl::event_y();
  int sxx = x(), syy = y(), sww = w(), shh = h();
  sxx += sww - buttonssize(); sww = buttonssize();
 
  if(!indrag && ( !sldrag || !((mx>=sxx && mx<=(sxx+sww)) &&
       (my>=syy && my<=(syy+shh))))  ) {  
          indrag=0;     
	  switch(event) {
            case FL_PUSH:
            case FL_DRAG:
	      sldrag=1;
	      break;
            case FL_FOCUS:
	       input.take_focus();
	       break;
           case FL_UNFOCUS:
	       redraw();
	       break;
	    default:
	      sldrag=0;
	  }
          input.type(step()>=1.0 ? FL_INT_INPUT : FL_FLOAT_INPUT);
          return input.handle(event);
  }

  switch (event) {
  case FL_PUSH:
//    if (!step()) goto DEFAULT;
    iy = my;
    ix = mx;
    drag = Fl::event_button();
    handle_push();
    indrag=1;
    mouseobj=1;
    Fl::add_timeout(.5, repeat_callback, this);  
    delta=0;
    if(Fl::event_inside(sxx,syy,sww,shh/2)) {
     deltadir=1;
    } else if (Fl::event_inside(sxx,syy+shh/2,sww,shh/2)) {
     deltadir=-1;
    } else {
     deltadir=0;
    }
    increment_cb();
    redraw();
    return 1;
  case FL_DRAG:
    if(mouseobj) {
      mouseobj=0;
      Fl::remove_timeout(repeat_callback, this);      
    }
//    if (!step()) goto DEFAULT;
    olddelta=delta;
    delta = - (Fl::event_y()-iy);
    if ((delta>5) || (delta<-5)  ) {  deltadir=((olddelta-delta)>0)?-1:(((olddelta-delta)<0)?1:0); } 
    else  { deltadir=0; delta = olddelta;}
    switch (drag) {
    case 3: v = increment(value(), deltadir*100); break;
    case 2: v = increment(value(), deltadir*10); break;
    default:v = increment(value(), deltadir); break;
    }
    v = round(v);
    handle_drag(soft()?softclamp(v):clamp(v));
    indrag=1;
    return 1;
  case FL_RELEASE:
    if(mouseobj) {
      Fl::remove_timeout(repeat_callback, this);      
    }
//    if (!step()) goto DEFAULT;
    indrag=0;
    delta=0;
    deltadir=0;
    mouseobj=0;
    handle_release();
    redraw();
    return 1;
  case FL_FOCUS:
    indrag=0;
    return input.take_focus();
  default:
    indrag=0;
    input.type(step()>=1.0 ? FL_INT_INPUT : FL_FLOAT_INPUT);
    return 1;
  }
}

Fl_Value_Input_Spin::~Fl_Value_Input_Spin() {
  Fl::remove_timeout(repeat_callback, this);
}

Fl_Value_Input_Spin::Fl_Value_Input_Spin(int in_x, int in_y, int in_w, int in_h, const char* in_l) : 
  Fl_Valuator(in_x,in_y,in_w,in_h,in_l), 
  input(in_x, in_y, in_w, in_h, nullptr)
{
  soft_ = 0;
  if (input.parent())
  {
    // defeat automatic-add
    ((Fl_Group*)input.parent())->remove(input);
  }
  input.parent((Fl_Group *)this); // kludge!
  input.callback(input_cb, this);
  input.when(FL_WHEN_CHANGED);
  selection_color(input.selection_color());
  align(FL_ALIGN_LEFT);
  box(input.box());
  value_damage();
  buttonssize(15);
  ix=in_x;
  iy=in_y; 
  drag=0;
  indrag=0;
  sldrag=0;
  mouseobj = 0;
  deltadir=0;
  delta=0;
}
