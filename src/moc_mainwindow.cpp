/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Sat Sep 25 01:06:00 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      47,   41,   11,   11, 0x08,
      88,   11,   11,   11, 0x08,
     115,   11,   11,   11, 0x08,
     141,   11,   11,   11, 0x08,
     167,   11,   11,   11, 0x08,
     197,  193,   11,   11, 0x08,
     221,   11,   11,   11, 0x08,
     235,   11,   11,   11, 0x08,
     257,   11,   11,   11, 0x08,
     282,   11,   11,   11, 0x08,
     301,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0on_actionSave_As_activated()\0"
    "index\0on_listWidget_doubleClicked(QModelIndex)\0"
    "on_actionAbout_activated()\0"
    "on_actionSave_activated()\0"
    "on_actionOpen_activated()\0"
    "on_actionQuit_activated()\0pos\0"
    "ShowContextMenu(QPoint)\0RefreshList()\0"
    "DeleteSelectedModel()\0DuplicateSelectedModel()\0"
    "CutSelectedModel()\0PasteSelectedModel()\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_actionSave_As_activated(); break;
        case 1: on_listWidget_doubleClicked((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 2: on_actionAbout_activated(); break;
        case 3: on_actionSave_activated(); break;
        case 4: on_actionOpen_activated(); break;
        case 5: on_actionQuit_activated(); break;
        case 6: ShowContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 7: RefreshList(); break;
        case 8: DeleteSelectedModel(); break;
        case 9: DuplicateSelectedModel(); break;
        case 10: CutSelectedModel(); break;
        case 11: PasteSelectedModel(); break;
        default: ;
        }
        _id -= 12;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
