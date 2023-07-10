#pragma once

#include <QMainWindow>
#include <QMessageBox>
#include <QShortcut>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:

    MainWindow( QWidget* parent = nullptr );

    ~MainWindow();

    /**
     * Clear the buffer, and clear the currently active file.
     */
    void openNewBuffer();

private:

    Ui::MainWindow* ui;
    QShortcut newShortcut, openShortcut, saveShortcut, saveAsShortcut;

    /**
     * Update the title bar to contain the path to the current file of the currently selected tab, if any.
     */
    void updateTitleBar();

    /**
     * Update the label of the currently selected tab to contain the path to the current file, if any.
     */
    void updateCurrentTabLabel();

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

    // called when a tab should be closed
    void slotCloseTab( int index );
};
