// 引入所需头文件
#include "dragbutton.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QBoxLayout>
#include <QLabel>
#include <QSoundEffect>

// 构造函数初始化
DragButton::DragButton(QWidget *parent) : QPushButton(parent)
{
    // 设置鼠标指针样式
    QCursor *myCursor = new QCursor(QPixmap(":/Green/images/Arrow.png"));
    this->setCursor(*myCursor);
    // 初始化状态变量
    isPressed = false;
    isMoved = false;
    lastPoint = QPoint(0, 0);
    // 初始化边界距离
    x_LeftDis = 0;
    x_RightDis = 0;
    y_TopDis = 0;
    y_BotDis = 0;
    // 初始点击状态
    BtnClick = false;
    // 设置按钮大小
    this->resize(120, 50);
    // 设置布局
    QHBoxLayout *layout = new QHBoxLayout(this);
    // 设置样式
    this->setStyleSheet ("QPushButton {"
                         "border: 1px solid #E0EEEE;"
                         "background-color: rgba(245,245,245,100);"
                         "border-radius: 4px;"
                         "}"
                         "QPushButton:hover {"
                         "background-color: #87CEFF;"
                         "}");
    // 设置窗口透明度
    this->setWindowOpacity(0.5);
    // 创建图标标签
    PixLabel = new QPushButton(this);
    // 创建文本标签
    QLabel *TextLabel = new QLabel(this);
    // 设置图标标签尺寸
    PixLabel->setFixedSize(30, 30);
    // 设置文本标签尺寸
    TextLabel->setFixedSize(60, 30);
    // 设置文本标签内容
    TextLabel->setText(tr("暂停"));
    // 设置字体
    QFont font = QFont("FZZJ-QDXKJW", 18);
    font.setBold(true);
    TextLabel->setFont(font);
    // 加载并缩放图标
    QPixmap ImgAllbackground(":/Green/images/start.png");
    fitimgpic = ImgAllbackground.scaled(30, 30, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    // 设置图标
    PixLabel->setIcon(QIcon(":/Green/images/start.png"));
    PixLabel->setIconSize(QSize(30, 30));
    // 添加到布局
    layout->addWidget(PixLabel);
    layout->addWidget(TextLabel);
    // 安装事件过滤器
    this->installEventFilter(this);
    // 连接信号与槽
    connect(PixLabel, &QPushButton::clicked, this, [ = ] {
        if(BtnClick) {
            emit goon();
        } else {
            emit stop();
        }
        BtnClick = !BtnClick;
    });
    // 播放点击音效
    connect(PixLabel, &QPushButton::clicked, this, [ = ] {
        QSoundEffect *startSound = new QSoundEffect(this);
        startSound->setSource(QUrl("qrc:/Green/images/btnclick1.wav"));
        startSound->setVolume(0.1f);
        startSound->play();
    });
}

// 事件过滤器，处理鼠标事件
bool DragButton::eventFilter(QObject *watched, QEvent *event)
{
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    switch(event->type()) {
        // 鼠标按下事件
        case QEvent::MouseButtonPress: {
                if (mouseEvent->button() == Qt::LeftButton) {
                    lastPoint = mouseEvent->pos();
                    isPressed = true;
                    QCursor *myCursor = new QCursor(QPixmap(":/Green/images/Hand2.png"));
                    this->setCursor(*myCursor);
                }
                break;
            }
        // 鼠标移动事件
        case QEvent::MouseMove: {
                if (isPressed) {
                    QCursor *myCursor = new QCursor(QPixmap(":/Green/images/Hand2.png"));
                    this->setCursor(*myCursor);
                    // 计算偏移量
                    int disx = mouseEvent->pos().x() - lastPoint.x();
                    int disy = mouseEvent->pos().y() - lastPoint.y();
                    // 移动位置
                    int nowx = this->x() + disx;
                    int nowy = this->y() + disy;
                    // 检查边界
                    int RDis = this->parentWidget()->width() - 2 * x_RightDis - this->width();
                    int BDis = this->parentWidget()->height() - 2 * y_BotDis - this->height();
                    if(nowx > x_LeftDis && nowx < RDis && nowy > y_TopDis && nowy < BDis) {
                        this->move(this->x() + disx, this->y() + disy);
                    }
                    isMoved = true;
                }
                break;
            }
        // 鼠标释放事件
        case QEvent::MouseButtonRelease: {
                if(isMoved != true) {
                    emit clicked();
                    emit toggled(!isChecked);
                    isChecked = !isChecked;
                } else {
                    QCursor *myCursor = new QCursor(QPixmap(":/Green/images/Arrow.png"));
                    this->setCursor(*myCursor);
                    isMoved = false;
                }
                isPressed = false;
                break;
            }
        // 其他事件
        default:
            break;
    }
    // 调用基类事件过滤器
    return QWidget::eventFilter(watched, event);
}
