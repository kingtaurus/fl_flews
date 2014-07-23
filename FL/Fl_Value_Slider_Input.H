#ifndef __Fl_Value_Slider_Input__
#define __Fl_Value_Slider_Input__
#ifndef FLTK_1_0_COMPAT
#define FLTK_1_0_COMPAT
#endif

#include <FL/Fl.H>
#include <FL/Fl_Input.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Value_Slider.H>
#include <math.h>

class Fl_Value_Slider_Input : 
  public Fl_Value_Slider {
private:

  char soft_;
  int txtboxsize;

  static FL_EXPORT void input_cb(Fl_Widget*,void*);
  virtual FL_EXPORT void value_damage() override; // cause damage() due to value() changing
  
#if FL_MAJOR_VERSION==1 && FL_MINOR_VERSION==3
// draw is now private on fltk 1.3
protected:
#else
// draw is public on 1.1.x
public:
#endif
  FL_EXPORT void draw() override;

public:
  // FUGLY HACK: since fltk 1.3 made the draw method private on now this ugly hack
  // is needed to acces the input widget draw method, this is temporary until
  // I get time to rewrite these composite widgets using an Fl_Group 
  // (but then they wouldn't show nicely on fluid :-(
  class Fl_Value_Slider_Input_Input :
    public Fl_Input
  {
  public:
    Fl_Value_Slider_Input_Input(int in_x,int in_y,int in_w,int in_h,const char * in_l = nullptr)
      : Fl_Input(in_x,in_y,in_w,in_h,in_l) {}
    void doDraw() { draw(); }
  };

  Fl_Value_Slider_Input_Input input;
  FL_EXPORT int handle(int) override;
  FL_EXPORT void resize(int,int,int,int) override;
  FL_EXPORT Fl_Value_Slider_Input(int x,int y,int w,int h, const char *l = nullptr);

  void soft(char in_x) {soft_ = in_x;}
  char soft() const {return soft_;}

  Fl_Font textfont() const {return input.textfont();}
  void textfont(uchar s) { input.textfont(s);}
  uchar textsize() const {return input.textsize();}
  void textsize(uchar s) {input.textsize(s);}
  Fl_Color textcolor() const {return input.textcolor();}
  void textcolor(uchar n) {input.textcolor(n);}
  Fl_Color cursor_color() const {return input.cursor_color();}
  void cursor_color(uchar n) {input.cursor_color(n);}
  void textboxsize(int s) { txtboxsize=s;}
  int textboxsize() const {return txtboxsize;}
};

#endif
