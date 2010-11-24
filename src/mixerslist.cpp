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


//QMimeData * MixersList::mimeData ( const QList<QListWidgetItem *> items )
//{
//    foreach(QListWidgetItem item, items)
//    {

//    }
//}

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
        QString itemString = lsVars.at(0).toString();
        int mixerNum = lsVars.at(1).toInt();


        if(itemString.isEmpty()) {};
    }


    return true;
}
