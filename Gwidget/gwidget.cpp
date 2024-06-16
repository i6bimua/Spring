#include <QSize>
#include <QFont>
#include <QRect>
#include "exer.h"
#include "comp.h"
#include "rank.h"
#include <QBrush>
#include <QPixmap>
#include <QPalette>
#include "gwidget.h"
#include <QSpacerItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <QImageReader>
#include <QFontDatabase>
#include "./ui_gwidget.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QVideoSink>
#include <QVideoWidget>
#include <QBoxLayout>
#include <QSpacerItem>
#include "dragbutton.h"
GWidget::GWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GWidget)
{
    QCursor *myCursor = new QCursor(QPixmap(":/Green/images/Arrow.png"));
    this->setCursor (*myCursor);
    ui->setupUi(this);
    startSound = new QMediaPlayer(this);
    QAudioOutput *audioOutput = new QAudioOutput(this);
    startSound->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.5);
    startSound->setSource(QUrl("qrc:/Green/images/bbgm.wav"));
    startSound->setLoops (10000);
    startSound->play();
    DragBtn = new DragButton(this);
    DragBtn->setGeometry (1380, 70, 120, 50);
    connect(DragBtn, &DragButton::stop, startSound, [ = ] {
        startSound->pause ();
    });
    connect(DragBtn, &DragButton::goon, startSound, [ = ] {
        startSound->play();
    });
    //加载字体
    // QMediaPlayer *player = new QMediaPlayer(this);
    // player->setSource(QUrl("qrc:/Green/images/back.mp4"));
    // QVideoWidget *videoWidget = new QVideoWidget(this);
    // player->setVideoOutput(videoWidget);
    // videoWidget->resize (1600, 200);
    // player->setLoops (10000);
    // videoWidget->setWindowFlags (Qt::WindowStaysOnBottomHint);
    // videoWidget->lower ();
    // videoWidget->show();
    // player->play();
    //videoWidget->setAspectRatioMode(Qt::IgnoreAspectRatio);
    int id = QFontDatabase::addApplicationFont(":/Green/Font/经典繁毛楷.TTF");
    if (id == -1) {
        qDebug() << "加载字体失败";
        return;
    }
    // QVBoxLayout *vlayout = new QVBoxLayout(this);
    // QHBoxLayout *hlayout = new QHBoxLayout();
    // vlayout->addWidget(videoWidget, 8);
    // vlayout->addLayout(hlayout, 0);
    // 获取加载成功后的字体家族名称
    QStringList families = QFontDatabase::applicationFontFamilies(id);
    if (families.isEmpty()) {
        qDebug() << "字体没有对应的字体族";
        return;
    }
    int id2 = QFontDatabase::addApplicationFont(":/Green/Font/方正字迹-启笛小楷简体.TTF");
    if (id2 == -1) {
        qDebug() << "加载字体失败";
        return;
    }
    // 获取加载成功后的字体家族名称
    QStringList families2 = QFontDatabase::applicationFontFamilies(id);
    if (families2.isEmpty()) {
        qDebug() << "字体没有对应的字体族";
        return;
    }
    //设置标题和窗口大小
    this->setWindowTitle (tr("植此青绿"));
    QIcon MainIcon = QIcon(":/Green/images/plant.png");
    if (MainIcon.isNull ()) {
        qDebug() << "加载主页面Icon失败";
        return;
    }
    this->setWindowIcon(MainIcon);
    this->setWindowFlag (Qt::MSWindowsFixedSizeDialogHint);
    this->resize (1600, 900);
    this->setFixedSize (this->width (), this->height());
    //主界面的三个pushbutton初始化新建
    exercise = new QPushButton(tr("练习模式"), this);
    compete = new QPushButton(tr("比赛模式"), this);
    rank = new QPushButton(tr("排行榜"), this);
    exercise->setFixedHeight (70);
    compete->setFixedHeight (70);
    rank->setFixedHeight (70);
    // hlayout->addSpacerItem(new QSpacerItem(120, 70));
    // hlayout->addWidget (exercise);
    // hlayout->addSpacerItem(new QSpacerItem(120, 70));
    // hlayout->addWidget (compete);
    // hlayout->addSpacerItem(new QSpacerItem(120, 70));
    // hlayout->addWidget (rank);
    // hlayout->addSpacerItem(new QSpacerItem(120, 70));
    //设置标题字体和大小
    QFont font(families.first(), 22);
    exercise->setFont(font);
    compete->setFont(font);
    rank->setFont(font);
    //设置按钮的高度
    //设置按钮的Icon并确保加载成功
    QIcon ExerIcon = QIcon(":/Green/images/exercise.png");
    if (ExerIcon.isNull ()) {
        qDebug() << "加载练习模式Icon失败";
        return;
    }
    QIcon CompIcon = QIcon(":/Green/images/compete.png");
    if (CompIcon.isNull ()) {
        qDebug() << "加载比赛模式Icon失败";
        return;
    }
    QIcon RankIcon = QIcon(":/Green/images/rank.png");
    if (RankIcon.isNull ()) {
        qDebug() << "加载排行榜Icon失败";
        return;
    }
    exercise->setIcon (ExerIcon);
    compete->setIcon (CompIcon);
    rank->setIcon (RankIcon);
    //设置Icon的大小
    exercise->setIconSize (QSize(30, 30));
    compete->setIconSize (QSize(30, 30));
    rank->setIconSize (QSize(30, 30));
    //创建布局管理器
    exercise->setGeometry (100, 720, 320, 70);
    compete->setGeometry (645, 720, 320, 70);
    rank->setGeometry (1190, 720, 320, 70);
    //设置背景
    QPixmap BackgroundPixmap = QPixmap(":/Green/images/back.webp");
    BackgroundPixmap = BackgroundPixmap.scaled(this->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    // 创建QBrush并设置图片
    QBrush backgroundBrush(BackgroundPixmap);
    backgroundBrush.setStyle(Qt::BrushStyle::TexturePattern);
    // 获取当前窗口或控件的调色板
    QPalette palette;
    palette = this->palette();
    // 设置背景色为图片画刷
    palette.setBrush(QPalette::Window, backgroundBrush);
    // 将新调色板应用到控件
    this->setPalette(palette);
    this->setAutoFillBackground(true); // 必须启用自动填充背景
    //videoWidget->lower();
    exercise->raise();
    compete->raise();
    rank->raise();
    //三个按钮的槽函数的连接，用于新建一个新页面
    connect (exercise, &QPushButton::clicked, this, [ = ] {
        QSoundEffect *startSound = new QSoundEffect(this); //创建一个音乐播放器
        startSound->setSource(QUrl("qrc:/Green/images/btnclick.wav"));
        startSound->setVolume(0.5f);
        startSound->play();
        this->openExer ();
    });
    connect (compete, &QPushButton::clicked, this, [ = ] {
        QSoundEffect *startSound = new QSoundEffect(this); //创建一个音乐播放器
        startSound->setSource(QUrl("qrc:/Green/images/btnclick.wav"));
        startSound->setVolume(0.5f);
        startSound->play();
        this->openComp ();
    });
    connect (rank, &QPushButton::clicked, this, [ = ] {
        QSoundEffect *startSound = new QSoundEffect(this); //创建一个音乐播放器
        startSound->setSource(QUrl("qrc:/Green/images/btnclick.wav"));
        startSound->setVolume(0.5f);
        startSound->play();
        this->openRank ();
    });
}

