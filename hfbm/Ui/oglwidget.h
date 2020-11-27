#pragma once

#include <memory>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include "../Mesh/Mesh.h"
#include "../OpenGL/Shader.h"

class OGLWidget :
    public QOpenGLWidget, protected QOpenGLExtraFunctions
{
public:
  OGLWidget(QWidget* parent);
  ~OGLWidget();

  void setMesh(std::unique_ptr<Mesh> newMesh) { mesh.swap(newMesh); }

  QOpenGLContext* getContext() const;
  const Mesh* getMesh() const { return mesh.get(); }

protected:
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();

private:
  void initOptions();
  void initMatrices();
  void updateUniforms();
  void updateProjectionMatrix(int w, int h);
  void updateViewMatrix();

private:
  std::unique_ptr<Mesh> mesh;
  std::shared_ptr<Shader> shader;

  glm::fmat4 viewMatrix = glm::fmat4(1);
  glm::fmat4 projectionMatrix = glm::fmat4(1);
};

