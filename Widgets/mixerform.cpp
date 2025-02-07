#include "mixerform.h"
#include "ui_mixerform.h"

MixerForm::MixerForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MixerForm)
{
    ui->setupUi(this);

    ui->motor_1_slider->setEnabled(false);
    ui->motor_2_slider->setEnabled(false);
    ui->motor_3_slider->setEnabled(false);
    ui->motor_4_slider->setEnabled(false);
    ui->motor_all_slider->setEnabled(false);

    connect(ui->enableMotorTestMode, &QCheckBox::checkStateChanged, this, &MixerForm::onMotorTestModeChange);
    connect(ui->enableMotorTestMode, &QCheckBox::toggled, this, &MixerForm::changeMotorTestMode);

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

void MixerForm::onMotorTestModeChange(Qt::CheckState status)
{
    if (status==Qt::CheckState::Checked) {
        ui->motor_1_slider->setEnabled(true);
        ui->motor_2_slider->setEnabled(true);
        ui->motor_3_slider->setEnabled(true);
        ui->motor_4_slider->setEnabled(true);
        ui->motor_all_slider->setEnabled(true);
    } else {
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
    }
}

void MixerForm::onFlightUpdate(struct anotc_blocking_queue_item item)
{
    if (item.frame.frame.fun==ANOTC_FRAME_CUSTOM_SYSTEM_INFO) {

    } else if (item.frame.frame.fun==ANOTC_FRAME_CUSTOM_MIXER_INFO) {

    }
}

void MixerForm::changeMotorTestMode(bool checked)
{
    if (checked) {
    } else {

    }
}

void MixerForm::onMotor1SliderChange(int value)
{
    if (ui->motor_1_slider->isEnabled()) {
        qDebug("%d", value);
    }

}

void MixerForm::onMotor2SliderChange(int value)
{

}

void MixerForm::onMotor3SliderChange(int value)
{

}

void MixerForm::onMotor4SliderChange(int value)
{

}

void MixerForm::onMotorAllSliderChange(int value)
{

}
