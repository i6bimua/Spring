#ifndef GETMAP2_H
#define GETMAP2_H

#include <QDialog>
#include <QLabel>

namespace Ui {
class GetMap2;
}

class GetMap2 : public QDialog
{
    Q_OBJECT

private:
    QPixmap fitimgpic;

    // 存储七个选项的控件及其在scrollArea中的序号
    QMap<int, QLabel*> optionBtn;

    // scrollArea 控件的区域(选项的区域)
    QRect ScrollRect;

    // 作答的区域
    QRect WidgetRect;

    // 记录成功移动选项后当前地图状态
    QVector<QVector<int>> vctCurSelectedMap;

    // 默认选项
    QVector<int> vctDefaultSlice = {4,3,3,2,2,1,1};

private:
    bool isPrettifyData(void);
    int getMapNum();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

public:
    explicit GetMap2(QWidget *parent = nullptr);
    ~GetMap2();

public:
    void paintEvent(QPaintEvent *event)override;
    void closeEvent(QCloseEvent *event)override;

signals:
    void ReturnMain();

private slots:
    void on_pushButton_clicked();

private:
    Ui::GetMap2 *ui;
};

#endif // GETMAP2_H
