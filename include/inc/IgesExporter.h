
#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include <inc/Exporter.h>

namespace inc {

class IgesExporter : public Exporter {
public:
    IgesExporter(const std::string& file_name);

    enum Section {
        IGES_START = 0,
        IGES_GLOBAL = 1,
        IGES_DIRECTORY_ENTRY = 2,
        IGES_PARAMETER_DATA = 3,
        IGES_TERMINATING = 4
    };

    void begin();
    void end();

    // sadly I don't know how to do this
    void set_layer(int) { }
    void add_layer() { }

    //void write_file();
    virtual void write_point(float vx, float vy, float vz);

    virtual void write_line(float v1x, float v1y, float v1z, float v2x, float v2y, float v2z);
    virtual void write_triangle(
        float v1x, float v1y, float v1z,
        float v2x, float v2y, float v2z,
        float v3x, float v3y, float v3z);

    virtual void write_tetrahedron(
        float v1x, float v1y, float v1z,
        float v2x, float v2y, float v2z,
        float v3x, float v3y, float v3z,
        float v4x, float v4y, float v4z);

private:
    void write_header();
    void write_footer();
    void write_line(const std::string&);
    void increment_section();
    void write_b_spline_surface_directory_entry(int line_start, int num_lines);
    void setup_line(char* l);
    void write_all_tetra_nodes();

    void write_directory_entry(char* l, std::string text, int index);

    void write_point_directory_entry(int line_start);
    void write_line_directory_entry(int param_pointer);
    void write_ruled_surface_directory_entry(int param_pointer);
    void write_node_directory_entry(int line_start);
    void write_tetrahedron_directory_entry(int line_start);

    void write_parameter_entry(std::string text, int pointer);
    void write_point_parameters(float x, float y, float z, int pointer);
    void write_line_parameters(float x1, float y1, float z1, float x2, float y2, float z2, int pointer);
    void write_ruled_surface_parameters(int pointer_1, int pointer_2, int back_pointer);
    void write_node_parameters(float x, float y, float z);
    void write_tetrahedron_parameters(const std::vector<int>& des);

    std::vector<int> section_line_numbers_;

    int paramater_line_count_;

    int line_counter_;
    Section current_section_;

    int entity_count_;

    std::vector<int> de_pointers_;
    std::vector<int> line_pointers_;

    std::vector<std::vector<int>> nodes_de_;
    std::vector<std::vector<float>> nodes_;

    std::ofstream file_;
    std::string file_name_;

    int current_layer_;


};

}