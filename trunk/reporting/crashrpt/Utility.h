/************************************************************************************* 
This file is a part of CrashRpt library.
Copyright (c) 2003-2013 The CrashRpt project authors. All Rights Reserved.

Use of this source code is governed by a BSD-style license
that can be found in the License.txt file in the root of the source
tree. All contributing project authors may
be found in the Authors.txt file in the root of the source tree.
***************************************************************************************/

// File: Utility.h
// Description: Miscellaneous helper functions
// Authors: mikecarruth, zexspectrum
// Date: 

#ifndef _UTILITY_H_
#define _UTILITY_H_

#include "stdafx.h"

namespace Utility
{
    // Returns base name of the EXE file that launched current process.
    WTL::CString getAppName();

    // Returns the unique tmp file name.
    WTL::CString getTempFileName();

    // Returns the path to the temporary directory.
    int getTempDirectory(WTL::CString& strTemp);

    // Returns path to directory where EXE or DLL module is located.
    WTL::CString GetModulePath(HMODULE hModule);

    // Returns the absolute path and name of the module
    WTL::CString GetModuleName(HMODULE hModule);

    // Generates unique identifier (GUID)
    int GenerateGUID(WTL::CString& sGUID);  

    // Returns current system time as string (uses UTC time format).
    int GetSystemTimeUTC(WTL::CString& sTime); 

    // Converts UTC string to local time.
    void UTC2SystemTime(WTL::CString sUTC, SYSTEMTIME& st);

    // Returns friendly name of operating system (name, version, service pack)
    int GetOSFriendlyName(WTL::CString& sOSName);  

    // Returns TRUE if Windows is 64-bit
    BOOL IsOS64Bit();  

    // Retrieves current geographic location
    int GetGeoLocation(WTL::CString& sGeoLocation);

    // Returns path to a special folder (for example %LOCAL_APP_DATA%)
    int GetSpecialFolder(int csidl, WTL::CString& sFolderPath);

    // Replaces restricted characters in file name
    WTL::CString ReplaceInvalidCharsInFileName(WTL::CString sFileName);

    // Moves a file to the Recycle Bin or removes the file permanently
    int RecycleFile(WTL::CString sFilePath, bool bPermanentDelete);

    // Retrieves a string from INI file.
    WTL::CString GetINIString(LPCTSTR pszFileName, LPCTSTR pszSection, LPCTSTR pszName);

    // Adds a string to INI file.
    void SetINIString(LPCTSTR pszFileName, LPCTSTR pszSection, LPCTSTR pszName, LPCTSTR pszValue);

    // Mirrors the content of a window.
    void SetLayoutRTL(HWND hWnd);

    // Formats the error message.
    WTL::CString FormatErrorMsg(DWORD dwErrorCode);

    // Parses file path and returns file name.
    WTL::CString GetFileName(WTL::CString sPath);

    // Parses file path and returns file name without extension.
    WTL::CString GetBaseFileName(WTL::CString sFileName);

    // Parses file path and returns file extension.
    WTL::CString GetFileExtension(WTL::CString sFileName);

    // Returns TRUE if the file name is a search pattern (containing ? or * characters).
    BOOL IsFileSearchPattern(WTL::CString sFileName);

    // Retrieves product version info from resources embedded into EXE or DLL
    WTL::CString GetProductVersion(WTL::CString sModuleName);

    // Creates a folder. If some intermediate folders in the path do not exist,
    // it creates them.
    BOOL CreateFolder(WTL::CString sFolderName);

    // Converts system time to UINT64
    ULONG64 SystemTimeToULONG64( const SYSTEMTIME& st );

    // Formats a string of file size
    WTL::CString FileSizeToStr(ULONG64 uFileSize); 

    // This helper function checks if the string is too long and truncates it with ellipsis (...).
    WTL::CString AddEllipsis(LPCTSTR szString, int nMaxLength);

    // Splits string into list of tokens.
    std::vector<WTL::CString> ExplodeStr(LPCTSTR szString, LPCTSTR szSeparators);

    // Returns file size
    long GetFileSize(const TCHAR *fileName);
};

#endif	// _UTILITY_H_
