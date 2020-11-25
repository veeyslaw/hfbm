#include "Shader.h"
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>

Shader::Shader(std::string_view vertexShaderFilename, std::string_view fragmentShaderFilename) {
	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexShaderFilename);
	GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentShaderFilename);

	linkProgram(vertexShader, fragmentShader);

	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
}

Shader::~Shader() {
	glDeleteProgram(program);
}

void Shader::use() {
	glUseProgram(program);
}

void Shader::unuse() {
	glUseProgram(0);
}

void Shader::setMatrix4fv(const glm::fmat4& matrix, std::string_view name) {
	use();
	
	glUniformMatrix4fv(glGetUniformLocation(program, name.data()), 1, GL_FALSE, glm::value_ptr(matrix));

	unuse();
}

std::string getShaderSource(std::string_view filename) {
	std::string source;

	std::ifstream ifs(filename.data());

	ifs.seekg(0, std::ios::end);
	source.resize(ifs.tellg());
	ifs.seekg(0);

	ifs.read(source.data(), source.size());

	return source;
}

GLuint Shader::loadShader(GLenum type, std::string_view filename) {
	std::string source = getShaderSource(filename);

	GLuint shader = glCreateShader(type);
	const char* sources = source.data();
	glShaderSource(shader, 1, &sources, NULL);
	glCompileShader(shader);

	/* TODO error checking
	GLint success;
	char infoLog[128];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 128, NULL, infoLog);
		print "Shader compile error: " filename "\n"
		print infoLog "\n"
	}
	*/

	return shader;
}

void Shader::linkProgram(GLuint vertexShader, GLuint fragmentShader) {
	program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	/* TODO error checking
	GLint success;
	char infoLog[128];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 128, NULL, infoLog);
		print "Program link error\n"
		print infoLog "\n"
	}
	*/

	glUseProgram(0);
}
