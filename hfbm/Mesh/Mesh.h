#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <QOpenGlExtraFunctions>
#include "Vertex.h"
#include "Triangle.h"
#include "Shader.h"

class Mesh : public QOpenGLExtraFunctions
{
public:
	Mesh(
		const std::vector<glm::fvec3>&,
		const std::vector<Triangle>&,
		glm::fvec3 position = glm::fvec3(0),
		glm::fvec3 rotation = glm::fvec3(0),
		glm::fvec3 scale = glm::vec3(1)
	);
	~Mesh();

	void saveToSTL(const std::string&) const;

private:
	void initVertexArrayObject();
	void updateModelMatrix();

	void updateUniforms(Shader* shader);
	void render(Shader* shader);

	void writeTriangleData(std::ofstream&) const;

	std::uint32_t getNoOfTriangles() const { return noOfTriangles; }
	std::vector<GLuint> getFlattenedTriangles() const;

private:
	static const glm::fvec3 origin;

	std::vector<Vertex> vertices;
	std::vector<Triangle> triangles;
	
	std::int64_t noOfVertices;
	std::int64_t noOfIndices;
	std::uint32_t noOfTriangles;

	glm::fvec3 position;
	glm::fvec3 rotation;
	glm::fvec3 scale;

	glm::fmat4 modelMatrix;

	GLuint vertexArrayObject;
	GLuint vertexBufferObject;
	GLuint elementBufferObject;
};
