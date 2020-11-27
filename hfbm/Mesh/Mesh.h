#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <QOpenGlExtraFunctions>
#include "../OpenGL/Shader.h"
#include "Triangle.h"
#include "Vertex.h"

class Mesh : public QOpenGLExtraFunctions
{
public:
	Mesh(
		QOpenGLContext* context,
		const std::vector<glm::fvec3>& points = std::vector<glm::fvec3>(),
		const std::vector<Triangle>& triangles = std::vector<Triangle>(),
		glm::fvec3 rotation = glm::fvec3(0),
		float scale = 1
	);
	void cleanUp();

	void rotate(const glm::fvec3& amount) { rotation += amount; }
	void scaleUp(float);
	void render(std::shared_ptr<Shader> shader);

	void saveToSTL(const std::string&) const;

private:
	void initVertexArrayObject();
	void updateModelMatrix();

	void updateUniforms(std::shared_ptr<Shader> shader);

	void writeTriangleData(std::ofstream&) const;

	std::vector<GLuint> getFlattenedTriangles() const;

private:
	glm::fvec3 baseOrigin = glm::fvec3(0);
	glm::fvec3 origin = glm::fvec3(0);
	glm::fvec3 rotation;
	float scale;

	glm::fmat4 modelMatrix;

	std::vector<Vertex> vertices;
	std::vector<Triangle> triangles;
	
	std::int64_t noOfVertices;
	std::int64_t noOfIndices;
	std::uint32_t noOfTriangles;

	static GLuint vertexArrayObject;
	static GLuint vertexBufferObject;
	static GLuint elementBufferObject;
};
