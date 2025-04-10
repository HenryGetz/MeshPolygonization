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
#include "solver/linear_program_solver.h"


inline std::vector<double> optimize(Mesh* mesh, std::vector<Plane_intersection>* edges, LinearProgramSolver::SolverName solver_name) {
	// Face attributes //
	// Face index
	Mesh::Property_map<Face, std::size_t> face_indices = mesh->property_map<Face, std::size_t>("f:index").value();

	// Number of supporting faces
	Mesh::Property_map<Face, std::size_t> supporting_face_num = mesh->property_map<Face, std::size_t>("f:supporting_face_num").value();

	// Covered area
	Mesh::Property_map<Face, double> covered_area = mesh->property_map<Face, double>("f:covered_area").value();

	// Total area
	Mesh::Property_map<Face, double> area = mesh->property_map<Face, double>("f:area").value();

	// Linear program coefficients
	double wt_fitting = 0.43;
	double wt_coverage = 0.27;
	double wt_complexity = 0.30;

	// Compute total number of supporting faces of the model
	double total_faces = 0.0;
	double total_area = 0.0;
	std::size_t idx = 0;
	for (auto f : mesh->faces()) {
		total_faces += supporting_face_num[f];
		total_area += area[f];
		face_indices[f] = idx;
		++idx;
	}

	// Binary variables:
	// x[0] ... x[num_faces - 1] : binary labels of all the input faces
	// x[num_faces] ... x[num_faces + num_edges - 1] : binary labels of all the intersecting edges (remain or not)

	// Determine variable number
	std::size_t num_faces = mesh->number_of_faces();
	std::size_t num_edges = edges->size();
	std::size_t total_variables = num_faces + num_edges + num_edges;

    LinearProgram program;
	// Add variable
	const std::vector<Variable*>& variables = program.create_n_variables(total_variables);
	for (std::size_t i = 0; i < total_variables; ++i) {
		Variable* v = variables[i];
		v->set_variable_type(Variable::BINARY);
	}

	// Proxy mesh bbox area
	Bbox_3 box = CGAL::Polygon_mesh_processing::bbox(*mesh);
	double dx = box.xmax() - box.xmin();
	double dy = box.ymax() - box.ymin();
	double dz = box.zmax() - box.zmin();
	double box_area = double(2.0) * (dx * dy + dy * dz + dz * dx);

	// Chooses a better scale for coefficients
	double coeff_data_fitting = wt_fitting / total_faces;
	double coeff_coverage = wt_coverage / box_area;
	double coeff_complexity = wt_complexity / double(edges->size());

	// Add objective: MINIMIZATION 
	LinearObjective* objective = program.create_objective(LinearObjective::MINIMIZE);

	for (auto f : mesh->faces()) {
		std::size_t var_idx = face_indices[f];

		// Accumulates data fitting term
		double num = supporting_face_num[f];
		objective->add_coefficient(var_idx, -coeff_data_fitting * num);

		// Accumulates model coverage term
		double uncovered_area = (area[f] - covered_area[f]);
		objective->add_coefficient(var_idx, coeff_coverage * uncovered_area);
	}

	std::size_t num_sharp_edges = 0;
	for (std::size_t i = 0; i < edges->size(); ++i) {
		std::size_t var_idx = num_faces + num_edges + num_sharp_edges;

		// Accumulates model complexity term
		objective->add_coefficient(var_idx, coeff_complexity);
		++num_sharp_edges;
	}

	// Adds constraints: the number of faces associated with an edge must be either 2 or 0
	std::size_t var_edge_used_idx = 0;
	for (std::size_t i = 0; i < edges->size(); i++) {
		LinearConstraint* c = program.create_constraint(LinearConstraint::FIXED, 0.0, 0.0);
		auto fan = (*edges)[i].fan;
		for (std::size_t j = 0; j < fan.size(); ++j) {
			auto f = fan[j];
			std::size_t var_idx = face_indices[f];
			c->add_coefficient(var_idx, 1.0);
		}

		// If edge is adjacent to less than 2 faces, delete them
		// If edge is adjacent to more than 2 faces, choose two of them
		if (fan.size() >= 2) {
			std::size_t var_idx = num_faces + var_edge_used_idx;
			c->add_coefficient(var_idx, -2.0);
			++var_edge_used_idx;
		}
	}

	// Optimization
	std::vector<double> X;

    LinearProgramSolver solver;
	if (solver.solve(&program, solver_name)) {
		// Marks results
		X = solver.solution();
	}
	return X;
}