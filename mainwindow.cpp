#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "TextBuffer.h"

//todo: add signal for current file changing, add slots to update title bar and tab label
//todo: clean up commented out code

MainWindow::MainWindow( QWidget* parent )
    : QMainWindow( parent ),
      ui( new Ui::MainWindow ),
      newShortcut( QKeySequence( "Ctrl+N" ), this, this, &MainWindow::slotNew ),
      openShortcut( QKeySequence( "Ctrl+O" ), this, this, &MainWindow::slotOpen ),
      saveShortcut( QKeySequence( "Ctrl+S" ), this, this, &MainWindow::slotSave ),
      saveAsShortcut( QKeySequence( "Ctrl+Shift+S" ), this, this, &MainWindow::slotSaveAs ) {

    ui->setupUi( this );

    connect(
        ui->actionNew, &QAction::triggered,
        this, &MainWindow::slotNew
    );

    connect(
        ui->actionOpen, &QAction::triggered,
        this, &MainWindow::slotOpen
    );

    connect(
        ui->actionSave, &QAction::triggered,
        this, &MainWindow::slotSave
    );

    connect(
        ui->actionSaveAs, &QAction::triggered,
        this, &MainWindow::slotSaveAs
    );

    connect(
        ui->actionExit, &QAction::triggered,
        this, &MainWindow::slotExit
    );

    connect(
        ui->tabWidget, &QTabWidget::currentChanged,
        this, &MainWindow::updateTitleBar
    );

    connect(
        ui->tabWidget, &QTabWidget::tabCloseRequested,
        this, &MainWindow::slotCloseTab
    );

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::slotNew() {
    createNewFile();
}

void MainWindow::slotOpen() {

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

void MainWindow::slotCloseTab( int index ) {
    TextBuffer* bufferToClose = static_cast<TextBuffer*>(ui->tabWidget->widget( index ));
    if ( bufferToClose == nullptr ) {
        return;
    }

    if ( bufferToClose->isChangedSinceLastSave() ) {
        switch ( showSaveConfirmationDialog() ) {
            case QMessageBox::Yes:
                if ( bufferToClose->getCurrentFile().isEmpty() ) {
                    QString savePath = showSaveDialog();
                    if ( savePath != "" ) {
                        bufferToClose->setCurrentFile( savePath );
                        bufferToClose->saveCurrentFile();
                    }
                    else {
                        return;
                    }
                }
                else {
                    bufferToClose->saveCurrentFile();
                }
                break;
            case QMessageBox::No:
                // do nothing, proceed directly to closing tab
                break;
            default:
                // do nothing and don't close
                return;
        }
    }

    // the program crashes if there isn't a different tab to show after the current is deleted
    if ( ui->tabWidget->count() <= 1 ) {
        createNewFile();
    }

    ui->tabWidget->removeTab( index );
    delete bufferToClose;
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
