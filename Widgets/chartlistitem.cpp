#include "chartlistitem.h"
#include "ui_chartlistitem.h"
#include <QPalette>

ChartListItem::ChartListItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChartListItem)
{
    ui->setupUi(this);

    connect(ui->closeBtn, &QToolButton::clicked, this, &ChartListItem::closeClick);
    connect(ui->checkBox, &QCheckBox::clicked, this, &ChartListItem::checkClick);
}

ChartListItem::~ChartListItem()
{
    delete ui;
}

void ChartListItem::setText(QString value)
{
    ui->title_label->setText(value);
}

QString ChartListItem::getText()
{
    return ui->title_label->text();
}

void ChartListItem::setTextColor(QColor color)
{
    QPalette palette = ui->title_label->palette();
    palette.setColor(QPalette::Text, color);
    ui->title_label->setPalette(palette);
}

void ChartListItem::setValue(QString value)
{
    ui->label->setText(value);
}

QString ChartListItem::getValue()
{
    return ui->label->text();
}

void ChartListItem::closeClick(bool checked)
{
    emit onRemove(this);
}

void ChartListItem::setId(int id)
{
    this->id = id;
}

int ChartListItem::getId()
{
    return this->id;
}

void ChartListItem::checkClick(bool checked)
{
    emit onCheckClick(this, checked);
}

void ChartListItem::setChecked(bool checked)
{
    ui->checkBox->setChecked(checked);
}
