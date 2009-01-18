/*
 *
 * (c) 2004-2009 Denora Team
 * Contact us at info@denorastats.org
 *
 * Please read COPYING and README for furhter details.
 *
 * Based on the original code of Anope by Anope Team.
 * Based on the original code of Thales by Lucas.
 * 
 * $Id$
 *
 */

#include "denora.h"

/*************************************************************************/

#ifdef _WIN32
/**
 * Print out the Windows version information
 * @return A string with the windows version, this needs to be free'd
 */
char *GetWindowsVersion(void)
{
    OSVERSIONINFOEX osvi;
    BOOL bOsVersionInfoEx;
    char buf[BUFSIZE];
    char *extra;
    char *cputype;
    SYSTEM_INFO si;

    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    ZeroMemory(&si, sizeof(SYSTEM_INFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    if (!(bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO *) & osvi))) {
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        if (!GetVersionEx((OSVERSIONINFO *) & osvi)) {
            return sstrdup("");
        }
    }
    GetSystemInfo(&si);

    /* Determine CPU type 32 or 64 */
    if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
        cputype = sstrdup(" 64-bit");
    } else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) {
        cputype = sstrdup(" 32-bit");
    } else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64) {
        cputype = sstrdup(" Itanium 64-bit");
    } else {
        cputype = sstrdup(" ");
    }

    switch (osvi.dwPlatformId) {
        /* test for the Windows NT product family. */
    case VER_PLATFORM_WIN32_NT:
        /* Windows Vista or Windows Server 2008 */
        if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0) {
            if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE) {
                extra = sstrdup("Enterprise Edition");
            } else if (osvi.wSuiteMask & VER_SUITE_DATACENTER) {
                extra = sstrdup("Datacenter Edition");
            } else if (osvi.wSuiteMask & VER_SUITE_PERSONAL) {
                extra = sstrdup("Home Premium/Basic");
            } else {
                extra = sstrdup(" ");
            }
            if (osvi.wProductType & VER_NT_WORKSTATION) {
                ircsnprintf(buf, sizeof(buf),
                            "Microsoft Windows Vista %s%s", cputype,
                            extra);
            } else {
                ircsnprintf(buf, sizeof(buf),
                            "Microsoft Windows Server 2008 %s%s", cputype,
                            extra);
            }
            free(extra);
        }
        /* Windows 2003 or Windows XP Pro 64 */
        if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2) {
            if (osvi.wSuiteMask & VER_SUITE_DATACENTER) {
                extra = sstrdup("Datacenter Edition");
            } else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE) {
                extra = sstrdup("Enterprise Edition");
            } else if (osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER) {
                extra = sstrdup("Compute Cluster Edition");
            } else if (osvi.wSuiteMask == VER_SUITE_BLADE) {
                extra = sstrdup("Web Edition");
            } else {
                extra = sstrdup("Standard Edition");
            }
            if (osvi.wProductType & VER_NT_WORKSTATION
                && si.wProcessorArchitecture ==
                PROCESSOR_ARCHITECTURE_AMD64) {
                ircsnprintf(buf, sizeof(buf),
                            "Windows XP Professional x64 Edition %s",
                            extra);
            } else {
                ircsnprintf(buf, sizeof(buf),
                            "Microsoft Windows Server 2003 Family %s%s",
                            cputype, extra);
            }
            free(extra);
        }
        if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1) {
            if (osvi.wSuiteMask & VER_SUITE_EMBEDDEDNT) {
                extra = sstrdup("Embedded");
            } else if (osvi.wSuiteMask & VER_SUITE_PERSONAL) {
                extra = sstrdup("Home Edition");
#ifdef SM_MEDIACENTER
            } else if (GetSystemMetrics(SM_MEDIACENTER)) {
                extra = sstrdup("Media Center Edition");
#endif
#ifdef SM_TABLETPC
            } else if (GetSystemMetrics(SM_TABLETPC)) {
                extra = sstrdup("Tablet Edition");
#endif
            } else {
                extra = sstrdup(" ");
            }
            ircsnprintf(buf, sizeof(buf), "Microsoft Windows XP %s",
                        extra);
            free(extra);
        }
        if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0) {
            if (osvi.wSuiteMask & VER_SUITE_DATACENTER) {
                extra = sstrdup("Datacenter Server");
            } else if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE) {
                extra = sstrdup("Advanced Server");
            } else {
                extra = sstrdup("Server");
            }
            ircsnprintf(buf, sizeof(buf), "Microsoft Windows 2000 %s",
                        extra);
            free(extra);
        }
        if (osvi.dwMajorVersion <= 4) {
            if (osvi.wSuiteMask & VER_SUITE_ENTERPRISE) {
                extra = sstrdup("Server 4.0, Enterprise Edition");
            } else {
                extra = sstrdup("Server 4.0");
            }
            ircsnprintf(buf, sizeof(buf), "Microsoft Windows NT %s",
                        extra);
            free(extra);
        }
    case VER_PLATFORM_WIN32_WINDOWS:
        if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0) {
            if (osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B') {
                extra = sstrdup("OSR2");
            } else {
                extra = sstrdup(" ");
            }
            ircsnprintf(buf, sizeof(buf), "Microsoft Windows 95 %s",
                        extra);
            free(extra);
        }
        if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10) {
            if (osvi.szCSDVersion[1] == 'A') {
                extra = sstrdup("SE");
            } else {
                extra = sstrdup(" ");
            }
            ircsnprintf(buf, sizeof(buf), "Microsoft Windows 98 %s",
                        extra);
            free(extra);
        }
        if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90) {
            ircsnprintf(buf, sizeof(buf),
                        "Microsoft Windows Millennium Edition");
        }
    }
    free(cputype);
    return sstrdup(buf);
}

