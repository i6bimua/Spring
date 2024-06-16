#ifndef OPTIONBUTTON_H
#define OPTIONBUTTON_H

#include <QWidget>
#include <QPushButton>
#include <QEvent>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QLabel>

class OptionButton : public QLabel
{
    Q_OBJECT
public:
    explicit OptionButton(QLabel *parent = nullptr);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;


private:
    QPoint lastPoint;           // 最后按下的坐标
    bool pressed;               // 鼠标是否按下
    bool leftButton;            // 限定鼠标左键
    QWidget *moveWidget;        // 移动的控件
    QScrollArea *oldParWidget;  // 移动的控件其原本的父控件
    QWidget *newParWidget;      // 移动的控件其新的父控件

signals:

public slots:
    // 设置是否限定鼠标左键
    void setLeftButton(bool leftButton);

    // 设置要移动的控件
    // QWidget *moveWidget 要移动的控件
    // QWidget *oldParWidget 要移动的控件其原本的父控件
    // QWidget *newParWidget 要移动的控件其新的父控件
    void setWidget(QWidget *moveWidget, QScrollArea *oldParWidget, QWidget *newParWidget);
};

#endif // OPTIONBUTTON_H
