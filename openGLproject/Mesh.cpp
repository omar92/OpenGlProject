#include "Mesh.h"

Mesh::Mesh(std::shared_ptr<std::vector<vertex>> vertices, std::shared_ptr<std::vector<GLuint>> indices, std::shared_ptr<Shader> shader)
{
	this->vertices = vertices;
	this->indices = indices;
	this->shader = shader;

	vbo = -1;
	ibo = -1;

	position = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	rotation = glm::vec4(0, 0, 0, 0);
	is_mode_mat_dirty = true;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*vertices->size(), vertices->data(), GL_STATIC_DRAW);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*indices->size(), indices->data(), GL_STATIC_DRAW);
	unBindBuffers();
}

Mesh::~Mesh()
{
}


void Mesh::render(glm::mat4 view_mat, glm::mat4 proj_mat, glm::vec3 cam_pos)
{
	//update metrixies
	GLuint viewMat_location = shader->get_uniform_loc("viewMat");
	GLuint projMat_location = shader->get_uniform_loc("projMat");
	glUniformMatrix4fv(viewMat_location, 1, GL_FALSE, glm::value_ptr(view_mat));
	glUniformMatrix4fv(projMat_location, 1, GL_FALSE, glm::value_ptr(proj_mat));
	if (is_mode_mat_dirty) {
		update_model_mat();
		is_mode_mat_dirty = false;
	}
	//draw
	shader->use();
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	GLuint vertex_position_location = shader->get_attr_loc("vertex_position");
	glVertexAttribPointer(vertex_position_location, 3, GL_FLOAT, false, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(vertex_position_location);
	GLuint vertex_normal_location = shader->get_attr_loc("vertex_normal");
	glVertexAttribPointer(vertex_normal_location, 3, GL_FLOAT, false, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	glEnableVertexAttribArray(vertex_normal_location);

	glDrawElements(GL_TRIANGLES, indices->size(), GL_UNSIGNED_INT, NULL);
	unBindBuffers();
}

void Mesh::render(Camera * activeCamera)
{
	render(activeCamera->get_view(), activeCamera->get_proj(), activeCamera->get_position());
}


void Mesh::update_model_mat()
{
	if (rotation.x + rotation.y + rotation.z != 0) {
		model_mat = glm::translate(position)*
			glm::rotate(rotation.w * 180 / 3.14f, glm::vec3(rotation.x, rotation.y, rotation.z))*
			glm::scale(scale);
	}
	else {
		model_mat = glm::translate(position)*
			glm::scale(scale);
	}

	glUniformMatrix4fv(shader->get_uniform_loc("modelMat"), 1, GL_FALSE, glm::value_ptr(model_mat));
}

void Mesh::unBindBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, -1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, -1);
}

void Mesh::CreateTriangle(glm::vec3 p[3], glm::vec3 normals[3], std::vector<vertex>& vertices, std::vector<GLuint>& indices) {
	for (int pointI = 0; pointI < 3; pointI++)
	{
		int index = -1;
		for (int i = 0; i < vertices.size(); i++)
		{
			if (vertices[i].position == p[pointI] && vertices[i].normal == normals[pointI]) {
				index = i;
			}
		}

		if (index < 0) {
			vertices.push_back({ glm::vec3(p[pointI]),normals[pointI] });
			index = vertices.size() - 1;
		}
		indices.push_back(index);
	}
}

void Mesh::CreateQuad(glm::vec3 p[4], glm::vec3 color, std::vector<vertex>& vertices, std::vector<GLuint>& indices) {
	glm::vec3 t1Points[] = { p[0],p[1],p[2] };
	glm::vec3 normal = glm::cross((p[2] - p[1]), (p[0] - p[1]));
	glm::vec3 t1Normals[] = { normal,normal,normal };
	CreateTriangle(t1Points, t1Normals, vertices, indices );

	glm::vec3 t2Points[] = { p[0],p[2],p[3] };
	normal = glm::cross((p[3] - p[2]), (p[0] - p[2]));
	glm::vec3 t2Normals[] = { normal,normal,normal };
	CreateTriangle(t2Points, t2Normals, vertices, indices);
}