GWidget::~GWidget()
{
    delete ui;
}

void GWidget::openExer()
{
    nMode = Mode::PraMode;
    //新建页面并且将原主界面隐藏
    Exer *exer = new Exer(this, this);
    this->setVisible (false);
    connect(exer, &Exer::stop, startSound, [ = ] {
        startSound->pause ();
    });
    connect(exer, &Exer::goon, startSound, [ = ] {
        startSound->play();
    });
    // 设置子窗口为顶级窗口
    exer->setWindowFlags(Qt::Window);
    //自动释放资源
    exer->setAttribute(Qt::WA_DeleteOnClose);
    //设置窗口的大小以及标题
    exer->setWindowTitle (tr("练习模式"));
    QIcon ExerIcon = QIcon(":/Green/images/exercise.png");
    if (ExerIcon.isNull ()) {
        qDebug() << "加载练习模式Icon失败";
        return;
    }
    exer->setWindowIcon(ExerIcon);
    exer->setWindowFlag (Qt::MSWindowsFixedSizeDialogHint);//固定大小
    exer->resize (1600, 900);
    exer->setFixedSize (this->width (), this->height());
    exer->show();
}

void GWidget::openComp()
{
    nMode = Mode::GameMode;
    //新建页面并且将原主界面隐藏
    Comp *comp = new Comp(this, this);
    this->setVisible (false);
    connect(comp, &Comp::stop, startSound, [ = ] {
        startSound->pause ();
    });
    connect(comp, &Comp::goon, startSound, [ = ] {
        startSound->play();
    });
    // 设置子窗口为顶级窗口
    comp->setWindowFlags(Qt::Window);
    //自动释放资源
    comp->setAttribute(Qt::WA_DeleteOnClose);
    //设置窗口的大小以及标题
    QIcon CompIcon = QIcon(":/Green/images/compete.png");
    if (CompIcon.isNull ()) {
        qDebug() << "加载比赛模式Icon失败";
        return;
    }
    comp->setWindowIcon(CompIcon);
    comp->setWindowTitle (tr("比赛模式"));
    comp->setWindowFlag (Qt::MSWindowsFixedSizeDialogHint);//固定大小
    comp->resize (1600, 900);
    comp->setFixedSize (this->width (), this->height());
    comp->show();
}

void GWidget::openRank()
{
    //新建页面并且将原主界面隐藏
    Rank *rank = new Rank(this, this);
    this->setVisible (false);
    connect(rank, &Rank::stop, startSound, [ = ] {
        startSound->pause ();
    });
    connect(rank, &Rank::goon, startSound, [ = ] {
        startSound->play();
    });
    // 设置子窗口为顶级窗口
    rank->setWindowFlags(Qt::Window);
    //自动释放资源
    rank->setAttribute(Qt::WA_DeleteOnClose);
    //设置窗口的大小以及标题
    QIcon RankIcon = QIcon(":/Green/images/rank.png");
    if (RankIcon.isNull ()) {
        qDebug() << "加载排行榜Icon失败";
        return;
    }
    rank->setWindowIcon(RankIcon);
    rank->setWindowTitle (tr("排行榜"));
    rank->setWindowFlag (Qt::MSWindowsFixedSizeDialogHint);//固定大小
    rank->resize (1600, 900);
    rank->setFixedSize (this->width (), this->height());
    rank->show();
}


//显示原窗口
void GWidget::showOriginWindow()
{
    this->setVisible (true);
    this->DragBtn->setVisible (true);
    this->DragBtn->raise ();
}

