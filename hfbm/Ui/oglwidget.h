#pragma once

#include <memory>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPoint>
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

  void mousePressEvent(QMouseEvent*) override;
  void mouseMoveEvent(QMouseEvent*) override;
  void mouseReleaseEvent(QMouseEvent*) override;
  void wheelEvent(QWheelEvent*) override;

private:
  void initOptions();
  void initMatrices();
  void updateUniforms();
  void updateProjectionMatrix(int, int );

  void rotateMesh(QMouseEvent*);
  void scaleMesh(QWheelEvent*);

private:
  std::unique_ptr<Mesh> mesh;
  std::shared_ptr<Shader> shader;

  glm::fmat4 viewMatrix = glm::fmat4(1);
  // TODO remove projectionMatrix?
  glm::fmat4 projectionMatrix = glm::fmat4(1); 

  glm::fvec3 cameraPosition = glm::fvec3(0, 0, 1);
  glm::fvec3 lightPosition = glm::fvec3(0, .5, 2);

  bool dragging = false;
  QPoint lastMousePoint;
};

