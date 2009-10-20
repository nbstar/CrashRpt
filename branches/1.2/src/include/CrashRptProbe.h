/*! \file   CrashRptProbe.h
 *  \brief  Defines the interface for the CrashRptProbe.DLL.
 *  \date   2009
 *  \author zexspectrum@gmail.com
 */

#ifndef __CRASHRPT_PROBE_H__
#define __CRASHRPT_PROBE_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CRASHRPTPROBE_EXPORTS
#define CRASHRPTPROBE_API __declspec(dllexport) WINAPI
#else
#define CRASHRPTPROBE_API __declspec(dllimport) WINAPI
#endif

typedef int CrpHandle;

/*! \defgroup CrashRptProbeAPI CrashRptProbe Functions*/
/*! \defgroup CrashRptProbeEnums CrashRptProbe Enumerations*/

/*! \ingroup CrashRptProbeAPI
 *  \brief Opens a zipped crash report file.
 *
 *  \return This function returns zero on success. 
 *
 *  \param[in] pszFileName Zipped report file name.
 *  \param[in] pszMd5Hash String containing MD5 hash for the ZIP file data.
 *  \param[in] pszSymSearchPath Symbol files (PDB) search path.
 *  \param[in] dwFlags Flags, reserved for future use.
 *  \param[out] pHandle Handle to the opened crash report.
 *
 *  \remarks
 *
 *  Use this function to open a ZIP archive containing an error report. The error report typically contains
 *  several compressed files, such as XML crash descriptor, crash minidump file, and optionally 
 *  application-defined files.
 *
 *  \a pszFileName should be the name of the error report (ZIP file) to open. This parameter is required.
 *
 *  \a pszMd5Hash is a string containing the MD5 hash calculated for \a pszFileName. The MD5
 *  hash is used for integrity checks. If this parameter is NULL, integrity check is not performed.
 *
 *  If the error report is delivered by HTTP, the MD5 hash can be extracted by server-side script from the
 *  'md5' parameter. When the error report is delivered by email, the MD5 hash is attached to the mail message.
 *  The integrity check can be performed to ensure the error report was not corrupted during delivery.
 *  For more information, see \ref sending_error_report.
 *
 *  \a pszSymSearchPath parameter defines where to look for symbols files (PDB). You can specify the list of 
 *  semicolom-separated directories to search in. If this parameter is NULL, the default search path is used.
 *
 *  Symbol files are required for crash report processing. They contain various information used by the debugger.
 *
 *  \a dwFlags is currently not used, should be zero.
 *
 *  \a pHandle parameter receives the handle to the opened crash report. If the function fails,
 *  this parameter becomes zero. 
 *
 *  This function does the following when opening report file:
 *    - It performs integrity checks for the error report being opened, if MD5 hash is specified.
 *    - It searches for crashrpt.xml and crashdump.dml files inside of ZIP archive, if such files
 *      do not present, it assumes the report was generated by CrashRpt v1.0. In such case it searches for
 *      any file having *.dmp or *.xml extension and assumes these are valid XML and DMP file.
 *    - It extracts and loads the XML file and checks its structure.
 *    - It extracts the minidump file to the temporary location.
 *
 *  On failure, use crpGetLastErrorMsg() function to get the last error message.
 *
 *  Use the crpCloseErrorReport() function to close the opened error report.
 *
 *  \note
 *
 *  The crpOpenErrorReportW() and crpOpenErrorReportA() are wide character and multibyte 
 *  character versions of crpOpenErrorReport(). 
 *
 *  The following example shows how to open an error report file:
 *
 * \code
 *   #include <CrashRptProbe.h>
 *   #include <stdio.h>
 *   ...
 *   
 *   CrpHandle hReport = 0;
 *   int result = crpOpenErrorReport(
 *                 _T("0b3b0c1b-3450-4c39-9459-42221ae66460.zip"), // Zip archive name
 *                 _T("2e4345603454a345064371ab34195316"), // MD5 hash for the file
 *                 _T("D:\\MyApp\\1.3.4\\Sym; D:\\MyApp\\1.3.5\\Sym"), // Where to look for symbols
 *                 0, // Reserved
 *                 &hReport
 *                 );
 *
 *  if(result!=0)
 *  {
 *    TCHAR szErrorMsg[256];
 *    crpGetLastErrorMsg(szErrorMsg, 256);
 *    return;
 *  }
 *
 *  // Do something with it...
 *
 *  // Finally, close the report
 *  crpCloseErrorReport(hReport);
 * \endcode
 *
 *  \sa 
 *    crpCloseErrorReport()
 */

