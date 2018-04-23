#-------------------------------------------------
#
# Project created by QtCreator 2011-03-23T14:17:15
#
#-------------------------------------------------
include( $${PWD}/qwtconfig.pri )
include( $${PWD}/qwtbuild.pri )


QT       += core gui

TARGET = UConnectGUI
TEMPLATE = app

CONFIG += qwt

SOURCES += main.cpp\
        uconnect.cpp \
        ../OSAL/source/uconn_OSAL.cpp \
        ../cm_client/source/uconn_SAPIwrapper.cpp \
    ../common/source/ULog.cpp \
    CMGraphStats.cpp \
    minview.cpp \
    detailedview.cpp

HEADERS  += uconnect.h \
    ../OSAL/include/uconn_OSAL.h \
    ../common/include/uconn_SAPIwrapper.h \
    ../common/include/UTypes.h \
    ../common/include/ULog.h \
    ../common/include/uconn_Event.h \
    ../common/include/uconn_Config.h \
    ../cm_server/include/uconn_ServerAPI.h \
    ../devices/broadcom/device_emulator/UEmulateSerialPort.h \
    ../DAL/include/uconn_Broadcom_DAL.h \
    CMGraphStats.h \
    minview.h \
    detailedview.h

FORMS    += uconnect.ui\
    detailedview.ui \
    minview.ui
   

OTHER_FILES +=

RESOURCES += \
    altair.qrc


INCLUDEPATH += ../OSAL/include \
    ../common/include \
    ../cm_client/include\
    ../cm_server/include\
    ../DAL/include\
    ../devices/broadcom/device_emulator/include\
    ../devices/broadcom/include\
    $${QWT_INSTALL_HEADERS}
    #../DAL/include\
    #../devices/broadcom/include\
    #../devices/broadcom/device_emulator\
    #D:/may2/UConnectGUI/Final4G \




DEFINES +=  ALTAIR \
            QTPROJECT \
            UCONN_CLIENT_USE_SOCKETS




