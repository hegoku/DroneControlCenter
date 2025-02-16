#include "mixerform.h"
#include "Anotc/anotc_data_frame.h"
#include "ui_mixerform.h"
#include "Anotc/anotc_cmd_frame.h"
#include "flight.h"
#include "Anotc/anotc_config_frame.h"
#include "DLog.h"

MixerForm::MixerForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MixerForm)
{
    ui->setupUi(this);

    connect(ui->save_btn, &QPushButton::clicked, this, &MixerForm::saveConfig);

    ui->save_btn->setEnabled(false);
    ui->motor_1_slider->setEnabled(false);
    ui->motor_2_slider->setEnabled(false);
    ui->motor_3_slider->setEnabled(false);
    ui->motor_4_slider->setEnabled(false);
    ui->motor_all_slider->setEnabled(false);

    connect(ui->enableMotorTestMode, &QCheckBox::clicked, this, &MixerForm::changeMotorTestMode);

    connect(ui->motor_1_slider, &QSlider::valueChanged, this, &MixerForm::onMotor1SliderChange);
    connect(ui->motor_2_slider, &QSlider::valueChanged, this, &MixerForm::onMotor2SliderChange);
    connect(ui->motor_3_slider, &QSlider::valueChanged, this, &MixerForm::onMotor3SliderChange);
    connect(ui->motor_4_slider, &QSlider::valueChanged, this, &MixerForm::onMotor4SliderChange);
    connect(ui->motor_all_slider, &QSlider::valueChanged, this, &MixerForm::onMotorAllSliderChange);
}

MixerForm::~MixerForm()
{
    delete ui;
}

void MixerForm::onFlightUpdate(struct anotc_parsed_data_frame item)
{
    if (item.func==ANOTC_FRAME_CUSTOM_SYSTEM_INFO) {
        unsigned char status = item.frame_value.at(0).value.uint8;
        flight_status = (enum flight_status)status;
        if (status != FLIGHT_STATUS_MOTOR_TEST) {
            ui->enableMotorTestMode->setCheckState(Qt::Unchecked);
            ui->motor_1_slider->setEnabled(false);
            ui->motor_2_slider->setEnabled(false);
            ui->motor_3_slider->setEnabled(false);
            ui->motor_4_slider->setEnabled(false);
            ui->motor_all_slider->setEnabled(false);
            ui->motor_1_slider->setValue(1000);
            ui->motor_2_slider->setValue(1000);
            ui->motor_3_slider->setValue(1000);
            ui->motor_4_slider->setValue(1000);
            ui->motor_all_slider->setValue(1000);
        } else {
            ui->enableMotorTestMode->setCheckState(Qt::Checked);
            ui->motor_1_slider->setEnabled(true);
            ui->motor_2_slider->setEnabled(true);
            ui->motor_3_slider->setEnabled(true);
            ui->motor_4_slider->setEnabled(true);
            ui->motor_all_slider->setEnabled(true);
        }
    } else if (item.func==ANOTC_FRAME_PWM) {
        ui->motor_1_throttle->setValue(item.frame_value.at(0).value.uint16);
        ui->motor_2_throttle->setValue(item.frame_value.at(1).value.uint16);
        ui->motor_3_throttle->setValue(item.frame_value.at(2).value.uint16);
        ui->motor_4_throttle->setValue(item.frame_value.at(3).value.uint16);
    }
}

void MixerForm::changeMotorTestMode(bool checked)
{
    if (checked) {
        anotc_send_cmd_motor_test_status(1);
    } else {
        anotc_send_cmd_motor_test_status(0);
    }
}

void MixerForm::onMotor1SliderChange(int value)
{
    unsigned short throttle[4];
    throttle[0] = ui->motor_1_slider->value();
    throttle[1] = ui->motor_2_slider->value();
    throttle[2] = ui->motor_3_slider->value();
    throttle[3] = ui->motor_4_slider->value();
    if (ui->motor_1_slider->isEnabled() && flight_status==FLIGHT_STATUS_MOTOR_TEST) {
        anotc_send_cmd_motor_test_throttle(throttle, 4);
    }
    ui->motor_1_value->setText(QString("%1").arg(value));
}

