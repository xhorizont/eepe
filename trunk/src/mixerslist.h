#ifndef MIXERSLIST_H
#define MIXERSLIST_H

#include <QtGui>

class MixersList : public QListWidget
{
    Q_OBJECT
public:
    explicit MixersList(QWidget *parent = 0);

    void dragEnterEvent(QDragEnterEvent *event);

signals:
    void itemDropped( int index, const QMimeData * data, Qt::DropAction action );

protected:
    bool dropMimeData( int index, const QMimeData * data, Qt::DropAction action );

public slots:

private:
    QPoint dragStartPosition;




};

#endif // MIXERSLIST_H
