#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:

    MainWindow( QWidget* parent = nullptr );

    ~MainWindow();

    void setCurrentFile( const QString &filePath );

    void createNewFile();

    void openCurrentFile();

private slots:

    void slotNew();

    void slotOpen();

    void slotSave();

    void slotSaveAs();

    void slotExit();

private:

    Ui::MainWindow* ui;
    QString currentFile;

    void updateTitleBar();

    void saveCurrentFile();

    bool showSaveDialog();

    bool showOpenDialog();

};

#endif // MAINWINDOW_H
