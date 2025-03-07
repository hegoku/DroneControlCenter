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
    connect(ui->start_btn, &QPushButton::clicked, this, &DataChartForm::start);

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
            frame_hash.insert(id, 1);
            QListWidgetItem* pItem =new QListWidgetItem();
            ChartListItem* widget = new ChartListItem();
            widget->setText(anotc_frame_defination_list.value(func)->params.at(seq)->name);
            widget->setValue("0.00");
            widget->setTextColor(QColor(c.mRed, c.mGreen, c.mBlue));
            widget->setId(id);
            pItem->setSizeHint(widget->sizeHint());
            ui->listWidget->addItem(pItem);
            ui->listWidget->setItemWidget(pItem, widget);
            connect(widget, &ChartListItem::onRemove, this, &DataChartForm::removeLine);
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
    unsigned short id = (unsigned short)item.func<<8;
    for (int i=0;i<item.frame_value.size();i++) {
        if (frame_hash.contains(id|i)) {
            switch(item.frame_value.at(i).type) {
            case 0:
                ui->chartView->addPoint(item.frame_value.at(i).name, t, item.frame_value.at(i).value.uint8);
                break;
            case 1:
                ui->chartView->addPoint(item.frame_value.at(i).name, t, item.frame_value.at(i).value.int8);
                break;
            case 2:
                ui->chartView->addPoint(item.frame_value.at(i).name, t, item.frame_value.at(i).value.uint16);
                break;
            case 3:
                ui->chartView->addPoint(item.frame_value.at(i).name, t, item.frame_value.at(i).value.int16);
                break;
            case 4:
                ui->chartView->addPoint(item.frame_value.at(i).name, t, item.frame_value.at(i).value.uint32);
                break;
            case 5:
                ui->chartView->addPoint(item.frame_value.at(i).name, t, item.frame_value.at(i).value.int32);
                break;
            case 6:
                ui->chartView->addPoint(item.frame_value.at(i).name, t, item.frame_value.at(i).value.uint64);
                break;
            case 7:
                ui->chartView->addPoint(item.frame_value.at(i).name, t, item.frame_value.at(i).value.int64);
                break;
            case 8:
                ui->chartView->addPoint(item.frame_value.at(i).name, t, item.frame_value.at(i).value.f);
                break;
            case 9:
                ui->chartView->addPoint(item.frame_value.at(i).name, t, item.frame_value.at(i).value.d);
                break;
            }
        }
    }
    t++;
}

void DataChartForm::changeScroll(int value)
{
    // unsigned int a = value*ui->chartView->getXSize()/ui->horizontalScrollBar->maximum();
    // ui->chartView->setXAxisRange(a-ui->chartView->max_x_range, a);
}

void DataChartForm::start()
{
    if (ui->start_btn->text().compare("Start")==0) {
        is_start = true;
        ui->start_btn->setText("Stop");
    } else {
        is_start = false;
        ui->start_btn->setText("Start");
    }
}

