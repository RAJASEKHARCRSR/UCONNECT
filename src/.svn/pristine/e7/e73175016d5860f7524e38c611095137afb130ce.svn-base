
#include <windows.h>
#include <devioctl.h>
#include <ntdddisk.h>
#include <ntddscsi.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <strsafe.h>
#include <intsafe.h>
#include <string.h>
#include "spti.h"

#include <SetupAPI.h> 
#include <cfgmgr32.h> 
#include <winioctl.h> 
#pragma comment(lib,"SetupApi.lib")
#define NAME_COUNT  25

#define BOOLEAN_TO_STRING(_b_) \
( (_b_) ? "True" : "False" )

#if defined(_X86_)
    #define PAGE_SIZE  0x1000
    #define PAGE_SHIFT 12L
#elif defined(_AMD64_)
    #define PAGE_SIZE  0x1000
    #define PAGE_SHIFT 12L
#elif defined(_IA64_)
    #define PAGE_SIZE 0x2000
    #define PAGE_SHIFT 13L
#else
    // undefined platform?
    #define PAGE_SIZE  0x1000
    #define PAGE_SHIFT 12L
#endif


LPCSTR BusTypeStrings[] = {
    "Unknown",
    "Scsi",
    "Atapi",
    "Ata",
    "1394",
    "Ssa",
    "Fibre",
    "Usb",
    "RAID",
    "Not Defined",
};
#define NUMBER_OF_BUS_TYPE_STRINGS (sizeof(BusTypeStrings)/sizeof(BusTypeStrings[0]))




// Finds the device interface for the CDROM drive with the given interface number. 
DEVINST GetDrivesDevInstByDeviceNumber(long DeviceNumber) 
{ 
    const GUID *guid = &GUID_DEVINTERFACE_CDROM; 
 
// Get device interface info set handle 
// for all devices attached to system 
    HDEVINFO hDevInfo = SetupDiGetClassDevs(guid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE); 
    if(hDevInfo == INVALID_HANDLE_VALUE) 
        return 0; 
 
// Retrieve a context structure for a device interface of a device information set. 
    BYTE                             buf[1024]; 
    PSP_DEVICE_INTERFACE_DETAIL_DATA pspdidd = (PSP_DEVICE_INTERFACE_DETAIL_DATA)buf; 
    SP_DEVICE_INTERFACE_DATA         spdid; 
    SP_DEVINFO_DATA                  spdd; 
    DWORD                            dwSize; 
 
    spdid.cbSize = sizeof(spdid); 
 
// Iterate through all the interfaces and try to match one based on 
// the device number. 
    for(DWORD i = 0; SetupDiEnumDeviceInterfaces(hDevInfo, NULL,guid, i, &spdid); i++) 
    { 
    // Get the device path. 
        dwSize = 0; 
        SetupDiGetDeviceInterfaceDetail(hDevInfo, &spdid, NULL, 0, &dwSize, NULL); 
        if(dwSize == 0 || dwSize > sizeof(buf)) 
            continue; 
 
        pspdidd->cbSize = sizeof(*pspdidd); 
        ZeroMemory((PVOID)&spdd, sizeof(spdd)); 
        spdd.cbSize = sizeof(spdd); 
        if(!SetupDiGetDeviceInterfaceDetail(hDevInfo, &spdid, pspdidd, 
                                            dwSize, &dwSize, &spdd)) 
            continue; 
 

    // Open the device. 
        HANDLE hDrive = CreateFile(pspdidd->DevicePath,0, 
                                   FILE_SHARE_READ | FILE_SHARE_WRITE, 
                                   NULL, OPEN_EXISTING, 0, NULL); 
		
        if(hDrive == INVALID_HANDLE_VALUE) 
            continue; 
 
    // Get the device number. 
        STORAGE_DEVICE_NUMBER sdn; 
        dwSize = 0; 
        if(DeviceIoControl(hDrive, 
                           IOCTL_STORAGE_GET_DEVICE_NUMBER, 
                           NULL, 0, &sdn, sizeof(sdn), 
                           &dwSize, NULL)) 
        { 
        // Does it match? 
            if(DeviceNumber == (long)sdn.DeviceNumber) 
            { 
                CloseHandle(hDrive); 
                SetupDiDestroyDeviceInfoList(hDevInfo); 
                return spdd.DevInst; 
            } 
        } 
        CloseHandle(hDrive); 
    } 
 
    SetupDiDestroyDeviceInfoList(hDevInfo); 
    return 0; 
} 
 
 
// Returns true if the given device instance belongs to the USB device with the given VID and PID. 
bool matchDevInstToUsbDevice(DEVINST device, char* deviceName) 
{ 
// This is the string we will be searching for in the device harware IDs. 
   // TCHAR hwid[64]; 
    
 
// Get a list of hardware IDs for all USB devices. 
    ULONG ulLen; 
    CM_Get_Device_ID_List_Size(&ulLen, NULL, CM_GETIDLIST_FILTER_NONE); 
    TCHAR *pszBuffer = new TCHAR[ulLen]; 
    CM_Get_Device_ID_List(NULL, pszBuffer, ulLen, CM_GETIDLIST_FILTER_NONE); 
 
// Iterate through the list looking for our ID. 
    for(TCHAR* pszDeviceID = pszBuffer; *pszDeviceID; pszDeviceID += strlen(pszDeviceID) + 1) 
    { 
    // Some versions of Windows have the string in upper case and other versions have it 
    // in lower case so just make it all upper. 
        for(int i = 0; pszDeviceID[i]; i++) 
            pszDeviceID[i] = toupper(pszDeviceID[i]); 
 
        if(strstr(pszDeviceID, deviceName)) 
        { 
        // Found the device, now we want the grandchild device, which is the "generic volume" 
            DEVINST MSDInst = 0; 
            if(CR_SUCCESS == CM_Locate_DevNode(&MSDInst, pszDeviceID, CM_LOCATE_DEVNODE_NORMAL)) 
            { 
                DEVINST DiskDriveInst = 0; 
                if(CR_SUCCESS == CM_Get_Child(&DiskDriveInst, MSDInst, 0)) 
                { 
                // Now compare the grandchild node against the given device instance. 
                    if(device == DiskDriveInst) 
                        return true; 
                } 
            } 
        } 
    } 
 
    return false; 
} 
 

 // Find a USB device by it's Vendor and Product IDs. When found, eject it. 
