#ifndef FRAMETABLE_H
#define FRAMETABLE_H

#include <QWidget>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QHash>
#include <Anotc/anotc_json.h>
#include <QTimer>

namespace Ui {
class FrameTable;
}

class FrameTable : public QWidget
{
    Q_OBJECT

public:
    explicit FrameTable(QWidget *parent = nullptr);
    ~FrameTable();
    void loadTable();

public slots:
    void selectionChanged(const QModelIndex &index);
    void updateData(unsigned char func, QList<anotc_value> value);
    void calculateFreq();

private:
    Ui::FrameTable *ui;

    QStandardItemModel *model;

    QHash<unsigned char, int> parameter_item_mapping;
    QHash<unsigned char, unsigned long int> parameter_counter;
    QHash<unsigned char, unsigned long int> parameter_freq;

    QTimer *timer;
};

#endif // FRAMETABLE_H
