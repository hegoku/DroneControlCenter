#ifndef DATAANALYSICSCHART_H
#define DATAANALYSICSCHART_H

#include <QChartView>
#include <QChart>
#include <QLineSeries>

class DataAnalysicsChart : public QChartView
{
    Q_OBJECT
public:
    DataAnalysicsChart();
    void addLine(QString name);

private:
    QChart *chart;
    QList<QLineSeries*> series_list;
};

#endif // DATAANALYSICSCHART_H
