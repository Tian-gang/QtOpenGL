#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLBuffer>
#include <QHash>
#include <QVector3D>
#include <QOpenGLFunctions_3_3_Core>

class Camera;

class BaseDemo : public QOpenGLWidget,
               public QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    BaseDemo(QWidget *parent = nullptr);
    ~BaseDemo();

protected:
    void paintGL() Q_DECL_OVERRIDE;
    void initializeGL() Q_DECL_OVERRIDE;

private:
    QOpenGLShaderProgram* m_shaderProgram = Q_NULLPTR;
    QOpenGLVertexArrayObject* m_vertexArrayObject = Q_NULLPTR;
    QOpenGLTexture* m_texture = Q_NULLPTR;

    QOpenGLBuffer m_vbo{QOpenGLBuffer::VertexBuffer};

    Camera* m_camera = Q_NULLPTR;

private:
    int m_rotateValue = 0;

};
#endif // WIDGET_H
