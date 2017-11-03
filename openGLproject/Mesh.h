#pragma once
#include<gl\glew\glew.h>
#include<gl\glm\gtc\type_ptr.hpp>
#include<gl\glm\gtx\transform.hpp>

#include<memory>
#include <vector>


#include "OUtilities.h"
#include "Camera.h"
#include "Shader.h"

class Mesh
{
private:
	GLuint vbo;
	GLuint ibo;

	std::shared_ptr<std::vector<vertex>> vertices;
	std::shared_ptr<std::vector<GLuint>> indices;
	std::shared_ptr< Shader> shader;

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec4 rotation;

	glm::mat4x4 model_mat;

	bool is_mode_mat_dirty;

	void update_model_mat();

	void unBindBuffers();

	static void Mesh::CreateTriangle(glm::vec3 p[3], glm::vec3 normals[3], std::vector<vertex>& vertices, std::vector<GLuint>& indices);
	static void CreateQuad(glm::vec3 p[4], glm::vec3 color, std::vector<vertex> &coreVertices, std::vector<GLuint> &indices);
	static void Mesh::divideTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, int iter, std::vector<vertex>& vertices, std::vector<GLuint>& indices);
public:
	Mesh() {};
	Mesh(std::shared_ptr<std::vector<vertex>> vertices, std::shared_ptr<std::vector<GLuint>> indices, std::shared_ptr<Shader> shader);
	~Mesh();

	static Mesh create_cube(std::shared_ptr< Shader> _shader);
	static Mesh create_sphere(std::shared_ptr< Shader> _shader,int iterations=3);

	void render(Camera *activeCamera);
	void render(glm::mat4 view_mat, glm::mat4 proj_mat, glm::vec3 cam_pos);

	void translate(float tx, float ty, float tz)
	{
		position = glm::vec3(position.x + tx, position.y + ty, position.z + tz);
		is_mode_mat_dirty = true;
	}
	void set_position(glm::vec3 _position)
	{
		position = _position;
		is_mode_mat_dirty = true;
	}
	glm::vec3 get_position() const
	{
		return position;
	}
	void set_scale(float sx, float sy, float sz)
	{
		scale = glm::vec3(scale.x*sx, scale.y*sy, scale.z*sz);
		is_mode_mat_dirty = true;
	}
	glm::vec3 get_scale()const
	{
		return scale;
	}
	void rotate(glm::float_t angle, glm::vec3 _direction)
	{
		if (_direction.x + _direction.y + _direction.z != 0) {
			rotation.x = _direction.x;
			rotation.y = _direction.y;
			rotation.z = _direction.z;
			rotation.w = angle;
			is_mode_mat_dirty = true;
		}
	}
	glm::vec4 get_euler_angles() const
	{
		return rotation;
	}
};

