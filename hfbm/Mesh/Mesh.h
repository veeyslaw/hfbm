#pragma once

#include <vector>
#include <QOpenGlExtraFunctions>
#include "Vertex.h"
#include <string>

class Mesh : public QOpenGLExtraFunctions
{
public:
	Mesh(const std::vector<Vertex>& vertices);
	~Mesh();

	void saveToSTL(const std::string& path) const;
private:
	uint32_t getNoOfTriangles() const { return noOfVertices / 3; }

private:
	const std::vector<Vertex>& vertices;
	
	uint64_t noOfVertices;
};

