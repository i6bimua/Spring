#ifndef GETMAP_H
#define GETMAP_H

#include <QWidget>
#include <QLineEdit>
namespace Ui
{
    class GetMap;
}

class GetMap : public QWidget
{
    Q_OBJECT

public:
    explicit GetMap(QWidget *parent = nullptr);
    ~GetMap();

private:
    Ui::GetMap *ui;
    std::vector<std::vector<QLineEdit *> > lineEditMatrix;
    void initializeMatrixLineEdits();
    QPixmap fitimgpic;
    void paintEvent(QPaintEvent *event)override;
    void closeEvent(QCloseEvent *event)override;
    int matrix[10][10];
    int temp[10][10];
    void init();
    void dfs(int x, int y, int cnt);
    bool flag = false;
    int size[10] = {0, 4, 3, 3, 2, 2, 1, 1};
    struct sd {
        int x, y;
    };
    int getMapNum();
    std::map<QVector<QVector<int>>, bool>LMap;
    void readFileBySixLines(const QString &filePath, int index);
    void processSixLines(const QStringList &sixLines);

public slots:
    void onReadMatrixButtonClicked();

signals:
    void ReturnMin();
};

#endif // GETMAP_H
