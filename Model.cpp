#include "Model.h"
#include <glad/glad.h>
#include <jy_mesh/surface_mesh_helper.h>
#include <surface_mesh/Surface_mesh.h>
#define _USE_MATH_DEFINES
#include <math.h>


Model::Model(std::string path,int sec)
{
	if (!mesh_io_helper::read_obj(path, V, F)) {
		std::cerr << "read src or target obj failure!" << std::endl;
	}
	position = glm::vec3(0.0, 0.0, 0.0);
	rotate = glm::mat4();
	scale = glm::vec3(1.0, 1.0, 1.0);

	//init
	N.setZero(3, V.cols());
	T.setZero(2, V.cols());
	T_PolarCoord.setZero(2, V.cols());
	graph.resize(V.cols());
	for (auto& item : graph) {
		item.resize(V.cols(), std::numeric_limits<double>::max());
	}

	calNormal();

	if (sec != -1)
		calTexCoord_DEM(sec);
		//calTexCoord_SP(sec);

	vector<Vertex> mesh;
	for (int i = 0; i != F.cols(); ++i) {
		auto col = F.col(i);
		for (int j = 0; j != 3; ++j) {
			int index = col(j);
			auto v_pos = V.col(index);
			auto v_norm = N.col(index);
			auto v_tex = T.col(index);

			Vertex vertex;
			vertex.Position = glm::vec3(v_pos(0), v_pos(1), v_pos(2));
			vertex.Normal = glm::vec3(v_norm(0), v_norm(1), v_norm(2));
			vertex.TexCoord = glm::vec2(v_tex(0), v_tex(1));

			mesh.push_back(vertex);
		}
	}
	meshes.push_back(mesh);
}
void Model::calNormal()
{
	for (size_t i = 0; i < F.cols(); ++i) {
		const Eigen::Vector3d &v1 = V.col(F(1, i)) - V.col(F(0, i));
		const Eigen::Vector3d &v2 = V.col(F(2, i)) - V.col(F(0, i));
		const Eigen::Vector3d n = v1.cross(v2);
		for (size_t j = 0; j < 3; ++j) {
			N.col(F(j, i)) += n;
		}
	}
	for (size_t i = 0; i < N.cols(); ++i) {
		N.col(i).normalize();
	}
}
void Model::calTexCoord_DEM(int sec) 
{
	buildGraph();

	dijkstra_DEM(sec);
	
	calTexCoord(sec);
}


