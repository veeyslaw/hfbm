#include "Shader.h"
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(
	QOpenGLContext* context,
	std::string_view vertexShaderFilename,
	std::string_view fragmentShaderFilename
) : QOpenGLExtraFunctions(context) {

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

void Shader::setVec3fv(const glm::fvec3& vector, std::string_view name) {
	use();
	glUniform3fv(glGetUniformLocation(program, name.data()), 1, glm::value_ptr(vector));
	unuse();
}

void Shader::setMatrix4fv(const glm::fmat4& matrix, std::string_view name) {
	use();
	glUniformMatrix4fv(glGetUniformLocation(program, name.data()), 1, GL_FALSE, glm::value_ptr(matrix));
	unuse();
}

std::string getShaderSource(std::string_view filename) {
	std::stringstream source;

	std::ifstream ifs(filename.data());
	if (!ifs.is_open()) {
		std::stringstream ss;
		ss << "Could not open shader file: " << filename << "\n";
		throw ss.str();
	}

	std::string line;
	while (std::getline(ifs, line)) {
		source << line << "\n";
	}

	return source.str();
}

GLuint Shader::loadShader(GLenum type, std::string_view filename) {
	std::string source = getShaderSource(filename);

	GLuint shader = glCreateShader(type);
	const char* sources = source.data();
	glShaderSource(shader, 1, &sources, NULL);
	glCompileShader(shader);

	GLint success;
	char infoLog[128];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 128, NULL, infoLog);
		std::stringstream ss;
		ss << "Shader compile error: " << filename << "\n";
		ss << infoLog << "\n";
		throw ss.str();
	}

	return shader;
}

void Shader::linkProgram(GLuint vertexShader, GLuint fragmentShader) {
	program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	GLint success;
	char infoLog[128];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 128, NULL, infoLog);
		std::stringstream ss;
		ss << "Program link error\n";
		ss << infoLog << "\n";
		throw ss.str();
	}

	glUseProgram(0);
}
