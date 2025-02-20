#ifndef DATACHARTFORM_H
#define DATACHARTFORM_H

#include <QWidget>
#include <QLineSeries>

namespace Ui {
class DataChartForm;
}

class DataChartForm : public QWidget
{
    Q_OBJECT

public:
    explicit DataChartForm(QWidget *parent = nullptr);
    ~DataChartForm();
    unsigned int t;
    float y;
    QLineSeries *series;

public slots:
    void onDataComing();

signals:
    void draw(QString name, float x, float y);

private:
    Ui::DataChartForm *ui;
};

#endif // DATACHARTFORM_H
