#include "LightDemo.h"
#include "Camera.h"

LightDemo::LightDemo(QWidget* parent)
    : QOpenGLWidget(parent), m_camera(new Camera(this))
{

}

void LightDemo::initializeGL()
{
    this->initializeOpenGLFunctions();
}

void LightDemo::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.4f, 0.5f);
    glClear(GL_COLOR_BUFFER_BIT);
}
