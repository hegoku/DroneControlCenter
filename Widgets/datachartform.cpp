#include "datachartform.h"
#include "ui_datachartform.h"
#include <QTimer>
#include <QThread>
#include <QValueAxis>

DataChartForm::DataChartForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DataChartForm)
{
    ui->setupUi(this);

    connect(ui->horizontalScrollBar, &QScrollBar::valueChanged, this, &DataChartForm::changeScroll);

    ui->chartView->addLine("X");
    ui->chartView->addLine("Y");

    t=0;
    y=0;
    // connect(this, &DataChartForm::draw, ui->chartView, &DataAnalysicsChart::addPoint);

    QTimer *timer = new QTimer();
    // QThread *timerThread = new QThread;
    // timer->moveToThread(timerThread);
    connect(timer, &QTimer::timeout, this, &DataChartForm::onDataComing);
    timer->setInterval(1);
    // timer->start();
    // timerThread->start();
}

DataChartForm::~DataChartForm()
{
    delete ui;
}


void DataChartForm::onDataComing()
{
    t++;
    y=sin(2*3.14*t);
    // emit draw("X", 0, 1234);
    // series->append(t, y);
    ui->chartView->addPoint("X", t, y);
    y=cos(2*3.14*t);
    if (t%100==0) {
        ui->chartView->addPoint("Y", t, y);
    }
    // ui->horizontalScrollBar->setValue(ui->horizontalScrollBar->maximum());
    // ui->horizontalScrollBar->setRange(0,t);
}

void DataChartForm::changeScroll(int value)
{
    qDebug("%d", value);
    unsigned int a = value*ui->chartView->getXSize()/ui->horizontalScrollBar->maximum();
    ui->chartView->setXAxisRange(a-ui->chartView->max_x_range, a);
}
