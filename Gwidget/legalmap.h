#ifndef LEGALMAP_H
#define LEGALMAP_H

#include <QObject>
#include <QRunnable>

class LegalMap : public QObject, public QRunnable
{
    Q_OBJECT
public:
    LegalMap(QObject *parent);
    void run()override;
private:
    std::map<QVector<QVector<QChar>>, int>LMap;
    void readFileBySixLines(const QString &filePath);
    void processSixLines(const QStringList &sixLines);

signals:
    void finish(std::map<QVector<QVector<QChar>>, int>Map);

};

#endif // LEGALMAP_H
