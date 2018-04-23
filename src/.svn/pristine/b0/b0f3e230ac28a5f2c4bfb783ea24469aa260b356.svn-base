/*****************************************************************************
**
**   debugwindow.cpp
**      This file contains the implementation of the debugger
**
**	- first a handler is written to read the config file and
**          three vectors General, MAC and PHY are created
**	- Write a handlers to add the header, parameters and values.
**      - Write a handler for refresh enable.
**      - Handler to update the values when timer expires.
**
**	Author: Sriram V Iyer
**              Ramya.B
**	Date  : 04-MARCH-2011
**  (c) United Mobile Apps Pvt Ltd

    VER		  DATE		AUTHOR		DESCRIPTION

*****************************************************************************/
#include "debugwindow.h"
#include "ui_debugwindow.h"
using namespace std;


DebugWindow::DebugWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DebugWindow)

{
    ui->setupUi(this);
}
/*******************************************************************************
  handler to read the data from device
  ******************************************************************************/
U_UINT8* ReadDataFromDevice()
{
    //GetValue(pHandle, STATISTICS_TYPE, STAT_SUB_TYPE, au8DataFromDevice);
    return au8DataFromDevice;
}
/*******************************************************************************
  Handler to update the values of different parameter to when the timer expires
  ******************************************************************************/
void DebugWindow::UpdateColumnValues(QTreeWidget*& pWidget, U_UINT8*& pCurrPointer, int& nCurrIndex)
{
      QTreeWidgetItemIterator it(pWidget);
    int value = 0;
    while (*it) {
        value = *(reinterpret_cast<U_PUINT32>(pCurrPointer + nCurrIndex));
        (*it)->setText(1, tr("%1").arg(value));
        nCurrIndex += 4;
        ++it;
    }
}
/********************************************************************************
 Function that calls update handler
 ********************************************************************************/
void DebugWindow::updateValues()
{
    U_UINT8* pDataFromDevice = ReadDataFromDevice();
    int curr_index = 0;

    UpdateColumnValues(ui->m_treeGenPara, pDataFromDevice, curr_index);
    UpdateColumnValues(ui->m_treeMACPara, pDataFromDevice, curr_index);
    UpdateColumnValues(ui->m_treePHYPara, pDataFromDevice, curr_index);
}
/**********************************************************************************
  Handler to add the column and headers
  ********************************************************************************/
void DebugWindow::InitTreeWidget(QTreeWidget*& pWidget, QString strCol1Desc, QString strCol2Desc)
{
    pWidget->setColumnCount(2);
    pWidget->setColumnWidth(0, 300);
    pWidget->setColumnWidth(1, 100);

    QStringList header;
    header << strCol1Desc << strCol2Desc;
    pWidget->setHeaderLabels(header);

}
/************************************************************************************
  Handler to add the parameter and value to widget
  ***********************************************************************************/
void DebugWindow::AddWidgetItem(QTreeWidget*& pWidget, QString strName, U_UINT32 u32Value)
{
    QTreeWidgetItem *pItem = new QTreeWidgetItem(pWidget);
    pItem->setText(0,strName);
    pItem->setText(1, tr("%1").arg(u32Value));
}
/**************************************************************************************
  - Function that calls the handler to add the column and headers
  - to read the parameter into different vectors and add it to the widget.
  ***********************************************************************************/
void DebugWindow::init()
{
    InitTreeWidget(ui->m_treeGenPara, tr("General Parameters"), tr("Values") );
    InitTreeWidget(ui->m_treeMACPara, tr("MAC Parameters"), tr("Values") );
    InitTreeWidget(ui->m_treePHYPara, tr("PHY Parameters"), tr("Values") );

    FILE* f;
    char configparams [50];
    vector<QString> lines;
    f = fopen ("ui.cfg" , "r");

       if (f == NULL)
           perror ("Error opening file");
       else   
       {
          while(( fgets (configparams , 50 , f))!= NULL)
           {
           cout<<configparams<<endl;
           QString qstr(configparams);
           lines.push_back( qstr );
           }
       }
   vector<QString>::iterator it;

   int curr_index = 0;

   for(it = lines.begin(); it != lines.end(); it++) {
       QString entry = *it;
       vector<UIEntry>* curr_vector;

       entry = entry.trimmed();
       if( entry.indexOf("@") != -1 ) {
           // This is a  tab
           if(entry == QObject::tr("@General")) {
               curr_vector = &m_tGeneral;
           }
           else if( entry == QObject::tr("@MAC")) {
               curr_vector = &m_tMAC;
           }
           else if(entry == QObject::tr("@PHY")) {
               curr_vector = &m_tPHY;
           }
           continue;
       }

       if( entry.length() != 0) {
           UIEntry uiEntry;
           uiEntry.tName = entry;
           uiEntry.u32Value = *reinterpret_cast<U_PUINT32>(ReadDataFromDevice() + curr_index);
           curr_index += 4;

           curr_vector->push_back( uiEntry );
       }
   }
  /************************************************************************************
     Here the parameters and there value are added to the widget
   ****************************************************************************/
   for(U_UINT32 i = 0; i < m_tGeneral.size(); i++) {
       AddWidgetItem(ui->m_treeGenPara, m_tGeneral[i].tName, m_tGeneral[i].u32Value );
   }

   for(U_UINT32 i = 0; i < m_tMAC.size(); i++) {
       AddWidgetItem(ui->m_treeMACPara, m_tMAC[i].tName, m_tMAC[i].u32Value );
   }

   for(U_UINT32 i = 0; i < m_tPHY.size(); i++) {
       AddWidgetItem(ui->m_treePHYPara, m_tPHY[i].tName, m_tPHY[i].u32Value );
   }

   m_tRefreshTimer =  new QTimer(this);
   connect(m_tRefreshTimer, SIGNAL(timeout()), this, SLOT(updateValues()));
}

DebugWindow::~DebugWindow()
{
    delete ui;
}
/********************************************************************************
     Handler that handles the timer when the refresh checkbox is enabled
 *******************************************************************************/
void DebugWindow::on_checkBox_toggled(bool checked)
{
    // If refresh is enabled, run timer
    if(checked) {
        bool ok;
        QString time = ui->m_editRefreshInterval->text();
        m_tRefreshTimer->start(time.toUInt(&ok, 10) * 1000);
    }
    else {
        m_tRefreshTimer->stop();
    }
}

void DebugWindow::on_m_editRefreshInterval_textChanged(QString )
{   //default timer takes 1second
    if(m_tRefreshTimer) {
        m_tRefreshTimer->stop();
    }
    QString time = ui->m_editRefreshInterval->text();
    bool ok;
    m_tRefreshTimer->start(time.toUInt(&ok, 10) * 1000);
}
