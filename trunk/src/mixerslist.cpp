#include "mixerslist.h"
//Q_DECLARE_METATYPE(QModelIndex); //at start of BookMarkList.cpp file (after #includes)

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


bool MixersList::dropMimeData( int index, const QMimeData * data, Qt::DropAction action )
{
    QByteArray dropData = data->data("application/x-qabstractitemmodeldatalist");
    QDataStream stream(&dropData, QIODevice::ReadOnly);

    while (!stream.atEnd())
    {
        int r,c;
        QMap<int, QVariant> v;
        stream >> r >> c >> v;
        QList<QVariant> lsVars;
        lsVars = v.values();
        QVariant qVar = lsVars.at(0);
        QString itemString = qVar.toString();

        if(itemString.isEmpty()) {};
    }


    return true;
}
