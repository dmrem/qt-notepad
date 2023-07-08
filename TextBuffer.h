#pragma once

#include <QTextEdit>

class TextBuffer : public QTextEdit {
Q_OBJECT

public:

    TextBuffer( QWidget* parent = nullptr );

    /**
     * Gets the path to the currently loaded file.
     * @return The file path.
     */
    const QString& getCurrentFile() const;

    /**
     * Set the currently active file. // todo update documentation once signal is added
     * @param filePath The path to the file.
     */
    void setCurrentFile( const QString& filePath );

    /**
     * Get whether the buffer has unsaved changes.
     * @return True if the buffer has unsaved changes.
     */
    bool isChangedSinceLastSave() const;

    /**
     * Set whether the buffer has unsaved changes.
     * @param value True if the buffer has unsaved changes.
     */
    void setChangedSinceLastSave( bool value );

    /**
     * Finds the file referred to by the path stored in currentFile, and opens it. Then reads the contents
     * into the buffer, and closes the file.
     */
    void openCurrentFile();

    /**
     * Opens the file referred to by the path stored in currentFile, creating it if necessary. Copies the text
     * from the buffer into the file, then closes it. Creates a message box on error.
     */
    void saveCurrentFile();

private:
    QString currentFile;
    bool changedSinceLastSave;

};


