#ifndef EXPLANATION_H
#define EXPLANATION_H

#include <QLabel>
#include <QWidget>
#include <QCloseEvent>

class Explanation : public QWidget
{
    Q_OBJECT
public:
    explicit Explanation(QWidget *parent = nullptr);
    //~Explanation();

protected:
    void closeEvent(QCloseEvent *Event) override;

signals:
    void aboutToClose();

private:
    QLabel *Title;
    QLabel *Rule;
    QLabel *Step;

};

#endif // EXPLANATION_H
