#pragma once
#include<gl\glew\glew.h>
#include<gl\glm\gtc\type_ptr.hpp>
#include<gl\glm\gtx\transform.hpp>
//struct vertex
//{
//	sf::Vector3f position;
//	sf::Vector3f color;
//	vertex(sf::Vector3f position = sf::Vector3f(), sf::Vector3f color = sf::Vector3f()) :position(position), color(color) {}
//};

struct vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	vertex() :position(), normal(), uv() {}
	vertex(glm::vec3 pos, glm::vec3 norm) :position(pos), normal(norm) {}
	vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 _uv) :position(pos), normal(norm), uv(_uv) {}
};

//void Log(char * text) {
//	std::cout << text;
//}