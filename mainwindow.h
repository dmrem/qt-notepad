#pragma once

#include <QMainWindow>
#include <QMessageBox>
#include <QShortcut>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

// todo: support for multiple buffers via tabs
public:

    MainWindow( QWidget* parent = nullptr );

    ~MainWindow();

    /**
     * Clear the buffer, and clear the currently active file.
     */
    void createNewFile();

private:

    Ui::MainWindow* ui;
    QShortcut newShortcut, openShortcut, saveShortcut, saveAsShortcut;

    /**
     * Update the title bar to contain the path to the current file, if any.
     */
    void updateTitleBar();

    /**
     * Opens a dialog window asking the user if they want to save their unsaved changes
     * @return The button the user pressed to exit the window.
     */
    QMessageBox::StandardButton showSaveConfirmationDialog();

    /**
     * Opens a dialog window which allows the user to choose the file to be opened. If the user chose
     * multiple files, only the first one is used.
     * @return The path to the file that the user chose. If the user did not choose a file, returns the empty string.
     */
    QString showOpenDialog();

    /**
     * Opens a dialog window which allows the user to choose the filename for saving. If the user chose
     * multiple files, only the first one is used.
     * @return The path to the file that the user chose. If the user did not choose a file, returns the empty string.
     */
    QString showSaveDialog();

private slots:

    // called when the user clicks file->new
    void slotNew();

    // called when the user clicks file->open
    void slotOpen();

    // called when the user clicks file->save
    void slotSave();

    // called when the user clicks file->save as
    void slotSaveAs();

    // called when the user clicks file->exit
    void slotExit();

    // called when the user types something in the buffer
//    void slotBufferChanged();
};
