
#include <boost/lexical_cast.hpp>

#include <inc/VrmlExporter.h>

namespace inc {

VrmlExporter::VrmlExporter(const std::string& file_name) {
    file_name_ = file_name + ".wrl";
}

void VrmlExporter::begin() {
    file_.open(file_name_, std::ios::out);
    write_header();
}

void VrmlExporter::end() {
    write_footer();
    file_.close();
}

void VrmlExporter::write_header() {
    file_ << "#VRML V2.0 utf8" << std::endl;
}

void VrmlExporter::write_footer() {
}

void VrmlExporter::write_point(float vx, float vy, float vz) {
}

void VrmlExporter::write_line(float v1x, float v1y, float v1z, float v2x, float v2y, float v2z) {
}

void VrmlExporter::write_triangle(float v1x, float v1y, float v1z, float v2x, float v2y, float v2z,
    float v3x, float v3y, float v3z) {
}

void VrmlExporter::write_tetrahedron(float v1x, float v1y, float v1z, float v2x, float v2y, float v2z,
    float v3x, float v3y, float v3z, float v4x, float v4y, float v4z) {

    file_ << "Shape {" << std::endl;
    //file_ << "solid TRUE" << std::endl;
    file_ << "geometry IndexedFaceSet {" << std::endl;
    file_ << "coord Coordinate {" << std::endl;
    file_ << "point [" << std::endl;
    file_ << boost::lexical_cast<std::string>(v1x) << " " << boost::lexical_cast<std::string>(v1y) << " " << boost::lexical_cast<std::string>(v1z) << " " << std::endl << 
        boost::lexical_cast<std::string>(v2x) << " " << boost::lexical_cast<std::string>(v2y) << " " << boost::lexical_cast<std::string>(v2z) << " " << std::endl << 
        boost::lexical_cast<std::string>(v3x) << " " << boost::lexical_cast<std::string>(v3y) << " " << boost::lexical_cast<std::string>(v3z) << " " << std::endl << 
        boost::lexical_cast<std::string>(v4x) << " " << boost::lexical_cast<std::string>(v4y) << " " << boost::lexical_cast<std::string>(v4z) << " " << std::endl <<
        "]" << std::endl << // end point
        "}" << std::endl; // end coord Coordinate
    file_ << "coordIndex [" << std::endl <<
        "0 3 2 -1 3 1 2 -1 0 1 3 -1 2 1 0 -1" << std::endl <<
        "]" << std::endl << // end coordIndex
        "}" << std::endl << // end geometry
        "}" << std::endl; // end shape
}

}
