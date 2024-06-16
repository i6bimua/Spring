#ifndef RANK_H
#define RANK_H

#include <QWidget>
#include "gwidget.h"
#include <QFile>
#include <QLabel>
#include <QPixmap>
#include <QVector>
#include <QJsonArray>
#include <QTableView>
#include <QJsonObject>
#include <QPaintEvent>
#include <QPushButton>
#include <QJsonDocument>
#include <QStandardItemModel>
#include <QMap>
#include "dragbutton.h"
class Rank : public QWidget
{
    Q_OBJECT
public:
    explicit Rank(GWidget *MainWin, QWidget *parent = nullptr);
    void insertNewRecord(RankItem ps);
    void insertNewVicRecord(RankItem ps);
    bool hasRecord(QString Id);

private:
    QVector<RankItem>RankVec;
    void loadNewRank();
    QLabel *RankTitle;
    QPushButton *VicButton;
    QPushButton *IdButton;
    QPushButton *PlayButton;
    QPushButton *MinButton;
    QTableView *tableView;
    QStandardItemModel *RankModel;
    QPixmap fitimgpic;
    QMap <QString, bool>HasRecord;
    DragButton *DragBtn;

protected:
    void closeEvent(QCloseEvent *Event) override;
    void paintEvent(QPaintEvent *event) override;


signals:
    void aboutToClose();
    void stop();
    void goon();

private slots:
    QJsonArray loadFileFromJson(QString filePath);
    void writeFileIntoJson(QVector<RankItem>RankVec, QString FilePath);
    void renewVic();
    void renewId();
    void renewPlay();
    void renewMin();
    void reload();
};

#endif // RANK_H
