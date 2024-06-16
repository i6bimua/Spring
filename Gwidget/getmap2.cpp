#include "getmap2.h"
#include "ui_getmap2.h"

#include <QCloseEvent>
#include <QPainter>
#include <QHBoxLayout>
#include <QMimeData>
#include <QDrag>
#include <QMessageBox>
#include <QDir>
#include <QFile>

bool GetMap2::isPrettifyData()
{
    bool e = false;
    int n = 0, o = 0, i = 0;
    if(vctCurSelectedMap.length() == 6 && vctCurSelectedMap[0].length() == 6) {
        for(int r = 0; r < 6; r++) {
            int a, s, c;
            for(a = 0, s = 0; s < 6; s++) {
                c = vctCurSelectedMap[r][s];
                a += c;
            }
            if(0 == a) {
                return false;
            }
        }
        for(int s = 0; s < 6; s++) {
            int a, r, c;
            for(a = 0, r = 0; r < 6; r++) {
                c = vctCurSelectedMap[r][s];
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
                0 == (c = vctCurSelectedMap[r][s]) ? i++ : 4 == c ? o++ : c >= 5 && n++;
            }
        }
    }
    return 1 == o && i < 10 && 0 == n && (e = !0), e;
}

int GetMap2::getMapNum()
{
    int MapNum = 0;
    QDir MapDir("./map");
    QFileInfoList entries = MapDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    for(const QFileInfo &entry : entries) {
        if(entry.isFile ()) {
            MapNum += 1;
        }
    }
    return MapNum;
}

void GetMap2::dragEnterEvent(QDragEnterEvent *event)
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

void GetMap2::dragMoveEvent(QDragMoveEvent *event)
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

void GetMap2::dropEvent(QDropEvent *event)
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
            QPoint curPointinWidget = curPoint - ui->widget->pos();
            int x = curPointinWidget.rx() % 84;
            int y = curPointinWidget.ry() % 84;
            if((x > -42 && x < 0) || (x > 42)) {
                curPointinWidget.rx() += 42;
            }
            if((y > -42 && y < 0) || (y > 42)) {
                curPointinWidget.ry() += 42;
            }
            curPointinWidget += ui->widget->pos();
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
                // stackMapRecord.push(vctCurSelectedMap);
                int nChunkX = n;
                int nChunkY = m;
                for(int i = 0; i < num; i++) {
                    for(int j = 0; j < num; j++) {
                        vctCurSelectedMap[nChunkY][nChunkX]++;
                        nChunkX++;
                    }
                    nChunkX = n;
                    nChunkY++;
                }
                QObjectList gs = ui->widget->children();
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
                // okBtn.push(index);
                // NextLevel();
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

void GetMap2::mousePressEvent(QMouseEvent *event)
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

GetMap2::GetMap2(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GetMap2)
{
    QIcon GetMapIcon = QIcon(":/Green/images/getmap1.png");
    if (GetMapIcon.isNull ()) {
        qDebug() << "加载Icon图片失败";
        return;
    }
    this->setWindowIcon (GetMapIcon);
    this->setWindowTitle (tr("创建地图"));
    QFont font = QFont("FZZJ-QDXKJW", 20);
    QFont lfont = QFont("FZZJ-QDXKJW", 30);
    font.setBold (true);
    lfont.setBold (true);
    ui->setupUi(this);
    ui->pushButton->setFont(font);
    ui->label->setStyleSheet ("QLabel{"
                              "background-color: transparent;"
                              "font-family:FZZJ-QDXKJW;"
                              "font-size:48px;"
                              "font-weight:1000;"
                              "color:#98FB98;"
                              "}");
    ui->pushButton->setStyleSheet ("QPushButton {"
                                   "border: 1px solid black;"
                                   "background-color:#F5F5F5;"
                                   "border-radius: 4px;"
                                   "}"
                                   "QPushButton:hover {"
                                   "background-color: #87CEFF;"
                                   "}");
    QPixmap ImgAllbackground(":/Green/images/bj.jpg");
    fitimgpic = ImgAllbackground.scaled(this->width(), this->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    // 设置固定窗口大小，使其无法放大缩小
    this->setFixedSize(this->width(), this->height());
    this->setAcceptDrops(true);       //设置能否放置
    //开启鼠标跟踪
    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    for(int i = 0; i < 6; i++) {
        QVector<int> r;
        for(int a = 0; a < 6; a++) {
            r.push_back(0);
        }
        vctCurSelectedMap.push_back(r);
    }
    // 设置作答区域
    ui->widget->setStyleSheet("background-color:#757898;");
    QObjectList gs = ui->widget->children();
    for(int i = 0; i < gs.size(); i++) {
        // 提取坐标
        // QString str = gs[i]->objectName();
        // QStringList list = str.split("_");
        // QString strM = list[1];
        // QString strN = list[2];
        // int m = strM.toInt();
        // int n = strN.toInt();
        // int num = vctCurMap[m][n];
        QString styleSheet = "border-width:1px;border-style:solid;border-color:rgb(255,170,0);"
                             "background-image:url(:/image/images/game_%1.png);";
        ((QLabel *)gs[i])->setFrameShape(QFrame::Box);
        // if(pParent->nMode == Mode::GameMode && num == 0)
        // {
        //     num = 2;
        // }
        ((QLabel *)gs[i])->setStyleSheet(styleSheet.arg(0));
    }
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
    WidgetRect = QRect(ui->widget->pos(), ui->widget->size());
}

GetMap2::~GetMap2()
{
    delete ui;
}

void GetMap2::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    // 确保当窗口大小改变时图片能适应整个窗口
    painter.drawPixmap(rect(), fitimgpic);
}

void GetMap2::closeEvent(QCloseEvent *event)
{
    emit ReturnMain();
    event->accept();
}

void GetMap2::on_pushButton_clicked()
{
    if(!isPrettifyData()) {
        QMessageBox msgBox;
        msgBox.setWindowTitle (tr("提示"));
        msgBox.setText (tr("输入的矩阵没有合法答案"));
        msgBox.setStandardButtons (QMessageBox::Yes | QMessageBox::No);
        QApplication::setQuitOnLastWindowClosed (false);
        int ret = msgBox.exec();
        if(ret == QMessageBox::No) {
            this->close ();
        }
    } else {
        QMessageBox msgBox;
        int num = getMapNum();
        num++;
        QString MapStr("./map/map%1.txt");
        QFile f(MapStr.arg(num));
        if(!f.open(QIODevice::WriteOnly)) {
            qDebug() << "无法打开文件";
            return;
        }
        QTextStream Text(&f);
        Text << num << '\n';
        for(int i = 0; i < 6; ++i) {
            QString Str = "";
            for(int j = 0; j < 6; ++j) {
                QString str = QString::number(vctCurSelectedMap[i][j]);
                Str.append(str);
                Str.append (", ");
            }
            Str.append("\n");
            Text << Str;
        }
        msgBox.setWindowTitle (tr("提示"));
        msgBox.setText (tr("已经存入地图"));
        msgBox.setStandardButtons (QMessageBox::Yes);
        QApplication::setQuitOnLastWindowClosed (false);
        msgBox.exec();
        this->close();
    }
}

