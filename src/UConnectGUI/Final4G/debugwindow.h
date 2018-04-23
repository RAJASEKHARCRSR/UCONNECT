#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <QMainWindow>
#include <vector>
#include <QTreeWidget>
#include <QTimer>
#include <QMessageBox>
#include <iostream>
#include <QString>
#include <fstream>
using namespace std;

#include "UIEntry.h"

extern U_UINT8 au8DataFromDevice[];

namespace Ui {
    class DebugWindow;
}

class DebugWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DebugWindow(QWidget *parent = 0);
    void init();
    //void updateValues();
    ~DebugWindow();

protected:
    vector<UIEntry> m_tGeneral;
    vector<UIEntry> m_tMAC;
    vector<UIEntry> m_tPHY;

    QTimer *m_tRefreshTimer;
    void UpdateColumnValues(QTreeWidget*& tWidget, U_UINT8*& pCurrPointer, int& nCurrIndex);
    void InitTreeWidget(QTreeWidget*& pWidget, QString strCol1Desc, QString strCol2Desc);
    void AddWidgetItem(QTreeWidget*& pWidget, QString strName, U_UINT32 u32Value);

private:
    Ui::DebugWindow *ui;

private slots:
    void on_m_editRefreshInterval_textChanged(QString );
    void on_checkBox_toggled(bool checked);
    void updateValues();
};

#endif // DEBUGWINDOW_H
