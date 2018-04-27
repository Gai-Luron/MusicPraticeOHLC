#ifndef RECENTFILES_H
#define RECENTFILES_H

#include <QObject>

class recentFiles
{
private:
    QString pathData;
    QStringList lof;
    void save();
    void load();
public:
    recentFiles();
    QStringList listOfFiles();
    void addFile(QString fileName);

};

#endif // RECENTFILES_H