HANDLE findDevice(char* deviceName) 
{ 
    TCHAR devicepath[8]; 
    strcpy(devicepath, "\\\\.\\?:"); 
 
    TCHAR drivepath[4]; 
    strcpy(drivepath, "?:\\"); 
 
// Iterate through every drive letter and check if it is our device. 
    for(TCHAR driveletter = 'A'; driveletter <= 'Z'; driveletter++) 
    { 
    // We are only interested in CDROM drives. 
        drivepath[0] = driveletter; 
        if(DRIVE_CDROM != GetDriveType(drivepath)) 
            continue; 
 
    // Get the "storage device number" for the current drive. 
        long DeviceNumber = -1; 
        devicepath[4]     = driveletter; 
        HANDLE hVolume    = CreateFile(devicepath, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 
                                        NULL, OPEN_EXISTING, 0, NULL); 

		
		if(INVALID_HANDLE_VALUE == hVolume) 
            continue; 
 
        STORAGE_DEVICE_NUMBER sdn; 
        DWORD dwBytesReturned = 0; 
        if(DeviceIoControl(hVolume, IOCTL_STORAGE_GET_DEVICE_NUMBER, 
                            NULL, 0, &sdn, sizeof(sdn), &dwBytesReturned, NULL)) 
            DeviceNumber = sdn.DeviceNumber; 
        
		if(DeviceNumber < 0) {
			CloseHandle(hVolume);         
			continue; 
		}
 
    // Use the data we have collected so far on our drive to find a device instance. 
        DEVINST DevInst = GetDrivesDevInstByDeviceNumber(DeviceNumber); 
 
    // If the device instance corresponds to the USB device we are looking for, eject it. 
        if(DevInst) 
        { 
            if(matchDevInstToUsbDevice(DevInst, deviceName)) 
               return hVolume; 
        }  
		CloseHandle(hVolume);  
    } 
} 