/*************************************************************************/

/**
 * Return the CPU architecture
 * @return char that needs to be free'd
 */
char *WindowsCPUArchitecture(void)
{
    SYSTEM_INFO si;
    char buf[BUFSIZE];
    char *cpuarch;

    GetSystemInfo(&si);
    if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
        cpuarch = sstrdup("x64 (AMD or Intel)");
    } else if (si.wProcessorArchitecture ==
               PROCESSOR_ARCHITECTURE_IA32_ON_WIN64) {
        cpuarch = sstrdup("Emmulation \"WOW64\"");
    } else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64) {
        cpuarch = sstrdup("Intel Itanium Processor Family (IPF)");
    } else if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) {
        cpuarch = sstrdup("x86");
    } else {
        cpuarch = sstrdup("Unknown processor");
    }
    ircsnprintf(buf, sizeof(buf), "arch: %s  number %d", cpuarch,
                si.dwNumberOfProcessors);
    return sstrdup(buf);
}


/*************************************************************************/

/**
 * Return if the version of windows is support
 * @return 0 if not supported 1 if supported
 */
int SupportedWindowsVersion(void)
{
    OSVERSIONINFOEX osvi;
    BOOL bOsVersionInfoEx;

    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    if (!(bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO *) & osvi))) {
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
        if (!GetVersionEx((OSVERSIONINFO *) & osvi)) {
            return 0;
        }
    }

    switch (osvi.dwPlatformId) {
        /* test for the Windows NT product family. */
    case VER_PLATFORM_WIN32_NT:
        /* win nt4 */
        if (osvi.dwMajorVersion <= 4) {
            return 0;
        }
        /* the rest */
        return 1;
        /* win95 win98 winME */
    case VER_PLATFORM_WIN32_WINDOWS:
        return 0;
    }
    return 0;
}

/* thanks to for gettimeofday()
 * Copyright (c) 2003 SRA, Inc.
 * Copyright (c) 2003 SKC, Inc.
 */
/* FILETIME of Jan 1 1970 00:00:00. */
static const unsigned __int64 epoch = 116444736000000000L;

/*
 * timezone information is stored outside the kernel so tzp isn't used anymore.
 */

