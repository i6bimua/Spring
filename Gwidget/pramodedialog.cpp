#include "pramodedialog.h"
#include "ui_pramodedialog.h"
#include "rank.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QDialog>
#include "gwidget.h"
#include <QFile>
#include <QMimeData>
#include <QDrag>
#include <QTextStream>
#include "explanation.h"
#include <QtMath>
#include <QCursor>
#include <QSoundEffect>
#include <QMessageBox>
#include "comp.h"
#include "exer.h"

enum Code {
    UpMap,
    UpMapData,
    TimeStart,
    TimeRun,
    TimeEnd
};

void PraModeDialog::timeout_slot()
{
    *qtime = qtime->addMSecs(1);
    ui->label_time->setText("时间: " + qtime->toString("mm:ss.zz"));

    QWidget *pParent = (QWidget *)this->parent();
    QString strClassName = pParent->metaObject()->className();
    if(strClassName == QStringLiteral("Comp"))
    {
        if(((Comp*)pParent)->tcpserver)
        {
            ((Comp*)pParent)->tcpserver->sendTime(Code::TimeRun);
        }

        if(((Comp*)pParent)->tcpclient)
        {
            ((Comp*)pParent)->tcpclient->sendTime(Code::TimeRun);
        }
    }
}

// void PraModeDialog::time_comout_slot()
// {
//     *qtime_com = qtime_com->addMSecs(1);
//     ui->label_com_time->setText("时间: " + qtime_com->toString("mm:ss.zz"));
// }

bool PraModeDialog::loadData(QString CurMapStr)
{
    QFile file(CurMapStr);
    // 打开文件
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        // 读取关卡地图数据
        while(!in.atEnd()) {
            in.readLine().toInt();
            QVector<QVector<int>> twoVct;
            QVector<int> oneVct;
            for(int i = 0; i < 6; i++) {
                QString str = in.readLine();
                QStringList list = str.split(",");
                for(int j = 0; j < 6; j++) {
                    int num = list[j].toInt();
                    oneVct.append(num);
                }
                twoVct.append(oneVct);
                oneVct.clear();
            }
            vctMaps.append(twoVct);
            twoVct.clear();
        }
        // 关闭文件
        file.close();
        return true;
    } else {
        return false;
    }
}

void PraModeDialog::createData()
{
    srand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    for(int n = 0; n < 1e4; n++) {
        for(int i = 0; i < vctDefaultSlice.length(); i++) {
            int a = vctDefaultSlice[i];
            int s = qMin(6, a);
            int c = qMin(6, a);
            selectMap.width = s;
            selectMap.height = c;
            createSubMapDataArrary(6, 6, s, c);
            vctSelectMaps.push_back(selectMap);
            selectMap.data.clear();
        }
        mergeMapDataArrary(6, 6);
        if(isPrettifyData()) {
            break;
        }
        vctSelectMaps.clear();
        vctCurMap.clear();
    }
}

void PraModeDialog::createSubMapDataArrary(int width, int height, int n, int o)
{
    int i = qFloor(rand() % (width - qMax(n - 1, 0)));
    int r = qFloor(rand() % (height - qMax(o - 1, 0)));
    for(int s = 0; s < height; s++) {
        QVector<int> c;
        for(int l = 0; l < width; l++) {
            int p = 0;
            if(l >= i && l < i + n && s >= r && s < r + o) {
                p = 1;
            }
            c.push_back(p);
        }
        selectMap.data.push_back(c);
    }
}

void PraModeDialog::mergeMapDataArrary(int width, int height)
{
    for(int i = 0; i < height; i++) {
        QVector<int> r;
        for(int a = 0; a < width; a++) {
            r.push_back(0);
        }
        vctCurMap.push_back(r);
    }
    for(int s = 0; s < vctSelectMaps.length(); s++) {
        auto c = vctSelectMaps[s].data;
        for(int i = 0; i < height; i++) {
            for(int a = 0; a < width; a++) {
                int l = c[i][a];
                vctCurMap[i][a] += l;
            }
        }
    }
}

bool PraModeDialog::isPrettifyData(void)
{
    bool e = false;
    int n = 0, o = 0, i = 0;
    if(vctCurMap.length() == 6 && vctCurMap[0].length() == 6) {
        for(int r = 0; r < 6; r++) {
            int a, s, c;
            for(a = 0, s = 0; s < 6; s++) {
                c = vctCurMap[r][s];
                a += c;
            }
            if(0 == a) {
                return false;
            }
        }
        for(int s = 0; s < 6; s++) {
            int a, r, c;
            for(a = 0, r = 0; r < 6; r++) {
                c = vctCurMap[r][s];
                a += c;
            }
            if(0 == a) {
                return false;
            }
        }
        for(int r = 0; r < 6; r++) {
            int s;
            for(s = 0; s < 6; s++) {
                int c;
                0 == (c = vctCurMap[r][s]) ? i++ : 4 == c ? o++ : c >= 5 && n++;
            }
        }
    }
    return 1 == o && i < 10 && 0 == n && (e = !0), e;
}

