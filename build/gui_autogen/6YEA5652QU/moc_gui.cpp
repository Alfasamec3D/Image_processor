/****************************************************************************
** Meta object code from reading C++ file 'gui.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.13)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../include/gui.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gui.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.13. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainAppWindow_t {
    QByteArrayData data[11];
    char stringdata0[132];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainAppWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainAppWindow_t qt_meta_stringdata_MainAppWindow = {
    {
QT_MOC_LITERAL(0, 0, 13), // "MainAppWindow"
QT_MOC_LITERAL(1, 14, 9), // "loadImage"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 14), // "applyGrayscale"
QT_MOC_LITERAL(4, 40, 9), // "saveImage"
QT_MOC_LITERAL(5, 50, 14), // "showBlurSlider"
QT_MOC_LITERAL(6, 65, 11), // "rotateImage"
QT_MOC_LITERAL(7, 77, 10), // "rotateCode"
QT_MOC_LITERAL(8, 88, 24), // "showBrightContrastSlider"
QT_MOC_LITERAL(9, 113, 9), // "flipImage"
QT_MOC_LITERAL(10, 123, 8) // "flipCode"

    },
    "MainAppWindow\0loadImage\0\0applyGrayscale\0"
    "saveImage\0showBlurSlider\0rotateImage\0"
    "rotateCode\0showBrightContrastSlider\0"
    "flipImage\0flipCode"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainAppWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x08 /* Private */,
       3,    0,   50,    2, 0x08 /* Private */,
       4,    0,   51,    2, 0x08 /* Private */,
       5,    0,   52,    2, 0x08 /* Private */,
       6,    1,   53,    2, 0x08 /* Private */,
       8,    0,   56,    2, 0x08 /* Private */,
       9,    1,   57,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,

       0        // eod
};

void MainAppWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainAppWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->loadImage(); break;
        case 1: _t->applyGrayscale(); break;
        case 2: _t->saveImage(); break;
        case 3: _t->showBlurSlider(); break;
        case 4: _t->rotateImage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->showBrightContrastSlider(); break;
        case 6: _t->flipImage((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainAppWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainAppWindow.data,
    qt_meta_data_MainAppWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainAppWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainAppWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainAppWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainAppWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
