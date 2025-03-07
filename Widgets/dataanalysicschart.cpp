#include "dataanalysicschart.h"
#include <QPen>
#include <QColor>

DataAnalysicsChart::DataAnalysicsChart(QWidget *parent)
    : QCustomPlot(parent) {

    autoScroll = true;
    max_x_range = 10000;
    y_range = 10;
    max_x = max_x_range;

    max_points = 1000000;

    this->yAxis->setRange(-y_range, y_range);
    this->xAxis->setRange(0, max_x);
    this->xAxis->setTicks(false);

    startTimer(30, Qt::CoarseTimer);
}

void DataAnalysicsChart::addLine(QString name) {
    QCPGraph *graph = addGraph();
    graph->setName(name);
    series_list.insert(name, graph);
    series_index_map.insert(name, this->graphCount()-1);
}

void DataAnalysicsChart::setLineColor(QString name, const QColor &color) {
    if (series_index_map.contains(name)) {
        series_list.value(name)->setPen(QPen(color));
    }
}

void DataAnalysicsChart::deleteLine(QString name)
{
    if (!series_list.contains(name)) return;
    QCPGraph *graph = series_list.value(name);
    series_list.remove(name);
    series_index_map.remove(name);
    this->removeGraph(graph);
}

void DataAnalysicsChart::addPoint(QString series, unsigned int x, float y)
{
    if (series_list.contains(series)) {
        series_list.value(series)->addData(x, y);
        float tmp = fabs(y)*1.1;
        if (tmp>y_range) {
            y_range = round(tmp);
        }

        if (max_x<x) {
            max_x = x;
        }

        if (series_list.value(series)->data()->size()>max_points) {
            series_list.value(series)->data()->remove(max_x-max_points);
        }
    }
}

void DataAnalysicsChart::setAutoScroll(bool value)
{
    autoScroll = value;
}

void DataAnalysicsChart::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    if(autoScroll) {
        this->yAxis->setRange(-y_range, y_range);
        this->xAxis->setRange(max_x - xAxis->range().size(), max_x);
    }
    this->replot();
}