VOID
__cdecl
modeSwitch(
    char* deviceName, char* switchPacket
    )

{
    BOOL status = 0;
    DWORD accessMode = 0, shareMode = 0;
    HANDLE fileHandle = NULL;
    ULONG alignmentMask = 0; // default == no alignment requirement
    PUCHAR dataBuffer = NULL;
    PUCHAR pUnAlignedBuffer = NULL;
    SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER sptdwb;

    ULONG length = 0,
          errorCode = 0,
          returned = 0,
          sectorSize = 512;




	
    shareMode = FILE_SHARE_READ|FILE_SHARE_WRITE;  // default
    accessMode = GENERIC_WRITE | GENERIC_READ;       // default

	
	if(strstr(deviceName,"CdRom")){
	fileHandle = CreateFile(deviceName,
       accessMode,
       shareMode,
       NULL,
       OPEN_EXISTING,
       0,
       NULL);
}
else{

	fileHandle = findDevice(deviceName);
}
	
	if (fileHandle == INVALID_HANDLE_VALUE) {
        errorCode = GetLastError();
        printf("Error opening %s. Error: %d\n",
               deviceName, errorCode);
        PrintError(errorCode);
        return;
    }

    //
    // Get the alignment requirements
    //

    status = GetAlignmentMaskForDevice(fileHandle, &alignmentMask);
    if (!status ) {
        errorCode = GetLastError();
        printf("Error getting device and/or adapter properties; "
               "error was %d\n", errorCode);
        PrintError(errorCode);
        CloseHandle(fileHandle);
        return;
    }


    ZeroMemory(&sptdwb, sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER));
    dataBuffer = AllocateAlignedBuffer(100,alignmentMask, &pUnAlignedBuffer);
	//UCHAR modeswitch[]={
	//0x55 ,0x53 ,0x42 ,0x43 ,0x10 ,0xF0 ,0x53 ,0x04 ,0x00 ,0x08 ,0x00 ,0x00 ,0x80 ,0x00 
	//,0x0A ,0x28 ,0x00 ,0x00 ,0x00 ,0x00 ,0x40 ,0x00 ,0x00 ,0x04 ,0x00 ,0x00 ,0x00 ,0x00
	//,0x00 ,0x00 ,0x00};

	//UCHAR modeswitch[]={
	//0x55 ,0x53 ,0x42 ,0x43 ,0x10 ,0x00 ,0x50 ,0x04 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 
	//,0x03 ,0xf0 ,0x01 ,0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
	//,0x00 ,0x00 ,0x00};				// 0xf0 Command

	//UCHAR modeswitch[]={
	//0x55 ,0x53 ,0x42 ,0x43 ,0xb0 ,0x96 ,0xd4 ,0x06 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 
	//,0x06 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
	//,0x00 ,0x00 ,0x00};

	//memcpy((char *)dataBuffer,(char *)modeswitch,31);

 //   sptdwb.sptd.Length = sizeof(SCSI_PASS_THROUGH_DIRECT);
 //   sptdwb.sptd.PathId = 0;
 //   sptdwb.sptd.TargetId = 1;
 //   sptdwb.sptd.Lun = 0;
 //   sptdwb.sptd.CdbLength = CDB10GENERIC_LENGTH;
 //   sptdwb.sptd.SenseInfoLength = SPT_SENSE_LENGTH;
 //   sptdwb.sptd.DataIn = SCSI_IOCTL_DATA_OUT;
 //   sptdwb.sptd.DataTransferLength = 31;
 //   sptdwb.sptd.TimeOutValue = 2;
	//sptdwb.sptd.DataBuffer = dataBuffer;
 //   sptdwb.sptd.SenseInfoOffset =
 //      offsetof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER,ucSenseBuf);
 //   sptdwb.sptd.Cdb[0] = SCSIOP_WRITE_DATA_BUFF;
 //   sptdwb.sptd.Cdb[1] = 2;                         // Data mode
 //  // sptdwb.sptd.Cdb[7] = (UCHAR)(31 >> 8);  // Parameter List length
 //  // sptdwb.sptd.Cdb[8] = 0;
 //   length = sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER);
 //   status = DeviceIoControl(fileHandle,
 //                            IOCTL_SCSI_PASS_THROUGH_DIRECT,
 //                            &sptdwb,
 //                            length,
 //                            &sptdwb,
 //                            length,
 //                            &returned,
 //                            FALSE);

 //   PrintStatusResults(status,returned,
 //      (PSCSI_PASS_THROUGH_WITH_BUFFERS)&sptdwb,length);
 //   if ((sptdwb.sptd.ScsiStatus == 0) && (status != 0)) {
 //      PrintDataBuffer(dataBuffer,sptdwb.sptd.DataTransferLength);
 //      }

	/* Send the proprietary MODE SWITCH USB command */

	//UCHAR modeswitch1[]={
	//0x55 ,0x53 ,0x42 ,0x43 ,0x40 ,0xd4 ,0x87 ,0x04 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 
	//,0x03 ,0xf0 ,0x01 ,0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
	//,0x00 ,0x00 ,0x00};

	//memcpy((char *)dataBuffer,(char *)modeswitch1,31);

    sptdwb.sptd.Length = sizeof(SCSI_PASS_THROUGH_DIRECT);
    sptdwb.sptd.PathId = 0;
    sptdwb.sptd.TargetId = 1;
    sptdwb.sptd.Lun = 0;
    sptdwb.sptd.CdbLength = CDB10GENERIC_LENGTH;
    sptdwb.sptd.SenseInfoLength = SPT_SENSE_LENGTH;
    sptdwb.sptd.DataIn = SCSI_IOCTL_DATA_OUT;
    sptdwb.sptd.DataTransferLength = 31;
    sptdwb.sptd.TimeOutValue = 2;
	sptdwb.sptd.DataBuffer = switchPacket;
    sptdwb.sptd.SenseInfoOffset =
       offsetof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER,ucSenseBuf);
    sptdwb.sptd.Cdb[0] = SCSIOP_WRITE_DATA_BUFF;
    sptdwb.sptd.Cdb[1] = 2;                         // Data mode
   // sptdwb.sptd.Cdb[7] = (UCHAR)(31 >> 8);  // Parameter List length
   // sptdwb.sptd.Cdb[8] = 0;
    length = sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_BUFFER);
    status = DeviceIoControl(fileHandle,
                             IOCTL_SCSI_PASS_THROUGH_DIRECT,
                             &sptdwb,
                             length,
                             &sptdwb,
                             length,
                             &returned,
                             FALSE);

    PrintStatusResults(status,returned,
       (PSCSI_PASS_THROUGH_WITH_BUFFERS)&sptdwb,length);
    if ((sptdwb.sptd.ScsiStatus == 0) && (status != 0)) {
       PrintDataBuffer(dataBuffer,sptdwb.sptd.DataTransferLength);
       }

    if (pUnAlignedBuffer != NULL) {
        free(pUnAlignedBuffer);
    }
    CloseHandle(fileHandle);
}

