#ifndef DRAGBUTTON_H
#define DRAGBUTTON_H

#include <QObject>
#include <QPushButton>
#include <QLabel>
class DragButton : public QPushButton
{
    Q_OBJECT
public:
    DragButton(QWidget *parent = nullptr);
    void setMargin(int left, int top, int right, int bottom);

protected:
    bool eventFilter(QObject *watched, QEvent *event)override;

private:
    QString text;
    bool isPressed;
    QPoint lastPoint;
    bool isMoved;
    QPixmap fitimgpic;
    QPushButton *PixLabel;
    bool BtnClick;
    int x_LeftDis;
    int y_TopDis;
    int x_RightDis;
    int y_BotDis;
    bool isChecked = false;

signals:
    void stop();
    void goon();
    void clicked();
    void doubleClicked();
    void toggled(bool isToggled);

};

#endif // DRAGBUTTON_H
