
#ifndef CUSTOMPUSHBUTTON_H
#define CUSTOMPUSHBUTTON_H

#include <QPushButton>
#include <QStackedLayout>

    enum PressType{
    LeftClick,
    RightClick
    };

class CustomPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CustomPushButton(QWidget *parent,QButtonGroup* btnGroup = nullptr);

    PressType getPressType() const {return pressType;}
    void EnableHover(){showHover = true;}
private:
    PressType pressType;
    QButtonGroup* btnGroup;
    bool showHover = false;

private slots:
    void mousePressEvent(QMouseEvent *e);
    void enterEvent(QEnterEvent *event);
};

#endif // CUSTOMPUSHBUTTON_H
