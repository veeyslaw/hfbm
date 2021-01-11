#pragma once

#include <glm.hpp>
#include "Triangulator.h"
#include "../Mesh/Mesh.h"

class Naive :
    public Triangulator
{
public:
  Naive(std::unique_ptr<HeightMap>, int);
  void run() override;
};

