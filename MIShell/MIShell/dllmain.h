// dllmain.h : Declaration of module class.

class CMIShellModule : public ATL::CAtlDllModuleT< CMIShellModule >
{
public :
	DECLARE_LIBID(LIBID_MIShellLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MISHELL, "{7C1ED0C7-B565-46C3-9CE5-98DBEAA4A312}")
};

extern class CMIShellModule _AtlModule;
