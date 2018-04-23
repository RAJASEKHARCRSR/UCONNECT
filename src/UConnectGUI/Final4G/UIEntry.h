#ifndef UIENTRY_H
#define UIENTRY_H

#include <QString>

typedef unsigned char   U_UINT8;
typedef unsigned int    U_UINT32;
typedef U_UINT32*       U_PUINT32;



struct UIEntry {
    QString     tName;
    U_UINT32    u32Value;
};

#endif // UIENTRY_H
