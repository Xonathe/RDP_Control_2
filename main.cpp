#include "widget.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    // скрыть консоль
    AllocConsole();
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