VOID
PrintError(ULONG ErrorCode)
{
    CHAR errorBuffer[80];
    ULONG count;

    count = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL,
                  ErrorCode,
                  0,
                  errorBuffer,
                  sizeof(errorBuffer),
                  NULL
                  );

    if (count != 0) {
        printf("%s\n", errorBuffer);
    } else {
        printf("Format message failed.  Error: %d\n", GetLastError());
    }
}

VOID
PrintDataBuffer(PUCHAR DataBuffer, ULONG BufferLength)
{
    ULONG Cnt;

    printf("      00  01  02  03  04  05  06  07   08  09  0A  0B  0C  0D  0E  0F\n");
    printf("      ---------------------------------------------------------------\n");
    for (Cnt = 0; Cnt < BufferLength; Cnt++) {
       if ((Cnt) % 16 == 0) {
          printf(" %03X  ",Cnt);
          }
       printf("%02X  ", DataBuffer[Cnt]);
       if ((Cnt+1) % 8 == 0) {
          printf(" ");
          }
       if ((Cnt+1) % 16 == 0) {
          printf("\n");
          }
       }
    printf("\n\n");
}

VOID
PrintAdapterDescriptor(PSTORAGE_ADAPTER_DESCRIPTOR AdapterDescriptor)
{
    ULONG trueMaximumTransferLength;
    LPCSTR busType;

    if (AdapterDescriptor->BusType < NUMBER_OF_BUS_TYPE_STRINGS) {
        busType = BusTypeStrings[AdapterDescriptor->BusType];
    } else {
        busType = BusTypeStrings[NUMBER_OF_BUS_TYPE_STRINGS-1];
    }

    // subtract one page, as transfers do not always start on a page boundary
    if (AdapterDescriptor->MaximumPhysicalPages > 1) {
        trueMaximumTransferLength = AdapterDescriptor->MaximumPhysicalPages - 1;
    } else {
        trueMaximumTransferLength = 1;
    }
    // make it into a byte value
    trueMaximumTransferLength <<= PAGE_SHIFT;

    // take the minimum of the two
    if (trueMaximumTransferLength > AdapterDescriptor->MaximumTransferLength) {
        trueMaximumTransferLength = AdapterDescriptor->MaximumTransferLength;
    }

    // always allow at least a single page transfer
    if (trueMaximumTransferLength < PAGE_SIZE) {
        trueMaximumTransferLength = PAGE_SIZE;
    }

    //puts("\n            ***** STORAGE ADAPTER DESCRIPTOR DATA *****");
    printf("              Version: %08x\n"
           "            TotalSize: %08x\n"
           "MaximumTransferLength: %08x (bytes)\n"
           " MaximumPhysicalPages: %08x\n"
           "  TrueMaximumTransfer: %08x (bytes)\n"
           "        AlignmentMask: %08x\n"
           "       AdapterUsesPio: %s\n"
           "     AdapterScansDown: %s\n"
           "      CommandQueueing: %s\n"
           "  AcceleratedTransfer: %s\n"
           "             Bus Type: %s\n"
           "    Bus Major Version: %04x\n"
           "    Bus Minor Version: %04x\n",
           AdapterDescriptor->Version,
           AdapterDescriptor->Size,
           AdapterDescriptor->MaximumTransferLength,
           AdapterDescriptor->MaximumPhysicalPages,
           trueMaximumTransferLength,
           AdapterDescriptor->AlignmentMask,
           BOOLEAN_TO_STRING(AdapterDescriptor->AdapterUsesPio),
           BOOLEAN_TO_STRING(AdapterDescriptor->AdapterScansDown),
           BOOLEAN_TO_STRING(AdapterDescriptor->CommandQueueing),
           BOOLEAN_TO_STRING(AdapterDescriptor->AcceleratedTransfer),
           busType,
           AdapterDescriptor->BusMajorVersion,
           AdapterDescriptor->BusMinorVersion);




    printf("\n\n");
}

