#include "oglwidget.h"

OGLWidget::OGLWidget(QWidget* parent)
  : QOpenGLWidget(parent) {}

OGLWidget::~OGLWidget() {}

void OGLWidget::initializeGL()
{
  initializeOpenGLFunctions();
  glClearColor(1, 1, 1, 1);
  glEnable(GL_DEPTH_TEST);
  
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
  // glEnable(GL_COLOR_MATERIAL); // TODO if cool keep, discard - else

  const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, -1.0f,
   -1., 0., 0
  };

  
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}

void OGLWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // TODO if needed | GL_STENCIL_BUFFER_BIT
  
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glVertexAttribPointer(
    0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
    3,                  // size
    GL_FLOAT,           // type
    GL_FALSE,           // normalized?
    0,                  // stride
    (void*)0            // array buffer offset
  );
  // Draw the triangle !
  glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
  glDisableVertexAttribArray(0);

}

void OGLWidget::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}