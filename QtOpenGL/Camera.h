#ifndef CAMERA_H
#define CAMERA_H

#include <QOpenGLWidget>
#include <QVector3D>
#include <QHash>

class Camera : public QObject
{
    Q_OBJECT
public:
    Camera(QOpenGLWidget *parent = Q_NULLPTR);

    QVector3D cameraPos() const
    {
        return m_cameraPos;
    }
    QVector3D cameraUp() const
    {
        return m_cameraUpPos;
    }
    QVector3D targetPos() const
    {
        return m_cameraPos + m_cameraFront;
    }

    float fieldOfVision() const
    {
        return m_fov;
    }

protected:
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;
    void keyProcess();

    bool keyValid(int key)
    {
        return key == Qt::Key_W ||
               key == Qt::Key_A ||
               key == Qt::Key_S ||
               key == Qt::Key_D ||
               key == Qt::Key_C ||
               key == Qt::Key_Space;
    }

private:
    QHash<int, bool> m_keyValues;
    QVector3D m_cameraPos{0.0f, 0.0f, 3.0f};
    QVector3D m_cameraUpPos{0.0f, 1.0f, 0.0f};
    QVector3D m_cameraFront{0.0f, 0.0f, -1.0f};

    float m_yaw = -90.0f;
    float m_pitch = 0.0f;

    float m_lastX = 0;
    float m_lastY = 0;
    float m_fov = 45.0f;
    bool m_firstMouse = true;
};

#endif // CAMERA_H