VOID
PrintDeviceDescriptor(PSTORAGE_DEVICE_DESCRIPTOR DeviceDescriptor)
{
    LPCSTR vendorId = "";
    LPCSTR productId = "";
    LPCSTR productRevision = "";
    LPCSTR serialNumber = "";
    LPCSTR busType;

    if (DeviceDescriptor->BusType < NUMBER_OF_BUS_TYPE_STRINGS) {
        busType = BusTypeStrings[DeviceDescriptor->BusType];
    } else {
        busType = BusTypeStrings[NUMBER_OF_BUS_TYPE_STRINGS-1];
    }

    if ((DeviceDescriptor->ProductIdOffset != 0) &&
        (DeviceDescriptor->ProductIdOffset != -1)) {
        productId        = (LPCSTR)(DeviceDescriptor);
        productId       += (ULONG_PTR)DeviceDescriptor->ProductIdOffset;
    }
    if ((DeviceDescriptor->VendorIdOffset != 0) &&
        (DeviceDescriptor->VendorIdOffset != -1)) {
        vendorId         = (LPCSTR)(DeviceDescriptor);
        vendorId        += (ULONG_PTR)DeviceDescriptor->VendorIdOffset;
    }
    if ((DeviceDescriptor->ProductRevisionOffset != 0) &&
        (DeviceDescriptor->ProductRevisionOffset != -1)) {
        productRevision  = (LPCSTR)(DeviceDescriptor);
        productRevision += (ULONG_PTR)DeviceDescriptor->ProductRevisionOffset;
    }
    if ((DeviceDescriptor->SerialNumberOffset != 0) &&
        (DeviceDescriptor->SerialNumberOffset != -1)) {
        serialNumber     = (LPCSTR)(DeviceDescriptor);
        serialNumber    += (ULONG_PTR)DeviceDescriptor->SerialNumberOffset;
    }


    //puts("\n            ***** STORAGE DEVICE DESCRIPTOR DATA *****");
    printf("              Version: %08x\n"
           "            TotalSize: %08x\n"
           "           DeviceType: %08x\n"
           "   DeviceTypeModifier: %08x\n"
           "       RemovableMedia: %s\n"
           "      CommandQueueing: %s\n"
           "            Vendor Id: %s\n"
           "           Product Id: %s\n"
           "     Product Revision: %s\n"
           "        Serial Number: %s\n"
           "             Bus Type: %s\n"
           "       Raw Properties: %s\n",
           DeviceDescriptor->Version,
           DeviceDescriptor->Size,
           DeviceDescriptor->DeviceType,
           DeviceDescriptor->DeviceTypeModifier,
           BOOLEAN_TO_STRING(DeviceDescriptor->RemovableMedia),
           BOOLEAN_TO_STRING(DeviceDescriptor->CommandQueueing),
           vendorId,
           productId,
           productRevision,
           serialNumber,
           busType,
           (DeviceDescriptor->RawPropertiesLength ? "Follows" : "None"));
    if (DeviceDescriptor->RawPropertiesLength != 0) {
        PrintDataBuffer(DeviceDescriptor->RawDeviceProperties,
                        DeviceDescriptor->RawPropertiesLength);
    }
    printf("\n\n");
}


