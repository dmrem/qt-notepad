#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
     * Set the currently active file, and update the title bar.
     * The currently active file is used when clicking save, to know where to save the file.
     * @param filePath The path.
     */
    void setCurrentFile( const QString &filePath );

    /**
     * Clear the buffer, and clear the currently active file.
     */
    void createNewFile();

    /**
     * Finds the file referred to by the path stored in currentFile, and opens it. Then reads the contents
     * into the buffer, and closes the file.
     */
    void openCurrentFile();

private:

    Ui::MainWindow* ui;
    QString currentFile;
    bool changedSinceLastSave;

    QShortcut newShortcut, openShortcut, saveShortcut, saveAsShortcut;

    /**
     * Update the title bar to contain the path to the current file, if any.
     */
    void updateTitleBar();

    /**
     * Update whether the buffer has unsaved changes, and trigger an update to everything that
     * uses that info.
     * @param value
     */
    void setChangedSinceLastSave( bool value );

    /**
     * Opens the file referred to by the path stored in currentFile, creating it if necessary. Copies the text
     * from the buffer into the file, then closes it. Creates a message box on error.
     */
    void saveCurrentFile();

    /**
     * Opens a dialog window asking the user if they want to save their unsaved changes
     * @return The button the user pressed to exit the window.
     */
    QMessageBox::StandardButton showSaveConfirmationDialog();

    /**
     * Opens a dialog window which allows the user to choose the filename for saving. If the user chose
     * multiple files, only the first one is used.
     * @return True if the user successfully chose a path, and false if the user clicked cancel. The return value
     * is independent of whether or not the file can actually be accessed or created.
     */
    bool showSaveDialog();

    /**
     * Opens a dialog window which allows the user to choose the file to be opened. If the user chose
     * multiple files, only the first one is used.
     * @return True if the user successfully chose a path, and false if the user clicked cancel. The return value
     * is independent of whether or not the file can actually be opened.
     */
    bool showOpenDialog();

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
    void slotBufferChanged();
};

#endif // MAINWINDOW_H
