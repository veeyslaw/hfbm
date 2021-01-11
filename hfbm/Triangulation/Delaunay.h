#pragma once

#include <glm.hpp>
#include "Triangulator.h"
#include "../Mesh/Mesh.h"

class Delaunay :
    public Triangulator
{
public:
  Delaunay(const QImage&, int, double);
  void run() override;
  void insert(glm::fvec3);
  int locate(glm::fvec3);
  bool notReady(const std::vector<std::pair<glm::fvec3, double>>&);
  double getError(glm::fvec3);
  double interpolate(glm::fvec3);
  bool pointInTriangle(glm::fvec3, int);
};

