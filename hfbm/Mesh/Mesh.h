#pragma once

#include <vector>
#include <QOpenGlExtraFunctions>
#include "Vertex.h"

class Mesh : public QOpenGLExtraFunctions
{
public:
	Mesh();
	~Mesh();
private:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

};