PUCHAR
AllocateAlignedBuffer(ULONG size, ULONG AlignmentMask, PUCHAR *pUnAlignedBuffer)
{
    PUCHAR ptr;

    // NOTE: This routine does not allow for a way to free
    //       memory.  This is an excercise left for the reader.
    UINT_PTR    align64 = (UINT_PTR)AlignmentMask;

    if (AlignmentMask == 0) {
       ptr = (PUCHAR)malloc(size);
    } else {
       ULONG totalSize;

       ULongAdd(size, AlignmentMask, &totalSize);
       ptr = (PUCHAR)malloc(totalSize);
       *pUnAlignedBuffer = ptr;
       ptr = (PUCHAR)(((UINT_PTR)ptr + align64) & ~align64);
    }

    if (ptr == NULL) {
       printf("Memory allocation error.  Terminating program\n");
       exit(1);
    } else {
       return ptr;
    }
}

VOID
PrintStatusResults(
    BOOL status,DWORD returned,PSCSI_PASS_THROUGH_WITH_BUFFERS psptwb,
    ULONG length)
{
    ULONG errorCode;

    if (!status ) {
       printf( "Error: %d  ",
          errorCode = GetLastError() );
       PrintError(errorCode);
       return;
       }
    if (psptwb->spt.ScsiStatus) {
       PrintSenseInfo(psptwb);
       return;
       }
    else {
       printf("Scsi status: %02Xh, Bytes returned: %Xh, ",
          psptwb->spt.ScsiStatus,returned);
       printf("Data buffer length: %Xh\n\n\n",
          psptwb->spt.DataTransferLength);
       PrintDataBuffer((PUCHAR)psptwb,length);
       }
}

VOID
PrintSenseInfo(PSCSI_PASS_THROUGH_WITH_BUFFERS psptwb)
{
    int i;

    printf("Scsi status: %02Xh\n\n",psptwb->spt.ScsiStatus);
    if (psptwb->spt.SenseInfoLength == 0) {
       return;
       }
    printf("Sense Info -- consult SCSI spec for details\n");
    printf("-------------------------------------------------------------\n");
    for (i=0; i < psptwb->spt.SenseInfoLength; i++) {
       printf("%02X ",psptwb->ucSenseBuf[i]);
       }
    printf("\n\n");
}

