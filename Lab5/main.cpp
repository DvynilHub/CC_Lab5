#include <QApplication>
#include <QStyleFactory>
#include "mywidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qApp->setStyle(QStyleFactory::create("Fusion"));
	
    mywidget w;
    w.setWindowTitle("Задача 10");
    w.setFixedSize(325,475);
    w.show();
	
    return a.exec();
}
