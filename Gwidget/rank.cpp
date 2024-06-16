#include <QFile>
#include <QFont>
#include <QDebug>
#include <QLabel>
#include "rank.h"
#include <QPixmap>
#include <QPainter>
#include <QPalette>
#include "gwidget.h"
#include <QBoxLayout>
#include <QTableView>
#include <QJsonArray>
#include <QJsonObject>
#include <QHeaderView>
#include <QPushButton>
#include <QCloseEvent>
#include <QJsonDocument>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QSoundEffect>
//固定列宽
const int fixedColumnWidth = 250;

Rank::Rank(GWidget *MainWin, QWidget *parent)
    : QWidget{parent}
{
    DragBtn = new DragButton(this);
    DragBtn->setGeometry (1380, 70, 120, 50);
    connect(DragBtn, &DragButton::stop, this, [ = ] {
        emit stop ();
    });
    connect(DragBtn, &DragButton::goon, this, [ = ] {
        emit goon();
    });
    QCursor *myCursor = new QCursor(QPixmap(":/Green/images/Arrow.png"));
    this->setCursor (*myCursor);
    //字体颜色
    QBrush brush(Qt::white);
    //设置布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    this->setLayout(layout);
    QHBoxLayout *hlayout = new QHBoxLayout();
    layout->addLayout (hlayout);
    //初始化
    RankTitle = new QLabel(tr("通关次数排行"), this);
    VicButton = new QPushButton(tr("通关次数"), this);
    IdButton = new QPushButton(tr("ID序号"), this);
    PlayButton = new QPushButton(tr("闯关总数"), this);
    MinButton = new QPushButton(tr("通关时间"), this);
    //预先加载资源
    QFont ButtonFont = QFont("FZZJ-QDXKJW", 22);
    ButtonFont.setBold (true);
    QIcon VicIcon = QIcon(":/Green/images/vic.png");
    if (VicIcon.isNull ()) {
        qDebug() << "加载通关次数Icon失败";
        return;
    }
    VicButton->setIcon (VicIcon);
    QIcon IdIcon = QIcon(":/Green/images/id.png");
    if (IdIcon.isNull ()) {
        qDebug() << "加载ID序号Icon失败";
        return;
    }
    IdButton->setIcon (IdIcon);
    QIcon PlayIcon = QIcon(":/Green/images/play.png");
    if (PlayIcon.isNull ()) {
        qDebug() << "加载闯关总数Icon失败";
        return;
    }
    PlayButton->setIcon (PlayIcon);
    QIcon MinIcon = QIcon(":/Green/images/min.png");
    if (MinIcon.isNull ()) {
        qDebug() << "加载通关时间Icon失败";
        return;
    }
    MinButton->setIcon (MinIcon);
    //设置外观样式
    VicButton->setIconSize (QSize(25, 25));
    IdButton->setIconSize (QSize(25, 25));
    PlayButton->setIconSize (QSize(25, 25));
    MinButton->setIconSize (QSize(25, 25));
    VicButton->setFont (ButtonFont);
    IdButton->setFont (ButtonFont);
    PlayButton->setFont (ButtonFont);
    MinButton->setFont (ButtonFont);
    VicButton->setStyleSheet("QPushButton {"
                             "border: 1px solid black;"
                             "background-color: #E0EEEE;"
                             "border-radius: 4px;"
                             "}"
                             "QPushButton:hover {"
                             "background-color: #87CEFF;"
                             "}");
    VicButton->setFixedHeight (40);
    IdButton->setStyleSheet("QPushButton {"
                            "border: 1px solid black;"
                            "background-color: #E0EEEE;"
                            "border-radius: 4px;"
                            "}"
                            "QPushButton:hover {"
                            "background-color: #87CEFF;"
                            "}");
    IdButton->setFixedHeight (40);
    PlayButton->setStyleSheet("QPushButton {"
                              "border: 1px solid black;"
                              "background-color: #E0EEEE;"
                              "border-radius: 4px;"
                              "}"
                              "QPushButton:hover {"
                              "background-color: #87CEFF;"
                              "}");
    PlayButton->setFixedHeight (40);
    MinButton->setStyleSheet("QPushButton {"
                             "border: 1px solid black;"
                             "background-color: #E0EEEE;"
                             "border-radius: 4px;"
                             "}"
                             "QPushButton:hover {"
                             "background-color: #87CEFF;"
                             "}");
    MinButton->setFixedHeight (40);
    //设置布局
    hlayout->addItem (new QSpacerItem(80, 0, QSizePolicy::Preferred, QSizePolicy::Fixed));
    hlayout->addWidget (VicButton);
    hlayout->addItem (new QSpacerItem(80, 0, QSizePolicy::Preferred, QSizePolicy::Fixed));
    hlayout->addWidget (IdButton);
    hlayout->addItem (new QSpacerItem(80, 0, QSizePolicy::Preferred, QSizePolicy::Fixed));
    //设置排行榜样式
    RankTitle ->setFixedSize (300, 80);
    RankTitle->setAlignment (Qt::AlignCenter);
    RankTitle->setStyleSheet ("QLabel{"
                              "background-color: transparent;"
                              "font-family:FZZJ-QDXKJW;"
                              "font-size:52px;"
                              "font-weight:1000;"
                              "color:#98FB98;"
                              "}");
    hlayout->addWidget(RankTitle);
    hlayout->addItem (new QSpacerItem(80, 0, QSizePolicy::Preferred, QSizePolicy::Fixed));
    hlayout->addWidget (PlayButton);
    hlayout->addItem (new QSpacerItem(80, 0, QSizePolicy::Preferred, QSizePolicy::Fixed));
    hlayout->addWidget (MinButton);
    hlayout->addItem (new QSpacerItem(80, 0, QSizePolicy::Preferred, QSizePolicy::Fixed));
    this->setWindowFlag (Qt::MSWindowsFixedSizeDialogHint);//固定大小
    this->resize (1600, 900);
    this->setFixedSize (this->width (), this->height());
    //RankItem是用于保存排行榜信息的一个结构体
    //设置背景
    QPixmap ImgAllbackground(":/Green/images/yui.png");
    fitimgpic = ImgAllbackground.scaled(this->width(), this->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    //加载json文件中的信息
    loadNewRank ();
    //创建TableView用于显示排行榜
    tableView = new QTableView(this);
    layout->addWidget(tableView);
    //设置居中
    tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    tableView->verticalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    RankModel = new QStandardItemModel(RankVec.size(), 6, tableView);
    //初始化排行榜的五列
    RankModel->setHeaderData(0, Qt::Horizontal, tr("排行"));
    RankModel->setHeaderData(1, Qt::Horizontal, tr("玩家昵称"));
    RankModel->setHeaderData(2, Qt::Horizontal, tr("玩家ID"));
    RankModel->setHeaderData(3, Qt::Horizontal, tr("通关次数"));
    RankModel->setHeaderData(4, Qt::Horizontal, tr("闯关总数"));
    RankModel->setHeaderData(5, Qt::Horizontal, tr("通关最短时间"));
    QFont font = QFont("FZZJ-QDXKJW", 26);
    font.setBold (true);
    //设置Rankmodel
    for (int i = 0; i < RankVec.size(); ++i) {
        auto NumItem = new QStandardItem();
        NumItem->setData(QVariant(i + 1), Qt::DisplayRole);
        NumItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
        NumItem->setData(QVariant(brush), Qt::ForegroundRole);
        NumItem->setFont(font);
        RankModel->setItem(i, 0, NumItem);
        auto NameItem = new QStandardItem(RankVec[i].Name);
        NameItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
        NameItem->setData(QVariant(brush), Qt::ForegroundRole);
        NameItem->setFont(font);
        RankModel->setItem(i, 1, NameItem);
        auto IdItem = new QStandardItem(RankVec[i].Id);
        IdItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
        IdItem->setData(QVariant(brush), Qt::ForegroundRole);
        IdItem->setFont(font);
        RankModel->setItem(i, 2, IdItem);
        auto VicItem = new QStandardItem();
        VicItem->setData(QVariant(RankVec[i].VicTimes), Qt::DisplayRole);
        VicItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
        VicItem->setData(QVariant(brush), Qt::ForegroundRole);
        VicItem->setFont(font);
        RankModel->setItem(i, 3, VicItem);
        auto PlayItem = new QStandardItem();
        PlayItem->setData(QVariant(RankVec[i].PlayTimes), Qt::DisplayRole);
        PlayItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
        PlayItem->setData(QVariant(brush), Qt::ForegroundRole);
        PlayItem->setFont(font);
        RankModel->setItem(i, 4, PlayItem);
        auto MinItem = new QStandardItem();
        MinItem->setData(QVariant(RankVec[i].MinTime), Qt::DisplayRole);
        MinItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
        MinItem->setData(QVariant(brush), Qt::ForegroundRole);
        MinItem->setFont(font);
        RankModel->setItem(i, 5, MinItem);
    }
    //将Model设置进tableView
    tableView->setFont (font);
    tableView->setModel (RankModel);
    //设置表头格式
    tableView->horizontalHeader()->setStyleSheet(
        "QHeaderView::section{"
        "background-color: transparent;"
        "font-family:FZZJ-QDXKJW;"
        "font-size:38px;"
        "font-weight:1000;"
        "color:#9AFF9A;"
        "}");
    tableView->verticalHeader()->setVisible (false);
    tableView->resizeRowsToContents();
    tableView->resizeColumnsToContents ();
    tableView->horizontalHeader()->setDefaultSectionSize(fixedColumnWidth);
    tableView->verticalHeader()->setDefaultSectionSize(20);
    tableView->setStyleSheet ("background-color:transparent;color:white;");
    tableView->setShowGrid(false); // 去除网格线
    tableView->setFrameShape(QFrame::NoFrame); // 去除边框
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //设置布局
    tableView->viewport()->update();
    //连接槽函数显示不同排行
    connect(this, &Rank::aboutToClose, MainWin, &GWidget::showOriginWindow);
    connect(VicButton, &QPushButton::clicked, this, [ = ] {
        QSoundEffect *startSound = new QSoundEffect(this); //创建一个音乐播放器
        startSound->setSource(QUrl("qrc:/Green/images/btnclick.wav"));
        startSound->setVolume(0.5f);
        startSound->play();
        this->renewVic ();
    });
    connect(IdButton, &QPushButton::clicked, this, [ = ] {
        QSoundEffect *startSound = new QSoundEffect(this); //创建一个音乐播放器
        startSound->setSource(QUrl("qrc:/Green/images/btnclick.wav"));
        startSound->setVolume(0.5f);
        startSound->play();
        this->renewId ();
    });
    connect(PlayButton, &QPushButton::clicked, this, [ = ] {
        QSoundEffect *startSound = new QSoundEffect(this); //创建一个音乐播放器
        startSound->setSource(QUrl("qrc:/Green/images/btnclick.wav"));
        startSound->setVolume(0.5f);
        startSound->play();
        this->renewPlay ();
    });
    connect(MinButton, &QPushButton::clicked, this, [ = ] {
        QSoundEffect *startSound = new QSoundEffect(this); //创建一个音乐播放器
        startSound->setSource(QUrl("qrc:/Green/images/btnclick.wav"));
        startSound->setVolume(0.5f);
        startSound->play();
        this->renewMin ();
    });
}
//设置背景
void Rank::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    // 确保当窗口大小改变时图片能适应整个窗口
    painter.drawPixmap(rect(), fitimgpic);
}

