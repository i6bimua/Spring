#include "getmap.h"
#include "ui_getmap.h"
#include <QPushButton>
#include <QGridLayout>
#include <QPainter>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDir>
#include <QFile>
GetMap::GetMap(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GetMap)
{
    int num = getMapNum ();
    for(int i = 1; i <= num; ++i) {
        QString MapStr("./map/map%1.txt");
        readFileBySixLines (MapStr, i);
    }
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
    initializeMatrixLineEdits();
    connect(ui->pushButton, &QPushButton::clicked, this, &GetMap::onReadMatrixButtonClicked);
    QPixmap ImgAllbackground(":/Green/images/yui.png");
    fitimgpic = ImgAllbackground.scaled(this->width(), this->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

GetMap::~GetMap()
{
    delete ui;
}

void GetMap::initializeMatrixLineEdits()
{
    lineEditMatrix.resize(7, std::vector<QLineEdit *>(7)); // 初始化6x6的二维向量
    for (int i = 1; i <= 6; ++i) {
        for (int j = 1; j <= 6; ++j) {
            QLineEdit *lineEdit = new QLineEdit(this);
            lineEdit->setFixedSize (50, 50);
            lineEdit->setStyleSheet ("QLineEdit{"
                                     "font-family:FZZJ-QDXKJW;"
                                     "font-size:24px;"
                                     "font-weight:1000;"
                                     "background-color:#F5F5F5;"
                                     "}"
                                     "QLineEdit:hover{"
                                     "background-color:#F5F5F5;"
                                     "}");
            lineEdit->setAlignment (Qt::AlignCenter);
            lineEditMatrix[i][j] = lineEdit; // 将新创建的QLineEdit添加到矩阵中
            ui->gridLayout->addWidget(lineEdit, i, j); // 添加到网格布局的对应位置
        }
    }
}

void GetMap::onReadMatrixButtonClicked()
{
    // 假设lineEditMatrix是一个QLineEdit*的动态数组，已正确初始化并添加到gridLayout中
    for (int i = 1; i <= 6; ++i) {
        for (int j = 1; j <= 6; ++j) {
            QString text = lineEditMatrix[i][j]->text(); // 获取文本
            if (text.length() == 1) { // 确保只输入了一个字符
                matrix[i ][j ] = (text.at(0).digitValue ()); // 将QString的第一个字符转换为QChar
            } else {
                // 处理错误情况，比如用户输入的不是单个字符
                QMessageBox msgBox;
                msgBox.setWindowTitle (tr("提示"));
                msgBox.setText (tr("输入有误，请重试"));
                msgBox.setStandardButtons (QMessageBox::Yes);
                QApplication::setQuitOnLastWindowClosed (false);
                msgBox.exec ();
                return;
            }
        }
    }
    init();
    flag = false;
    for(int i = 1; i <= 3; i++) {
        for(int j = 1; j <= 3; j++) {
            dfs(i, j, 1);
        }
    }
    QMessageBox msgBox;
    if(flag) {
        int num = getMapNum();
        num++;
        QVector<QVector<int>>Mat(6, QVector<int>(6, 0));
        for(int i = 0; i < 6; ++i) {
            for(int j = 0; j < 6; ++j) {
                Mat[i][j] = matrix[i + 1][j + 1];
            }
        }
        if(LMap[Mat]) {
            msgBox.setWindowTitle (tr("警告"));
            msgBox.setText (tr("地图已经存在"));
            msgBox.setStandardButtons (QMessageBox::Yes);
            QApplication::setQuitOnLastWindowClosed (false);
            msgBox.exec();
            this->close();
        } else {
            LMap[Mat] = true;
        }
        QString MapStr("./map/map%1.txt");
        QFile f(MapStr.arg(num));
        if(!f.open(QIODevice::WriteOnly)) {
            qDebug() << "无法打开文件";
            return;
        }
        QTextStream Text(&f);
        int sum = 0;
        std::map<int, int>map;
        for(int i = 1; i <= 6; ++i) {
            for(int j = 1; j <= 6; ++j) {
                map[matrix[i][j]]++;
                if(matrix[i][j] > 0) {
                    sum ++;
                }
            }
        }
        for(int i = 1; i <= 6; ++i) {
            for(int j = 1; j <= 6; ++j) {
                if(j > 1 && matrix[i][j] == matrix[i][j - 1]) {
                    sum--;
                }
                if(i > 1 && matrix[i][j] == matrix[i - 1][j]) {
                    sum--;
                }
            }
        }
        //qDebug() << sum << '\n';
        if(sum <= -7) {
            Text << 1 << '\n';
        } else if(sum <= -4) {
            Text << 2 << '\n';
        } else if(sum <= -1) {
            Text << 3 << '\n';
        } else if(sum <= 2) {
            Text << 4 << '\n';
        } else {
            Text << 5 << '\n';
        }
        for(int i = 1; i <= 6; ++i) {
            QString Str = "";
            for(int j = 1; j <= 6; ++j) {
                QString str = QString::number(matrix[i][j]);
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
    } else {
        msgBox.setWindowTitle (tr("提示"));
        msgBox.setText (tr("输入的矩阵没有合法答案"));
        msgBox.setStandardButtons (QMessageBox::Yes | QMessageBox::No);
        QApplication::setQuitOnLastWindowClosed (false);
        int ret = msgBox.exec();
        if(ret == QMessageBox::No) {
            this->close ();
        }
    }
}
void GetMap::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    // 确保当窗口大小改变时图片能适应整个窗口
    painter.drawPixmap(rect(), fitimgpic);
}
void GetMap::closeEvent(QCloseEvent *event)
{
    emit ReturnMin ();
    event->accept();
}

void GetMap::init()
{
    for(int i = 1; i <= 6; i++) {
        for(int j = 1; j <= 6; j++) {
            temp[i][j] = matrix[i][j];
        }
    }
}

void GetMap::dfs(int x, int y, int cnt)
{
    if(flag) {
        return ;
    }
    bool flag1 = true;
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
}

int GetMap::getMapNum()
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

void GetMap::readFileBySixLines(const QString &filePath, int index)
{
    QFile f(filePath.arg(index));
    if(!f.open(QIODevice::ReadOnly)) {
        qDebug() << "无法打开文件";
        return;
    }
    QTextStream in(&f);
    QStringList lines;
    QString line;
    // 读取文件直到结束
    QString null = in.readLine();
    //qDebug() << null;
    while (!in.atEnd()) {
        lines.clear(); // 重置行列表，为下一轮六行数据做准备
        for (int i = 0; i < 6 && !in.atEnd(); ++i) {
            line = in.readLine(); // 读取一行
            QString temp;
            for(auto &c : line) {
                if(c >= '0' && c <= '9') {
                    temp.append(c);
                }
            }
            //qDebug().noquote() << temp;
            lines.append(temp); // 添加到当前批次的行列表中
        }
    }
    if (!lines.isEmpty()) {
        if(lines.size() == 6) {
            processSixLines(lines);
        }
    }
    qDebug() << "完成文件加载";
    //qDebug() << lines.size();
    f.close();
}
void GetMap::processSixLines(const QStringList &sixLines)
{
    QVector<QVector<int>>Map(6, QVector<int>(6, 0));
    int lineId = 0;
    foreach(const QString &line, sixLines) {
        for(int i = 0; i < 6; ++i) {
            Map[lineId][i] = line[i].digitValue ();
        }
        lineId++;
    }
    LMap[Map] = true;
    qDebug() << LMap.size() << '\n';
}
