/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Fri Dec 23 10:13:04 2005
 */
/* Compiler settings for spatialreferenceidl.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
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

#ifndef __spatialreferenceidl_h__
#define __spatialreferenceidl_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __ISpatialReferenceInfo_FWD_DEFINED__
#define __ISpatialReferenceInfo_FWD_DEFINED__
typedef interface ISpatialReferenceInfo ISpatialReferenceInfo;
#endif 	/* __ISpatialReferenceInfo_FWD_DEFINED__ */


#ifndef __IUnit_FWD_DEFINED__
#define __IUnit_FWD_DEFINED__
typedef interface IUnit IUnit;
#endif 	/* __IUnit_FWD_DEFINED__ */


#ifndef __IAngularUnit_FWD_DEFINED__
#define __IAngularUnit_FWD_DEFINED__
typedef interface IAngularUnit IAngularUnit;
#endif 	/* __IAngularUnit_FWD_DEFINED__ */


#ifndef __ILinearUnit_FWD_DEFINED__
#define __ILinearUnit_FWD_DEFINED__
typedef interface ILinearUnit ILinearUnit;
#endif 	/* __ILinearUnit_FWD_DEFINED__ */


#ifndef __IEllipsoid_FWD_DEFINED__
#define __IEllipsoid_FWD_DEFINED__
typedef interface IEllipsoid IEllipsoid;
#endif 	/* __IEllipsoid_FWD_DEFINED__ */


#ifndef __IHorizontalDatum_FWD_DEFINED__
#define __IHorizontalDatum_FWD_DEFINED__
typedef interface IHorizontalDatum IHorizontalDatum;
#endif 	/* __IHorizontalDatum_FWD_DEFINED__ */


#ifndef __IPrimeMeridian_FWD_DEFINED__
#define __IPrimeMeridian_FWD_DEFINED__
typedef interface IPrimeMeridian IPrimeMeridian;
#endif 	/* __IPrimeMeridian_FWD_DEFINED__ */


#ifndef __ISpatialReference_FWD_DEFINED__
#define __ISpatialReference_FWD_DEFINED__
typedef interface ISpatialReference ISpatialReference;
#endif 	/* __ISpatialReference_FWD_DEFINED__ */


#ifndef __IGeodeticSpatialReference_FWD_DEFINED__
#define __IGeodeticSpatialReference_FWD_DEFINED__
typedef interface IGeodeticSpatialReference IGeodeticSpatialReference;
#endif 	/* __IGeodeticSpatialReference_FWD_DEFINED__ */


#ifndef __IGeographicCoordinateSystem_FWD_DEFINED__
#define __IGeographicCoordinateSystem_FWD_DEFINED__
typedef interface IGeographicCoordinateSystem IGeographicCoordinateSystem;
#endif 	/* __IGeographicCoordinateSystem_FWD_DEFINED__ */


#ifndef __IParameter_FWD_DEFINED__
#define __IParameter_FWD_DEFINED__
typedef interface IParameter IParameter;
#endif 	/* __IParameter_FWD_DEFINED__ */


#ifndef __IParameterInfo_FWD_DEFINED__
#define __IParameterInfo_FWD_DEFINED__
typedef interface IParameterInfo IParameterInfo;
#endif 	/* __IParameterInfo_FWD_DEFINED__ */


#ifndef __IGeographicTransform_FWD_DEFINED__
#define __IGeographicTransform_FWD_DEFINED__
typedef interface IGeographicTransform IGeographicTransform;
#endif 	/* __IGeographicTransform_FWD_DEFINED__ */


#ifndef __IProjection_FWD_DEFINED__
#define __IProjection_FWD_DEFINED__
typedef interface IProjection IProjection;
#endif 	/* __IProjection_FWD_DEFINED__ */


#ifndef __IProjectedCoordinateSystem_FWD_DEFINED__
#define __IProjectedCoordinateSystem_FWD_DEFINED__
typedef interface IProjectedCoordinateSystem IProjectedCoordinateSystem;
#endif 	/* __IProjectedCoordinateSystem_FWD_DEFINED__ */


#ifndef __ISpatialReferenceFactory_FWD_DEFINED__
#define __ISpatialReferenceFactory_FWD_DEFINED__
typedef interface ISpatialReferenceFactory ISpatialReferenceFactory;
#endif 	/* __ISpatialReferenceFactory_FWD_DEFINED__ */


#ifndef __ISpatialReferenceAuthorityFactory_FWD_DEFINED__
#define __ISpatialReferenceAuthorityFactory_FWD_DEFINED__
typedef interface ISpatialReferenceAuthorityFactory ISpatialReferenceAuthorityFactory;
#endif 	/* __ISpatialReferenceAuthorityFactory_FWD_DEFINED__ */


/* header files for imported files */
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_spatialreferenceidl_0000 */
/* [local] */ 

typedef struct  WKSPoint
    {
    double x;
    double y;
    }	WKSPoint;



extern RPC_IF_HANDLE __MIDL_itf_spatialreferenceidl_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_spatialreferenceidl_0000_v0_0_s_ifspec;

#ifndef __ISpatialReferenceInfo_INTERFACE_DEFINED__
#define __ISpatialReferenceInfo_INTERFACE_DEFINED__

/* interface ISpatialReferenceInfo */
/* [uuid][object] */ 