int
CRASHRPTPROBE_API
crpOpenErrorReportW(
  __in LPCWSTR pszFileName,
  __in_opt LPCWSTR pszMd5Hash,
  __in_opt LPCWSTR pszSymSearchPath,
  __reserved DWORD dwFlags,
  __out CrpHandle* phReport
);

/*! \ingroup CrashRptProbeAPI
 *  \copydoc crpOpenErrorReportW()
 *
 */

int
CRASHRPTPROBE_API
crpOpenErrorReportA(
  __in LPCSTR pszFileName,
  __in_opt LPCSTR pszMd5Hash,
  __in_opt LPCSTR pszSymSearchPath,  
  __reserved DWORD dwFlags,
  __out CrpHandle* phReport
);

/*! \brief Character set-independent mapping of crpOpenErrorReportW() and crpOpenErrorReportA() functions. 
 *  \ingroup CrashRptProbeAPI
 */

#ifdef UNICODE
#define crpOpenErrorReport crpOpenErrorReportW
#else
#define crpOpenErrorReport crpOpenErrorReportA
#endif //UNICODE

/*! 
 *  \brief Closes the crash report.
 *  \return This function returns zero if successful, else non-zero.
 *  \param[in] hReport Handle to the opened error report.
 *
 *  \remarks
 *
 *  Use this function to close the error report previously opened with crpOpenErrorReport()
 *  function.
 *
 *  If this function fails, use crpGetLastErrorMsg() function to get the error message.
 *
 *  See crpOpenErrorReportW() for code example. 
 *
 *  \sa
 *    crpOpenErrorReport(), crpOpenErrorReportW(), crpOpenErrorReportA(), crpGetLastErrorMsg()
 */

int
CRASHRPTPROBE_API 
crpCloseErrorReport(
  CrpHandle hReport  
);

/*! \ingroup CrashRptProbeEnums
 *  \brief Table names passed to crpGetProperty() function. 
 *
 *  \remarks
 *
 *  An error report can be presented as a set of properties. These properties
 *  are groupped into tables and can be accessed by the table id, column id and row index.
 *  
 *  For the detailed description of available tables and code examples, see \ref crprobe_properties section.
 */

enum CRP_TableId
{ 
  CRP_TBL_META                         = 0,    //!< Table: Contains information about all ather tables.
  CRP_TBL_XMLDESC_MISC                 = 1,    //!< Table: Miscellaneous info contained in crash descriptor XML file. 
  CRP_TBL_XMLDESC_FILE_ITEMS           = 2,    //!< Table: The list of file items contained in error report.
  CRP_TBL_MDMP_MISC                    = 3,    //!< Table: Miscellaneous info contained in crash minidump file.  
  CRP_TBL_MDMP_MODULES                 = 4,    //!< Table: The list of loaded modules.
  CRP_TBL_MDMP_THREADS                 = 5,    //!< Table: The list of threads.
};

/*! \ingroup CrashRptProbeEnums
 *  \brief Column names passed to crpGetProperty() function. 
 *
 *  \remarks
 *
 *  An error report can be presented as a set of properties. These properties
 *  are groupped into tables and can be accessed by the table id, column id and row index.
 *  
 *  For the detailed description of available column names and code examples, see \ref crprobe_properties section.
 */

enum CRP_ColumnId
{   
  CRP_COL_ROW_COUNT               = 0,    //!< Column: Count of rows in a table.
  