void MixerForm::onMotor2SliderChange(int value)
{
    unsigned short throttle[4];
    throttle[0] = ui->motor_1_slider->value();
    throttle[1] = ui->motor_2_slider->value();
    throttle[2] = ui->motor_3_slider->value();
    throttle[3] = ui->motor_4_slider->value();
    if (ui->motor_2_slider->isEnabled() && flight_status==FLIGHT_STATUS_MOTOR_TEST) {
        anotc_send_cmd_motor_test_throttle(throttle, 4);
    }
    ui->motor_2_value->setText(QString("%1").arg(value));
}

void MixerForm::onMotor3SliderChange(int value)
{
    unsigned short throttle[4];
    throttle[0] = ui->motor_1_slider->value();
    throttle[1] = ui->motor_2_slider->value();
    throttle[2] = ui->motor_3_slider->value();
    throttle[3] = ui->motor_4_slider->value();
    if (ui->motor_3_slider->isEnabled() && flight_status==FLIGHT_STATUS_MOTOR_TEST) {
        anotc_send_cmd_motor_test_throttle(throttle, 4);
    }
    ui->motor_3_value->setText(QString("%1").arg(value));
}

void MixerForm::onMotor4SliderChange(int value)
{
    unsigned short throttle[4];
    throttle[0] = ui->motor_1_slider->value();
    throttle[1] = ui->motor_2_slider->value();
    throttle[2] = ui->motor_3_slider->value();
    throttle[3] = ui->motor_4_slider->value();
    if (ui->motor_4_slider->isEnabled() && flight_status==FLIGHT_STATUS_MOTOR_TEST) {
        anotc_send_cmd_motor_test_throttle(throttle, 4);
    }
    ui->motor_4_value->setText(QString("%1").arg(value));
}

void MixerForm::onMotorAllSliderChange(int value)
{
    unsigned short throttle[4];
    throttle[0] = ui->motor_all_slider->value();
    throttle[1] = ui->motor_all_slider->value();
    throttle[2] = ui->motor_all_slider->value();
    throttle[3] = ui->motor_all_slider->value();
    if (ui->motor_all_slider->isEnabled() && flight_status==FLIGHT_STATUS_MOTOR_TEST) {
        anotc_send_cmd_motor_test_throttle(throttle, 4);
    }
    ui->motor_all_value->setText(QString("%1").arg(value));
}

void MixerForm::saveConfig()
{
    unsigned char protocol = ui->esc_protocol_list->currentIndex();
    anotc_send_config_set_param(ANOTC_CONFIG_PAR_ESC_PROTOCOL, 0, &protocol);
    anotc_send_cmd_reboot();
}

void MixerForm::onConnect()
{
    ui->save_btn->setEnabled(true);
    ui->enableMotorTestMode->setEnabled(true);
    // anotc_send_config_get_param_value(ANOTC_CONFIG_PAR_ESC_PROTOCOL);
}

void MixerForm::onDisconnect()
{
    ui->save_btn->setEnabled(false);
    ui->enableMotorTestMode->setEnabled(false);
    ui->motor_1_slider->setEnabled(false);
    ui->motor_2_slider->setEnabled(false);
    ui->motor_3_slider->setEnabled(false);
    ui->motor_4_slider->setEnabled(false);
    ui->motor_all_slider->setEnabled(false);
}

void MixerForm::paramUpdated(struct anotc_parsed_parameter_frame item)
{
    if (item.func==ANOTC_FRAME_CONFIG_READ_WRITE) {
        unsigned short par_id = item.frame_value.value(0).value.uint16;
        if (par_id==ANOTC_CONFIG_PAR_ESC_PROTOCOL) {
            unsigned char protocol = (unsigned char)item.frame_value.value(1).string.toLatin1().data()[0];
            ui->esc_protocol_list->setCurrentIndex(protocol);
        }
    }
}

