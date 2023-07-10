#include <QFile>
#include <QMessageBox>
#include "TextBuffer.h"

TextBuffer::TextBuffer( QWidget* parent )
    : QTextEdit( parent ) {
    currentFile = "";
    changedSinceLastSave = false;

    connect(
        this, &TextBuffer::textChanged,
        [this]() { setChangedSinceLastSave( true ); }
    );
}

void TextBuffer::setCurrentFile( const QString& filePath ) {
    currentFile = filePath;
    emit headerShouldChange();
}

void TextBuffer::setChangedSinceLastSave( bool changed ) {
    if ( changedSinceLastSave != changed ) {
        changedSinceLastSave = changed;
        emit headerShouldChange();
    }
}

const QString& TextBuffer::getCurrentFile() const {
    return currentFile;
}

bool TextBuffer::isChangedSinceLastSave() const {
    return changedSinceLastSave;
}

void TextBuffer::openCurrentFile() {
    QFile file( currentFile );
    if ( file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
        this->setText( QString( file.readAll() ) );
        setChangedSinceLastSave( false );
    }
}

void TextBuffer::saveCurrentFile() {
    QFile file( currentFile );
    if ( file.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
        file.write( this->toPlainText().toUtf8() );
        setChangedSinceLastSave( false );
    }
    else {
        QMessageBox::critical( this, "Error", "Could not save file" );
    }
}