  CRP_COL_CRASHRPT_VERSION        = 100,  //!< Column: Version of CrashRpt library that generated the report.
  CRP_COL_CRASH_GUID              = 101,  //!< Column: Globally unique identifier (GUID) of the error report.
  CRP_COL_APP_NAME                = 102,  //!< Column: Application name.
  CRP_COL_APP_VERSION             = 103,  //!< Column: Application version.
  CRP_COL_IMAGE_NAME              = 104,  //!< Column: Path to the executable file.
  CRP_COL_OPERATING_SYSTEM        = 105,  //!< Column: Opration system name, including build number and service pack.
  CRP_COL_SYSTEM_TIME_UTC         = 106,  //!< Column: Time (UTC) when the crash occured.
  CRP_COL_EXCEPTION_TYPE          = 107,  //!< Column: Code of exception handler that cought the exception.
  CRP_COL_EXCEPTION_CODE          = 108,  //!< Column: Exception code; for the structured exceptions only, hexadecimal number.
  CRP_COL_INVPARAM_FUNCTION       = 109, //!< Column: Function name; for invalid parameter errors only.
  CRP_COL_INVPARAM_EXPRESSION     = 110, //!< Column: Expression; for invalid parameter errors only.
  CRP_COL_INVPARAM_FILE           = 111, //!< Column: Source file name; for invalid parameter errors only.
  CRP_COL_INVPARAM_LINE           = 112, //!< Column: Source line; for invalid parameter errors only.
  CRP_COL_FPE_SUBCODE             = 113, //!< Column: Subcode of floating point exception; for FPE exceptions only.
  CRP_COL_USER_EMAIL              = 114, //!< Column: Email of the user who sent this report.
  CRP_COL_PROBLEM_DESCRIPTION     = 115, //!< Column: User-provided problem description.
  
  CRP_COL_FILE_ITEM_NAME          = 200, //!< Column: File list: Name of the file contained in the report, vectored.
  CRP_COL_FILE_ITEM_DESCRIPTION   = 201, //!< Column: File list: Description of the file contained in the report.

  CRP_COL_CPU_ARCHITECTURE        = 300, //!< Column: Processor architecture.
  CRP_COL_CPU_COUNT               = 301, //!< Column: Number of processors.
  CRP_COL_SYSTEM_TYPE             = 302, //!< Column: Type of system (server or workstation).
  CRP_COL_OS_VER_MAJOR            = 303, //!< Column: OS major version.
  CRP_COL_OS_VER_MINOR            = 304, //!< Column: OS minor version.
  CRP_COL_OS_VER_BUILD            = 305, //!< Column: OS build number.
  CRP_COL_OS_VER_CSD              = 306, //!< Column: The latest service pack installed.

  CRP_COL_EXCPTRS_EXCEPTION_CODE    = 400, //!< Column: Code of the structured exception.
  CRP_COL_EXCPTRS_EXCEPTION_ADDRESS = 401, //!< Column: Exception address.
  CRP_COL_EXCPTRS_EXCEPTION_THREAD_ROWID = 403, //!< Column: ROWID in CRP_TBL_THREADS of the thread in which exception occurred. 

  CRP_COL_MODULE_NAME             = 500, //!< Column: Module name.
  CRP_COL_MODULE_IMAGE_NAME       = 501, //!< Column: Image name containing full path.  
  CRP_COL_MODULE_BASE_ADDRESS     = 502, //!< Column: Module base load address.
  CRP_COL_MODULE_SIZE             = 503, //!< Column: Module size.
  CRP_COL_MODULE_LOADED_PDB_NAME  = 504,  //!< Column: The full path and file name of the .pdb file. 

  CRP_COL_THREAD_ID               = 601, //!< Column: Thread ID.
  CRP_COL_THREAD_STACK_TABLEID    = 602, //!< Column: The TABLEID of the table containing stack trace for this thread.

