#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow( QWidget* parent )
    : QMainWindow( parent ),
      ui( new Ui::MainWindow ),
      newShortcut( QKeySequence( "Ctrl+N" ), this, this, &MainWindow::slotNew ),
      openShortcut( QKeySequence( "Ctrl+O" ), this, this, &MainWindow::slotOpen ),
      saveShortcut( QKeySequence( "Ctrl+S" ), this, this, &MainWindow::slotSave ),
      saveAsShortcut( QKeySequence( "Ctrl+Shift+S" ), this, this, &MainWindow::slotSaveAs ),
      currentFile( "" ),
      changedSinceLastSave( false ) {

    ui->setupUi( this );


    QApplication::connect(
        ui->actionNew, &QAction::triggered,
        this, &MainWindow::slotNew
    );

    QApplication::connect(
        ui->actionOpen, &QAction::triggered,
        this, &MainWindow::slotOpen
    );

    QApplication::connect(
        ui->actionSave, &QAction::triggered,
        this, &MainWindow::slotSave
    );

    QApplication::connect(
        ui->actionSaveAs, &QAction::triggered,
        this, &MainWindow::slotSaveAs
    );

    QApplication::connect(
        ui->actionExit, &QAction::triggered,
        this, &MainWindow::slotExit
    );

    QApplication::connect(
        ui->mainTextArea, &QTextEdit::textChanged,
        this, &MainWindow::slotBufferChanged
    );

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::slotNew() {
    if ( changedSinceLastSave ) {
        switch ( showSaveConfirmationDialog() ) {
            case QMessageBox::Yes:
                if ( currentFile.isEmpty() ) {
                    if ( showSaveDialog() ) {
                        saveCurrentFile();
                    }
                    else {
                        return;
                    }
                }
                else {
                    saveCurrentFile();
                }
                break;
            case QMessageBox::No:
                // do nothing, proceed directly to creating new file
                break;
            default:
                // do nothing and don't save
                return;
        }
    }

    createNewFile();
}

void MainWindow::slotOpen() {
    if ( changedSinceLastSave ) {
        switch ( showSaveConfirmationDialog() ) {
            case QMessageBox::Yes:
                if ( currentFile.isEmpty() ) {
                    if ( showSaveDialog() ) {
                        saveCurrentFile();
                    }
                    else {
                        return;
                    }
                }
                else {
                    saveCurrentFile();
                }
                break;
            case QMessageBox::No:
                // do nothing, proceed directly to opening file
                break;
            default:
                // do nothing and don't open
                return;
        }
    }

    if ( showOpenDialog() ) {
        openCurrentFile();
    }
}

void MainWindow::slotSave() {
    if ( currentFile.isEmpty() ) {
        if ( showSaveDialog() ) {
            saveCurrentFile();
        }
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

void MainWindow::slotBufferChanged() {
    if ( !changedSinceLastSave ) {
        setChangedSinceLastSave( true );
        updateTitleBar();
    }
}

void MainWindow::setCurrentFile( const QString &filePath ) {
    currentFile = filePath;
    updateTitleBar();
}

QMessageBox::StandardButton MainWindow::showSaveConfirmationDialog() {
    return QMessageBox::question(
        this,
        "Editor",
        "You have unsaved changes. Do you want to save?",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
        QMessageBox::Cancel
    );
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
    setChangedSinceLastSave( false );
    setCurrentFile( "" );
    updateTitleBar();
}

void MainWindow::updateTitleBar() {
    QString titleString;
    QTextStream stream( &titleString );

    stream << ( changedSinceLastSave ? "*" : "" ) << "Editor - "
        << ( currentFile.isEmpty() ? "New File" : currentFile );
    setWindowTitle( titleString );
}

void MainWindow::setChangedSinceLastSave( bool value ) {
    changedSinceLastSave = value;
    updateTitleBar();
}

void MainWindow::openCurrentFile() {
    QFile file( currentFile );
    if ( file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
        ui->mainTextArea->setText( QString( file.readAll() ) );
        setChangedSinceLastSave( false );
    }
    file.close();
}

void MainWindow::saveCurrentFile() {
    QFile file( currentFile );
    if ( file.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
        file.write( ui->mainTextArea->toPlainText().toUtf8() );
        setChangedSinceLastSave( false );
    }
    else {
        QMessageBox::critical( this, "Error", "Could not save file" );
    }
    file.close();
}
