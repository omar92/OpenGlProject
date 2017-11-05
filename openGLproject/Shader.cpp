#include "Shader.h"

Shader::~Shader()
{
}

char * Shader::readFile(const char * fileName)
{
	std::ifstream shaderfile(fileName, std::ios::ate);//ate-> at the end
	if (!shaderfile.is_open())
	{
		std::cerr << "OpenFileError File:" << fileName;
		return NULL;
	}
	std::streampos size = shaderfile.tellg();
	char* filedata = new char[size];
	for (int i = 0; i < size; i++)
		filedata[i] = 0;
	shaderfile.seekg(std::ios::beg);
	shaderfile.read(filedata, size);
	shaderfile.close();
	return filedata;
}

bool Shader::compiledStatus(GLint ShaderID)
{
	GLint compiled = 0;
	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &compiled);
	if (compiled)
		return true;
	else
	{
		GLint logLength;
		glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &logLength);
		char* msgBuffer = new char[logLength];
		glGetShaderInfoLog(ShaderID, logLength, NULL, msgBuffer);
		printf("%s\n", msgBuffer);
		delete(msgBuffer);
		return false;
	}
}

GLuint Shader::makeShader(const char * ShaderSourceCode, GLenum Shader_TYPE)
{
	GLuint vertShaderID = glCreateShader(Shader_TYPE);
	glShaderSource(vertShaderID, 1, (const GLchar**)&ShaderSourceCode, NULL);
	glCompileShader(vertShaderID);
	bool compiledCorrectly = compiledStatus(vertShaderID);
	if (compiledCorrectly)

		return vertShaderID;
	else
		return -1;
}

GLuint Shader::makeShaderProgram(GLuint vertShaderID, GLuint fragShaderID)
{
	GLuint shaderID = glCreateProgram();
	glAttachShader(shaderID, vertShaderID);
	glAttachShader(shaderID, fragShaderID);
	glLinkProgram(shaderID);
	return shaderID;
}

GLuint Shader::InitShader(const char * vertex_shader_file_name, const char * fragment_shader_file_name, const char * texture_file_name)
{
	GLuint shaderProgramID = -1;
	GLuint vertexShaderID = -1;
	GLuint fragmentShaderID = -1;

	char* vertexShaderSourceCode = readFile(vertex_shader_file_name);
	char* fragmentShaderSourceCode = readFile(fragment_shader_file_name);
	vertexShaderID = makeShader(vertexShaderSourceCode, GL_VERTEX_SHADER);
	fragmentShaderID = makeShader(fragmentShaderSourceCode, GL_FRAGMENT_SHADER);
	printf("Vertex Shader ID:%d", vertexShaderID);
	printf("\nFragment Shader ID:%d", fragmentShaderID);
	if (vertexShaderID != -1 && fragmentShaderID != -1) {
		shaderProgramID = makeShaderProgram(vertexShaderID, fragmentShaderID);
		printf("\nshaderProgramID:%d", shaderProgramID);
	}
	else
	{
		printf("\makeShaderProgram Faild :\n");
	}

	glEnable(GL_DEPTH_TEST);

	return shaderProgramID;
}



GLuint Shader::get_uniform_loc(const std::string & uniform)
{
	if (uniforms.find(uniform) == uniforms.end()) {
		uniforms[uniform] = glGetUniformLocation(Shader_program_id, &uniform[0]);
	}
	return uniforms[uniform];
}

GLuint Shader::get_attr_loc(const std::string & Attrib)
{
	if (Attribs.find(Attrib) == Attribs.end()) {
		Attribs[Attrib] = glGetAttribLocation(Shader_program_id, &Attrib[0]);
	}
	return Attribs[Attrib];
}

Shader::Shader(GLuint shaderProgramID, GLuint _textureID)
{
	Shader_program_id = shaderProgramID;
	textureID = _textureID;
	//InitShader(vertex_Shader_filename, fragment_Shader_filename);
}
std::shared_ptr<Shader> Shader::LoadFromFile(const char * vertex_Shader_filename, const char * fragment_Shader_filename, const char * texture_file_name)
{
	GLuint Shader_program_id = InitShader(vertex_Shader_filename, fragment_Shader_filename, texture_file_name);
	if (Shader_program_id != -1) {

		sf::Image image;
		if (!image.loadFromFile(texture_file_name))
		{
			std::cout << "error loading image!" << std::endl;
			if (!image.loadFromFile("defult.png"))
			{
				std::cout << "error loading image!" << std::endl;
			}
		}
		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		std::shared_ptr<Shader> sPtr = std::shared_ptr<Shader>(new Shader(Shader_program_id, textureID));
		return sPtr;
	}
	else {
		return nullptr;
	}
}
