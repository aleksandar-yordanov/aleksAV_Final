#include "privilegeescalation.h"


PrivilegeEscalation::PrivilegeEscalation(HANDLE hToken, LPCSTR pszPrivilege, BOOL bEnabled)
{
    if(hToken == INVALID_HANDLE_VALUE)
    {
        this->EnablePrivilege(pszPrivilege,bEnabled);
    }
    else
    {
        this->EnablePrivilege(hToken,pszPrivilege,bEnabled);
    }
}

BOOL PrivilegeEscalation::EnablePrivilege( HANDLE hToken, LPCTSTR pszPrivilege, BOOL bEnabled )
{
    BOOL bResult = FALSE;
    __try {

        LUID luid;
        TOKEN_PRIVILEGES priv;

        bResult = LookupPrivilegeValue( NULL, pszPrivilege, &luid );
        assert( TRUE == bResult );
        if( FALSE == bResult ) { __leave; }

        priv.PrivilegeCount = 1;
        priv.Privileges[0].Luid = luid;
        priv.Privileges[0].Attributes =
            (bEnabled ? SE_PRIVILEGE_ENABLED : FALSE );

        bResult = AdjustTokenPrivileges( hToken, FALSE, &priv, sizeof(priv), NULL, NULL );
        if( ERROR_NOT_ALL_ASSIGNED == GetLastError() ) { bResult = FALSE; }

        if( FALSE == bResult ) { __leave; }

        bResult = TRUE;
    }
    __finally { ; }

    return bResult;
}

BOOL PrivilegeEscalation::EnablePrivilege( LPCTSTR pszPrivilege, BOOL bEnabled ){

    BOOL bResult = FALSE;
    HANDLE hToken = NULL;

    __try {

        bResult = OpenThreadToken( GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES, TRUE, &hToken );
        if( !bResult )
        {
            bResult = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken );
            assert( TRUE == bResult );
            if( FALSE == bResult ) { __leave; }
        }

        bResult = EnablePrivilege( hToken, pszPrivilege, bEnabled );
        DWORD dwError = GetLastError();
        assert( TRUE == bResult || ( ERROR_SUCCESS == GetLastError() || ERROR_NOT_ALL_ASSIGNED == dwError ) );
        if( ERROR_NOT_ALL_ASSIGNED == dwError ) { bResult = FALSE; }
    }

    __finally {
        if( NULL != hToken ) {
            CloseHandle( hToken ); hToken = NULL;
        }
    }

    return bResult;
}