int gettimeofday(struct timeval *tp, struct timezone *tzp)
{
    FILETIME file_time;
    SYSTEMTIME system_time;
    ULARGE_INTEGER ularge;

    GetSystemTime(&system_time);
    SystemTimeToFileTime(&system_time, &file_time);
    ularge.LowPart = file_time.dwLowDateTime;
    ularge.HighPart = file_time.dwHighDateTime;

    tp->tv_sec = (long) ((ularge.QuadPart - epoch) / 10000000L);
    tp->tv_usec = (long) (system_time.wMilliseconds * 1000);

    return 0;
}

#ifdef USE_THREADS
/* Simulate pthread conditional variable waiting */
int deno_cond_wait(deno_cond_t cond, deno_mutex_t mutex)
{
    ReleaseMutex(mutex);
    if (WaitForSingleObject(cond, INFINITE) == WAIT_FAILED)
        return 1;
    if (WaitForSingleObject(mutex, INFINITE) == WAIT_FAILED)
        return 1;
    return 0;
}

/* Used for the cleanup functions */
deno_thread_start __declspec(thread) cleanup_func = NULL;

#endif
#endif

/*************************************************************************/
/* Windows support functions */

#ifdef _WIN32
/* Microsoft makes things nice and fun for us! */
struct u_WSA_errors {
    int error_code;
    char *error_string;
};

/*************************************************************************/

/* Must be sorted ascending by error code */
struct u_WSA_errors WSAErrors[] = {
    {WSAEINTR, "Interrupted system call"},
    {WSAEBADF, "Bad file number"},
    {WSAEACCES, "Permission denied"},
    {WSAEFAULT, "Bad address"},
    {WSAEINVAL, "Invalid argument"},
    {WSAEMFILE, "Too many open sockets"},
    {WSAEWOULDBLOCK, "Operation would block"},
    {WSAEINPROGRESS, "Operation now in progress"},
    {WSAEALREADY, "Operation already in progress"},
    {WSAENOTSOCK, "Socket operation on non-socket"},
    {WSAEDESTADDRREQ, "Destination address required"},
    {WSAEMSGSIZE, "Message too long"},
    {WSAEPROTOTYPE, "Protocol wrong type for socket"},
    {WSAENOPROTOOPT, "Bad protocol option"},
    {WSAEPROTONOSUPPORT, "Protocol not supported"},
    {WSAESOCKTNOSUPPORT, "Socket type not supported"},
    {WSAEOPNOTSUPP, "Operation not supported on socket"},
    {WSAEPFNOSUPPORT, "Protocol family not supported"},
    {WSAEAFNOSUPPORT, "Address family not supported"},
    {WSAEADDRINUSE, "Address already in use"},
    {WSAEADDRNOTAVAIL, "Can't assign requested address"},
    {WSAENETDOWN, "Network is down"},
    {WSAENETUNREACH, "Network is unreachable"},
    {WSAENETRESET, "Net connection reset"},
    {WSAECONNABORTED, "Software caused connection abort"},
    {WSAECONNRESET, "Connection reset by peer"},
    {WSAENOBUFS, "No buffer space available"},
    {WSAEISCONN, "Socket is already connected"},
    {WSAENOTCONN, "Socket is not connected"},
    {WSAESHUTDOWN, "Can't send after socket shutdown"},
    {WSAETOOMANYREFS, "Too many references, can't splice"},
    {WSAETIMEDOUT, "Connection timed out"},
    {WSAECONNREFUSED, "Connection refused"},
    {WSAELOOP, "Too many levels of symbolic links"},
    {WSAENAMETOOLONG, "File name too long"},
    {WSAEHOSTDOWN, "Host is down"},
    {WSAEHOSTUNREACH, "No route to host"},
    {WSAENOTEMPTY, "Directory not empty"},
    {WSAEPROCLIM, "Too many processes"},
    {WSAEUSERS, "Too many users"},
    {WSAEDQUOT, "Disc quota exceeded"},
    {WSAESTALE, "Stale NFS file handle"},
    {WSAEREMOTE, "Too many levels of remote in path"},
    {WSASYSNOTREADY, "Network subsystem is unavailable"},
    {WSAVERNOTSUPPORTED, "Winsock version not supported"},
    {WSANOTINITIALISED, "Winsock not yet initialized"},
    {WSAHOST_NOT_FOUND, "Host not found"},
    {WSATRY_AGAIN, "Non-authoritative host not found"},
    {WSANO_RECOVERY, "Non-recoverable errors"},
    {WSANO_DATA, "Valid name, no data record of requested type"},
    {WSAEDISCON, "Graceful disconnect in progress"},
#ifdef WSASYSCALLFAILURE
    {WSASYSCALLFAILURE, "System call failure"},
#endif
    {0, NULL}
};

