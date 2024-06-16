#include <QIcon>
#include <QRect>
#include <QDebug>
#include <QLabel>
#include <QPoint>
#include <QPixmap>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include "explanation.h"
Explanation::Explanation( QWidget *parent)
    : QWidget{parent}
{
    QCursor *myCursor = new QCursor(QPixmap(":/Green/images/Arrow.png"));
    this->setCursor (*myCursor);
    //设置背景
    QPixmap WinBackground = QPixmap(":/Green/images/explanback.png");
    QPalette palette = this->palette();
    palette.setBrush(this->backgroundRole(), QBrush(WinBackground));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    //设置规则内容
    QVBoxLayout *layout = new QVBoxLayout(this);
    QSpacerItem *TopSpacer = new QSpacerItem(0, 45, QSizePolicy::Minimum, QSizePolicy::Fixed);
    layout->addItem(TopSpacer);
    Title = new QLabel(this);
    QFont TitleFont("经典繁毛楷", 40);
    Title->setFont(TitleFont);
    Title->setContentsMargins (20, 20, 20, 20);
    Title->setText(tr("规则说明"));
    Title->setAlignment (Qt::AlignHCenter);
    Title->setStyleSheet("color: #E0EEEE;");
    layout->addWidget(Title);
    Rule = new QLabel(this);
    QFont RuleFont("FZZJ-QDXKJW", 24);
    RuleFont.setBold (true);
    QString RuleText = "<p><img src=':/image/images/game_0.png'width='42' height='42'/>"
                       "<span>代表该方格<span style='color:#00CD66;'>没有</span>树(仅在练习模式出现)</span></p>"
                       "<p><img src=':/image/images/game_1.png'width='42' height='42'/>"
                       "<span>代表该方格有<span style='color:#00CD66;'>1或3</span>棵树</span></p>"
                       "<p><img src=':/image/images/game_2.png'width='42' height='42'/>"
                       "<span>代表该方格有<span style='color:#00CD66;'>2或0</span>棵树(比赛模式)</span></p>"
                       "<p><img src=':/image/images/game_4.png'width='42' height='42'/>"
                       "<span>代表该方格有<span style='color:#00CD66;'>4<</span>棵树(比赛模式)</span></p>";
    Rule->setTextFormat(Qt::RichText);
    Rule->setText(RuleText);
    Rule->setWordWrap(true);
    Rule->setFont (RuleFont);
    Rule->setStyleSheet("color: #E0EEEE;");
    Rule->setContentsMargins (20, 20, 20, 20);
    layout->addWidget(Rule);
    QSpacerItem *MiddleSpacer = new QSpacerItem(0, 125, QSizePolicy::Minimum, QSizePolicy::Fixed);
    layout->addItem(MiddleSpacer);
    Step = new QLabel(this);
    QFont StepFont("FZZJ-QDXKJW", 24);
    StepFont.setBold (true);
    QString StepText = "<p>1.你需要把下面<span style='color:#00CD66;'>7</span>个不同大小的树木切片放置在中间盘面</p>"
                       "<p>2.移动之后，盘面中格子右上角的数字代表<span style='color:#00CD66;'>该方格已经种树的数量</span></p>"
                       "<p>3.你需要使得每个格子上<span style='color:#00CD66;'>图案所代表的数量</span>和所种树木的数量匹配，即可通关</p>";
    Step->setText(StepText);
    Step->setWordWrap(true);
    Step->setFont (StepFont);
    Step->setContentsMargins (20, 20, 20, 20);
    Step->setStyleSheet("color: #E0EEEE;");
    layout->addWidget(Step);
    QSpacerItem *bottomSpacer = new QSpacerItem(0, 75, QSizePolicy::Minimum, QSizePolicy::Fixed);
    layout->addItem(bottomSpacer);
    this->setLayout (layout);
}

//重写关闭事件
void Explanation::closeEvent(QCloseEvent *Event)
{
    emit aboutToClose();
    Event->accept();
}

