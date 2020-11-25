#pragma once

#include "glm.hpp"

#include "Triangulator.h"
#include "../Mesh/Mesh.h"

class Naive :
    public Triangulator
{
public:
  Naive(const QImage&);
  void run() override;

private:

private:
  
};

