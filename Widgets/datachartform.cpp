#include "datachartform.h"
#include "ui_datachartform.h"
#include <QTimer>
#include <QThread>
#include <QValueAxis>
#include <QColor>
#include "Widgets/chartlistitem.h"

DataChartForm::DataChartForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DataChartForm)
{
    ui->setupUi(this);

    is_start = false;

    connect(ui->horizontalScrollBar, &QScrollBar::valueChanged, this, &DataChartForm::changeScroll);
    connect(ui->start_toolButton, &QPushButton::clicked, this, &DataChartForm::start);
    connect(ui->clear_toolButton, &QPushButton::clicked, this, &DataChartForm::clearData);
    connect(ui->hideall_btn, &QPushButton::clicked, this, &DataChartForm::hideAllLine);

    connect(ui->tracer_toolButton, &QToolButton::clicked, this, &DataChartForm::toggleTracer);
    connect(ui->drag_toolButton, &QToolButton::clicked, this, &DataChartForm::toggleDrag);
    connect(ui->autoscroll_toolButton, &QToolButton::clicked, this, &DataChartForm::toggleAutoScroll);
    connect(ui->zoom_toolButton, &QToolButton::clicked, this, &DataChartForm::toggleZoom);
    connect(ui->save_toolButton, &QToolButton::clicked, this, &DataChartForm::saveData);

    connect(ui->chartView, &DataAnalysicsChart::tracerEnableChanged, this, &DataChartForm::tracerChanaged);
    connect(ui->chartView, &DataAnalysicsChart::autoScrollChanged, this, &DataChartForm::autoScrollChanaged);
    connect(ui->chartView, &DataAnalysicsChart::rangeDragChanged, this, &DataChartForm::dragChanaged);
    connect(ui->chartView, &DataAnalysicsChart::zoomStatusChanged, this, &DataChartForm::zoomChanged);

    ui->listWidget->setStyleSheet("QListView::item:selected {background-color:white;}");

    t=0;

    std::vector<ColorType> e;
    getUniqueColors(50, color_list, e);
}

DataChartForm::~DataChartForm()
{
    delete ui;
}

void DataChartForm::addLine(unsigned char func, unsigned char seq)
{
    unsigned short id = (unsigned short)func<<8 | seq;
    if (!frame_hash.contains(id)) {
        if (!anotc_frame_defination_list.contains(func)) return;
        if (anotc_frame_defination_list.value(func)->params.size()>seq) {
            ui->chartView->addLine(anotc_frame_defination_list.value(func)->params.at(seq)->name);
            ColorType c = color_list.at(frame_hash.size());
            ui->chartView->setLineColor(anotc_frame_defination_list.value(func)->params.at(seq)->name, QColor(c.mRed, c.mGreen, c.mBlue));
            QListWidgetItem* pItem =new QListWidgetItem();
            ChartListItem* widget = new ChartListItem();
            widget->setText(anotc_frame_defination_list.value(func)->params.at(seq)->name);
            widget->setValue("0.00");
            widget->setCheckBoxColor(QColor(c.mRed, c.mGreen, c.mBlue));
            widget->setId(id);
            widget->setChecked(true);
            pItem->setSizeHint(widget->sizeHint());
            ui->listWidget->addItem(pItem);
            ui->listWidget->setItemWidget(pItem, widget);
            connect(widget, &ChartListItem::onRemove, this, &DataChartForm::removeLine);
            connect(widget, &ChartListItem::onCheckClick, this, &DataChartForm::hideLine);
            // frame_hash.insert(id, ui->listWidget->count()-1);
            frame_hash.insert(id, widget);
        }
    }
}

void DataChartForm::deleteLine(unsigned char func, unsigned char seq)
{
    unsigned short id = (unsigned short)func<<8 | seq;
    if (frame_hash.contains(id)) {
        frame_hash.remove(id);
        ui->chartView->deleteLine(anotc_frame_defination_list.value(func)->params.at(seq)->name);
    }
}

void DataChartForm::removeLine(ChartListItem* item)
{
    if (frame_hash.contains(item->getId())) {
        unsigned char seq = (unsigned char)item->getId();
        unsigned char func = item->getId()>>8;
        deleteLine(func, seq);
        for (int i=0;i<ui->listWidget->count();i++) {
            ChartListItem *tmp = dynamic_cast<ChartListItem*>(ui->listWidget->itemWidget(ui->listWidget->item(i)));
            if (tmp->getId()==item->getId()) {
                ui->listWidget->removeItemWidget(ui->listWidget->item(i));
                delete ui->listWidget->item(i);
                delete item;
            }
        }
    }
}

