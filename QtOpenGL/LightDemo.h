#ifndef LIGHTDEMO_H
#define LIGHTDEMO_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class Camera;
class LightDemo : public QOpenGLWidget,
    protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    LightDemo(QWidget *parent = Q_NULLPTR);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

private:
    QOpenGLShaderProgram *m_cubeShaderProgram = Q_NULLPTR;
    QOpenGLShaderProgram *m_lightShaderProgram = Q_NULLPTR;

    QOpenGLBuffer m_vbo{QOpenGLBuffer::VertexBuffer};

    QOpenGLVertexArrayObject *m_vaoObject = Q_NULLPTR;
    QOpenGLVertexArrayObject *m_vaoLight = Q_NULLPTR;

    Camera *m_camera = Q_NULLPTR;
};

#endif // LIGHTDEMO_H
