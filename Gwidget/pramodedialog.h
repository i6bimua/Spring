#ifndef PRAMODEDIALOG_H
#define PRAMODEDIALOG_H

#include <QDialog>
#include <QCloseEvent>
//#include "optionbutton.h"
#include <QStack>
#include <QTimer>
#include <QTime>
#include <QLabel>

namespace Ui
{
    class PraModeDialog;
}

class PraModeDialog : public QDialog
{
    Q_OBJECT

private:
    // 记录操作的选项的序号，用于撤回后退
    QStack<int> okBtn;

    // 存储七个选项的控件及其在scrollArea中的序号
    QMap<int, QLabel *> optionBtn;

    // scrollArea 控件的区域(选项的区域)
    QRect ScrollRect;

    // 作答的区域
    QRect WidgetRect;

    // 关卡地图数据(从文件加载的所有关卡地图数据)
    QVector<QVector<QVector<int>>> vctMaps;

    // 默认选项
    QVector<int> vctDefaultSlice = {4, 3, 3, 2, 2, 1, 1};

    // 选项数据结构体
    typedef struct mapDataStruct {
        int height;
        int width;
        QVector<QVector<int>> data;
    } MapDataStruct;

    // 选项数据
    MapDataStruct selectMap;
    QVector<MapDataStruct> vctSelectMaps;

    // 当前关卡
    int nCurLevel;

    // 当前关卡地图
    QVector<QVector<int>> vctCurMap;

    // 记录成功移动选项后当前地图状态
    QVector<QVector<int>> vctCurSelectedMap;

    // 记录每次成功移动选项后的地图状态
    QStack<QVector<QVector<int>>> stackMapRecord;
    typedef struct TipStruct {
        int x;
        int y;
        int len;
    } tip;

    QStack<std::vector<tip>>stackTipRecord;

    tip lastPoint;

    std::vector<tip>TipVec;

    void init();

    void dfs(int x, int y, int cnt);

    int temp[10][10];

    bool flag = false;

    int size[10] = {0, 4, 3, 3, 2, 2, 1, 1};

    struct sd {
        int x, y;
    };

public:
    QTime *qtime;
    QTimer *qtimer;
    QTimer *TipTimer;

    QTime *qtime_com;
    QTimer *qtimer_com;

public slots:
    void timeout_slot();
    // void time_comout_slot();
    void Recover();

private:
    bool loadData(QString CurMapStr);
    void createData();
    void createSubMapDataArrary(int width, int height, int n, int o);
    void mergeMapDataArrary(int width, int height);
    bool isPrettifyData(void);
    void NextLevel();
    void Tip();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

public:
    void closeEvent(QCloseEvent *)override;

    QString CurMapString = "./map/map1.txt";

signals:
    void ReturnMain();
    void VicSig();
    void Nextlevel();

public:
    explicit PraModeDialog(QWidget *parent = 0, QString CurMapStrr = "./map/map1.txt");
    ~PraModeDialog();

private slots:
    void on_pushButton_ret_clicked();
    void on_pushButton_help_clicked();
    void on_pushButton_clicked();
    void on_pushButton_back_clicked();
    void on_pushButton_reset_clicked();
    void on_pushButton_click_start_clicked();
    void on_pushButton_next_level_clicked();

    void Up_comMap(QByteArray Messagereceive);
    void Up_comMapData(QByteArray Messagereceive);
    void Up_comTime(int code);

public:
    Ui::PraModeDialog *ui;
};

#endif // PRAMODEDIALOG_H
