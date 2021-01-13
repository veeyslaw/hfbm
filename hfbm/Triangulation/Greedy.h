#pragma once

#include <glm.hpp>
#include "Triangulator.h"
#include "../Mesh/Mesh.h"

class Greedy :
    public Triangulator
{
public:
  Greedy(std::unique_ptr<HeightMap>, double, double);
  void run() override;

protected:
  virtual void insert(glm::ivec2, int);
  void addTriangle(Triangle, int, int, int, int);
  std::pair<double, glm::ivec2> getMaxError(Triangle);
  void updateNeighbor(int, std::pair<int, int>, int);
  int getUncommonPointOffset(int, std::pair<int, int>);

protected:
  std::vector<std::pair<double, glm::ivec2>> maxErrors;
  std::vector<int> toUpdate;
  std::vector<int> neighbors;
  std::vector<bool> used;
};