Mesh Mesh::create_cube(std::shared_ptr<Shader> _shader) {
	auto vertices = std::shared_ptr<std::vector<vertex>>(new std::vector<vertex>());
	auto indices = std::shared_ptr<std::vector<GLuint>>(new std::vector<GLuint>());

	glm::vec3 cubeP[8] = 
	{
		glm::vec3(-0.5f,0.5f,0.5f),
		glm::vec3(-0.5f,-0.5f,0.5f),
		glm::vec3(0.5f,-0.5f,0.5f),
		glm::vec3(0.5f,0.5f,0.5f),
		glm::vec3(0.5f,0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f,0.5f,-0.5f),
	};
	glm::vec3 front[] = { cubeP[0],cubeP[1],cubeP[2],cubeP[3] };
	CreateQuad(front, glm::vec3(0, 0, 0), *vertices, *indices);

	glm::vec3 right[] = { cubeP[3],cubeP[2],cubeP[5],cubeP[4] };
	CreateQuad(right, glm::vec3(0, 0, 1), *vertices, *indices);

	glm::vec3 back[] = { cubeP[4],cubeP[5],cubeP[6],cubeP[7] };
	CreateQuad(back, glm::vec3(0, 1, 0), *vertices, *indices);

	glm::vec3 left[] = { cubeP[7],cubeP[6],cubeP[1],cubeP[0] };
	CreateQuad(left, glm::vec3(0, 1, 1), *vertices, *indices);

	glm::vec3 top[] = { cubeP[7],cubeP[0],cubeP[3],cubeP[4] };
	CreateQuad(top, glm::vec3(1, 0, 0), *vertices, *indices);

	glm::vec3 bottom[] = { cubeP[2],cubeP[1],cubeP[6],cubeP[5] };
	CreateQuad(bottom, glm::vec3(1, 0, 1), *vertices, *indices);

	return Mesh(vertices, indices, _shader);
}

void Mesh::divideTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, int iter, std::vector<vertex>& _vertices, std::vector<GLuint>& _indices)
{
	//std::cout<<"devide "<< iter<<std::endl;
	if (iter > 0)
	{
		//make new points at sphere surface 
		glm::vec3 v1 = glm::normalize((a + b) / 2.0f);
		glm::vec3 v2 = glm::normalize((b + c) / 2.0f);
		glm::vec3 v3 = glm::normalize((c + a) / 2.0f);

		divideTriangle(a, v1, v3, iter - 1, _vertices, _indices);
		divideTriangle(v1, b, v2, iter - 1, _vertices, _indices);
		divideTriangle(v1, v2, v3, iter - 1, _vertices, _indices);
		divideTriangle(v3, v2, c, iter - 1, _vertices, _indices);

	}
	else
	{
		glm::vec3 tPoints[] = {
			glm::vec3(a.x,a.y,a.z),
			glm::vec3(b.x,b.y,b.z),
			glm::vec3(c.x,c.y,c.z),
		};
		CreateTriangle(tPoints, tPoints, _vertices, _indices);
	}
}

Mesh Mesh::create_sphere(std::shared_ptr<Shader> _shader, int iterations)
{
	auto _vertices = std::shared_ptr<std::vector<vertex>>(new std::vector<vertex>());
	auto _indices = std::shared_ptr<std::vector<GLuint>>(new std::vector<GLuint>());

	glm::vec3 prism[] =
	{
		glm::vec3(0,0,1),
		glm::vec3(0,0.942809f,-0.333333f),
		glm::vec3(-0.816497f,-0.471405f,-0.333333f),
		glm::vec3(0.816497f,-0.471405f,-0.333333f)
	};

	divideTriangle(prism[0], prism[1], prism[2], iterations, *_vertices, *_indices);
	divideTriangle(prism[0], prism[3], prism[1], iterations, *_vertices, *_indices);
	divideTriangle(prism[0], prism[2], prism[3], iterations, *_vertices, *_indices);
	divideTriangle(prism[3], prism[2], prism[1], iterations, *_vertices, *_indices);

	return Mesh(_vertices, _indices, _shader);
}
