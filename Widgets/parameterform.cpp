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
    connect(ui->sendBtn, &QPushButton::clicked, this, &ParameterForm::sendParameter);
    connect(ui->saveBtn, &QPushButton::clicked, this, &ParameterForm::saveParameter);

    model = new QStandardItemModel();
    ui->tableView->setModel(model);

    ui->tableView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->hide();
    connect(model, &QStandardItemModel::itemChanged, this, &ParameterForm::itemChanged);
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
    model->removeRows(0, model->rowCount());
    anotc_send_config_get_count();
}

void ParameterForm::updateData(struct anotc_parsed_parameter_frame item)
{
    if (item.func==ANOTC_FRAME_CONFIG_CMD) {
        if (item.frame_value.value(0).value.uint8==ANOTC_CONFIG_FRAME_CMD_READ_COUNT) {
            param_count = item.frame_value.value(1).value.uint16;
            DLogI(QString("Parameter count:%1").arg(param_count));
            DLogN(QString("Start to read parameters' info ..."));
            if (param_count>0) {
                // QMetaObject::invokeMethod(this, [=]{
                    anotc_send_config_get_param_info(0);
                // }, Qt::QueuedConnection);

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
        if (param_count>d->par_id+1) {
            // QMetaObject::invokeMethod(this, [=]{
                anotc_send_config_get_param_info(d->par_id+1);
            // }, Qt::QueuedConnection);
        } else {
            DLogN(QString("Start to read parameters' value ..."));
            anotc_send_config_get_param_value(0);
        }
        changed_par.clear();
    } else if (item.func==ANOTC_FRAME_CONFIG_READ_WRITE) {
        unsigned short par_id = item.frame_value.value(0).value.uint16;
        unsigned char *tmp = (unsigned char*)malloc(sizeof(unsigned char)*item.frame_value.value(1).string.length());
        for (int i=0;i<item.frame_value.value(1).string.length();i++) {
            tmp[i] = (unsigned char)item.frame_value.value(1).string.toLatin1().data()[i];
        }
        switch (anotc_parameter_defination_list.value(par_id)->par_type) {
        case 0:
            model->item(par_id, 1)->setText(QString::number(tmp[0]));
            break;
        case 1:
            model->item(par_id, 1)->setText(QString::number(tmp[0]));
            break;
        case 2:
            model->item(par_id, 1)->setText(QString::number(tmp[0] | (((unsigned short)tmp[1])<<8)));
            break;
        case 3:
            model->item(par_id, 1)->setText(QString::number(tmp[0] | (((short)tmp[1])<<8)));
            break;
        case 4:
            model->item(par_id, 1)->setText(QString::number(tmp[0] | (((unsigned int)tmp[1])<<8) | (((unsigned int)tmp[2])<<16) | (((unsigned int)tmp[3])<<24) ));
            break;
        case 5:
            model->item(par_id, 1)->setText(QString::number(tmp[0] | (((int)tmp[1])<<8) | (((int)tmp[2])<<16) | (((int)tmp[3])<<24) ));
            break;
        case 8:{
            unsigned int tmp1 = (unsigned int)tmp[0] | (((unsigned int)tmp[1])<<8) | (((unsigned int)tmp[2])<<16) | (((unsigned int)tmp[3])<<24);

            model->item(par_id, 1)->setText(QString::number(*((float*)&tmp1)));
            break;
            }
        case 10:
                model->item(par_id, 1)->setText(item.frame_value.value(1).string);
            break;
        }
        model->item(par_id, 1)->setEditable(true);

        if (param_count>par_id+1) {
            anotc_send_config_get_param_value(par_id+1);
        } else {
            DLogN(QString("Read parameters' value done"));
        }
        changed_par.clear();
    }
}

void ParameterForm::receiveCheckFrame(struct anotc_blocking_queue_item item)
{
    if (item.frame.frame.fun==ANOTC_FRAME_FRAME_CHECK) {
        if (item.frame.frame.data[0]==ANOTC_FRAME_CONFIG_READ_WRITE) {
            changed_par.remove(changed_par.firstKey());
            if (changed_par.isEmpty()==false) {
                formatAndSendParam(changed_par.firstKey());
            } else {
                DLogN(QString("Start to save parameters' value ..."));
                anotc_send_config_save_param();
            }
        } else if (item.frame.frame.data[0]==ANOTC_FRAME_CONFIG_CMD) {
            DLogN(QString("Save parameters' value done<br/>start to refresh parameters ..."));
            anotc_send_config_get_count();
        }

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

void ParameterForm::itemChanged(QStandardItem *item)
{
    if (item->column()!=1) return;
    changed_par.insert(item->row(), 1);
}

void ParameterForm::formatAndSendParam(unsigned short row)
{
    switch (anotc_parameter_defination_list.value(row)->par_type) {
        case 0:
        {
            unsigned char tmp = (unsigned char)model->item(row, 1)->text().toInt();
            anotc_send_config_set_param(row, anotc_parameter_defination_list.value(row)->par_type, &tmp);
            break;
        }
        case 1:
        {
            char tmp = (char)model->item(row, 1)->text().toInt();
            anotc_send_config_set_param(row, anotc_parameter_defination_list.value(row)->par_type, (unsigned char*)&tmp);
            break;
        }
        case 2:
        {
            unsigned short tmp = model->item(row, 1)->text().toUShort();
            anotc_send_config_set_param(row, anotc_parameter_defination_list.value(row)->par_type, (unsigned char*)&tmp);
            break;
        }
        case 3:
        {
            short tmp = model->item(row, 1)->text().toShort();
            anotc_send_config_set_param(row, anotc_parameter_defination_list.value(row)->par_type, (unsigned char*)&tmp);
            break;
        }
        case 4:
        {
            unsigned int tmp = model->item(row, 1)->text().toUInt();
            anotc_send_config_set_param(row, anotc_parameter_defination_list.value(row)->par_type, (unsigned char*)&tmp);
            break;
        }
        case 5:
        {
            int tmp = model->item(row, 1)->text().toInt();
            anotc_send_config_set_param(row, anotc_parameter_defination_list.value(row)->par_type, (unsigned char*)&tmp);
            break;
        }
        case 6:
        {
            uint64_t tmp = model->item(row, 1)->text().toULongLong();
            anotc_send_config_set_param(row, anotc_parameter_defination_list.value(row)->par_type, (unsigned char*)&tmp);
            break;
        }
        case 7:
        {
            int64_t tmp = model->item(row, 1)->text().toLongLong();
            anotc_send_config_set_param(row, anotc_parameter_defination_list.value(row)->par_type, (unsigned char*)&tmp);
            break;
        }
        case 8:
        {
            float tmp = model->item(row, 1)->text().toFloat();
            anotc_send_config_set_param(row, anotc_parameter_defination_list.value(row)->par_type, (unsigned char*)&tmp);
            break;
        }
        case 9:
        {
            double tmp = model->item(row, 1)->text().toDouble();
            anotc_send_config_set_param(row, anotc_parameter_defination_list.value(row)->par_type, (unsigned char*)&tmp);
            break;
        }
        case 10:
        {
            QByteArray byteData = model->item(row, 1)->text().toLocal8Bit();
            char *tmp = byteData.data();
            anotc_send_config_set_param(row, anotc_parameter_defination_list.value(row)->par_type, (unsigned char*)tmp);
            break;
        }
    }
}

void ParameterForm::sendParameter()
{
    if (changed_par.isEmpty()) return;
    DLogN(QString("Start to send parameters' value ..."));
    formatAndSendParam(changed_par.firstKey());
}

void ParameterForm::saveParameter()
{
    anotc_send_config_save_param();
}
