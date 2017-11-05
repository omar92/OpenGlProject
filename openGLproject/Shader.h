#pragma once
#include <map>
#include<gl\glew\glew.h>
#include<fstream>
#include<iostream>
#include<memory>
#include <SFML/Graphics.hpp>

class Shader
{
private:
	std::map<std::string, int> Attribs;
	std::map<std::string, int> uniforms;

	static char* readFile(const char* fileName);
	static bool compiledStatus(GLint ShaderID);
	static GLuint makeShader(const char* OhaderSourceCode, GLenum Shader_TYPE);
	static GLuint makeShaderProgram(GLuint vertShaderID, GLuint fragShaderID);
	static GLuint InitShader(const char *vertex_Shader_file_name, const  char *fragment_Shader_file_name, const  char * texture_file_name);

	const char * texture_file_name;

	//GLuint vertex_Shader_id;
	//GLuint fragment_Shader_id;
	GLuint Shader_program_id;
	GLuint textureID;
public:
	Shader(GLuint shaderProgramID);
	static std::shared_ptr<Shader> LoadFromFile(const char *  vertex_Shader_filename, const char *  fragment_Shader_filename, const char * texture_file_name="");
	GLuint get_uniform_loc(const std::string &uniform);
	GLuint get_attr_loc(const std::string &Attrib);
	void use() const
	{
		glUseProgram(Shader_program_id);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}
	GLuint getProgramID()const { return Shader_program_id; }
	~Shader();
};


//class usage:

/*
std::shared_ptr<Shader> ptr = Shader::loadShader("", "");
if (ptr)
{
ptr->getProgramID();
ptr->use();
ptr->getUniformLocation("theta");
getAttribLocation("vertex_position");

}*/