//重写关闭事件
void Rank::closeEvent(QCloseEvent *Event)
{
    emit aboutToClose();
    Event->accept();
}

//从json文件中加载原先的记录作为一个JsonArray
QJsonArray Rank::loadFileFromJson(QString FilePath)
{
    QFile RankFile(FilePath);
    if (RankFile.open(QIODevice::ReadOnly)) {
        QByteArray RankFileData = RankFile.readAll();
        QJsonDocument RankFileDoc = QJsonDocument::fromJson(RankFileData);
        QJsonArray RankFileArray = RankFileDoc.array();
        RankFile.close();
        return RankFileArray;
    } else {
        qDebug() << "加载排行榜失败";
        return QJsonArray();
    }
}

//将新的数据写入json文件储存
void Rank::writeFileIntoJson(QVector<RankItem> RankVec, QString FilePath)
{
    QJsonArray SortedArray;
    for (const auto &ps : RankVec) {
        QJsonObject playerObj;
        playerObj.insert("id", ps.Id);
        playerObj.insert("name", ps.Name);
        playerObj.insert("vic", ps.VicTimes);
        playerObj.insert("play", ps.PlayTimes);
        playerObj.insert("mint", ps.MinTime);
        SortedArray.append(playerObj);
    }
    QJsonDocument sortedDoc(SortedArray);
    QFile sortedFile(FilePath);
    if (sortedFile.open(QIODevice::Truncate | QIODevice::WriteOnly)) {
        sortedFile.write(sortedDoc.toJson());
        sortedFile.close();
    } else {
        qDebug() << "储存排行榜失败";
        return;
    }
}

