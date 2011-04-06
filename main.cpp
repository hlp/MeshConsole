
#include <iostream>

#include <boost/lexical_cast.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Mesh_triangulation_3.h>
#include <CGAL/Mesh_complex_3_in_triangulation_3.h>
#include <CGAL/Mesh_criteria_3.h>

#include <CGAL/Implicit_mesh_domain_3.h>
#include <CGAL/make_mesh_3.h>

#include <inc/DxfSaver.h>
#include <inc/AcisExporter.h>
#include <inc/VrmlExporter.h>
#include <inc/IgesExporter.h>

// Domain
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::FT FT;
typedef K::Point_3 Point;
typedef FT (Function)(const Point&);
typedef CGAL::Implicit_mesh_domain_3<Function,K> Mesh_domain;

// Triangulation
typedef CGAL::Mesh_triangulation_3<Mesh_domain>::type Tr;
typedef CGAL::Mesh_complex_3_in_triangulation_3<Tr> C3t3;

// Criteria
typedef CGAL::Mesh_criteria_3<Tr> Mesh_criteria;

// To avoid verbose function and named parameters call
using namespace CGAL::parameters;

// Function
FT sphere_function (const Point& p) { 
    return CGAL::squared_distance(p, Point(CGAL::ORIGIN))-1; 
}

static void print_usage() {
	std::cout << "Usage: MeshConsole [-facet_angle val] [-facet_size val] [-facet_distance val] [-cell_radius_edge val] [-cell_size val] [-split_layers] [-output output_name.dxf]\n" << std::endl;
}

float facet_angle_parse = 30;
float facet_size_parse = 0.1;
float facet_distance_parse = 0.025;
float cell_radius_edge_parse = 2;
float cell_size_parse = 0.1;
bool split_layers = false;

std::string output_name;

void parse_args(int argc, char* argv[]) {
	// Parse arguments
    argc--; argv++;
    while (argc > 0) {
        if ((*argv)[0] == '-') {

            if (!strcmp(*argv, "-split_layers")) { 
				split_layers = true; 
			}

            if (!strcmp(*argv, "-facet_angle")) { 
                argc--; argv++;
                facet_angle_parse = boost::lexical_cast<float>(*argv);
            } else if (!strcmp(*argv, "-facet_size")) { 
                 argc--; argv++;
                facet_size_parse = boost::lexical_cast<float>(*argv);
            } else if (!strcmp(*argv, "-facet_distance")) { 
                argc--; argv++;
                facet_distance_parse = boost::lexical_cast<float>(*argv);
            } else if (!strcmp(*argv, "-cell_radius_edge")) { 
                argc--; argv++;
                cell_radius_edge_parse = boost::lexical_cast<float>(*argv);
            } else if (!strcmp(*argv, "-cell_size")) { 
                argc--; argv++;
                cell_size_parse = boost::lexical_cast<float>(*argv);
            } else if (!strcmp(*argv, "-output")) { 
                argc--; argv++;
                output_name = std::string(*argv);
            } else if (!strcmp(*argv, "-help")) { 
                print_usage();
                exit(1); 
            } else { 
                fprintf(stderr, "Invalid program argument: %s", *argv); 
                print_usage();
                exit(1); 
            }
            argv++; argc--;
        }
        else {
            fprintf(stderr, "Invalid program argument: %s", *argv); 
            print_usage();
            exit(1); 
            argv++; argc--;
        }
    }
}

int main(int argc, char* argv[]) {
    output_name = "mesh";

    parse_args(argc, argv);

    // Domain (Warning: Sphere_3 constructor uses squared radius !)
    Mesh_domain domain(sphere_function, K::Sphere_3(CGAL::ORIGIN, 2.));

    // Mesh criteria, uses Magic Boost constructor
    Mesh_criteria criteria(facet_angle=facet_angle_parse, facet_size=facet_size_parse, 
        facet_distance=facet_distance_parse,cell_radius_edge=cell_radius_edge_parse, 
        cell_size=cell_size_parse);

    // Mesh generation
    C3t3 c3t3 = CGAL::make_mesh_3<C3t3>(domain, criteria);

    // Output
    //std::ofstream medit_file("out.mesh");

    //c3t3.output_to_medit(medit_file);

    inc::IgesExporter saver(output_name);

    saver.begin();
    
    for (C3t3::Cell_iterator it = c3t3.cells_begin(); it != c3t3.cells_end(); ++it) {
        /*
        for (int i = 0; i < 3; ++i) {
            saver.write_triangle(it->vertex(i)->point().x(),
                it->vertex(i)->point().y(),
                it->vertex(i)->point().z());
        }
        */
        /*
        saver.write_triangle(it->vertex(0)->point().x(),
            it->vertex(0)->point().y(),
            it->vertex(0)->point().z(),
            it->vertex(1)->point().x(),
            it->vertex(1)->point().y(),
            it->vertex(1)->point().z(),
            it->vertex(2)->point().x(),
            it->vertex(2)->point().y(),
            it->vertex(2)->point().z());

        saver.write_triangle(it->vertex(2)->point().x(),
            it->vertex(2)->point().y(),
            it->vertex(2)->point().z(),
            it->vertex(1)->point().x(),
            it->vertex(1)->point().y(),
            it->vertex(1)->point().z(),
            it->vertex(3)->point().x(),
            it->vertex(3)->point().y(),
            it->vertex(3)->point().z());

        saver.write_triangle(it->vertex(3)->point().x(),
            it->vertex(3)->point().y(),
            it->vertex(3)->point().z(),
            it->vertex(1)->point().x(),
            it->vertex(1)->point().y(),
            it->vertex(1)->point().z(),
            it->vertex(0)->point().x(),
            it->vertex(0)->point().y(),
            it->vertex(0)->point().z());

        saver.write_triangle(it->vertex(0)->point().x(),
            it->vertex(0)->point().y(),
            it->vertex(0)->point().z(),
            it->vertex(3)->point().x(),
            it->vertex(3)->point().y(),
            it->vertex(3)->point().z(),
            it->vertex(2)->point().x(),
            it->vertex(2)->point().y(),
            it->vertex(2)->point().z());
        */

        saver.write_tetrahedron(it->vertex(0)->point().x(),
            it->vertex(0)->point().y(),
            it->vertex(0)->point().z(),
            it->vertex(1)->point().x(),
            it->vertex(1)->point().y(),
            it->vertex(1)->point().z(),
            it->vertex(2)->point().x(),
            it->vertex(2)->point().y(),
            it->vertex(2)->point().z(),
            it->vertex(3)->point().x(),
            it->vertex(3)->point().y(),
            it->vertex(3)->point().z());

        if (split_layers)
            saver.add_layer();
    }

    saver.end();

    return 0;
}
