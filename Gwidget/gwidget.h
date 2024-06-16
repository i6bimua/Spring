#ifndef GWIDGET_H
#define GWIDGET_H

#include <QLabel>
#include <QWidget>
#include <QPushButton>
#include "dragbutton.h"
#include <QMediaPlayer>
QT_BEGIN_NAMESPACE
namespace Ui
{
    class GWidget;
}
QT_END_NAMESPACE

struct RankItem {
    QString Name;
    QString Id;
    int VicTimes;
    int PlayTimes;
    QString MinTime;
};

enum Mode {
    PraMode,
    GameMode
};

class GWidget : public QWidget
{
    Q_OBJECT

public:
    GWidget(QWidget *parent = nullptr);
    ~GWidget();

private:
    Ui::GWidget *ui;
    QPushButton *exercise;
    QPushButton *compete;
    QPushButton *rank;
    DragButton *DragBtn;
    QMediaPlayer *startSound;

public:
    int nMode;

private slots:
    void openExer();
    void openComp();
    void openRank();

public slots:
    void showOriginWindow();

};
#endif // GWIDGET_H

