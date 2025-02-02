#ifndef DRONEMODEL_H
#define DRONEMODEL_H

#include <QWidget>
#include <QPointer>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DRender/QCamera>
#include <Qt3DCore/QEntity>
#include <Qt3DRender/QMesh>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DCore/QTransform>
#include <Qt3DInput/QInputAspect>
#include <Anotc/anotc_data_frame.h>

namespace Ui {
class DroneModel;
}

class DroneModel : public QWidget
{
    Q_OBJECT

public:
    explicit DroneModel(QWidget *parent = nullptr);
    ~DroneModel();

    void initializate(const QString & obj_url);
    void rotateModel(float x, float y, float z);

public slots:
    void onAttitudeUpdate(struct anotc_parsed_data_frame);

private:

    Ui::DroneModel *ui;

    struct date_struct
    {
        QPointer<Qt3DExtras::Qt3DWindow> m_windowOn3D;
        QPointer<Qt3DCore::QEntity> m_rootEntity;
        QPointer<Qt3DRender::QMaterial> m_material;
        QPointer<Qt3DCore::QTransform> m_trans;
        QPointer<Qt3DInput::QInputAspect> m_input;
        QPointer<Qt3DRender::QCamera> m_camera;
    }db;
};

#endif // DRONEMODEL_H