void PraModeDialog::NextLevel()
{
    if(vctCurMap == vctCurSelectedMap) {
        // 成功通关
        emit VicSig ();
        QSoundEffect *startSound = new QSoundEffect(this); //创建一个音乐播放器
        startSound->setSource(QUrl("qrc:/Green/images/vic.wav"));
        startSound->setVolume(0.5f);
        startSound->play();
        qtimer->stop();
        ui->widget_next_level->show();
        ui->pushButton_next_level->setStyleSheet("background-color:rgba(0,0,0,150);color:white");
        ui->label_next_level->setStyleSheet("background-color:transparent;color:white");
        ui->label_good_time->setStyleSheet("background-color:transparent;color:orange;");
        ui->label_good_time->setText("时间: " + qtime->toString("mm:ss.zz"));
        connect (ui->pushButton_next_level, &QPushButton::clicked, this, [ = ] {
            emit Nextlevel ();
        });

        QWidget *pParent = (QWidget *)this->parent();
        QString strClassName = pParent->metaObject()->className();
        if(strClassName == QStringLiteral("Comp"))
        {
            if(((Comp*)pParent)->tcpserver)
            {
                ((Comp*)pParent)->tcpserver->sendTime(Code::TimeEnd);
            }

            if(((Comp*)pParent)->tcpclient)
            {
                ((Comp*)pParent)->tcpclient->sendTime(Code::TimeEnd);
            }
        }
    }
}

void PraModeDialog::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void PraModeDialog::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void PraModeDialog::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);
        int index;
        dataStream >> index;
        if(WidgetRect.contains(event->pos())) {
            int num = vctDefaultSlice[index];
            // 获取左上角的坐标
            QPoint curPoint = event->pos() - QPoint{(num * 84 / 2), (num * 84 / 2)};
            QPoint curPointinWidget = curPoint - ui->widget_2->pos();
            int x = curPointinWidget.rx() % 84;
            int y = curPointinWidget.ry() % 84;
            if((x > -42 && x < 0) || (x > 42)) {
                curPointinWidget.rx() += 42;
            }
            if((y > -42 && y < 0) || (y > 42)) {
                curPointinWidget.ry() += 42;
            }
            curPointinWidget += ui->widget_2->pos();
            // 左上角落在那个 label 控件上
            QLabel *child = static_cast<QLabel *>(childAt(curPointinWidget));
            if (!child) {
                return;
            }
            QString str = child->objectName();
            QStringList list = str.split("_");
            QString strM = list[1];
            QString strN = list[2];
            int m = strM.toInt();
            int n = strN.toInt();
            if((m + num > 6) || (n + num > 6)) {
                optionBtn[index]->show();
            } else {
                stackMapRecord.push(vctCurSelectedMap);
                stackTipRecord.push(TipVec);
                int nChunkX = n;
                int nChunkY = m;
                //qDebug() << m << n << num;
                for(int i = 0; i < TipVec.size(); ++i) {
                    auto t = TipVec[i];
                    if(t.x == m + 1 && t.y == n + 1 && t.len == num) {
                        TipVec.erase(TipVec.begin() + i);
                        //qDebug() << "yes";
                        break;
                    }
                }
                for(int i = 0; i < num; i++) {
                    for(int j = 0; j < num; j++) {
                        vctCurSelectedMap[nChunkY][nChunkX]++;
                        nChunkX++;
                    }
                    nChunkX = n;
                    nChunkY++;
                }


                QObjectList gs = ui->widget_2->children();
                for(int i = 0; i < gs.size(); i++) {
                    // 提取坐标
                    str = gs[i]->objectName();
                    list = str.split("_");
                    strM = list[1];
                    strN = list[2];
                    m = strM.toInt();
                    n = strN.toInt();
                    num = vctCurSelectedMap[m][n];
                    if(num == 0) {
                        ((QLabel *)gs[i])->clear();
                    } else {
                        QString strText = "%1";
                        ((QLabel *)gs[i])->setText(strText.arg(num));
                    }
                }

                QWidget *pParent = (QWidget *)this->parent();
                QString strClassName = pParent->metaObject()->className();
                if(strClassName == QStringLiteral("Comp"))
                {
                    if(((Comp*)pParent)->tcpserver)
                    {
                        ((Comp*)pParent)->tcpserver->sendMap(vctCurSelectedMap,  Code::UpMapData);
                    }

                    if(((Comp*)pParent)->tcpclient)
                    {
                        ((Comp*)pParent)->tcpclient->sendMap(vctCurSelectedMap,  Code::UpMapData);
                    }
                }


                // QObjectList gs3 = ui->widget_3->children();
                // for(int i = 0; i < gs3.size(); i++) {
                //     // 提取坐标
                //     str = gs3[i]->objectName();
                //     list = str.split("_");
                //     strM = list[2];
                //     strN = list[3];
                //     m = strM.toInt();
                //     n = strN.toInt();
                //     num = vctCurSelectedMap[m][n];
                //     if(num == 0) {
                //         ((QLabel *)gs3[i])->clear();
                //     } else {
                //         QString strText3 = "%1";
                //         ((QLabel *)gs3[i])->setText(strText3.arg(num));
                //     }
                // }

                okBtn.push(index);
                QSoundEffect *startSound = new QSoundEffect(this);
                startSound->setSource(QUrl("qrc:/Green/images/in.wav"));
                startSound->setVolume(0.5f);
                startSound->play();
                NextLevel();
            }
        } else {
            optionBtn[index]->show();
        }
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void PraModeDialog::mousePressEvent(QMouseEvent *event)
{
    QLabel *child = static_cast<QLabel *>(childAt(event->pos()));
    if (!child) {
        return;
    }
    int index = optionBtn.key(child);
    if(index == -1) {
        return;
    }
    QString str = child->metaObject()->className();
    if(str == QStringLiteral("QWidget")) {
        return;
    }
    if(!ScrollRect.contains(event->pos())) {
        return;
    }
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << index;
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-dnditemdata", itemData);
    QString styleSheet = ":/image/images/tree_%1_move.png";
    QPixmap pixmap(styleSheet.arg(index + 1));
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(QPoint(pixmap.width() / 2, pixmap.height() / 2));
    child->hide();
    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
    } else {
        child->show();
    }
}

