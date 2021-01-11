#pragma once

#include <glm.hpp>
#include "Triangulator.h"
#include "../Mesh/Mesh.h"

struct Suspect {
  Suspect(int triangle, int point1, int point2, int point) :
    triangle(triangle), edge({ point1, point2 }), point(point) {}

  int triangle;
  int point;
  std::array<int, 2> edge;
};

class Delaunay :
    public Triangulator
{
public:
  Delaunay(std::unique_ptr<HeightMap>, int, double);
  void run() override;
  void insert(glm::fvec3);
  int locate(glm::fvec3);
  std::pair<int, int> locate(Suspect);
  bool notReady(const std::vector<std::pair<glm::fvec3, double>>&);
  double getError(glm::fvec3);
  double interpolate(glm::fvec3);
  bool pointInTriangle(glm::fvec3, int);
};

