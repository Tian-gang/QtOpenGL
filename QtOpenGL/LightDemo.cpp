#include "LightDemo.h"
#include "Camera.h"

#include <QDebug>
#include <QMatrix4x4>
#include <QTimer>

LightDemo::LightDemo(QWidget* parent)
    : QOpenGLWidget(parent), m_camera(new Camera(this)),
      m_cubeShaderProgram(new QOpenGLShaderProgram(this)),
      m_lightShaderProgram(new QOpenGLShaderProgram(this)),
      m_vaoObject(new QOpenGLVertexArrayObject(this)),
      m_vaoLight(new QOpenGLVertexArrayObject(this))
{
    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]()
    {
       update();
    });

    resize(600, 400);

    timer->start(1000 / 30);
}

void LightDemo::initializeGL()
{
    this->initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);

    if(!m_cubeShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/light/cubeVertex.vsh"))
    {
        qDebug() << m_cubeShaderProgram->log();
        return;
    }

    if(!m_cubeShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/light/cubeFragment.fsh"))
    {
        qDebug() << m_cubeShaderProgram->log();
        return;
    }

    m_cubeShaderProgram->link();
    m_cubeShaderProgram->bind();

     if(!m_lightShaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/light/lightVertexShader.vsh"))
     {
         qDebug() << m_lightShaderProgram->log();
         return;
     }

     if(!m_lightShaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/light/lightFragmentShader.fsh"))
     {
         qDebug() << m_lightShaderProgram->log();
         return;
     }

     m_lightShaderProgram->link();

    GLfloat vertices[] = {
           -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
           -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
           -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

           -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
           -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
           -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

           -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
           -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
           -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
           -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
           -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
           -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

           -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
           -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
           -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

           -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
           -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
           -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        };

    QOpenGLVertexArrayObject::Binder binderObject(m_vaoObject);
    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertices, sizeof(vertices));

    m_cubeShaderProgram->bind();
    m_cubeShaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 6);
    m_cubeShaderProgram->enableAttributeArray(0);
    m_cubeShaderProgram->setAttributeBuffer(1, GL_FLOAT, sizeof(GLfloat) * 3, 3, sizeof(GLfloat) * 6);
    m_cubeShaderProgram->enableAttributeArray(1);

    m_cubeShaderProgram->setUniformValue("objColor", QVector3D(1.0f, 0.5f, 0.31f));
    m_cubeShaderProgram->setUniformValue("lightColor",  QVector3D(1.0f, 1.0f, 1.0f));
    m_cubeShaderProgram->setUniformValue("lightPos", QVector3D(1.2f, 1.0f, 2.0f));

    binderObject.release();

    QOpenGLVertexArrayObject::Binder binderLight(m_vaoLight);
    m_vbo.bind();
    m_lightShaderProgram->bind();
    m_lightShaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 6);
    m_lightShaderProgram->enableAttributeArray(0);

    QMatrix4x4 model;
    model.translate(1.2f, 1.0f, 2.0f);
    model.scale(0.2f);
    m_lightShaderProgram->setUniformValue("model", model);

}

void LightDemo::paintGL()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_cubeShaderProgram->bind();
    QMatrix4x4 projection;
    projection.perspective(m_camera->fieldOfVision(), 1.0f * width() / height(), 0.1f, 100.f);
    m_cubeShaderProgram->setUniformValue("projection", projection);

    QMatrix4x4 view;
    view.lookAt(m_camera->cameraPos(), m_camera->targetPos(), m_camera->cameraUp());
    m_cubeShaderProgram->setUniformValue("view", view);

    QMatrix4x4 model;
    m_cubeShaderProgram->setUniformValue("model", model);

    {
        QOpenGLVertexArrayObject::Binder binderLight(m_vaoObject);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    m_cubeShaderProgram->release();

    m_lightShaderProgram->bind();
    m_lightShaderProgram->setUniformValue("projection", projection);
    m_lightShaderProgram->setUniformValue("view", view);

    {
        QOpenGLVertexArrayObject::Binder binderLight(m_vaoLight);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    m_lightShaderProgram->release();
}
