//
// "Gleam" drawing routines for the Fast Light Tool Kit (FLTK).
//
// These box types provide a sort of Clearlooks Glossy scheme
// for FLTK.
//
// Copyright 2001-2005 by Colin Jones.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems on the following page:
//
//     http://www.fltk.org/str.php
//

// Box drawing code for an obscure box type.
// These box types are in seperate files so they are not linked
// in if not used.

#include <FL/Fl.H>
#include <FL/fl_draw.H>

static void gleam_color(Fl_Color c) {
  if (Fl::draw_box_active()) fl_color(c);
  else fl_color(fl_inactive(c));
}

static void frame_rect(int x, int y, int w, int h, Fl_Color bc) {

    // Draw the outline around the perimeter of the box
    gleam_color(bc);
    fl_line(x, y, x + w, y);
    fl_line(x + w, y, x + w, y + h);
    fl_line(x + w, y +h, x, y + h);
    fl_line(x, y +h, x, y);
  
}

static void shade_rect_up(int x, int y, int w, int h, Fl_Color bc) {
 // Draws the shiny
  int half = h / 2;
  gleam_color(bc);
  fl_rectf(x, y, w, h);

  gleam_color(fl_color_average(bc, FL_WHITE, .50f));  
  fl_rectf(x, y, w, half + 1);
  
}

static void shade_rect_down(int x, int y, int w, int h, Fl_Color bc) {
 // Draw a rect without the shiny.
  gleam_color(bc);	
  fl_rectf(x, y, w, h);
}


static void up_frame(int x, int y, int w, int h, Fl_Color c) {
  frame_rect(x, y, w - 1, h - 1, fl_darker(c));
}


static void thin_up_box(int x, int y, int w, int h, Fl_Color c) {

    shade_rect_down(x, y, w - 1, h - 1,  c);
    frame_rect(x + 1, y + 1, w - 3, h - 3, fl_color_average(c, FL_WHITE, .25f));   	
    frame_rect(x, y, w - 1, h - 1, fl_darker(c));
 
}


static void thin_down_box(int x, int y, int w, int h, Fl_Color c) {

    shade_rect_down(x, y, w - 1, h - 1,  c);
    frame_rect(x, y, w - 1, h - 1, fl_color_average(c, FL_BLACK, .34f));
 
}

static void up_box(int x, int y, int w, int h, Fl_Color c) {
    shade_rect_up(x, y, w - 1, h - 1,  c);
    frame_rect(x, y, w - 1, h - 1, fl_darker(c));
    //draw the inner rect.
    frame_rect(x + 1, y + 1, w - 3, h - 3, fl_color_average(c, FL_WHITE, .25f));

}


static void down_frame(int x, int y, int w, int h, Fl_Color c) {
  frame_rect(x, y, w - 1, h - 1, fl_darker(c));
}


static void down_box(int x, int y, int w, int h, Fl_Color c) {
    shade_rect_down(x, y, w, h,  c);
    down_frame(x, y, w, h, fl_darker(c));
    //draw the inner rect.
    //frame_rect(x + 1, y + 1, w - 3, h - 3, fl_color_average(c, FL_BLACK, .65));
 
  
}

extern void fl_internal_boxtype(Fl_Boxtype, Fl_Box_Draw_F*);


Fl_Boxtype fl_define_FL_GLEAM_UP_BOX() {
  fl_internal_boxtype(_FL_GLEAM_UP_BOX, up_box);
  fl_internal_boxtype(_FL_GLEAM_DOWN_BOX, down_box);
  fl_internal_boxtype(_FL_GLEAM_UP_FRAME, up_frame);
  fl_internal_boxtype(_FL_GLEAM_DOWN_FRAME, down_frame);
  fl_internal_boxtype(_FL_GLEAM_THIN_UP_BOX, thin_up_box);
  fl_internal_boxtype(_FL_GLEAM_THIN_DOWN_BOX, thin_down_box);
  fl_internal_boxtype(_FL_GLEAM_ROUND_UP_BOX, up_box);
  fl_internal_boxtype(_FL_GLEAM_ROUND_DOWN_BOX, down_box);

  return _FL_GLEAM_UP_BOX;
}
