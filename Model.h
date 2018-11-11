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
	void render(const Shader& shader);
	void renderOffScreen(const Shader& colorShader);

	void calNormal();
	void calTexCoord(int sec);
	void dijkstra(int sec);
	Eigen::Vector3d ProjectOnPlane(Eigen::Vector3d vp, Eigen::Vector3d vn, Eigen::Vector3d vq);

	void setPosition(const glm::vec3& pos) { this->position = pos; }
	void setScale(const glm::vec3& scale) { this->scale = scale; }
	void setRotate(const float angle, const glm::vec3& axis);

	glm::vec3 getPosition()const { return position; }
	glm::vec3 getScale()const { return scale; }
	glm::mat4 getRotate()const { return rotate; }

public:
	Eigen::Matrix3Xd V;
	Eigen::Matrix3Xi F;
	Eigen::Matrix3Xd N;
	Eigen::Matrix2Xd T;
	Eigen::Matrix2Xd T_PolarCoord;
private:
	vector<Mesh> meshes;

	glm::vec3 position;
	glm::vec3 scale;
	glm::mat4 rotate;

	vector<vector<double>> graph;
	vector<double> d;
	vector<int> p;
};