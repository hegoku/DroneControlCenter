#include "parameterform.h"
#include "ui_parameterform.h"
#include "Anotc/anotc.h"
#include "Anotc/anotc_config_frame.h"
#include "DLog.h"

ParameterForm::ParameterForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ParameterForm)
{
    ui->setupUi(this);

    par_type_map.insert(0, QString("UInt8"));
    par_type_map.insert(1, QString("Int8"));
    par_type_map.insert(2, QString("UInt16"));
    par_type_map.insert(3, QString("Int16"));
    par_type_map.insert(4, QString("UInt32"));
    par_type_map.insert(5, QString("Int32"));
    par_type_map.insert(6, QString("UInt64"));
    par_type_map.insert(7, QString("Int64"));
    par_type_map.insert(8, QString("Float"));
    par_type_map.insert(9, QString("Double"));
    par_type_map.insert(10, QString("String"));

    connect(ui->readBtn, &QPushButton::clicked, this, &ParameterForm::readParameter);

    model = new QStandardItemModel();
    ui->tableView->setModel(model);

    ui->tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    // connect(ui->treeView, &QTreeView::clicked, this, &DataTable::selectionChanged);

    model->setColumnCount(4);
    model->setHeaderData(0, Qt::Horizontal, "Name");
    model->setHeaderData(1, Qt::Horizontal, "Value");
    model->setHeaderData(2, Qt::Horizontal, "Type");
    model->setHeaderData(3, Qt::Horizontal, "Info");
}

ParameterForm::~ParameterForm()
{
    delete ui;
}

void ParameterForm::readParameter()
{
    read_param_count = 0;
    QMap<unsigned short, struct anotc_parameter_defination*>::iterator itor;
    for (itor=anotc_parameter_defination_list.begin();itor!=anotc_parameter_defination_list.end();itor++) {
        free(itor.value());
    }
    anotc_parameter_defination_list.clear();
    model->clear();
    anotc_send_config_get_count();
}

void ParameterForm::updateData(struct anotc_parsed_parameter_frame item)
{
    if (item.func==ANOTC_FRAME_CONFIG_CMD) {
        if (item.frame_value.value(0).value.uint8==ANOTC_CONFIG_FRAME_CMD_READ_COUNT) {
            param_count = item.frame_value.value(1).value.uint16;
            DLogI(QString("Parameter count:%1").arg(param_count));
            if (param_count>0) {
                QMetaObject::invokeMethod(this, [=]{
                    anotc_send_config_get_param_info(0);
                }, Qt::QueuedConnection);

            }
        }
    } else if (item.func==ANOTC_FRAME_CONFIG_INFO) {
        struct anotc_parameter_defination *d = (struct anotc_parameter_defination*)malloc(sizeof(struct anotc_parameter_defination));
        d->par_id = item.frame_value.value(0).value.uint16;
        d->par_type = item.frame_value.value(1).value.uint8;
        d->par_name = item.frame_value.value(2).string;
        d->par_info = item.frame_value.value(3).string;
        anotc_parameter_defination_list.insert(d->par_id, d);
        insertParam(d);
        if (param_count>d->par_id) {
            QMetaObject::invokeMethod(this, [=]{
                anotc_send_config_get_param_info(d->par_id+1);
            }, Qt::QueuedConnection);
        } else {

        }
    } else if (item.func==ANOTC_FRAME_CONFIG_READ_WRITE) {

    }
}

void ParameterForm::insertParam(struct anotc_parameter_defination* param)
{
    QStandardItem *item_name = new QStandardItem(param->par_name);
    item_name->setEditable(false);
    model->setItem(param->par_id, 0, item_name);

    QStandardItem *item_value = new QStandardItem("ReadErr");
    item_value->setEditable(false);
    model->setItem(param->par_id, 1, item_value);

    QStandardItem *item_type = new QStandardItem(par_type_map.value(param->par_type));
    item_type->setEditable(false);
    model->setItem(param->par_id, 2, item_type);

    QStandardItem *item_desc = new QStandardItem(param->par_info);
    item_desc->setEditable(false);
    model->setItem(param->par_id, 3, item_desc);

}
