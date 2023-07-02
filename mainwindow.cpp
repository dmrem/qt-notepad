#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
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
    if ( showOpenDialog() ) {
        openCurrentFile();
    }
}

void MainWindow::slotSave() {
    if ( currentFile.isEmpty() ) {
        slotSaveAs();
    }
    else {
        saveCurrentFile();
    }
}

void MainWindow::slotSaveAs() {
    if ( showSaveDialog() ) {
        saveCurrentFile();
    }

}

void MainWindow::slotExit() {
    QCoreApplication::exit();
}

void MainWindow::setCurrentFile( const QString &filePath ) {
    currentFile = filePath;
    updateTitleBar();
}

bool MainWindow::showOpenDialog() {
    QFileDialog dialog( this );
    dialog.setDirectory( QStandardPaths::standardLocations( QStandardPaths::DesktopLocation ).constFirst() );
    dialog.setOption( QFileDialog::DontConfirmOverwrite );
    dialog.setAcceptMode( QFileDialog::AcceptOpen );
    dialog.setFileMode( QFileDialog::AnyFile );

    if ( dialog.exec() ) {
        setCurrentFile( dialog.selectedFiles().constFirst() );
        return true;
    }
    else {
        return false;
    }
}

bool MainWindow::showSaveDialog() {
    QFileDialog dialog( this );
    dialog.setDirectory( QStandardPaths::standardLocations( QStandardPaths::DesktopLocation ).constFirst() );
    dialog.setAcceptMode( QFileDialog::AcceptSave );
    dialog.setFileMode( QFileDialog::AnyFile );

    if ( dialog.exec() ) {
        setCurrentFile( dialog.selectedFiles().constFirst() );
        return true;
    }
    else {
        return false;
    }
}

void MainWindow::createNewFile() {
    ui->mainTextArea->clear();
    setCurrentFile( "" );
    updateTitleBar();
}

void MainWindow::updateTitleBar() {
    if ( currentFile.isEmpty() ) {
        setWindowTitle( "Editor - New File" );
    }
    else {
        setWindowTitle( "Editor - " + currentFile );
    }
}

void MainWindow::openCurrentFile() {
    QFile file( currentFile );
    if ( file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
        ui->mainTextArea->setText( QString( file.readAll() ) );
    }
    file.close();
}

void MainWindow::saveCurrentFile() {
    QFile file( currentFile );
    if ( file.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
        file.write( ui->mainTextArea->toPlainText().toUtf8() );
    }
    else {
        QMessageBox::critical( this, "Error", "Could not save file" );
    }
    file.close();
}
