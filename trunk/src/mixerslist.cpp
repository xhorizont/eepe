#include "mixerslist.h"

MixersList::MixersList(QWidget *parent) :
    QListWidget(parent)
{
    setFont(QFont("Courier New",12));
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
}

void MixersList::dragEnterEvent(QDragEnterEvent *event)
{
    QListWidget::dragEnterEvent(event);
}

bool MixersList::dropMimeData( int index, const QMimeData * data, Qt::DropAction action )
{
    emit itemDropped(index, data, action);
    return true;
}
