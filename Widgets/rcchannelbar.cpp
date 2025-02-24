#include "rcchannelbar.h"
#include "ui_rcchannelbar.h"
#include <QPainter>
#include <QRect>
#include <QPen>
#include <QColor>
#include <QBrush>

RCChannelBar::RCChannelBar(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RCChannelBar)
{
    ui->setupUi(this);

    value = 0;
    min = 0;
    max = 100;
    background_color = QString("#ffffff");
    chunk_color = QString("#FEB125");
    text_color = QString("black");
}

RCChannelBar::~RCChannelBar()
{
    delete ui;
}

void RCChannelBar::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    QRect rect(0, 0, width(), height());
    painter.setViewport(rect);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    QPen pen;
    pen.setWidth(2);
    pen.setColor(QColor("#000000"));
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::SquareCap);
    pen.setJoinStyle(Qt::MiterJoin);
    painter.setPen(pen);

    QBrush brush;
    brush.setColor(QColor(background_color));
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);

    rect.setRect(0, 0, width(), height());
    painter.drawRect(rect);

    if (value>min && value<=max) {
        brush.setColor(QColor(chunk_color));
        pen.setColor(QColor(chunk_color));
        float v = ((float)value-(float)min) / ((float)max-(float)min);
        rect.setRect(2, 2, fmax(v*width()-4, 0), height()-4);
    } else {
        brush.setColor(QColor(background_color));
        pen.setColor(QColor(background_color));
        rect.setRect(2, 2, width()-4, height()-4);
    }
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(rect);

    QFontMetrics textSize(this->font());
    QString str = QString("%1").arg(value);
    QRect textRect = textSize.boundingRect(str);

    painter.setFont(this->font());
    pen.setColor(QColor(text_color));
    painter.setPen(pen);

    painter.drawText(width()/2-textRect.width()/2, height()/2+textRect.height()/2-textRect.bottom(), str);

}

void RCChannelBar::setValue(int value)
{
    this->value = value;
    update();
}

void RCChannelBar::setMin(int value)
{
    min = value;
}


void RCChannelBar::setMax(int value)
{
    max = value;
}
