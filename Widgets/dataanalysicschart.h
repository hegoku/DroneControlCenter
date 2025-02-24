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

public slots:
    void addPoint(QString series, unsigned int x, float y);

private:
    float y_range;
    unsigned long long max_x;
    QHash<QString, QCPGraph*> series_list;
    QHash<QString, unsigned int> series_index_map;

    bool autoScroll;
};

#endif // DATAANALYSICSCHART_H
