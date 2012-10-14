#include "MovieFilesOrganizer.h"
#include "MovieFileSearcher.h"
#include "globals/NameFormatter.h"

#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QDir>

/**
 * @brief MovieFilesOrganizer::MovieFilesOrganizer
 * @param parent
 */
MovieFilesOrganizer::MovieFilesOrganizer(QObject *parent) :
    QThread(parent)
{
    m_progressMessageId = Constants::MovieFilesToDirsProgressMessageId;
}

/**
 * @brief MovieFilesOrganizer::~MovieFilesOrganizer
 */
MovieFilesOrganizer::~MovieFilesOrganizer()
{
}

/**
 * @brief Starts the foldering process
 */
void MovieFilesOrganizer::moveToDirs(QString path)
{
    path = QDir::toNativeSeparators(path);
    QFileInfo fi(path);
    if (!fi.isDir()) {
        qDebug() << "Source " << path << " is no directory.";
        canceled("Source " + path + " is no directory");
    }

    QList<QStringList> contents;
    MovieFileSearcher *fileSearcher = new MovieFileSearcher(this);
    fileSearcher->scanDir(path, contents, false, true);

    int pos = path.lastIndexOf(QDir::separator());
    QString dirName = path.right(path.length() - pos -1);
    QString fileName;
    NameFormatter *nameFormat = NameFormatter::instance(this);


    foreach (QStringList movie, contents) {
        int pos = movie.at(0).lastIndexOf(QDir::separator());
        if (!(movie.at(0).left(pos).endsWith(dirName))) {
            qDebug() << "skipping " << movie.at(0);
            continue;
        }

        fi.setFile(movie.at(0));
        fileName = fi.completeBaseName();
        QDir *dir = new QDir();

        QString newFolder;
        if (movie.length() == 1)
            newFolder = path + QDir::separator() + nameFormat->formatName(fileName);
        else if (movie.length() > 1)
            newFolder = path + QDir::separator() + nameFormat->formatName(
                        nameFormat->formatParts(fileName));
        else
            continue;

        if (!(dir->mkdir(newFolder)))
            continue;

        foreach (QString file, movie) {
            if (!dir->rename(file, newFolder +
                             QDir::separator() +
                             file.right(file.length() -
                                        file.lastIndexOf
                                        (QDir::separator()) - 1)))
                qDebug() << "Moving " << file << "to " << newFolder << " failed.";
        }
    }
}

void MovieFilesOrganizer::canceled(QString msg)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText("Operation not possible.");
    msgBox.setInformativeText(msg + "\n Operation Canceled.");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
}