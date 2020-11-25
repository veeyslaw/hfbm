#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <QOpenGlExtraFunctions>
#include "Vertex.h"
#include "TriangleIndices.h"

class Mesh : public QOpenGLExtraFunctions
{
public:
	Mesh(const std::vector<glm::fvec3>&, const std::vector<TriangleIndices>&);

	void saveToSTL(const std::string&) const;
private:
	std::uint32_t getNoOfTriangles() const { return noOfTriangles; }
	void writeTriangleData(std::ofstream& fileStream) const;

private:
	std::vector<Vertex> vertices;
	std::vector<TriangleIndices> triangleIndices;
	
	std::int64_t noOfVertices;
	std::uint32_t noOfTriangles;
};
