#include "mainwindow.h"

#include <QApplication>
#include "Anotc/anotc_json.h"

int main(int argc, char *argv[])
{
    loadFrameDefination(":/FrameDefination.json");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
