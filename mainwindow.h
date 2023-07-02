#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

// todo: keep track if the file is saved, add "do you want to save" dialog when exiting or creating a new file if it isn't
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

    /**
     * Update the title bar to contain the path to the current file, if any.
     */
    void updateTitleBar();

    /**
     * Opens the file referred to by the path stored in currentFile, creating it if necessary. Copies the text
     * from the buffer into the file, then closes it. Creates a message box on error.
     */
    void saveCurrentFile();

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

};

#endif // MAINWINDOW_H
