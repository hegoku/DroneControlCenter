#ifndef DATATABLE_H
#define DATATABLE_H

#include <QWidget>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QHash>
#include <Anotc/anotc_data_frame.h>
#include <QTimer>

namespace Ui {
class DataTable;
}

class DataTable : public QWidget
{
    Q_OBJECT

public:
    explicit DataTable(QWidget *parent = nullptr);
    ~DataTable();
    void loadTable();

public slots:
    void selectionChanged(const QModelIndex &index);
    void updateData(struct anotc_parsed_data_frame);
    void calculateFreq();

private:
    Ui::DataTable *ui;

    QStandardItemModel *model;

    QHash<unsigned char, int> parameter_item_mapping;
    QHash<unsigned char, unsigned long int> parameter_counter;
    QHash<unsigned char, unsigned long int> parameter_freq;

    QTimer *timer;
};

#endif // DATATABLE_H