//新增一条记录
void Rank::insertNewRecord(RankItem ps)
{
    if(HasRecord[ps.Name]) {
        for(auto &rank : RankVec) {
            if(ps.Name == rank.Name) {
                rank.MinTime = std::min(ps.MinTime, rank.MinTime);
                rank.PlayTimes += 1;
                break;
            }
        }
    } else {
        RankVec.push_back (ps);
        HasRecord[ps.Name] = true;
    }
    std::sort(RankVec.begin(), RankVec.end(), [&](const RankItem & a, const RankItem & b) {
        return a.VicTimes > b.VicTimes;
    });
    //qDebug() << "wolegedou";
    writeFileIntoJson (RankVec, "success.json");
}


void Rank::insertNewVicRecord(RankItem ps)
{
    if(HasRecord[ps.Name]) {
        for(auto &rank : RankVec) {
            if(ps.Name == rank.Name) {
                rank.MinTime = std::min(ps.MinTime, rank.MinTime);
                rank.VicTimes += 1;
                break;
            }
        }
    }
    std::sort(RankVec.begin(), RankVec.end(), [&](const RankItem & a, const RankItem & b) {
        return a.VicTimes > b.VicTimes;
    });
    //qDebug() << "wolegedou";
    writeFileIntoJson (RankVec, "success.json");
}

