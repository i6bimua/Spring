#include "comp.h"
#include "gwidget.h"
#include "rank.h"
#include <QBoxLayout>
#include <QCloseEvent>
#include "tcpserver.h"
#include "tcpclient.h"
#include <QLineEdit>
#include <QMessageBox>
#include <QApplication>
#include "pramodedialog.h"
#include "pasword.h"
#include <QSoundEffect>
Comp::Comp(GWidget *MainWin, QWidget *parent)
    : QWidget{parent}
{
    //设置窗口的外观
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
    StartId = PassWord.size() + 1;
    loadUser ();
    VicNew = false;
    QLabel *TitleLabel = new QLabel(this);
    TitleLabel->setText (tr("登录页面"));
    TitleLabel->setStyleSheet ("QLabel{"
                               "background-color: transparent;"
                               "font-family:FZZJ-QDXKJW;"
                               "font-size:52px;"
                               "font-weight:1000;"
                               "color:#808000;"
                               "}");
    TitleLabel->setGeometry (710, 50, 250, 70);
    PasWord *SetPassWordEdit = new PasWord(this);
    QLabel *SetTextLabel = new QLabel(this);
    QPushButton *AckSet = new QPushButton(this);
    SetTextLabel->setVisible (false);
    QFont fffont = QFont("FZZJ-QDXKJW", 12);
    fffont.setBold(true);
    AckSet->setFont(fffont);
    AckSet->setVisible (false);
    SetPassWordEdit->setVisible (false);
    QFont hfont("经典繁毛楷", 18);
    QIcon HomeIcon = QIcon(":/Green/images/back.png");
    if (HomeIcon.isNull ()) {
        qDebug() << "加载返回按钮图片失败";
        return;
    }
    Home = new QPushButton(tr("返回主页面"), this);
    Home->setFont (hfont);
    Home->setGeometry (1370, 60, 180, 50);
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
    //设置背景
    // tcpserver = new TcpServer(this);
    // tcpclient = new TcpClient(this);
    // tcpclient->setVisible (false);
    // tcpserver->setVisible (false);
    QPalette PAllbackground = this->palette();
    QImage ImgAllbackground(":/Green/images/back2.png");
    QImage fitimgpic = ImgAllbackground.scaled(this->width(), this->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    PAllbackground.setBrush(QPalette::Window, QBrush(fitimgpic));
    this->setPalette(PAllbackground);
    QIcon NetIcon = QIcon(":/Green/images/net.png");
    if (NetIcon.isNull ()) {
        qDebug() << "加载服务器窗口Icon失败";
        return;
    }
    QIcon StartIcon = QIcon(":/Green/images/start.png");
    if (NetIcon.isNull ()) {
        qDebug() << "加载开始按钮Icon失败";
        return;
    }
    rank = new Rank(MainWin, this);
    rank->setVisible (false);
    QFont font = QFont("FZZJ-QDXKJW", 22);
    font.setBold (true);
    openServer = new QPushButton(tr("联网服务器"), this);
    openServer->setIcon (NetIcon);
    openServer->setIconSize (QSize(25, 25));
    openServer->setFont (font);
    openServer->setFixedSize (QSize(250, 50));
    openServer->setStyleSheet("QPushButton {"
                              "border: 1px solid black;"
                              "background-color: #FDF5E6;"
                              "border-radius: 8px;"
                              "}"
                              "QPushButton:hover {"
                              "background-color: #87CEFF;"
                              "}");//在页面关闭的时候显示主页面
    openServer->setGeometry (50, 780, 320, 70);
    QFont Startfont = QFont("FZZJ-QDXKJW", 24);
    Startfont.setBold (true);
    Start = new QPushButton(tr("开始"), this);
    Start->setIcon (StartIcon);
    Start->setIconSize (QSize(25, 25));
    Start->setFont (Startfont);
    Start->setFixedSize (QSize(280, 60));
    Start->setStyleSheet("QPushButton {"
                         "border: 1px solid black;"
                         "background-color: #FDF5E6;"
                         "border-radius: 8px;"
                         "}"
                         "QPushButton:hover {"
                         "background-color: #87CEFF;"
                         "}");//在页面关闭的时候显示主页面
    Start->setGeometry (680, 760, 350, 120);
    openClient = new QPushButton(tr("联网客户端"), this);
    openClient->setIcon (NetIcon);
    openClient->setIconSize (QSize(25, 25));
    openClient->setFont (font);
    openClient->setFixedSize (QSize(250, 50));
    openClient->setStyleSheet("QPushButton {"
                              "border: 1px solid black;"
                              "background-color: #FDF5E6;"
                              "border-radius: 8px;"
                              "}"
                              "QPushButton:hover {"
                              "background-color: #87CEFF;"
                              "}");
    openClient->setGeometry (1300, 780, 320, 70);
    font = QFont("FZZJ-QDXKJW", 24);
    font.setBold (true);
    QLabel *NickNameLabel = new QLabel(this);
    NickNameLabel->setText (tr("请输入昵称"));
    NickNameLabel->setGeometry (500, 150, 250, 70);
    NickNameLabel->setAlignment (Qt::AlignCenter);
    QLineEdit *NickNameEdit = new QLineEdit(this);
    NickNameEdit->setGeometry (760, 150, 400, 70);
    QLabel *PassWordLabel = new QLabel(this);
    PassWordLabel->setText (tr("请输入密码"));
    PassWordLabel->setGeometry (500, 300, 250, 70);
    PassWordLabel->setAlignment (Qt::AlignCenter);
    QFont pasfont = QFont("FZZJ-QDXKJW", 20);
    pasfont.setBold (true);
    PasWord *PassWordEdit = new PasWord(this);
    PassWordEdit->setGeometry (760, 300, 400, 70);
    QPushButton *CheckBtn = new QPushButton(this);
    QPushButton *QuitBtn = new QPushButton(this);
    QPushButton *setPassWord = new QPushButton(this);
    setPassWord->setFont(fffont);
    QuitBtn->setVisible (false);
    setPassWord->setVisible (false);
    QLabel *TextLabel = new QLabel(this);
    TextLabel->setGeometry (650, 600, 350, 100);
    CheckBtn->setText (tr("登录"));
    CheckBtn->setGeometry (730, 500, 200, 50);
    QuitBtn->setText (tr("退出登录"));
    QuitBtn->setGeometry (730, 500, 200, 50);
    setPassWord->setFont(fffont);
    setPassWord->setText (tr("修改密码"));
    setPassWord->setGeometry (980, 515, 70, 30);
    NickNameLabel->setStyleSheet ("QLabel{color:black;"
                                  "background-color:rgba(240,248,255,50%);"
                                  "border: 1px solid black;"
                                  "border-radius: 8px;}");
    NickNameLabel->setWindowOpacity (0.5);
    PassWordLabel->setStyleSheet ("QLabel{color:black;"
                                  "background-color:rgba(240,248,255,50%);"
                                  "border: 1px solid black;"
                                  "border-radius: 8px;}");
    PassWordLabel->setWindowOpacity (0.5);
    TextLabel->setStyleSheet ("QLabel{color:red;"
                              "background-color:rgba(240,248,255,50%);"
                              "border: 1px solid black;"
                              "border-radius: 8px;}");
    TextLabel->setWindowOpacity (0.5);
    TextLabel->setAlignment (Qt::AlignCenter);
    //CheckBtn->setStyleSheet ("QPushButton{color:#98FB98;}");
    NickNameLabel->setFont (font);
    NickNameEdit->setFont (font);
    PassWordLabel->setFont (font);
    PassWordEdit->setFont (pasfont);
    CheckBtn->setFont (font);
    QuitBtn->setFont (font);
    QFont ackfont = QFont("FZZJ-QDXKJW", 12);
    ackfont.setBold(true);
    AckSet->setFont(ackfont);
    TextLabel->setFont (Startfont);
    connect(CheckBtn, &QPushButton::clicked, this, [ = ] {
        QSoundEffect *startSound = new QSoundEffect(this); //创建一个音乐播放器
        startSound->setSource(QUrl("qrc:/Green/images/btnclick.wav"));
        startSound->setVolume(0.5f);
        startSound->play();
        bool ok = true;
        loadUser ();
        QString NickName = NickNameEdit->text ();
        QString UserPas = PassWordEdit->text ();
        StartId = PassWord.size() + 1;
        std::string IdStr = std::to_string(StartId);
        while(IdStr.size() < 5) {
            IdStr = "0" + IdStr;
        }
        QString Id = QString::fromStdString(IdStr);
        if(NickName.size() == 0)
        {
            ok = false;
            TextLabel->setText (tr("昵称不能为空！"));
        }
        if(UserPas.size() == 0)
        {
            ok = false;
            TextLabel->setText (tr("密码不能为空！"));
        }
        if(Id.size() != 5)
        {
            ok = false;
            TextLabel->setText (tr("Id的长度必须是5位！"));
        }
        if(ok)
        {
            bool flag = true;
            if(PassWord.count (NickName)) {
                QMessageBox msgBox;
                if(PassWord[NickName] != UserPas) {
                    msgBox.setWindowTitle (tr("警告"));
                    msgBox.setText (tr("输入密码错误，请重新输入"));
                    msgBox.setStandardButtons (QMessageBox::Yes );
                    QApplication::setQuitOnLastWindowClosed (false);
                    msgBox.exec();
                    return;
                }
                //qDebug() << PassWord[NickName] << UserPas;
                msgBox.setWindowTitle (tr("提示"));
                msgBox.setText (tr("登陆成功,是否使用该账户进行游戏"));
                msgBox.setStandardButtons (QMessageBox::Yes | QMessageBox::No);
                QApplication::setQuitOnLastWindowClosed (false);
                int ret = msgBox.exec();
                if(ret == QMessageBox::No) {
                    ok = false;
                    TextLabel->setText(tr("请重新输入账号信息"));
                }
                flag = false;
                StartId = PassWord.size() + 1;
                IdStr = std::to_string(StartId);
                while(IdStr.size() < 5) {
                    IdStr = "0" + IdStr;
                }
                Id = QString::fromStdString(IdStr);
                ps.Id = Id;
                ps.Name = NickName;
                ps.MinTime = "00:00:00";
                ps.PlayTimes = 1;
                ps.VicTimes = 0;
            } else {
                QMessageBox msgBox;
                msgBox.setWindowTitle (tr("提示"));
                msgBox.setText (tr("未查找到账号，是否一键注册"));
                msgBox.setStandardButtons (QMessageBox::Yes | QMessageBox::No);
                QApplication::setQuitOnLastWindowClosed (false);
                int ret = msgBox.exec();
                if(ret == QMessageBox::No) {
                    flag = false;
                    return;
                }
                PassWord[NickName] = UserPas;
                writeFileIntoJson (PassWord, "user.json");
                loadUser();
                StartId = PassWord.size() + 1;
                IdStr = std::to_string(StartId);
                while(IdStr.size() < 5) {
                    IdStr = "0" + IdStr;
                }
                Id = QString::fromStdString(IdStr);
                TextLabel->setText (tr("创建成功!"));
                ps.Id = Id;
                ps.Name = NickName;
                ps.MinTime = "00:00:00";
                ps.PlayTimes = 1;
                ps.VicTimes = 0;
                ok = 1;
            }
        }
        VicNew |= ok;
        if(VicNew)
        {
            QuitBtn->setVisible(true);
            CheckBtn->setVisible (false);
            setPassWord->setVisible(true);
        }
    });
    this->setWindowOpacity(0.7);
    connect(this, &Comp::aboutToClose, MainWin, &GWidget::showOriginWindow);
    connect(openClient, &QPushButton::clicked, this, [ = ] {
        QSoundEffect *startSound = new QSoundEffect(this); //创建一个音乐播放器
        startSound->setSource(QUrl("qrc:/Green/images/btnclick.wav"));
        startSound->setVolume(0.5f);
        startSound->play();
        this->openCli ();
    });
    connect(openServer, &QPushButton::clicked, this, [ = ] {
        QSoundEffect *startSound = new QSoundEffect(this); //创建一个音乐播放器
        startSound->setSource(QUrl("qrc:/Green/images/btnclick.wav"));
        startSound->setVolume(0.5f);
        startSound->play();
        this->openSer ();
    });
    connect(Start, &QPushButton::clicked, this, [ = ] {
        QSoundEffect *startSound = new QSoundEffect(this); //创建一个音乐播放器
        startSound->setSource(QUrl("qrc:/Green/images/btnclick.wav"));
        startSound->setVolume(0.5f);
        startSound->play();
        if(!VicNew) {
            TextLabel->setText (tr("未成功登录！"));
            return;
        }
        //插入实验数据
        PraModeDialog *praModeDlg = new PraModeDialog(this, "");
        praModeDlg->setWindowTitle (tr("比赛模式"));
        QIcon WinIcon(":/Green/images/compete.png");
        praModeDlg->setWindowIcon (WinIcon);
        praModeDlg->show();
        this->hide();
        connect(praModeDlg, SIGNAL(ReturnMain()), this, SLOT(show()));
        rank->setVisible (false);
        rank->insertNewRecord(ps);
        connect(praModeDlg, &PraModeDialog::VicSig, this, [ = ]{
            ps.PlayTimes = 0;
            ps.VicTimes = 1;
            ps.MinTime = praModeDlg->qtime->toString ("mm:ss.zz");
            rank->setVisible (false);
            rank->insertNewVicRecord(ps);
        });
        connect (praModeDlg, &PraModeDialog::Nextlevel, rank, [ = ]{
            ps.PlayTimes = 1;
            ps.VicTimes = 0;
            ps.MinTime = "00:00:00";
            rank->insertNewRecord (ps);
        });
        if(tcpclient)
        {
            tcpclient->sendRank();
        }

    });
    connect(Home, &QPushButton::clicked, this, [ = ] {
        this->close();
    });
    connect(QuitBtn, &QPushButton::clicked, this, [ = ] {
        VicNew = 0;
        ps.PlayTimes = 1;
        ps.VicTimes = 0;
        ps.MinTime = "00:00:00";
        CheckBtn->setVisible (true);
        QuitBtn->setVisible(false);
        setPassWord->setVisible(true);
    });
    connect (setPassWord, &QPushButton::clicked, this, [ = ] {
        QMessageBox msgBox;
        qDebug() << 1;
        if(!VicNew) {
            PassWordEdit->setVisible(true);
            PassWordLabel->setVisible (true);
            setPassWord->setVisible (true);
            SetTextLabel->setVisible (false);
            AckSet->setVisible (false);
            SetPassWordEdit->setVisible (false);
            msgBox.setWindowTitle (tr("警告"));
            msgBox.setText (tr("未登录状态！"));
            msgBox.setStandardButtons (QMessageBox::Yes );
            QApplication::setQuitOnLastWindowClosed (false);
            msgBox.exec();
            return;
        }
        PassWordEdit->setVisible(false);
        PassWordLabel->setVisible (false);
        setPassWord->setVisible (false);
        SetTextLabel->setVisible (true);
        AckSet->setVisible (true);
        SetPassWordEdit->setVisible (true);
        SetTextLabel->setText (tr("请输入新密码"));
        SetTextLabel->setStyleSheet ("QLabel{color:black;"
                                     "background-color:rgba(240,248,255,50%);"
                                     "border: 1px solid black;"
                                     "border-radius: 8px;}");
        SetPassWordEdit->setFont (pasfont);
        SetTextLabel->setFont (font);
        SetTextLabel->setGeometry (500, 300, 250, 70);
        SetTextLabel->setAlignment (Qt::AlignCenter);
        SetTextLabel->setWindowOpacity (0.5);
        SetPassWordEdit->setGeometry (760, 300, 400, 70);
        QFont fffont = QFont("FZZJ-QDXKJW", 12);
        fffont.setBold (true);
        AckSet->setFont(fffont);
        AckSet->setText (tr("确认修改"));
        AckSet->setGeometry (980, 515, 70, 30);
        connect(AckSet, &QPushButton::clicked, this, [ = ] {
            bool ok = true;
            loadUser ();
            QString UserPas = SetPassWordEdit->text ();
            if(UserPas.size() == 0) {
                TextLabel->setText (tr("请输入密码！"));
                return;
            } else {
                PassWord[ps.Name] = UserPas;
                TextLabel->setText (tr("修改成功!"));
                PassWordEdit->setVisible(true);
                PassWordLabel->setVisible (true);
                setPassWord->setVisible (true);
                SetTextLabel->setVisible (false);
                AckSet->setVisible (false);
                SetPassWordEdit->setVisible (false);
                writeFileIntoJson (PassWord, "user.json");
                loadUser();
                return;
            }
        });
    });
}

//emit 即将关闭窗口的信号
void Comp::closeEvent(QCloseEvent *Event)
{
    emit aboutToClose();
    if(tcpclient) {
        tcpclient->sendRank();
    }
    // tcpclient->sendRank();
    if(tcpserver) {
        tcpserver->sendRank();
    }
    Event->accept();
}

void Comp::openSer()
{
    if(!tcpserver) {
        tcpserver = new TcpServer(this);
    }
    tcpserver->show ();
}

void Comp::openCli()
{
    if(!tcpclient) {
        tcpclient = new TcpClient(this);
    }
    tcpclient->show ();
}

QJsonArray Comp::loadFileFromJson(QString FilePath)
{
    QFile RankFile(FilePath);
    if (RankFile.open(QIODevice::ReadOnly)) {
        QByteArray RankFileData = RankFile.readAll();
        QJsonDocument RankFileDoc = QJsonDocument::fromJson(RankFileData);
        QJsonArray RankFileArray = RankFileDoc.array();
        RankFile.close();
        return RankFileArray;
    } else {
        qDebug() << "加载User列表失败";
        return QJsonArray();
    }
}

//将新的数据写入json文件储存
void Comp::writeFileIntoJson(std::map<QString, QString>passWord, QString FilePath)
{
    QJsonArray SortedArray;
    for (const auto &[Name, Pas] : passWord) {
        QJsonObject playerObj;
        playerObj.insert("name", Name);
        playerObj.insert("password", Pas);
        SortedArray.append(playerObj);
    }
    QJsonDocument sortedDoc(SortedArray);
    QFile sortedFile(FilePath);
    if (sortedFile.open(QIODevice::WriteOnly )) {
        sortedFile.write(sortedDoc.toJson());
        sortedFile.close();
    } else {
        qDebug() << "储存排行榜失败";
        return;
    }
}

void Comp::loadUser()
{
    PassWord.clear();
    QJsonArray SucArray = loadFileFromJson("user.json");
    for (const auto &item : SucArray) {
        QJsonObject obj = item.toObject();
        std::pair<QString, QString>User;
        User.first = obj["name"].toString();
        User.second = obj["password"].toString();
        PassWord[User.first] = User.second;
    }
}