void DataChartForm::onDataComing(struct anotc_parsed_data_frame item)
{
    if (is_start==false) return;
    bool has_data = false;
    unsigned short id = (unsigned short)item.func<<8;
    for (int i=0;i<item.frame_value.size();i++) {
        if (frame_hash.contains(id|i)) {
            has_data = true;
            // ChartListItem *tmp = dynamic_cast<ChartListItem*>(ui->listWidget->itemWidget(ui->listWidget->item(frame_hash.value(id|i))));
            ChartListItem *tmp = frame_hash.value(id|i);
            switch(item.frame_value.at(i).type) {
            case 0:
                ui->chartView->addPoint(item.frame_value.at(i).name, t, item.frame_value.at(i).value.uint8);
                tmp->setValue(QString::number(item.frame_value.at(i).value.uint8));
                break;
            case 1:
                ui->chartView->addPoint(item.frame_value.at(i).name, t, item.frame_value.at(i).value.int8);
                tmp->setValue(QString::number(item.frame_value.at(i).value.int8));
                break;
            case 2:
                ui->chartView->addPoint(item.frame_value.at(i).name, t, item.frame_value.at(i).value.uint16);
                tmp->setValue(QString::number(item.frame_value.at(i).value.uint16));
                break;
            case 3:
                ui->chartView->addPoint(item.frame_value.at(i).name, t, item.frame_value.at(i).value.int16);
                tmp->setValue(QString::number(item.frame_value.at(i).value.int16));
                break;
            case 4:
                ui->chartView->addPoint(item.frame_value.at(i).name, t, item.frame_value.at(i).value.uint32);
                tmp->setValue(QString::number(item.frame_value.at(i).value.uint32));
                break;
            case 5:
                ui->chartView->addPoint(item.frame_value.at(i).name, t, item.frame_value.at(i).value.int32);
                tmp->setValue(QString::number(item.frame_value.at(i).value.int32));
                break;
            case 6:
                ui->chartView->addPoint(item.frame_value.at(i).name, t, item.frame_value.at(i).value.uint64);
                tmp->setValue(QString::number(item.frame_value.at(i).value.uint64));
                break;
            case 7:
                ui->chartView->addPoint(item.frame_value.at(i).name, t, item.frame_value.at(i).value.int64);
                tmp->setValue(QString::number(item.frame_value.at(i).value.int64));
                break;
            case 8:
                ui->chartView->addPoint(item.frame_value.at(i).name, t, item.frame_value.at(i).value.f);
                tmp->setValue(QString::number(item.frame_value.at(i).value.f));
                break;
            case 9:
                ui->chartView->addPoint(item.frame_value.at(i).name, t, item.frame_value.at(i).value.d);
                tmp->setValue(QString::number(item.frame_value.at(i).value.d));
                break;
            }
        }
    }
    if (has_data) {
        t++;
    }
}

void DataChartForm::changeScroll(int value)
{
    // unsigned int a = value*ui->chartView->getXSize()/ui->horizontalScrollBar->maximum();
    // ui->chartView->setXAxisRange(a-ui->chartView->max_x_range, a);
}

void DataChartForm::start()
{
    if (ui->start_toolButton->isChecked()) {
        is_start = true;
        ui->start_toolButton->setText("||");
    } else {
        is_start = false;
        ui->start_toolButton->setText(">");
    }
}

void DataChartForm::clearData()
{
    bool start = is_start;
    is_start=false;
    t=0;
    ui->chartView->clearData();
    is_start = start;
}

void DataChartForm::toggleTracer(bool checked)
{
    this->ui->chartView->setTracerEnable(checked);
}

void DataChartForm::toggleDrag(bool checked)
{
    this->ui->chartView->setRangeDrag(checked);
}

void DataChartForm::toggleAutoScroll(bool checked)
{
    this->ui->chartView->setAutoScroll(checked);
}

void DataChartForm::toggleZoom(bool checked)
{
    this->ui->chartView->setZoom(checked);
}

void DataChartForm::tracerChanaged(bool enable)
{
    this->ui->tracer_toolButton->setChecked(enable);
}

void DataChartForm::dragChanaged(bool enable)
{
    ui->drag_toolButton->setChecked(enable);
}

void DataChartForm::autoScrollChanaged(bool enable)
{
    ui->autoscroll_toolButton->setChecked(enable);
}

void DataChartForm::zoomChanged(bool enable)
{
    ui->zoom_toolButton->setChecked(enable);
}

void DataChartForm::hideLine(ChartListItem* item, bool show)
{
    if (frame_hash.contains(item->getId())) {
        unsigned char seq = (unsigned char)item->getId();
        unsigned char func = item->getId()>>8;
        ui->chartView->showHideLine(anotc_frame_defination_list.value(func)->params.at(seq)->name, show);
    }
}

void DataChartForm::saveData()
{
    t++;
    for (long long x=0; x<t; x++) {
        for (auto i = frame_hash.cbegin(), end = frame_hash.cend(); i != end; ++i) {
            unsigned char seq = (unsigned char)i.key();
            unsigned char func = i.key()>>8;
            // ui->chartView->getLine(anotc_frame_defination_list.value(func)->params.at(seq)->name)->addData(12,11);
            // qDebug("%f", ui->chartView->getLine(anotc_frame_defination_list.value(func)->params.at(seq)->name)->data()->keyRange());
        }
    }
}

void DataChartForm::hideAllLine()
{
    for (auto i = frame_hash.cbegin(), end = frame_hash.cend(); i != end; ++i) {
        unsigned char seq = (unsigned char)i.key();
        unsigned char func = i.key()>>8;
        ChartListItem *tmp = i.value();
        tmp->setChecked(false);
        ui->chartView->showHideLine(anotc_frame_defination_list.value(func)->params.at(seq)->name, false);
    }
}
