
#include <boost/lexical_cast.hpp>

#include <inc/AcisExporter.h>

namespace inc {

AcisExporter::AcisExporter(const std::string& file_name) {
    file_name_ = file_name + ".sat";
    entity_count_ = 0;
}

void AcisExporter::begin() {
    file_.open(file_name_, std::ios::out);
    write_header();

    start_body();
}

void AcisExporter::end() {
    write_footer();
    file_.close();
}

void AcisExporter::start_body() {
    //write_entity("body $1 -1 $-1 $2 $-1 $-1");
    //write_entity("ref_vt-eye-attrib $-1 -1 $-1 $-1 $0 $3 $4");
    //write_entity("lump $5 -1 $-1 $-1 $6 $0");
file_ << 
"body $1 -1 $-1 $2 $-1 $-1 #" << std::endl <<
"ref_vt-eye-attrib $-1 -1 $-1 $-1 $0 $3 $4 #" << std::endl <<
"lump $5 -1 $-1 $-1 $6 $0 #" << std::endl <<
"eye_refinement $-1 -1 @5 grid  1 @3 tri 1 @4 surf 0 @3 adj 0 @4 grad 0 @9 postcheck 0 @4 stol 0.053173065185546875 @4 ntol 30 @4 dsil 0 @8 flatness 0 @7 pixarea 0 @4 hmax 0 @6 gridar 0 @5 mgrid 3000 @5 ugrid 0 @5 vgrid 0 @10 end_fields #" << std::endl <<
"vertex_template $-1 -1 3 0 1 8 #" << std::endl <<
"ref_vt-eye-attrib $-1 -1 $-1 $-1 $2 $3 $4 #" << std::endl <<
"shell $7 -1 $-1 $-1 $-1 $8 $-1 $2 #" << std::endl;
}

void AcisExporter::write_entity(const std::string& str) {
//    file_ << "-" << boost::lexical_cast<std::string>(entity_count_) << " " <<
//        str << " #" << std::endl;

    file_ << str << " #" << std::endl;

    //entity_count_++;
}

void AcisExporter::write_header() {
    file_ << "700 132 1 0" << std::endl;
    file_ << "16 Autodesk AutoCAD 19 ASM 216.0.0.4501 NT 24 Tue Apr 05 21:03:20 2011" << std::endl;
    file_ << "25.399999999999999 9.9999999999999995e-007 1e-01" << std::endl;
}

void AcisExporter::write_footer() {
    file_ << "End-of-ACIS-data" << std::endl;
}

void AcisExporter::write_point(float vx, float vy, float vz) {
    write_entity(std::string("point $-1 -1 $-1") + boost::lexical_cast<std::string>(vx) + 
        boost::lexical_cast<std::string>(vy) + boost::lexical_cast<std::string>(vz));
}

void AcisExporter::write_line(float v1x, float v1y, float v1z, float v2x, float v2y, float v2z) {
}

void AcisExporter::write_triangle(float v1x, float v1y, float v1z, float v2x, float v2y, float v2z,
    float v3x, float v3y, float v3z) {
file_ << 
"ref_vt-eye-attrib $-1 -1 $-1 $-1 $6 $3 $4 #" << std::endl << 
"face $9 -1 $-1 $-1 $10 $6 $-1 $11 reversed double out #" << std::endl <<
"color-adesk-attrib $-1 -1 $12 $-1 $8 256 #" << std::endl <<
"loop $-1 -1 $-1 $-1 $13 $8 #" << std::endl <<
"plane-surface $-1 -1 $-1 -0.3666666666666667 -0.73333333333333339 -1.0999999999999999 0.40824829046386196 -0.81649658092772626 0.40824829046386357 -0.70710678118654891 0 0.70710678118654613 forward_v I I I I #" << std::endl <<
"fmesh-eye-attrib $-1 -1 $14 $9 $8 #" << std::endl <<
"coedge $-1 -1 $-1 $15 $16 $-1 $17 forward $10 $-1 #" << std::endl <<
"ref_vt-eye-attrib $-1 -1 $18 $12 $8 $3 $4 #" << std::endl <<
"coedge $-1 -1 $-1 $19 $13 $-1 $20 forward $10 $-1 #" << std::endl <<
"coedge $-1 -1 $-1 $13 $19 $-1 $21 reversed $10 $-1 #" << std::endl <<
"edge $22 -1 $-1 $23 -0 $24 5.7157676649772959 $13 $25 forward @7 unknown #" << std::endl <<
"persubent-acadSolidHistory-attrib $-1 -1 $-1 $14 $8 1 1 1 1 #" << std::endl <<
"coedge $-1 -1 $-1 $16 $15 $-1 $26 forward $10 $-1 #" << std::endl <<
"edge $27 -1 $-1 $24 0 $28 13.694889557787604 $15 $29 forward @7 unknown #" << std::endl <<
"edge $30 -1 $-1 $23 0 $31 13.694889557787604 $16 $32 forward @7 unknown #" << std::endl <<
"color-adesk-attrib $-1 -1 $33 $-1 $17 256 #" << std::endl <<
"vertex $-1 -1 $-1 $21 $34 #" << std::endl <<
"vertex $-1 -1 $-1 $20 $35 #" << std::endl <<
"straight-curve $-1 -1 $-1 " << boost::lexical_cast<std::string>(v1x) << " " << boost::lexical_cast<std::string>(v1y) << " " <<
    boost::lexical_cast<std::string>(v1z) << " -0.57735026918962584 -0.57735026918962573 -0.57735026918962573 I I #" << std::endl <<
"edge $36 -1 $-1 $28 0 $31 5.7157676649772959 $19 $37 forward @7 unknown #" << std::endl <<
"color-adesk-attrib $-1 -1 $38 $-1 $20 256 #" << std::endl <<
"vertex $-1 -1 $-1 $20 $39 #" << std::endl <<
"straight-curve $-1 -1 $-1 " << boost::lexical_cast<std::string>(v3x) << " " << boost::lexical_cast<std::string>(v3y) << " " <<
    boost::lexical_cast<std::string>(v3z) << " 0.40160966445124946 0.56225353023174929 0.72289739601224889 I I #" << std::endl <<
"color-adesk-attrib $-1 -1 $40 $-1 $21 256 #" << std::endl <<
"vertex $-1 -1 $-1 $21 $41 #" << std::endl <<
"straight-curve $-1 -1 $-1 " <<  boost::lexical_cast<std::string>(v1x) << " " << boost::lexical_cast<std::string>(v1y) << " " <<
    boost::lexical_cast<std::string>(v1z) << " 0.40160966445124946 0.56225353023174929 0.72289739601224889 I I #" << std::endl <<
"ptlist-eye-attrib $-1 -1 $42 $22 $17 #" << std::endl <<
"point $-1 -1 $-1 " <<  boost::lexical_cast<std::string>(v1x) << " " << boost::lexical_cast<std::string>(v1y) << " " <<
    boost::lexical_cast<std::string>(v1z) << " #" << std::endl <<
"point $-1 -1 $-1 " <<  boost::lexical_cast<std::string>(v3x) << " " << boost::lexical_cast<std::string>(v3y) << " " <<
    boost::lexical_cast<std::string>(v3z) << " #" << std::endl <<
"color-adesk-attrib $-1 -1 $43 $-1 $26 256 #" << std::endl <<
"straight-curve $-1 -1 $-1 " <<  boost::lexical_cast<std::string>(v1x) << " " << boost::lexical_cast<std::string>(v1y) << " " <<
    boost::lexical_cast<std::string>(v1z) << " 0.57735026918962584 0.57735026918962573 0.57735026918962573 I I #" << std::endl <<
"ptlist-eye-attrib $-1 -1 $44 $27 $20 #" << std::endl <<
"point $-1 -1 $-1 " <<  boost::lexical_cast<std::string>(v1x) << " " << boost::lexical_cast<std::string>(v1y) << " " <<
    boost::lexical_cast<std::string>(v1z) << " #" << std::endl <<
"ptlist-eye-attrib $-1 -1 $45 $30 $21 #" << std::endl <<
"point $-1 -1 $-1 " <<  boost::lexical_cast<std::string>(v2x) << " " << boost::lexical_cast<std::string>(v2y) << " " <<
    boost::lexical_cast<std::string>(v2z) << " #" << std::endl <<
"persubent-acadSolidHistory-attrib $-1 -1 $-1 $33 $17 1 1 1 10 #" << std::endl <<
"ptlist-eye-attrib $-1 -1 $46 $36 $26 #" << std::endl << 
"persubent-acadSolidHistory-attrib $-1 -1 $-1 $38 $20 2 1 1 10011 1 1 20021 #" << std::endl <<
"persubent-acadSolidHistory-attrib $-1 -1 $-1 $40 $21 2 1 1 10021 1 1 20011 #" << std::endl <<
"persubent-acadSolidHistory-attrib $-1 -1 $-1 $43 $26 1 1 1 20 #" << std::endl;

    return;

    std::string line = "plane-surface $-1 -1 $-1";

    line += " ";
    line += boost::lexical_cast<std::string>(v1x);
    line += " ";
    line += boost::lexical_cast<std::string>(v1y);
    line += " ";
    line += boost::lexical_cast<std::string>(v1z);
    line += " ";
    line += boost::lexical_cast<std::string>(v2x);
    line += " ";
    line += boost::lexical_cast<std::string>(v2y);
    line += " ";
    line += boost::lexical_cast<std::string>(v2z);
    line += " ";
    line += boost::lexical_cast<std::string>(v3x);
    line += " ";
    line += boost::lexical_cast<std::string>(v3y);
    line += " ";
    line += boost::lexical_cast<std::string>(v3z);
    line += " forward_v I I I I";
    
    write_entity(line);
}

}
