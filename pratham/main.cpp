#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("Styled Quiz Game");
    window.resize(600, 400);
    window.show();
    return app.exec();
}
