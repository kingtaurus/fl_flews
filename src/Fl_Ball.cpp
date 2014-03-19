#ifndef FLTK_1_0_COMPAT
#define FLTK_1_0_COMPAT
#endif

#include <FL/Fl.H>
#include <FL/Fl_Group.H>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "FL/Fl_Ball.H"
#include <FL/fl_draw.H>

#ifndef _WIN32
#include <values.h>
#endif

#ifndef M_PI
#define M_PI            3.14159265358979323846
#define M_PI_2          1.57079632679489661923
#endif

#define _FL_M_PI180          0.017453
#define _FL_M_180PI 	     57.295780
#define _FL_DEGTORAD(x)	     (x*_FL_M_PI180) 
#define _FL_RADTODEG(x)	     (x*_FL_M_180PI)

#ifndef UNUSED
#define UNUSED(expr) do { (void)(expr); } while (0)
#endif

void Fl_Ball::rotateball(float rotx,float roty,float rotz)
{
  float a[3];
  a[0]=rotx;
  a[1]=roty;
  a[2]=rotz;
  curquat.fromEuler(a);   
  curquat.toRotMatrix(mat);
}		     

void Fl_Ball::getrot(float &rotx,float &roty,float &rotz)
{
  float a[3];
  fl_rotMatrixToEuler(mat, a);
  rotx=a[0];
  roty=a[1];
  rotz=a[2];   
}
void Fl_Ball::transform_ball_vertex(float& in_x,float& in_y,float& in_z)
{
 float x1,y1,z1;
    x1 = in_x * mat[0][0] + in_y * mat[1][0] + in_z * mat[2][0];
    y1 = in_x * mat[0][1] + in_y * mat[1][1] + in_z * mat[2][1];
    z1 = in_x * mat[0][2] + in_y * mat[1][2] + in_z * mat[2][2];
 in_x = x1;
 in_y = y1;
 in_z = z1;
 return;
}

void Fl_Ball::rotate(float &in_x,float &in_y,float &in_z, 
                     float rotx,float roty,float rotz)
{
  float cx,sx,cy,sy,cz,sz;
  float x1,y1,z1;
  cx=float(cos(rotx));
  cy=float(cos(roty));
  cz=float(cos(rotz));
  sx=float(sin(rotx));
  sy=float(sin(roty));
  sz=float(sin(rotz));
  x1 = in_x*cz + in_y*sz;
  y1 = in_y*cz - in_x*sz;
  z1 = in_z;
  in_x = x1*cy + z1*sy;
  in_y = y1;
  in_z = z1*cy - x1*sy; 
  x1= in_x;
  y1= in_y*cx + in_z*sx;
  z1= in_z*cx - in_y*sx;
  in_x=x1;
  in_y=y1;
  in_z=z1;
}

