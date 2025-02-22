#include "controlwindow.h"
#include "ui_controlwindow.h"
#include "Anotc/anotc_cmd_frame.h"

ControlWindow::ControlWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ControlWindow)
{
    ui->setupUi(this);

    connect(ui->open_check, &QCheckBox::clicked, this, &ControlWindow::open);

    connect(ui->throttle_slider, &QSlider::valueChanged, this, &ControlWindow::onChannelChanged);
    connect(ui->roll_slider, &QSlider::valueChanged, this, &ControlWindow::onChannelChanged);
    connect(ui->pitch_slider, &QSlider::valueChanged, this, &ControlWindow::onChannelChanged);
    connect(ui->yaw_slider, &QSlider::valueChanged, this, &ControlWindow::onChannelChanged);
    connect(ui->aux1_slider, &QSlider::valueChanged, this, &ControlWindow::onChannelChanged);
    connect(ui->aux2_slider, &QSlider::valueChanged, this, &ControlWindow::onChannelChanged);

    timer = new QTimer();
    timer->setInterval(10);
    connect(timer, &QTimer::timeout, this, &ControlWindow::send);
}

ControlWindow::~ControlWindow()
{
    delete ui;
}

void ControlWindow::open(bool checked)
{
    if (checked) {
        timer->start();
    } else {
        timer->stop();
    }
}

void ControlWindow::onChannelChanged(int value)
{
    ui->throttle_value->setText(QString("%1").arg(ui->throttle_slider->value()));
    ui->roll_value->setText(QString("%1").arg(ui->roll_slider->value()));
    ui->pitch_value->setText(QString("%1").arg(ui->pitch_slider->value()));
    ui->yaw_value->setText(QString("%1").arg(ui->yaw_slider->value()));
    ui->aux1_value->setText(QString("%1").arg(ui->aux1_slider->value()));
    ui->aux2_value->setText(QString("%1").arg(ui->aux2_slider->value()));
}

void ControlWindow::send()
{
    unsigned short channel[6];
    channel[0] = ui->roll_slider->value();
    channel[1] = ui->pitch_slider->value();
    channel[2] = ui->throttle_slider->value();
    channel[3] = ui->yaw_slider->value();
    channel[4] = ui->aux1_slider->value();
    channel[5] = ui->aux2_slider->value();
    anotc_send_cmd_pc_rc(channel, 6);
}
