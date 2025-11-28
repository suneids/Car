#include "joystick.h"
#include <QMouseEvent>
#include <QDebug>
void Joystick::mousePressEvent(QMouseEvent *event){
    locked = true;
    if(event == nullptr)
        return;
}


void Joystick::mouseMoveEvent(QMouseEvent *event){
    if(locked){
        QPoint relative = mapToParent(event->pos());
        dx = relative.x() - frameCenterX;
        dy = relative.y() - frameCenterY;
        float distance = std::sqrt(dx*dx + dy*dy);
        int maxRadius = parentWidget()->width()/2 - width()/2;
        if(distance > maxRadius){
            dx = dx*maxRadius/distance;
            dy = dy*maxRadius/distance;
        }
        move(frameCenterX + dx - width()/2, frameCenterY + dy -height()/2);
    }
}


void Joystick::mouseReleaseEvent(QMouseEvent *event){
    if(event == nullptr)
        return;
    locked = false;
    this->setGeometry(frameCenterX-width()/2, frameCenterY - height()/2, widthJ, heightJ);
    dx = 0;
    dy = 0;
}

void Joystick::showEvent(QShowEvent *) {
    frameCenterX = parentWidget()->width()/2;
    frameCenterY = parentWidget()->height()/2;
    this->setGeometry(frameCenterX-width()/2, frameCenterY - height()/2, widthJ, heightJ);
}

void Joystick::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::red);
    painter.setPen(Qt::black);
    painter.drawEllipse(0, 0, widthJ, heightJ);
}
