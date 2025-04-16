#ifndef DATACHARTFORM_H
#define DATACHARTFORM_H

#include <QWidget>
#include <QLineSeries>
#include <QHash>
#include <QList>
#include <Anotc/anotc_data_frame.h>
#include "util.h"
#include "Widgets/chartlistitem.h"

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
    QLineSeries *series;
    void deleteLine(unsigned char func, unsigned char seq);

public slots:
    void onDataComing(struct anotc_parsed_data_frame);
    void changeScroll(int value);
    void start();
    void addLine(unsigned char func, unsigned char seq);
    void removeLine(ChartListItem* item);
    void hideLine(ChartListItem* item, bool show);
    void clearData();
    void toggleTracer(bool checked);
    void toggleDrag(bool checked);
    void toggleAutoScroll(bool checked);
    void toggleZoom(bool checked);
    void tracerChanaged(bool enable);
    void autoScrollChanaged(bool enable);
    void dragChanaged(bool enable);
    void zoomChanged(bool enable);
    void saveData();
    void hideAllLine();

private:
    Ui::DataChartForm *ui;

    QHash<unsigned short, ChartListItem*> frame_hash;
    bool is_start;
    std::vector<ColorType> color_list;
};

#endif // DATACHARTFORM_H