void PraModeDialog::closeEvent(QCloseEvent *)
{
    emit ReturnMain();
}

PraModeDialog::PraModeDialog(QWidget *parent, QString CurMapStrr) :
    QDialog(parent),
    ui(new Ui::PraModeDialog)
{
    lastPoint = {0, 0, 0};
    ui->setupUi(this);
    // 设置固定窗口大小，使其无法放大缩小
    QCursor *myCursor = new QCursor(QPixmap(":/Green/images/Arrow.png"));
    this->setCursor (*myCursor);
    this->setFixedSize(this->width(), this->height());
    this->setAcceptDrops(true);       //设置能否放置
    //开启鼠标跟踪
    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    nCurLevel = 0;
    CurMapString = CurMapStrr;
    if(loadData(CurMapString)) {
        vctCurMap = vctMaps[nCurLevel];
    } else {
        createData();
    }
    for(int i = 0; i < 6; i++) {
        QVector<int> r;
        for(int a = 0; a < 6; a++) {
            r.push_back(0);
        }
        vctCurSelectedMap.push_back(r);
    }
    ui->pushButton_click_start->setStyleSheet("QPushButton{"
            "background-color:rgba(0,0,0,200);"
            "color:white"
            "}");
    ui->widget_next_level->setStyleSheet("background-color:rgba(0,0,0,200);");
    // 设置帮助窗口隐藏
    ui->widget->hide();
    // 设置挑战成功窗口隐藏
    ui->widget_next_level->hide();
    // 设置模式

    ui->label->setAlignment(Qt::AlignCenter);
    ui->label->setFont(QFont("Algerian", 25));
    ui->label->setStyleSheet("QLabel{background:transparent;"
                             "color:white;"
                             "}");
    // 设置按钮样式
    ui->pushButton_ret->setStyleSheet(tr("border-image: url(:/image/images/return.png)"));
    ui->pushButton_reset->setText("重置");
    ui->pushButton_reset->setFont(QFont("Algerian", 14));
    ui->pushButton_reset->setStyleSheet("QPushButton{"
                                        "background:transparent;"
                                        "color:white;"
                                        "border-image: url(:/image/images/reset.png);"
                                        "}");
    ui->pushButton_help->setText("帮助");
    ui->pushButton_help->setFont(QFont("Algerian", 14));
    ui->pushButton_help->setStyleSheet("QPushButton{"
                                       "background:transparent;"
                                       "color:white;"
                                       "border-image: url(:/image/images/help_btn.png);"
                                       "}");
    ui->pushButton_back->setText("后退");
    ui->pushButton_back->setFont(QFont("Algerian", 14));
    ui->pushButton_back->setStyleSheet("QPushButton{"
                                       "background:transparent;"
                                       "color:white;"
                                       "border-image: url(:/image/images/help_btn.png);"
                                       "}");
    // 设置 widget 控件上所有控件的样式
    ui->pushButton->setText("关闭");
    ui->pushButton->setFont(QFont("Algerian", 14));
    ui->pushButton->setStyleSheet("QPushButton{"
                                  "background:transparent;"
                                  "color:white;"
                                  "border-image: url(:/image/images/help_btn.png);"
                                  "}");
    ui->label_2->setText("使用帮助");
    ui->label_2->setAlignment(Qt::AlignCenter);
    ui->label_2->setFont(QFont("Algerian", 25));
    ui->label_2->setStyleSheet("QLabel{"
                               "background:transparent;"
                               "color:white;"
                               "}");
    ui->widget->setStyleSheet("border-image: url(:/image/images/background.png)");
    // 设置使用帮助
    ui->label_3->setPixmap(QPixmap(":/image/images/help.png"));
    ui->label_3->setScaledContents(true);
    // 设置背景图
    QPalette pal = this->palette();
    QPixmap pixmap(":/Green/images/back3.png");
    pixmap = pixmap.scaled(this->width(), this->height(), Qt::IgnoreAspectRatio);
    pal.setBrush(QPalette::Window, QBrush(pixmap));
    this->setPalette(pal);
    // 设置作答区域
    ui->widget_2->setStyleSheet("background-color:#757898;");
    QObjectList gs = ui->widget_2->children();
    for(int i = 0; i < gs.size(); i++) {
        // 提取坐标
        QString str = gs[i]->objectName();
        QStringList list = str.split("_");
        QString strM = list[1];
        QString strN = list[2];
        int m = strM.toInt();
        int n = strN.toInt();
        int num = vctCurMap[m][n];
        QString styleSheet = "border-width:1px;border-style:solid;border-color:rgb(255,170,0);"
                             "background-image:url(:/image/images/game_%1.png);";
        ((QLabel *)gs[i])->setFrameShape(QFrame::Box);
        QWidget *pParent = (QWidget *)this->parent();
        QString strClassName = pParent->metaObject()->className();
        if(strClassName == QStringLiteral("Comp") && num == 0)
        {
            num = 2;
        }

        ((QLabel *)gs[i])->setStyleSheet(styleSheet.arg(num));
    }


    QWidget *pParent = (QWidget *)this->parent();
    QString strClassName = pParent->metaObject()->className();
    if(strClassName == QStringLiteral("Comp"))
    {
        if(((Comp*)pParent)->tcpserver)
        {
            bool bl = connect(((Comp*)pParent)->tcpserver, SIGNAL(readyMap(QByteArray)), this, SLOT(Up_comMap(QByteArray)));
            bl = connect(((Comp*)pParent)->tcpserver, SIGNAL(readyMapData(QByteArray)), this, SLOT(Up_comMapData(QByteArray)));
            bl = connect(((Comp*)pParent)->tcpserver, SIGNAL(readyTime(int)), this, SLOT(Up_comTime(int)));
            qDebug() << bl << '\n';
            ((Comp*)pParent)->tcpserver->sendMap(vctCurMap,  Code::UpMap);
        }

        if(((Comp*)pParent)->tcpclient)
        {
            bool bl = connect(((Comp*)pParent)->tcpclient, SIGNAL(readyMap(QByteArray)), this, SLOT(Up_comMap(QByteArray)));
            bl = connect(((Comp*)pParent)->tcpclient, SIGNAL(readyMapData(QByteArray)), this, SLOT(Up_comMapData(QByteArray)));
            bl = connect(((Comp*)pParent)->tcpclient, SIGNAL(readyTime(int)), this, SLOT(Up_comTime(int)));
            qDebug() << bl << '\n';
            ((Comp*)pParent)->tcpclient->sendMap(vctCurMap,  Code::UpMap);
        }
    }

    // ui->widget_3->setStyleSheet("background-color:#757898;");
    // QObjectList gs3 = ui->widget_3->children();
    // for(int i = 0; i < gs3.size(); i++) {
    //     // 提取坐标
    //     QString str3 = gs3[i]->objectName();
    //     QStringList list3 = str3.split("_");
    //     QString strM3 = list3[2];
    //     QString strN3 = list3[3];
    //     int m3 = strM3.toInt();
    //     int n3 = strN3.toInt();
    //     int num3 = vctCurMap[m3][n3];
    //     QString styleSheet3 = "border-width:1px;border-style:solid;border-color:rgb(255,170,0);"
    //                          "background-image:url(:/image/images/game_min_%1.png);";
    //     ((QLabel *)gs3[i])->setFrameShape(QFrame::Box);
    //     QWidget *pParent3 = (QWidget *)this->parent();
    //     QString strClassName3 = pParent3->metaObject()->className();
    //     if(strClassName3 == QStringLiteral("Comp") && num3 == 0)
    //     {
    //         num3 = 2;
    //     }
    //     ((QLabel *)gs3[i])->setStyleSheet(styleSheet3.arg(num3));
    // }



    ui->scrollArea->setStyleSheet("background-color:#757898;");
    // 设置选项控件
    QHBoxLayout *pLayout = new QHBoxLayout();
    for(int i = 0; i < 7; i++) {
        QLabel *pLab = new QLabel();
        pLab->setMinimumSize(QSize(115, 115));
        pLab->setMaximumSize(QSize(115, 115));
        QString styleSheet = ":/image/images/tree_%1.png";
        pLab->setPixmap(QPixmap(styleSheet.arg(i + 1)));
        pLab->setAttribute(Qt::WA_DeleteOnClose);
        optionBtn.insert(i, pLab);
        pLayout->addWidget(pLab);
    }
    ui->scrollArea->widget()->setLayout(pLayout);
    ScrollRect = QRect(ui->scrollArea->pos(), ui->scrollArea->size());
    WidgetRect = QRect(ui->widget_2->pos(), ui->widget_2->size());
    qtime = new QTime;
    qtimer = new QTimer;
    QFont font("SimSun", 15);
    ui->label_time->setStyleSheet("color:white;");
    ui->label_time->setText("时间: 00:00.00");
    ui->label_time->setFont(font);
    init();
    flag = false;
    for(int i = 1; i <= 3; i++) {
        for(int j = 1; j <= 3; j++) {
            dfs(i, j, 1);
        }
    }
    // qDebug() << flag;
    // for(auto i : TipVec) {
    //     qDebug() << i.x << ' ' << i.y << ' ' << i.len;
    // }
    int sum = 0;
    std::map<int, int>map;
    for(int i = 0; i < 6; ++i) {
        for(int j = 0; j < 6; ++j) {
            map[vctCurMap[i][j]]++;
            if(vctCurMap[i][j] > 0) {
                sum ++;
            }
        }
    }
    for(int i = 0; i < 6; ++i) {
        for(int j = 0; j < 6; ++j) {
            if(j >= 1 && vctCurMap[i][j] == vctCurMap[i][j - 1]) {
                sum--;
            }
            if(i >= 1 && vctCurMap[i][j] == vctCurMap[i - 1][j]) {
                sum--;
            }
        }
    }
    int pixnum = 0;
    if (sum <= -13) {
        pixnum = 1;
    } else if (sum <= -6) {
        pixnum = 2;
    } else if (sum <= 0) {
        pixnum = 3;
    } else if (sum <= 4) {
        pixnum = 4;
    } else {
        pixnum = 5;
    }
    //qDebug() << sum << '\n';
    QString HRate = ":/Green/images/star%1.png";
    QPixmap *ppixmap = new QPixmap(HRate.arg(pixnum));
    ppixmap->scaled(ui->RatLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->RatLabel->setScaledContents(true);
    ui->RatLabel->setPixmap(*ppixmap);
    QFont Txfont("FZZJ-QDXKJW", 28);
    ui->PixLabel->setFont (Txfont);
    ui->PixLabel->setText (tr(" 难度系数："));
    ui->PixLabel->setStyleSheet ("QLabel{"
                                 "background-color: transparent;"
                                 "font-family:FZZJ-QDXKJW;"
                                 "font-size:28px;"
                                 "font-weight:1000;"
                                 "color:#98FB98;"
                                 "}");
    connect(qtimer, SIGNAL(timeout()), this, SLOT(timeout_slot()));
    connect(ui->tipBtn, &QToolButton::clicked, this, &PraModeDialog::Tip );
    TipTimer = new QTimer(this);
    TipTimer->setSingleShot (true);
    TipTimer->setInterval (2300);

    qtime_com = new QTime;
    qtimer_com = new QTimer;
    // ui->label_com_time->setStyleSheet("color:white;");
    // ui->label_com_time->setText("时间: 00:00.00");
    // ui->label_com_time->setFont(font);
    // connect(qtimer_com, SIGNAL(timeout()), this, SLOT(time_comout_slot()));
}

PraModeDialog::~PraModeDialog()
{
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    delete ui;
}

void PraModeDialog::on_pushButton_ret_clicked()
{
    this->close();
    emit ReturnMain();
}

void PraModeDialog::on_pushButton_help_clicked()
{
    // 设置帮助窗口显示
    Explanation *Explan = new Explanation(this);
    // 设置子窗口为顶级窗口
    Explan->setWindowFlags(Qt::Window);
    //自动释放资源
    Explan->setAttribute(Qt::WA_DeleteOnClose);
    //设置窗口的大小以及标题
    Explan->setWindowTitle (tr("规则说明"));
    QIcon ExerIcon = QIcon(":/Green/images/explan.png");
    if (ExerIcon.isNull ()) {
        qDebug() << "加载规则说明Icon失败";
        return;
    }
    Explan->setWindowIcon(ExerIcon);
    Explan->setWindowFlag (Qt::MSWindowsFixedSizeDialogHint);//固定大小
    Explan->setFixedSize (800, 900);
    Explan->setWindowModality (Qt::ApplicationModal);
    Explan->show();
    qtimer->stop();
}

void PraModeDialog::on_pushButton_clicked()
{
    // 设置帮助窗口隐藏
    ui->widget->hide();
    qtimer->start(1);
}

void PraModeDialog::on_pushButton_back_clicked()
{
    if(okBtn.isEmpty()) {
        return;
    }
    int index = okBtn.pop();
    optionBtn[index]->show();
    //    stackMapRecord.pop();
    vctCurSelectedMap = stackMapRecord.pop();
    if(stackTipRecord.size()) {
        TipVec = stackTipRecord.pop();
    }
    QObjectList gs = ui->widget_2->children();
    for(int i = 0; i < gs.size(); i++) {
        // 提取坐标
        QString str = gs[i]->objectName();
        QStringList list = str.split("_");
        QString strM = list[1];
        QString strN = list[2];
        int m = strM.toInt();
        int n = strN.toInt();
        int num = vctCurSelectedMap[m][n];
        if(num == 0) {
            ((QLabel *)gs[i])->clear();
        } else {
            QString strText = "%1";
            ((QLabel *)gs[i])->setText(strText.arg(num));
        }
    }

    QWidget *pParent = (QWidget *)this->parent();
    QString strClassName = pParent->metaObject()->className();
    if(strClassName == QStringLiteral("Comp"))
    {
        if(((Comp*)pParent)->tcpserver)
        {
            ((Comp*)pParent)->tcpserver->sendMap(vctCurSelectedMap,  Code::UpMapData);
        }

        if(((Comp*)pParent)->tcpclient)
        {
            ((Comp*)pParent)->tcpclient->sendMap(vctCurSelectedMap,  Code::UpMapData);
        }
    }

    // QObjectList gs3 = ui->widget_3->children();
    // for(int i = 0; i < gs3.size(); i++) {
    //     // 提取坐标
    //     QString str3 = gs3[i]->objectName();
    //     QStringList list3 = str3.split("_");
    //     QString strM3 = list3[2];
    //     QString strN3 = list3[3];
    //     int m3 = strM3.toInt();
    //     int n3 = strN3.toInt();
    //     int num3 = vctCurSelectedMap[m3][n3];
    //     if(num3 == 0) {
    //         ((QLabel *)gs3[i])->clear();
    //     } else {
    //         QString strText3 = "%1";
    //         ((QLabel *)gs3[i])->setText(strText3.arg(num3));
    //     }
    // }
}

void PraModeDialog::on_pushButton_reset_clicked()
{
    if(nCurLevel < vctMaps.length()) {
        vctCurMap = vctMaps[nCurLevel];
    } else {
        createData();
    }
    vctCurSelectedMap.clear();
    for(int i = 0; i < 6; i++) {
        QVector<int> r;
        for(int a = 0; a < 6; a++) {
            r.push_back(0);
        }
        vctCurSelectedMap.push_back(r);
    }


    QObjectList gs = ui->widget_2->children();
    for(int i = 0; i < gs.size(); i++) {
        // 提取坐标
        QString str = gs[i]->objectName();
        QStringList list = str.split("_");
        QString strM = list[1];
        QString strN = list[2];
        int m = strM.toInt();
        int n = strN.toInt();
        int num = vctCurMap[m][n];
        QString styleSheet = "border-width:1px;border-style:solid;border-color:rgb(255,170,0);"
                             "background-image:url(:/image/images/game_%1.png);";
        ((QLabel *)gs[i])->setFrameShape(QFrame::Box);
        QWidget *pParent = (QWidget *)this->parent();
        QString strClassName = pParent->metaObject()->className();
        if(strClassName == QStringLiteral("Comp") && num == 0)
        {
            num = 2;
        }

        ((QLabel *)gs[i])->setStyleSheet(styleSheet.arg(num));
        ((QLabel *)gs[i])->clear();
    }

    QWidget *pParent = (QWidget *)this->parent();
    QString strClassName = pParent->metaObject()->className();
    if(strClassName == QStringLiteral("Comp"))
    {
        if(((Comp*)pParent)->tcpserver)
        {
            ((Comp*)pParent)->tcpserver->sendMap(vctCurMap,  Code::UpMap);
            ((Comp*)pParent)->tcpserver->sendTime(Code::TimeEnd);
        }

        if(((Comp*)pParent)->tcpclient)
        {
            ((Comp*)pParent)->tcpclient->sendMap(vctCurMap,  Code::UpMap);
            ((Comp*)pParent)->tcpclient->sendTime(Code::TimeEnd);
        }
    }


    // QObjectList gs3 = ui->widget_3->children();
    // for(int i = 0; i < gs3.size(); i++) {
    //     // 提取坐标
    //     QString str3 = gs3[i]->objectName();
    //     QStringList list3 = str3.split("_");
    //     QString strM3 = list3[2];
    //     QString strN3 = list3[3];
    //     int m3 = strM3.toInt();
    //     int n3 = strN3.toInt();
    //     int num3 = vctCurMap[m3][n3];
    //     QString styleSheet = "border-width:1px;border-style:solid;border-color:rgb(255,170,0);"
    //                          "background-image:url(:/image/images/game_min_%1.png);";
    //     ((QLabel *)gs3[i])->setFrameShape(QFrame::Box);
    //     QWidget *pParent3 = (QWidget *)this->parent();
    //     QString strClassName3 = pParent3->metaObject()->className();
    //     if(strClassName3 == QStringLiteral("Comp") && num3 == 0)
    //     {
    //         num3 = 2;
    //     }
    //     ((QLabel *)gs3[i])->setStyleSheet(styleSheet.arg(num3));
    //     ((QLabel *)gs3[i])->clear();
    // }


    // 清空之前的数据
    okBtn.clear();
    stackMapRecord.clear();
    stackTipRecord.clear();
    // 选项控件全部显示
    for(int i = 0; i < optionBtn.size(); i++) {
        optionBtn[i]->show();
    }
    qtimer->stop();
    ui->pushButton_click_start->show();
    TipVec.clear();
    init();
    flag = false;
    for(int i = 1; i <= 3; i++) {
        for(int j = 1; j <= 3; j++) {
            dfs(i, j, 1);
        }
    }
    int sum = 0;
    std::map<int, int>map;
    for(int i = 0; i < 6; ++i) {
        for(int j = 0; j < 6; ++j) {
            map[vctCurMap[i][j]]++;
            if(vctCurMap[i][j] > 0) {
                sum ++;
            }
        }
    }
    for(int i = 0; i < 6; ++i) {
        for(int j = 0; j < 6; ++j) {
            if(j >= 1 && vctCurMap[i][j] == vctCurMap[i][j - 1]) {
                sum--;
            }
            if(i >= 1 && vctCurMap[i][j] == vctCurMap[i - 1][j]) {
                sum--;
            }
        }
    }
    int pixnum = 0;
    if (sum <= -13) {
        pixnum = 1;
    } else if (sum <= -6) {
        pixnum = 2;
    } else if (sum <= 0) {
        pixnum = 3;
    } else if (sum <= 4) {
        pixnum = 4;
    } else {
        pixnum = 5;
    }
    //qDebug() << sum << '\n';
    QString HRate = ":/Green/images/star%1.png";
    QPixmap *ppixmap = new QPixmap(HRate.arg(pixnum));
    ppixmap->scaled(ui->RatLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->RatLabel->setScaledContents(true);
    ui->RatLabel->setPixmap(*ppixmap);
    QFont Txfont("FZZJ-QDXKJW", 28);
    ui->PixLabel->setFont (Txfont);
    ui->PixLabel->setText (tr(" 难度系数："));
    ui->PixLabel->setStyleSheet ("QLabel{"
                                 "background-color: transparent;"
                                 "font-family:FZZJ-QDXKJW;"
                                 "font-size:28px;"
                                 "font-weight:1000;"
                                 "color:#98FB98;"
                                 "}");
}

void PraModeDialog::on_pushButton_click_start_clicked()
{
    // 重置计时
    qtime->setHMS(0, 0, 0, 0);
    ui->label_time->setText("时间: " + qtime->toString("mm:ss.zz"));
    qtimer->start(1);
    ui->pushButton_click_start->hide();

    QWidget *pParent = (QWidget *)this->parent();
    QString strClassName = pParent->metaObject()->className();
    if(strClassName == QStringLiteral("Comp"))
    {
        if(((Comp*)pParent)->tcpserver)
        {
            ((Comp*)pParent)->tcpserver->sendTime(Code::TimeStart);
        }

        if(((Comp*)pParent)->tcpclient)
        {
            ((Comp*)pParent)->tcpclient->sendTime(Code::TimeStart);
        }
    }
}

void PraModeDialog::on_pushButton_next_level_clicked()
{
    nCurLevel++;
    if(nCurLevel < vctMaps.length()) {
        vctCurMap = vctMaps[nCurLevel];
    } else {
        createData();
    }
    vctCurSelectedMap.clear();
    for(int i = 0; i < 6; i++) {
        QVector<int> r;
        for(int a = 0; a < 6; a++) {
            r.push_back(0);
        }
        vctCurSelectedMap.push_back(r);
    }


    QObjectList gs = ui->widget_2->children();
    for(int i = 0; i < gs.size(); i++) {
        // 提取坐标
        QString str = gs[i]->objectName();
        QStringList list = str.split("_");
        QString strM = list[1];
        QString strN = list[2];
        int m = strM.toInt();
        int n = strN.toInt();
        int num = vctCurMap[m][n];
        QString styleSheet = "border-width:1px;border-style:solid;border-color:rgb(255,170,0);"
                             "background-image:url(:/image/images/game_%1.png);";
        ((QLabel *)gs[i])->setFrameShape(QFrame::Box);
        QWidget *pParent = (QWidget *)this->parent();
        QString strClassName = pParent->metaObject()->className();
        if(strClassName == QStringLiteral("Comp") && num == 0)
        {
            num = 2;
        }
        ((QLabel *)gs[i])->setStyleSheet(styleSheet.arg(num));
        ((QLabel *)gs[i])->clear();
    }


    QWidget *pParent = (QWidget *)this->parent();
    QString strClassName = pParent->metaObject()->className();
    if(strClassName == QStringLiteral("Comp"))
    {
        if(((Comp*)pParent)->tcpserver)
        {
            ((Comp*)pParent)->tcpserver->sendMap(vctCurMap,  Code::UpMap);
        }

        if(((Comp*)pParent)->tcpclient)
        {
            ((Comp*)pParent)->tcpclient->sendMap(vctCurMap,  Code::UpMap);
        }
    }


    // 清空之前的数据
    okBtn.clear();
    stackMapRecord.clear();
    stackTipRecord.clear();
    // 选项控件全部显示
    for(int i = 0; i < optionBtn.size(); i++) {
        optionBtn[i]->show();
    }
    ui->widget_next_level->hide();
    ui->pushButton_click_start->show();
    TipVec.clear();
    init();
    flag = false;
    for(int i = 1; i <= 3; i++) {
        for(int j = 1; j <= 3; j++) {
            dfs(i, j, 1);
        }
    }
    int sum = 0;
    std::map<int, int>map;
    for(int i = 0; i < 6; ++i) {
        for(int j = 0; j < 6; ++j) {
            map[vctCurMap[i][j]]++;
            if(vctCurMap[i][j] > 0) {
                sum ++;
            }
        }
    }
    for(int i = 0; i < 6; ++i) {
        for(int j = 0; j < 6; ++j) {
            if(j >= 1 && vctCurMap[i][j] == vctCurMap[i][j - 1]) {
                sum--;
            }
            if(i >= 1 && vctCurMap[i][j] == vctCurMap[i - 1][j]) {
                sum--;
            }
        }
    }
    int pixnum = 0;
    if (sum <= -13) {
        pixnum = 1;
    } else if (sum <= -6) {
        pixnum = 2;
    } else if (sum <= 0) {
        pixnum = 3;
    } else if (sum <= 4) {
        pixnum = 4;
    } else {
        pixnum = 5;
    }
    //qDebug() << sum << '\n';
    QString HRate = ":/Green/images/star%1.png";
    QPixmap *ppixmap = new QPixmap(HRate.arg(pixnum));
    ppixmap->scaled(ui->RatLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->RatLabel->setScaledContents(true);
    ui->RatLabel->setPixmap(*ppixmap);
    QFont Txfont("FZZJ-QDXKJW", 28);
    ui->PixLabel->setFont (Txfont);
    ui->PixLabel->setText (tr(" 难度系数："));
    ui->PixLabel->setStyleSheet ("QLabel{"
                                 "background-color: transparent;"
                                 "font-family:FZZJ-QDXKJW;"
                                 "font-size:28px;"
                                 "font-weight:1000;"
                                 "color:#98FB98;"
                                 "}");
}

void PraModeDialog::Up_comMap(QByteArray Messagereceive)
{
    QVector<QVector<int>> vctCurComMap;

    for(int i = 0; i < 6; i++) {
        QVector<int> r;
        for(int j = 0; j < 6; j++) {

            QByteArray byte = Messagereceive.mid(i*6+j, 1);
            r.push_back(byte.toInt());
        }
        vctCurComMap.push_back(r);
    }

    ui->widget_3->setStyleSheet("background-color:#757898;");
    QObjectList gs3 = ui->widget_3->children();
    for(int i = 0; i < gs3.size(); i++) {
        // 提取坐标
        QString str3 = gs3[i]->objectName();
        QStringList list3 = str3.split("_");
        QString strM3 = list3[2];
        QString strN3 = list3[3];
        int m3 = strM3.toInt();
        int n3 = strN3.toInt();
        int num3 = vctCurComMap[m3][n3];
        QString styleSheet3 = "border-width:1px;border-style:solid;border-color:rgb(255,170,0);"
                              "background-image:url(:/image/images/game_min_%1.png);";
        ((QLabel *)gs3[i])->setFrameShape(QFrame::Box);
        QWidget *pParent3 = (QWidget *)this->parent();
        QString strClassName3 = pParent3->metaObject()->className();
        if(strClassName3 == QStringLiteral("Comp") && num3 == 0)
        {
            num3 = 2;
        }
        ((QLabel *)gs3[i])->setStyleSheet(styleSheet3.arg(num3));
        ((QLabel *)gs3[i])->clear();
    }
}

void PraModeDialog::Up_comMapData(QByteArray Messagereceive)
{
    QVector<QVector<int>> vctCurComMap;

    for(int i = 0; i < 6; i++) {
        QVector<int> r;
        for(int j = 0; j < 6; j++) {

            QByteArray byte = Messagereceive.mid(i*6+j, 1);
            r.push_back(byte.toInt());
        }
        vctCurComMap.push_back(r);
    }

    QObjectList gs3 = ui->widget_3->children();
    for(int i = 0; i < gs3.size(); i++) {
        // 提取坐标
        QString str = gs3[i]->objectName();
        QStringList list = str.split("_");
        QString strM = list[2];
        QString strN = list[3];
        int m = strM.toInt();
        int n = strN.toInt();
        int num = vctCurComMap[m][n];
        if(num == 0) {
            ((QLabel *)gs3[i])->clear();
        } else {
            QString strText3 = "%1";
            ((QLabel *)gs3[i])->setText(strText3.arg(num));
        }
    }
}

void PraModeDialog::Up_comTime(int code)
{
    if(code == Code::TimeStart)
    {
        static bool isFirst = true;
        if(isFirst)
        {
            QFont font("SimSun", 15);
            ui->label_com_time->setStyleSheet("color:white;");
            ui->label_com_time->setText("时间: 00:00.00");
            ui->label_com_time->setFont(font);

            isFirst = false;
        }

        qtime_com->setHMS(0, 0, 0, 0);
        ui->label_com_time->setText("时间: " + qtime_com->toString("mm:ss.zz"));
        qtimer_com->start(1);
    }
    else if(code == Code::TimeRun)
    {
        *qtime_com = qtime_com->addMSecs(1);
        ui->label_com_time->setText("时间: " + qtime_com->toString("mm:ss.zz"));
    }
    else if(code == Code::TimeEnd)
    {
        qtimer_com->stop();
    }
}

void PraModeDialog::Tip()
{
    //qDebug() << getTip;
    QWidget *pParent = (QWidget *)this->parent();
    QString strClassName = pParent->metaObject()->className();
    if(strClassName != QStringLiteral("Comp"))
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle (tr("警告"));
        msgBox.setText (tr("比赛模式禁止作弊!"));
        msgBox.setStandardButtons (QMessageBox::Yes );
        QApplication::setQuitOnLastWindowClosed (false);
        msgBox.exec();
        return;
    }
    if(!TipVec.size()) {
        delete TipTimer;
        return;
    }
    auto [x, y, len] = TipVec.front();
    x--, y--;
    QObjectList gs = ui->widget_2->children();
    for(int t = 0; t < gs.size(); t++) {
        // 提取坐标
        QString str = gs[t]->objectName();
        QStringList list = str.split("_");
        QString strM = list[1];
        QString strN = list[2];
        int m = strM.toInt();
        int n = strN.toInt();
        if(m >= x && m < x + len && n >= y && n < y + len) {
            QString styleSheet = "border-width:1px;border-style:solid;border-color:rgb(255,170,0);"
                                 "background-image:url(:/Green/images/tip%1.png);";
            ((QLabel *)gs[t])->clear();
            ((QLabel *)gs[t])->setFrameShape(QFrame::Box);
            ((QLabel *)gs[t])->setStyleSheet(styleSheet.arg(1));
            ((QLabel *)gs[t])->clear();
        }
    }
    TipTimer->setSingleShot (true);
    TipTimer->setInterval (2300);
    TipTimer->start(2300);
    connect(TipTimer, SIGNAL(timeout()), this, SLOT(Recover()));
}

void PraModeDialog::Recover()
{
    QObjectList gs = ui->widget_2->children();
    for(int i = 0; i < gs.size(); i++) {
        // 提取坐标
        QString str = gs[i]->objectName();
        QStringList list = str.split("_");
        QString strM = list[1];
        QString strN = list[2];
        int m = strM.toInt();
        int n = strN.toInt();
        int num = vctCurMap[m][n];
        QString styleSheet = "border-width:1px;border-style:solid;border-color:rgb(255,170,0);"
                             "background-image:url(:/image/images/game_%1.png);";
        ((QLabel *)gs[i])->setFrameShape(QFrame::Box);
        QWidget *pParent = (QWidget *)this->parent();
        QString strClassName = pParent->metaObject()->className();
        if(strClassName == QStringLiteral("Comp") && num == 0)
        {
            num = 2;
        }
        ((QLabel *)gs[i])->setStyleSheet(styleSheet.arg(num));
    }
}

void PraModeDialog::init()
{
    for(int i = 1; i <= 6; i++) {
        for(int j = 1; j <= 6; j++) {
            temp[i][j] = vctCurMap[i - 1][j - 1];
        }
    }
    if(TipVec.size()) {
        TipVec.clear();
    }
}

void PraModeDialog::dfs(int x, int y, int cnt)
{
    if(flag) {
        return ;
    }
    bool flag1 = true;
    TipVec.push_back({x, y, size[cnt]});
    for(int i = x; i < x + size[cnt]; i++) {
        for(int j = y; j < y + size[cnt]; j++) {
            temp[i][j]--;
            if(temp[i][j] < 0) {
                flag1 = false;
            }
        }
    }
    if(!flag1) {
        for(int i = x; i < x + size[cnt]; i++) {
            for(int j = y; j < y + size[cnt]; j++) {
                temp[i][j]++;
            }
        }
        if(TipVec.size()) {
            TipVec.pop_back ();
        }
        return ;
    }
    if(cnt == 7) {
        flag = true;
        return ;
    }
    for(int i = 1; i <= 7 - size[cnt + 1]; i++) {
        for(int j = 1; j <= 7 - size[cnt + 1]; j++) {
            dfs(i, j, cnt + 1);
        }
    }
    for(int i = x; i < x + size[cnt]; i++) {
        for(int j = y; j < y + size[cnt]; j++) {
            temp[i][j]++;
        }
    }
    if(flag) {
        return;
    }
    if(TipVec.size()) {
        TipVec.pop_back ();
    }
}
