#include "dataanalysicschart.h"
#include <QPen>
#include <QColor>
#include <math.h>

DataAnalysicsChart::DataAnalysicsChart(QWidget *parent)
    : QCustomPlot(parent) {

    autoScroll = false;
    max_x_range = 5000;

    max_points = 1000000;

    max_y_value = this->yAxis->range().upper;
    min_y_value = this->yAxis->range().lower;
    max_x_value = max_x_range;
    min_x_value = 0;
    has_value = false;

    this->xAxis->setRange(0, max_x_value);
    // this->xAxis->setTicks(false);

    timer_id = startTimer(30, Qt::CoarseTimer);
    tracer = new QCPItemTracer(this);
    tracer->setVisible(false);
    QPen pen = tracer->pen();
    pen.setColor(QColor(Qt::gray));
    pen.setStyle(Qt::DashDotDotLine);
    pen.setWidthF(1.5);
    tracer->setPen(pen);
    tracer->setStyle(QCPItemTracer::tsCrosshair);

    tracerLabel = new QCPItemText(this);
    tracerLabel->setVisible(false);
    tracerLabel->setLayer("overlay");                                 //设置图层为overlay，因为需要频繁刷新
    pen = tracerLabel->pen();
    pen.setColor(QColor(Qt::gray));
    tracerLabel->setPen(pen);                              //设置游标说明颜色
    tracerLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignBottom);   //右上
    tracerLabel->setFont(QFont(font().family(),10));                   //字体大小
    tracerLabel->setPadding(QMargins(4,4,4,4));                        //文字距离边框几个像素
    tracerLabel->position->setParentAnchor(tracer->position);

    connect(xAxis, SIGNAL(rangeChanged(QCPRange,QCPRange)), this, SLOT(xAxisChanged(QCPRange,QCPRange)));
    connect(yAxis, SIGNAL(rangeChanged(QCPRange,QCPRange)), this, SLOT(yAxisChanged(QCPRange,QCPRange)));
}

void DataAnalysicsChart::addLine(QString name) {
    QCPGraph *graph = addGraph();
    graph->setName(name);
    series_list.insert(name, graph);
    series_index_map.insert(name, this->graphCount()-1);
}

void DataAnalysicsChart::setLineColor(QString name, const QColor &color) {
    if (series_index_map.contains(name)) {
        QPen pen = series_list.value(name)->pen();
        pen.setColor(color);
        pen.setWidthF(1.1);
        series_list.value(name)->setPen(pen);
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

        if (has_value==false) {
            has_value = true;
            max_y_value = y;
            min_y_value = y;
        } else {
            if (y>max_y_value) {
                max_y_value = y;
            }
            if (y<min_y_value) {
                min_y_value = y;
            }
            if (min_x_value>x) {
                min_x_value = x;
            }
            if (max_x_value<x) {
                max_x_value = x;
            }
        }

        if (series_list.value(series)->data()->size()>max_points) {
            series_list.value(series)->data()->remove(max_x_value-max_points);
        }
    }
}

void DataAnalysicsChart::setAutoScroll(bool value)
{
    autoScroll = value;
    if (autoScroll) {
        this->setInteraction(QCP::iRangeDrag, false);
        this->setInteraction(QCP::iRangeZoom, false);
        emit rangeDragChanged(false);
        this->setSelectionRectMode(QCP::SelectionRectMode::srmNone);
        emit zoomStatusChanged(false);
    }
    emit autoScrollChanged(value);
}

void DataAnalysicsChart::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    if(autoScroll) {
        // this->yAxis->rescale(true);
        if (has_value) {
            this->yAxis->setRange(min_y_value, max_y_value);
            this->xAxis->setRange(max_x_value - xAxis->range().size(), max_x_value);
        }
    }
    this->replot();
}

