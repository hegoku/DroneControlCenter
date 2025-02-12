#ifndef CALIBRATIONFORM_H
#define CALIBRATIONFORM_H

#include <QWidget>
#include "Anotc/anotc_receive_check.h"
#include "Anotc/anotc_data_frame.h"

namespace Ui {
class CalibrationForm;
}

class CalibrationForm : public QWidget
{
    Q_OBJECT

public:
    explicit CalibrationForm(QWidget *parent = nullptr);
    ~CalibrationForm();

public slots:
    void calibrate_gyro();
    void receiveCheckFrame(struct anotc_parsed_check_frame);
    void updateData(struct anotc_parsed_data_frame item);
    void cmdResponse(struct anotc_blocking_queue_item);
    void calibrate_accel();
    void calibrate_accel_up();
    void calibrate_accel_down();
    void calibrate_accel_forward();
    void calibrate_accel_backward();
    void calibrate_accel_left();
    void calibrate_accel_right();

private:
    Ui::CalibrationForm *ui;
};

#endif // CALIBRATIONFORM_H
