#ifndef CREATEDATA_H
#define CREATEDATA_H

#include <QObject>

class CreateData : public QObject
{
    Q_OBJECT
public:
    explicit CreateData(QObject *parent = nullptr);

signals:
};

#endif // CREATEDATA_H
