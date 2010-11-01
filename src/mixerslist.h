#ifndef MIXERSLIST_H
#define MIXERSLIST_H

#include <QtGui>

class MixersList : public QListWidget
{
    Q_OBJECT
public:
    explicit MixersList(QWidget *parent = 0);

    void dropEvent( QDropEvent * event );

signals:
    void itemDropped( int index, const QMimeData * data, Qt::DropAction action );

protected:
    bool dropMimeData( int index, const QMimeData * data, Qt::DropAction action );

public slots:



};

#endif // MIXERSLIST_H
