#include <QThread>
#include <QBuffer>//>
#include <QHostInfo>
#include <QFileDialog>
#include <QMessageBox>
#include "tcpclient.h"
#include <QHostAddress>
#include "ui_tcpclient.h"

enum Code {
    UpMap,
    UpMapData,
    TimeStart,
    TimeRun,
    TimeEnd
};

TcpClient::TcpClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TcpClient)
{
    QPalette pal = this->palette();
    QPixmap pixmap(":/Green/images/bochi.png");
    pixmap = pixmap.scaled(750, 470, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    pal.setBrush(QPalette::Window, QBrush(pixmap));
    this->setPalette(pal);
    ui->setupUi(this);
    ui->comboServer->setEditable (true);
    //创建socket变量
    tcpClient = new QTcpSocket(this);
    //状态栏标签
    StateLabel = new QLabel("Socket状态：");
    StateLabel->setMinimumWidth(250);
    ui->statusBar->addWidget(StateLabel);
    //获取本地的IP
    QString localIP = getLocalIP();
    this->setWindowTitle(this->windowTitle() + "[本机IP：" + localIP + "]");
    ui->comboServer->addItem(localIP);
    QFont font = QFont("FZZJ-QDXKJW", 14, QFont::Bold);
    ui->plainTextEdit->setFont (font);
    ui->plainTextEdit->setStyleSheet ("background-color:rga(0,0,0,150);");
    //连接槽函数和socket自带的信号
    connect(tcpClient, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(tcpClient, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(tcpClient, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onChange(QAbstractSocket::SocketState)));
    connect(tcpClient, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    FileBytes = SendBytes = RestBytes = 0;
}

//发送排行榜文件
void TcpClient::sendRank()
{
    bool isConnected = tcpClient->isOpen() && tcpClient->state() == QTcpSocket::ConnectedState;
    if(!isConnected) {
        return;
    }
    QFileInfo FileInfo("./rank/success.json");
    QString FileName_str = FileInfo.fileName();
    QByteArray byte_array_FileName = FileName_str.toUtf8();
    int FileNameLength = byte_array_FileName.size();
    FileBytes = FileInfo.size();
    File = new QFile("./rank/success.json");
    File->open(QIODevice::ReadOnly);
    //向socket写入文件
    QByteArray Data;
    Data.append((FileNameLength & 0xff00) >> 0b1000);
    Data.append(FileNameLength & 0xff);
    Data.append(byte_array_FileName);
    Data.append((FileBytes & 0xff000000) >> 0b11000);
    Data.append((FileBytes & 0x00ff0000) >> 0b10000);
    Data.append((FileBytes & 0x0000ff00) >> 0b1000);
    Data.append(FileBytes & 0xff);
    int DataLength = Data.size();
    QByteArray FileSend;
    FileSend.append(0x1C);
    FileSend.append(0x41);
    FileSend.append((DataLength & 0xff000000) >> 0b11000);
    FileSend.append((DataLength & 0x00ff0000) >> 0b10000);
    FileSend.append((DataLength & 0x0000ff00) >> 0b1000);
    FileSend.append(DataLength & 0xff);
    FileSend.append(Data);
    tcpClient->write(FileSend);
    ui->plainTextEdit->appendPlainText("[发送：] " + byte_array_FileName);
    //    qDebug()<<"FileSend:"<<FileSend;
    //    qDebug()<<"文件已选择！";
}

void TcpClient::sendMap(QVector<QVector<int> > &vctCurMap, int code)
{
    bool isConnected = tcpClient->isOpen() && tcpClient->state() == QTcpSocket::ConnectedState;
    if(!isConnected) {
        return;
    }
    QString strCurMap;
    for(int i = 0; i < 6; i++) {
        for(int j = 0; j < 6; j++) {
            QString strTmp = "%1";
            strCurMap.append(strTmp.arg(vctCurMap[i][j]));
        }
    }
    QByteArray byte_array_curmap = strCurMap.toUtf8();
    int len = byte_array_curmap.size();
    QByteArray Data;
    Data.append(code);
    Data.append(0x46);
    Data.append((len & 0xff000000) >> 0b11000);
    Data.append((len & 0x00ff0000) >> 0b10000);
    Data.append((len & 0x0000ff00) >> 0b1000);
    Data.append(len & 0xff);
    Data.append(byte_array_curmap);
    tcpClient->write(Data);
    ui->plainTextEdit->appendPlainText("[发送：] " + byte_array_curmap);
}

void TcpClient::sendTime(int code)
{
    bool isConnected = tcpClient->isOpen() && tcpClient->state() == QTcpSocket::ConnectedState;
    if(!isConnected) {
        return;
    }
    // 创建一个临时数据数组，初始化数据内容为0x01
    QByteArray byte_array_start;
    byte_array_start.append(0x01);
    // 计算当前数据的长度
    int len = byte_array_start.size();
    // 创建一个新的QByteArray对象FileSend，用于存放构造好的完整待发送数据包
    QByteArray Data;
    // 添加固定头部信息
    Data.append(code);
    Data.append(0x47);
    // 将数据长度按照大端序依次拆分为4个字节并添加到FileSend中
    Data.append((len & 0xff000000) >> 0b11000); // 高字节
    Data.append((len & 0x00ff0000) >> 0b10000); // 次高字节
    Data.append((len & 0x0000ff00) >> 0b1000);  // 次低字节
    Data.append(len & 0xff);             // 最低字节
    // 将原始数据Data添加到FileSend末尾
    Data.append(byte_array_start);
    // 使用TCP客户端连接对象tcpSocket，将构造好的完整数据包FileSend发送出去
    tcpClient->write(Data);
    ui->plainTextEdit->appendPlainText("[发送：] " + byte_array_start);
}

QString TcpClient::getLocalIP()
{
    //本地主机名
    QString HostName = QHostInfo::localHostName();
    //获取本地详细信息
    QHostInfo HostInfo = QHostInfo::fromName(HostName);
    QString localIP = "";
    //找到所有IP地址
    QList<QHostAddress> AllList = HostInfo.addresses();
    //找到第一个IPV4地址
    if (!AllList.isEmpty())
        for (const auto &NowHost : AllList) {
            if (QAbstractSocket::IPv4Protocol == NowHost.protocol()) {
                localIP = NowHost.toString();
                break;
            }
        }
    return localIP;
}

TcpClient::~TcpClient()
{
    delete ui;
}

void TcpClient::closeEvent(QCloseEvent *event)
{
    if (tcpClient->state() == QAbstractSocket::ConnectedState) {
        tcpClient->disconnectFromHost();
    }
    event->accept();
}

void TcpClient::onConnected()
{
    //connected()信号槽函数
    ui->plainTextEdit->appendPlainText("[已连接到服务器]");
    ui->plainTextEdit->appendPlainText("[服务器地址:" +
                                       tcpClient->peerAddress().toString() + "]");
    ui->plainTextEdit->appendPlainText("[服务器端口号:" +
                                       QString::number(tcpClient->peerPort()) + "]");
    ui->actConnect->setEnabled(false);
    ui->actDisconnect->setEnabled(true);
}

void TcpClient::onDisconnected()
{
    //disConnected()信号槽函数
    ui->plainTextEdit->appendPlainText("[已断开与服务器的连接]");
    ui->actConnect->setEnabled(true);
    ui->actDisconnect->setEnabled(false);
}


void TcpClient::onReadyRead()
{
    // 从TCP客户端socket中读取所有当前可读的数据
    QByteArray temp = tcpClient->readAll();
    // 将接收到的数据添加到累计接收数据缓冲区
    RcvData.append(temp);
    // 当累计接收数据缓冲区中有数据时，持续处理
    while(RcvData.size() > 0) {
        // 如果当前报文长度未知（初始化或上一条报文已处理完）
        if(PackLen == 0) {
            // 从累计接收数据缓冲区的第2个字节开始提取4个字节作为报文长度信息
            // 并将这4个字节的十六进制表示转为十进制整数，即获取实际报文长度
            PackLen = RcvData.mid(2, 4).toHex().toInt(NULL, 16);
            // qDebug() << "PackLen : " << PackLen;
            // qDebug() << "RcvData.size : " << RcvData.size();
        }
        // 判断累计接收数据缓冲区中的数据是否足以包含完整的一条报文（含包头和包体）
        if(RcvData.size() >= (6 + PackLen)) {
            // 提取出一条完整的报文（包含6字节包头和实际报文体）
            QByteArray RcvPerData = RcvData.mid(0, 6 + PackLen);
            // 更新累计接收数据缓冲区，移除已处理的完整报文部分
            RcvData = RcvData.mid(6 + PackLen);
            // 对这条完整报文进行解析或处理操作
            DealFile(RcvPerData);
            // 重置当前报文长度标识
            PackLen = 0;
        }
        // 如果累计接收数据缓冲区中的数据还不足以构成一条完整的报文，则退出循环等待下一次数据到来
        if(RcvData.size() < (6 + PackLen)) {
            break;
        }
    }
}


void TcpClient::DealFile(QByteArray FileRcvPer)
{
    // qDebug()<<"FileRcvPer"<<FileRcvPer;
    // 从接收到的数据中提取文件标志位
    int FileMark = FileRcvPer.mid(1, 1).toHex().toInt(NULL, 16);
    // 根据不同的文件标志位进行相应处理
    if(FileMark == 17) {
        // 若文件标志位为0x10，表示接收到的是普通文本信息
        QByteArray Messagereceive = FileRcvPer.mid(6, FileRcvPer.size() - 6);
        // 将接收到的消息追加到UI控件（例如文本编辑框）中显示
        ui->plainTextEdit->appendPlainText("[玩家1:] " + Messagereceive);
    } else if(FileMark == 65) {
        // 若文件标志位为0x41，开始接收文件前的准备工作
        // 从接收到的数据中提取文件名长度（以两个字节表示）
        RcvNameLen = FileRcvPer.mid(6, 2).toHex().toInt(NULL, 16);
        // 提取并存储接收到的文件名
        QByteArray ReceiveFileName = FileRcvPer.mid(8, RcvNameLen);
        // 在UI控件中显示接收到的文件名
        ui->plainTextEdit->appendPlainText("[接收：] " + ReceiveFileName);
        RcvFileSize = FileRcvPer.mid(8 + RcvNameLen, 4).toHex().toInt(NULL, 16);
        FileRcv = new QFile(ReceiveFileName);
        // 打开文件，设置为覆盖写入模式
        FileRcv->open(QFile::WriteOnly);
        // 调用ConFile()函数，用于发送一个确认接收文件准备好的信号
        ConFile();
    } else if(FileMark == 66) {
        // 若文件标志位为0x42，表示已经收到对方对于接收文件准备好的确认
        // 调用AckFile()函数，用于请求发送方开始发送文件内容
        AckFile();
    } else if(FileMark == 67) {
        // 若文件标志位为0x44，开始接收文件内容片段
        // 从数据中提取此次接收到的数据段长度（以四个字节表示）
        int DataLength_per = FileRcvPer.mid(2, 4).toHex().toInt(NULL, 16);
        // 提取并存储接收到的文件内容片段
        QByteArray PerFilieCon = FileRcvPer.mid(6, DataLength_per);
        // 检查已接收数据总量是否小于预期文件大小
        if(CurDataLen < RcvFileSize) {
            // 将接收到的文件内容片段写入目标文件
            FileRcv->write(PerFilieCon);
            // 更新已接收数据总量
            CurDataLen += DataLength_per;
            // 当已接收数据总量等于文件大小时，关闭文件，并准备发送接收完成的确认信号
            if(CurDataLen == RcvFileSize) {
                FileRcv->close();
                CurDataLen = 0;
                // 调用CompFile()函数，可能用于发送一个接收文件完成的确认信号
                CompFile();
                // 在用户界面弹出一个消息框，通知用户文件接收成功
                QApplication::setQuitOnLastWindowClosed (false);
                //QMessageBox::about(NULL, "文件", "文件接收成功");
            }
        }
    } else if(FileMark == 69) {
        // 若文件标志位为0x45，表示接收到对方发送文件结束的确认信号
        // 在用户界面弹出一个消息框，通知用户文件发送成功
        QApplication::setQuitOnLastWindowClosed (false);
        QMessageBox::about(NULL, "文件", "数据同步成功");
    } else if(FileMark == 70) {
        // 若文件标志位为0x46，表示接收到地图更新数据
        QByteArray Messagereceive = FileRcvPer.mid(6, FileRcvPer.size() - 6);
        // 将接收到的消息追加到UI控件（例如文本编辑框）中显示
        ui->plainTextEdit->appendPlainText("[玩家2:] " + Messagereceive);
        // 从接收到的数据中提取指令码
        int code = FileRcvPer.mid(0, 1).toHex().toInt(NULL, 16);
        if(code == Code::UpMap) {
            emit readyMap(Messagereceive);
        } else if(code == Code::UpMapData) {
            emit readyMapData(Messagereceive);
        }
    } else if(FileMark == 71) {
        // 若文件标志位为0x47，表示接收到游戏开始信号，更新计时
        QByteArray Messagereceive = FileRcvPer.mid(6, FileRcvPer.size() - 6);
        // 将接收到的消息追加到UI控件（例如文本编辑框）中显示
        ui->plainTextEdit->appendPlainText("[玩家2:] " + Messagereceive);
        // 从接收到的数据中提取指令码
        int code = FileRcvPer.mid(0, 1).toHex().toInt(NULL, 16);
        emit readyTime(code);
    }
}


void TcpClient::ConFile()
{
    // 创建一个临时数据数组，初始化数据内容为0x01
    QByteArray Data;
    Data.append(0x01);
    // 计算当前数据的长度
    int DataLength = Data.size();
    // 创建一个新的QByteArray对象FileSend，用于存放构造好的完整待发送数据包
    QByteArray FileSend;
    // 添加固定头部信息
    FileSend.append(0x1C);
    FileSend.append(0x42);
    // 将数据长度按照大端序依次拆分为4个字节并添加到FileSend中
    FileSend.append((DataLength & 0xff000000) >> 0b11000); // 高字节
    FileSend.append((DataLength & 0x00ff0000) >> 0b10000); // 次高字节
    FileSend.append((DataLength & 0x0000ff00) >> 0b1000);  // 次低字节
    FileSend.append(DataLength & 0xff);             // 最低字节
    // 将原始数据Data添加到FileSend末尾
    FileSend.append(Data);
    // 使用TCP客户端连接对象tcpClient，将构造好的完整数据包FileSend发送出去
    tcpClient->write(FileSend);
}


void TcpClient::AckFile()
{
    // 将剩余未发送的文件字节数量赋值给RestBytes变量
    RestBytes = FileBytes;
    // 进入无限循环，直至文件内容全部发送完毕
    while(1) {
        // 当还有剩余字节未发送时
        if(RestBytes > 0) {
            // 从文件中模拟读取一定数量的数据到缓冲区（此处设定每次读取1200字节）
            QByteArray ReadBuf = File->read(1200);
            // 构造发送的数据包Filefun，包含协议头和数据内容
            QByteArray Filefun;
            // 协议标识符
            Filefun.append(0x1C);
            Filefun.append(0x43);
            // 将要发送的数据块大小转换为大端序并添加至数据包
            Filefun.append((ReadBuf.size() & 0xff000000) >> 0b11000);
            Filefun.append((ReadBuf.size() & 0x00ff0000) >> 0b10000);
            Filefun.append((ReadBuf.size() & 0x0000ff00) >> 0b1000);
            Filefun.append(ReadBuf.size() & 0xff);
            // 将数据块内容添加至数据包
            Filefun.append(ReadBuf);
            // 使用TCP客户端连接对象将构造好的数据包发送出去
            tcpClient->write(Filefun);
            // 更新剩余未发送的字节数量
            RestBytes -= ReadBuf.size();
            // 更新已发送的字节数量
            SendBytes += ReadBuf.size();
        }
        // 当已发送的字节数量等于文件总字节数时，说明文件内容已全部发送完毕
        if(SendBytes == FileBytes) {
            // 关闭文件资源
            File->close();
            // 清空文件名字符串
            FileStr.clear();
            // 将已发送字节数量重置为0
            SendBytes = 0;
            // 调用SendFile()函数，可能用于发送文件发送完成的确认消息
            SendFile();
            // 结束循环
            break;
        }
    }
}


void TcpClient::SendFile()
{
    // 创建一个临时数据数组，这里填充0x01
    QByteArray Data;
    Data.append(0x01);
    // 计算当前数据的长度
    int DataLength = Data.size();
    // 创建一个新的QByteArray对象FileSend，用于存放完整待发送数据包
    QByteArray FileSend;
    // 添加固定的协议头，此处为0x1C 0x44，表示“文件传输完成”指令
    FileSend.append(0x1C);
    FileSend.append(0x44);
    // 将数据长度按照大端序依次拆分为4个字节并添加到FileSend中
    FileSend.append((DataLength & 0xff000000) >> 0b11000); // 高字节
    FileSend.append((DataLength & 0x00ff0000) >> 0b10000); // 次高字节
    FileSend.append((DataLength & 0x0000ff00) >> 0b1000);  // 次低字节
    FileSend.append(DataLength & 0xff);             // 最低字节
    // 将原始数据Data添加到FileSend末尾
    FileSend.append(Data);
    // 使用TCP客户端连接对象tcpClient，将构造好的完整数据包FileSend发送出去
    tcpClient->write(FileSend);
    // qDebug()<<"SendFile已发送。";
}


void TcpClient::CompFile()
{
    // 同样创建一个临时数据数组，这里填充0x01
    QByteArray Data;
    Data.append(0x01);
    // 计算当前数据的长度
    int DataLength = Data.size();
    // 创建一个新的QByteArray对象FileSend，用于存放完整待发送数据包
    QByteArray FileSend;
    // 添加固定的协议头，此处为0x1C 0x45，表示“文件接收完成”指令
    FileSend.append(0x1C);
    FileSend.append(0x45);
    // 同样按照大端序将数据长度拆分并添加到FileSend中
    FileSend.append((DataLength & 0xff000000) >> 0b11000);
    FileSend.append((DataLength & 0x00ff0000) >> 0b10000);
    FileSend.append((DataLength & 0x0000ff00) >> 0b1000);
    FileSend.append(DataLength & 0xff);
    // 添加原始数据Data到FileSend末尾
    FileSend.append(Data);
    // 使用TCP客户端连接对象tcpClient，将构造好的完整数据包FileSend发送出去
    tcpClient->write(FileSend);
    // qDebug()<<"CompFile已发送。";
}

void TcpClient::onChange(QAbstractSocket::SocketState socketState)
{
    //stateChange()信号槽函数
    switch(socketState) {
        case QAbstractSocket::UnconnectedState:
            StateLabel->setText("scoket状态：未连接");
            break;
        case QAbstractSocket::HostLookupState:
            StateLabel->setText("scoket状态：正在查找主机");
            break;
        case QAbstractSocket::ConnectingState:
            StateLabel->setText("scoket状态：正在连接中");
            break;
        case QAbstractSocket::ConnectedState:
            StateLabel->setText("scoket状态：已连接");
            break;
        case QAbstractSocket::BoundState:
            StateLabel->setText("scoket状态：已绑定到地址和端口");
            break;
        case QAbstractSocket::ClosingState:
            StateLabel->setText("scoket状态：即将关闭");
            break;
        case QAbstractSocket::ListeningState:
            StateLabel->setText("scoket状态：正在监听中");
    }
}


void TcpClient::on_actConnect_triggered()
{
    ui->comboServer->setEnabled(false);
    ui->spinPort->setEnabled(false);
    QString addr = ui->comboServer->currentText();
    quint16 port = ui->spinPort->value();
    tcpClient->connectToHost(addr, port);
}



void TcpClient::on_actDisconnect_triggered()
{
    if (tcpClient->state() == QAbstractSocket::ConnectedState) {
        tcpClient->disconnectFromHost();
    }
    ui->comboServer->setEnabled(true);
    ui->spinPort->setEnabled(true);
}



void TcpClient::on_actClear_triggered()
{
    ui->plainTextEdit->clear();
}

//发送信息
void TcpClient::on_btnSend_clicked()
{
    QString  msg = ui->editMsg->text();
    ui->plainTextEdit->appendPlainText("[本机:] " + msg);
    ui->editMsg->clear();
    ui->editMsg->setFocus();
    QByteArray  str = msg.toUtf8();
    //    str.append('\n');
    QByteArray message;
    message.append(0x1C);
    message.append(0x10);
    message.append((str.size() & 0xff000000) >> 0b11000);
    message.append((str.size() & 0x00ff0000) >> 0b10000);
    message.append((str.size() & 0x0000ff00) >> 0b1000);
    message.append(str.size() & 0xff);
    message.append(str);
    tcpClient->write(message);
}

void TcpClient::on_actbackend_triggered()
{
    this->setVisible (false);
}

