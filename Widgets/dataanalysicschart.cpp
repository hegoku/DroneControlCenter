#include "dataanalysicschart.h"

DataAnalysicsChart::DataAnalysicsChart(QWidget *parent)
    : QChartView(parent) {

    setRubberBand(QChartView::RectangleRubberBand);

    chart = new QChart();
    chart->legend()->hide();
    setChart(chart);

}


void DataAnalysicsChart::addLine(QString name) {
    QLineSeries *series = new QLineSeries();
    series->setName(name);

    chart->addSeries(series);
}
