#include "dronemodel.h"
#include "ui_dronemodel.h"
#include <QVBoxLayout>
#include <QtGui/QScreen>
#include <Qt3DRender/qpointlight.h>
#include <Qt3DExtras/qfirstpersoncameracontroller.h>
#include <Qt3DExtras/QDiffuseMapMaterial>

DroneModel::DroneModel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DroneModel)
{
    ui->setupUi(this);

    db.m_windowOn3D = new Qt3DExtras::Qt3DWindow();
    db.m_rootEntity = new Qt3DCore::QEntity();
    db.m_material = new Qt3DRender::QMaterial();
    db.m_trans = new Qt3DCore::QTransform();
    db.m_input = new Qt3DInput::QInputAspect();

    initializate("qrc:/3dmodel/quad_x/quad_x.obj");
}

DroneModel::~DroneModel()
{
    delete ui;
}

void DroneModel::initializate(const QString & obj_url)
{
    // 如果将3d放置在界面中，则需要创建一个widget来“装”它
    QWidget *container = QWidget::createWindowContainer(db.m_windowOn3D);
    // 设置场景大小
    QSize screenSize = db.m_windowOn3D->screen()->size();
    // 当你缩小对话框时，你希望它最小是多少
    container->setMinimumSize(QSize(200, 100));
    // 当你缩小对话框时，你希望它最大是多少
    container->setMaximumSize(screenSize);

    QHBoxLayout *hLayout = new QHBoxLayout(this);
    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setAlignment(Qt::AlignTop);
    // 将3d场景veiw放到widget中
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);

    // 设置3d场景的输入
    db.m_windowOn3D->registerAspect(db.m_input);

    // 设置相机属性
    // 观测视角
    db.m_windowOn3D->camera()->lens()->setPerspectiveProjection(10.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    // 相机放哪里
    db.m_windowOn3D->camera()->setPosition(QVector3D(0, 0, 20.0f));
    db.m_windowOn3D->camera()->setUpVector(QVector3D(0, 1, 0));
    // 视角中心在哪里
    db.m_windowOn3D->camera()->setViewCenter(QVector3D(0, 0, 0));

    Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(db.m_rootEntity);
    Qt3DRender::QPointLight *light = new Qt3DRender::QPointLight(lightEntity);
    light->setColor("white");
    light->setIntensity(1);
    lightEntity->addComponent(light);

    Qt3DCore::QTransform *lightTransform = new Qt3DCore::QTransform(lightEntity);
    lightTransform->setTranslation(db.m_windowOn3D->camera()->position());
    lightEntity->addComponent(lightTransform);

    db.m_trans->setScale(0.01f);
    // 模型的缩放，我加载的模型比较大，这里要缩小显示
    db.m_trans->setTranslation(QVector3D(.0f, 0.0f, 19.45f));

    Qt3DRender::QMesh *mesh = new Qt3DRender::QMesh();
    mesh->setSource(QUrl(obj_url));

    Qt3DRender::QMaterial *material = new Qt3DExtras::QPhongMaterial(db.m_rootEntity);

    db.m_rootEntity->addComponent(mesh);
    db.m_rootEntity->addComponent(material);
    db.m_rootEntity->addComponent(db.m_trans);

    // Qt3DExtras::QFirstPersonCameraController *camController = new Qt3DExtras::QFirstPersonCameraController(db.m_rootEntity);
    // camController->setCamera(db.m_windowOn3D->camera());

    db.m_windowOn3D->setRootEntity(db.m_rootEntity);

}

void DroneModel::rotateModel(float x, float y, float z)
{
    db.m_trans->setRotationX(x);
    db.m_trans->setRotationX(y);
    db.m_trans->setRotationX(z);
}
