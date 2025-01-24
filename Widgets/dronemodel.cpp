#include "dronemodel.h"
#include "ui_dronemodel.h"
#include <QVBoxLayout>

DroneModel::DroneModel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DroneModel)
{
    ui->setupUi(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    Qt3DExtras::Qt3DWindow *view = new Qt3DExtras::Qt3DWindow();
    QWidget *container = QWidget::createWindowContainer(view);
    // view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));
}

DroneModel::~DroneModel()
{
    delete ui;
}
