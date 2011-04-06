
#include <string>
#include <fstream>

#include <inc/Exporter.h>

namespace inc {

class AcisExporter : public Exporter {
public:
    AcisExporter(const std::string& file_name);

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

private:
    void write_header();
    void write_footer();
    void write_entity(const std::string& str);
    void start_body();

    int entity_count_;

    std::ofstream file_;
    std::string file_name_;

    int current_layer_;
};

}