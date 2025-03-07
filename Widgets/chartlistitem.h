#ifndef CHARTLISTITEM_H
#define CHARTLISTITEM_H

#include <QWidget>
#include <QColor>

namespace Ui {
class ChartListItem;
}

class ChartListItem : public QWidget
{
    Q_OBJECT

public:
    explicit ChartListItem(QWidget *parent = nullptr);
    ~ChartListItem();

    void setText(QString value);
    QString getText();
    void setTextColor(QColor color);
    void setValue(QString value);
    QString getValue();
    void setId(int id);
    int getId();

public slots:
    void closeClick(bool checked = false);

signals:
    void onRemove(ChartListItem*);

private:
    Ui::ChartListItem *ui;
    int id;

};

#endif // CHARTLISTITEM_H
