#ifndef PASWORD_H
#define PASWORD_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QValidator>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QEvent>
#include <QDebug>

class PasWord : public QWidget
{
    Q_OBJECT
public:
    explicit PasWord(QWidget *parent = 0);
    QString text() const;
protected:
    bool eventFilter(QObject *watched, QEvent *event);
private:
    QLineEdit *PassEdit;
};

#endif // PASWORD_H
