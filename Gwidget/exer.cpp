#include "exer.h"
#include <QIcon>
#include <QMovie>
#include <QPainter>
#include "gwidget.h"
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpacerItem>
#include "Explanation.h"
#include <QFontDatabase>
#include "pramodedialog.h"
#include <QSoundEffect>
#include <QThreadPool>
#include <QThread>
Exer::Exer(GWidget *MainWin, QWidget *parent)
    : QWidget{parent}
{
    DragBtn = new DragButton(this);
    DragBtn->setGeometry (1380, 130, 120, 50);
    connect(DragBtn, &DragButton::stop, this, [ = ] {
        emit stop ();
    });
    connect(DragBtn, &DragButton::goon, this, [ = ] {
        emit goon();
    });
    QCursor *myCursor = new QCursor(QPixmap(":/Green/images/Arrow.png"));
    this->setCursor (*myCursor);
    this->setWindowFlag (Qt::MSWindowsFixedSizeDialogHint);
    this->resize (1600, 900);
    this->setFixedSize (this->width (), this->height());
    //背景
    QPixmap ImgAllbackground(":/Green/images/back4.jpg");
    fitimgpic = ImgAllbackground.scaled(this->width(), this->height(), Qt::IgnoreAspectRatio,  Qt::SmoothTransformation);
    if(fitimgpic.isNull ()) {
        qDebug() << "加载背景图片失败";
    }
    //资源的加载
    // QLabel ImgLabel = QLabel(this);
    // ImgLabel.setGeometry(50, 50, 300, 200);
    // QPixmap pixmap(":/Green/images/bj.png");
    // pixmap = pixmap.scaled(ImgLabel.size(), Qt::KeepAspectRatio); // 缩放图片
    // QPalette palette;
    // palette.setBrush(QPalette::Window, pixmap);
    // ImgLabel.setPalette(palette);
    // ImgLabel.setAutoFillBackground(true);
    QFont font("经典繁毛楷", 18);
    QFont buttonfont("FZZJ-QDXKJW", 26);
    buttonfont.setBold (true);
    QIcon HomeIcon = QIcon(":/Green/images/back.png");
    if (HomeIcon.isNull ()) {
        qDebug() << "加载返回按钮图片失败";
        return;
    }
    QIcon ExplanIcon = QIcon(":/Green/images/explan.png");
    if (ExplanIcon.isNull ()) {
        qDebug() << "加载规则说明按钮图片失败";
        return;
    }
    //对于返回按钮和规则说明按钮的初始化和设置
    QIcon GetMapIcon = QIcon(":/Green/images/getmap1.png");
    if (GetMapIcon.isNull ()) {
        qDebug() << "加载创建地图按钮图片失败";
        return;
    }
    QIcon GetMapIcon2 = QIcon(":/Green/images/drag.png");
    if (GetMapIcon2.isNull ()) {
        qDebug() << "加载创建地图按钮图片失败";
        return;
    }
    QPushButton *GetMapBtn = new QPushButton(this);
    GetMapBtn->setText ("创建地图（拖拽）");
    GetMapBtn->setFont(font);
    GetMapBtn->setGeometry (60, 60, 250, 50);
    GetMapBtn->setIcon (GetMapIcon);
    GetMapBtn->setIconSize (QSize(30, 30));
    GetMapBtn->setStyleSheet("QPushButton {"
                             "border: 1px solid black;"
                             "background-color: #E0EEEE;"
                             "border-radius: 8px;"
                             "color:black;"
                             "}"
                             "QPushButton:hover {"
                             "background-color: #87CEFF;"
                             "}");
    QPushButton *GetMapBtn2 = new QPushButton(this);
    GetMapBtn2->setText ("创建地图（矩阵）");
    GetMapBtn2->setFont(font);
    GetMapBtn2->setGeometry (1290, 60, 250, 50);
    GetMapBtn2->setIcon (GetMapIcon2);
    GetMapBtn2->setIconSize (QSize(30, 30));
    GetMapBtn2->setStyleSheet("QPushButton {"
                              "border: 1px solid black;"
                              "background-color: #E0EEEE;"
                              "border-radius: 8px;"
                              "color:black;"
                              "}"
                              "QPushButton:hover {"
                              "background-color: #87CEFF;"
                              "}");
    connect (GetMapBtn, &QPushButton::clicked, this, [ = ] {
        // this->setVisible (false);
        // GetMap *getmap = new GetMap(this);
        // getmap->setWindowFlags(Qt::Window);
        // //自动释放资源
        // getmap->setAttribute(Qt::WA_DeleteOnClose);
        // getmap->setVisible(true);
        // connect(getmap, &GetMap::ReturnMin, this, [ = ] {
        //     this->setVisible(true);
        // });


        GetMap2 *getmap2 = new GetMap2(this);
        getmap2->setWindowFlags(Qt::Window);
        //自动释放资源
        getmap2->setAttribute(Qt::WA_DeleteOnClose);
        getmap2->setVisible(true);
        connect(getmap2, &GetMap2::ReturnMain, this, [ = ] {
            this->setVisible(true);
        });
    });
    connect (GetMapBtn2, &QPushButton::clicked, this, [ = ] {
        GetMap *getmap = new GetMap(this);
        getmap->setWindowFlags(Qt::Window);
        //自动释放资源
        getmap->setAttribute(Qt::WA_DeleteOnClose);
        getmap->setVisible(true);



    });
    Home = new QPushButton(tr("返回主页面"), this);
    Home->setFont (font);
    Home->setGeometry (1370, 790, 180, 50);
    Home->setIcon (HomeIcon);
    Home->setIconSize (QSize(30, 30));
    Home->setStyleSheet("QPushButton {"
                        "border: 1px solid black;"
                        "background-color: #E0EEEE;"
                        "border-radius: 8px;"
                        "color:black;"
                        "}"
                        "QPushButton:hover {"
                        "background-color: #87CEFF;"
                        "}");
    Explan = new QPushButton(this);
    Explan->setGeometry (60, 790, 180, 50);
    Explan->setFont (font);
    Explan->setText (tr("规则说明"));
    Explan->setIcon (ExplanIcon);
    Explan->setIconSize (QSize(30, 30));
    Explan->setStyleSheet("QPushButton {"
                          "border: 1px solid black;"
                          "background-color: #E0EEEE;"
                          "border-radius: 8px;"
                          "color:black;"
                          "}"
                          "QPushButton:hover {"
                          "background-color: #87CEFF;"
                          "}");
    //设置布局
    QHBoxLayout *hlayout = new QHBoxLayout(this);
    QVBoxLayout *ButtonLayout = new QVBoxLayout();
    QSpacerItem *LeftSpaceItem = new QSpacerItem(600, 0, QSizePolicy::Fixed, QSizePolicy::Preferred);
    QSpacerItem *RightSpaceItem = new QSpacerItem(600, 0, QSizePolicy::Fixed, QSizePolicy::Preferred);
    int ButtonNum = getMapNum ();
    font.setBold (true);
    for(int i = 1; i <= ButtonNum; ++i) {
        QHBoxLayout *layout = new QHBoxLayout();
        QPushButton *button = new QPushButton(this);
        QString ButtonText = QString("关卡 %1").arg(i);
        QLabel *label = new QLabel(this);
        QLabel *tlabel = new QLabel(this);
        tlabel->setFixedSize(130, 30);
        tlabel->setText(" 难度系数：");
        label->setFixedSize (85, 85);
        tlabel->setStyleSheet("QLabel {"
                              "background-color: transparent;"
                              "font-family:FZZJ-QDXKJW;"
                              "font-size:26px;"
                              "font-weight:1000;"
                              "color:black;"
                              "}");
        QString HRate = ":/Green/images/star%1.png";
        button->setText (ButtonText);
        button->setFixedSize (QSize(250, 40));
        button->setStyleSheet("QPushButton {"
                              "border: 1px solid black;"
                              "background-color: #FDF5E6;"
                              "border-radius: 4px;"
                              "color:black;"
                              "}"
                              "QPushButton:hover {"
                              "background-color: #87CEFF;"
                              "}");
        button->setFont(buttonfont);
        layout->addWidget(button);
        layout->addWidget (tlabel);
        layout->addSpacerItem(new QSpacerItem(5, 50));
        QString CurMapStr = QString("./map/map%1.txt").arg(i);
        QFile MapFile(CurMapStr);
        if(MapFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&MapFile);
            int num = in.readLine().toInt();
            QPixmap *pixmap = new QPixmap(HRate.arg(num));
            pixmap->scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            label->setScaledContents(true);
            label->setPixmap(*pixmap);
            layout->addWidget (label);
            layout->setSpacing (0);
        }
        button->raise();
        label->raise();
        ButtonLayout->addLayout(layout);
        connect(button, &QPushButton::clicked, this, [ = ] {
            QSoundEffect *startSound = new QSoundEffect(this); //创建一个音乐播放器
            startSound->setSource(QUrl("qrc:/Green/images/btnclick.wav"));
            startSound->setVolume(0.5f);
            startSound->play();
            PraModeDialog *praModeDlg = new PraModeDialog(this, CurMapStr);
            praModeDlg->show();
            this->hide();
            connect(praModeDlg, SIGNAL(ReturnMain()), this, SLOT(show()));
        });
    }
    QPushButton *button = new QPushButton(this);
    QString ButtonText = QString("随机关卡");
    button->setText (ButtonText);
    button->setFixedSize (QSize(250, 40));
    button->setStyleSheet("QPushButton {"
                          "border: 1px solid black;"
                          "background-color: #FDF5E6;"
                          "border-radius: 4px;"
                          "color:black;"
                          "}"
                          "QPushButton:hover {"
                          "background-color: #87CEFF;"
                          "}");
    button->setFont(buttonfont);
    ButtonLayout->addWidget (button);
    connect(button, &QPushButton::clicked, this, [ = ] {
        QSoundEffect *startSound = new QSoundEffect(this); //创建一个音乐播放器
        startSound->setSource(QUrl("qrc:/Green/images/btnclick.wav"));
        startSound->setVolume(0.5f);
        startSound->play();
        PraModeDialog *praModeDlg = new PraModeDialog(parent, "");
        praModeDlg->setWindowTitle (tr("练习模式"));
        QIcon WinIcon = QIcon(":/Green/images/exercise.png");
        praModeDlg->setWindowIcon (WinIcon);
        praModeDlg->show();
        this->hide();
        connect(praModeDlg, SIGNAL(ReturnMain()), this, SLOT(show()));
    });
    button->raise();
    hlayout->addSpacerItem (LeftSpaceItem);
    hlayout->addLayout (ButtonLayout);
    hlayout->addSpacerItem (RightSpaceItem);
    //设置按钮在最顶层
    Explan->raise();
    Home->raise();
    // ImgLabel.raise ();
    //设置点击按钮关闭窗口
    connect(Home, &QPushButton::clicked, this, [ = ] {
        QSoundEffect *startSound = new QSoundEffect(this); //创建一个音乐播放器
        startSound->setSource(QUrl("qrc:/Green/images/btnclick.wav"));
        startSound->setVolume(0.5f);
        startSound->play();
        this->close();
    });
    //打开规则说明窗口
    connect(Explan, &QPushButton::clicked, this, [ = ] {
        QSoundEffect *startSound = new QSoundEffect(this); //创建一个音乐播放器
        startSound->setSource(QUrl("qrc:/Green/images/btnclick.wav"));
        startSound->setVolume(0.5f);
        startSound->play();
        this->openExplan ();
    });
    //退出时打开原窗口
    connect(this, &Exer::aboutToClose, MainWin, &GWidget::showOriginWindow);
}
void Exer::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    // 确保当窗口大小改变时图片能适应整个窗口
    painter.drawPixmap(rect(), fitimgpic);
}
//打开规则说明窗口
void Exer::openExplan()
{
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
}
//重写关闭窗口事件
void Exer::closeEvent(QCloseEvent *Event)
{
    emit aboutToClose();
    Event->accept();
}
//显示原来窗口的槽函数
void Exer::showOriginWindow ()
{
    this->setVisible (true);
}
//查看现在一共有多少个关卡
int Exer::getMapNum()
{
    int MapNum = 0;
    QDir MapDir(DirPath);
    QFileInfoList entries = MapDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    for(const QFileInfo &entry : entries) {
        if(entry.isFile ()) {
            MapNum += 1;
        }
    }
    return MapNum;
}