EXTERN_C const IID IID_ISpatialReferenceInfo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("bcca38a0-fe1c-11d0-ad87-080009b6f22b")
    ISpatialReferenceInfo : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR __RPC_FAR *name) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Name( 
            /* [in] */ BSTR name) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Authority( 
            /* [retval][out] */ BSTR __RPC_FAR *name) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Authority( 
            /* [in] */ BSTR name) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Code( 
            /* [retval][out] */ long __RPC_FAR *code) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Code( 
            /* [in] */ long code) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Alias( 
            /* [retval][out] */ BSTR __RPC_FAR *alias) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Alias( 
            /* [in] */ BSTR alias) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Abbreviation( 
            /* [retval][out] */ BSTR __RPC_FAR *abbrev) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Abbreviation( 
            /* [in] */ BSTR abbrev) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Remarks( 
            /* [retval][out] */ BSTR __RPC_FAR *remarks) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Remarks( 
            /* [in] */ BSTR remarks) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_WellKnownText( 
            /* [retval][out] */ BSTR __RPC_FAR *wkt) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISpatialReferenceInfoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISpatialReferenceInfo __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISpatialReferenceInfo __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISpatialReferenceInfo __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            ISpatialReferenceInfo __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Name )( 
            ISpatialReferenceInfo __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Authority )( 
            ISpatialReferenceInfo __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Authority )( 
            ISpatialReferenceInfo __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Code )( 
            ISpatialReferenceInfo __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *code);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Code )( 
            ISpatialReferenceInfo __RPC_FAR * This,
            /* [in] */ long code);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Alias )( 
            ISpatialReferenceInfo __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *alias);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Alias )( 
            ISpatialReferenceInfo __RPC_FAR * This,
            /* [in] */ BSTR alias);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Abbreviation )( 
            ISpatialReferenceInfo __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *abbrev);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Abbreviation )( 
            ISpatialReferenceInfo __RPC_FAR * This,
            /* [in] */ BSTR abbrev);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Remarks )( 
            ISpatialReferenceInfo __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *remarks);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Remarks )( 
            ISpatialReferenceInfo __RPC_FAR * This,
            /* [in] */ BSTR remarks);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WellKnownText )( 
            ISpatialReferenceInfo __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *wkt);
        
        END_INTERFACE
    } ISpatialReferenceInfoVtbl;

    interface ISpatialReferenceInfo
    {
        CONST_VTBL struct ISpatialReferenceInfoVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISpatialReferenceInfo_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISpatialReferenceInfo_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISpatialReferenceInfo_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISpatialReferenceInfo_get_Name(This,name)	\
    (This)->lpVtbl -> get_Name(This,name)

#define ISpatialReferenceInfo_put_Name(This,name)	\
    (This)->lpVtbl -> put_Name(This,name)

#define ISpatialReferenceInfo_get_Authority(This,name)	\
    (This)->lpVtbl -> get_Authority(This,name)

#define ISpatialReferenceInfo_put_Authority(This,name)	\
    (This)->lpVtbl -> put_Authority(This,name)

#define ISpatialReferenceInfo_get_Code(This,code)	\
    (This)->lpVtbl -> get_Code(This,code)

#define ISpatialReferenceInfo_put_Code(This,code)	\
    (This)->lpVtbl -> put_Code(This,code)

#define ISpatialReferenceInfo_get_Alias(This,alias)	\
    (This)->lpVtbl -> get_Alias(This,alias)

#define ISpatialReferenceInfo_put_Alias(This,alias)	\
    (This)->lpVtbl -> put_Alias(This,alias)

#define ISpatialReferenceInfo_get_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> get_Abbreviation(This,abbrev)

#define ISpatialReferenceInfo_put_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> put_Abbreviation(This,abbrev)

#define ISpatialReferenceInfo_get_Remarks(This,remarks)	\
    (This)->lpVtbl -> get_Remarks(This,remarks)

#define ISpatialReferenceInfo_put_Remarks(This,remarks)	\
    (This)->lpVtbl -> put_Remarks(This,remarks)

#define ISpatialReferenceInfo_get_WellKnownText(This,wkt)	\
    (This)->lpVtbl -> get_WellKnownText(This,wkt)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE ISpatialReferenceInfo_get_Name_Proxy( 
    ISpatialReferenceInfo __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *name);


void __RPC_STUB ISpatialReferenceInfo_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE ISpatialReferenceInfo_put_Name_Proxy( 
    ISpatialReferenceInfo __RPC_FAR * This,
    /* [in] */ BSTR name);


void __RPC_STUB ISpatialReferenceInfo_put_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE ISpatialReferenceInfo_get_Authority_Proxy( 
    ISpatialReferenceInfo __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *name);


void __RPC_STUB ISpatialReferenceInfo_get_Authority_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE ISpatialReferenceInfo_put_Authority_Proxy( 
    ISpatialReferenceInfo __RPC_FAR * This,
    /* [in] */ BSTR name);


void __RPC_STUB ISpatialReferenceInfo_put_Authority_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE ISpatialReferenceInfo_get_Code_Proxy( 
    ISpatialReferenceInfo __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *code);


void __RPC_STUB ISpatialReferenceInfo_get_Code_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE ISpatialReferenceInfo_put_Code_Proxy( 
    ISpatialReferenceInfo __RPC_FAR * This,
    /* [in] */ long code);


void __RPC_STUB ISpatialReferenceInfo_put_Code_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE ISpatialReferenceInfo_get_Alias_Proxy( 
    ISpatialReferenceInfo __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *alias);


void __RPC_STUB ISpatialReferenceInfo_get_Alias_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE ISpatialReferenceInfo_put_Alias_Proxy( 
    ISpatialReferenceInfo __RPC_FAR * This,
    /* [in] */ BSTR alias);


void __RPC_STUB ISpatialReferenceInfo_put_Alias_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE ISpatialReferenceInfo_get_Abbreviation_Proxy( 
    ISpatialReferenceInfo __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *abbrev);


void __RPC_STUB ISpatialReferenceInfo_get_Abbreviation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE ISpatialReferenceInfo_put_Abbreviation_Proxy( 
    ISpatialReferenceInfo __RPC_FAR * This,
    /* [in] */ BSTR abbrev);


void __RPC_STUB ISpatialReferenceInfo_put_Abbreviation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE ISpatialReferenceInfo_get_Remarks_Proxy( 
    ISpatialReferenceInfo __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *remarks);


void __RPC_STUB ISpatialReferenceInfo_get_Remarks_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE ISpatialReferenceInfo_put_Remarks_Proxy( 
    ISpatialReferenceInfo __RPC_FAR * This,
    /* [in] */ BSTR remarks);


void __RPC_STUB ISpatialReferenceInfo_put_Remarks_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE ISpatialReferenceInfo_get_WellKnownText_Proxy( 
    ISpatialReferenceInfo __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *wkt);


void __RPC_STUB ISpatialReferenceInfo_get_WellKnownText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISpatialReferenceInfo_INTERFACE_DEFINED__ */


#ifndef __IUnit_INTERFACE_DEFINED__
#define __IUnit_INTERFACE_DEFINED__

/* interface IUnit */
/* [uuid][object] */ 


EXTERN_C const IID IID_IUnit;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("221733b0-fe1d-11d0-ad87-080009b6f22b")
    IUnit : public ISpatialReferenceInfo
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IUnitVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IUnit __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IUnit __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IUnit __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IUnit __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Name )( 
            IUnit __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Authority )( 
            IUnit __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Authority )( 
            IUnit __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Code )( 
            IUnit __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *code);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Code )( 
            IUnit __RPC_FAR * This,
            /* [in] */ long code);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Alias )( 
            IUnit __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *alias);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Alias )( 
            IUnit __RPC_FAR * This,
            /* [in] */ BSTR alias);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Abbreviation )( 
            IUnit __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *abbrev);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Abbreviation )( 
            IUnit __RPC_FAR * This,
            /* [in] */ BSTR abbrev);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Remarks )( 
            IUnit __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *remarks);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Remarks )( 
            IUnit __RPC_FAR * This,
            /* [in] */ BSTR remarks);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WellKnownText )( 
            IUnit __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *wkt);
        
        END_INTERFACE
    } IUnitVtbl;

    interface IUnit
    {
        CONST_VTBL struct IUnitVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IUnit_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IUnit_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IUnit_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IUnit_get_Name(This,name)	\
    (This)->lpVtbl -> get_Name(This,name)

#define IUnit_put_Name(This,name)	\
    (This)->lpVtbl -> put_Name(This,name)

#define IUnit_get_Authority(This,name)	\
    (This)->lpVtbl -> get_Authority(This,name)

#define IUnit_put_Authority(This,name)	\
    (This)->lpVtbl -> put_Authority(This,name)

#define IUnit_get_Code(This,code)	\
    (This)->lpVtbl -> get_Code(This,code)

#define IUnit_put_Code(This,code)	\
    (This)->lpVtbl -> put_Code(This,code)

#define IUnit_get_Alias(This,alias)	\
    (This)->lpVtbl -> get_Alias(This,alias)

#define IUnit_put_Alias(This,alias)	\
    (This)->lpVtbl -> put_Alias(This,alias)

#define IUnit_get_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> get_Abbreviation(This,abbrev)

#define IUnit_put_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> put_Abbreviation(This,abbrev)

#define IUnit_get_Remarks(This,remarks)	\
    (This)->lpVtbl -> get_Remarks(This,remarks)

#define IUnit_put_Remarks(This,remarks)	\
    (This)->lpVtbl -> put_Remarks(This,remarks)

#define IUnit_get_WellKnownText(This,wkt)	\
    (This)->lpVtbl -> get_WellKnownText(This,wkt)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IUnit_INTERFACE_DEFINED__ */


#ifndef __IAngularUnit_INTERFACE_DEFINED__
#define __IAngularUnit_INTERFACE_DEFINED__

/* interface IAngularUnit */
/* [uuid][object] */ 


EXTERN_C const IID IID_IAngularUnit;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4febc550-fe1d-11d0-ad87-080009b6f22b")
    IAngularUnit : public IUnit
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_RadiansPerUnit( 
            /* [retval][out] */ double __RPC_FAR *factor) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_RadiansPerUnit( 
            /* [in] */ double factor) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAngularUnitVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAngularUnit __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAngularUnit __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAngularUnit __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IAngularUnit __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Name )( 
            IAngularUnit __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Authority )( 
            IAngularUnit __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Authority )( 
            IAngularUnit __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Code )( 
            IAngularUnit __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *code);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Code )( 
            IAngularUnit __RPC_FAR * This,
            /* [in] */ long code);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Alias )( 
            IAngularUnit __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *alias);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Alias )( 
            IAngularUnit __RPC_FAR * This,
            /* [in] */ BSTR alias);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Abbreviation )( 
            IAngularUnit __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *abbrev);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Abbreviation )( 
            IAngularUnit __RPC_FAR * This,
            /* [in] */ BSTR abbrev);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Remarks )( 
            IAngularUnit __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *remarks);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Remarks )( 
            IAngularUnit __RPC_FAR * This,
            /* [in] */ BSTR remarks);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WellKnownText )( 
            IAngularUnit __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *wkt);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_RadiansPerUnit )( 
            IAngularUnit __RPC_FAR * This,
            /* [retval][out] */ double __RPC_FAR *factor);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_RadiansPerUnit )( 
            IAngularUnit __RPC_FAR * This,
            /* [in] */ double factor);
        
        END_INTERFACE
    } IAngularUnitVtbl;

    interface IAngularUnit
    {
        CONST_VTBL struct IAngularUnitVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAngularUnit_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAngularUnit_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAngularUnit_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAngularUnit_get_Name(This,name)	\
    (This)->lpVtbl -> get_Name(This,name)

#define IAngularUnit_put_Name(This,name)	\
    (This)->lpVtbl -> put_Name(This,name)

#define IAngularUnit_get_Authority(This,name)	\
    (This)->lpVtbl -> get_Authority(This,name)

#define IAngularUnit_put_Authority(This,name)	\
    (This)->lpVtbl -> put_Authority(This,name)

#define IAngularUnit_get_Code(This,code)	\
    (This)->lpVtbl -> get_Code(This,code)

#define IAngularUnit_put_Code(This,code)	\
    (This)->lpVtbl -> put_Code(This,code)

#define IAngularUnit_get_Alias(This,alias)	\
    (This)->lpVtbl -> get_Alias(This,alias)

#define IAngularUnit_put_Alias(This,alias)	\
    (This)->lpVtbl -> put_Alias(This,alias)

#define IAngularUnit_get_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> get_Abbreviation(This,abbrev)

#define IAngularUnit_put_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> put_Abbreviation(This,abbrev)

#define IAngularUnit_get_Remarks(This,remarks)	\
    (This)->lpVtbl -> get_Remarks(This,remarks)

#define IAngularUnit_put_Remarks(This,remarks)	\
    (This)->lpVtbl -> put_Remarks(This,remarks)

#define IAngularUnit_get_WellKnownText(This,wkt)	\
    (This)->lpVtbl -> get_WellKnownText(This,wkt)



#define IAngularUnit_get_RadiansPerUnit(This,factor)	\
    (This)->lpVtbl -> get_RadiansPerUnit(This,factor)

#define IAngularUnit_put_RadiansPerUnit(This,factor)	\
    (This)->lpVtbl -> put_RadiansPerUnit(This,factor)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IAngularUnit_get_RadiansPerUnit_Proxy( 
    IAngularUnit __RPC_FAR * This,
    /* [retval][out] */ double __RPC_FAR *factor);


void __RPC_STUB IAngularUnit_get_RadiansPerUnit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IAngularUnit_put_RadiansPerUnit_Proxy( 
    IAngularUnit __RPC_FAR * This,
    /* [in] */ double factor);


void __RPC_STUB IAngularUnit_put_RadiansPerUnit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAngularUnit_INTERFACE_DEFINED__ */


#ifndef __ILinearUnit_INTERFACE_DEFINED__
#define __ILinearUnit_INTERFACE_DEFINED__

/* interface ILinearUnit */
/* [uuid][object] */ 


EXTERN_C const IID IID_ILinearUnit;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("80855df0-fe1d-11d0-ad87-080009b6f22b")
    ILinearUnit : public IUnit
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_MetersPerUnit( 
            /* [retval][out] */ double __RPC_FAR *factor) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_MetersPerUnit( 
            /* [in] */ double factor) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILinearUnitVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ILinearUnit __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ILinearUnit __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ILinearUnit __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            ILinearUnit __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Name )( 
            ILinearUnit __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Authority )( 
            ILinearUnit __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Authority )( 
            ILinearUnit __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Code )( 
            ILinearUnit __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *code);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Code )( 
            ILinearUnit __RPC_FAR * This,
            /* [in] */ long code);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Alias )( 
            ILinearUnit __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *alias);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Alias )( 
            ILinearUnit __RPC_FAR * This,
            /* [in] */ BSTR alias);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Abbreviation )( 
            ILinearUnit __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *abbrev);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Abbreviation )( 
            ILinearUnit __RPC_FAR * This,
            /* [in] */ BSTR abbrev);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Remarks )( 
            ILinearUnit __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *remarks);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Remarks )( 
            ILinearUnit __RPC_FAR * This,
            /* [in] */ BSTR remarks);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WellKnownText )( 
            ILinearUnit __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *wkt);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_MetersPerUnit )( 
            ILinearUnit __RPC_FAR * This,
            /* [retval][out] */ double __RPC_FAR *factor);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_MetersPerUnit )( 
            ILinearUnit __RPC_FAR * This,
            /* [in] */ double factor);
        
        END_INTERFACE
    } ILinearUnitVtbl;

    interface ILinearUnit
    {
        CONST_VTBL struct ILinearUnitVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILinearUnit_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ILinearUnit_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ILinearUnit_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ILinearUnit_get_Name(This,name)	\
    (This)->lpVtbl -> get_Name(This,name)

#define ILinearUnit_put_Name(This,name)	\
    (This)->lpVtbl -> put_Name(This,name)

#define ILinearUnit_get_Authority(This,name)	\
    (This)->lpVtbl -> get_Authority(This,name)

#define ILinearUnit_put_Authority(This,name)	\
    (This)->lpVtbl -> put_Authority(This,name)

#define ILinearUnit_get_Code(This,code)	\
    (This)->lpVtbl -> get_Code(This,code)

#define ILinearUnit_put_Code(This,code)	\
    (This)->lpVtbl -> put_Code(This,code)

#define ILinearUnit_get_Alias(This,alias)	\
    (This)->lpVtbl -> get_Alias(This,alias)

#define ILinearUnit_put_Alias(This,alias)	\
    (This)->lpVtbl -> put_Alias(This,alias)

#define ILinearUnit_get_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> get_Abbreviation(This,abbrev)

#define ILinearUnit_put_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> put_Abbreviation(This,abbrev)

#define ILinearUnit_get_Remarks(This,remarks)	\
    (This)->lpVtbl -> get_Remarks(This,remarks)

#define ILinearUnit_put_Remarks(This,remarks)	\
    (This)->lpVtbl -> put_Remarks(This,remarks)

#define ILinearUnit_get_WellKnownText(This,wkt)	\
    (This)->lpVtbl -> get_WellKnownText(This,wkt)



#define ILinearUnit_get_MetersPerUnit(This,factor)	\
    (This)->lpVtbl -> get_MetersPerUnit(This,factor)

#define ILinearUnit_put_MetersPerUnit(This,factor)	\
    (This)->lpVtbl -> put_MetersPerUnit(This,factor)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE ILinearUnit_get_MetersPerUnit_Proxy( 
    ILinearUnit __RPC_FAR * This,
    /* [retval][out] */ double __RPC_FAR *factor);


void __RPC_STUB ILinearUnit_get_MetersPerUnit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE ILinearUnit_put_MetersPerUnit_Proxy( 
    ILinearUnit __RPC_FAR * This,
    /* [in] */ double factor);


void __RPC_STUB ILinearUnit_put_MetersPerUnit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ILinearUnit_INTERFACE_DEFINED__ */


#ifndef __IEllipsoid_INTERFACE_DEFINED__
#define __IEllipsoid_INTERFACE_DEFINED__

/* interface IEllipsoid */
/* [uuid][object] */ 


EXTERN_C const IID IID_IEllipsoid;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("ce7266c0-fe1d-11d0-ad87-080009b6f22b")
    IEllipsoid : public ISpatialReferenceInfo
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_SemiMajorAxis( 
            /* [retval][out] */ double __RPC_FAR *axis) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_SemiMajorAxis( 
            /* [in] */ double axis) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_SemiMinorAxis( 
            /* [retval][out] */ double __RPC_FAR *axis) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_SemiMinorAxis( 
            /* [in] */ double axis) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_InverseFlattening( 
            /* [retval][out] */ double __RPC_FAR *invFlat) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_InverseFlattening( 
            /* [in] */ double invFlat) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_AxisUnit( 
            /* [retval][out] */ ILinearUnit __RPC_FAR *__RPC_FAR *unit) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_AxisUnit( 
            /* [in] */ ILinearUnit __RPC_FAR *unit) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEllipsoidVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IEllipsoid __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IEllipsoid __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IEllipsoid __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IEllipsoid __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Name )( 
            IEllipsoid __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Authority )( 
            IEllipsoid __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Authority )( 
            IEllipsoid __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Code )( 
            IEllipsoid __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *code);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Code )( 
            IEllipsoid __RPC_FAR * This,
            /* [in] */ long code);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Alias )( 
            IEllipsoid __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *alias);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Alias )( 
            IEllipsoid __RPC_FAR * This,
            /* [in] */ BSTR alias);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Abbreviation )( 
            IEllipsoid __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *abbrev);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Abbreviation )( 
            IEllipsoid __RPC_FAR * This,
            /* [in] */ BSTR abbrev);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Remarks )( 
            IEllipsoid __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *remarks);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Remarks )( 
            IEllipsoid __RPC_FAR * This,
            /* [in] */ BSTR remarks);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WellKnownText )( 
            IEllipsoid __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *wkt);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SemiMajorAxis )( 
            IEllipsoid __RPC_FAR * This,
            /* [retval][out] */ double __RPC_FAR *axis);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SemiMajorAxis )( 
            IEllipsoid __RPC_FAR * This,
            /* [in] */ double axis);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SemiMinorAxis )( 
            IEllipsoid __RPC_FAR * This,
            /* [retval][out] */ double __RPC_FAR *axis);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SemiMinorAxis )( 
            IEllipsoid __RPC_FAR * This,
            /* [in] */ double axis);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_InverseFlattening )( 
            IEllipsoid __RPC_FAR * This,
            /* [retval][out] */ double __RPC_FAR *invFlat);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_InverseFlattening )( 
            IEllipsoid __RPC_FAR * This,
            /* [in] */ double invFlat);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_AxisUnit )( 
            IEllipsoid __RPC_FAR * This,
            /* [retval][out] */ ILinearUnit __RPC_FAR *__RPC_FAR *unit);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_AxisUnit )( 
            IEllipsoid __RPC_FAR * This,
            /* [in] */ ILinearUnit __RPC_FAR *unit);
        
        END_INTERFACE
    } IEllipsoidVtbl;

    interface IEllipsoid
    {
        CONST_VTBL struct IEllipsoidVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEllipsoid_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEllipsoid_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEllipsoid_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEllipsoid_get_Name(This,name)	\
    (This)->lpVtbl -> get_Name(This,name)

#define IEllipsoid_put_Name(This,name)	\
    (This)->lpVtbl -> put_Name(This,name)

#define IEllipsoid_get_Authority(This,name)	\
    (This)->lpVtbl -> get_Authority(This,name)

#define IEllipsoid_put_Authority(This,name)	\
    (This)->lpVtbl -> put_Authority(This,name)

#define IEllipsoid_get_Code(This,code)	\
    (This)->lpVtbl -> get_Code(This,code)

#define IEllipsoid_put_Code(This,code)	\
    (This)->lpVtbl -> put_Code(This,code)

#define IEllipsoid_get_Alias(This,alias)	\
    (This)->lpVtbl -> get_Alias(This,alias)

#define IEllipsoid_put_Alias(This,alias)	\
    (This)->lpVtbl -> put_Alias(This,alias)

#define IEllipsoid_get_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> get_Abbreviation(This,abbrev)

#define IEllipsoid_put_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> put_Abbreviation(This,abbrev)

#define IEllipsoid_get_Remarks(This,remarks)	\
    (This)->lpVtbl -> get_Remarks(This,remarks)

#define IEllipsoid_put_Remarks(This,remarks)	\
    (This)->lpVtbl -> put_Remarks(This,remarks)

#define IEllipsoid_get_WellKnownText(This,wkt)	\
    (This)->lpVtbl -> get_WellKnownText(This,wkt)


#define IEllipsoid_get_SemiMajorAxis(This,axis)	\
    (This)->lpVtbl -> get_SemiMajorAxis(This,axis)

#define IEllipsoid_put_SemiMajorAxis(This,axis)	\
    (This)->lpVtbl -> put_SemiMajorAxis(This,axis)

#define IEllipsoid_get_SemiMinorAxis(This,axis)	\
    (This)->lpVtbl -> get_SemiMinorAxis(This,axis)

#define IEllipsoid_put_SemiMinorAxis(This,axis)	\
    (This)->lpVtbl -> put_SemiMinorAxis(This,axis)

#define IEllipsoid_get_InverseFlattening(This,invFlat)	\
    (This)->lpVtbl -> get_InverseFlattening(This,invFlat)

#define IEllipsoid_put_InverseFlattening(This,invFlat)	\
    (This)->lpVtbl -> put_InverseFlattening(This,invFlat)

#define IEllipsoid_get_AxisUnit(This,unit)	\
    (This)->lpVtbl -> get_AxisUnit(This,unit)

#define IEllipsoid_put_AxisUnit(This,unit)	\
    (This)->lpVtbl -> put_AxisUnit(This,unit)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IEllipsoid_get_SemiMajorAxis_Proxy( 
    IEllipsoid __RPC_FAR * This,
    /* [retval][out] */ double __RPC_FAR *axis);


void __RPC_STUB IEllipsoid_get_SemiMajorAxis_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IEllipsoid_put_SemiMajorAxis_Proxy( 
    IEllipsoid __RPC_FAR * This,
    /* [in] */ double axis);


void __RPC_STUB IEllipsoid_put_SemiMajorAxis_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IEllipsoid_get_SemiMinorAxis_Proxy( 
    IEllipsoid __RPC_FAR * This,
    /* [retval][out] */ double __RPC_FAR *axis);


void __RPC_STUB IEllipsoid_get_SemiMinorAxis_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IEllipsoid_put_SemiMinorAxis_Proxy( 
    IEllipsoid __RPC_FAR * This,
    /* [in] */ double axis);


void __RPC_STUB IEllipsoid_put_SemiMinorAxis_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IEllipsoid_get_InverseFlattening_Proxy( 
    IEllipsoid __RPC_FAR * This,
    /* [retval][out] */ double __RPC_FAR *invFlat);


void __RPC_STUB IEllipsoid_get_InverseFlattening_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IEllipsoid_put_InverseFlattening_Proxy( 
    IEllipsoid __RPC_FAR * This,
    /* [in] */ double invFlat);


void __RPC_STUB IEllipsoid_put_InverseFlattening_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IEllipsoid_get_AxisUnit_Proxy( 
    IEllipsoid __RPC_FAR * This,
    /* [retval][out] */ ILinearUnit __RPC_FAR *__RPC_FAR *unit);


void __RPC_STUB IEllipsoid_get_AxisUnit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IEllipsoid_put_AxisUnit_Proxy( 
    IEllipsoid __RPC_FAR * This,
    /* [in] */ ILinearUnit __RPC_FAR *unit);


void __RPC_STUB IEllipsoid_put_AxisUnit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEllipsoid_INTERFACE_DEFINED__ */


#ifndef __IHorizontalDatum_INTERFACE_DEFINED__
#define __IHorizontalDatum_INTERFACE_DEFINED__

/* interface IHorizontalDatum */
/* [uuid][object] */ 


EXTERN_C const IID IID_IHorizontalDatum;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("f699c510-fe1d-11d0-ad87-080009b6f22b")
    IHorizontalDatum : public ISpatialReferenceInfo
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Ellipsoid( 
            /* [retval][out] */ IEllipsoid __RPC_FAR *__RPC_FAR *ellipsoid) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Ellipsoid( 
            /* [in] */ IEllipsoid __RPC_FAR *ellipsoid) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IHorizontalDatumVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IHorizontalDatum __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IHorizontalDatum __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IHorizontalDatum __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IHorizontalDatum __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Name )( 
            IHorizontalDatum __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Authority )( 
            IHorizontalDatum __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Authority )( 
            IHorizontalDatum __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Code )( 
            IHorizontalDatum __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *code);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Code )( 
            IHorizontalDatum __RPC_FAR * This,
            /* [in] */ long code);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Alias )( 
            IHorizontalDatum __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *alias);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Alias )( 
            IHorizontalDatum __RPC_FAR * This,
            /* [in] */ BSTR alias);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Abbreviation )( 
            IHorizontalDatum __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *abbrev);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Abbreviation )( 
            IHorizontalDatum __RPC_FAR * This,
            /* [in] */ BSTR abbrev);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Remarks )( 
            IHorizontalDatum __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *remarks);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Remarks )( 
            IHorizontalDatum __RPC_FAR * This,
            /* [in] */ BSTR remarks);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WellKnownText )( 
            IHorizontalDatum __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *wkt);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Ellipsoid )( 
            IHorizontalDatum __RPC_FAR * This,
            /* [retval][out] */ IEllipsoid __RPC_FAR *__RPC_FAR *ellipsoid);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Ellipsoid )( 
            IHorizontalDatum __RPC_FAR * This,
            /* [in] */ IEllipsoid __RPC_FAR *ellipsoid);
        
        END_INTERFACE
    } IHorizontalDatumVtbl;

    interface IHorizontalDatum
    {
        CONST_VTBL struct IHorizontalDatumVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IHorizontalDatum_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IHorizontalDatum_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IHorizontalDatum_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IHorizontalDatum_get_Name(This,name)	\
    (This)->lpVtbl -> get_Name(This,name)

#define IHorizontalDatum_put_Name(This,name)	\
    (This)->lpVtbl -> put_Name(This,name)

#define IHorizontalDatum_get_Authority(This,name)	\
    (This)->lpVtbl -> get_Authority(This,name)

#define IHorizontalDatum_put_Authority(This,name)	\
    (This)->lpVtbl -> put_Authority(This,name)

#define IHorizontalDatum_get_Code(This,code)	\
    (This)->lpVtbl -> get_Code(This,code)

#define IHorizontalDatum_put_Code(This,code)	\
    (This)->lpVtbl -> put_Code(This,code)

#define IHorizontalDatum_get_Alias(This,alias)	\
    (This)->lpVtbl -> get_Alias(This,alias)

#define IHorizontalDatum_put_Alias(This,alias)	\
    (This)->lpVtbl -> put_Alias(This,alias)

#define IHorizontalDatum_get_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> get_Abbreviation(This,abbrev)

#define IHorizontalDatum_put_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> put_Abbreviation(This,abbrev)

#define IHorizontalDatum_get_Remarks(This,remarks)	\
    (This)->lpVtbl -> get_Remarks(This,remarks)

#define IHorizontalDatum_put_Remarks(This,remarks)	\
    (This)->lpVtbl -> put_Remarks(This,remarks)

#define IHorizontalDatum_get_WellKnownText(This,wkt)	\
    (This)->lpVtbl -> get_WellKnownText(This,wkt)


#define IHorizontalDatum_get_Ellipsoid(This,ellipsoid)	\
    (This)->lpVtbl -> get_Ellipsoid(This,ellipsoid)

#define IHorizontalDatum_put_Ellipsoid(This,ellipsoid)	\
    (This)->lpVtbl -> put_Ellipsoid(This,ellipsoid)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IHorizontalDatum_get_Ellipsoid_Proxy( 
    IHorizontalDatum __RPC_FAR * This,
    /* [retval][out] */ IEllipsoid __RPC_FAR *__RPC_FAR *ellipsoid);


void __RPC_STUB IHorizontalDatum_get_Ellipsoid_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IHorizontalDatum_put_Ellipsoid_Proxy( 
    IHorizontalDatum __RPC_FAR * This,
    /* [in] */ IEllipsoid __RPC_FAR *ellipsoid);


void __RPC_STUB IHorizontalDatum_put_Ellipsoid_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IHorizontalDatum_INTERFACE_DEFINED__ */


#ifndef __IPrimeMeridian_INTERFACE_DEFINED__
#define __IPrimeMeridian_INTERFACE_DEFINED__

/* interface IPrimeMeridian */
/* [uuid][object] */ 


EXTERN_C const IID IID_IPrimeMeridian;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("15129940-fe1e-11d0-ad87-080009b6f22b")
    IPrimeMeridian : public ISpatialReferenceInfo
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Longitude( 
            /* [retval][out] */ double __RPC_FAR *longitude) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Longitude( 
            /* [in] */ double longitude) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_AngularUnit( 
            /* [retval][out] */ IAngularUnit __RPC_FAR *__RPC_FAR *unit) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_AngularUnit( 
            /* [in] */ IAngularUnit __RPC_FAR *unit) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPrimeMeridianVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPrimeMeridian __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPrimeMeridian __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPrimeMeridian __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IPrimeMeridian __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Name )( 
            IPrimeMeridian __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Authority )( 
            IPrimeMeridian __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Authority )( 
            IPrimeMeridian __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Code )( 
            IPrimeMeridian __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *code);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Code )( 
            IPrimeMeridian __RPC_FAR * This,
            /* [in] */ long code);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Alias )( 
            IPrimeMeridian __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *alias);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Alias )( 
            IPrimeMeridian __RPC_FAR * This,
            /* [in] */ BSTR alias);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Abbreviation )( 
            IPrimeMeridian __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *abbrev);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Abbreviation )( 
            IPrimeMeridian __RPC_FAR * This,
            /* [in] */ BSTR abbrev);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Remarks )( 
            IPrimeMeridian __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *remarks);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Remarks )( 
            IPrimeMeridian __RPC_FAR * This,
            /* [in] */ BSTR remarks);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WellKnownText )( 
            IPrimeMeridian __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *wkt);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Longitude )( 
            IPrimeMeridian __RPC_FAR * This,
            /* [retval][out] */ double __RPC_FAR *longitude);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Longitude )( 
            IPrimeMeridian __RPC_FAR * This,
            /* [in] */ double longitude);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_AngularUnit )( 
            IPrimeMeridian __RPC_FAR * This,
            /* [retval][out] */ IAngularUnit __RPC_FAR *__RPC_FAR *unit);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_AngularUnit )( 
            IPrimeMeridian __RPC_FAR * This,
            /* [in] */ IAngularUnit __RPC_FAR *unit);
        
        END_INTERFACE
    } IPrimeMeridianVtbl;

    interface IPrimeMeridian
    {
        CONST_VTBL struct IPrimeMeridianVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPrimeMeridian_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPrimeMeridian_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPrimeMeridian_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPrimeMeridian_get_Name(This,name)	\
    (This)->lpVtbl -> get_Name(This,name)

#define IPrimeMeridian_put_Name(This,name)	\
    (This)->lpVtbl -> put_Name(This,name)

#define IPrimeMeridian_get_Authority(This,name)	\
    (This)->lpVtbl -> get_Authority(This,name)

#define IPrimeMeridian_put_Authority(This,name)	\
    (This)->lpVtbl -> put_Authority(This,name)

#define IPrimeMeridian_get_Code(This,code)	\
    (This)->lpVtbl -> get_Code(This,code)

#define IPrimeMeridian_put_Code(This,code)	\
    (This)->lpVtbl -> put_Code(This,code)

#define IPrimeMeridian_get_Alias(This,alias)	\
    (This)->lpVtbl -> get_Alias(This,alias)

#define IPrimeMeridian_put_Alias(This,alias)	\
    (This)->lpVtbl -> put_Alias(This,alias)

#define IPrimeMeridian_get_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> get_Abbreviation(This,abbrev)

#define IPrimeMeridian_put_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> put_Abbreviation(This,abbrev)

#define IPrimeMeridian_get_Remarks(This,remarks)	\
    (This)->lpVtbl -> get_Remarks(This,remarks)

#define IPrimeMeridian_put_Remarks(This,remarks)	\
    (This)->lpVtbl -> put_Remarks(This,remarks)

#define IPrimeMeridian_get_WellKnownText(This,wkt)	\
    (This)->lpVtbl -> get_WellKnownText(This,wkt)


#define IPrimeMeridian_get_Longitude(This,longitude)	\
    (This)->lpVtbl -> get_Longitude(This,longitude)

#define IPrimeMeridian_put_Longitude(This,longitude)	\
    (This)->lpVtbl -> put_Longitude(This,longitude)

#define IPrimeMeridian_get_AngularUnit(This,unit)	\
    (This)->lpVtbl -> get_AngularUnit(This,unit)

#define IPrimeMeridian_put_AngularUnit(This,unit)	\
    (This)->lpVtbl -> put_AngularUnit(This,unit)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IPrimeMeridian_get_Longitude_Proxy( 
    IPrimeMeridian __RPC_FAR * This,
    /* [retval][out] */ double __RPC_FAR *longitude);


void __RPC_STUB IPrimeMeridian_get_Longitude_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPrimeMeridian_put_Longitude_Proxy( 
    IPrimeMeridian __RPC_FAR * This,
    /* [in] */ double longitude);


void __RPC_STUB IPrimeMeridian_put_Longitude_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPrimeMeridian_get_AngularUnit_Proxy( 
    IPrimeMeridian __RPC_FAR * This,
    /* [retval][out] */ IAngularUnit __RPC_FAR *__RPC_FAR *unit);


void __RPC_STUB IPrimeMeridian_get_AngularUnit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IPrimeMeridian_put_AngularUnit_Proxy( 
    IPrimeMeridian __RPC_FAR * This,
    /* [in] */ IAngularUnit __RPC_FAR *unit);


void __RPC_STUB IPrimeMeridian_put_AngularUnit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPrimeMeridian_INTERFACE_DEFINED__ */


#ifndef __ISpatialReference_INTERFACE_DEFINED__
#define __ISpatialReference_INTERFACE_DEFINED__

/* interface ISpatialReference */
/* [uuid][object] */ 


EXTERN_C const IID IID_ISpatialReference;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4c4c5c00-fe1e-11d0-ad87-080009b6f22b")
    ISpatialReference : public ISpatialReferenceInfo
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ISpatialReferenceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISpatialReference __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISpatialReference __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISpatialReference __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            ISpatialReference __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Name )( 
            ISpatialReference __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Authority )( 
            ISpatialReference __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Authority )( 
            ISpatialReference __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Code )( 
            ISpatialReference __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *code);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Code )( 
            ISpatialReference __RPC_FAR * This,
            /* [in] */ long code);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Alias )( 
            ISpatialReference __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *alias);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Alias )( 
            ISpatialReference __RPC_FAR * This,
            /* [in] */ BSTR alias);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Abbreviation )( 
            ISpatialReference __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *abbrev);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Abbreviation )( 
            ISpatialReference __RPC_FAR * This,
            /* [in] */ BSTR abbrev);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Remarks )( 
            ISpatialReference __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *remarks);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Remarks )( 
            ISpatialReference __RPC_FAR * This,
            /* [in] */ BSTR remarks);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WellKnownText )( 
            ISpatialReference __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *wkt);
        
        END_INTERFACE
    } ISpatialReferenceVtbl;

    interface ISpatialReference
    {
        CONST_VTBL struct ISpatialReferenceVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISpatialReference_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISpatialReference_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISpatialReference_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISpatialReference_get_Name(This,name)	\
    (This)->lpVtbl -> get_Name(This,name)

#define ISpatialReference_put_Name(This,name)	\
    (This)->lpVtbl -> put_Name(This,name)

#define ISpatialReference_get_Authority(This,name)	\
    (This)->lpVtbl -> get_Authority(This,name)

#define ISpatialReference_put_Authority(This,name)	\
    (This)->lpVtbl -> put_Authority(This,name)

#define ISpatialReference_get_Code(This,code)	\
    (This)->lpVtbl -> get_Code(This,code)

#define ISpatialReference_put_Code(This,code)	\
    (This)->lpVtbl -> put_Code(This,code)

#define ISpatialReference_get_Alias(This,alias)	\
    (This)->lpVtbl -> get_Alias(This,alias)

#define ISpatialReference_put_Alias(This,alias)	\
    (This)->lpVtbl -> put_Alias(This,alias)

#define ISpatialReference_get_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> get_Abbreviation(This,abbrev)

#define ISpatialReference_put_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> put_Abbreviation(This,abbrev)

#define ISpatialReference_get_Remarks(This,remarks)	\
    (This)->lpVtbl -> get_Remarks(This,remarks)

#define ISpatialReference_put_Remarks(This,remarks)	\
    (This)->lpVtbl -> put_Remarks(This,remarks)

#define ISpatialReference_get_WellKnownText(This,wkt)	\
    (This)->lpVtbl -> get_WellKnownText(This,wkt)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ISpatialReference_INTERFACE_DEFINED__ */


#ifndef __IGeodeticSpatialReference_INTERFACE_DEFINED__
#define __IGeodeticSpatialReference_INTERFACE_DEFINED__

/* interface IGeodeticSpatialReference */
/* [uuid][object] */ 


EXTERN_C const IID IID_IGeodeticSpatialReference;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7c3c56d0-fe1e-11d0-ad87-080009b6f22b")
    IGeodeticSpatialReference : public ISpatialReference
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IGeodeticSpatialReferenceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IGeodeticSpatialReference __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IGeodeticSpatialReference __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IGeodeticSpatialReference __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IGeodeticSpatialReference __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Name )( 
            IGeodeticSpatialReference __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Authority )( 
            IGeodeticSpatialReference __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Authority )( 
            IGeodeticSpatialReference __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Code )( 
            IGeodeticSpatialReference __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *code);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Code )( 
            IGeodeticSpatialReference __RPC_FAR * This,
            /* [in] */ long code);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Alias )( 
            IGeodeticSpatialReference __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *alias);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Alias )( 
            IGeodeticSpatialReference __RPC_FAR * This,
            /* [in] */ BSTR alias);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Abbreviation )( 
            IGeodeticSpatialReference __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *abbrev);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Abbreviation )( 
            IGeodeticSpatialReference __RPC_FAR * This,
            /* [in] */ BSTR abbrev);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Remarks )( 
            IGeodeticSpatialReference __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *remarks);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Remarks )( 
            IGeodeticSpatialReference __RPC_FAR * This,
            /* [in] */ BSTR remarks);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WellKnownText )( 
            IGeodeticSpatialReference __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *wkt);
        
        END_INTERFACE
    } IGeodeticSpatialReferenceVtbl;

    interface IGeodeticSpatialReference
    {
        CONST_VTBL struct IGeodeticSpatialReferenceVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGeodeticSpatialReference_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IGeodeticSpatialReference_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IGeodeticSpatialReference_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IGeodeticSpatialReference_get_Name(This,name)	\
    (This)->lpVtbl -> get_Name(This,name)

#define IGeodeticSpatialReference_put_Name(This,name)	\
    (This)->lpVtbl -> put_Name(This,name)

#define IGeodeticSpatialReference_get_Authority(This,name)	\
    (This)->lpVtbl -> get_Authority(This,name)

#define IGeodeticSpatialReference_put_Authority(This,name)	\
    (This)->lpVtbl -> put_Authority(This,name)

#define IGeodeticSpatialReference_get_Code(This,code)	\
    (This)->lpVtbl -> get_Code(This,code)

#define IGeodeticSpatialReference_put_Code(This,code)	\
    (This)->lpVtbl -> put_Code(This,code)

#define IGeodeticSpatialReference_get_Alias(This,alias)	\
    (This)->lpVtbl -> get_Alias(This,alias)

#define IGeodeticSpatialReference_put_Alias(This,alias)	\
    (This)->lpVtbl -> put_Alias(This,alias)

#define IGeodeticSpatialReference_get_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> get_Abbreviation(This,abbrev)

#define IGeodeticSpatialReference_put_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> put_Abbreviation(This,abbrev)

#define IGeodeticSpatialReference_get_Remarks(This,remarks)	\
    (This)->lpVtbl -> get_Remarks(This,remarks)

#define IGeodeticSpatialReference_put_Remarks(This,remarks)	\
    (This)->lpVtbl -> put_Remarks(This,remarks)

#define IGeodeticSpatialReference_get_WellKnownText(This,wkt)	\
    (This)->lpVtbl -> get_WellKnownText(This,wkt)



#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IGeodeticSpatialReference_INTERFACE_DEFINED__ */


#ifndef __IGeographicCoordinateSystem_INTERFACE_DEFINED__
#define __IGeographicCoordinateSystem_INTERFACE_DEFINED__

/* interface IGeographicCoordinateSystem */
/* [uuid][object] */ 


EXTERN_C const IID IID_IGeographicCoordinateSystem;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("a3fd5390-fe1e-11d0-ad87-080009b6f22b")
    IGeographicCoordinateSystem : public IGeodeticSpatialReference
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Usage( 
            /* [retval][out] */ BSTR __RPC_FAR *usage) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Usage( 
            /* [in] */ BSTR usage) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_HorizontalDatum( 
            /* [retval][out] */ IHorizontalDatum __RPC_FAR *__RPC_FAR *datum) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_HorizontalDatum( 
            /* [in] */ IHorizontalDatum __RPC_FAR *datum) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_AngularUnit( 
            /* [retval][out] */ IAngularUnit __RPC_FAR *__RPC_FAR *unit) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_AngularUnit( 
            /* [in] */ IAngularUnit __RPC_FAR *unit) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_PrimeMeridian( 
            /* [retval][out] */ IPrimeMeridian __RPC_FAR *__RPC_FAR *prmMerid) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_PrimeMeridian( 
            /* [in] */ IPrimeMeridian __RPC_FAR *prmMerid) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IGeographicCoordinateSystemVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IGeographicCoordinateSystem __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IGeographicCoordinateSystem __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IGeographicCoordinateSystem __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IGeographicCoordinateSystem __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Name )( 
            IGeographicCoordinateSystem __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Authority )( 
            IGeographicCoordinateSystem __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Authority )( 
            IGeographicCoordinateSystem __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Code )( 
            IGeographicCoordinateSystem __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *code);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Code )( 
            IGeographicCoordinateSystem __RPC_FAR * This,
            /* [in] */ long code);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Alias )( 
            IGeographicCoordinateSystem __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *alias);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Alias )( 
            IGeographicCoordinateSystem __RPC_FAR * This,
            /* [in] */ BSTR alias);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Abbreviation )( 
            IGeographicCoordinateSystem __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *abbrev);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Abbreviation )( 
            IGeographicCoordinateSystem __RPC_FAR * This,
            /* [in] */ BSTR abbrev);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Remarks )( 
            IGeographicCoordinateSystem __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *remarks);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Remarks )( 
            IGeographicCoordinateSystem __RPC_FAR * This,
            /* [in] */ BSTR remarks);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WellKnownText )( 
            IGeographicCoordinateSystem __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *wkt);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Usage )( 
            IGeographicCoordinateSystem __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *usage);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Usage )( 
            IGeographicCoordinateSystem __RPC_FAR * This,
            /* [in] */ BSTR usage);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_HorizontalDatum )( 
            IGeographicCoordinateSystem __RPC_FAR * This,
            /* [retval][out] */ IHorizontalDatum __RPC_FAR *__RPC_FAR *datum);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_HorizontalDatum )( 
            IGeographicCoordinateSystem __RPC_FAR * This,
            /* [in] */ IHorizontalDatum __RPC_FAR *datum);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_AngularUnit )( 
            IGeographicCoordinateSystem __RPC_FAR * This,
            /* [retval][out] */ IAngularUnit __RPC_FAR *__RPC_FAR *unit);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_AngularUnit )( 
            IGeographicCoordinateSystem __RPC_FAR * This,
            /* [in] */ IAngularUnit __RPC_FAR *unit);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PrimeMeridian )( 
            IGeographicCoordinateSystem __RPC_FAR * This,
            /* [retval][out] */ IPrimeMeridian __RPC_FAR *__RPC_FAR *prmMerid);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PrimeMeridian )( 
            IGeographicCoordinateSystem __RPC_FAR * This,
            /* [in] */ IPrimeMeridian __RPC_FAR *prmMerid);
        
        END_INTERFACE
    } IGeographicCoordinateSystemVtbl;

    interface IGeographicCoordinateSystem
    {
        CONST_VTBL struct IGeographicCoordinateSystemVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGeographicCoordinateSystem_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IGeographicCoordinateSystem_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IGeographicCoordinateSystem_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IGeographicCoordinateSystem_get_Name(This,name)	\
    (This)->lpVtbl -> get_Name(This,name)

#define IGeographicCoordinateSystem_put_Name(This,name)	\
    (This)->lpVtbl -> put_Name(This,name)

#define IGeographicCoordinateSystem_get_Authority(This,name)	\
    (This)->lpVtbl -> get_Authority(This,name)

#define IGeographicCoordinateSystem_put_Authority(This,name)	\
    (This)->lpVtbl -> put_Authority(This,name)

#define IGeographicCoordinateSystem_get_Code(This,code)	\
    (This)->lpVtbl -> get_Code(This,code)

#define IGeographicCoordinateSystem_put_Code(This,code)	\
    (This)->lpVtbl -> put_Code(This,code)

#define IGeographicCoordinateSystem_get_Alias(This,alias)	\
    (This)->lpVtbl -> get_Alias(This,alias)

#define IGeographicCoordinateSystem_put_Alias(This,alias)	\
    (This)->lpVtbl -> put_Alias(This,alias)

#define IGeographicCoordinateSystem_get_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> get_Abbreviation(This,abbrev)

#define IGeographicCoordinateSystem_put_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> put_Abbreviation(This,abbrev)

#define IGeographicCoordinateSystem_get_Remarks(This,remarks)	\
    (This)->lpVtbl -> get_Remarks(This,remarks)

#define IGeographicCoordinateSystem_put_Remarks(This,remarks)	\
    (This)->lpVtbl -> put_Remarks(This,remarks)

#define IGeographicCoordinateSystem_get_WellKnownText(This,wkt)	\
    (This)->lpVtbl -> get_WellKnownText(This,wkt)




#define IGeographicCoordinateSystem_get_Usage(This,usage)	\
    (This)->lpVtbl -> get_Usage(This,usage)

#define IGeographicCoordinateSystem_put_Usage(This,usage)	\
    (This)->lpVtbl -> put_Usage(This,usage)

#define IGeographicCoordinateSystem_get_HorizontalDatum(This,datum)	\
    (This)->lpVtbl -> get_HorizontalDatum(This,datum)

#define IGeographicCoordinateSystem_put_HorizontalDatum(This,datum)	\
    (This)->lpVtbl -> put_HorizontalDatum(This,datum)

#define IGeographicCoordinateSystem_get_AngularUnit(This,unit)	\
    (This)->lpVtbl -> get_AngularUnit(This,unit)

#define IGeographicCoordinateSystem_put_AngularUnit(This,unit)	\
    (This)->lpVtbl -> put_AngularUnit(This,unit)

#define IGeographicCoordinateSystem_get_PrimeMeridian(This,prmMerid)	\
    (This)->lpVtbl -> get_PrimeMeridian(This,prmMerid)

#define IGeographicCoordinateSystem_put_PrimeMeridian(This,prmMerid)	\
    (This)->lpVtbl -> put_PrimeMeridian(This,prmMerid)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IGeographicCoordinateSystem_get_Usage_Proxy( 
    IGeographicCoordinateSystem __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *usage);


void __RPC_STUB IGeographicCoordinateSystem_get_Usage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IGeographicCoordinateSystem_put_Usage_Proxy( 
    IGeographicCoordinateSystem __RPC_FAR * This,
    /* [in] */ BSTR usage);


void __RPC_STUB IGeographicCoordinateSystem_put_Usage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IGeographicCoordinateSystem_get_HorizontalDatum_Proxy( 
    IGeographicCoordinateSystem __RPC_FAR * This,
    /* [retval][out] */ IHorizontalDatum __RPC_FAR *__RPC_FAR *datum);


void __RPC_STUB IGeographicCoordinateSystem_get_HorizontalDatum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IGeographicCoordinateSystem_put_HorizontalDatum_Proxy( 
    IGeographicCoordinateSystem __RPC_FAR * This,
    /* [in] */ IHorizontalDatum __RPC_FAR *datum);


void __RPC_STUB IGeographicCoordinateSystem_put_HorizontalDatum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IGeographicCoordinateSystem_get_AngularUnit_Proxy( 
    IGeographicCoordinateSystem __RPC_FAR * This,
    /* [retval][out] */ IAngularUnit __RPC_FAR *__RPC_FAR *unit);


void __RPC_STUB IGeographicCoordinateSystem_get_AngularUnit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IGeographicCoordinateSystem_put_AngularUnit_Proxy( 
    IGeographicCoordinateSystem __RPC_FAR * This,
    /* [in] */ IAngularUnit __RPC_FAR *unit);


void __RPC_STUB IGeographicCoordinateSystem_put_AngularUnit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IGeographicCoordinateSystem_get_PrimeMeridian_Proxy( 
    IGeographicCoordinateSystem __RPC_FAR * This,
    /* [retval][out] */ IPrimeMeridian __RPC_FAR *__RPC_FAR *prmMerid);


void __RPC_STUB IGeographicCoordinateSystem_get_PrimeMeridian_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IGeographicCoordinateSystem_put_PrimeMeridian_Proxy( 
    IGeographicCoordinateSystem __RPC_FAR * This,
    /* [in] */ IPrimeMeridian __RPC_FAR *prmMerid);


void __RPC_STUB IGeographicCoordinateSystem_put_PrimeMeridian_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IGeographicCoordinateSystem_INTERFACE_DEFINED__ */


#ifndef __IParameter_INTERFACE_DEFINED__
#define __IParameter_INTERFACE_DEFINED__

/* interface IParameter */
/* [uuid][object] */ 


EXTERN_C const IID IID_IParameter;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9a5e32d0-fe1f-11d0-ad87-080009b6f22b")
    IParameter : public ISpatialReferenceInfo
    {
    public:
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_ValueUnit( 
            /* [in] */ IUnit __RPC_FAR *unit) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ValueUnit( 
            /* [retval][out] */ IUnit __RPC_FAR *__RPC_FAR *unit) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Value( 
            /* [in] */ double value) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Value( 
            /* [retval][out] */ double __RPC_FAR *value) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IParameterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IParameter __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IParameter __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IParameter __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IParameter __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Name )( 
            IParameter __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Authority )( 
            IParameter __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Authority )( 
            IParameter __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Code )( 
            IParameter __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *code);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Code )( 
            IParameter __RPC_FAR * This,
            /* [in] */ long code);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Alias )( 
            IParameter __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *alias);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Alias )( 
            IParameter __RPC_FAR * This,
            /* [in] */ BSTR alias);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Abbreviation )( 
            IParameter __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *abbrev);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Abbreviation )( 
            IParameter __RPC_FAR * This,
            /* [in] */ BSTR abbrev);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Remarks )( 
            IParameter __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *remarks);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Remarks )( 
            IParameter __RPC_FAR * This,
            /* [in] */ BSTR remarks);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WellKnownText )( 
            IParameter __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *wkt);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ValueUnit )( 
            IParameter __RPC_FAR * This,
            /* [in] */ IUnit __RPC_FAR *unit);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ValueUnit )( 
            IParameter __RPC_FAR * This,
            /* [retval][out] */ IUnit __RPC_FAR *__RPC_FAR *unit);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Value )( 
            IParameter __RPC_FAR * This,
            /* [in] */ double value);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Value )( 
            IParameter __RPC_FAR * This,
            /* [retval][out] */ double __RPC_FAR *value);
        
        END_INTERFACE
    } IParameterVtbl;

    interface IParameter
    {
        CONST_VTBL struct IParameterVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IParameter_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IParameter_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IParameter_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IParameter_get_Name(This,name)	\
    (This)->lpVtbl -> get_Name(This,name)

#define IParameter_put_Name(This,name)	\
    (This)->lpVtbl -> put_Name(This,name)

#define IParameter_get_Authority(This,name)	\
    (This)->lpVtbl -> get_Authority(This,name)

#define IParameter_put_Authority(This,name)	\
    (This)->lpVtbl -> put_Authority(This,name)

#define IParameter_get_Code(This,code)	\
    (This)->lpVtbl -> get_Code(This,code)

#define IParameter_put_Code(This,code)	\
    (This)->lpVtbl -> put_Code(This,code)

#define IParameter_get_Alias(This,alias)	\
    (This)->lpVtbl -> get_Alias(This,alias)

#define IParameter_put_Alias(This,alias)	\
    (This)->lpVtbl -> put_Alias(This,alias)

#define IParameter_get_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> get_Abbreviation(This,abbrev)

#define IParameter_put_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> put_Abbreviation(This,abbrev)

#define IParameter_get_Remarks(This,remarks)	\
    (This)->lpVtbl -> get_Remarks(This,remarks)

#define IParameter_put_Remarks(This,remarks)	\
    (This)->lpVtbl -> put_Remarks(This,remarks)

#define IParameter_get_WellKnownText(This,wkt)	\
    (This)->lpVtbl -> get_WellKnownText(This,wkt)


#define IParameter_put_ValueUnit(This,unit)	\
    (This)->lpVtbl -> put_ValueUnit(This,unit)

#define IParameter_get_ValueUnit(This,unit)	\
    (This)->lpVtbl -> get_ValueUnit(This,unit)

#define IParameter_put_Value(This,value)	\
    (This)->lpVtbl -> put_Value(This,value)

#define IParameter_get_Value(This,value)	\
    (This)->lpVtbl -> get_Value(This,value)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propput] */ HRESULT STDMETHODCALLTYPE IParameter_put_ValueUnit_Proxy( 
    IParameter __RPC_FAR * This,
    /* [in] */ IUnit __RPC_FAR *unit);


