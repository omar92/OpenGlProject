#pragma comment(lib,"sfml-graphics.lib" )
#pragma comment(lib,"sfml-window.lib" )
#pragma comment(lib,"sfml-system.lib" )
#pragma comment(lib,"opengl32.lib" )
#pragma comment(lib,"glew32.lib")

#include <gl\glew\glew.h>

#include<gl\glm\glm.hpp>
#include<gl\glm\gtc\type_ptr.hpp>
#include<gl\glm\gtx\transform.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <iostream>
#include <fstream>
#include <memory>

#include "Window.h"
#include "OUtilities.h"
#include "Shader.h"
#include "Mesh.h"
#include "Camera.h"

#define  SCREEN_WIDTH 500//1366	
#define  SCREEN_Height 500//768


void onHover(sf::Vector2i cPos);
void onStart(WindowHandler &uim);
void onUpdate(WindowHandler &uim);
void onRender(WindowHandler &uim);
glm::vec2 convertMousePos(glm::vec2 pos);
std::shared_ptr<Shader> shader;
std::shared_ptr<std::vector<vertex>> coreVertices;
std::shared_ptr<std::vector<GLuint>> cubeindices;
Camera* activeCamera;
Mesh mesh;
Mesh mesh2;
float theta = 0;

int main()
{
	auto window = WindowHandler::createWindow(sf::Vector2i(SCREEN_WIDTH, SCREEN_Height));
	window->onStart = onStart;
	window->onUpdate = onUpdate;
	window->onHover = onHover;
	window->onRender = onRender;
	shader = Shader::LoadFromFile("texture_vs.glsl", "texture_fs.glsl");
	//shader->use();
	window->startDisplay();
}	

void onStart(WindowHandler & uim)
{
	activeCamera = new Camera(PROJECTION_TYPE::PERSPECTIVE, glm::vec2(SCREEN_WIDTH, SCREEN_Height));
	activeCamera->set_position(glm::vec3(0, 1, 5));
	activeCamera->set_fov(50);
	activeCamera->lookAt(glm::vec3(0, 0, 0));
	mesh = Mesh::create_cube(shader);
	mesh2 = Mesh::create_sphere(shader,5);
	mesh.translate(0, 0, 0);
}

void onUpdate(WindowHandler & uim)
{
	theta += 0.001f;
	//	mesh.rotate(theta, glm::vec3(0, 1, 0));
	mesh2.rotate(-theta, glm::vec3(0, 1, 0));
}

void onRender(WindowHandler & uim)
{
//	mesh.render(activeCamera);
	mesh2.render(activeCamera);
}


void onHover(sf::Vector2i cPos)
{
	auto p = convertMousePos(glm::vec2(cPos.x, cPos.y));
	activeCamera->lookAt(glm::vec3(p.x, p.y, 0.0f));
}


glm::vec2 convertMousePos(glm::vec2 pos) {
	//printf("\n(%f,%f):\t", pos.x, pos.y);
	pos.x = -1 + ((pos.x * 2) / SCREEN_WIDTH);
	pos.y = 1 - ((pos.y * 2) / SCREEN_Height);
	return pos;
}
