#include "BaseDemo.h"
#include <QSurfaceFormat>
#include <QDebug>
#include <QMatrix4x4>
#include <QVector3D>
#include <QTimer>
#include <QImage>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QtMath>

#include "Camera.h"

BaseDemo::BaseDemo(QWidget *parent)
    : QOpenGLWidget(parent),
      m_shaderProgram(new QOpenGLShaderProgram(this)),
      m_vertexArrayObject(new QOpenGLVertexArrayObject(this)),
      m_camera(new Camera(this))
{
    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]()
    {
       m_rotateValue++;
       update();
    });

    resize(800, 800);


    timer->start(1000 / 30);

    installEventFilter(m_camera);
}

void BaseDemo::initializeGL()
{
    this->initializeOpenGLFunctions();

    glEnable(GL_DEPTH_TEST);

    if(!m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/vertexShader.vsh"))
    {
        qDebug() << m_shaderProgram->log();
    }

    if(!m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/fragmentShader.fsh"))
    {
        qDebug() << m_shaderProgram->log();
    }

    m_shaderProgram->link();
    m_shaderProgram->bind();

    GLfloat vertexData[] =
    {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    QOpenGLVertexArrayObject::Binder binder(m_vertexArrayObject);
    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertexData, sizeof(vertexData));

    m_shaderProgram->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(GLfloat) * 5);
    m_shaderProgram->enableAttributeArray(0);
    m_shaderProgram->setAttributeBuffer(1, GL_FLOAT, sizeof(GLfloat) * 3, 2, sizeof(GLfloat) * 5);
    m_shaderProgram->enableAttributeArray(1);

    m_texture = new QOpenGLTexture(QImage(":/texture/texture2.jpg").mirrored());
    m_texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::Repeat);
    m_texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::Repeat);
    m_texture->setMinificationFilter(QOpenGLTexture::Linear);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);

    m_shaderProgram->setUniformValue("texture0", 0);
}

static QVector3D cubePositions[] = {
  QVector3D( 0.0f,  0.0f,  0.0f),
  QVector3D( 2.0f,  5.0f, -15.0f),
  QVector3D(-1.5f, -2.2f, -2.5f),
  QVector3D(-3.8f, -2.0f, -12.3f),
  QVector3D( 2.4f, -0.4f, -3.5f),
  QVector3D(-1.7f,  3.0f, -7.5f),
  QVector3D( 1.3f, -2.0f, -2.5f),
  QVector3D( 1.5f,  2.0f, -2.5f),
  QVector3D( 1.5f,  0.2f, -1.5f),
  QVector3D(-1.3f,  1.0f, -1.5f)
};

void BaseDemo::paintGL()
{
    glClearColor(0.3, 0.4, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_texture->bind(0);
    m_shaderProgram->bind();
    QMatrix4x4 view;
    view.lookAt(m_camera->cameraPos(), m_camera->targetPos(), m_camera->cameraUp());
    m_shaderProgram->setUniformValue("view", view);

    QMatrix4x4 projection;
    projection.perspective(m_camera->fieldOfVision(), 1.0f * width() / height(), 0.1f, 100.0f);
    m_shaderProgram->setUniformValue("projection", projection);
    {
        QOpenGLVertexArrayObject::Binder binder(m_vertexArrayObject);

        for(int i = 0; i < 10; ++i)
        {
            QMatrix4x4 transfrom;
            transfrom.translate(cubePositions[i]);

            float angle = (i + 1.0f) * m_rotateValue;
            transfrom.rotate(angle, QVector3D(1.0f, 0.3f, 0.5f));

            m_shaderProgram->setUniformValue("model", transfrom);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    m_shaderProgram->release();
    m_texture->release();
}
BaseDemo::~BaseDemo()
{
}