void __RPC_STUB IParameter_put_ValueUnit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IParameter_get_ValueUnit_Proxy( 
    IParameter __RPC_FAR * This,
    /* [retval][out] */ IUnit __RPC_FAR *__RPC_FAR *unit);


void __RPC_STUB IParameter_get_ValueUnit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IParameter_put_Value_Proxy( 
    IParameter __RPC_FAR * This,
    /* [in] */ double value);


void __RPC_STUB IParameter_put_Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IParameter_get_Value_Proxy( 
    IParameter __RPC_FAR * This,
    /* [retval][out] */ double __RPC_FAR *value);


void __RPC_STUB IParameter_get_Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IParameter_INTERFACE_DEFINED__ */


#ifndef __IParameterInfo_INTERFACE_DEFINED__
#define __IParameterInfo_INTERFACE_DEFINED__

/* interface IParameterInfo */
/* [uuid][object] */ 


EXTERN_C const IID IID_IParameterInfo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7309b460-fe1f-11d0-ad87-080009b6f22b")
    IParameterInfo : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_NumParameters( 
            /* [retval][out] */ long __RPC_FAR *numParams) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Parameter( 
            /* [in] */ long index,
            /* [retval][out] */ IParameter __RPC_FAR *__RPC_FAR *param) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Parameter( 
            /* [in] */ long index,
            /* [in] */ IParameter __RPC_FAR *param) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IParameterInfoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IParameterInfo __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IParameterInfo __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IParameterInfo __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_NumParameters )( 
            IParameterInfo __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *numParams);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Parameter )( 
            IParameterInfo __RPC_FAR * This,
            /* [in] */ long index,
            /* [retval][out] */ IParameter __RPC_FAR *__RPC_FAR *param);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Parameter )( 
            IParameterInfo __RPC_FAR * This,
            /* [in] */ long index,
            /* [in] */ IParameter __RPC_FAR *param);
        
        END_INTERFACE
    } IParameterInfoVtbl;

    interface IParameterInfo
    {
        CONST_VTBL struct IParameterInfoVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IParameterInfo_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IParameterInfo_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IParameterInfo_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IParameterInfo_get_NumParameters(This,numParams)	\
    (This)->lpVtbl -> get_NumParameters(This,numParams)

#define IParameterInfo_get_Parameter(This,index,param)	\
    (This)->lpVtbl -> get_Parameter(This,index,param)

#define IParameterInfo_put_Parameter(This,index,param)	\
    (This)->lpVtbl -> put_Parameter(This,index,param)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IParameterInfo_get_NumParameters_Proxy( 
    IParameterInfo __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *numParams);


void __RPC_STUB IParameterInfo_get_NumParameters_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IParameterInfo_get_Parameter_Proxy( 
    IParameterInfo __RPC_FAR * This,
    /* [in] */ long index,
    /* [retval][out] */ IParameter __RPC_FAR *__RPC_FAR *param);


void __RPC_STUB IParameterInfo_get_Parameter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IParameterInfo_put_Parameter_Proxy( 
    IParameterInfo __RPC_FAR * This,
    /* [in] */ long index,
    /* [in] */ IParameter __RPC_FAR *param);


void __RPC_STUB IParameterInfo_put_Parameter_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IParameterInfo_INTERFACE_DEFINED__ */


#ifndef __IGeographicTransform_INTERFACE_DEFINED__
#define __IGeographicTransform_INTERFACE_DEFINED__

/* interface IGeographicTransform */
/* [uuid][object] */ 


EXTERN_C const IID IID_IGeographicTransform;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5eb513c0-fe1f-11d0-ad87-080009b6f22b")
    IGeographicTransform : public ISpatialReferenceInfo
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_SourceGCS( 
            /* [retval][out] */ IGeographicCoordinateSystem __RPC_FAR *__RPC_FAR *gcs) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_SourceGCS( 
            /* [in] */ IGeographicCoordinateSystem __RPC_FAR *gcs) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_TargetGCS( 
            /* [retval][out] */ IGeographicCoordinateSystem __RPC_FAR *__RPC_FAR *gcs) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_TargetGCS( 
            /* [in] */ IGeographicCoordinateSystem __RPC_FAR *gcs) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_DefaultParameterInfo( 
            /* [retval][out] */ IParameterInfo __RPC_FAR *__RPC_FAR *paramInfo) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ParameterInfo( 
            /* [retval][out] */ IParameterInfo __RPC_FAR *__RPC_FAR *paramInfo) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_ParameterInfo( 
            /* [in] */ IParameterInfo __RPC_FAR *paramInfo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Forward( 
            /* [in] */ long count,
            /* [size_is][out][in] */ WKSPoint __RPC_FAR points[  ]) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Inverse( 
            /* [in] */ long count,
            /* [size_is][out][in] */ WKSPoint __RPC_FAR points[  ]) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IGeographicTransformVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IGeographicTransform __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IGeographicTransform __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IGeographicTransform __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IGeographicTransform __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Name )( 
            IGeographicTransform __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Authority )( 
            IGeographicTransform __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Authority )( 
            IGeographicTransform __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Code )( 
            IGeographicTransform __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *code);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Code )( 
            IGeographicTransform __RPC_FAR * This,
            /* [in] */ long code);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Alias )( 
            IGeographicTransform __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *alias);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Alias )( 
            IGeographicTransform __RPC_FAR * This,
            /* [in] */ BSTR alias);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Abbreviation )( 
            IGeographicTransform __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *abbrev);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Abbreviation )( 
            IGeographicTransform __RPC_FAR * This,
            /* [in] */ BSTR abbrev);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Remarks )( 
            IGeographicTransform __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *remarks);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Remarks )( 
            IGeographicTransform __RPC_FAR * This,
            /* [in] */ BSTR remarks);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WellKnownText )( 
            IGeographicTransform __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *wkt);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SourceGCS )( 
            IGeographicTransform __RPC_FAR * This,
            /* [retval][out] */ IGeographicCoordinateSystem __RPC_FAR *__RPC_FAR *gcs);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SourceGCS )( 
            IGeographicTransform __RPC_FAR * This,
            /* [in] */ IGeographicCoordinateSystem __RPC_FAR *gcs);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_TargetGCS )( 
            IGeographicTransform __RPC_FAR * This,
            /* [retval][out] */ IGeographicCoordinateSystem __RPC_FAR *__RPC_FAR *gcs);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_TargetGCS )( 
            IGeographicTransform __RPC_FAR * This,
            /* [in] */ IGeographicCoordinateSystem __RPC_FAR *gcs);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DefaultParameterInfo )( 
            IGeographicTransform __RPC_FAR * This,
            /* [retval][out] */ IParameterInfo __RPC_FAR *__RPC_FAR *paramInfo);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ParameterInfo )( 
            IGeographicTransform __RPC_FAR * This,
            /* [retval][out] */ IParameterInfo __RPC_FAR *__RPC_FAR *paramInfo);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ParameterInfo )( 
            IGeographicTransform __RPC_FAR * This,
            /* [in] */ IParameterInfo __RPC_FAR *paramInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Forward )( 
            IGeographicTransform __RPC_FAR * This,
            /* [in] */ long count,
            /* [size_is][out][in] */ WKSPoint __RPC_FAR points[  ]);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Inverse )( 
            IGeographicTransform __RPC_FAR * This,
            /* [in] */ long count,
            /* [size_is][out][in] */ WKSPoint __RPC_FAR points[  ]);
        
        END_INTERFACE
    } IGeographicTransformVtbl;

    interface IGeographicTransform
    {
        CONST_VTBL struct IGeographicTransformVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGeographicTransform_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IGeographicTransform_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IGeographicTransform_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IGeographicTransform_get_Name(This,name)	\
    (This)->lpVtbl -> get_Name(This,name)

#define IGeographicTransform_put_Name(This,name)	\
    (This)->lpVtbl -> put_Name(This,name)

#define IGeographicTransform_get_Authority(This,name)	\
    (This)->lpVtbl -> get_Authority(This,name)

#define IGeographicTransform_put_Authority(This,name)	\
    (This)->lpVtbl -> put_Authority(This,name)

#define IGeographicTransform_get_Code(This,code)	\
    (This)->lpVtbl -> get_Code(This,code)

#define IGeographicTransform_put_Code(This,code)	\
    (This)->lpVtbl -> put_Code(This,code)

#define IGeographicTransform_get_Alias(This,alias)	\
    (This)->lpVtbl -> get_Alias(This,alias)

#define IGeographicTransform_put_Alias(This,alias)	\
    (This)->lpVtbl -> put_Alias(This,alias)

#define IGeographicTransform_get_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> get_Abbreviation(This,abbrev)

#define IGeographicTransform_put_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> put_Abbreviation(This,abbrev)

#define IGeographicTransform_get_Remarks(This,remarks)	\
    (This)->lpVtbl -> get_Remarks(This,remarks)

#define IGeographicTransform_put_Remarks(This,remarks)	\
    (This)->lpVtbl -> put_Remarks(This,remarks)

#define IGeographicTransform_get_WellKnownText(This,wkt)	\
    (This)->lpVtbl -> get_WellKnownText(This,wkt)


#define IGeographicTransform_get_SourceGCS(This,gcs)	\
    (This)->lpVtbl -> get_SourceGCS(This,gcs)

#define IGeographicTransform_put_SourceGCS(This,gcs)	\
    (This)->lpVtbl -> put_SourceGCS(This,gcs)

#define IGeographicTransform_get_TargetGCS(This,gcs)	\
    (This)->lpVtbl -> get_TargetGCS(This,gcs)

#define IGeographicTransform_put_TargetGCS(This,gcs)	\
    (This)->lpVtbl -> put_TargetGCS(This,gcs)

#define IGeographicTransform_get_DefaultParameterInfo(This,paramInfo)	\
    (This)->lpVtbl -> get_DefaultParameterInfo(This,paramInfo)

#define IGeographicTransform_get_ParameterInfo(This,paramInfo)	\
    (This)->lpVtbl -> get_ParameterInfo(This,paramInfo)

#define IGeographicTransform_put_ParameterInfo(This,paramInfo)	\
    (This)->lpVtbl -> put_ParameterInfo(This,paramInfo)

#define IGeographicTransform_Forward(This,count,points)	\
    (This)->lpVtbl -> Forward(This,count,points)

#define IGeographicTransform_Inverse(This,count,points)	\
    (This)->lpVtbl -> Inverse(This,count,points)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IGeographicTransform_get_SourceGCS_Proxy( 
    IGeographicTransform __RPC_FAR * This,
    /* [retval][out] */ IGeographicCoordinateSystem __RPC_FAR *__RPC_FAR *gcs);


void __RPC_STUB IGeographicTransform_get_SourceGCS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IGeographicTransform_put_SourceGCS_Proxy( 
    IGeographicTransform __RPC_FAR * This,
    /* [in] */ IGeographicCoordinateSystem __RPC_FAR *gcs);


void __RPC_STUB IGeographicTransform_put_SourceGCS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IGeographicTransform_get_TargetGCS_Proxy( 
    IGeographicTransform __RPC_FAR * This,
    /* [retval][out] */ IGeographicCoordinateSystem __RPC_FAR *__RPC_FAR *gcs);


void __RPC_STUB IGeographicTransform_get_TargetGCS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IGeographicTransform_put_TargetGCS_Proxy( 
    IGeographicTransform __RPC_FAR * This,
    /* [in] */ IGeographicCoordinateSystem __RPC_FAR *gcs);


void __RPC_STUB IGeographicTransform_put_TargetGCS_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IGeographicTransform_get_DefaultParameterInfo_Proxy( 
    IGeographicTransform __RPC_FAR * This,
    /* [retval][out] */ IParameterInfo __RPC_FAR *__RPC_FAR *paramInfo);


void __RPC_STUB IGeographicTransform_get_DefaultParameterInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IGeographicTransform_get_ParameterInfo_Proxy( 
    IGeographicTransform __RPC_FAR * This,
    /* [retval][out] */ IParameterInfo __RPC_FAR *__RPC_FAR *paramInfo);


void __RPC_STUB IGeographicTransform_get_ParameterInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IGeographicTransform_put_ParameterInfo_Proxy( 
    IGeographicTransform __RPC_FAR * This,
    /* [in] */ IParameterInfo __RPC_FAR *paramInfo);


void __RPC_STUB IGeographicTransform_put_ParameterInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IGeographicTransform_Forward_Proxy( 
    IGeographicTransform __RPC_FAR * This,
    /* [in] */ long count,
    /* [size_is][out][in] */ WKSPoint __RPC_FAR points[  ]);


void __RPC_STUB IGeographicTransform_Forward_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IGeographicTransform_Inverse_Proxy( 
    IGeographicTransform __RPC_FAR * This,
    /* [in] */ long count,
    /* [size_is][out][in] */ WKSPoint __RPC_FAR points[  ]);


void __RPC_STUB IGeographicTransform_Inverse_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IGeographicTransform_INTERFACE_DEFINED__ */


#ifndef __IProjection_INTERFACE_DEFINED__
#define __IProjection_INTERFACE_DEFINED__

/* interface IProjection */
/* [uuid][object] */ 


EXTERN_C const IID IID_IProjection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5002f420-fe1f-11d0-ad87-080009b6f22b")
    IProjection : public ISpatialReferenceInfo
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Usage( 
            /* [retval][out] */ BSTR __RPC_FAR *usage) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Classification( 
            /* [retval][out] */ BSTR __RPC_FAR *classification) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Forward( 
            /* [in] */ long count,
            /* [size_is][out][in] */ WKSPoint __RPC_FAR points[  ]) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Inverse( 
            /* [in] */ long count,
            /* [size_is][out][in] */ WKSPoint __RPC_FAR points[  ]) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_DefaultParameterInfo( 
            /* [retval][out] */ IParameterInfo __RPC_FAR *__RPC_FAR *paramInfo) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ParameterInfo( 
            /* [retval][out] */ IParameterInfo __RPC_FAR *__RPC_FAR *paramInfo) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_ParameterInfo( 
            /* [in] */ IParameterInfo __RPC_FAR *paramInfo) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_AngularUnit( 
            /* [retval][out] */ IAngularUnit __RPC_FAR *__RPC_FAR *unit) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_AngularUnit( 
            /* [in] */ IAngularUnit __RPC_FAR *unit) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_LinearUnit( 
            /* [retval][out] */ ILinearUnit __RPC_FAR *__RPC_FAR *unit) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_LinearUnit( 
            /* [in] */ ILinearUnit __RPC_FAR *unit) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Ellipsoid( 
            /* [retval][out] */ IEllipsoid __RPC_FAR *__RPC_FAR *ellipsoid) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Ellipsoid( 
            /* [in] */ IEllipsoid __RPC_FAR *ellipsoid) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IProjectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IProjection __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IProjection __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IProjection __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IProjection __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Name )( 
            IProjection __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Authority )( 
            IProjection __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Authority )( 
            IProjection __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Code )( 
            IProjection __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *code);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Code )( 
            IProjection __RPC_FAR * This,
            /* [in] */ long code);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Alias )( 
            IProjection __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *alias);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Alias )( 
            IProjection __RPC_FAR * This,
            /* [in] */ BSTR alias);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Abbreviation )( 
            IProjection __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *abbrev);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Abbreviation )( 
            IProjection __RPC_FAR * This,
            /* [in] */ BSTR abbrev);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Remarks )( 
            IProjection __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *remarks);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Remarks )( 
            IProjection __RPC_FAR * This,
            /* [in] */ BSTR remarks);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WellKnownText )( 
            IProjection __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *wkt);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Usage )( 
            IProjection __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *usage);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Classification )( 
            IProjection __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *classification);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Forward )( 
            IProjection __RPC_FAR * This,
            /* [in] */ long count,
            /* [size_is][out][in] */ WKSPoint __RPC_FAR points[  ]);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Inverse )( 
            IProjection __RPC_FAR * This,
            /* [in] */ long count,
            /* [size_is][out][in] */ WKSPoint __RPC_FAR points[  ]);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DefaultParameterInfo )( 
            IProjection __RPC_FAR * This,
            /* [retval][out] */ IParameterInfo __RPC_FAR *__RPC_FAR *paramInfo);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ParameterInfo )( 
            IProjection __RPC_FAR * This,
            /* [retval][out] */ IParameterInfo __RPC_FAR *__RPC_FAR *paramInfo);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ParameterInfo )( 
            IProjection __RPC_FAR * This,
            /* [in] */ IParameterInfo __RPC_FAR *paramInfo);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_AngularUnit )( 
            IProjection __RPC_FAR * This,
            /* [retval][out] */ IAngularUnit __RPC_FAR *__RPC_FAR *unit);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_AngularUnit )( 
            IProjection __RPC_FAR * This,
            /* [in] */ IAngularUnit __RPC_FAR *unit);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LinearUnit )( 
            IProjection __RPC_FAR * This,
            /* [retval][out] */ ILinearUnit __RPC_FAR *__RPC_FAR *unit);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_LinearUnit )( 
            IProjection __RPC_FAR * This,
            /* [in] */ ILinearUnit __RPC_FAR *unit);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Ellipsoid )( 
            IProjection __RPC_FAR * This,
            /* [retval][out] */ IEllipsoid __RPC_FAR *__RPC_FAR *ellipsoid);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Ellipsoid )( 
            IProjection __RPC_FAR * This,
            /* [in] */ IEllipsoid __RPC_FAR *ellipsoid);
        
        END_INTERFACE
    } IProjectionVtbl;

    interface IProjection
    {
        CONST_VTBL struct IProjectionVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IProjection_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IProjection_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IProjection_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IProjection_get_Name(This,name)	\
    (This)->lpVtbl -> get_Name(This,name)

#define IProjection_put_Name(This,name)	\
    (This)->lpVtbl -> put_Name(This,name)

#define IProjection_get_Authority(This,name)	\
    (This)->lpVtbl -> get_Authority(This,name)

#define IProjection_put_Authority(This,name)	\
    (This)->lpVtbl -> put_Authority(This,name)

#define IProjection_get_Code(This,code)	\
    (This)->lpVtbl -> get_Code(This,code)

#define IProjection_put_Code(This,code)	\
    (This)->lpVtbl -> put_Code(This,code)

#define IProjection_get_Alias(This,alias)	\
    (This)->lpVtbl -> get_Alias(This,alias)

#define IProjection_put_Alias(This,alias)	\
    (This)->lpVtbl -> put_Alias(This,alias)

#define IProjection_get_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> get_Abbreviation(This,abbrev)

#define IProjection_put_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> put_Abbreviation(This,abbrev)

#define IProjection_get_Remarks(This,remarks)	\
    (This)->lpVtbl -> get_Remarks(This,remarks)

#define IProjection_put_Remarks(This,remarks)	\
    (This)->lpVtbl -> put_Remarks(This,remarks)

#define IProjection_get_WellKnownText(This,wkt)	\
    (This)->lpVtbl -> get_WellKnownText(This,wkt)


#define IProjection_get_Usage(This,usage)	\
    (This)->lpVtbl -> get_Usage(This,usage)

#define IProjection_get_Classification(This,classification)	\
    (This)->lpVtbl -> get_Classification(This,classification)

#define IProjection_Forward(This,count,points)	\
    (This)->lpVtbl -> Forward(This,count,points)

#define IProjection_Inverse(This,count,points)	\
    (This)->lpVtbl -> Inverse(This,count,points)

#define IProjection_get_DefaultParameterInfo(This,paramInfo)	\
    (This)->lpVtbl -> get_DefaultParameterInfo(This,paramInfo)

#define IProjection_get_ParameterInfo(This,paramInfo)	\
    (This)->lpVtbl -> get_ParameterInfo(This,paramInfo)

#define IProjection_put_ParameterInfo(This,paramInfo)	\
    (This)->lpVtbl -> put_ParameterInfo(This,paramInfo)

#define IProjection_get_AngularUnit(This,unit)	\
    (This)->lpVtbl -> get_AngularUnit(This,unit)

#define IProjection_put_AngularUnit(This,unit)	\
    (This)->lpVtbl -> put_AngularUnit(This,unit)

#define IProjection_get_LinearUnit(This,unit)	\
    (This)->lpVtbl -> get_LinearUnit(This,unit)

#define IProjection_put_LinearUnit(This,unit)	\
    (This)->lpVtbl -> put_LinearUnit(This,unit)

#define IProjection_get_Ellipsoid(This,ellipsoid)	\
    (This)->lpVtbl -> get_Ellipsoid(This,ellipsoid)

#define IProjection_put_Ellipsoid(This,ellipsoid)	\
    (This)->lpVtbl -> put_Ellipsoid(This,ellipsoid)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IProjection_get_Usage_Proxy( 
    IProjection __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *usage);


void __RPC_STUB IProjection_get_Usage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IProjection_get_Classification_Proxy( 
    IProjection __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *classification);


void __RPC_STUB IProjection_get_Classification_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IProjection_Forward_Proxy( 
    IProjection __RPC_FAR * This,
    /* [in] */ long count,
    /* [size_is][out][in] */ WKSPoint __RPC_FAR points[  ]);


void __RPC_STUB IProjection_Forward_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IProjection_Inverse_Proxy( 
    IProjection __RPC_FAR * This,
    /* [in] */ long count,
    /* [size_is][out][in] */ WKSPoint __RPC_FAR points[  ]);


void __RPC_STUB IProjection_Inverse_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IProjection_get_DefaultParameterInfo_Proxy( 
    IProjection __RPC_FAR * This,
    /* [retval][out] */ IParameterInfo __RPC_FAR *__RPC_FAR *paramInfo);


void __RPC_STUB IProjection_get_DefaultParameterInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IProjection_get_ParameterInfo_Proxy( 
    IProjection __RPC_FAR * This,
    /* [retval][out] */ IParameterInfo __RPC_FAR *__RPC_FAR *paramInfo);


void __RPC_STUB IProjection_get_ParameterInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IProjection_put_ParameterInfo_Proxy( 
    IProjection __RPC_FAR * This,
    /* [in] */ IParameterInfo __RPC_FAR *paramInfo);


void __RPC_STUB IProjection_put_ParameterInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IProjection_get_AngularUnit_Proxy( 
    IProjection __RPC_FAR * This,
    /* [retval][out] */ IAngularUnit __RPC_FAR *__RPC_FAR *unit);


void __RPC_STUB IProjection_get_AngularUnit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IProjection_put_AngularUnit_Proxy( 
    IProjection __RPC_FAR * This,
    /* [in] */ IAngularUnit __RPC_FAR *unit);


void __RPC_STUB IProjection_put_AngularUnit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IProjection_get_LinearUnit_Proxy( 
    IProjection __RPC_FAR * This,
    /* [retval][out] */ ILinearUnit __RPC_FAR *__RPC_FAR *unit);


void __RPC_STUB IProjection_get_LinearUnit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IProjection_put_LinearUnit_Proxy( 
    IProjection __RPC_FAR * This,
    /* [in] */ ILinearUnit __RPC_FAR *unit);


void __RPC_STUB IProjection_put_LinearUnit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IProjection_get_Ellipsoid_Proxy( 
    IProjection __RPC_FAR * This,
    /* [retval][out] */ IEllipsoid __RPC_FAR *__RPC_FAR *ellipsoid);


void __RPC_STUB IProjection_get_Ellipsoid_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IProjection_put_Ellipsoid_Proxy( 
    IProjection __RPC_FAR * This,
    /* [in] */ IEllipsoid __RPC_FAR *ellipsoid);


void __RPC_STUB IProjection_put_Ellipsoid_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IProjection_INTERFACE_DEFINED__ */


#ifndef __IProjectedCoordinateSystem_INTERFACE_DEFINED__
#define __IProjectedCoordinateSystem_INTERFACE_DEFINED__

/* interface IProjectedCoordinateSystem */
/* [uuid][object] */ 


EXTERN_C const IID IID_IProjectedCoordinateSystem;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3dc39ff0-fe1f-11d0-ad87-080009b6f22b")
    IProjectedCoordinateSystem : public IGeodeticSpatialReference
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Usage( 
            /* [retval][out] */ BSTR __RPC_FAR *usage) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Usage( 
            /* [in] */ BSTR usage) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_GeographicCoordinateSystem( 
            /* [retval][out] */ IGeographicCoordinateSystem __RPC_FAR *__RPC_FAR *gcs) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_GeographicCoordinateSystem( 
            /* [in] */ IGeographicCoordinateSystem __RPC_FAR *gcs) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_LinearUnit( 
            /* [retval][out] */ ILinearUnit __RPC_FAR *__RPC_FAR *unit) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_LinearUnit( 
            /* [in] */ ILinearUnit __RPC_FAR *unit) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Projection( 
            /* [retval][out] */ IProjection __RPC_FAR *__RPC_FAR *projection) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_Projection( 
            /* [in] */ IProjection __RPC_FAR *projection) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_DefaultParameterInfo( 
            /* [retval][out] */ IParameterInfo __RPC_FAR *__RPC_FAR *paramInfo) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_ParameterInfo( 
            /* [retval][out] */ IParameterInfo __RPC_FAR *__RPC_FAR *paramInfo) = 0;
        
        virtual /* [propput] */ HRESULT STDMETHODCALLTYPE put_ParameterInfo( 
            /* [in] */ IParameterInfo __RPC_FAR *paramInfo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Forward( 
            /* [in] */ long count,
            /* [size_is][out][in] */ WKSPoint __RPC_FAR points[  ]) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Inverse( 
            /* [in] */ long count,
            /* [size_is][out][in] */ WKSPoint __RPC_FAR points[  ]) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IProjectedCoordinateSystemVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IProjectedCoordinateSystem __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IProjectedCoordinateSystem __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Name )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Name )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Authority )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *name);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Authority )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [in] */ BSTR name);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Code )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *code);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Code )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [in] */ long code);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Alias )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *alias);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Alias )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [in] */ BSTR alias);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Abbreviation )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *abbrev);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Abbreviation )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [in] */ BSTR abbrev);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Remarks )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *remarks);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Remarks )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [in] */ BSTR remarks);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_WellKnownText )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *wkt);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Usage )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *usage);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Usage )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [in] */ BSTR usage);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_GeographicCoordinateSystem )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [retval][out] */ IGeographicCoordinateSystem __RPC_FAR *__RPC_FAR *gcs);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_GeographicCoordinateSystem )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [in] */ IGeographicCoordinateSystem __RPC_FAR *gcs);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LinearUnit )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [retval][out] */ ILinearUnit __RPC_FAR *__RPC_FAR *unit);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_LinearUnit )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [in] */ ILinearUnit __RPC_FAR *unit);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Projection )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [retval][out] */ IProjection __RPC_FAR *__RPC_FAR *projection);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Projection )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [in] */ IProjection __RPC_FAR *projection);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DefaultParameterInfo )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [retval][out] */ IParameterInfo __RPC_FAR *__RPC_FAR *paramInfo);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ParameterInfo )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [retval][out] */ IParameterInfo __RPC_FAR *__RPC_FAR *paramInfo);
        
        /* [propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ParameterInfo )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [in] */ IParameterInfo __RPC_FAR *paramInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Forward )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [in] */ long count,
            /* [size_is][out][in] */ WKSPoint __RPC_FAR points[  ]);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Inverse )( 
            IProjectedCoordinateSystem __RPC_FAR * This,
            /* [in] */ long count,
            /* [size_is][out][in] */ WKSPoint __RPC_FAR points[  ]);
        
        END_INTERFACE
    } IProjectedCoordinateSystemVtbl;

    interface IProjectedCoordinateSystem
    {
        CONST_VTBL struct IProjectedCoordinateSystemVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IProjectedCoordinateSystem_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IProjectedCoordinateSystem_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IProjectedCoordinateSystem_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IProjectedCoordinateSystem_get_Name(This,name)	\
    (This)->lpVtbl -> get_Name(This,name)

#define IProjectedCoordinateSystem_put_Name(This,name)	\
    (This)->lpVtbl -> put_Name(This,name)

#define IProjectedCoordinateSystem_get_Authority(This,name)	\
    (This)->lpVtbl -> get_Authority(This,name)

#define IProjectedCoordinateSystem_put_Authority(This,name)	\
    (This)->lpVtbl -> put_Authority(This,name)

#define IProjectedCoordinateSystem_get_Code(This,code)	\
    (This)->lpVtbl -> get_Code(This,code)

#define IProjectedCoordinateSystem_put_Code(This,code)	\
    (This)->lpVtbl -> put_Code(This,code)

#define IProjectedCoordinateSystem_get_Alias(This,alias)	\
    (This)->lpVtbl -> get_Alias(This,alias)

#define IProjectedCoordinateSystem_put_Alias(This,alias)	\
    (This)->lpVtbl -> put_Alias(This,alias)

#define IProjectedCoordinateSystem_get_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> get_Abbreviation(This,abbrev)

#define IProjectedCoordinateSystem_put_Abbreviation(This,abbrev)	\
    (This)->lpVtbl -> put_Abbreviation(This,abbrev)

#define IProjectedCoordinateSystem_get_Remarks(This,remarks)	\
    (This)->lpVtbl -> get_Remarks(This,remarks)

#define IProjectedCoordinateSystem_put_Remarks(This,remarks)	\
    (This)->lpVtbl -> put_Remarks(This,remarks)

#define IProjectedCoordinateSystem_get_WellKnownText(This,wkt)	\
    (This)->lpVtbl -> get_WellKnownText(This,wkt)




#define IProjectedCoordinateSystem_get_Usage(This,usage)	\
    (This)->lpVtbl -> get_Usage(This,usage)

#define IProjectedCoordinateSystem_put_Usage(This,usage)	\
    (This)->lpVtbl -> put_Usage(This,usage)

#define IProjectedCoordinateSystem_get_GeographicCoordinateSystem(This,gcs)	\
    (This)->lpVtbl -> get_GeographicCoordinateSystem(This,gcs)

#define IProjectedCoordinateSystem_put_GeographicCoordinateSystem(This,gcs)	\
    (This)->lpVtbl -> put_GeographicCoordinateSystem(This,gcs)

#define IProjectedCoordinateSystem_get_LinearUnit(This,unit)	\
    (This)->lpVtbl -> get_LinearUnit(This,unit)

#define IProjectedCoordinateSystem_put_LinearUnit(This,unit)	\
    (This)->lpVtbl -> put_LinearUnit(This,unit)

#define IProjectedCoordinateSystem_get_Projection(This,projection)	\
    (This)->lpVtbl -> get_Projection(This,projection)

#define IProjectedCoordinateSystem_put_Projection(This,projection)	\
    (This)->lpVtbl -> put_Projection(This,projection)

#define IProjectedCoordinateSystem_get_DefaultParameterInfo(This,paramInfo)	\
    (This)->lpVtbl -> get_DefaultParameterInfo(This,paramInfo)

#define IProjectedCoordinateSystem_get_ParameterInfo(This,paramInfo)	\
    (This)->lpVtbl -> get_ParameterInfo(This,paramInfo)

#define IProjectedCoordinateSystem_put_ParameterInfo(This,paramInfo)	\
    (This)->lpVtbl -> put_ParameterInfo(This,paramInfo)

#define IProjectedCoordinateSystem_Forward(This,count,points)	\
    (This)->lpVtbl -> Forward(This,count,points)

#define IProjectedCoordinateSystem_Inverse(This,count,points)	\
    (This)->lpVtbl -> Inverse(This,count,points)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IProjectedCoordinateSystem_get_Usage_Proxy( 
    IProjectedCoordinateSystem __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *usage);


void __RPC_STUB IProjectedCoordinateSystem_get_Usage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IProjectedCoordinateSystem_put_Usage_Proxy( 
    IProjectedCoordinateSystem __RPC_FAR * This,
    /* [in] */ BSTR usage);


void __RPC_STUB IProjectedCoordinateSystem_put_Usage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IProjectedCoordinateSystem_get_GeographicCoordinateSystem_Proxy( 
    IProjectedCoordinateSystem __RPC_FAR * This,
    /* [retval][out] */ IGeographicCoordinateSystem __RPC_FAR *__RPC_FAR *gcs);


void __RPC_STUB IProjectedCoordinateSystem_get_GeographicCoordinateSystem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IProjectedCoordinateSystem_put_GeographicCoordinateSystem_Proxy( 
    IProjectedCoordinateSystem __RPC_FAR * This,
    /* [in] */ IGeographicCoordinateSystem __RPC_FAR *gcs);


void __RPC_STUB IProjectedCoordinateSystem_put_GeographicCoordinateSystem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IProjectedCoordinateSystem_get_LinearUnit_Proxy( 
    IProjectedCoordinateSystem __RPC_FAR * This,
    /* [retval][out] */ ILinearUnit __RPC_FAR *__RPC_FAR *unit);


void __RPC_STUB IProjectedCoordinateSystem_get_LinearUnit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IProjectedCoordinateSystem_put_LinearUnit_Proxy( 
    IProjectedCoordinateSystem __RPC_FAR * This,
    /* [in] */ ILinearUnit __RPC_FAR *unit);


void __RPC_STUB IProjectedCoordinateSystem_put_LinearUnit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IProjectedCoordinateSystem_get_Projection_Proxy( 
    IProjectedCoordinateSystem __RPC_FAR * This,
    /* [retval][out] */ IProjection __RPC_FAR *__RPC_FAR *projection);


void __RPC_STUB IProjectedCoordinateSystem_get_Projection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IProjectedCoordinateSystem_put_Projection_Proxy( 
    IProjectedCoordinateSystem __RPC_FAR * This,
    /* [in] */ IProjection __RPC_FAR *projection);


void __RPC_STUB IProjectedCoordinateSystem_put_Projection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IProjectedCoordinateSystem_get_DefaultParameterInfo_Proxy( 
    IProjectedCoordinateSystem __RPC_FAR * This,
    /* [retval][out] */ IParameterInfo __RPC_FAR *__RPC_FAR *paramInfo);


void __RPC_STUB IProjectedCoordinateSystem_get_DefaultParameterInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IProjectedCoordinateSystem_get_ParameterInfo_Proxy( 
    IProjectedCoordinateSystem __RPC_FAR * This,
    /* [retval][out] */ IParameterInfo __RPC_FAR *__RPC_FAR *paramInfo);


void __RPC_STUB IProjectedCoordinateSystem_get_ParameterInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propput] */ HRESULT STDMETHODCALLTYPE IProjectedCoordinateSystem_put_ParameterInfo_Proxy( 
    IProjectedCoordinateSystem __RPC_FAR * This,
    /* [in] */ IParameterInfo __RPC_FAR *paramInfo);


