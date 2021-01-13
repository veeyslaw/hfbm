#pragma once

#include <glm.hpp>
#include "Triangulator.h"
#include "../Mesh/Mesh.h"

class Naive :
    public Triangulator
{
public:
  Naive(std::unique_ptr<HeightMap>, double);
  void run() override;
};

