#ifndef MIXERFORM_H
#define MIXERFORM_H

#include <QWidget>
#include "Anotc/anotc.h"
#include "Anotc/anotc_data_frame.h"
#include "flight.h"

namespace Ui {
class MixerForm;
}

class MixerForm : public QWidget
{
    Q_OBJECT

public:
    explicit MixerForm(QWidget *parent = nullptr);
    ~MixerForm();

public slots:
    void onFlightUpdate(struct anotc_parsed_data_frame);
    void changeMotorTestMode(bool checked);
    void onMotor1SliderChange(int value);
    void onMotor2SliderChange(int value);
    void onMotor3SliderChange(int value);
    void onMotor4SliderChange(int value);
    void onMotorAllSliderChange(int value);
    void saveConfig();
    void onConnect();
    void onDisconnect();

private:
    Ui::MixerForm *ui;

    enum flight_status flight_status;
};

#endif // MIXERFORM_H
