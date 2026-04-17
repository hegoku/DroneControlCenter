#include "datachartform.h"
#include "ui_datachartform.h"
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QStringList>
#include <QTextStream>
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
    connect(ui->wheelZoom_toolButton, &QToolButton::clicked, this, &DataChartForm::toggleWheelZoom);

    connect(ui->chartView, &DataAnalysicsChart::tracerEnableChanged, this, &DataChartForm::tracerChanaged);
    connect(ui->chartView, &DataAnalysicsChart::autoScrollChanged, this, &DataChartForm::autoScrollChanaged);
    connect(ui->chartView, &DataAnalysicsChart::rangeDragChanged, this, &DataChartForm::dragChanaged);
    connect(ui->chartView, &DataAnalysicsChart::zoomStatusChanged, this, &DataChartForm::zoomChanged);
    connect(ui->chartView, &DataAnalysicsChart::wheelZoomStatusChanged, this, &DataChartForm::wheelZoomChanged);

    ui->listWidget->setStyleSheet("QListView::item:selected {background-color:white;}");

    t=0;
    this->toggleAutoScroll(true);
    this->toggleWheelZoom(true);

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
        removeSavedMeasureData(id);
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
    const bool merge_with_last_row = !plot_rows.isEmpty() && plot_rows.last().timestamp == item.timestamp;
    const unsigned int sample_index = merge_with_last_row && t>0 ? t-1 : t;
    SavedPlotRow plot_row;
    plot_row.timestamp = item.timestamp;
    unsigned short id = (unsigned short)item.func<<8;
    for (int i=0;i<item.frame_value.size();i++) {
        if (frame_hash.contains(id|i)) {
            has_data = true;
            ChartListItem *tmp = frame_hash.value(id|i);
            const struct anotc_value &value = item.frame_value.at(i);
            const QString display_value = frameValueToDisplayString(value);
            const QString export_value = frameValueToExportString(value);
            ui->chartView->addPoint(value.name, sample_index, frameValueToNumber(value));
            tmp->setValue(display_value);
            plot_row.values.insert(id|i, export_value);
        }
    }
    if (has_data) {
        if (merge_with_last_row) {
            SavedPlotRow &last_row = plot_rows.last();
            for (auto value_it = plot_row.values.cbegin(), value_end = plot_row.values.cend(); value_it != value_end; ++value_it) {
                last_row.values.insert(value_it.key(), value_it.value());
            }
        } else {
            plot_rows.append(plot_row);
            t++;
        }
    }
}

void DataChartForm::changeScroll(int value)
{
    Q_UNUSED(value);
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
    plot_rows.clear();
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
    QList<unsigned short> measure_ids;
    QStringList header;

    for (int i=0;i<ui->listWidget->count();i++) {
        ChartListItem *item = qobject_cast<ChartListItem*>(ui->listWidget->itemWidget(ui->listWidget->item(i)));
        if (item==nullptr) continue;
        measure_ids.append(item->getId());
        header.append(csvEscape(item->getText()));
    }

    if (measure_ids.isEmpty()) {
        QMessageBox::information(this, tr("Save Plot Data"), tr("No measures are selected in the chart list."));
        return;
    }

    QString file_name = QFileDialog::getSaveFileName(
                this,
                tr("Save Plot Data"),
                QDir::home().filePath(QString("plot_data_%1.csv").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss_zzz"))),
                tr("CSV Files (*.csv);;All Files (*)"));
    if (file_name.isEmpty()) {
        return;
    }

    if (QFileInfo(file_name).suffix().isEmpty()) {
        file_name.append(".csv");
    }

    QFile file(file_name);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Save Plot Data"), tr("Failed to open %1 for writing.").arg(file_name));
        return;
    }

    QTextStream stream(&file);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    stream.setCodec("UTF-8");
#endif

    stream << "timestamp";
    for (const QString &column_name : header) {
        stream << "," << column_name;
    }
    stream << "\n";

    QHash<unsigned short, QString> last_values;
    for (const SavedPlotRow &row : plot_rows) {
        for (auto value_it = row.values.cbegin(), value_end = row.values.cend(); value_it != value_end; ++value_it) {
            last_values.insert(value_it.key(), value_it.value());
        }

        stream << csvEscape(formatTimestamp(row.timestamp));
        for (unsigned short id : measure_ids) {
            stream << "," << csvEscape(last_values.value(id));
        }
        stream << "\n";
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

void DataChartForm::toggleWheelZoom(bool checked)
{
    ui->chartView->setWheelZoom(checked);
}

void DataChartForm::wheelZoomChanged(bool checked)
{
    ui->wheelZoom_toolButton->setChecked(checked);
}

QString DataChartForm::frameValueToDisplayString(const struct anotc_value &value) const
{
    switch(value.type) {
    case 0:
        return QString::number(value.value.uint8);
    case 1:
        return QString::number(value.value.int8);
    case 2:
        return QString::number(value.value.uint16);
    case 3:
        return QString::number(value.value.int16);
    case 4:
        return QString::number(value.value.uint32);
    case 5:
        return QString::number(value.value.int32);
    case 6:
        return QString::number(value.value.uint64);
    case 7:
        return QString::number(value.value.int64);
    case 8:
        return QString::number(value.value.f);
    case 9:
        return QString::number(value.value.d);
    default:
        return QString();
    }
}

QString DataChartForm::frameValueToExportString(const struct anotc_value &value) const
{
    switch(value.type) {
    case 8:
        return QString::number(value.value.f, 'g', 9);
    case 9:
        return QString::number(value.value.d, 'g', 17);
    default:
        return frameValueToDisplayString(value);
    }
}

double DataChartForm::frameValueToNumber(const struct anotc_value &value) const
{
    switch(value.type) {
    case 0:
        return value.value.uint8;
    case 1:
        return value.value.int8;
    case 2:
        return value.value.uint16;
    case 3:
        return value.value.int16;
    case 4:
        return value.value.uint32;
    case 5:
        return value.value.int32;
    case 6:
        return value.value.uint64;
    case 7:
        return value.value.int64;
    case 8:
        return value.value.f;
    case 9:
        return value.value.d;
    default:
        return 0.0;
    }
}

QString DataChartForm::formatTimestamp(qint64 timestamp) const
{
    return QDateTime::fromMSecsSinceEpoch(timestamp).toString("yyyy-MM-dd HH:mm:ss.zzz");
}

QString DataChartForm::csvEscape(const QString &value) const
{
    QString escaped = value;
    escaped.replace("\"", "\"\"");
    return QString("\"%1\"").arg(escaped);
}

void DataChartForm::removeSavedMeasureData(unsigned short id)
{
    for (SavedPlotRow &row : plot_rows) {
        row.values.remove(id);
    }
}
