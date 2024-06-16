#ifndef COMP_H
#define COMP_H

#include <QFile>
#include <QWidget>
#include "gwidget.h"
#include "rank.h"
#include <QTcpSocket>
#include "tcpserver.h"
#include "tcpclient.h"
#include "pramodedialog.h"
#include "dragbutton.h"
class Comp : public QWidget
{
    Q_OBJECT
public:
    explicit Comp(GWidget *MainWin, QWidget *parent = nullptr);
    int nMode = Mode::GameMode;
    TcpServer *tcpserver = nullptr;
    TcpClient *tcpclient = nullptr;

protected:
    void closeEvent(QCloseEvent *Event) override;

private:
    QPushButton *openServer;
    QPushButton *openClient;
    QPushButton *Start;
    QPushButton *Home;
    PraModeDialog *pramodedialog;
    RankItem ps;
    Rank *rank;
    bool VicNew = false;
    std::map<QString, QString>PassWord;
    qint64 StartId;
    DragButton *DragBtn;

    QJsonArray loadFileFromJson(QString filePath);
    void writeFileIntoJson(std::map<QString, QString>passWord, QString FilePath);
    void loadUser();

signals:
    void aboutToClose();
    void goon();
    void stop();

private slots:
    void openSer();
    void openCli();
};

#endif // COMP_H
