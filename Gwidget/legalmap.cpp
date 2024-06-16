#include "legalmap.h"
#include <QFile>
#include <QDebug>
LegalMap::LegalMap(QObject *parent): QObject(parent), QRunnable()
{
    setAutoDelete (true);
}
void LegalMap::run ()
{
    QString filePath = "D:/cppfinal/Gwidget/images/lp2.txt";
    readFileBySixLines(filePath);
}
void LegalMap::readFileBySixLines(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件：" << filePath;
        return;
    }
    QTextStream in(&file);
    QStringList lines;
    QString line;
    // 读取文件直到结束
    int x = 0;
    while (!in.atEnd()) {
        lines.clear(); // 重置行列表，为下一轮六行数据做准备
        for (int i = 0; i < 6 && !in.atEnd(); ++i) {
            line = in.readLine(); // 读取一行
            lines.append(line); // 添加到当前批次的行列表中
        }
        // 当收集够六行或到达文件末尾时，处理这六行数据
        if (!lines.isEmpty()) {
            if(line.size() == 6) {
                processSixLines(lines);
                x++;
                qDebug() << x << '\n';
            } else {
                break;
            }
        }
    }
    qDebug() << "完成文件加载";
    qDebug() << LMap.size();
    emit finish (LMap);
    file.close();
}
void LegalMap::processSixLines(const QStringList &sixLines)
{
    QVector<QVector<QChar>>Map(6, QVector<QChar>(6, '0'));
    int lineId = 0;
    foreach(const QString &line, sixLines) {
        for(int i = 0; i < 6; ++i) {
            Map[lineId][i] = line[i];
        }
        lineId++;
    }
    LMap[Map] ++;
    //qDebug() << LMap.size() << '\n';
}