void Model::dijkstra_DEM(int sec)
{
	Eigen::Vector3d p_pos = V.col(sec);
	Eigen::Vector3d p_norm = N.col(sec);
	p_norm.normalize();
	Eigen::Vector3d p_e1=ProjectOnPlane(p_pos, p_norm, Eigen::Vector3d(1, 0, 0));
	p_e1.normalize();

	std::vector<int>vis;
	vis.resize(V.cols(), 0);

	vector<double> d;
	vector<int> p;
	d.resize(V.cols(), std::numeric_limits<double>::max());
	p.resize(V.cols(), -1);

	for (int i = 0; i<V.cols(); i++)
	{
		d[i] = graph[sec][i];
	}
	vis[sec] = 1; 
	d[sec] = 0;
	p[sec] = -1;

	for (int i = 1; i<V.cols(); i++)
	{
		double min = std::numeric_limits<double>::max();
		int min_num;
		for (int j = 0; j<V.cols(); j++)
		{
			if (!vis[j] && d[j] < min)
			{
				min = d[j];
				min_num = j;
			}
		}
		vis[min_num] = 1;
		if (p[min_num] == -1) {
			int q_index = min_num;
			Eigen::Vector3d q_pos = V.col(q_index);
			double geodesic_dis = (q_pos - p_pos).norm();
			Eigen::Vector3d	v= ProjectOnPlane(p_pos, p_norm, q_pos);
			v.normalize();
			double polar_angle = acos(p_e1.dot(v));
			Eigen::Vector3d c = p_e1.cross(v);
			if (c.dot(p_norm) < 0) {
				polar_angle = 2 * M_PI - polar_angle;
			}

			double x = geodesic_dis * cos(polar_angle);
			double y = geodesic_dis * sin(polar_angle);
			T_PolarCoord.col(q_index) = Eigen::Vector2d(x,y);
		}
		else {
			int r_index = min_num;
			int q_index = p[min_num];
			Eigen::Vector3d q_pos = V.col(q_index);
			Eigen::Vector3d q_norm = N.col(q_index);
			q_norm.normalize();
			Eigen::Vector3d r_pos = V.col(r_index);


			double geodesic_dis = (r_pos - q_pos).norm();

			Eigen::Vector3d	v = ProjectOnPlane(q_pos, q_norm, r_pos);
			v.normalize();
			Eigen::Vector3d q_e1 = ProjectOnPlane(q_pos, q_norm, Eigen::Vector3d(1, 0, 0));
			q_e1.normalize();
			double polar_angle = acos(q_e1.dot(v));//
			Eigen::Vector3d c = q_e1.cross(v);
			if (c.dot(q_norm) < 0) {
				polar_angle = 2 * M_PI - polar_angle;
			}
			double x = geodesic_dis * cos(polar_angle);
			double y = geodesic_dis * sin(polar_angle);
			Eigen::Vector2d Urq = Eigen::Vector2d(x, y);


			Eigen::AngleAxisd rotation_vector(acos(q_norm.dot(p_norm)), (q_norm.cross(p_norm)).normalized());
			Eigen::Isometry3d T = Eigen::Isometry3d::Identity();
			T.rotate(rotation_vector);                                        // 按照rotation_vector进行旋转
			T.pretranslate(Eigen::Vector3d(0, 0, 0));

			Eigen::Vector3d q_e1_transformed = T * q_e1;
			q_e1_transformed.normalize();

			double theta = acos(q_e1_transformed.dot(p_e1));
			c = p_e1.cross(q_e1_transformed);
			if (c.dot(p_norm) < 0) {
				theta = 2 * M_PI - theta;
			}
			Eigen::Matrix2d M;

			M << cos(theta), -sin(theta), sin(theta), cos(theta);

			if (Urq.cwiseAbs().maxCoeff() < 1e-6) T_PolarCoord.col(r_index) = T_PolarCoord.col(q_index);
			else T_PolarCoord.col(r_index) = T_PolarCoord.col(q_index) + M*Urq;
			//std::cout << T_PolarCoord.col(r_index) << std::endl;
		}
		for (int j = 0; j<V.cols(); j++)
		{
			if (!vis[j]&&d[j]>min + graph[min_num][j])
			{
				p[j] = min_num;
				d[j] = min + graph[min_num][j];
			}
		}
	}
}




void Model::calTexCoord_SP(int sec) 
{
	buildGraph();

	dijkstra_SP(sec);

	calTexCoord(sec);
}

void Model::dijkstra_SP(int sec)
{

}

void Model::buildGraph()
{
	for (int i = 0; i < F.cols(); ++i) {
		int index_0 = F(0, i);
		int index_1 = F(1, i);
		int index_2 = F(2, i);
		graph[index_0][index_1] = graph[index_1][index_0] = (V.col(index_0) - V.col(index_1)).norm();
		graph[index_1][index_2] = graph[index_2][index_1] = (V.col(index_1) - V.col(index_2)).norm();
		graph[index_2][index_0] = graph[index_0][index_2] = (V.col(index_2) - V.col(index_0)).norm();
	}
}

void Model::calTexCoord(int sec)
{
	for (int i = 0; i < V.cols(); ++i) {
		if (i == sec) {
			T.col(i) = Eigen::Vector2d(0.5, 0.5);
			continue;
		}
		Eigen::Vector2d uv = T_PolarCoord.col(i);
		double x = uv.x()*0.5;
		double y = uv.y()*0.5;

		T.col(i) = Eigen::Vector2d((0.5 + x), (0.5 + y));
		//std::cout << "(" << 0.5 + x << "," << 0.5 + y << ")" << std::endl;
	}
}

Eigen::Vector3d Model::ProjectOnPlane(Eigen::Vector3d p, Eigen::Vector3d n, Eigen::Vector3d q) {
	if (((q - p).norm()) == 0)return q;
	else return (q - p) - (q - p).dot(n) * n;
}

void Model::setRotate(const float angle, const glm::vec3& axis) {
	glm::mat4 temp;
	temp = glm::rotate(temp, angle, axis);
	rotate = temp * rotate;
}

void Model::render(const Shader& shader)
{
	glm::mat4 model;
	model = glm::translate(model, position);
	model *= rotate;
	model = glm::scale(model, scale);

	shader.use();
	shader.setMat4("model", model);
	for (auto mesh : meshes)
		mesh.render(shader);
}

void Model::renderOffScreen(const Shader& colorShader) {
	glm::mat4 model;
	model = glm::translate(model, position);
	model *= rotate;
	model = glm::scale(model, scale);

	colorShader.use();
	colorShader.setMat4("model", model);

	for (auto mesh : meshes)
		mesh.renderOffScreen(colorShader);
}