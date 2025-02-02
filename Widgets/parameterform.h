#ifndef PARAMETERFORM_H
#define PARAMETERFORM_H

#include <QWidget>
#include "Anotc/anotc_config_frame.h"
#include <QMap>
#include <QStandardItemModel>

struct anotc_parameter_defination
{
    unsigned short par_id;
    unsigned char par_type;
    QString par_name;
    QString par_info;
};

namespace Ui {
class ParameterForm;
}

class ParameterForm : public QWidget
{
    Q_OBJECT

public:
    explicit ParameterForm(QWidget *parent = nullptr);
    ~ParameterForm();

public slots:
    void readParameter();
    void updateData(struct anotc_parsed_parameter_frame);

private:
    Ui::ParameterForm *ui;
    unsigned short param_count;
    unsigned short read_param_count;
    QMap<unsigned short, struct anotc_parameter_defination*> anotc_parameter_defination_list;
    QStandardItemModel *model;
    QMap<unsigned char, QString> par_type_map;

    void insertParam(struct anotc_parameter_defination*);
};

#endif // PARAMETERFORM_H
