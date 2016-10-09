#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QTimer"

#include <QtSerialPort>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("QMainWindow {background: 'white';}");
    configurePlot(ui->plotAngle, "Czas [s]", "Kąt nachylenia \npodłoża [°]", qMakePair(-15, 15));
    configurePlot(ui->plotCadence, "Czas [s]", "\nKadencja [obr/min]", qMakePair(-5, 99));
    configurePlot(ui->plotVelocity, "Czas [s]", "Prędkość\nroweru [km/h]", qMakePair(-2, 40));
    configurePlot(ui->plotGear, "Czas [s]", "Numer\nprzełożenia", qMakePair(0.6, 8.3), true, 6);
    configurePlot(ui->plotShiftDown, "Czas [s]", "Redukcja\nbiegu", qMakePair(-0.2, 1.2), true, 1);
    configurePlot(ui->plotShiftUp, "Czas [s]", "Zwiększenie\nbiegu", qMakePair(-0.2, 1.2), true, 1);
    addGraph(ui->plotCadence, QColor(139, 121, 94));
    addGraph(ui->plotCadence, QColor(139, 121, 94));
    addGraph(ui->plotAngle, QColor(139, 121, 94));
    addGraph(ui->plotAngle, QColor(139, 121, 94));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    QTimer *dataTimer = new QTimer(this);
    connect(dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    dataTimer->start(0); // Interval 0 means to refresh as fast as possible

    serial = new QSerialPort(this);
    openSerialPort();
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::handleUartData);
}

void MainWindow::realtimeDataSlot(void)
{
    static QTime time(QTime::currentTime());
    static const int xRange = 16;
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
      // add data to lines:
      ui->plotAngle->graph(0)->addData(key, bikeParams.surfaceAngle);

      ui->plotCadence->graph(0)->addData(key, bikeParams.cadence);
      if(bikeParams.currentMode != static_cast<int> (Modes::Remote))
      {
          ui->plotCadence->graph(1)->addData(key, getCadenceRange(bikeParams.currentMode, bikeParams.surfaceAngle).first);
          ui->plotCadence->graph(2)->addData(key, getCadenceRange(bikeParams.currentMode, bikeParams.surfaceAngle).second);
      }
      if(bikeParams.currentMode == static_cast<int> (Modes::Sport))
      {
          ui->plotAngle->graph(1)->addData(key, -3);
          ui->plotAngle->graph(2)->addData(key, 3);
      }
      ui->plotVelocity->graph(0)->addData(key, bikeParams.velocity);

      ui->plotGear->graph(0)->addData(key, bikeParams.currentGear);

      ui->plotShiftDown->graph(0)->addData(key, bikeParams.upShift);

      ui->plotShiftUp->graph(0)->addData(key, bikeParams.downShift);

      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->plotAngle->xAxis->setRange(key, xRange, Qt::AlignRight);
    ui->plotAngle->replot();

    ui->plotCadence->xAxis->setRange(key, xRange, Qt::AlignRight);
    ui->plotCadence->replot();

    ui->plotVelocity->xAxis->setRange(key, xRange, Qt::AlignRight);
    ui->plotVelocity->replot();

    ui->plotGear->xAxis->setRange(key, xRange, Qt::AlignRight);
    ui->plotGear->replot();

    ui->plotShiftDown->xAxis->setRange(key, xRange, Qt::AlignRight);
    ui->plotShiftDown->replot();

    ui->plotShiftUp->xAxis->setRange(key, xRange, Qt::AlignRight);
    ui->plotShiftUp->replot();
}

void MainWindow::configurePlot(QCustomPlot *plot, QString xLabel, QString yLabel, QPair<double, double> yRange, bool setTicker, int tickerLength, int fontSize)
{
    addGraph(plot, QColor(40, 110, 255));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    plot->xAxis->setTicker(timeTicker);
    plot->axisRect()->setupFullAxesBox();
    // make left and bottom axes transfer their ranges to right and top axes:
    connect(plot->xAxis, SIGNAL(rangeChanged(QCPRange)), plot->xAxis2, SLOT(setRange(QCPRange)));
    connect(plot->yAxis, SIGNAL(rangeChanged(QCPRange)), plot->yAxis2, SLOT(setRange(QCPRange)));
    plot->xAxis->setLabel(xLabel);

    if(setTicker)
    {
        plot->yAxis->setSubTicks(false);
        plot->yAxis->ticker()->setTickCount(tickerLength);
        plot->yAxis2->setSubTicks(false);
        plot->yAxis2->ticker()->setTickCount(tickerLength);

    }
    plot->yAxis->setLabel(yLabel);
    plot->yAxis->setRange(yRange.first, yRange.second);
    plot->xAxis->setLabelFont(QFont("DejaVu Sans", 11));
    plot->xAxis->setTickLabelFont(QFont("DejaVu Sans", 11));

    plot->yAxis->setLabelFont(QFont("DejaVu Sans", fontSize));
    plot->yAxis->setTickLabelFont(QFont("DejaVu Sans", fontSize));
    plot->yAxis->setLabelPadding(35);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addGraph(QCustomPlot *plot, QColor color)
{
    plot->addGraph(); // blue line
    plot->graph(plot->graphCount()-1)->setPen(QPen(color));
}

void MainWindow::handleUartData(void)
{
    QByteArray data = serial->readAll();
    while(serial->waitForReadyRead(10)) {
            data += serial->readAll();
    }
    QList<QByteArray> params = data.split(';');

    if(7 == params.size())
    {
        bikeParams.setParams(params);
    }

    bikeParams.printParams();

}

void MainWindow::openSerialPort()
{
    serial->setPortName("/dev/ttyACM0");
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (serial->open(QIODevice::ReadOnly))
    {
        qDebug() << "Serial port opened";
    }
    else
    {
        qDebug() << "Could not open serial port";
        QCoreApplication::exit();
    }

}

QPair<int, int> MainWindow::getCadenceRange(int currentMode, int currentAngle)
{
    static QMap<int, QPair<int, int>> cadenceRanges =
    {
        {0, QPair<int, int>(0,0)},
        {1, QPair<int, int>(45,60)},
        {2, QPair<int, int>(55,70)},
        {3, QPair<int, int>(65,80)},
        {4, QPair<int, int>(55,70)},

    };
    if(currentMode == 3 && (currentAngle < -3 || currentAngle > 3))
        return cadenceRanges[4];
    return cadenceRanges[currentMode];
}

void BikeParams::setParams(QList<QByteArray> &params)
{
    try
    {
        currentGear = params.at(0).toInt() + 1;
        currentMode = params.at(1).toInt();
        downShift = params.at(2).toInt();
        upShift = params.at(3).toInt();
        velocity = params.at(4).toDouble();
        surfaceAngle = params.at(5).toDouble();
        cadence = params.at(6).toDouble();
    }
    catch(...)
    {
        qDebug() << "Data received on serial port probably was corrupted";
    }
}


void BikeParams::printParams()
{
    qDebug() << currentGear << " " << currentMode << " " << downShift << " " << upShift << " " << velocity << " " << surfaceAngle << " " << cadence ;
}

