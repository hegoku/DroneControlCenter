#include "rcform.h"
#include "ui_rcform.h"
#include "Anotc/anotc_cmd_frame.h"
#include "flight.h"
#include "Anotc/anotc_config_frame.h"
#include "DLog.h"

RCForm::RCForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RCForm)
{
    ui->setupUi(this);

    ui->save_btn->setEnabled(false);
    ui->protocol_list->setEnabled(false);

    connect(ui->save_btn, &QPushButton::clicked, this, &RCForm::saveConfig);
    connect(ui->show_control_win, &QPushButton::clicked, this, &RCForm::showControlWin);
    control_win = new ControlWindow(this);
}

RCForm::~RCForm()
{
    delete ui;
}

void RCForm::showControlWin()
{
    if (control_win->isHidden()) {
        control_win->show();
    } else {
        control_win->hide();
    }
}

void RCForm::saveConfig()
{
    unsigned char protocol = ui->protocol_list->currentIndex();
    anotc_send_config_set_param(ANOTC_CONFIG_PAR_RC_PROTOCOL, 0, &protocol);
    anotc_send_cmd_reboot();
}

void RCForm::onConnect()
{
    ui->save_btn->setEnabled(true);
    ui->protocol_list->setEnabled(true);
    anotc_send_config_get_param_value(ANOTC_CONFIG_PAR_RC_PROTOCOL);
}

void RCForm::onDisconnect()
{
    ui->save_btn->setEnabled(false);
    ui->protocol_list->setEnabled(false);
}

void RCForm::paramUpdated(struct anotc_parsed_parameter_frame item)
{
    if (item.func==ANOTC_FRAME_CONFIG_READ_WRITE) {
        unsigned short par_id = item.frame_value.value(0).value.uint16;
        if (par_id==ANOTC_CONFIG_PAR_RC_PROTOCOL) {
            unsigned char protocol = (unsigned char)item.frame_value.value(1).string.toLatin1().data()[0];
            ui->protocol_list->setCurrentIndex(protocol);
        }
    }
}

void RCForm::onFlightUpdate(struct anotc_parsed_data_frame item)
{
    if (item.func==ANOTC_FRAME_CUSTOM_SYSTEM_INFO) {
        unsigned char status = item.frame_value.at(0).value.uint8;
        if (status != FLIGHT_STATUS_READY) {
            ui->save_btn->setEnabled(false);
            ui->protocol_list->setEnabled(false);
        } else {
            ui->save_btn->setEnabled(true);
            ui->protocol_list->setEnabled(true);
        }
    }
}
