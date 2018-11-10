#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Eigen/Core>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include <jy_mesh/mesh_io.h>
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"
using std::vector;
using std::string;


class Model
{
public:
	Model(std::string,int sec=-1);
	void draw(const Shader& shader);

	void calNormal();
	void calTexCoord(int sec);
	void dijkstra(int sec);
	Eigen::Vector3d ProjectOnPlane(Eigen::Vector3d vp, Eigen::Vector3d vn, Eigen::Vector3d vq);

public:
	Eigen::Matrix3Xd V;
	Eigen::Matrix3Xi F;
	Eigen::Matrix3Xd N;
	Eigen::Matrix2Xd T;
	Eigen::Matrix2Xd T_PolarCoord;
private:
	vector<Mesh> meshes;
	vector<vector<double>> graph;
	vector<double> d;
	vector<int> p;
};