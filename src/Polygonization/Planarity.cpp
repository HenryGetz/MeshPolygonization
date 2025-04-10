/**
 * MeshPolygonization is the implementation of the MVS (Multi-view Stereo)
 * building mesh simplification method described in the following paper:
 *      Vasileios Bouzas, Hugo Ledoux, and  Liangliang Nan.
 *      Structure-aware Building Mesh Polygonization.
 *      ISPRS Journal of Photogrammetry and Remote Sensing. 167(2020), 432-442,
 * 2020. Please cite the above paper if you use the code/program (or part of
 * it).
 *
 * LICENSE:
 *      MeshPolygonization is free for academic use. If you are interested in a
 * commercial license please contact the 3D Geoinformation group.
 *
 * Copyright (C) 2019 3D Geoinformation Research Group
 * https://3d.bk.tudelft.nl/
 */

#include "Planarity.h"
#include <omp.h>

Planarity::Planarity() {}

Planarity::~Planarity() {}

void Planarity::compute(Mesh *mesh, unsigned int num_rings) {
  // Create planarity attribute (default init to -9999)
  VProp_double planarity =
      mesh->add_property_map<Vertex, double>("v:planarity", -9999).first;

  // Gather all vertices into a vector for indexed access
  std::vector<Vertex> verts;
  verts.reserve(mesh->number_of_vertices());
  for (auto v : mesh->vertices())
    verts.push_back(v);

  // Parallel compute planarity per vertex
#pragma omp parallel for schedule(dynamic)
  for (int i = 0; i < (int)verts.size(); ++i) {
    planarity[verts[i]] = compute_k_ring_planarity(mesh, verts[i], num_rings);
  }

  // Assign planarity to faces (also parallelized below)
  planarity_to_faces(mesh);
}

// K-Ring Neighborhood Planarity
double Planarity::compute_k_ring_planarity(const Mesh *mesh,
                                           const Vertex vertex,
                                           unsigned int k) {
  // Collect k-ring neighbors
  std::set<Vertex> vertices = get_k_ring_vertices(mesh, vertex, k);

  // Retrieve geometry of neighbors
  std::vector<Point_3> points;
  VProp_geom geom = mesh->points();
  for (auto v : vertices) {
    points.push_back(geom[v]);
  }

  // Calculate planarity
  Plane_3 plane;
  double planarity = linear_least_squares_fitting_3(
      points.begin(), points.end(), plane, CGAL::Dimension_tag<0>());

  return planarity;
}

// Assign planarity to faces
void Planarity::planarity_to_faces(Mesh *mesh) {
  // Retrieve planarity property for vertices
  VProp_double v_planar =
      mesh->property_map<Vertex, double>("v:planarity").value();

  // Create planarity property for faces (initialized to 0)
  FProp_double f_planar =
      mesh->add_property_map<Face, double>("f:planarity", 0.0).first;

  // Gather faces into vector for indexed access
  std::vector<Face> faces;
  faces.reserve(mesh->number_of_faces());
  for (auto f : mesh->faces())
    faces.push_back(f);

  // Parallel assign per‑face planarity
#pragma omp parallel for schedule(dynamic)
  for (int i = 0; i < (int)faces.size(); ++i) {
    const Face face = faces[i];
    auto verts = vertex_around_face(mesh, face);
    double sum = 0.0;
    for (auto v : verts)
      sum += v_planar[v];
    f_planar[face] = sum / verts.size();
  }
}