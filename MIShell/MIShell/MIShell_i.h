

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Aug 19 14:09:54 2010
 */
/* Compiler settings for MIShell.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __MIShell_i_h__
#define __MIShell_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMediaInfoShell_FWD_DEFINED__
#define __IMediaInfoShell_FWD_DEFINED__
typedef interface IMediaInfoShell IMediaInfoShell;
#endif 	/* __IMediaInfoShell_FWD_DEFINED__ */


#ifndef __MediaInfoShell_FWD_DEFINED__
#define __MediaInfoShell_FWD_DEFINED__

#ifdef __cplusplus
typedef class MediaInfoShell MediaInfoShell;
#else
typedef struct MediaInfoShell MediaInfoShell;
#endif /* __cplusplus */

#endif 	/* __MediaInfoShell_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IMediaInfoShell_INTERFACE_DEFINED__
#define __IMediaInfoShell_INTERFACE_DEFINED__

/* interface IMediaInfoShell */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IMediaInfoShell;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F25E23AC-D33F-44E8-9BC7-AF9672B62965")
    IMediaInfoShell : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IMediaInfoShellVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMediaInfoShell * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMediaInfoShell * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMediaInfoShell * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMediaInfoShell * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMediaInfoShell * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMediaInfoShell * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMediaInfoShell * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IMediaInfoShellVtbl;

    interface IMediaInfoShell
    {
        CONST_VTBL struct IMediaInfoShellVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMediaInfoShell_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMediaInfoShell_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMediaInfoShell_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMediaInfoShell_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMediaInfoShell_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMediaInfoShell_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMediaInfoShell_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMediaInfoShell_INTERFACE_DEFINED__ */



#ifndef __MIShellLib_LIBRARY_DEFINED__
#define __MIShellLib_LIBRARY_DEFINED__

/* library MIShellLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_MIShellLib;

EXTERN_C const CLSID CLSID_MediaInfoShell;

#ifdef __cplusplus

class DECLSPEC_UUID("DA444047-BFD7-484C-A409-24C219267BAB")
MediaInfoShell;
#endif
#endif /* __MIShellLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


