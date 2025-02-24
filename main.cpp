#include "mainwindow.h"

#include <QApplication>
#include "Anotc/anotc_data_frame.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("Dragonballsoft");
    QCoreApplication::setOrganizationDomain("dragonballsoft.com");
    QCoreApplication::setApplicationName("DroneControlCenter");

    loadDataFrameDefination(":/DataFrameDefination.json");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