void DataAnalysicsChart::clearData()
{
    killTimer(timer_id);
    for (auto i = series_list.cbegin(), end = series_list.cend(); i != end; ++i) {
        i.value()->data()->clear();
    }
    has_value = false;
    max_x_value = max_x_range;
    min_x_value = 0;
    this->xAxis->setRange(0, max_x_range);
    this->replot();
    timer_id = startTimer(30, Qt::CoarseTimer);
}

void DataAnalysicsChart::showHideLine(QString series, bool show)
{
    if (series_list.contains(series)) {
        series_list.value(series)->setVisible(show);
    }
}

void DataAnalysicsChart::mouseMoveEvent(QMouseEvent *event)
{
    QCustomPlot::mouseMoveEvent(event);

    if (!tracerEnable) return;
    int x = this->xAxis->pixelToCoord(event->pos().x());
    double y = this->yAxis->pixelToCoord(event->pos().y());
    if (event->pos().y() > this->axisRect()->bottom()) {
        y = this->yAxis->pixelToCoord(this->axisRect()->bottom());
    }
    if (event->pos().y() < this->axisRect()->top()) {
        y = this->yAxis->pixelToCoord(this->axisRect()->top());
    }
    if (event->pos().x() < this->axisRect()->left()) {
        x = this->xAxis->pixelToCoord(this->axisRect()->left());
    }
    if (event->pos().x() > this->axisRect()->right()) {
        x = this->xAxis->pixelToCoord(this->axisRect()->right());
    }

    tracer->position->setCoords(x,y);
    tracerLabel->setText(QString("X:%1\nY:%2").arg(QString::number(x)).arg(QString::number(y, 'f', 2)));
    // this->replot();
}

void DataAnalysicsChart::setTracerEnable(bool enable)
{
    this->tracerEnable = enable;
    tracer->setVisible(enable);
    tracerLabel->setVisible(enable);
    emit rangeDragChanged(false);
}

void DataAnalysicsChart::setRangeDrag(bool enable)
{
    if (enable) {
        this->autoScroll = false;
        this->setInteraction(QCP::iRangeDrag, true);
        emit autoScrollChanged(false);
    } else {
        this->setInteraction(QCP::iRangeDrag, false);
    }
    emit rangeDragChanged(enable);
}

void DataAnalysicsChart::setZoom(bool enable)
{
    if (enable) {
        this->autoScroll = false;
        this->setInteraction(QCP::iRangeZoom, true);
        this->setSelectionRectMode(QCP::SelectionRectMode::srmZoom);
        emit autoScrollChanged(false);
    } else {
        this->setInteraction(QCP::iRangeZoom, false);
        this->setSelectionRectMode(QCP::SelectionRectMode::srmNone);
    }
    emit zoomStatusChanged(enable);
}

void DataAnalysicsChart::xAxisChanged(const QCPRange &newRange, const QCPRange &oldRange)
{
    if (newRange.lower<0) {
        this->xAxis->setRangeLower(0);
        if (newRange.size()<oldRange.size()) {
            this->xAxis->setRangeUpper(oldRange.size());
        }
    }
    if (newRange.upper>max_x_value) {
        this->xAxis->setRangeUpper(max_x_value);
        if (newRange.size()<oldRange.size()) {
            this->xAxis->setRangeLower(max_x_value-oldRange.size());
        }
    }
}

void DataAnalysicsChart::yAxisChanged(const QCPRange &newRange, const QCPRange &oldRange)
{
    if (newRange.lower<min_y_value) {
        this->yAxis->setRangeLower(min_y_value);
        if (newRange.size()<oldRange.size()) {
            this->yAxis->setRangeUpper(oldRange.size());
        }
    }
    if (newRange.upper>max_y_value) {
        this->yAxis->setRangeUpper(max_y_value);
        if (newRange.size()<oldRange.size()) {
            this->yAxis->setRangeLower(max_y_value-oldRange.size());
        }
    }
}

QCPGraph* DataAnalysicsChart::getLine(QString name)
{
    if (series_list.contains(name)) {
        return series_list.value(name);
    }
    return 0;
}
