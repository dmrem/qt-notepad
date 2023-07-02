#include "mainwindow.h"

#include <QApplication>
#include <QDebug>

int main( int argc, char* argv[] ) {
    QApplication a( argc, argv );
    MainWindow w;
    w.show();

    QStringList arguments = QCoreApplication::arguments();
    if ( arguments.length() > 1 ) {
        w.setCurrentFile( arguments.at( 1 ) );
        w.openCurrentFile();
    }
    else {
        w.createNewFile();
    }

    return a.exec();

}
