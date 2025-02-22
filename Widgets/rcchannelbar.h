#ifndef RCCHANNELBAR_H
#define RCCHANNELBAR_H

#include <QWidget>
#include <QPaintEvent>

namespace Ui {
class RCChannelBar;
}

class RCChannelBar : public QWidget
{
    Q_OBJECT

public:
    explicit RCChannelBar(QWidget *parent = nullptr);
    ~RCChannelBar();
    void setValue(int value);
    void setMin(int value);
    void setMax(int value);
    void setBackgroundColor(QString color);
    void setChunkColor(QString color);

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::RCChannelBar *ui;
    int value;
    int min;
    int max;
    QString background_color;
    QString chunk_color;
    QString text_color;
};

#endif // RCCHANNELBAR_H
