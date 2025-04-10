/**
 * MeshPolygonization is the implementation of the MVS (Multi-view Stereo) building mesh simplification method
 * described in the following paper:
 *      Vasileios Bouzas, Hugo Ledoux, and  Liangliang Nan.
 *      Structure-aware Building Mesh Polygonization.
 *      ISPRS Journal of Photogrammetry and Remote Sensing. 167(2020), 432-442, 2020.
 * Please cite the above paper if you use the code/program (or part of it).
 *
 * LICENSE:
 *      MeshPolygonization is free for academic use. If you are interested in a commercial license please contact
 *      the 3D Geoinformation group.
 *
 * Copyright (C) 2019 3D Geoinformation Research Group
 * https://3d.bk.tudelft.nl/
 */

#pragma once

#include "Utils.h"


// Select segment by id
inline std::set<Face> select_segment(const Mesh* mesh, unsigned int id) {
	std::set<Face> segment;

	FProp_int chart = mesh->property_map<Face, int>("f:chart").value();
	for (auto face : mesh->faces()) {
		if (chart[face] == id) { segment.insert(face); }
	}

	return segment;
}


// Retrieve segment color
inline Point_3 get_segment_color(const Mesh* mesh, unsigned int id) {
	FProp_int chart = mesh->property_map<Face, int>("f:chart").value();
	FProp_color color = mesh->property_map<Face, Point_3>("f:color").value();
	for (auto face : mesh->faces()) {
		if (chart[face] == id) { return color[face]; }
	}

	return Point_3(0, 0, 0);
}


// Compute segment orientation
inline Vector_3 compute_segment_orientation(const Mesh* mesh, unsigned int id) {
	// Select segment by id
	std::set<Face> segment = select_segment(mesh, id);

	FProp_double planarity = mesh->property_map<Face, double>("f:planarity").value();
	auto max_face = std::max_element(segment.begin(), segment.end(),
		                             [&](const Face &a, const Face &b)
	                                 {return planarity[a] < planarity[b]; });

	Vector_3 normal = CGAL::Polygon_mesh_processing::compute_face_normal(*max_face, *mesh);
	return normal;
}


// Retrieve segment vertices
inline std::set<Vertex> get_segment_vertices(const Mesh* mesh, unsigned int id) {
	std::set<Vertex> vertices;

	// Select segment by id
	std::set<Face> segment = select_segment(mesh, id);

	// Iterate faces
	std::vector<Vertex> new_vertices;
	for (auto face : segment) {
		// Retrieve face vertices
		new_vertices = vertex_around_face(mesh, face);
		vertices.insert(new_vertices.begin(), new_vertices.end());
	}

	return vertices;
}


// Retrieve segment edges
inline std::vector<Segment_3> get_segment_edges(const Mesh* mesh, unsigned int id) {
	std::vector<Segment_3> edges;

	// Select segment by id
	std::set<Face> segment = select_segment(mesh, id);

	// Iterate faces
	Point_3 source, target;
	VProp_geom geom = mesh->points();
	for (auto face : segment) {
		Halfedge hf = mesh->halfedge(face);
		for (Halfedge h : halfedges_around_face(hf, *mesh)) {
			source = geom[mesh->source(h)];
			target = geom[mesh->target(h)];
			edges.push_back(Segment_3(source, target));
		}
	}

	return edges;
}


// Retrieve interior points
inline std::vector<Point_3> get_interior_points(const Mesh* mesh, unsigned int id) {
	std::vector<Point_3> interior;

	// Retrieve segment vertices
	std::set<Vertex> vertices = get_segment_vertices(mesh, id);

	// Retrieve points
	std::vector<Face> neighbors;
	FProp_int chart = mesh->property_map<Face, int>("f:chart").value();
	VProp_geom geom = mesh->points();
	for (auto vertex : vertices) {
		bool is_in = true;

		// Retrieve neighboring vertices
		neighbors = face_around_vertex(mesh, vertex);

		// Check neighbors
		for (auto neighbor : neighbors) {
			if (chart[neighbor] != id) { is_in = false; break; }
		}

		// Add point
		if (is_in) { interior.push_back(geom[vertex]); }
	}

	return interior;
}


// Retrieve segment border
inline std::vector<Segment_3> get_segment_border(const Mesh* mesh, unsigned int id) {
	Segment_3 edge;
	std::vector<Segment_3> border;

	// Select segment by id
	std::set<Face> segment = select_segment(mesh, id);

	// Chart property
	FProp_int chart = mesh->property_map<Face, int>("f:chart").value();
	VProp_geom geom = mesh->points();

	// Iterate segment faces
	Face opp_face;
	for (auto face : segment) {
		// Halfedfge around target circulator
		Halfedge hf = mesh->halfedge(face);
		for (Halfedge h : halfedges_around_face(hf, *mesh)) {
			opp_face = mesh->face(mesh->opposite(h));
			if (opp_face != mesh->null_face() && chart[opp_face] != id) {
				edge = Segment_3(geom[mesh->source(h)], geom[mesh->target(h)]);
				border.push_back(edge); 
			}
		}
	}

	return border;
}


// Retrieve segment centroid
inline Point_3 get_segment_centroid(const Mesh* mesh, unsigned int id) {
	Point_3 centroid;

	// Collect segment vertices
	std::set<Vertex> vertices = get_segment_vertices(mesh, id);

	// Collect segment points
	std::vector<Point_3> points; 
	VProp_geom geom = mesh->points();
	for (auto vertex : vertices) {
		points.push_back(geom[vertex]);
	}


	// Compute centroid
	centroid = CGAL::centroid(points.begin(), points.end(), CGAL::Dimension_tag<0>());

	return centroid;
}
