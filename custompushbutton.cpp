
#include "custompushbutton.h"
#include "qbuttongroup.h"

#include <QKeyEvent>



CustomPushButton::CustomPushButton(QWidget *parent,QButtonGroup* btnGroup)
    :QPushButton(parent),
    btnGroup(btnGroup)
{

}

void CustomPushButton::mousePressEvent(QMouseEvent *e)
{

    if(e->button()==Qt::RightButton){
        pressType = RightClick;
    }else {
        pressType = LeftClick;
    }
    emit btnGroup->buttonPressed(this);
}