  CRP_COL_STACK_MODULE_ROWID      = 700, //!< Column: Stack trace: ROWID of the module in the CRP_TBL_MODULES table.
  CRP_COL_STACK_SYMBOL_NAME       = 701, //!< Column: Stack trace: symbol name.
  CRP_COL_STACK_OFFSET_IN_SYMBOL  = 702, //!< Column: Stack trace: offset in symbol, hexadecimal.
  CRP_COL_STACK_SOURCE_FILE       = 703, //!< Column: Stack trace: source file name.
  CRP_COL_STACK_SOURCE_LINE       = 704, //!< Column: Stack trace: source file line number.
};


/*! \ingroup CrashRptProbeAPI
 *  \brief Retrieves a string property from crash report.
 *  \return This function returns zero on success.
 *
 *  \param[in]  hReport Handle to the previously opened crash report.
 *  \param[in]  nPropId Property ID.
 *  \param[in]  nIndex Index of the property in the table.
 *  \param[out] lpszBuffer Output buffer.
 *  \param[in]  cchBuffSize Size of the output buffer in characters.
 *  \param[out] pcchCount Count of characters written to the buffer.
 *
 *  \remarks
 *
 *  Use this function to retrieve data from the crash report that was previously opened with the
 *  crpOpenErrorReport() function.
 *
 *  Some properties are loaded from crash descriptor XML file, while others are loaded from crash minidump file.
 *  The minidump is loaded once and only when you retrive a property from it. This reduces the overall processing time.
 *
 *  \a hReport should be the handle to the opened error report.
 *
 *  \a nPropId represents the ID of the property to retrieve. For the list of available 
 *  properties, see CRP_ErrorReportProperty() enumeration.
 *
 *  \a nIndex defines the zero-based index of the property (used for some properties that are groupped in tables).
 *  
 *  \a lpszBuffer defines the buffer where retrieved property value will be placed. If this parameter
 *  is NULL, it is ignored and \c pcchCount is set with the required size in characters of the buffer.
 *
 *  \a cchBuffSize defines the buffer size in characters. To calculate required buffer size, set \a lpszBuffer with NULL, 
 *  the function will set \pcchCount with the number of characters required.
 *
 *  \a pcchCount is set with the actual count of characters copied to the \a lpszBuffer. If this parameter is NULL,
 *  it is ignored.
 *
 *  If this function fails, use crpGetLastErrorMsg() function to get the error message.
 *
 *  For code examples of using this function, see \ref crashrptprobe_api_examples.
 *
 *  \note
 *  The crpGetPropertyW() and crpGetPropertyA() are wide character and multibyte 
 *  character versions of crpGetProperty(). 
 *
 *  \sa
 *    crpGetPropertyW(), crpGetPropertyA(), crpOpenErrorReport(), crpGetLastErrorMsg()
 */ 

int
CRASHRPTPROBE_API 
crpGetPropertyW(
  CrpHandle hReport,
  CRP_TableId TableId,
  CRP_ColumnId ColumnId,
  INT nRowIndex,
  __out_ecount_z(pcchBuffSize) LPWSTR lpszBuffer,
  ULONG cchBuffSize,
  __out PULONG pcchCount
);

/*! \ingroup CrashRptProbeAPI
 *  \copydoc crpGetPropertyW()
 *
 */

int
CRASHRPTPROBE_API 
crpGetPropertyA(
  CrpHandle hReport,
  CRP_TableId TableId,
  CRP_ColumnId ColumnId,
  INT nRowIndex,
  __out_ecount_z(pcchBuffSize) LPSTR lpszBuffer,
  ULONG pcchBuffSize,
  __out PULONG pcchCount
);

/*! \brief Character set-independent mapping of crpGetPropertyW() and crpGetPropertyA() functions. 
 *  \ingroup CrashRptProbeAPI
 */

#ifdef UNICODE
#define crpGetProperty crpGetPropertyW
#else
#define crpGetProperty crpGetPropertyA
#endif //UNICODE

