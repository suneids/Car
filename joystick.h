#ifndef JOYSTICK_H
#define JOYSTICK_H
#include <QWidget>
#include <QPainter>
class Joystick : public QWidget
{
    Q_OBJECT
public:
    explicit Joystick(QWidget *parent = nullptr):QWidget(parent){
        if(parent != nullptr){
            setMouseTracking(true);
            setAttribute(Qt::WA_MouseTracking, true);
            setAttribute(Qt::WA_TransparentForMouseEvents, false);
            setAutoFillBackground(true);
            setStyleSheet("background-color: red; border-radius: 50%; border: 2px solid black;");
        }
    }
    int dy = 0, dx = 0, frameRadius = 0;
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void showEvent(QShowEvent *) override;
    void paintEvent(QPaintEvent *) override;
private:
    int frameCenterX = 0, frameCenterY = 0, widthJ = 40, heightJ = 40;

    bool locked = false;
};

#endif // JOYSTICK_H
