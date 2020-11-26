#pragma once

#include <memory>
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include "../Mesh/Mesh.h"

class OGLWidget :
    public QOpenGLWidget, protected QOpenGLExtraFunctions
{
public:
  OGLWidget(QWidget* parent);
  ~OGLWidget();

  void setMesh(std::unique_ptr<Mesh> newMesh) {
    mesh.swap(newMesh);
  }

  QOpenGLContext* getContext() const;
  const Mesh* getMesh() const { return mesh.get(); }

protected:
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();

private:
  std::unique_ptr<Mesh> mesh;
};

