#include "calibrationform.h"
#include "ui_calibrationform.h"
#include "Anotc/anotc_cmd_frame.h"
#include "DLog.h"
#include "flight.h"

CalibrationForm::CalibrationForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CalibrationForm)
{
    ui->setupUi(this);

    connect(ui->gyro_btn, &QPushButton::clicked, this, &CalibrationForm::calibrate_gyro);

    connect(ui->calibrate_accel, &QPushButton::clicked, this, &CalibrationForm::calibrate_accel);
    connect(ui->calibrate_accel_up, &QPushButton::clicked, this, &CalibrationForm::calibrate_accel_up);
    connect(ui->calibrate_accel_down, &QPushButton::clicked, this, &CalibrationForm::calibrate_accel_down);
    connect(ui->calibrate_accel_forward, &QPushButton::clicked, this, &CalibrationForm::calibrate_accel_forward);
    connect(ui->calibrate_accel_backward, &QPushButton::clicked, this, &CalibrationForm::calibrate_accel_backward);
    connect(ui->calibrate_accel_left, &QPushButton::clicked, this, &CalibrationForm::calibrate_accel_left);
    connect(ui->calibrate_accel_right, &QPushButton::clicked, this, &CalibrationForm::calibrate_accel_right);
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
            ui->calibrate_accel->setEnabled(true);
            ui->calibrate_accel_up->setEnabled(false);
            ui->calibrate_accel_down->setEnabled(false);
            ui->calibrate_accel_forward->setEnabled(false);
            ui->calibrate_accel_backward->setEnabled(false);
            ui->calibrate_accel_left->setEnabled(false);
            ui->calibrate_accel_right->setEnabled(false);
        } else if (status == FLIGHT_STATUS_CALIBRATION_ACCEL) {
            ui->calibrate_accel->setEnabled(false);
            ui->calibrate_accel_up->setEnabled(true);
            ui->calibrate_accel_down->setEnabled(true);
            ui->calibrate_accel_forward->setEnabled(true);
            ui->calibrate_accel_backward->setEnabled(true);
            ui->calibrate_accel_left->setEnabled(true);
            ui->calibrate_accel_right->setEnabled(true);
        } else {
            ui->gyro_btn->setEnabled(false);
            ui->calibrate_accel->setEnabled(false);
            ui->calibrate_accel_up->setEnabled(false);
            ui->calibrate_accel_down->setEnabled(false);
            ui->calibrate_accel_forward->setEnabled(false);
            ui->calibrate_accel_backward->setEnabled(false);
            ui->calibrate_accel_left->setEnabled(false);
            ui->calibrate_accel_right->setEnabled(false);
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
            DLogN("Gyro has been calibrated");
        }
    } else if (cid==ANOTC_CMD_CALIBRATE_ACCEL) {
        char direction = item.frame.frame.data[4];
        unsigned char percentage = item.frame.frame.data[5];
        switch (direction) {
        case 0:
            DLogN("Accelerator has been calibreated");
            break;
        case 'U':
            ui->progressBar_U->setValue(percentage);
            if (percentage==100) {
                DLogN("Upward data has been collected");
            }
            break;
        case 'D':
            ui->progressBar_D->setValue(percentage);
            if (percentage==100) {
                DLogN("Downward data has been collected");
            }
            break;
        case 'F':
            ui->progressBar_F->setValue(percentage);
            if (percentage==100) {
                DLogN("Forward data has been collected");
            }
            break;
        case 'B':
            ui->progressBar_B->setValue(percentage);
            if (percentage==100) {
                DLogN("Backward data has been collected");
            }
            break;
        case 'L':
            ui->progressBar_L->setValue(percentage);
            if (percentage==100) {
                DLogN("Left data has been collected");
            }
            break;
        case 'R':
            ui->progressBar_R->setValue(percentage);
            if (percentage==100) {
                DLogN("Right data has been collected");
            }
            break;
        default:
            break;
        }
    }
}

void CalibrationForm::calibrate_accel()
{
    anotc_send_cmd_calibrate_accel(0);
}

void CalibrationForm::calibrate_accel_up()
{
    anotc_send_cmd_calibrate_accel('U');
}

void CalibrationForm::calibrate_accel_down()
{
    anotc_send_cmd_calibrate_accel('D');
}

void CalibrationForm::calibrate_accel_forward()
{
    anotc_send_cmd_calibrate_accel('F');
}

void CalibrationForm::calibrate_accel_backward()
{
    anotc_send_cmd_calibrate_accel('B');
}

void CalibrationForm::calibrate_accel_left()
{
    anotc_send_cmd_calibrate_accel('L');
}

void CalibrationForm::calibrate_accel_right()
{
    anotc_send_cmd_calibrate_accel('R');
}
