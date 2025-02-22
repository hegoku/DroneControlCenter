#ifndef CONTROLWINDOW_H
#define CONTROLWINDOW_H

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class ControlWindow;
}

class ControlWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ControlWindow(QWidget *parent = nullptr);
    ~ControlWindow();

public slots:
    void open(bool checked);
    void onChannelChanged(int value);
    void send();

private:
    Ui::ControlWindow *ui;
    QTimer *timer;
};

#endif // CONTROLWINDOW_H
