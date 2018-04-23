################################################################
# Qwt Widget Library
# Copyright (C) 1997   Josef Wilgen
# Copyright (C) 2002   Uwe Rathmann
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Qwt License, Version 1.0
###################################################################

QWT_ROOT = $${PWD}/
include( $${QWT_ROOT}/qwtconfig.pri )
include( $${QWT_ROOT}/qwtbuild.pri )
#QWT_ROOT = $${PWD}/
#include( ./qwtconfig.pri )
#include( ./qwtbuild.pri )

TEMPLATE     = app

INCLUDEPATH += C:/Qt/qwt-6.0.1/src
#$${QWT_ROOT}/../../src
DEPENDPATH  += C:/Qt/qwt-6.0.1/src
#$${QWT_ROOT}/../../src
#DESTDIR      = $${QWT_ROOT}/bin11

QMAKE_RPATHDIR *= C:/Qt/qwt-6.0.1/lib
#$${QWT_ROOT}/../../lib

contains(QWT_CONFIG, QwtFramework) {

    LIBS      += -FC:/Qt/qwt-6.0.1/lib#$${QWT_ROOT}/../../lib
}
else {

    LIBS      += -LC:/Qt/qwt-6.0.1/lib#$${QWT_ROOT}/../../lib
}

IPATH       = $${INCLUDEPATH}
qtAddLibrary(qwt)
INCLUDEPATH = $${IPATH}

contains(QWT_CONFIG, QwtSvg) {

    QT += svg
}
else {

    DEFINES += QWT_NO_SVG
}


win32 {
    contains(QWT_CONFIG, QwtDll) {
        DEFINES    += QT_DLL QWT_DLL
    }
}
