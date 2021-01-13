#pragma once

#include <glm.hpp>
#include "Greedy.h"
#include "../Mesh/Mesh.h"

class Delaunay :
    public Greedy
{
public:
  Delaunay(std::unique_ptr<HeightMap>, int, double);

protected:
  void insert(glm::ivec2, int) override;
};