void Fl_Ball::draw_solid_ball(float radius,int in_slices,int in_stacks)
{
  float rho, drho, theta, dtheta;
  float srho,crho,snrho,cnrho;
  float ctheta,stheta;
  int i,j;
  float local_x[4], local_y[4], local_z[4];
  float xv0,yv0;
  float xv1,yv1;
  float zn;
  
  drho = float(M_PI) / float(in_stacks);
  dtheta = float(2.0) * float(M_PI) / float(in_slices);
  for (i=0;i<in_stacks;i++)
  { 
    rho = i * drho;
    srho=float(sin(rho));
    crho=float(cos(rho));
    snrho=float(sin(rho+drho));
    cnrho=float(cos(rho+drho));
    for (j=0;j<in_slices;j++)
    {
      theta = j * dtheta;
      ctheta=float(cos(theta));
      stheta=float(sin(theta));
      local_x[0] = ctheta * srho;
      local_y[0] = stheta * srho;
      local_z[0] = crho;
      transform_ball_vertex(local_x[0],local_y[0],local_z[0]);       
      local_x[3] = ctheta * snrho;
      local_y[3] = stheta * snrho;
      local_z[3] = cnrho;
      transform_ball_vertex(local_x[3],local_y[3],local_z[3]);       
       
      theta = (j+1) * dtheta;
      ctheta=float(cos(theta));
      stheta=float(sin(theta));
      local_x[1] = ctheta * srho;
      local_y[1] = stheta * srho;
      local_z[1] = crho;
      transform_ball_vertex(local_x[1],local_y[1],local_z[1]);       
      local_x[2] = ctheta * snrho;
      local_y[2] = stheta * snrho;
      local_z[2] = cnrho;
      transform_ball_vertex(local_x[2],local_y[2],local_z[2]);       

      xv0=local_x[1]-local_x[0];      yv0=local_y[1]-local_y[0];       
      xv1=local_x[2]-local_x[1];      yv1=local_y[2]-local_y[1];       
      zn=xv0*yv1-xv1*yv0;
      if(zn>=-1e-6)
      {
        if((i+j)&1)
        {
          fl_color(contrast(FL_WHITE,selection_color()));
        }
        else
        {
          fl_color(selection_color());
        }
        fl_begin_polygon();
           fl_vertex( (local_x[0]*radius), (local_y[0]*radius) );
           fl_vertex( (local_x[1]*radius), (local_y[1]*radius) );
           fl_vertex( (local_x[2]*radius), (local_y[2]*radius) );
           fl_vertex( (local_x[3]*radius), (local_y[3]*radius) );
        fl_end_polygon();
      }
    }
  }
}

void Fl_Ball::draw_wire_ball(float radius,int in_slices,int in_stacks)
{
  float rho, drho, theta, dtheta;
  int i,j;
  float local_x, local_y, local_z;
  
  drho = float(M_PI) / float(in_stacks);
  dtheta = float(2.0) * float(M_PI) / float(in_slices);
  for (i=1;i<in_stacks;i++) {  
    rho = i * drho;
    fl_begin_line();
    for (j=0;j<=in_slices;j++) {
       theta = j * dtheta;
       local_x = float(cos(theta)) * float(sin(rho));
       local_y = float(sin(theta)) * float(sin(rho));
       local_z = float(cos(rho));
       transform_ball_vertex(local_x,local_y,local_z);
       fl_vertex( (local_x*radius), (local_y*radius) );
    }
    fl_end_line();
  }
 
  for (j=0;j<=in_slices;j++) {
    theta = j * dtheta;
    fl_begin_line();
    for (i=0;i<=in_stacks;i++) {
       rho = i * drho;
       local_z = float(cos(rho));
       local_x = float(cos(theta)) * float(sin(rho));
       local_y = float(sin(theta)) * float(sin(rho));
       transform_ball_vertex(local_x,local_y,local_z);
       fl_vertex( (local_x*radius), (local_y*radius) );
    }
    fl_end_line();
  }
}
void Fl_Ball::draw() {

  int sxx = x(), syy = y(), sww = w(), shh = h(); 
  Fl_Boxtype box1 = (Fl_Boxtype)(box());
  int border_size=Fl::box_dx(box());

  if (damage()&~FL_DAMAGE_CHILD) clear_damage(FL_DAMAGE_ALL);

  if (!box1) box1 = (Fl_Boxtype)(box()&-2);
  
  draw_box(box(),sxx,syy,sww,shh,color());

  sxx+=border_size;
  syy+=border_size;
  sww-=border_size*2;  
  shh-=border_size*2;

  if (active_r()) {
        fl_color(selection_color());  
  } else {
        fl_color(selection_color() | 8);
  }    
  fl_clip(sxx,syy,sww,shh);
    fl_push_matrix();
      fl_translate(sxx+sww/2.0,syy+shh/2.0);
      fl_scale(ballscale());
      if(horizontal() ) {
        draw_wire_ball((sww<shh)?(float)(sww/2.0):(float)(shh/(float)2.0),stacks(),slices());
      } else {
        draw_solid_ball((sww<shh)?(float)(sww/2.0):(float)(shh/(float)2.0),stacks(),slices());
      }
    fl_pop_matrix();
  fl_pop_clip();

  clear_damage();  
} 

