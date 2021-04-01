TARGET = QGVCore
TEMPLATE = lib
CONFIG += shared

 DEFINES += WITH_CGRAPH
 CONFIG += link_pkgconfig
 PKGCONFIG += libcdt libgvc libcgraph

INCLUDEPATH += $$PWD/../../qgv
INCLUDEPATH += $$PWD/../../qgv/private


HEADERS += \
    $$PWD/../../qgv/private/QGVCore.h \
    $$PWD/../../qgv/private/QGVEdgePrivate.h \
    $$PWD/../../qgv/private/QGVGraphPrivate.h \
    $$PWD/../../qgv/private/QGVGvcPrivate.h \
    $$PWD/../../qgv/private/QGVNodePrivate.h \
    $$PWD/../../qgv/qgv.h \
    $$PWD/../../qgv/QGVEdge.h \
    $$PWD/../../qgv/QGVNode.h \
    $$PWD/../../qgv/QGVScene.h \
    $$PWD/../../qgv/QGVSubGraph.h

SOURCES += \
    $$PWD/../../qgv/private/QGVCore.cpp \
    $$PWD/../../qgv/private/QGVEdgePrivate.cpp \
    $$PWD/../../qgv/private/QGVGraphPrivate.cpp \
    $$PWD/../../qgv/private/QGVGvcPrivate.cpp \
    $$PWD/../../qgv/private/QGVNodePrivate.cpp \
    $$PWD/../../qgv/QGVEdge.cpp \
    $$PWD/../../qgv/QGVNode.cpp \
    $$PWD/../../qgv/QGVScene.cpp \
    $$PWD/../../qgv/QGVSubGraph.cpp









