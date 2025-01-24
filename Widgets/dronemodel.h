#ifndef DRONEMODEL_H
#define DRONEMODEL_H

#include <QWidget>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DRender/QCamera>
#include <Qt3DCore/QEntity>
#include <Qt3DRender/QMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DCore/QTransform>
#include <Qt3DInput/QInputAspect>

namespace Ui {
class DroneModel;
}

class DroneModel : public QWidget
{
    Q_OBJECT

public:
    explicit DroneModel(QWidget *parent = nullptr);
    ~DroneModel();

private:
    Ui::DroneModel *ui;
};

#endif // DRONEMODEL_H