void Fl_Ball::repeat_callback(void* v) {
  Fl_Ball* b = (Fl_Ball*)v;
  if (b->mouseobj) {
    Fl::add_timeout(.1, repeat_callback, b);
    b->increment_cb();
    b->redraw();
  }
}

void Fl_Ball::increment_cb() {
//  if (!mouseobj) return;
  Fl_Quaternion tq(lastquat);
  tq.add(curquat);
  curquat.copy(tq);
  curquat.toRotMatrix(mat);  
  value_damage();
  if (when() & FL_WHEN_CHANGED) do_callback();
  else set_changed();
}

void Fl_Ball::handle_drag(double v) {
	UNUSED(v);
  value_damage();
  if (when() & FL_WHEN_CHANGED) do_callback();
  else set_changed();
}

void Fl_Ball::handle_release() {
  if (when()&FL_WHEN_RELEASE) {
    // insure changed() is off even if no callback is done.  It may have
    // been turned on by the drag, and then the slider returned to it's
    // initial position:
    clear_changed();
    // now do the callback only if slider in new position or always is on:
    if (mouseobj || when() & FL_WHEN_NOT_CHANGED)
      do_callback();
  }
}

int Fl_Ball::handle(int event) {
  int mx = Fl::event_x();
  int my = Fl::event_y();
  int sxx = x(), syy = y(), sww = w(), shh = h();
  
  switch (event) {
  case FL_PUSH:
//    if (!step()) goto DEFAULT;
    drag = Fl::event_button();
    spinning=0;
    handle_push();
    if(mouseobj) {
       Fl::remove_timeout(repeat_callback, this);      
    }
//    TRACKBALLSIZE=sqrt(((2.0 * (mx-sxx) - sww) / sww)*((2.0 * (mx-sxx) - sww)/sww)+
//   ((shh - 2.0 * (my-syy)) / shh)*((shh - 2.0 * (my-syy)) / shh));
//   TRACKBALLSIZE=(TRACKBALLSIZE>ballscale())?ballscale():TRACKBALLSIZE;
//  TRACKBALLSIZE=ballscale();
//       TRACKBALLSIZE=1.0;
    iy = my;
    ix = mx;
    mouseobj=0;
    redraw();
    return 1;
  case FL_DRAG:
     fl_trackball(lastquat,
      -(float)(2.0 * (ix-sxx) - sww) / sww,   
      (float)(shh - 2.0 * (iy-syy)) / shh,
      -(float)(2.0 * (mx-sxx) - sww) / sww,
      (float)(shh - 2.0 * (my-syy)) / shh
      );
      spinning=1;
 //   if (!step()) goto DEFAULT;
      mouseobj=1;
      increment_cb();
      ix=mx;
      iy=my;
      handle_drag();
      redraw();      
    return 1;
  case FL_RELEASE:
//    if (!step()) goto DEFAULT;
     if(drag==1 && mouseobj) {
         mouseobj=1;
         Fl::add_timeout(.1, repeat_callback, this);  
    } 
    drag=0;
    handle_release();
    redraw();
    return 1;
  default:
    drag=0;    
    return Fl_Valuator::handle(event);
  }
}

Fl_Ball::~Fl_Ball() {
  Fl::remove_timeout(repeat_callback, this);
}

Fl_Ball::Fl_Ball(int in_x, int in_y, int in_w, int in_h, const char * in_l) :
  Fl_Valuator(in_x,in_y,in_w,in_h,in_l)
{
  soft_ = 0;
  ix = in_x;
  iy = in_y; 
  drag=0;
  spinning=0;
  mouseobj = 0;
  ballscale(1.0);
  stacks(10);
  slices(10);  
  rotateball();
  
  for(int i=0; i<4; i++) 
  { 
    for(int j=0; j<4; j++)
    {
      mat[i][j]=(i==j)? float(1.0) : float(0.0);
    }
  } 
  fl_trackball(curquat, 0.0, 0.0, 0.0, 0.0);
  fl_trackball(lastquat, 0.0, 0.0, 0.0, 0.0);
}
