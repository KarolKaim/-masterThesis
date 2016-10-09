#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QPair>
#include <QMap>
#include <qcustomplot.h>

class BikeParams
{
public:
    int currentGear = 1;
    int currentMode = 3;
    int downShift = 1;
    int upShift = 0;
    double surfaceAngle = 0;
    double velocity = 0;
    double cadence = 0;
    void setParams(QList<QByteArray> &parmas);
    void printParams();
};

enum class Modes {Remote, Comfort, Active, Sport};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void realtimeDataSlot(void);
    void handleUartData(void);
    void openSerialPort(void);
private:
    Ui::MainWindow *ui;
    void configurePlot(QCustomPlot *plot, QString xLabel, QString yLabel, QPair<double, double> yRange, bool setTicker = false, int tickerLength = 0, int fontSize = 11);
    QSerialPort *serial;
    BikeParams bikeParams;
    void addGraph(QCustomPlot *plot, QColor color);
    QPair<int, int> getCadenceRange(int currentMode, int currentAngle);

};

#endif // MAINWINDOW_H
