#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow( QWidget* parent )
    : QMainWindow( parent ), ui( new Ui::MainWindow ) {
    ui->setupUi( this );

    QApplication::connect(
        ui->actionNew,
        SIGNAL( triggered() ),
        this,
        SLOT( slotNew() )
    );

    QApplication::connect(
        ui->actionOpen,
        SIGNAL( triggered() ),
        this,
        SLOT( slotOpen() )
    );

    QApplication::connect(
        ui->actionSave,
        SIGNAL( triggered() ),
        this,
        SLOT( slotSave() )
    );

    QApplication::connect(
        ui->actionSaveAs,
        SIGNAL( triggered() ),
        this,
        SLOT( slotSaveAs() )
    );

    QApplication::connect(
        ui->actionExit,
        SIGNAL( triggered() ),
        this,
        SLOT( slotExit() )
    );

    currentFile = "";

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::slotNew() {
    createNewFile();
}

void MainWindow::slotOpen() {

}

void MainWindow::slotSave() {

}

void MainWindow::slotSaveAs() {

}

void MainWindow::slotExit() {
    QCoreApplication::exit();
}

void MainWindow::createNewFile() {
    ui->mainTextArea->clear();
    currentFile = "";
    updateTitleBar();
}

void MainWindow::openFile(QString fileName) {

}

void MainWindow::updateTitleBar() {
    if ( currentFile.isEmpty() ) {
        setWindowTitle( "Editor - New File" );
    }
    else {
        setWindowTitle( "Editor - " + currentFile );
    }
}