/*************************************************************************/

char *deno_sockstrerror(int error)
{
    static char unkerr[64];
    int start = 0;
    int stop = sizeof(WSAErrors) / sizeof(WSAErrors[0]) - 1;
    int mid;

    /* Microsoft decided not to use sequential numbers for the error codes,
     * so we can't just use the array index for the code. But, at least
     * use a binary search to make it as fast as possible. 
     */
    while (start <= stop) {
        mid = (start + stop) / 2;
        if (WSAErrors[mid].error_code > error)
            stop = mid - 1;

        else if (WSAErrors[mid].error_code < error)
            start = mid + 1;
        else
            return WSAErrors[mid].error_string;
    }
    ircsnprintf(unkerr, sizeof(unkerr), "Unknown Error: %d", error);
    return unkerr;
}

/*************************************************************************/

int deno_socksetnonb(deno_socket_t fd)
{
    int i = 1;
    return (!ioctlsocket(fd, FIONBIO, &i) ? -1 : 1);
}
#endif

/*************************************************************************/

#ifdef _WIN32
void Win32LoadCoreDir(char *dirname)
{
#ifdef USE_MODULES
    BOOL fFinished;
    HANDLE hList;
    TCHAR szDir[MAX_PATH + 1];
    TCHAR szSubDir[MAX_PATH + 1];
    WIN32_FIND_DATA FileData;
    char *mname = NULL;
    Module *m;
    int status;
    char buffer[_MAX_PATH];

    /* Get the current working directory: */
    if (_getcwd(buffer, _MAX_PATH) == NULL) {
        alog(LOG_DEBUG, "debug: Unable to set Current working directory");
    }
    ircsnprintf(szDir, sizeof(szDir), "%s\\%s\\*", buffer, dirname);

    hList = FindFirstFile(szDir, &FileData);
    if (hList != INVALID_HANDLE_VALUE) {
        fFinished = FALSE;
        while (!fFinished) {
            if (!(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                mname = sstrdup(FileData.cFileName);
                mname[strlen(FileData.cFileName) - 4] = '\0';
                m = findModule(mname);
                if (!m) {
                    m = createCoreModule(mname);
                    mod_current_module = m;
                    mod_current_user = NULL;
                    alog(LOG_DEBUG, "trying to load [%s]",
                         mod_current_module->name);
                    status = loadCoreModule(mod_current_module, NULL);
                    alog(LOG_DEBUG, langstr(ALOG_MOD_STATUS), status,
                         ModuleGetErrStr(status));
                    if (status != MOD_ERR_OK) {
                        destroyModule(m);
                    }
                    mod_current_module = NULL;
                    mod_current_user = NULL;
                }
                dfree(mname);
            }
            if (!FindNextFile(hList, &FileData)) {
                if (GetLastError() == ERROR_NO_MORE_FILES) {
                    fFinished = TRUE;
                }
            }
        }
    } else {
        printf("Invalid File Handle. GetLastError reports %d\n",
               GetLastError());
    }
    FindClose(hList);
#endif
}
#endif

/*************************************************************************/

#ifdef _WIN32
const char *deno_moderr(void)
{
    static char errbuf[513];
    DWORD err = GetLastError();
    if (err == 0)
        return NULL;
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
                  FORMAT_MESSAGE_IGNORE_INSERTS, NULL, err, 0, errbuf, 512,
                  NULL);
    return errbuf;
}
#endif

/*************************************************************************/
