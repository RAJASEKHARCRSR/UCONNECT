/*****************************************************************************
**
**  main.cpp
**
**	This file contains the definitions of the enums,
      structures and functions used in Uconnect GUI
**
**	Authors: Suresh Lingadalli & Ramya.B
**	Date  : 15-MAR-2011
**  (c) United Mobile Apps Pvt Ltd - (2010 - 2015)

    VER		  DATE		AUTHOR		DESCRIPTION
    0.1		15/03/10       RB		Creation

***************************************************************************/
#include <QtGui/QApplication>
#include "uconnect.h"
#include <QDesktopServices>
#include <QUrl>


Uconnect *gUConnect;
U_UINT16 serverPort = 55555;


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);



    if(argc == 3)
    {
        ////qDebug() << "port:" << argv[1] << argv[2];

        if(!argv[2] || (U_SINT8)(*argv[2]) == '-')
        {
            ////qDebug() << "Port not specified, Hence taking default port as 55555";
        }
        else
        {
            serverPort = (U_UINT16)atoi((const char*)argv[2]);
            ////qDebug() << "serverPort:" << serverPort;
        }
    }





    //QDesktopServices::openUrl(QUrl("file:///CMsocketservice.exe"));
    //QString path = QApplication::applicationDirPath() + "/CMsocketservice.exe";
    //QDesktopServices::openUrl(QUrl::fromLocalFile(path));



    QFile file(":/stylesheet");
     file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    app.setStyleSheet(styleSheet);
    file.close();


    QSharedMemory sharedMemory;
    sharedMemory.setKey("UCONNECT1");
    if (sharedMemory.create(sizeof(qint32), QSharedMemory::ReadWrite))
    {
        gUConnect = new Uconnect();
        gUConnect->init();
        return app.exec();
    }
    else
          exit(0);

}
