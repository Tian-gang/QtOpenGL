#include "Camera.h"
#include <QEvent>
#include <QKeyEvent>
#include <QtMath>
#include <QMouseEvent>

Camera::Camera(QObject *parent) : QObject(parent)
{

}

bool Camera::eventFilter(QObject *watched, QEvent *event)
{
    auto keyEventFunc = [=](bool pressed)
    {
        if(auto keyEvent = dynamic_cast<QKeyEvent*>(event))
        {
            if(keyValid(keyEvent->key()))
            {
                m_keyValues[keyEvent->key()] = pressed;
            }
        }
    };

    if(event->type() == QEvent::KeyPress)
    {
        keyEventFunc(true);
    }
    else if(event->type() == QEvent::KeyRelease)
    {
        keyEventFunc(false);
    }
    else if(event->type() == QEvent::MouseButtonPress)
    {
        auto mouseEvent = dynamic_cast<QMouseEvent*>(event);
        if(mouseEvent && (mouseEvent->button() == Qt::LeftButton))
        {
            m_firstMouse = true;
        }
    }
    else if(event->type() == QEvent::MouseMove)
    {
        auto mouseEvent = dynamic_cast<QMouseEvent*>(event);
        if(mouseEvent && (mouseEvent->buttons() & Qt::LeftButton))
        {
           if(m_firstMouse)
           {
               m_lastX = mouseEvent->pos().x();
               m_lastY = mouseEvent->pos().y();
               m_firstMouse = false;
           }

           auto mousePos = mouseEvent->pos();
           float xOffset = mousePos.x() - m_lastX;
           float yOffset = m_lastY - mousePos.y();

           m_lastX = mousePos.x();
           m_lastY = mousePos.y();

           float sensitivity = 0.3f;
           xOffset *= sensitivity;
           yOffset *= sensitivity;

           m_yaw += xOffset;
           m_pitch += yOffset;
           m_pitch = qMin(qMax(-89.0f, m_pitch), 89.0f);

           QVector3D front;

           auto pitchRadians = qDegreesToRadians(m_pitch);
           auto yawRadians = qDegreesToRadians(m_yaw);

           front.setX(qCos(pitchRadians) * qCos(yawRadians));
           front.setY(qSin(pitchRadians));
           front.setZ(qCos(pitchRadians) * qSin(yawRadians));

           m_cameraFront = front.normalized();
        }
    }
    else if(event->type() == QEvent::Wheel)
    {
        if(auto wheelEvent = dynamic_cast<QWheelEvent*>(event))
        {
            auto numPixels = wheelEvent->pixelDelta();
            auto numDegress = wheelEvent->angleDelta() / 8;
            auto numSetp = !numPixels.isNull() ? numPixels : numDegress / 15;
            auto targetFov = m_fov - numSetp.y();

            m_fov = qMin(qMax(1.0f, targetFov), 150.0f);
        }
    }
    else if(event->type() == QEvent::Paint)
    {
        keyProcess();
    }

    return QObject::eventFilter(watched, event);
}

void Camera::keyProcess()
{
    float speed = 0.1f;

    if(m_keyValues[Qt::Key_W]) {
        m_cameraPos += m_cameraFront * speed;
    }

    if(m_keyValues[Qt::Key_S]) {
        m_cameraPos -= m_cameraFront * speed;
    }

    if(m_keyValues[Qt::Key_A]) {
        m_cameraPos -= QVector3D::crossProduct(m_cameraFront, m_cameraUpPos).normalized() * speed;
    }

    if(m_keyValues[Qt::Key_D]) {
        m_cameraPos += QVector3D::crossProduct(m_cameraFront, m_cameraUpPos).normalized() * speed;
    }

    if(m_keyValues[Qt::Key_Space]) {
        m_cameraPos += m_cameraUpPos * speed;
    }

    if(m_keyValues[Qt::Key_C]) {
        m_cameraPos -= m_cameraUpPos * speed;
    }
}
