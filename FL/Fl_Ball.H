#ifndef __Fl_Ball__
#define __Fl_Ball__
#ifndef FLTK_1_0_COMPAT
#define FLTK_1_0_COMPAT
#endif

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Valuator.H>
#include <FL/Fl_Quaternion.H>
#include <math.h>

class Fl_Ball : 
  public Fl_Valuator
{
private:
  int ix, iy, drag;
  int spinning;
  
  int ballstacks,ballslices;
  float ballsize;

  Fl_Quaternion curquat;
  Fl_Quaternion lastquat;
  float mat[4][4];
  
  char soft_;
  uchar mouseobj;

  static FL_EXPORT void repeat_callback(void *);
  FL_EXPORT void increment_cb(); 
  FL_EXPORT void vertex_by_matrix(float &x,float &y,float &z);
  
  FL_EXPORT void transform_ball_vertex(float &x,float &y,float &z);
  FL_EXPORT void rotate(float &x,float &y,float &z, 
                        float rotx=0.0,float roty=0.0,float  rotz=0.0);
  FL_EXPORT void draw_solid_ball(float radius,int slices,int stacks);
  FL_EXPORT void draw_wire_ball(float radius,int slices,int stacks);

#if FL_MAJOR_VERSION==1 && FL_MINOR_VERSION==3
// draw is now private on fltk 1.3
protected:
#else
// draw is public on 1.1.x
public:
#endif
   FL_EXPORT void draw() override;

public:
  FL_EXPORT void handle_drag(double v=0.0);
  FL_EXPORT void handle_release();
  FL_EXPORT int handle(int) override;
  FL_EXPORT Fl_Ball(int in_x,int in_y,int in_w,int in_h, const char *in_l = nullptr);

  void soft(char in_x) {soft_ = in_x;}
  char soft() const {return soft_;}

  float ballscale() const {return ballsize;}
  void ballscale(float s) { ballsize=s;}

  void rotateball(float rotx = 0.0, float roty = 0.0,float rotz = 0.0);
  void getrot(float &rotx,float &roty,float &rotz);
  
  void stacks(int s) { ballstacks=s; }
  int stacks() const { return ballstacks; }

  void slices(int s) { ballslices=s; }
  int slices() const { return ballslices; }

  ~Fl_Ball();
};

#endif
