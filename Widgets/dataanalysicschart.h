#ifndef DATAANALYSICSCHART_H
#define DATAANALYSICSCHART_H

#include <QWidget>
#include <QHash>
#include "qcustomplot.h"

class DataAnalysicsChart : public QCustomPlot
{
    Q_OBJECT
public:
    DataAnalysicsChart(QWidget *parent=nullptr);
    void addLine(QString name);
    void deleteLine(QString name);
    unsigned int max_points;
    unsigned int max_x_range;

    void setAutoScroll(bool value);
    void timerEvent(QTimerEvent *event);
    void setLineColor(QString name, const QColor &color);
    void clearData();
    void showHideLine(QString series, bool show);
    void setTracerEnable(bool enable);
    void setRangeDrag(bool enable);
    void setZoom(bool enable);

public slots:
    void addPoint(QString series, unsigned int x, float y);
    void xAxisChanged(const QCPRange &newRange, const QCPRange &oldRange);
    void yAxisChanged(const QCPRange &newRange, const QCPRange &oldRange);

signals:
    void tracerEnableChanged(bool enable);
    void autoScrollChanged(bool enable);
    void rangeDragChanged(bool enable);
    void zoomStatusChanged(bool enable);

private:
    QHash<QString, QCPGraph*> series_list;
    QHash<QString, unsigned int> series_index_map;
    int timer_id;
    bool autoScroll;
    QCPItemTracer *tracer;
    QCPItemText *tracerLabel;
    bool tracerEnable;

    float max_y_value;
    float min_y_value;
    double max_x_value;
    double min_x_value;
    bool has_value;

    virtual void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
};

#endif // DATAANALYSICSCHART_H
