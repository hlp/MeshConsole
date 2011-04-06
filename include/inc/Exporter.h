
#pragma once

namespace inc {

class Exporter {
public:
    virtual ~Exporter() { }

    virtual void write_point(float vx, float vy, float vz) = 0;
    virtual void write_line(float v1x, float v1y, float v1z, float v2x, float v2y, float v2z) = 0;
    virtual void write_triangle(
        float v1x, float v1y, float v1z,
        float v2x, float v2y, float v2z,
        float v3x, float v3y, float v3z) = 0;

    virtual void set_layer(int) = 0;
    virtual void add_layer() = 0;
};

}
