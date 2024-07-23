#include <Windows.h>
#include <ntstatus.h>
#include "jbugcheck.h"
#include "com_mamangava_jbugcheck_JBugCheck.h"

// Declaração das rotinas utilizadas para escalar o privilégio do processo e chamar um bug check.
typedef NTSYSCALLAPI
NTSTATUS
(NTAPI
*NtRaiseHardError)(
    IN NTSTATUS ErrorStatus,
    IN ULONG NumberOfParameters,
    IN ULONG UnicodeStringParameterMask,
    IN PULONG_PTR Parameters,
    IN HARDERROR_RESPONSE_OPTION ValidResponseOptions,
    OUT HARDERROR_RESPONSE * Response);

typedef NTSYSAPI
NTSTATUS
(NTAPI
*RtlAdjustPrivilege)(
    ULONG Privilege,
    BOOLEAN Enable,
    BOOLEAN Client,
    PBOOLEAN WasEnabled);

JNIEXPORT void JNICALL Java_com_mamangava_jbugcheck_JBugCheck_BugCheck
  (JNIEnv * env, jobject this) {
    NtRaiseHardError pfnNtRaiseHardError;
    RtlAdjustPrivilege pfnRtlAdjustPrivilege;

    // Aqui eu pego o handle da ntdll (dll que contém as rotinas que eu vou utilizar)
    HANDLE hNtdll = GetModuleHandleA("ntdll.dll");

    if(hNtdll == 0) return;

    // Aqui eu pego o endereço das rotinas que eu vou utilizar para fazer essa traquinagem
    pfnNtRaiseHardError = (NtRaiseHardError)GetProcAddress(hNtdll, "NtRaiseHardError");
    pfnRtlAdjustPrivilege = (RtlAdjustPrivilege)GetProcAddress(hNtdll, "RtlAdjustPrivilege");

    if((pfnNtRaiseHardError || pfnRtlAdjustPrivilege) == 0) return;

    // BOOM!!!
    BOOLEAN bPrevPrivilegeStatus;
    HARDERROR_RESPONSE response;
    
    if(pfnRtlAdjustPrivilege(SE_SHUTDOWN_PRIVILEGE, TRUE, TRUE, &bPrevPrivilegeStatus) == STATUS_NO_TOKEN) {
        pfnRtlAdjustPrivilege(SE_SHUTDOWN_PRIVILEGE, TRUE, FALSE, &bPrevPrivilegeStatus);
    }

    // Código de erro: FODEFODE!!!!!
    pfnNtRaiseHardError(0xF0DEF0DE, 0, 0, 0, OptionShutdownSystem, &response);
}