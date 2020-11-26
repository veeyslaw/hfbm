#pragma once

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <memory>
#include "../Mesh/Mesh.h"

class OGLWidget :
    public QOpenGLWidget, protected QOpenGLExtraFunctions
{
public:
  OGLWidget(QWidget* parent);
  ~OGLWidget();

  void setMesh(std::shared_ptr<Mesh> newMesh) {
    mesh.swap(newMesh);

    fkinPoints = mesh->vertices;
    fkinIndices = mesh->getFlattenedTriangles();
    /*
    fkinPoints = std::vector<Vertex>();
    fkinPoints.push_back(Vertex(glm::fvec3(0)));
    fkinPoints.push_back(Vertex(glm::fvec3(0, 1, 0)));
    fkinPoints.push_back(Vertex(glm::fvec3(1, 0, 0)));
    fkinIndices = std::vector<GLuint>();
    fkinIndices.push_back(0);
    fkinIndices.push_back(1);
    fkinIndices.push_back(2);
    */
    initVAO();
  }

  QOpenGLContext* getContext() const { 
    QOpenGLContext* sharedContext = new QOpenGLContext;
    sharedContext->setShareContext(context());
    sharedContext->create();
    return sharedContext; 
  }
  //std::shared_ptr<QOpenGLContext> getContext() const { return openGLContext; }
  std::shared_ptr<Mesh> getMesh() const {return mesh; }

protected:
  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();

  void initPwease();
  void wendewPwease();
  void initVAO();

private:
  std::shared_ptr<QOpenGLContext> openGLContext;

  std::shared_ptr<Mesh> mesh;

  std::vector<Vertex> fkinPoints;
  std::vector<GLuint> fkinIndices;
  GLuint vertexArrayObject = 0;
  GLuint vertexBufferObject = 0;
  GLuint elementBufferObject = 0;
};

