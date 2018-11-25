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
	Model(std::string,int* sec=nullptr,int length=0);
	void render(const Shader& shader);
	void renderOffScreen(const Shader& colorShader);

	void calNormal();
	void calTexCoord_DEM(int sec);
	void calTexCoord_SP(std::vector<int> sec);

	void setPosition(const glm::vec3& pos) { this->_position = pos; }
	void setScale(const glm::vec3& scale) { this->_scale = scale; }
	void setRotate(const float angle, const glm::vec3& axis);

	glm::vec3 getPosition()const { return _position; }
	glm::vec3 getScale()const { return _scale; }
	glm::mat4 getRotate()const { return _rotate; }

private:
	void buildGraph();
	void dijkstra_DEM(int sec);
	void dijkstra_SP(std::vector<int> sec);
	Eigen::Vector3d ProjectOnPlane(Eigen::Vector3d vp, Eigen::Vector3d vn, Eigen::Vector3d vq);
public:
	Eigen::Matrix3Xd V;
	Eigen::Matrix3Xi F;
	Eigen::Matrix3Xd N;
	Eigen::Matrix2Xd T;
	Eigen::Matrix2Xd T_PolarCoord;
private:
	glm::vec3 _position;
	glm::vec3 _scale;
	glm::mat4 _rotate;

	vector<Mesh> _meshes;
	vector<vector<double>> _graph;
};