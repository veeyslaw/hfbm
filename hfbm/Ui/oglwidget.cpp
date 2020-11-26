#include "oglwidget.h"
// #include <QSurface>

OGLWidget::OGLWidget(QWidget* parent)
  : QOpenGLWidget(parent) {

  //setSurfaceType(QSurface::OpenGLSurface);
  //create();
  //context = std::make_shared<QOpenGLContext>(new QOpenGLContext);
  //context->create();
  //makeCurrent();
  //initializeOpenGLFunctions();
  //initializeGL();
  //initializeOpenGLFunctions();
  //openGLContext = std::make_shared<QOpenGLContext>(context());

  //mesh = std::make_shared<Mesh>(Mesh(openGLContext));
}

OGLWidget::~OGLWidget() {}

void OGLWidget::initializeGL()
{
  //context->makeCurrent(this);
  initializeOpenGLFunctions();
  openGLContext = std::make_shared<QOpenGLContext>(context());

  glClearColor(1, 1, 1, 1);
  glEnable(GL_DEPTH_TEST);
  // glEnable(GL_LIGHT0);
  // glEnable(GL_LIGHTING);
  // glEnable(GL_COLOR_MATERIAL); // TODO if cool keep, discard - else

  //mesh = std::make_shared<Mesh>(openGLContext);
  mesh = std::make_shared<Mesh>(getContext());
  initPwease();
}

void OGLWidget::initPwease() {
  //fkinPoints = mesh->vertices;
  //fkinIndices = mesh->getFlattenedTriangles();

  fkinPoints = std::vector<Vertex>();
  fkinPoints.push_back(Vertex(glm::fvec3(0)));
  fkinPoints.push_back(Vertex(glm::fvec3(0, -1, 0)));
  fkinPoints.push_back(Vertex(glm::fvec3(-1, 0, 0)));
  fkinPoints.push_back(Vertex(glm::fvec3(0)));
  fkinPoints.push_back(Vertex(glm::fvec3(1, 1, 0)));
  fkinPoints.push_back(Vertex(glm::fvec3(1, -1, 0)));
  fkinIndices = std::vector<GLuint>();
  fkinIndices.push_back(0);
  fkinIndices.push_back(1);
  fkinIndices.push_back(2);
  fkinIndices.push_back(3);
  fkinIndices.push_back(4);
  fkinIndices.push_back(5);

  initVAO();
}

void OGLWidget::initVAO() {
  if (vertexArrayObject == 0) {
    //glDeleteBuffers(1, &elementBufferObject);
    //glDeleteBuffers(1, &vertexBufferObject);
    //glDeleteVertexArrays(1, &vertexArrayObject);
    glGenVertexArrays(1, &vertexArrayObject);
    glGenBuffers(1, &vertexBufferObject);
  }

  //vertexArrayObject = 1;
  glBindVertexArray(vertexArrayObject);

  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, fkinPoints.size() * sizeof Vertex, fkinPoints.data(), GL_STATIC_DRAW);
  //glBindBuffer(GL_ARRAY_BUFFER, 0);

  /*
  glGenBuffers(1, &elementBufferObject);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, fkinIndices.size() * sizeof GLuint, fkinIndices.data(), GL_STATIC_DRAW);
  */
  //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float), NULL);
  //glEnableVertexAttribArray(0);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof Vertex,
    reinterpret_cast<const void*>(offsetof(Vertex, position)));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof Vertex,
    reinterpret_cast<const void*>(offsetof(Vertex, normal)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof Vertex,
    reinterpret_cast<const void*>(offsetof(Vertex, color)));
  glEnableVertexAttribArray(2);

  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof Vertex,
    reinterpret_cast<const void*>(offsetof(Vertex, texcoord)));
  glEnableVertexAttribArray(3);

  // unbind vertex array object
  glBindVertexArray(0);
}

void OGLWidget::wendewPwease() {
  glBindVertexArray(vertexArrayObject);

  glDrawArrays(GL_TRIANGLES, 0, fkinPoints.size());
  //glDrawElements(GL_TRIANGLES, noOfIndices, GL_UNSIGNED_INT, NULL);

  glBindVertexArray(0);
}

void OGLWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // TODO if needed | GL_STENCIL_BUFFER_BIT
  //mesh->render();
  wendewPwease();
}

void OGLWidget::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);
  /*
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  */
}