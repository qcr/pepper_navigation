#-------------------------------------------------
#
# Project created by QtCreator 2013-04-17T09:06:06
#
#-------------------------------------------------

QT       += core

QT += widgets


TARGET = QGVCore
TEMPLATE = lib
CONFIG += shared

 DEFINES += WITH_CGRAPH
 INCLUDEPATH += private
 CONFIG += link_pkgconfig
 PKGCONFIG += libcdt libgvc libcgraph

SOURCES += QGVScene.cpp \
    QGVNode.cpp \
    QGVEdge.cpp \
    QGVSubGraph.cpp \
		private/QGVCore.cpp \
		private/QGVGraphPrivate.cpp \
		private/QGVGvcPrivate.cpp \
		private/QGVEdgePrivate.cpp \
		private/QGVNodePrivate.cpp

HEADERS  += QGVScene.h \
    QGVNode.h \
    QGVEdge.h \
    QGVSubGraph.h \
		private/QGVCore.h \
		private/QGVGraphPrivate.h \
		private/QGVGvcPrivate.h \
		private/QGVEdgePrivate.h \
		private/QGVNodePrivate.h \
    qgv.h
