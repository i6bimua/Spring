#include "optionbutton.h"
#include <QScrollArea>
#include <QMimeData>
#include <QDrag>
#include <QPainter>


OptionButton::OptionButton(QLabel *parent) : QLabel(parent)
{
    QCursor *myCursor = new QCursor(QPixmap(":/Green/images/Arrow.png"));
    this->setCursor (*myCursor);
    lastPoint = QPoint(0, 0);
    pressed = false;
    leftButton = true;
    moveWidget = 0;
    oldParWidget = 0;
    newParWidget = 0;
}


bool OptionButton::eventFilter(QObject *watched, QEvent *event)
{
    if(moveWidget != 0 && watched == moveWidget) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        // 鼠标按下
        if(mouseEvent->type() == QEvent::MouseButtonPress) {
            // 如果限定了只能鼠标左键拖动则判断当前是否是鼠标左键
            if(leftButton && mouseEvent->button() != Qt::LeftButton) {
                return false;
            }
            // 判断控件的区域是否包含了当前鼠标的坐标
            if(moveWidget->rect().contains(mouseEvent->pos())) {
                lastPoint = mouseEvent->pos();
                pressed = true;
            }
        }
    }
    return QLabel::eventFilter(watched, event);
}


void OptionButton::setLeftButton(bool leftButton)
{
    this->leftButton = leftButton;
}


void OptionButton::setWidget(QWidget *moveWidget, QScrollArea *oldParWidget, QWidget *newParWidget)
{
    if(this->moveWidget == 0) {
        this->moveWidget = moveWidget;
        this->moveWidget->installEventFilter(this);
        this->oldParWidget = oldParWidget;
        this->newParWidget = newParWidget;
    }
}

