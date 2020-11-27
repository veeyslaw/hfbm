#pragma once

#include <string_view>
#include <QOpenGlExtraFunctions>
#include <glm.hpp>

static std::string_view defaultVertexShaderFilename("OpenGL/VertexShader.glsl");
static std::string_view defaultFragmentShaderFilename("OpenGL/FragmentShader.glsl");

class Shader : public QOpenGLExtraFunctions
{
public:
	Shader(
		QOpenGLContext* context,
		std::string_view vertexShaderFilename = defaultVertexShaderFilename,
		std::string_view fragmentShaderFilename = defaultFragmentShaderFilename
	);

	~Shader();

	void use();
	void unuse();

	void setMatrix4fv(const glm::fmat4&, std::string_view);

private:
	GLuint loadShader(GLenum, std::string_view);
	void linkProgram(GLuint, GLuint);

private:
	GLuint program;
	
};

