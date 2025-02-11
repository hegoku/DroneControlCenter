#include "calibrationform.h"
#include "ui_calibrationform.h"
#include "Anotc/anotc_cmd_frame.h"
#include "DLog.h"

enum flight_status {
    FLIGHT_STATUS_READY,
    FLIGHT_STATUS_ANGLE_RATE_MODE,
    FLIGHT_STATUS_ANGLE_MODE,
    FLIGHT_STATUS_CALIBRATION_ACCEL,
    FLIGHT_STATUS_CALIBRATION_GYRO,
    FLIGHT_STATUS_CALIBRATION_COMPASS,
    FLIGHT_STATUS_MOTOR_TEST
};

CalibrationForm::CalibrationForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CalibrationForm)
{
    ui->setupUi(this);

    connect(ui->gyro_btn, &QPushButton::clicked, this, &CalibrationForm::calibrate_gyro);
}

CalibrationForm::~CalibrationForm()
{
    delete ui;
}

void CalibrationForm::calibrate_gyro()
{
    anotc_send_cmd_calibrate_gyro();
}

void CalibrationForm::receiveCheckFrame(struct anotc_parsed_check_frame item)
{
    if (item.func==ANOTC_FRAME_CMD_SEND) {
        if (item.code) {
            DLogE(item.msg);
        }
    }
}

void CalibrationForm::updateData(struct anotc_parsed_data_frame item)
{
    QString value_string;
    if (item.func==ANOTC_FRAME_CUSTOM_SYSTEM_INFO) {
        unsigned char status = item.frame_value.at(0).value.uint8;
        if (status == FLIGHT_STATUS_READY) {
            ui->gyro_btn->setText("Calibrate Gyro");
            ui->gyro_btn->setEnabled(true);
        } else if (status & FLIGHT_STATUS_CALIBRATION_GYRO) {
            ui->gyro_btn->setEnabled(false);
        } else {
            ui->gyro_btn->setEnabled(false);
        }
    }
}

void CalibrationForm::cmdResponse(struct anotc_blocking_queue_item item)
{
    unsigned int cid = ((unsigned int)item.frame.frame.data[0])<<16 | ((unsigned int)item.frame.frame.data[1])<<8 | item.frame.frame.data[2];
    unsigned char code = item.frame.frame.data[3];
    if (cid==ANOTC_CMD_CALIBRATE_GYRO) {
        unsigned char percentage = item.frame.frame.data[4];
        ui->gyro_btn->setText(QString("Calibrating gyro %1%...").arg(percentage));
        if (percentage==100) {
            DLogN("Gyro has been Calibrated");
        }
    }
}
