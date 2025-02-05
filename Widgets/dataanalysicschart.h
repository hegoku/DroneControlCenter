#ifndef DATAANALYSICSCHART_H
#define DATAANALYSICSCHART_H

#include <QWidget>
#include <QChartView>
#include <QChart>
#include <QLineSeries>

class DataAnalysicsChart : public QChartView
{
    Q_OBJECT
public:
    DataAnalysicsChart(QWidget *parent=nullptr);
    void addLine(QString name);

private:
    QChart *chart;
    QList<QLineSeries*> series_list;
};

#endif // DATAANALYSICSCHART_H
