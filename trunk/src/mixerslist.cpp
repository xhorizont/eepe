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


void MixersList::dropEvent(QDropEvent * event)
{
    if (event->source() == this && d->movement != Static) {
        event->ignore();
        return;
    }

    if (event->source() == this && (event->dropAction() == Qt::MoveAction ||
                                    dragDropMode() == QAbstractItemView::InternalMove)) {
        QModelIndex topIndex;
        int col = -1;
        int row = -1;
        if (d->dropOn(event, &row, &col, &topIndex)) {
            QList<QModelIndex> selIndexes = selectedIndexes();
            QList<QPersistentModelIndex> persIndexes;
            for (int i = 0; i < selIndexes.count(); i++)
                persIndexes.append(selIndexes.at(i));

            if (persIndexes.contains(topIndex))
                return;
            qSort(persIndexes); // The dropped items will remain in the same visual order.

            QPersistentModelIndex dropRow = model()->index(row, col, topIndex);

            int r = row == -1 ? count() : (dropRow.row() >= 0 ? dropRow.row() : row);
            for (int i = 0; i < persIndexes.count(); ++i) {
                const QPersistentModelIndex &pIndex = persIndexes.at(i);
                d->listModel()->move(pIndex.row(), r);
                r = pIndex.row() + 1;   // Dropped items are inserted contiguously and in the right order.
            }

            event->accept();
            // Don't want QAbstractItemView to delete it because it was "moved" we already did it
            event->setDropAction(Qt::CopyAction);
        }
    }

    QListView::dropEvent(event);
}

bool MixersList::dropMimeData( int index, const QMimeData * data, Qt::DropAction action )
{
    emit itemDropped(index, data, action);
    return true;
}
