#pragma once

#include <glm.hpp>
#include "Triangulator.h"
#include "../Mesh/Mesh.h"

class Delaunay :
    public Triangulator
{
public:
  Delaunay(const QImage&);
  void run() override;
};

