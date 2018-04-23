MACHINE= $(shell uname -s)


CFLAGS?= -DUCONN_CLIENT_USE_SOCKETS -w

ifeq ($(MACHINE),Darwin)
  CCLINK?= -lpthread 
else
 CCLINK?= -lpthread -lrt 
endif   

DEBUG?= -g -rdynamic -ggdb 
CCOPT= $(CFLAGS)  $(ARCH) $(PROF)
CC =g++
AR = ar rcs
SLIB_NAME=cmserver
SOCKETSERVICE_EXENAME = cmsocketservice.out
CLI_EXENAME = cmcli.out
OBJDIR = build

CMSERVER_OBJLIB := $(addprefix $(OBJDIR)/,uconn_OSAL.o  ULog.o   uconn_ConnectedState.o  uconn_ConnectingState.o \
       uconn_DisabledState.o uconn_DisconnectingState.o uconn_EnabledState.o uconn_IdleState.o   uconn_SearchingState.o \
       uconn_server.o  uconn_ServerAPI.o uconn_StateMachine.o UStateMachine.o uconn_AT_DAL.o UBufferParser.o uconn_ATHandler.o UEmulator.o UEmulateSerialPort.o uconn_Broadcom_DAL.o uconn_Altair_DAL.o UBroadcomParser.o UAltairParser.o UCommonFunctions.o LTE_NM.o NetworkManager.o ThreeG_NM.o UClientProcess.o XMLParser.o BearerManager.o Three_g_dal.o ThreeGParser.o UVpn.o WiFi_NM.o uWlanHlpr.o uWlanUtils.o USrPortCommUsage.o UCommonUtils.o uconn_Huawei_Dal.o UHuawei4GParser.o )

CLI_OBJ := $(addprefix $(OBJDIR)/,uconn_SAPIwrapper.o  Uconn_cliMain.o  )

SS_OBJ := $(addprefix $(OBJDIR)/,cm_server.o  )

vpath %.cpp OSAL/source \
            common/source \
            cm_server/source \
            NetworkManager/source/ \
            DAL/source devices/device_emulator/source/ \
            cm_client/source/ \
            CMsocketservice/source 
            
            

#SOCKETSERVICE= CMsocketservice/source/cm_server.cpp

#CLI=	cm_client/source/uconn_SAPIwrapper.cpp \
#	cm_client/source/Uconn_cliMain.cpp

INC  = -IOSAL/include \
       -Icm_server/include \
      -Idevices/broadcom/device_emulator/include \
      -Icommon/include \
      -Icm_server/include \
      -IDAL/include \
      -INetworkManager/include \
      -ICMsocketservice/include \
      -Icm_client/include \
      -Idevices/device_emulator/include 
      
      
      

subsystem:

	 make all

$(CMSERVER_OBJLIB) : |$(OBJDIR)
$(OBJDIR):
	 mkdir $(OBJDIR) 
	 cp CMsocketservice/configFile.xml $(OBJDIR) 


$(OBJDIR)/%.o:%.cpp 
	$(CC) -c  $(INC) $(CCOPT) $(DEBUG) $(CCLINK) $< -o $@
    

all:  $(CMSERVER_OBJLIB) $(CLI_OBJ) $(SS_OBJ)
	$(AR)  lib$(SLIB_NAME).a $(CMSERVER_OBJLIB)
	$(CC)  $(INC) $(CCOPT) $(SS_OBJ) $(DEBUG) $(CCLINK) -l$(SLIB_NAME) -L ./ -o $(OBJDIR)/$(SOCKETSERVICE_EXENAME)
	$(CC)  $(INC) $(CCOPT) $(CLI_OBJ) $(CCLINK) -l$(SLIB_NAME) -L ./ -o $(OBJDIR)/$(CLI_EXENAME) 

	
clean:
	rm -rf $(OBJDIR)/*
	rmdir $(OBJDIR)
dep:
	$(CC) -MM *.cpp

