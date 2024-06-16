#ifndef EXER_H
#define EXER_H

#include <QMovie>
#include <QLabel>
#include <QDIr>
#include <QWidget>
#include <QVector>
#include "gwidget.h"
#include <QFileInfo>
#include <QPushButton>
#include <QElapsedTimer>
#include "pramodedialog.h"
#include "getmap.h"
#include "getmap2.h"

#include "dragbutton.h"
class Exer : public QWidget
{
    Q_OBJECT
public:
    explicit Exer(GWidget *MainWin, QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *Event) override;
    void paintEvent(QPaintEvent *event) override;

signals:
    void aboutToClose();
    void goon();
    void stop();

public slots:
    void openExplan();

public:
    void showOriginWindow();
    int nMode = Mode::PraMode;

private:
    QPushButton *Home;
    QPushButton *Explan;
    //背景图片
    QPixmap fitimgpic;

    //获得关卡数量
    int getMapNum();
    const QString DirPath = QStringLiteral ("./map");
    PraModeDialog *pramodedialog;
    DragButton *DragBtn;

};

#endif // EXER_H