BOOL
GetAlignmentMaskForDevice(
    IN HANDLE DeviceHandle,
    OUT PULONG AlignmentMask
    )
{
    PSTORAGE_ADAPTER_DESCRIPTOR adapterDescriptor = NULL;
    PSTORAGE_DEVICE_DESCRIPTOR deviceDescriptor = NULL;
    STORAGE_DESCRIPTOR_HEADER header = {0};

    BOOL ok = TRUE;
    BOOL failed = TRUE;
    ULONG i;

    *AlignmentMask = 0; // default to no alignment

    // Loop twice:
    //  First, get size required for storage adapter descriptor
    //  Second, allocate and retrieve storage adapter descriptor
    //  Third, get size required for storage device descriptor
    //  Fourth, allocate and retrieve storage device descriptor
    for (i=0;i<4;i++) {

        PVOID buffer = NULL;
        ULONG bufferSize = 0;
        ULONG returnedData;

        STORAGE_PROPERTY_QUERY query = {(STORAGE_PROPERTY_ID)0};

        switch(i) {
            case 0: {
                query.QueryType = PropertyStandardQuery;
                query.PropertyId = StorageAdapterProperty;
                bufferSize = sizeof(STORAGE_DESCRIPTOR_HEADER);
                buffer = &header;
                break;
            }
            case 1: {
                query.QueryType = PropertyStandardQuery;
                query.PropertyId = StorageAdapterProperty;
                bufferSize = header.Size;
                if (bufferSize != 0) {
                    adapterDescriptor = (PSTORAGE_ADAPTER_DESCRIPTOR)LocalAlloc(LPTR, bufferSize);
                    if (adapterDescriptor == NULL) {
                        goto Cleanup;
                    }
                }
                buffer = adapterDescriptor;
                break;
            }
            case 2: {
                query.QueryType = PropertyStandardQuery;
                query.PropertyId = StorageDeviceProperty;
                bufferSize = sizeof(STORAGE_DESCRIPTOR_HEADER);
                buffer = &header;
                break;
            }
            case 3: {
                query.QueryType = PropertyStandardQuery;
                query.PropertyId = StorageDeviceProperty;
                bufferSize = header.Size;

                if (bufferSize != 0) {
                    deviceDescriptor = (PSTORAGE_DEVICE_DESCRIPTOR)LocalAlloc(LPTR, bufferSize);
                    if (deviceDescriptor == NULL) {
                        goto Cleanup;
                    }
                }
                buffer = deviceDescriptor;
                break;
            }
        }

        // buffer can be NULL if the property queried DNE.
        if (buffer != NULL) {
            RtlZeroMemory(buffer, bufferSize);

            // all setup, do the ioctl
            ok = DeviceIoControl(DeviceHandle,
                                 IOCTL_STORAGE_QUERY_PROPERTY,
                                 &query,
                                 sizeof(STORAGE_PROPERTY_QUERY),
                                 buffer,
                                 bufferSize,
                                 &returnedData,
                                 FALSE);
            if (!ok) {
                if (GetLastError() == ERROR_MORE_DATA) {
                    // this is ok, we'll ignore it here
                } else if (GetLastError() == ERROR_INVALID_FUNCTION) {
                    // this is also ok, the property DNE
                } else if (GetLastError() == ERROR_NOT_SUPPORTED) {
                    // this is also ok, the property DNE
                } else {
                    // some unexpected error -- exit out
                    goto Cleanup;
                }
                // zero it out, just in case it was partially filled in.
                RtlZeroMemory(buffer, bufferSize);
            }
        }
    } // end i loop

    // adapterDescriptor is now allocated and full of data.
    // deviceDescriptor is now allocated and full of data.

    if (adapterDescriptor == NULL) {
        printf("   ***** No adapter descriptor supported on the device *****\n");
    } else {
        PrintAdapterDescriptor(adapterDescriptor);
        *AlignmentMask = adapterDescriptor->AlignmentMask;
    }

    if (deviceDescriptor == NULL) {
        printf("   ***** No device descriptor supported on the device  *****\n");
    } else {
        PrintDeviceDescriptor(deviceDescriptor);
    }

    failed = FALSE;

Cleanup:
    if (adapterDescriptor != NULL) {
        LocalFree( adapterDescriptor );
    }
    if (deviceDescriptor != NULL) {
        LocalFree( deviceDescriptor );
    }
    return (!failed);

}



