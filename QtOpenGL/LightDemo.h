#ifndef LIGHTDEMO_H
#define LIGHTDEMO_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

class Camera;
class LightDemo : public QOpenGLWidget,
        protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    LightDemo(QWidget* parent = Q_NULLPTR);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

private:
    Camera* m_camera = Q_NULLPTR;
};

#endif // LIGHTDEMO_H
