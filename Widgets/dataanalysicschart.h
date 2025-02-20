#ifndef DATAANALYSICSCHART_H
#define DATAANALYSICSCHART_H

#include <QWidget>
#include <QChartView>
#include <QChart>
#include <QLineSeries>
#include <QHash>
#include <QtCharts/QValueAxis>

class DataAnalysicsChart : public QChartView
{
    Q_OBJECT
public:
    DataAnalysicsChart(QWidget *parent=nullptr);
    void addLine(QString name);
    unsigned int max_points;
    unsigned int max_x_range;

public slots:
    void addPoint(QString series, unsigned int x, float y);

private:
    QChart *chart;
    QValueAxis *axisX;
    QValueAxis *axisY;
    float min;
    float max;
    QHash<QString, QLineSeries*> series_list;
};

#endif // DATAANALYSICSCHART_H
