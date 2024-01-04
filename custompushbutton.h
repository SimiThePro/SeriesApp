
#ifndef CUSTOMPUSHBUTTON_H
#define CUSTOMPUSHBUTTON_H

#include <QPushButton>

    enum PressType{
    LeftClick,
    RightClick
    };

class CustomPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CustomPushButton(QWidget *parent,QButtonGroup* btnGroup);

    PressType getPressType() const {return pressType;}
private:
    PressType pressType;
    QButtonGroup* btnGroup;

private slots:
    void mousePressEvent(QMouseEvent *e);
};

#endif // CUSTOMPUSHBUTTON_H
