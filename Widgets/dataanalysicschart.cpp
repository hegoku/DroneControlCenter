#include "dataanalysicschart.h"

DataAnalysicsChart::DataAnalysicsChart(QWidget *parent)
    : QChartView(parent) {

    setRubberBand(QChartView::RectangleRubberBand);

    autoScroll = true;
    max_x_range = 10000;

    chart = new QChart();
    chart->legend()->hide();
    setChart(chart);

    axisX = new QValueAxis(this);
    axisY = new QValueAxis(this);
    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);

    axisX->setRange(0, max_x_range);
    axisX->setTickCount(11);

    max_points = max_x_range+100;
}

void DataAnalysicsChart::addLine(QString name) {
    QLineSeries *series = new QLineSeries(this);
    series->setName(name);

    chart->addSeries(series);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    series_list.insert(name, series);
}

void DataAnalysicsChart::addPoint(QString series, unsigned int x, float y)
{
    if (series_list.contains(series)) {
        series_list.value(series)->append(x, y);
        if(autoScroll && x > max_x_range) {
            qreal scroll = chart->plotArea().width() / max_x_range;
            chart->scroll(scroll,0);
            axisX->setRange(x-max_x_range, x);
        }
        if (min>y) {
            min = y;
        }
        if (max<y) {
            max = y;
        }

        axisY->setRange(min, max);
        if (series_list.value(series)->count()>max_points) {
            series_list.value(series)->removePoints(0, 1);
        }
    }
}

void DataAnalysicsChart::setAutoScroll(bool value)
{
    autoScroll = value;
}
