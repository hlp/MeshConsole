
/*  Copyright (c) 2010, Patrick Tierney
 *
 *  This file is part of INC (INC's Not CAD).
 *
 *  INC is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  INC is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with INC.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * 
 *  Based on the original DXF writer from Simon Greenwold, February 2004.
 *  Updated for Processing 0070 by Ben Fry in September 2004,
 *  and again for Processing beta in April 2005.
 *  Rewritten to support beginRaw/endRaw by Ben Fry in February 2006.
 *  Updated again for inclusion as a core library in March 2006.
 *  Constructor modifications in September 2008 as we approach 1.0.
 *  Ported to C++ September 2010 by Patrick Tierney
 */


#include <inc/DxfSaver.h>

namespace inc {

DxfSaver::DxfSaver(const std::string& file_name) {
    file_name_ = file_name + ".dwg";
    current_layer_ = 0;
    INC_EPSILON = 0.0001f;
}

void DxfSaver::set_path(const std::string& file_name) {
    file_name_ = file_name;
}

void DxfSaver::set_layer(int layer) {
    current_layer_ = layer;
}

void DxfSaver::add_layer() {
    ++current_layer_;
}

void DxfSaver::begin() {
    file_.open(file_name_, std::ios::out);
    write_header();
}

void DxfSaver::end() {
    write_footer();
    file_.close();
}

void DxfSaver::write_header() {
    file_ << "0" << std::endl;
    file_ << "SECTION" << std::endl;
    file_ << "2" << std::endl;
    file_ << "ENTITIES" << std::endl;
}

void DxfSaver::write_footer() {
    file_ << "0" << std::endl;
    file_ << "ENDSEC" << std::endl;
    file_ << "0" << std::endl;
    file_ << "EOF" << std::endl;
}

/**
* Write a command on one line (as a String), then start a new line
* and write out a formatted float. Available for anyone who wants to
* insert additional commands into the DXF stream.
*/
void DxfSaver::write(std::string cmd, float val) {
    file_ << cmd << std::endl;
    // don't format, will cause trouble on systems that aren't en-us
    file_ << val << std::endl;
}

void DxfSaver::write_point(float vx, float vy, float vz) {
    write_triangle(vx, vy, vz, 
        vx + INC_EPSILON, vy + INC_EPSILON, vz + INC_EPSILON,
        vx - INC_EPSILON, vy - INC_EPSILON, vz - INC_EPSILON);
}

void DxfSaver::write_line(float v1x, float v1y, float v1z, float v2x, float v2y, float v2z) {
    file_ << "0" << std::endl;
    file_ << "LINE" << std::endl;

    // write out the layer
    file_ << "8" << std::endl;
    file_ << current_layer_ << std::endl;

    write("10", v1x);
    write("20", v1y);
    write("30", v1z);

    write("11", v2x);
    write("21", v2y);
    write("31", v2z);
}


void DxfSaver::write_triangle(float v1x, float v1y, float v1z,
        float v2x, float v2y, float v2z,
        float v3x, float v3y, float v3z) {
    file_ << "0" << std::endl;
    file_ << "3DFACE" << std::endl;

    // write out the layer
    file_ << "8" << std::endl;

    /*
    if (i < MAX_TRI_LAYERS) {
      if (layerList[i] >= 0) {
        currentLayer = layerList[i];
      }
    }
    */

    file_ << current_layer_ << std::endl;

    write("10", v1x);
    write("20", v1y);
    write("30", v1z);

    write("11", v2x);
    write("21", v2y);
    write("31", v2z);

    write("12", v3x);
    write("22", v3y);
    write("32", v3z);

    // without adding EPSILON, rhino kinda freaks out
    // a face is actually a quad, not a triangle,
    // so instead kinda fudging the final point here.
    write("13", v3x + INC_EPSILON);
    write("23", v3y + INC_EPSILON);
    write("33", v3z + INC_EPSILON);
}



}