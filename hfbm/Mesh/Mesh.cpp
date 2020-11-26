#include <fstream>
#include <glm.hpp>
#include <gtx/normal.hpp>
#include <gtx/transform.hpp>
#include "Mesh.h"

#define STL_HEADER_LENGTH 80

const glm::fvec3 Mesh::origin = glm::fvec3(0);
GLuint Mesh::vertexArrayObject = 0;
GLuint Mesh::vertexBufferObject = 0;
GLuint Mesh::elementBufferObject = 0;

Mesh::Mesh(
	QOpenGLContext* context,
	const std::vector<glm::fvec3>& points,
	const std::vector<Triangle>& triangles,
	glm::fvec3 position,
	glm::fvec3 rotation,
	glm::fvec3 scale
	) :
	QOpenGLExtraFunctions(context),
	triangles(triangles),
	noOfVertices(points.size()),
	noOfIndices(3 * triangles.size()),
	noOfTriangles(triangles.size()),
	position(position),
	rotation(rotation),
	scale(scale) {

	vertices.reserve(noOfVertices);
	for (auto i = 0; i < noOfVertices; i++) {
		vertices.push_back(points.at(i));
	}

	initVertexArrayObject();
	// updateModelMatrix();
}

void Mesh::cleanUp() {
	glDeleteBuffers(1, &elementBufferObject);
	glDeleteBuffers(1, &vertexBufferObject);
	glDeleteVertexArrays(1, &vertexArrayObject);
}

void Mesh::initVertexArrayObject() {
	vertices = std::vector<Vertex>();
	vertices.push_back(glm::fvec3(0, 1, 0));
	vertices.push_back(glm::fvec3(1, 0, 0));
	vertices.push_back(glm::fvec3(0, 0, 1));
	noOfVertices = vertices.size();
	triangles = std::vector<Triangle>();
	triangles.push_back({ 0, 1, 2 });
	noOfTriangles = triangles.size();
	noOfIndices = noOfTriangles * 3;

	if (vertexArrayObject == 0) {
		glGenVertexArrays(1, &vertexArrayObject);
		glBindVertexArray(vertexArrayObject);
		glGenBuffers(1, &vertexBufferObject);
		glGenBuffers(1, &elementBufferObject);
	}
	else {
		glBindVertexArray(vertexArrayObject);
	}

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, noOfVertices * sizeof Vertex, vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof Vertex,
		reinterpret_cast<const void*>(offsetof(Vertex, position)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof Vertex,
		reinterpret_cast<const void*>(offsetof(Vertex, color)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
	auto flattenedTriangles = getFlattenedTriangles();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, noOfIndices * sizeof GLuint, flattenedTriangles.data(), GL_STATIC_DRAW);

	// unbind vertex array object
	glBindVertexArray(0);
}

void Mesh::updateModelMatrix() {
	modelMatrix = glm::fmat4(1);
	modelMatrix = glm::translate(modelMatrix, origin);
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::fvec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::fvec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::fvec3(0, 0, 1));
	modelMatrix = glm::translate(modelMatrix, position - origin);
	modelMatrix = glm::scale(modelMatrix, scale);
}

void writeHeader(std::ofstream& fileStream) {
	std::string header("STL generated by hfbm");
	header.reserve(STL_HEADER_LENGTH);
	header.insert(header.size(), STL_HEADER_LENGTH - header.size(), ' ');

	fileStream.write(header.c_str(), STL_HEADER_LENGTH);
}

void writeVector(std::ofstream& ofs, const glm::fvec3& vector) {
	ofs.write(reinterpret_cast<const char*>(&vector.x), sizeof vector.x);
	ofs.write(reinterpret_cast<const char*>(&vector.y), sizeof vector.y);
	ofs.write(reinterpret_cast<const char*>(&vector.z), sizeof vector.z);
}

void Mesh::writeTriangleData(std::ofstream& fileStream) const {
	// This field will not be used
	static const std::uint16_t attribByteCount = 0;

	for (auto i = 0; i < noOfTriangles; i++) {
		auto indices = triangles.at(i);

		auto vertex1 = vertices.at(indices.points.at(0)).position;
		auto vertex2 = vertices.at(indices.points.at(1)).position;
		auto vertex3 = vertices.at(indices.points.at(2)).position;

		auto normal = glm::triangleNormal(vertex1, vertex2, vertex3);

		writeVector(fileStream, normal);
		writeVector(fileStream, vertex1);
		writeVector(fileStream, vertex2);
		writeVector(fileStream, vertex3);

		fileStream.write(reinterpret_cast<const char*>(&attribByteCount), sizeof attribByteCount);
	}
}

void Mesh::saveToSTL(const std::string& path) const {
	std::ofstream fileStream(path, std::ios_base::out | std::ios::binary);
	if (!fileStream.is_open()) {
		// TODO handle error
		return;
	}
	
	writeHeader(fileStream);
	fileStream.write(reinterpret_cast<const char*>(&noOfTriangles), sizeof noOfTriangles);
	writeTriangleData(fileStream);
}

std::vector<GLuint> Mesh::getFlattenedTriangles() const {
	std::vector<GLuint> flatTriangles;
	flatTriangles.reserve(noOfIndices);

	for (auto triangle : triangles) {
		flatTriangles.push_back(triangle.points.at(0));
		flatTriangles.push_back(triangle.points.at(1));
		flatTriangles.push_back(triangle.points.at(2));
	}

	return flatTriangles;
}

void Mesh::updateUniforms(Shader* shader) {
	shader->setMatrix4fv(modelMatrix, "ModelMatrix");
}

// void Mesh::render(Shader* shader) {
void Mesh::render() {
	//updateModelMatrix();
	//updateUniforms(shader);

	//shader->use();

	glBindVertexArray(vertexArrayObject);

	glDrawArrays(GL_TRIANGLES, 0, noOfVertices);
	//glDrawElements(GL_TRIANGLES, noOfIndices, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	//shader->unuse();
}