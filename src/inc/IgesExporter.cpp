
#include <cstdio>

#include <boost/lexical_cast.hpp>

#include <inc/IgesExporter.h>

namespace inc {

IgesExporter::IgesExporter(const std::string& file_name) {
    file_name_ = file_name + ".igs";
    current_section_ = IGES_START;
    line_counter_ = 1;
    paramater_line_count_ = 1;
    section_line_numbers_.resize(5);
}

void IgesExporter::begin() {
    file_.open(file_name_, std::ios::out);
    write_header();
}

void IgesExporter::end() {
    increment_section(); // set section to paramaters

    write_all_tetra_nodes();

    increment_section(); // set section to Terminating

    write_footer();
    file_.close();
}

void IgesExporter::setup_line(char* l) {
    // null out the line
    for (int i = 0; i < 80; ++i) {
        l[i] = ' ';
    }

    // add terminal NULL
    l[80] = '\0';
}

void IgesExporter::write_line(const std::string& line) {
    if (line.length() > 72)
        return;

    char l[81];
    char c;
    
    setup_line(l);

    switch (current_section_) {
    case IGES_START:
        c = 'S';
        break;

    case IGES_GLOBAL:
        c = 'G';
        break;

    case IGES_DIRECTORY_ENTRY:
        c = 'D';
        break;

    case IGES_PARAMETER_DATA:
        c = 'P';
        break;

    case IGES_TERMINATING:
    default:
        c = 'T';
        break;
    }

    // set the section data
    l[72] = c;

    // write the line number

    std::string line_num_str = boost::lexical_cast<std::string>(line_counter_);

    int line_num_index = 80 - line_num_str.length();

    strcpy(&l[line_num_index], line_num_str.c_str());

    // now actually write the line

    strncpy(l, line.c_str(), line.length());

    file_ << l << std::endl;

    ++line_counter_;
    
}

void IgesExporter::increment_section() {
    if (current_section_ == IGES_TERMINATING) {
        end();
        return;
    }

    int lines = line_counter_ - 1;
    line_counter_ = 1;

    switch (current_section_) {
    case IGES_START:
        section_line_numbers_[0] = lines;
        current_section_ = IGES_GLOBAL;
        break;

    case IGES_GLOBAL:
        section_line_numbers_[1] = lines;
        current_section_ = IGES_DIRECTORY_ENTRY;
        break;

    case IGES_DIRECTORY_ENTRY:
        section_line_numbers_[2] = lines;
        current_section_ = IGES_PARAMETER_DATA;
        break;

    case IGES_PARAMETER_DATA:
    default:
        section_line_numbers_[3] = lines;
        current_section_ = IGES_TERMINATING;
        break;
    }

    
}

////////////////////////////////////
// SETUP AND GLOBAL
////////////////////////////////////

void IgesExporter::write_header() {
    // start section doesn't matter
    write_line(" ");
    increment_section();

    // this is mainly unit info, TODO: make real
    write_line("1H,,1H;,,");
    write_line("32HC:\\Users\\patrick\\Desktop\\tri.igs,");
    write_line("26HRhinoceros ( Sep 23 2008 ),31HTrout Lake IGES 012 Sep 23 2008,");
    write_line("32,38,6,308,15,");
    write_line(",");
    write_line("1.0D0,4,2HFT,1,0.0008333333333333333D0,13H110406.041253,");
    write_line("0.01D0,");
    write_line("7.899957317595756D0,");
    write_line(",");
    write_line(",");
    write_line("10,0,13H110406.041253;");

    increment_section(); // move to directory entry
}


///////////////////////////////////////
// DIRECTORY ENTRIES
//////////////////////////////////////

void IgesExporter::write_point_directory_entry(int line_start) {
    char l1[73];
    char l2[73];

    for (int i = 0; i < 73; ++i) {
        l1[i] = ' ';
        l2[i] = ' ';
    }

    l1[72] = '\0';
    l2[72] = '\0';

    std::string type_num = "116";

    write_directory_entry(l1, type_num, 1);
    write_directory_entry(l1, boost::lexical_cast<std::string>(line_start), 2);
    write_directory_entry(l1, "0", 3);
    write_directory_entry(l1, "0", 4);
    write_directory_entry(l1, "-1", 5);
    write_directory_entry(l1, "0", 6);
    write_directory_entry(l1, "0", 7);
    write_directory_entry(l1, "0", 8);
    write_directory_entry(l1, "00000000", 9);
    // nothing else is important / defined

    write_directory_entry(l2, type_num, 1);
    write_directory_entry(l2, "0", 2);
    write_directory_entry(l2, "-1", 3);
    write_directory_entry(l2, "5", 4); // always 5 
    write_directory_entry(l2, "0", 5);
    write_directory_entry(l2, "0", 6);
    write_directory_entry(l2, "0", 7);
    write_directory_entry(l2, "3d Point", 8);
    write_directory_entry(l2, "0", 9);

    std::string line_1(l1);
    std::string line_2(l2);

    write_line(line_1);
    write_line(line_2);
}

void IgesExporter::write_node_directory_entry(int line_start) {
    char l1[73];
    char l2[73];

    for (int i = 0; i < 73; ++i) {
        l1[i] = ' ';
        l2[i] = ' ';
    }

    l1[72] = '\0';
    l2[72] = '\0';

    std::string type_num = "134";

    write_directory_entry(l1, type_num, 1);
    write_directory_entry(l1, boost::lexical_cast<std::string>(line_start), 2);
    // nothing else is important / defined

    write_directory_entry(l2, type_num, 1);
    write_directory_entry(l2, "5", 4); // always 5 
    write_directory_entry(l2, "Node", 8);

    std::string line_1(l1);
    std::string line_2(l2);

    write_line(line_1);
    write_line(line_2);
}

void IgesExporter::write_tetrahedron_directory_entry(int line_start) {
    char l1[73];
    char l2[73];

    for (int i = 0; i < 73; ++i) {
        l1[i] = ' ';
        l2[i] = ' ';
    }

    l1[72] = '\0';
    l2[72] = '\0';

    std::string type_num = "136";

    write_directory_entry(l1, type_num, 1);
    write_directory_entry(l1, boost::lexical_cast<std::string>(line_start), 2);
    // nothing else is important / defined

    write_directory_entry(l2, type_num, 1);
    write_directory_entry(l2, "8", 4); // always 8 
    write_directory_entry(l2, "Tetra", 8);

    std::string line_1(l1);
    std::string line_2(l2);

    write_line(line_1);
    write_line(line_2);
}

void IgesExporter::write_b_spline_surface_directory_entry(int line_start, int num_lines) {
    char l1[73];
    char l2[73];

    for (int i = 0; i < 73; ++i) {
        l1[i] = ' ';
        l2[i] = ' ';
    }

    l1[72] = '\0';
    l2[72] = '\0';

    std::string type_num = "128";

    write_directory_entry(l1, type_num, 1);
    write_directory_entry(l1, boost::lexical_cast<std::string>(line_start), 2);
    write_directory_entry(l1, "0", 3);
    write_directory_entry(l1, "0", 4);
    write_directory_entry(l1, "1", 5);
    write_directory_entry(l1, "0", 6);
    write_directory_entry(l1, "0", 7);
    write_directory_entry(l1, "0", 8);
    write_directory_entry(l1, "00000000", 9);

    write_directory_entry(l2, type_num, 1);
    write_directory_entry(l2, "0", 2);
    write_directory_entry(l2, "-1", 3);
    write_directory_entry(l2, boost::lexical_cast<std::string>(num_lines), 4);
    write_directory_entry(l2, "0", 5);
    write_directory_entry(l2, "0", 6);
    write_directory_entry(l2, "0", 7);
    write_directory_entry(l2, "TrimSrf", 8);
    write_directory_entry(l2, "0", 9);

    std::string line_1(l1);
    std::string line_2(l2);

    write_line(line_1);
    write_line(line_2);
}

void IgesExporter::write_directory_entry(char* l, std::string text, int index) {
    strncpy(&l[index * 8 - text.length()], text.c_str(), text.length());
}



///////////////////////////////////////////
// PARAMATERS
//////////////////////////////////////////

void IgesExporter::write_parameter_entry(std::string text, int pointer) {
    char l[73];

    for (int i = 0; i < 73; ++i) {
        l[i] = ' ';
    }

    l[72] = '\0';

    for (int i = 8 * 8; i < 8 * 9; ++i) {
        l[i] = '0';
    }

    strncpy(l, text.c_str(), text.length());

    write_directory_entry(l, boost::lexical_cast<std::string>(pointer), 9);

    write_line(std::string(l));
}

void IgesExporter::write_point_parameters(float x, float y, float z, int pointer) {
    write_parameter_entry("116,", pointer); // type name
    write_parameter_entry(boost::lexical_cast<std::string>(x) + "D0,", pointer);
    write_parameter_entry(boost::lexical_cast<std::string>(y) + "D0,", pointer);
    write_parameter_entry(boost::lexical_cast<std::string>(z) + "D0,", pointer);
    write_parameter_entry("0;", pointer); // this is the coordinate system
}

void IgesExporter::write_node_parameters(float x, float y, float z) {
    write_line("134,"); // type name
    write_line(boost::lexical_cast<std::string>(x) + ",");
    write_line(boost::lexical_cast<std::string>(y) + ",");
    write_line(boost::lexical_cast<std::string>(z) + ",");
    write_line(";"); // this is the coordinate system
}

void IgesExporter::write_tetrahedron_parameters(const std::vector<int>& des) {
    write_line("136,"); // type name
    write_line("1,"); // topology type?? Linear?
    write_line("4,"); // 4 nodes define this
    write_line(boost::lexical_cast<std::string>(des[0]) + ",");
    write_line(boost::lexical_cast<std::string>(des[1]) + ",");
    write_line(boost::lexical_cast<std::string>(des[2]) + ",");
    write_line(boost::lexical_cast<std::string>(des[3]) + ",");
    write_line("16HLinearSolidTetra;"); // this is the coordinate system
}

void IgesExporter::write_footer() {
    char l[73];

    for (int i = 0; i < 73; ++i) {
        l[i] = ' ';
    }

    l[72] = '\0';

    char e1[9];
    char e2[9];
    char e3[9];
    char e4[9];

    for (int i = 0; i < 8; ++i) {
        e1[i] = '0';
        e2[i] = '0';
        e3[i] = '0';
        e4[i] = '0';
    }

    e1[0] = 'S';
    e2[0] = 'G';
    e3[0] = 'D';
    e4[0] = 'P';
    e1[8] = e2[8] = e3[8] = e4[8] = '\0';


    std::vector<std::string> strings;

    for (std::vector<int>::iterator it = section_line_numbers_.begin(); 
        it != section_line_numbers_.end(); ++it) {
        strings.push_back(boost::lexical_cast<std::string>(*it));
    }

    // I think these need 0s padding them 

    strncpy(&e1[8-strings[0].length()], strings[0].c_str(), strings[0].length());
    strncpy(&e2[8-strings[1].length()], strings[1].c_str(), strings[1].length());
    strncpy(&e3[8-strings[2].length()], strings[2].c_str(), strings[2].length());
    strncpy(&e4[8-strings[3].length()], strings[3].c_str(), strings[3].length());

    write_directory_entry(l, e1, 1);
    write_directory_entry(l, e2, 2);
    write_directory_entry(l, e3, 3);
    write_directory_entry(l, e4, 4);

    std::string line(l);

    write_line(line);
}

void IgesExporter::write_point(float vx, float vy, float vz) {
}

void IgesExporter::write_line(float v1x, float v1y, float v1z, float v2x, float v2y, float v2z) {
}

void IgesExporter::write_triangle(float v1x, float v1y, float v1z, float v2x, float v2y, float v2z,
    float v3x, float v3y, float v3z) {
}

void IgesExporter::write_tetrahedron(float v1x, float v1y, float v1z, float v2x, float v2y, float v2z,
    float v3x, float v3y, float v3z, float v4x, float v4y, float v4z) {

    std::vector<float> tetra;
    tetra.push_back(v1x); tetra.push_back(v1y); tetra.push_back(v1z);
    tetra.push_back(v2x); tetra.push_back(v2y); tetra.push_back(v2z);
    tetra.push_back(v3x); tetra.push_back(v3y); tetra.push_back(v3z);
    tetra.push_back(v4x); tetra.push_back(v4y); tetra.push_back(v4z);

    nodes_.push_back(tetra);

    //write_tetrahedron_directory_entry(paramater_line_count_);

    for (int i = 0; i < 4; ++i) {
        point_de_pointers_.push_back(line_counter_);
        write_point_directory_entry(paramater_line_count_);
        paramater_line_count_ += 5;
    }

    // we know that the paramaters for tetrahedrons will always take up 7 lines (cuz I say so)
    //paramater_line_count_ += 8;

    /*
    std::vector<int> de;

    for (int i = 0; i < 4; ++i) {
        de.push_back(line_counter_);

        write_node_directory_entry(paramater_line_count_);
        paramater_line_count_ += 5;
    }

    nodes_de_.push_back(de);
    */
}

void IgesExporter::write_all_tetra_nodes() {
    std::vector<int>::iterator de_pointers_it = point_de_pointers_.begin();

    for (std::vector<std::vector<float>>::iterator it = nodes_.begin(); it != nodes_.end(); ++it) {
        write_point_parameters(it->at(0), it->at(1), it->at(2), *de_pointers_it); ++de_pointers_it;
        write_point_parameters(it->at(3), it->at(4), it->at(5), *de_pointers_it); ++de_pointers_it;
        write_point_parameters(it->at(6), it->at(7), it->at(8), *de_pointers_it); ++de_pointers_it;
        write_point_parameters(it->at(9), it->at(10), it->at(11), *de_pointers_it); ++de_pointers_it;
    }
}

}