/*! \ingroup CrashRptProbeAPI
 *  \brief Extracts a file from the opened error report.
 *  \return This function returns zero if succeeded.
 *
 *  \param[in] hReport Handle to the opened error report.
 *  \param[in] lpszFileName The name of the file to extract.
 *  \param[in] lpszFileSaveAs The resulting name of the extracted file.
 *  \param[in] bOverwriteExisting Overwrite the destination file if it already exists?
 *
 *  \remarks
 *
 *  Use this function to extract a compressed file from the error report (ZIP) file.
 *
 *  \a lpszFileName parameter should be the name of the file to extract. For more information
 *  about enumerating file names, see \ref example_enum_file_items.
 *
 *  \a lpszFileSaveAs defines the name of the file to extract to. 
 *
 *  \a bOverwriteExisting flag defines the behavior when the destination file is already exists.
 *  If this parameter is TRUE, the file is overwritten, otherwise the function fails.
 *
 *  If this function fails, use crpGetLastErrorMsg() to retrieve the error message.
 *
 *  \note
 *    The crpExtractFileW() and crpExtractFileA() are wide character and multibyte 
 *    character versions of crpExtractFile(). 
 *
 *  \sa
 *    crpExtractFileA(), crpExtractFileW(), crpExtractFile()
 */

int
CRASHRPTPROBE_API 
crpExtractFileW(
  CrpHandle hReport,
  LPCWSTR lpszFileName,
  LPCWSTR lpszFileSaveAs,
  BOOL bOverwriteExisting
);

/*! \ingroup CrashRptProbeAPI
 *  \copydoc crpExtractFileW() 
 */

int
CRASHRPTPROBE_API 
crpExtractFileA(
  CrpHandle hReport,
  LPCSTR lpszFileName,
  LPCSTR lpszFileSaveAs,
  BOOL bOverwriteExisting
);

/*! \brief Character set-independent mapping of crpExtractFileW() and crpExtractFileA() functions. 
 *  \ingroup CrashRptProbeAPI
 */

#ifdef UNICODE
#define crpExtractFile crpExtractFileW
#else
#define crpExtractFile crpExtractFileA
#endif //UNICODE

/*! \ingroup CrashRptProbeAPI 
 *  \brief Gets the last CrashRptProbe error message.
 *
 *  \return This function returns length of error message in characters.
 *
 *  \param[out] pszBuffer Pointer to the buffer.
 *  \param[in]  uBuffSize Size of buffer in characters.
 *
 *  \remarks
 *
 *  This function gets the last CrashRptProbe error message. You can use this function
 *  to retrieve the text status of the last called CrashRptProbe function.
 *
 *  If buffer is too small for the error message, the message is truncated.
 *
 *  \note 
 *    crpGetLastErrorMsgW() and crpGetLastErrorMsgA() are wide-character and multi-byte character versions
 *    of crpGetLastErrorMsg(). The crpGetLastErrorMsg() macro defines character set independent mapping.
 *
 *  The following example shows how to use crpGetLastErrorMsg() function.
 *
 *  \code
 *  
 *  // .. call some CrashRptProbe function
 *
 *  // Get the status message
 *  TCHAR szErrorMsg[256];
 *  crpGetLastErrorMsg(szErrorMsg, 256);
 *  \endcode
 *
 *  \sa crpGetLastErrorMsgA(), crpGetLastErrorMsgW(), crpGetLastErrorMsg()
 */

int
CRASHRPTPROBE_API
crpGetLastErrorMsgW(
  __out_ecount(cchBuffSize) LPTSTR pszBuffer, 
  __in UINT cchBuffSize);

/*! \ingroup CrashRptProbeAPI
 *  \copydoc crpGetLastErrorMsgW()
 *
 */

int
CRASHRPTPROBE_API
crpGetLastErrorMsgA(
  __out_ecount(cchBuffSize) LPSTR pszBuffer, 
  __in UINT cchBuffSize);

/*! \brief Defines character set-independent mapping for crpGetLastErrorMsgW() and crpGetLastErrorMsgA().
 *  \ingroup CrashRptProbeAPI
 */

#ifdef UNICODE
#define crpGetLastErrorMsg crpGetLastErrorMsgW
#else
#define crpGetLastErrorMsg crpGetLastErrorMsgA
#endif //UNICODE

#ifdef __cplusplus
}
#endif

#endif __CRASHRPT_PROBE_H__