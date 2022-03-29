#include "includes.h"

class PrivilegeEscalation
{
public:
    PrivilegeEscalation(HANDLE hToken = INVALID_HANDLE_VALUE, LPCSTR pszPrivilege = "", BOOL bEnabled = FALSE);
private:
    BOOL EnablePrivilege( HANDLE hToken, LPCTSTR pszPrivilege, BOOL bEnabled );
    BOOL EnablePrivilege( LPCTSTR pszPrivilege, BOOL bEnabled );
};


