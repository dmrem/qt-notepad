#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "TextBuffer.h"

//todo: add signal for current file changing, add slots to update title bar and tab label
//todo: make the tab close buttons work
//todo: clean up commented out code

MainWindow::MainWindow( QWidget* parent )
    : QMainWindow( parent ),
      ui( new Ui::MainWindow ),
      newShortcut( QKeySequence( "Ctrl+N" ), this, this, &MainWindow::slotNew ),
      openShortcut( QKeySequence( "Ctrl+O" ), this, this, &MainWindow::slotOpen ),
      saveShortcut( QKeySequence( "Ctrl+S" ), this, this, &MainWindow::slotSave ),
      saveAsShortcut( QKeySequence( "Ctrl+Shift+S" ), this, this, &MainWindow::slotSaveAs ) {

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
        ui->tabWidget, &QTabWidget::currentChanged,
        this, &MainWindow::updateTitleBar
    );

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::slotNew() {
    // move this to close tab button
//    if ( changedSinceLastSave ) {
//        switch ( showSaveConfirmationDialog() ) {
//            case QMessageBox::Yes:
//                if ( currentFile.isEmpty() ) {
//                    if ( showSaveDialog() ) {
//                        saveCurrentFile();
//                    }
//                    else {
//                        return;
//                    }
//                }
//                else {
//                    saveCurrentFile();
//                }
//                break;
//            case QMessageBox::No:
//                // do nothing, proceed directly to creating new file
//                break;
//            default:
//                // do nothing and don't save
//                return;
//        }
//    }

    createNewFile();
}

void MainWindow::slotOpen() {
//    if ( changedSinceLastSave ) {
//        switch ( showSaveConfirmationDialog() ) {
//            case QMessageBox::Yes:
//                if ( currentFile.isEmpty() ) {
//                    if ( showSaveDialog() ) {
//                        saveCurrentFile();
//                    }
//                    else {
//                        return;
//                    }
//                }
//                else {
//                    saveCurrentFile();
//                }
//                break;
//            case QMessageBox::No:
//                // do nothing, proceed directly to opening file
//                break;
//            default:
//                // do nothing and don't open
//                return;
//        }
//    }

    QString selectedFile = showOpenDialog();
    if ( selectedFile == "" ) {
        return;
    }

    TextBuffer* currentBuffer = static_cast<TextBuffer*>(ui->tabWidget->currentWidget());

    TextBuffer* buffer;
    if ( currentBuffer->getCurrentFile().isEmpty() && currentBuffer->toPlainText().isEmpty() ) {
        buffer = currentBuffer;
    }
    else {
        buffer = new TextBuffer( ui->tabWidget );
        ui->tabWidget->setCurrentIndex( ui->tabWidget->addTab( buffer, selectedFile ) );
    }

    buffer->setCurrentFile( selectedFile );
    buffer->openCurrentFile();

}

void MainWindow::slotSave() {
    TextBuffer* currentBuffer = static_cast<TextBuffer*>(ui->tabWidget->currentWidget());
    if ( currentBuffer->getCurrentFile().isEmpty() ) {
        QString savePath = showSaveDialog();
        if ( savePath != "" ) {
            currentBuffer->setCurrentFile( savePath );
            currentBuffer->saveCurrentFile();
        }
    }
    else {
        currentBuffer->saveCurrentFile();
    }
}

void MainWindow::slotSaveAs() {
    TextBuffer* currentBuffer = static_cast<TextBuffer*>(ui->tabWidget->currentWidget());
    QString savePath = showSaveDialog();
    if ( savePath != "" ) {
        currentBuffer->setCurrentFile( savePath );
        currentBuffer->saveCurrentFile();
    }
}

void MainWindow::slotExit() {
    QCoreApplication::exit();
}

//void MainWindow::slotBufferChanged() {
//    if ( !changedSinceLastSave ) {
//        setChangedSinceLastSave( true );
//        updateTitleBar();
//    }
//}

QMessageBox::StandardButton MainWindow::showSaveConfirmationDialog() {
    return QMessageBox::question(
        this,
        "Editor",
        "You have unsaved changes. Do you want to save?",
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
        QMessageBox::Cancel
    );
}

QString MainWindow::showOpenDialog() {
    QFileDialog dialog( this );
    dialog.setDirectory( QStandardPaths::standardLocations( QStandardPaths::DesktopLocation ).constFirst() );
    dialog.setOption( QFileDialog::DontConfirmOverwrite );
    dialog.setAcceptMode( QFileDialog::AcceptOpen );
    dialog.setFileMode( QFileDialog::AnyFile );

    if ( dialog.exec() ) {
        return dialog.selectedFiles().constFirst();
    }
    else {
        return "";
    }
}

QString MainWindow::showSaveDialog() {
    QFileDialog dialog( this );
    dialog.setDirectory( QStandardPaths::standardLocations( QStandardPaths::DesktopLocation ).constFirst() );
    dialog.setAcceptMode( QFileDialog::AcceptSave );
    dialog.setFileMode( QFileDialog::AnyFile );

    if ( dialog.exec() ) {
        return dialog.selectedFiles().constFirst();
    }
    else {
        return "";
    }
}

void MainWindow::createNewFile() {
    TextBuffer* buffer = new TextBuffer( ui->tabWidget );

    ui->tabWidget->setCurrentIndex( ui->tabWidget->addTab( buffer, "New File" ) );
}

void MainWindow::updateTitleBar() {
    QString titleString;
    QTextStream stream( &titleString );
    TextBuffer* buffer = static_cast<TextBuffer*>(ui->tabWidget->currentWidget());

    stream
        << ( buffer->isChangedSinceLastSave() ? "*" : "" )
        << "Editor - "
        << ( buffer->getCurrentFile().isEmpty() ? "New File" : buffer->getCurrentFile() );
    setWindowTitle( titleString );
}
