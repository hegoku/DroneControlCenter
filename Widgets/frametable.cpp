#include "frametable.h"
#include "ui_frametable.h"
#include "Anotc/anotc_json.h"

FrameTable::FrameTable(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FrameTable)
{
    ui->setupUi(this);

    timer = new QTimer();
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(calculateFreq()));

    model = new QStandardItemModel();
    ui->treeView->setModel(model);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->setExpandsOnDoubleClick(false);

    // ui->tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    // ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    // ui->tableView->horizontalHeader()->setStretchLastSection(true);
    // ui->tableView->horizontalHeader()->setSectionsClickable(true);

    connect(ui->treeView, &QTreeView::clicked, this, &FrameTable::selectionChanged);

    model->setColumnCount(5);
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Name");
    model->setHeaderData(2, Qt::Horizontal, "Value");
    model->setHeaderData(3, Qt::Horizontal, "Type");
    model->setHeaderData(4, Qt::Horizontal, "Description");

    timer->start();
}

FrameTable::~FrameTable()
{
    timer->stop();
    delete ui;
}

void FrameTable::loadTable()
{
    int row = 0;
    for (QMap<unsigned char, struct anotc_frame_defination*>::const_iterator i = anotc_frame_defination_list.constBegin();i!=anotc_frame_defination_list.constEnd();i++) {
        QStandardItem *item_id = new QStandardItem(QString("0x%1").arg(i.key(), 2, 16, QLatin1Char( '0' )));
        item_id->setEditable(false);
        model->setItem(row, 0, item_id);

        QStandardItem *item_name = new QStandardItem(i.value()->name);
        item_name->setEditable(false);
        model->setItem(row, 1, item_name);

        QStandardItem *item_value = new QStandardItem("");
        item_value->setEditable(false);
        model->setItem(row, 2, item_value);

        QStandardItem *item_type = new QStandardItem("");
        item_type->setEditable(false);
        model->setItem(row, 3, item_type);

        QStandardItem *item_desc = new QStandardItem(i.value()->desc);
        item_desc->setEditable(false);
        model->setItem(row, 4, item_desc);

        parameter_item_mapping.insert(i.key(), row);

        row++;

        int child_row = 0;
        for (int j=0;j<i.value()->params.size();j++) {
            struct anotc_frame_param *param = i.value()->params.at(j);
            QStandardItem *id_child = new QStandardItem("");
            id_child->setEditable(false);
            item_id->setChild(child_row,0,id_child);

            QStandardItem *name_child = new QStandardItem(param->name);
            name_child->setEditable(false);
            item_id->setChild(child_row,1,name_child);
            // model->setItem(row, 1, item_name);

            QStandardItem * value_child = new QStandardItem("");
            value_child->setEditable(false);
            item_id->setChild(child_row,2,value_child);

            item_type = new QStandardItem(param->type);
            item_type->setEditable(false);
            item_id->setChild(child_row,3,item_type);

            item_desc = new QStandardItem(param->desc);
            item_desc->setEditable(false);
            item_id->setChild(child_row,4,item_desc);

            child_row++;
        }
    }
}

void FrameTable::selectionChanged(const QModelIndex &index)
{
    ui->treeView->isExpanded(index)? ui->treeView->collapse(index) : ui->treeView->expand(index);
}

void FrameTable::updateData(union _un_anotc_v8_frame *frame)
{
    QList<struct anotc_value> *frame_value = new QList<struct anotc_value>();
    QString value_string;
    if (anotc_parse_data_frame(frame, frame_value)==0) {
        if (parameter_counter.contains(frame->frame.fun)) {
            parameter_counter.insert(frame->frame.fun, parameter_counter.value(frame->frame.fun)+1);
        } else {
            parameter_counter.insert(frame->frame.fun, 0);
        }
        for (int i=0;i<frame_value->size();i++) {
            if (frame_value->at(i).type==0) {
                value_string = QString::number(frame_value->at(i).value.uint8);
            } else if (frame_value->at(i).type==1) {
                value_string = QString::number(frame_value->at(i).value.int8);
            } else if (frame_value->at(i).type==2) {
                value_string = QString::number(frame_value->at(i).value.uint16);
            } else if (frame_value->at(i).type==3) {
                value_string = QString::number(frame_value->at(i).value.int16);
            } else if (frame_value->at(i).type==4) {
                value_string = QString::number(frame_value->at(i).value.uint32);
            } else if (frame_value->at(i).type==5) {
                value_string = QString::number(frame_value->at(i).value.int32);
            } else if (frame_value->at(i).type==8) {
                value_string = QString::number(frame_value->at(i).value.f);
            }
            model->item(parameter_item_mapping.value(frame->frame.fun), 0)->child(i, 2)->setText(value_string);
        }
    }
    delete frame_value;
}

void FrameTable::calculateFreq()
{
    unsigned long int current_count = 0;
    unsigned int freq = 0;
    for (int i=0;i<parameter_counter.keys().size();i++) {
        if (parameter_freq.contains(parameter_counter.keys().at(i))) {
            current_count = parameter_counter.value(parameter_counter.keys().at(i));
            freq = current_count - parameter_freq.value(parameter_counter.keys().at(i));
            model->item(parameter_item_mapping.value(parameter_counter.keys().at(i)), 2)->setText(QString::number(freq));
        }
        parameter_freq.insert(parameter_counter.keys().at(i), current_count);
    }
}