void __RPC_STUB IProjectedCoordinateSystem_put_ParameterInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IProjectedCoordinateSystem_Forward_Proxy( 
    IProjectedCoordinateSystem __RPC_FAR * This,
    /* [in] */ long count,
    /* [size_is][out][in] */ WKSPoint __RPC_FAR points[  ]);


void __RPC_STUB IProjectedCoordinateSystem_Forward_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IProjectedCoordinateSystem_Inverse_Proxy( 
    IProjectedCoordinateSystem __RPC_FAR * This,
    /* [in] */ long count,
    /* [size_is][out][in] */ WKSPoint __RPC_FAR points[  ]);


void __RPC_STUB IProjectedCoordinateSystem_Inverse_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IProjectedCoordinateSystem_INTERFACE_DEFINED__ */


#ifndef __ISpatialReferenceFactory_INTERFACE_DEFINED__
#define __ISpatialReferenceFactory_INTERFACE_DEFINED__

/* interface ISpatialReferenceFactory */
/* [uuid][object] */ 


EXTERN_C const IID IID_ISpatialReferenceFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("620600B1-FEA1-11d0-B04B-0080C7F79481")
    ISpatialReferenceFactory : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateFromWKT( 
            /* [in] */ BSTR wktSpatialReference,
            /* [retval][out] */ ISpatialReference __RPC_FAR *__RPC_FAR *sref) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISpatialReferenceFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISpatialReferenceFactory __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISpatialReferenceFactory __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISpatialReferenceFactory __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateFromWKT )( 
            ISpatialReferenceFactory __RPC_FAR * This,
            /* [in] */ BSTR wktSpatialReference,
            /* [retval][out] */ ISpatialReference __RPC_FAR *__RPC_FAR *sref);
        
        END_INTERFACE
    } ISpatialReferenceFactoryVtbl;

    interface ISpatialReferenceFactory
    {
        CONST_VTBL struct ISpatialReferenceFactoryVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISpatialReferenceFactory_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISpatialReferenceFactory_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISpatialReferenceFactory_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISpatialReferenceFactory_CreateFromWKT(This,wktSpatialReference,sref)	\
    (This)->lpVtbl -> CreateFromWKT(This,wktSpatialReference,sref)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ISpatialReferenceFactory_CreateFromWKT_Proxy( 
    ISpatialReferenceFactory __RPC_FAR * This,
    /* [in] */ BSTR wktSpatialReference,
    /* [retval][out] */ ISpatialReference __RPC_FAR *__RPC_FAR *sref);


void __RPC_STUB ISpatialReferenceFactory_CreateFromWKT_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISpatialReferenceFactory_INTERFACE_DEFINED__ */


#ifndef __ISpatialReferenceAuthorityFactory_INTERFACE_DEFINED__
#define __ISpatialReferenceAuthorityFactory_INTERFACE_DEFINED__

/* interface ISpatialReferenceAuthorityFactory */
/* [uuid][object] */ 


EXTERN_C const IID IID_ISpatialReferenceAuthorityFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("30ae14f0-fe1f-11d0-ad87-080009b6f22b")
    ISpatialReferenceAuthorityFactory : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Authority( 
            /* [retval][out] */ BSTR __RPC_FAR *authority) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateProjectedCoordinateSystem( 
            /* [in] */ long code,
            /* [retval][out] */ IProjectedCoordinateSystem __RPC_FAR *__RPC_FAR *pcs) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateGeographicCoordinateSystem( 
            /* [in] */ long code,
            /* [retval][out] */ IGeographicCoordinateSystem __RPC_FAR *__RPC_FAR *gcs) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateProjection( 
            /* [in] */ long code,
            /* [retval][out] */ IProjection __RPC_FAR *__RPC_FAR *projection) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateGeographicTransform( 
            /* [in] */ long code,
            /* [retval][out] */ IGeographicTransform __RPC_FAR *__RPC_FAR *gt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateHorizontalDatum( 
            /* [in] */ long code,
            /* [retval][out] */ IHorizontalDatum __RPC_FAR *__RPC_FAR *datum) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateEllipsoid( 
            /* [in] */ long code,
            /* [retval][out] */ IEllipsoid __RPC_FAR *__RPC_FAR *ellipsoid) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreatePrimeMeridian( 
            /* [in] */ long code,
            /* [retval][out] */ IPrimeMeridian __RPC_FAR *__RPC_FAR *prmMerid) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateLinearUnit( 
            /* [in] */ long code,
            /* [retval][out] */ ILinearUnit __RPC_FAR *__RPC_FAR *unit) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateAngularUnit( 
            /* [in] */ long code,
            /* [retval][out] */ IAngularUnit __RPC_FAR *__RPC_FAR *unit) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISpatialReferenceAuthorityFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISpatialReferenceAuthorityFactory __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISpatialReferenceAuthorityFactory __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISpatialReferenceAuthorityFactory __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Authority )( 
            ISpatialReferenceAuthorityFactory __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *authority);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateProjectedCoordinateSystem )( 
            ISpatialReferenceAuthorityFactory __RPC_FAR * This,
            /* [in] */ long code,
            /* [retval][out] */ IProjectedCoordinateSystem __RPC_FAR *__RPC_FAR *pcs);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateGeographicCoordinateSystem )( 
            ISpatialReferenceAuthorityFactory __RPC_FAR * This,
            /* [in] */ long code,
            /* [retval][out] */ IGeographicCoordinateSystem __RPC_FAR *__RPC_FAR *gcs);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateProjection )( 
            ISpatialReferenceAuthorityFactory __RPC_FAR * This,
            /* [in] */ long code,
            /* [retval][out] */ IProjection __RPC_FAR *__RPC_FAR *projection);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateGeographicTransform )( 
            ISpatialReferenceAuthorityFactory __RPC_FAR * This,
            /* [in] */ long code,
            /* [retval][out] */ IGeographicTransform __RPC_FAR *__RPC_FAR *gt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateHorizontalDatum )( 
            ISpatialReferenceAuthorityFactory __RPC_FAR * This,
            /* [in] */ long code,
            /* [retval][out] */ IHorizontalDatum __RPC_FAR *__RPC_FAR *datum);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateEllipsoid )( 
            ISpatialReferenceAuthorityFactory __RPC_FAR * This,
            /* [in] */ long code,
            /* [retval][out] */ IEllipsoid __RPC_FAR *__RPC_FAR *ellipsoid);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreatePrimeMeridian )( 
            ISpatialReferenceAuthorityFactory __RPC_FAR * This,
            /* [in] */ long code,
            /* [retval][out] */ IPrimeMeridian __RPC_FAR *__RPC_FAR *prmMerid);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateLinearUnit )( 
            ISpatialReferenceAuthorityFactory __RPC_FAR * This,
            /* [in] */ long code,
            /* [retval][out] */ ILinearUnit __RPC_FAR *__RPC_FAR *unit);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateAngularUnit )( 
            ISpatialReferenceAuthorityFactory __RPC_FAR * This,
            /* [in] */ long code,
            /* [retval][out] */ IAngularUnit __RPC_FAR *__RPC_FAR *unit);
        
        END_INTERFACE
    } ISpatialReferenceAuthorityFactoryVtbl;

    interface ISpatialReferenceAuthorityFactory
    {
        CONST_VTBL struct ISpatialReferenceAuthorityFactoryVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISpatialReferenceAuthorityFactory_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISpatialReferenceAuthorityFactory_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISpatialReferenceAuthorityFactory_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISpatialReferenceAuthorityFactory_get_Authority(This,authority)	\
    (This)->lpVtbl -> get_Authority(This,authority)

#define ISpatialReferenceAuthorityFactory_CreateProjectedCoordinateSystem(This,code,pcs)	\
    (This)->lpVtbl -> CreateProjectedCoordinateSystem(This,code,pcs)

#define ISpatialReferenceAuthorityFactory_CreateGeographicCoordinateSystem(This,code,gcs)	\
    (This)->lpVtbl -> CreateGeographicCoordinateSystem(This,code,gcs)

#define ISpatialReferenceAuthorityFactory_CreateProjection(This,code,projection)	\
    (This)->lpVtbl -> CreateProjection(This,code,projection)

#define ISpatialReferenceAuthorityFactory_CreateGeographicTransform(This,code,gt)	\
    (This)->lpVtbl -> CreateGeographicTransform(This,code,gt)

#define ISpatialReferenceAuthorityFactory_CreateHorizontalDatum(This,code,datum)	\
    (This)->lpVtbl -> CreateHorizontalDatum(This,code,datum)

#define ISpatialReferenceAuthorityFactory_CreateEllipsoid(This,code,ellipsoid)	\
    (This)->lpVtbl -> CreateEllipsoid(This,code,ellipsoid)

#define ISpatialReferenceAuthorityFactory_CreatePrimeMeridian(This,code,prmMerid)	\
    (This)->lpVtbl -> CreatePrimeMeridian(This,code,prmMerid)

#define ISpatialReferenceAuthorityFactory_CreateLinearUnit(This,code,unit)	\
    (This)->lpVtbl -> CreateLinearUnit(This,code,unit)

#define ISpatialReferenceAuthorityFactory_CreateAngularUnit(This,code,unit)	\
    (This)->lpVtbl -> CreateAngularUnit(This,code,unit)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE ISpatialReferenceAuthorityFactory_get_Authority_Proxy( 
    ISpatialReferenceAuthorityFactory __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *authority);


void __RPC_STUB ISpatialReferenceAuthorityFactory_get_Authority_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpatialReferenceAuthorityFactory_CreateProjectedCoordinateSystem_Proxy( 
    ISpatialReferenceAuthorityFactory __RPC_FAR * This,
    /* [in] */ long code,
    /* [retval][out] */ IProjectedCoordinateSystem __RPC_FAR *__RPC_FAR *pcs);


void __RPC_STUB ISpatialReferenceAuthorityFactory_CreateProjectedCoordinateSystem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpatialReferenceAuthorityFactory_CreateGeographicCoordinateSystem_Proxy( 
    ISpatialReferenceAuthorityFactory __RPC_FAR * This,
    /* [in] */ long code,
    /* [retval][out] */ IGeographicCoordinateSystem __RPC_FAR *__RPC_FAR *gcs);


void __RPC_STUB ISpatialReferenceAuthorityFactory_CreateGeographicCoordinateSystem_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpatialReferenceAuthorityFactory_CreateProjection_Proxy( 
    ISpatialReferenceAuthorityFactory __RPC_FAR * This,
    /* [in] */ long code,
    /* [retval][out] */ IProjection __RPC_FAR *__RPC_FAR *projection);


void __RPC_STUB ISpatialReferenceAuthorityFactory_CreateProjection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpatialReferenceAuthorityFactory_CreateGeographicTransform_Proxy( 
    ISpatialReferenceAuthorityFactory __RPC_FAR * This,
    /* [in] */ long code,
    /* [retval][out] */ IGeographicTransform __RPC_FAR *__RPC_FAR *gt);


void __RPC_STUB ISpatialReferenceAuthorityFactory_CreateGeographicTransform_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpatialReferenceAuthorityFactory_CreateHorizontalDatum_Proxy( 
    ISpatialReferenceAuthorityFactory __RPC_FAR * This,
    /* [in] */ long code,
    /* [retval][out] */ IHorizontalDatum __RPC_FAR *__RPC_FAR *datum);


void __RPC_STUB ISpatialReferenceAuthorityFactory_CreateHorizontalDatum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpatialReferenceAuthorityFactory_CreateEllipsoid_Proxy( 
    ISpatialReferenceAuthorityFactory __RPC_FAR * This,
    /* [in] */ long code,
    /* [retval][out] */ IEllipsoid __RPC_FAR *__RPC_FAR *ellipsoid);


void __RPC_STUB ISpatialReferenceAuthorityFactory_CreateEllipsoid_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpatialReferenceAuthorityFactory_CreatePrimeMeridian_Proxy( 
    ISpatialReferenceAuthorityFactory __RPC_FAR * This,
    /* [in] */ long code,
    /* [retval][out] */ IPrimeMeridian __RPC_FAR *__RPC_FAR *prmMerid);


void __RPC_STUB ISpatialReferenceAuthorityFactory_CreatePrimeMeridian_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpatialReferenceAuthorityFactory_CreateLinearUnit_Proxy( 
    ISpatialReferenceAuthorityFactory __RPC_FAR * This,
    /* [in] */ long code,
    /* [retval][out] */ ILinearUnit __RPC_FAR *__RPC_FAR *unit);


void __RPC_STUB ISpatialReferenceAuthorityFactory_CreateLinearUnit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpatialReferenceAuthorityFactory_CreateAngularUnit_Proxy( 
    ISpatialReferenceAuthorityFactory __RPC_FAR * This,
    /* [in] */ long code,
    /* [retval][out] */ IAngularUnit __RPC_FAR *__RPC_FAR *unit);


void __RPC_STUB ISpatialReferenceAuthorityFactory_CreateAngularUnit_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISpatialReferenceAuthorityFactory_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
