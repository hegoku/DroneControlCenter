#ifndef RCFORM_H
#define RCFORM_H

#include <QWidget>
#include "Anotc/anotc.h"
#include "Anotc/anotc_data_frame.h"
#include "Anotc/anotc_config_frame.h"
#include "Anotc/anotc_receive_check.h"
#include "flight.h"
#include "controlwindow.h"
#include "Widgets/rcchannelbar.h"

namespace Ui {
class RCForm;
}

class RCForm : public QWidget
{
    Q_OBJECT

public:
    explicit RCForm(QWidget *parent = nullptr);
    ~RCForm();

public slots:
    void showControlWin();
    void saveConfig();
    void onConnect();
    void onDisconnect();
    void paramUpdated(struct anotc_parsed_parameter_frame);
    void onFlightUpdate(struct anotc_parsed_data_frame);

private:
    Ui::RCForm *ui;

    ControlWindow *control_win;
    QList<RCChannelBar*> channel_bar;
};

#endif // RCFORM_H
