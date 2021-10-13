#include "EquipmentTester.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EquipmentTester w;
    w.showMaximized();
    w.setWindowTitle(QString::fromLocal8Bit("主控制器调试软件"));
    return a.exec();
}
