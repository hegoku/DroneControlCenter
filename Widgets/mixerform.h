#ifndef MIXERFORM_H
#define MIXERFORM_H

#include <QWidget>
#include "Anotc/anotc.h"

namespace Ui {
class MixerForm;
}

class MixerForm : public QWidget
{
    Q_OBJECT

public:
    explicit MixerForm(QWidget *parent = nullptr);
    ~MixerForm();

private slots:
    void onMotorTestModeChange(Qt::CheckState);
    void onFlightUpdate(struct anotc_blocking_queue_item);
    void changeMotorTestMode(bool checked);
    void onMotor1SliderChange(int value);
    void onMotor2SliderChange(int value);
    void onMotor3SliderChange(int value);
    void onMotor4SliderChange(int value);
    void onMotorAllSliderChange(int value);

private:
    Ui::MixerForm *ui;
};

#endif // MIXERFORM_H