//加载RankVec
void Rank::loadNewRank()
{
    RankVec.clear();
    QJsonArray SucArray = loadFileFromJson("success.json");
    for (const auto &item : SucArray) {
        QJsonObject obj = item.toObject();
        RankItem ps;
        ps.Name = obj["name"].toString();
        ps.Id = obj["id"].toString();
        ps.VicTimes = obj["vic"].toInt();
        ps.PlayTimes = obj["play"].toInt();
        ps.MinTime = obj["mint"].toString ();
        HasRecord[ps.Name] = true;
        RankVec.push_back(ps);
    }
    std::sort(RankVec.begin(), RankVec.end(), [&](const RankItem & a, const RankItem & b) {
        return a.VicTimes > b.VicTimes;
    });
}

//加载RankModel到TableView
void Rank::reload()
{
    RankModel->clear();
    QBrush brush(Qt::white);
    RankModel = new QStandardItemModel(RankVec.size(), 6, tableView);
    //初始化排行榜的五列
    RankModel->setHeaderData(0, Qt::Horizontal, tr("排行"));
    RankModel->setHeaderData(1, Qt::Horizontal, tr("玩家昵称"));
    RankModel->setHeaderData(2, Qt::Horizontal, tr("玩家ID"));
    RankModel->setHeaderData(3, Qt::Horizontal, tr("通关次数"));
    RankModel->setHeaderData(4, Qt::Horizontal, tr("闯关总数"));
    RankModel->setHeaderData(5, Qt::Horizontal, tr("通关最短时间"));
    QFont font = QFont("FZZJ-QDXKJW", 26);
    font.setBold (true);
    //设置Rankmodel
    for (int i = 0; i < RankVec.size(); ++i) {
        auto NumItem = new QStandardItem();
        NumItem->setData(QVariant(i + 1), Qt::DisplayRole);
        NumItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
        NumItem->setData(QVariant(brush), Qt::ForegroundRole);
        NumItem->setFont(font);
        RankModel->setItem(i, 0, NumItem);
        auto NameItem = new QStandardItem(RankVec[i].Name);
        NameItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
        NameItem->setData(QVariant(brush), Qt::ForegroundRole);
        NameItem->setFont(font);
        RankModel->setItem(i, 1, NameItem);
        auto IdItem = new QStandardItem(RankVec[i].Id);
        IdItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
        IdItem->setData(QVariant(brush), Qt::ForegroundRole);
        IdItem->setFont(font);
        RankModel->setItem(i, 2, IdItem);
        auto VicItem = new QStandardItem();
        VicItem->setData(QVariant(RankVec[i].VicTimes), Qt::DisplayRole);
        VicItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
        VicItem->setData(QVariant(brush), Qt::ForegroundRole);
        VicItem->setFont(font);
        RankModel->setItem(i, 3, VicItem);
        auto PlayItem = new QStandardItem();
        PlayItem->setData(QVariant(RankVec[i].PlayTimes), Qt::DisplayRole);
        PlayItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
        PlayItem->setData(QVariant(brush), Qt::ForegroundRole);
        PlayItem->setFont(font);
        RankModel->setItem(i, 4, PlayItem);
        auto MinItem = new QStandardItem();
        MinItem->setData(QVariant(RankVec[i].MinTime), Qt::DisplayRole);
        MinItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
        MinItem->setData(QVariant(brush), Qt::ForegroundRole);
        MinItem->setFont(font);
        //MinItem->setForeground (brush);
        RankModel->setItem(i, 5, MinItem);
    }
    //将Model设置进tableView
    tableView->setFont (font);
    tableView->setModel (RankModel);
    tableView->viewport()->update(); // 强制视图重绘
}


//按Id排列
void Rank::renewId()
{
    RankTitle->setText (tr("ID排行"));
    std::sort(RankVec.begin(), RankVec.end(), [&](const RankItem & a, const RankItem & b) {
        return a.Id < b.Id;
    });
    reload();
}


//按闯关总数排列
void Rank::renewPlay()
{
    RankTitle->setText (tr("闯关总数排行"));
    std::sort(RankVec.begin(), RankVec.end(), [&](const RankItem & a, const RankItem & b) {
        return a.PlayTimes > b.PlayTimes;
    });
    reload();
}


//按通关时间排列
void Rank::renewMin()
{
    RankTitle->setText (tr("通关时间排行"));
    std::sort(RankVec.begin(), RankVec.end(), [&](const RankItem & a, const RankItem & b) {
        return a.MinTime < b.MinTime;
    });
    reload();
}


//按通关次数排列
void Rank::renewVic()
{
    RankTitle->setText (tr("通关次数排行"));
    std::sort(RankVec.begin(), RankVec.end(), [&](const RankItem & a, const RankItem & b) {
        return a.VicTimes > b.VicTimes;
    });
    reload();
}

bool Rank::hasRecord(QString Name)
{
    return HasRecord[Name] == true;
}
