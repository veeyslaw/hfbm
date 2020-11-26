#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <QOpenGlExtraFunctions>
#include "Shader.h"
#include "Triangle.h"
#include "Vertex.h"

class Mesh : public QOpenGLExtraFunctions
{
public:
	Mesh(
		QOpenGLContext* context,
		const std::vector<glm::fvec3>& points = std::vector<glm::fvec3>(),
		const std::vector<Triangle>& triangles = std::vector<Triangle>(),
		glm::fvec3 position = glm::fvec3(0),
		glm::fvec3 rotation = glm::fvec3(0),
		glm::fvec3 scale = glm::vec3(1)
	);
	void cleanUp();

	// void render(Shader* shader);
	void render();

	void saveToSTL(const std::string&) const;

private:
	void initVertexArrayObject();
	void updateModelMatrix();

	void updateUniforms(Shader* shader);

	void writeTriangleData(std::ofstream&) const;

	std::vector<GLuint> getFlattenedTriangles() const;

private:
	static const glm::fvec3 origin;
	glm::fvec3 position;
	glm::fvec3 rotation;
	glm::fvec3 scale;

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
