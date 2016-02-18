#ifdef HOOKDLL_EXPORTS
#define HOOKDLL_API __declspec(dllexport)
#else
#define HOOKDLL_API __declspec(dllimport)
#endif

HOOKDLL_API BOOL InstallHook(HWND hwndParent);

HOOKDLL_API BOOL UninstallHook();