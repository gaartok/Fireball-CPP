

#include <windows.h>
#include "debug.h"



#define MAX_VALUE_NAME     200



DWORD CreateRegKey(char *keyName)
   {
   HKEY  hKey;
   DWORD dwResult;
   DWORD disposition;
   char  classStr[200];

   classStr[0] = 0;
   dwResult = RegCreateKeyEx(HKEY_CURRENT_USER, keyName, 0, classStr, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &disposition);

   if (dwResult == ERROR_SUCCESS)
      RegCloseKey(hKey);

   return dwResult;
   }



DWORD SetRegString(char *keyName, char *valueName, char *dataOut)
   {
   HKEY  hKey;
   DWORD dwResult;
   DWORD disposition;
   char  classStr[200];
//   char  strOut[200];

   classStr[0] = 0;
   dwResult = RegCreateKeyEx(HKEY_CURRENT_USER, keyName, 0, classStr, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &disposition);

   if (dwResult != ERROR_SUCCESS)
      {
//      FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwResult, 0, strOut, 199, NULL);
//      dprintf("Error message = %d = %s\n", dwResult, strOut);
      return dwResult;
      }

   dwResult = RegSetValueEx(hKey, valueName, 0, REG_SZ, (unsigned char *)dataOut, strlen(dataOut) + 1);

   RegCloseKey(hKey);

//   if (dwResult != ERROR_SUCCESS)
//      {
//      FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwResult, 0, strOut, 199, NULL);
//      dprintf("Error message = %d = %s\n", dwResult, strOut);
//      }

   return dwResult;
   }




DWORD GetRegString(char *keyName, char *valueName, char *dataOut)
   {
   HKEY  hKey;
   DWORD dwResult;
   DWORD valueIndex;
   DWORD valueSize;
   DWORD dataType;
   DWORD dataSize;
//   char  strOut[200];
   char  value[MAX_VALUE_NAME];
   UCHAR data[200];

   dataOut[0] = 0;

   if ((dwResult = RegOpenKeyEx(HKEY_CURRENT_USER, keyName, 0, KEY_EXECUTE, &hKey)) != ERROR_SUCCESS)
      {
//      FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwResult, 0, strOut, 199, NULL);
//      dprintf("Error message = %s\n", strOut);
      return dwResult;
      }

   valueIndex = 0;
   dwResult = ERROR_SUCCESS;
   while (dwResult == ERROR_SUCCESS)
      {
      valueSize = MAX_VALUE_NAME; 
      value[0] = 0; 
      dataSize = 199;
      dwResult = RegEnumValue(hKey, valueIndex, value, &valueSize, NULL, &dataType, data, &dataSize);

      if ((dataType == REG_SZ) && (!strcmpi(valueName, value)))
         {
         strcpy(dataOut, (char *)data);
         break;
         }

      valueIndex += 1;
      }

//   FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwResult, 0, strOut, 199, NULL);
//   dprintf("Error message = %d = %s\n", dwResult, strOut);

   RegCloseKey(hKey);
   return dwResult;
   }


