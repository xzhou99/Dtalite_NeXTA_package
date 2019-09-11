/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Apr 19 22:10:03 2001
 */
/* Compiler settings for geometryidl.idl:
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

#ifndef __geometryidl_h__
#define __geometryidl_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IGeometry_FWD_DEFINED__
#define __IGeometry_FWD_DEFINED__
typedef interface IGeometry IGeometry;
#endif 	/* __IGeometry_FWD_DEFINED__ */


#ifndef __IWks_FWD_DEFINED__
#define __IWks_FWD_DEFINED__
typedef interface IWks IWks;
#endif 	/* __IWks_FWD_DEFINED__ */


#ifndef __IGeometryFactory_FWD_DEFINED__
#define __IGeometryFactory_FWD_DEFINED__
typedef interface IGeometryFactory IGeometryFactory;
#endif 	/* __IGeometryFactory_FWD_DEFINED__ */


#ifndef __IPoint_FWD_DEFINED__
#define __IPoint_FWD_DEFINED__
typedef interface IPoint IPoint;
#endif 	/* __IPoint_FWD_DEFINED__ */


#ifndef __IPointZ_FWD_DEFINED__
#define __IPointZ_FWD_DEFINED__
typedef interface IPointZ IPointZ;
#endif 	/* __IPointZ_FWD_DEFINED__ */


#ifndef __ICurve_FWD_DEFINED__
#define __ICurve_FWD_DEFINED__
typedef interface ICurve ICurve;
#endif 	/* __ICurve_FWD_DEFINED__ */


#ifndef __ILineString_FWD_DEFINED__
#define __ILineString_FWD_DEFINED__
typedef interface ILineString ILineString;
#endif 	/* __ILineString_FWD_DEFINED__ */


#ifndef __ILinearRing_FWD_DEFINED__
#define __ILinearRing_FWD_DEFINED__
typedef interface ILinearRing ILinearRing;
#endif 	/* __ILinearRing_FWD_DEFINED__ */


#ifndef __ISurface_FWD_DEFINED__
#define __ISurface_FWD_DEFINED__
typedef interface ISurface ISurface;
#endif 	/* __ISurface_FWD_DEFINED__ */


#ifndef __IGeometryCollection_FWD_DEFINED__
#define __IGeometryCollection_FWD_DEFINED__
typedef interface IGeometryCollection IGeometryCollection;
#endif 	/* __IGeometryCollection_FWD_DEFINED__ */


#ifndef __IPolygon_FWD_DEFINED__
#define __IPolygon_FWD_DEFINED__
typedef interface IPolygon IPolygon;
#endif 	/* __IPolygon_FWD_DEFINED__ */


#ifndef __IMultiCurve_FWD_DEFINED__
#define __IMultiCurve_FWD_DEFINED__
typedef interface IMultiCurve IMultiCurve;
#endif 	/* __IMultiCurve_FWD_DEFINED__ */


#ifndef __IMultiSurface_FWD_DEFINED__
#define __IMultiSurface_FWD_DEFINED__
typedef interface IMultiSurface IMultiSurface;
#endif 	/* __IMultiSurface_FWD_DEFINED__ */


#ifndef __ISpatialRelation_FWD_DEFINED__
#define __ISpatialRelation_FWD_DEFINED__
typedef interface ISpatialRelation ISpatialRelation;
#endif 	/* __ISpatialRelation_FWD_DEFINED__ */


#ifndef __ISpatialRelation2_FWD_DEFINED__
#define __ISpatialRelation2_FWD_DEFINED__
typedef interface ISpatialRelation2 ISpatialRelation2;
#endif 	/* __ISpatialRelation2_FWD_DEFINED__ */


#ifndef __ISpatialOperator_FWD_DEFINED__
#define __ISpatialOperator_FWD_DEFINED__
typedef interface ISpatialOperator ISpatialOperator;
#endif 	/* __ISpatialOperator_FWD_DEFINED__ */


/* header files for imported files */
#include "ocidl.h"
#include "SpatialReferenceIdl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_geometryidl_0000 */
/* [local] */ 















extern RPC_IF_HANDLE __MIDL_itf_geometryidl_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_geometryidl_0000_v0_0_s_ifspec;

#ifndef __IGeometry_INTERFACE_DEFINED__
#define __IGeometry_INTERFACE_DEFINED__

/* interface IGeometry */
/* [uuid][object] */ 


EXTERN_C const IID IID_IGeometry;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6A124031-FE38-11d0-BECE-00805F7C4268")
    IGeometry : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Dimension( 
            /* [retval][out] */ long __RPC_FAR *dimension) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_SpatialReference( 
            /* [retval][out] */ ISpatialReference __RPC_FAR *__RPC_FAR *spatialRef) = 0;
        
        virtual /* [propputref] */ HRESULT STDMETHODCALLTYPE putref_SpatialReference( 
            /* [in] */ ISpatialReference __RPC_FAR *spatialRef) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsEmpty( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isEmpty) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetEmpty( void) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsSimple( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isSimple) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Envelope( 
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *envelope) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *newShape) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Project( 
            /* [in] */ ISpatialReference __RPC_FAR *newSystem,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Extent2D( 
            /* [out] */ double __RPC_FAR *minX,
            /* [out] */ double __RPC_FAR *minY,
            /* [out] */ double __RPC_FAR *maxX,
            /* [out] */ double __RPC_FAR *maxY) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IGeometryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IGeometry __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IGeometry __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IGeometry __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Dimension )( 
            IGeometry __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *dimension);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SpatialReference )( 
            IGeometry __RPC_FAR * This,
            /* [retval][out] */ ISpatialReference __RPC_FAR *__RPC_FAR *spatialRef);
        
        /* [propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_SpatialReference )( 
            IGeometry __RPC_FAR * This,
            /* [in] */ ISpatialReference __RPC_FAR *spatialRef);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsEmpty )( 
            IGeometry __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isEmpty);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEmpty )( 
            IGeometry __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsSimple )( 
            IGeometry __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isSimple);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Envelope )( 
            IGeometry __RPC_FAR * This,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *envelope);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IGeometry __RPC_FAR * This,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *newShape);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Project )( 
            IGeometry __RPC_FAR * This,
            /* [in] */ ISpatialReference __RPC_FAR *newSystem,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Extent2D )( 
            IGeometry __RPC_FAR * This,
            /* [out] */ double __RPC_FAR *minX,
            /* [out] */ double __RPC_FAR *minY,
            /* [out] */ double __RPC_FAR *maxX,
            /* [out] */ double __RPC_FAR *maxY);
        
        END_INTERFACE
    } IGeometryVtbl;

    interface IGeometry
    {
        CONST_VTBL struct IGeometryVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGeometry_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IGeometry_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IGeometry_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IGeometry_get_Dimension(This,dimension)	\
    (This)->lpVtbl -> get_Dimension(This,dimension)

#define IGeometry_get_SpatialReference(This,spatialRef)	\
    (This)->lpVtbl -> get_SpatialReference(This,spatialRef)

#define IGeometry_putref_SpatialReference(This,spatialRef)	\
    (This)->lpVtbl -> putref_SpatialReference(This,spatialRef)

#define IGeometry_get_IsEmpty(This,isEmpty)	\
    (This)->lpVtbl -> get_IsEmpty(This,isEmpty)

#define IGeometry_SetEmpty(This)	\
    (This)->lpVtbl -> SetEmpty(This)

#define IGeometry_get_IsSimple(This,isSimple)	\
    (This)->lpVtbl -> get_IsSimple(This,isSimple)

#define IGeometry_Envelope(This,envelope)	\
    (This)->lpVtbl -> Envelope(This,envelope)

#define IGeometry_Clone(This,newShape)	\
    (This)->lpVtbl -> Clone(This,newShape)

#define IGeometry_Project(This,newSystem,result)	\
    (This)->lpVtbl -> Project(This,newSystem,result)

#define IGeometry_Extent2D(This,minX,minY,maxX,maxY)	\
    (This)->lpVtbl -> Extent2D(This,minX,minY,maxX,maxY)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IGeometry_get_Dimension_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *dimension);


void __RPC_STUB IGeometry_get_Dimension_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IGeometry_get_SpatialReference_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [retval][out] */ ISpatialReference __RPC_FAR *__RPC_FAR *spatialRef);


void __RPC_STUB IGeometry_get_SpatialReference_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propputref] */ HRESULT STDMETHODCALLTYPE IGeometry_putref_SpatialReference_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [in] */ ISpatialReference __RPC_FAR *spatialRef);


void __RPC_STUB IGeometry_putref_SpatialReference_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IGeometry_get_IsEmpty_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isEmpty);


void __RPC_STUB IGeometry_get_IsEmpty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IGeometry_SetEmpty_Proxy( 
    IGeometry __RPC_FAR * This);


void __RPC_STUB IGeometry_SetEmpty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IGeometry_get_IsSimple_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isSimple);


void __RPC_STUB IGeometry_get_IsSimple_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IGeometry_Envelope_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *envelope);


void __RPC_STUB IGeometry_Envelope_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IGeometry_Clone_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *newShape);


void __RPC_STUB IGeometry_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IGeometry_Project_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [in] */ ISpatialReference __RPC_FAR *newSystem,
    /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result);


void __RPC_STUB IGeometry_Project_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IGeometry_Extent2D_Proxy( 
    IGeometry __RPC_FAR * This,
    /* [out] */ double __RPC_FAR *minX,
    /* [out] */ double __RPC_FAR *minY,
    /* [out] */ double __RPC_FAR *maxX,
    /* [out] */ double __RPC_FAR *maxY);


void __RPC_STUB IGeometry_Extent2D_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IGeometry_INTERFACE_DEFINED__ */


#ifndef __IWks_INTERFACE_DEFINED__
#define __IWks_INTERFACE_DEFINED__

/* interface IWks */
/* [uuid][object] */ 


EXTERN_C const IID IID_IWks;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6A124032-FE38-11d0-BECE-00805F7C4268")
    IWks : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE ExportToWKB( 
            /* [retval][out] */ VARIANT __RPC_FAR *wkb) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ExportToWKT( 
            /* [retval][out] */ BSTR __RPC_FAR *wkt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ImportFromWKB( 
            /* [in] */ VARIANT wkb,
            /* [in] */ ISpatialReference __RPC_FAR *spatialRef) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ImportFromWKT( 
            /* [in] */ BSTR wkt,
            /* [in] */ ISpatialReference __RPC_FAR *spatialRef) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IWksVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IWks __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IWks __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IWks __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ExportToWKB )( 
            IWks __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *wkb);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ExportToWKT )( 
            IWks __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *wkt);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ImportFromWKB )( 
            IWks __RPC_FAR * This,
            /* [in] */ VARIANT wkb,
            /* [in] */ ISpatialReference __RPC_FAR *spatialRef);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ImportFromWKT )( 
            IWks __RPC_FAR * This,
            /* [in] */ BSTR wkt,
            /* [in] */ ISpatialReference __RPC_FAR *spatialRef);
        
        END_INTERFACE
    } IWksVtbl;

    interface IWks
    {
        CONST_VTBL struct IWksVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWks_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IWks_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IWks_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IWks_ExportToWKB(This,wkb)	\
    (This)->lpVtbl -> ExportToWKB(This,wkb)

#define IWks_ExportToWKT(This,wkt)	\
    (This)->lpVtbl -> ExportToWKT(This,wkt)

#define IWks_ImportFromWKB(This,wkb,spatialRef)	\
    (This)->lpVtbl -> ImportFromWKB(This,wkb,spatialRef)

#define IWks_ImportFromWKT(This,wkt,spatialRef)	\
    (This)->lpVtbl -> ImportFromWKT(This,wkt,spatialRef)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IWks_ExportToWKB_Proxy( 
    IWks __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *wkb);


void __RPC_STUB IWks_ExportToWKB_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWks_ExportToWKT_Proxy( 
    IWks __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *wkt);


void __RPC_STUB IWks_ExportToWKT_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWks_ImportFromWKB_Proxy( 
    IWks __RPC_FAR * This,
    /* [in] */ VARIANT wkb,
    /* [in] */ ISpatialReference __RPC_FAR *spatialRef);


void __RPC_STUB IWks_ImportFromWKB_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IWks_ImportFromWKT_Proxy( 
    IWks __RPC_FAR * This,
    /* [in] */ BSTR wkt,
    /* [in] */ ISpatialReference __RPC_FAR *spatialRef);


void __RPC_STUB IWks_ImportFromWKT_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IWks_INTERFACE_DEFINED__ */


#ifndef __IGeometryFactory_INTERFACE_DEFINED__
#define __IGeometryFactory_INTERFACE_DEFINED__

/* interface IGeometryFactory */
/* [uuid][object] */ 


EXTERN_C const IID IID_IGeometryFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6A124033-FE38-11d0-BECE-00805F7C4268")
    IGeometryFactory : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateFromWKB( 
            /* [in] */ VARIANT wkb,
            /* [in] */ ISpatialReference __RPC_FAR *spatialRef,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *geometry) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateFromWKT( 
            /* [in] */ BSTR wkt,
            /* [in] */ ISpatialReference __RPC_FAR *spatialRef,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *geometry) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IGeometryFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IGeometryFactory __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IGeometryFactory __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IGeometryFactory __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateFromWKB )( 
            IGeometryFactory __RPC_FAR * This,
            /* [in] */ VARIANT wkb,
            /* [in] */ ISpatialReference __RPC_FAR *spatialRef,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *geometry);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateFromWKT )( 
            IGeometryFactory __RPC_FAR * This,
            /* [in] */ BSTR wkt,
            /* [in] */ ISpatialReference __RPC_FAR *spatialRef,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *geometry);
        
        END_INTERFACE
    } IGeometryFactoryVtbl;

    interface IGeometryFactory
    {
        CONST_VTBL struct IGeometryFactoryVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGeometryFactory_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IGeometryFactory_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IGeometryFactory_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IGeometryFactory_CreateFromWKB(This,wkb,spatialRef,geometry)	\
    (This)->lpVtbl -> CreateFromWKB(This,wkb,spatialRef,geometry)

#define IGeometryFactory_CreateFromWKT(This,wkt,spatialRef,geometry)	\
    (This)->lpVtbl -> CreateFromWKT(This,wkt,spatialRef,geometry)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IGeometryFactory_CreateFromWKB_Proxy( 
    IGeometryFactory __RPC_FAR * This,
    /* [in] */ VARIANT wkb,
    /* [in] */ ISpatialReference __RPC_FAR *spatialRef,
    /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *geometry);


void __RPC_STUB IGeometryFactory_CreateFromWKB_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IGeometryFactory_CreateFromWKT_Proxy( 
    IGeometryFactory __RPC_FAR * This,
    /* [in] */ BSTR wkt,
    /* [in] */ ISpatialReference __RPC_FAR *spatialRef,
    /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *geometry);


void __RPC_STUB IGeometryFactory_CreateFromWKT_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IGeometryFactory_INTERFACE_DEFINED__ */


#ifndef __IPoint_INTERFACE_DEFINED__
#define __IPoint_INTERFACE_DEFINED__

/* interface IPoint */
/* [uuid][object] */ 


EXTERN_C const IID IID_IPoint;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6A124035-FE38-11d0-BECE-00805F7C4268")
    IPoint : public IGeometry
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Coords( 
            /* [out] */ double __RPC_FAR *x,
            /* [out] */ double __RPC_FAR *y) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_X( 
            /* [retval][out] */ double __RPC_FAR *x) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Y( 
            /* [retval][out] */ double __RPC_FAR *y) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPointVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPoint __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPoint __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPoint __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Dimension )( 
            IPoint __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *dimension);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SpatialReference )( 
            IPoint __RPC_FAR * This,
            /* [retval][out] */ ISpatialReference __RPC_FAR *__RPC_FAR *spatialRef);
        
        /* [propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_SpatialReference )( 
            IPoint __RPC_FAR * This,
            /* [in] */ ISpatialReference __RPC_FAR *spatialRef);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsEmpty )( 
            IPoint __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isEmpty);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEmpty )( 
            IPoint __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsSimple )( 
            IPoint __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isSimple);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Envelope )( 
            IPoint __RPC_FAR * This,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *envelope);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IPoint __RPC_FAR * This,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *newShape);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Project )( 
            IPoint __RPC_FAR * This,
            /* [in] */ ISpatialReference __RPC_FAR *newSystem,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Extent2D )( 
            IPoint __RPC_FAR * This,
            /* [out] */ double __RPC_FAR *minX,
            /* [out] */ double __RPC_FAR *minY,
            /* [out] */ double __RPC_FAR *maxX,
            /* [out] */ double __RPC_FAR *maxY);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Coords )( 
            IPoint __RPC_FAR * This,
            /* [out] */ double __RPC_FAR *x,
            /* [out] */ double __RPC_FAR *y);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_X )( 
            IPoint __RPC_FAR * This,
            /* [retval][out] */ double __RPC_FAR *x);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Y )( 
            IPoint __RPC_FAR * This,
            /* [retval][out] */ double __RPC_FAR *y);
        
        END_INTERFACE
    } IPointVtbl;

    interface IPoint
    {
        CONST_VTBL struct IPointVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPoint_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPoint_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPoint_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPoint_get_Dimension(This,dimension)	\
    (This)->lpVtbl -> get_Dimension(This,dimension)

#define IPoint_get_SpatialReference(This,spatialRef)	\
    (This)->lpVtbl -> get_SpatialReference(This,spatialRef)

#define IPoint_putref_SpatialReference(This,spatialRef)	\
    (This)->lpVtbl -> putref_SpatialReference(This,spatialRef)

#define IPoint_get_IsEmpty(This,isEmpty)	\
    (This)->lpVtbl -> get_IsEmpty(This,isEmpty)

#define IPoint_SetEmpty(This)	\
    (This)->lpVtbl -> SetEmpty(This)

#define IPoint_get_IsSimple(This,isSimple)	\
    (This)->lpVtbl -> get_IsSimple(This,isSimple)

#define IPoint_Envelope(This,envelope)	\
    (This)->lpVtbl -> Envelope(This,envelope)

#define IPoint_Clone(This,newShape)	\
    (This)->lpVtbl -> Clone(This,newShape)

#define IPoint_Project(This,newSystem,result)	\
    (This)->lpVtbl -> Project(This,newSystem,result)

#define IPoint_Extent2D(This,minX,minY,maxX,maxY)	\
    (This)->lpVtbl -> Extent2D(This,minX,minY,maxX,maxY)


#define IPoint_Coords(This,x,y)	\
    (This)->lpVtbl -> Coords(This,x,y)

#define IPoint_get_X(This,x)	\
    (This)->lpVtbl -> get_X(This,x)

#define IPoint_get_Y(This,y)	\
    (This)->lpVtbl -> get_Y(This,y)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IPoint_Coords_Proxy( 
    IPoint __RPC_FAR * This,
    /* [out] */ double __RPC_FAR *x,
    /* [out] */ double __RPC_FAR *y);


void __RPC_STUB IPoint_Coords_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPoint_get_X_Proxy( 
    IPoint __RPC_FAR * This,
    /* [retval][out] */ double __RPC_FAR *x);


void __RPC_STUB IPoint_get_X_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPoint_get_Y_Proxy( 
    IPoint __RPC_FAR * This,
    /* [retval][out] */ double __RPC_FAR *y);


void __RPC_STUB IPoint_get_Y_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPoint_INTERFACE_DEFINED__ */


#ifndef __IPointZ_INTERFACE_DEFINED__
#define __IPointZ_INTERFACE_DEFINED__

/* interface IPointZ */
/* [uuid][object] */ 


EXTERN_C const IID IID_IPointZ;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D4579E2D-1D2B-11d3-80BE-00C04F680FFF")
    IPointZ : public IPoint
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Z( 
            /* [retval][out] */ double __RPC_FAR *z) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPointZVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPointZ __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPointZ __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPointZ __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Dimension )( 
            IPointZ __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *dimension);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SpatialReference )( 
            IPointZ __RPC_FAR * This,
            /* [retval][out] */ ISpatialReference __RPC_FAR *__RPC_FAR *spatialRef);
        
        /* [propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_SpatialReference )( 
            IPointZ __RPC_FAR * This,
            /* [in] */ ISpatialReference __RPC_FAR *spatialRef);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsEmpty )( 
            IPointZ __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isEmpty);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEmpty )( 
            IPointZ __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsSimple )( 
            IPointZ __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isSimple);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Envelope )( 
            IPointZ __RPC_FAR * This,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *envelope);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IPointZ __RPC_FAR * This,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *newShape);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Project )( 
            IPointZ __RPC_FAR * This,
            /* [in] */ ISpatialReference __RPC_FAR *newSystem,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Extent2D )( 
            IPointZ __RPC_FAR * This,
            /* [out] */ double __RPC_FAR *minX,
            /* [out] */ double __RPC_FAR *minY,
            /* [out] */ double __RPC_FAR *maxX,
            /* [out] */ double __RPC_FAR *maxY);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Coords )( 
            IPointZ __RPC_FAR * This,
            /* [out] */ double __RPC_FAR *x,
            /* [out] */ double __RPC_FAR *y);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_X )( 
            IPointZ __RPC_FAR * This,
            /* [retval][out] */ double __RPC_FAR *x);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Y )( 
            IPointZ __RPC_FAR * This,
            /* [retval][out] */ double __RPC_FAR *y);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Z )( 
            IPointZ __RPC_FAR * This,
            /* [retval][out] */ double __RPC_FAR *z);
        
        END_INTERFACE
    } IPointZVtbl;

    interface IPointZ
    {
        CONST_VTBL struct IPointZVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPointZ_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPointZ_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPointZ_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPointZ_get_Dimension(This,dimension)	\
    (This)->lpVtbl -> get_Dimension(This,dimension)

#define IPointZ_get_SpatialReference(This,spatialRef)	\
    (This)->lpVtbl -> get_SpatialReference(This,spatialRef)

#define IPointZ_putref_SpatialReference(This,spatialRef)	\
    (This)->lpVtbl -> putref_SpatialReference(This,spatialRef)

#define IPointZ_get_IsEmpty(This,isEmpty)	\
    (This)->lpVtbl -> get_IsEmpty(This,isEmpty)

#define IPointZ_SetEmpty(This)	\
    (This)->lpVtbl -> SetEmpty(This)

#define IPointZ_get_IsSimple(This,isSimple)	\
    (This)->lpVtbl -> get_IsSimple(This,isSimple)

#define IPointZ_Envelope(This,envelope)	\
    (This)->lpVtbl -> Envelope(This,envelope)

#define IPointZ_Clone(This,newShape)	\
    (This)->lpVtbl -> Clone(This,newShape)

#define IPointZ_Project(This,newSystem,result)	\
    (This)->lpVtbl -> Project(This,newSystem,result)

#define IPointZ_Extent2D(This,minX,minY,maxX,maxY)	\
    (This)->lpVtbl -> Extent2D(This,minX,minY,maxX,maxY)


#define IPointZ_Coords(This,x,y)	\
    (This)->lpVtbl -> Coords(This,x,y)

#define IPointZ_get_X(This,x)	\
    (This)->lpVtbl -> get_X(This,x)

#define IPointZ_get_Y(This,y)	\
    (This)->lpVtbl -> get_Y(This,y)


#define IPointZ_get_Z(This,z)	\
    (This)->lpVtbl -> get_Z(This,z)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IPointZ_get_Z_Proxy( 
    IPointZ __RPC_FAR * This,
    /* [retval][out] */ double __RPC_FAR *z);


void __RPC_STUB IPointZ_get_Z_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPointZ_INTERFACE_DEFINED__ */


#ifndef __ICurve_INTERFACE_DEFINED__
#define __ICurve_INTERFACE_DEFINED__

/* interface ICurve */
/* [uuid][object] */ 


EXTERN_C const IID IID_ICurve;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6A124036-FE38-11d0-BECE-00805F7C4268")
    ICurve : public IGeometry
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Length( 
            /* [retval][out] */ double __RPC_FAR *value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE StartPoint( 
            /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *sp) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EndPoint( 
            /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *ep) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsClosed( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isClosed) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Value( 
            /* [in] */ double t,
            /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *p) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ICurveVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICurve __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICurve __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICurve __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Dimension )( 
            ICurve __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *dimension);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SpatialReference )( 
            ICurve __RPC_FAR * This,
            /* [retval][out] */ ISpatialReference __RPC_FAR *__RPC_FAR *spatialRef);
        
        /* [propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_SpatialReference )( 
            ICurve __RPC_FAR * This,
            /* [in] */ ISpatialReference __RPC_FAR *spatialRef);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsEmpty )( 
            ICurve __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isEmpty);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEmpty )( 
            ICurve __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsSimple )( 
            ICurve __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isSimple);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Envelope )( 
            ICurve __RPC_FAR * This,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *envelope);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            ICurve __RPC_FAR * This,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *newShape);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Project )( 
            ICurve __RPC_FAR * This,
            /* [in] */ ISpatialReference __RPC_FAR *newSystem,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Extent2D )( 
            ICurve __RPC_FAR * This,
            /* [out] */ double __RPC_FAR *minX,
            /* [out] */ double __RPC_FAR *minY,
            /* [out] */ double __RPC_FAR *maxX,
            /* [out] */ double __RPC_FAR *maxY);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Length )( 
            ICurve __RPC_FAR * This,
            /* [retval][out] */ double __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StartPoint )( 
            ICurve __RPC_FAR * This,
            /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *sp);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EndPoint )( 
            ICurve __RPC_FAR * This,
            /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *ep);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsClosed )( 
            ICurve __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isClosed);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Value )( 
            ICurve __RPC_FAR * This,
            /* [in] */ double t,
            /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *p);
        
        END_INTERFACE
    } ICurveVtbl;

    interface ICurve
    {
        CONST_VTBL struct ICurveVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICurve_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICurve_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICurve_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICurve_get_Dimension(This,dimension)	\
    (This)->lpVtbl -> get_Dimension(This,dimension)

#define ICurve_get_SpatialReference(This,spatialRef)	\
    (This)->lpVtbl -> get_SpatialReference(This,spatialRef)

#define ICurve_putref_SpatialReference(This,spatialRef)	\
    (This)->lpVtbl -> putref_SpatialReference(This,spatialRef)

#define ICurve_get_IsEmpty(This,isEmpty)	\
    (This)->lpVtbl -> get_IsEmpty(This,isEmpty)

#define ICurve_SetEmpty(This)	\
    (This)->lpVtbl -> SetEmpty(This)

#define ICurve_get_IsSimple(This,isSimple)	\
    (This)->lpVtbl -> get_IsSimple(This,isSimple)

#define ICurve_Envelope(This,envelope)	\
    (This)->lpVtbl -> Envelope(This,envelope)

#define ICurve_Clone(This,newShape)	\
    (This)->lpVtbl -> Clone(This,newShape)

#define ICurve_Project(This,newSystem,result)	\
    (This)->lpVtbl -> Project(This,newSystem,result)

#define ICurve_Extent2D(This,minX,minY,maxX,maxY)	\
    (This)->lpVtbl -> Extent2D(This,minX,minY,maxX,maxY)


#define ICurve_get_Length(This,value)	\
    (This)->lpVtbl -> get_Length(This,value)

#define ICurve_StartPoint(This,sp)	\
    (This)->lpVtbl -> StartPoint(This,sp)

#define ICurve_EndPoint(This,ep)	\
    (This)->lpVtbl -> EndPoint(This,ep)

#define ICurve_get_IsClosed(This,isClosed)	\
    (This)->lpVtbl -> get_IsClosed(This,isClosed)

#define ICurve_Value(This,t,p)	\
    (This)->lpVtbl -> Value(This,t,p)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE ICurve_get_Length_Proxy( 
    ICurve __RPC_FAR * This,
    /* [retval][out] */ double __RPC_FAR *value);


void __RPC_STUB ICurve_get_Length_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICurve_StartPoint_Proxy( 
    ICurve __RPC_FAR * This,
    /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *sp);


void __RPC_STUB ICurve_StartPoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICurve_EndPoint_Proxy( 
    ICurve __RPC_FAR * This,
    /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *ep);


void __RPC_STUB ICurve_EndPoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE ICurve_get_IsClosed_Proxy( 
    ICurve __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isClosed);


void __RPC_STUB ICurve_get_IsClosed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ICurve_Value_Proxy( 
    ICurve __RPC_FAR * This,
    /* [in] */ double t,
    /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *p);


void __RPC_STUB ICurve_Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ICurve_INTERFACE_DEFINED__ */


#ifndef __ILineString_INTERFACE_DEFINED__
#define __ILineString_INTERFACE_DEFINED__

/* interface ILineString */
/* [uuid][object] */ 


EXTERN_C const IID IID_ILineString;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6A124037-FE38-11d0-BECE-00805F7C4268")
    ILineString : public ICurve
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_NumPoints( 
            /* [retval][out] */ long __RPC_FAR *numPoints) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Point( 
            /* [in] */ long index,
            /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *point) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILineStringVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ILineString __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ILineString __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ILineString __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Dimension )( 
            ILineString __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *dimension);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SpatialReference )( 
            ILineString __RPC_FAR * This,
            /* [retval][out] */ ISpatialReference __RPC_FAR *__RPC_FAR *spatialRef);
        
        /* [propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_SpatialReference )( 
            ILineString __RPC_FAR * This,
            /* [in] */ ISpatialReference __RPC_FAR *spatialRef);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsEmpty )( 
            ILineString __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isEmpty);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEmpty )( 
            ILineString __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsSimple )( 
            ILineString __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isSimple);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Envelope )( 
            ILineString __RPC_FAR * This,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *envelope);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            ILineString __RPC_FAR * This,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *newShape);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Project )( 
            ILineString __RPC_FAR * This,
            /* [in] */ ISpatialReference __RPC_FAR *newSystem,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Extent2D )( 
            ILineString __RPC_FAR * This,
            /* [out] */ double __RPC_FAR *minX,
            /* [out] */ double __RPC_FAR *minY,
            /* [out] */ double __RPC_FAR *maxX,
            /* [out] */ double __RPC_FAR *maxY);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Length )( 
            ILineString __RPC_FAR * This,
            /* [retval][out] */ double __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StartPoint )( 
            ILineString __RPC_FAR * This,
            /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *sp);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EndPoint )( 
            ILineString __RPC_FAR * This,
            /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *ep);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsClosed )( 
            ILineString __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isClosed);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Value )( 
            ILineString __RPC_FAR * This,
            /* [in] */ double t,
            /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *p);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_NumPoints )( 
            ILineString __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *numPoints);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Point )( 
            ILineString __RPC_FAR * This,
            /* [in] */ long index,
            /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *point);
        
        END_INTERFACE
    } ILineStringVtbl;

    interface ILineString
    {
        CONST_VTBL struct ILineStringVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILineString_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ILineString_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ILineString_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ILineString_get_Dimension(This,dimension)	\
    (This)->lpVtbl -> get_Dimension(This,dimension)

#define ILineString_get_SpatialReference(This,spatialRef)	\
    (This)->lpVtbl -> get_SpatialReference(This,spatialRef)

#define ILineString_putref_SpatialReference(This,spatialRef)	\
    (This)->lpVtbl -> putref_SpatialReference(This,spatialRef)

#define ILineString_get_IsEmpty(This,isEmpty)	\
    (This)->lpVtbl -> get_IsEmpty(This,isEmpty)

#define ILineString_SetEmpty(This)	\
    (This)->lpVtbl -> SetEmpty(This)

#define ILineString_get_IsSimple(This,isSimple)	\
    (This)->lpVtbl -> get_IsSimple(This,isSimple)

#define ILineString_Envelope(This,envelope)	\
    (This)->lpVtbl -> Envelope(This,envelope)

#define ILineString_Clone(This,newShape)	\
    (This)->lpVtbl -> Clone(This,newShape)

#define ILineString_Project(This,newSystem,result)	\
    (This)->lpVtbl -> Project(This,newSystem,result)

#define ILineString_Extent2D(This,minX,minY,maxX,maxY)	\
    (This)->lpVtbl -> Extent2D(This,minX,minY,maxX,maxY)


#define ILineString_get_Length(This,value)	\
    (This)->lpVtbl -> get_Length(This,value)

#define ILineString_StartPoint(This,sp)	\
    (This)->lpVtbl -> StartPoint(This,sp)

#define ILineString_EndPoint(This,ep)	\
    (This)->lpVtbl -> EndPoint(This,ep)

#define ILineString_get_IsClosed(This,isClosed)	\
    (This)->lpVtbl -> get_IsClosed(This,isClosed)

#define ILineString_Value(This,t,p)	\
    (This)->lpVtbl -> Value(This,t,p)


#define ILineString_get_NumPoints(This,numPoints)	\
    (This)->lpVtbl -> get_NumPoints(This,numPoints)

#define ILineString_Point(This,index,point)	\
    (This)->lpVtbl -> Point(This,index,point)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE ILineString_get_NumPoints_Proxy( 
    ILineString __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *numPoints);


void __RPC_STUB ILineString_get_NumPoints_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ILineString_Point_Proxy( 
    ILineString __RPC_FAR * This,
    /* [in] */ long index,
    /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *point);


void __RPC_STUB ILineString_Point_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ILineString_INTERFACE_DEFINED__ */


#ifndef __ILinearRing_INTERFACE_DEFINED__
#define __ILinearRing_INTERFACE_DEFINED__

/* interface ILinearRing */
/* [uuid][object] */ 


EXTERN_C const IID IID_ILinearRing;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6A124038-FE38-11d0-BECE-00805F7C4268")
    ILinearRing : public ILineString
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ILinearRingVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ILinearRing __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ILinearRing __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ILinearRing __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Dimension )( 
            ILinearRing __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *dimension);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SpatialReference )( 
            ILinearRing __RPC_FAR * This,
            /* [retval][out] */ ISpatialReference __RPC_FAR *__RPC_FAR *spatialRef);
        
        /* [propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_SpatialReference )( 
            ILinearRing __RPC_FAR * This,
            /* [in] */ ISpatialReference __RPC_FAR *spatialRef);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsEmpty )( 
            ILinearRing __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isEmpty);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEmpty )( 
            ILinearRing __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsSimple )( 
            ILinearRing __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isSimple);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Envelope )( 
            ILinearRing __RPC_FAR * This,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *envelope);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            ILinearRing __RPC_FAR * This,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *newShape);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Project )( 
            ILinearRing __RPC_FAR * This,
            /* [in] */ ISpatialReference __RPC_FAR *newSystem,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Extent2D )( 
            ILinearRing __RPC_FAR * This,
            /* [out] */ double __RPC_FAR *minX,
            /* [out] */ double __RPC_FAR *minY,
            /* [out] */ double __RPC_FAR *maxX,
            /* [out] */ double __RPC_FAR *maxY);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Length )( 
            ILinearRing __RPC_FAR * This,
            /* [retval][out] */ double __RPC_FAR *value);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StartPoint )( 
            ILinearRing __RPC_FAR * This,
            /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *sp);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *EndPoint )( 
            ILinearRing __RPC_FAR * This,
            /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *ep);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsClosed )( 
            ILinearRing __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isClosed);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Value )( 
            ILinearRing __RPC_FAR * This,
            /* [in] */ double t,
            /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *p);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_NumPoints )( 
            ILinearRing __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *numPoints);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Point )( 
            ILinearRing __RPC_FAR * This,
            /* [in] */ long index,
            /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *point);
        
        END_INTERFACE
    } ILinearRingVtbl;

    interface ILinearRing
    {
        CONST_VTBL struct ILinearRingVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILinearRing_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ILinearRing_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ILinearRing_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ILinearRing_get_Dimension(This,dimension)	\
    (This)->lpVtbl -> get_Dimension(This,dimension)

#define ILinearRing_get_SpatialReference(This,spatialRef)	\
    (This)->lpVtbl -> get_SpatialReference(This,spatialRef)

#define ILinearRing_putref_SpatialReference(This,spatialRef)	\
    (This)->lpVtbl -> putref_SpatialReference(This,spatialRef)

#define ILinearRing_get_IsEmpty(This,isEmpty)	\
    (This)->lpVtbl -> get_IsEmpty(This,isEmpty)

#define ILinearRing_SetEmpty(This)	\
    (This)->lpVtbl -> SetEmpty(This)

#define ILinearRing_get_IsSimple(This,isSimple)	\
    (This)->lpVtbl -> get_IsSimple(This,isSimple)

#define ILinearRing_Envelope(This,envelope)	\
    (This)->lpVtbl -> Envelope(This,envelope)

#define ILinearRing_Clone(This,newShape)	\
    (This)->lpVtbl -> Clone(This,newShape)

#define ILinearRing_Project(This,newSystem,result)	\
    (This)->lpVtbl -> Project(This,newSystem,result)

#define ILinearRing_Extent2D(This,minX,minY,maxX,maxY)	\
    (This)->lpVtbl -> Extent2D(This,minX,minY,maxX,maxY)


#define ILinearRing_get_Length(This,value)	\
    (This)->lpVtbl -> get_Length(This,value)

#define ILinearRing_StartPoint(This,sp)	\
    (This)->lpVtbl -> StartPoint(This,sp)

#define ILinearRing_EndPoint(This,ep)	\
    (This)->lpVtbl -> EndPoint(This,ep)

#define ILinearRing_get_IsClosed(This,isClosed)	\
    (This)->lpVtbl -> get_IsClosed(This,isClosed)

#define ILinearRing_Value(This,t,p)	\
    (This)->lpVtbl -> Value(This,t,p)


#define ILinearRing_get_NumPoints(This,numPoints)	\
    (This)->lpVtbl -> get_NumPoints(This,numPoints)

#define ILinearRing_Point(This,index,point)	\
    (This)->lpVtbl -> Point(This,index,point)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ILinearRing_INTERFACE_DEFINED__ */


#ifndef __ISurface_INTERFACE_DEFINED__
#define __ISurface_INTERFACE_DEFINED__

/* interface ISurface */
/* [uuid][object] */ 


EXTERN_C const IID IID_ISurface;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6A124039-FE38-11d0-BECE-00805F7C4268")
    ISurface : public IGeometry
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Area( 
            /* [retval][out] */ double __RPC_FAR *area) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Centroid( 
            /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PointOnSurface( 
            /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *result) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISurfaceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISurface __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISurface __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISurface __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Dimension )( 
            ISurface __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *dimension);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SpatialReference )( 
            ISurface __RPC_FAR * This,
            /* [retval][out] */ ISpatialReference __RPC_FAR *__RPC_FAR *spatialRef);
        
        /* [propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_SpatialReference )( 
            ISurface __RPC_FAR * This,
            /* [in] */ ISpatialReference __RPC_FAR *spatialRef);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsEmpty )( 
            ISurface __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isEmpty);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEmpty )( 
            ISurface __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsSimple )( 
            ISurface __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isSimple);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Envelope )( 
            ISurface __RPC_FAR * This,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *envelope);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            ISurface __RPC_FAR * This,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *newShape);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Project )( 
            ISurface __RPC_FAR * This,
            /* [in] */ ISpatialReference __RPC_FAR *newSystem,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Extent2D )( 
            ISurface __RPC_FAR * This,
            /* [out] */ double __RPC_FAR *minX,
            /* [out] */ double __RPC_FAR *minY,
            /* [out] */ double __RPC_FAR *maxX,
            /* [out] */ double __RPC_FAR *maxY);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Area )( 
            ISurface __RPC_FAR * This,
            /* [retval][out] */ double __RPC_FAR *area);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Centroid )( 
            ISurface __RPC_FAR * This,
            /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PointOnSurface )( 
            ISurface __RPC_FAR * This,
            /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *result);
        
        END_INTERFACE
    } ISurfaceVtbl;

    interface ISurface
    {
        CONST_VTBL struct ISurfaceVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISurface_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISurface_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISurface_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISurface_get_Dimension(This,dimension)	\
    (This)->lpVtbl -> get_Dimension(This,dimension)

#define ISurface_get_SpatialReference(This,spatialRef)	\
    (This)->lpVtbl -> get_SpatialReference(This,spatialRef)

#define ISurface_putref_SpatialReference(This,spatialRef)	\
    (This)->lpVtbl -> putref_SpatialReference(This,spatialRef)

#define ISurface_get_IsEmpty(This,isEmpty)	\
    (This)->lpVtbl -> get_IsEmpty(This,isEmpty)

#define ISurface_SetEmpty(This)	\
    (This)->lpVtbl -> SetEmpty(This)

#define ISurface_get_IsSimple(This,isSimple)	\
    (This)->lpVtbl -> get_IsSimple(This,isSimple)

#define ISurface_Envelope(This,envelope)	\
    (This)->lpVtbl -> Envelope(This,envelope)

#define ISurface_Clone(This,newShape)	\
    (This)->lpVtbl -> Clone(This,newShape)

#define ISurface_Project(This,newSystem,result)	\
    (This)->lpVtbl -> Project(This,newSystem,result)

#define ISurface_Extent2D(This,minX,minY,maxX,maxY)	\
    (This)->lpVtbl -> Extent2D(This,minX,minY,maxX,maxY)


#define ISurface_get_Area(This,area)	\
    (This)->lpVtbl -> get_Area(This,area)

#define ISurface_Centroid(This,result)	\
    (This)->lpVtbl -> Centroid(This,result)

#define ISurface_PointOnSurface(This,result)	\
    (This)->lpVtbl -> PointOnSurface(This,result)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE ISurface_get_Area_Proxy( 
    ISurface __RPC_FAR * This,
    /* [retval][out] */ double __RPC_FAR *area);


void __RPC_STUB ISurface_get_Area_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISurface_Centroid_Proxy( 
    ISurface __RPC_FAR * This,
    /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *result);


void __RPC_STUB ISurface_Centroid_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISurface_PointOnSurface_Proxy( 
    ISurface __RPC_FAR * This,
    /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *result);


void __RPC_STUB ISurface_PointOnSurface_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISurface_INTERFACE_DEFINED__ */


#ifndef __IGeometryCollection_INTERFACE_DEFINED__
#define __IGeometryCollection_INTERFACE_DEFINED__

/* interface IGeometryCollection */
/* [uuid][object] */ 


EXTERN_C const IID IID_IGeometryCollection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6A12403A-FE38-11d0-BECE-00805F7C4268")
    IGeometryCollection : public IGeometry
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_NumGeometries( 
            /* [retval][out] */ long __RPC_FAR *numberOf) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Geometry( 
            /* [in] */ long index,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *geometry) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IGeometryCollectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IGeometryCollection __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IGeometryCollection __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IGeometryCollection __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Dimension )( 
            IGeometryCollection __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *dimension);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SpatialReference )( 
            IGeometryCollection __RPC_FAR * This,
            /* [retval][out] */ ISpatialReference __RPC_FAR *__RPC_FAR *spatialRef);
        
        /* [propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_SpatialReference )( 
            IGeometryCollection __RPC_FAR * This,
            /* [in] */ ISpatialReference __RPC_FAR *spatialRef);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsEmpty )( 
            IGeometryCollection __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isEmpty);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEmpty )( 
            IGeometryCollection __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsSimple )( 
            IGeometryCollection __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isSimple);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Envelope )( 
            IGeometryCollection __RPC_FAR * This,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *envelope);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IGeometryCollection __RPC_FAR * This,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *newShape);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Project )( 
            IGeometryCollection __RPC_FAR * This,
            /* [in] */ ISpatialReference __RPC_FAR *newSystem,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Extent2D )( 
            IGeometryCollection __RPC_FAR * This,
            /* [out] */ double __RPC_FAR *minX,
            /* [out] */ double __RPC_FAR *minY,
            /* [out] */ double __RPC_FAR *maxX,
            /* [out] */ double __RPC_FAR *maxY);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_NumGeometries )( 
            IGeometryCollection __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *numberOf);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Geometry )( 
            IGeometryCollection __RPC_FAR * This,
            /* [in] */ long index,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *geometry);
        
        END_INTERFACE
    } IGeometryCollectionVtbl;

    interface IGeometryCollection
    {
        CONST_VTBL struct IGeometryCollectionVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGeometryCollection_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IGeometryCollection_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IGeometryCollection_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IGeometryCollection_get_Dimension(This,dimension)	\
    (This)->lpVtbl -> get_Dimension(This,dimension)

#define IGeometryCollection_get_SpatialReference(This,spatialRef)	\
    (This)->lpVtbl -> get_SpatialReference(This,spatialRef)

#define IGeometryCollection_putref_SpatialReference(This,spatialRef)	\
    (This)->lpVtbl -> putref_SpatialReference(This,spatialRef)

#define IGeometryCollection_get_IsEmpty(This,isEmpty)	\
    (This)->lpVtbl -> get_IsEmpty(This,isEmpty)

#define IGeometryCollection_SetEmpty(This)	\
    (This)->lpVtbl -> SetEmpty(This)

#define IGeometryCollection_get_IsSimple(This,isSimple)	\
    (This)->lpVtbl -> get_IsSimple(This,isSimple)

#define IGeometryCollection_Envelope(This,envelope)	\
    (This)->lpVtbl -> Envelope(This,envelope)

#define IGeometryCollection_Clone(This,newShape)	\
    (This)->lpVtbl -> Clone(This,newShape)

#define IGeometryCollection_Project(This,newSystem,result)	\
    (This)->lpVtbl -> Project(This,newSystem,result)

#define IGeometryCollection_Extent2D(This,minX,minY,maxX,maxY)	\
    (This)->lpVtbl -> Extent2D(This,minX,minY,maxX,maxY)


#define IGeometryCollection_get_NumGeometries(This,numberOf)	\
    (This)->lpVtbl -> get_NumGeometries(This,numberOf)

#define IGeometryCollection_Geometry(This,index,geometry)	\
    (This)->lpVtbl -> Geometry(This,index,geometry)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IGeometryCollection_get_NumGeometries_Proxy( 
    IGeometryCollection __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *numberOf);


void __RPC_STUB IGeometryCollection_get_NumGeometries_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IGeometryCollection_Geometry_Proxy( 
    IGeometryCollection __RPC_FAR * This,
    /* [in] */ long index,
    /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *geometry);


void __RPC_STUB IGeometryCollection_Geometry_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IGeometryCollection_INTERFACE_DEFINED__ */


#ifndef __IPolygon_INTERFACE_DEFINED__
#define __IPolygon_INTERFACE_DEFINED__

/* interface IPolygon */
/* [uuid][object] */ 


EXTERN_C const IID IID_IPolygon;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6A12403C-FE38-11d0-BECE-00805F7C4268")
    IPolygon : public ISurface
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE ExteriorRing( 
            /* [retval][out] */ ILinearRing __RPC_FAR *__RPC_FAR *exteriorRing) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_NumInteriorRings( 
            /* [retval][out] */ long __RPC_FAR *count) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InteriorRing( 
            /* [in] */ long index,
            /* [retval][out] */ ILinearRing __RPC_FAR *__RPC_FAR *interiorRing) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPolygonVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IPolygon __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IPolygon __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IPolygon __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Dimension )( 
            IPolygon __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *dimension);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SpatialReference )( 
            IPolygon __RPC_FAR * This,
            /* [retval][out] */ ISpatialReference __RPC_FAR *__RPC_FAR *spatialRef);
        
        /* [propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_SpatialReference )( 
            IPolygon __RPC_FAR * This,
            /* [in] */ ISpatialReference __RPC_FAR *spatialRef);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsEmpty )( 
            IPolygon __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isEmpty);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEmpty )( 
            IPolygon __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsSimple )( 
            IPolygon __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isSimple);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Envelope )( 
            IPolygon __RPC_FAR * This,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *envelope);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IPolygon __RPC_FAR * This,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *newShape);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Project )( 
            IPolygon __RPC_FAR * This,
            /* [in] */ ISpatialReference __RPC_FAR *newSystem,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Extent2D )( 
            IPolygon __RPC_FAR * This,
            /* [out] */ double __RPC_FAR *minX,
            /* [out] */ double __RPC_FAR *minY,
            /* [out] */ double __RPC_FAR *maxX,
            /* [out] */ double __RPC_FAR *maxY);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Area )( 
            IPolygon __RPC_FAR * This,
            /* [retval][out] */ double __RPC_FAR *area);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Centroid )( 
            IPolygon __RPC_FAR * This,
            /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PointOnSurface )( 
            IPolygon __RPC_FAR * This,
            /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ExteriorRing )( 
            IPolygon __RPC_FAR * This,
            /* [retval][out] */ ILinearRing __RPC_FAR *__RPC_FAR *exteriorRing);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_NumInteriorRings )( 
            IPolygon __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *count);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InteriorRing )( 
            IPolygon __RPC_FAR * This,
            /* [in] */ long index,
            /* [retval][out] */ ILinearRing __RPC_FAR *__RPC_FAR *interiorRing);
        
        END_INTERFACE
    } IPolygonVtbl;

    interface IPolygon
    {
        CONST_VTBL struct IPolygonVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPolygon_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPolygon_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPolygon_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPolygon_get_Dimension(This,dimension)	\
    (This)->lpVtbl -> get_Dimension(This,dimension)

#define IPolygon_get_SpatialReference(This,spatialRef)	\
    (This)->lpVtbl -> get_SpatialReference(This,spatialRef)

#define IPolygon_putref_SpatialReference(This,spatialRef)	\
    (This)->lpVtbl -> putref_SpatialReference(This,spatialRef)

#define IPolygon_get_IsEmpty(This,isEmpty)	\
    (This)->lpVtbl -> get_IsEmpty(This,isEmpty)

#define IPolygon_SetEmpty(This)	\
    (This)->lpVtbl -> SetEmpty(This)

#define IPolygon_get_IsSimple(This,isSimple)	\
    (This)->lpVtbl -> get_IsSimple(This,isSimple)

#define IPolygon_Envelope(This,envelope)	\
    (This)->lpVtbl -> Envelope(This,envelope)

#define IPolygon_Clone(This,newShape)	\
    (This)->lpVtbl -> Clone(This,newShape)

#define IPolygon_Project(This,newSystem,result)	\
    (This)->lpVtbl -> Project(This,newSystem,result)

#define IPolygon_Extent2D(This,minX,minY,maxX,maxY)	\
    (This)->lpVtbl -> Extent2D(This,minX,minY,maxX,maxY)


#define IPolygon_get_Area(This,area)	\
    (This)->lpVtbl -> get_Area(This,area)

#define IPolygon_Centroid(This,result)	\
    (This)->lpVtbl -> Centroid(This,result)

#define IPolygon_PointOnSurface(This,result)	\
    (This)->lpVtbl -> PointOnSurface(This,result)


#define IPolygon_ExteriorRing(This,exteriorRing)	\
    (This)->lpVtbl -> ExteriorRing(This,exteriorRing)

#define IPolygon_get_NumInteriorRings(This,count)	\
    (This)->lpVtbl -> get_NumInteriorRings(This,count)

#define IPolygon_InteriorRing(This,index,interiorRing)	\
    (This)->lpVtbl -> InteriorRing(This,index,interiorRing)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IPolygon_ExteriorRing_Proxy( 
    IPolygon __RPC_FAR * This,
    /* [retval][out] */ ILinearRing __RPC_FAR *__RPC_FAR *exteriorRing);


void __RPC_STUB IPolygon_ExteriorRing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IPolygon_get_NumInteriorRings_Proxy( 
    IPolygon __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *count);


void __RPC_STUB IPolygon_get_NumInteriorRings_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IPolygon_InteriorRing_Proxy( 
    IPolygon __RPC_FAR * This,
    /* [in] */ long index,
    /* [retval][out] */ ILinearRing __RPC_FAR *__RPC_FAR *interiorRing);


void __RPC_STUB IPolygon_InteriorRing_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPolygon_INTERFACE_DEFINED__ */


#ifndef __IMultiCurve_INTERFACE_DEFINED__
#define __IMultiCurve_INTERFACE_DEFINED__

/* interface IMultiCurve */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMultiCurve;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6A12403D-FE38-11d0-BECE-00805F7C4268")
    IMultiCurve : public IGeometryCollection
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Length( 
            /* [retval][out] */ double __RPC_FAR *length) = 0;
        
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_IsClosed( 
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isClosed) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMultiCurveVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMultiCurve __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMultiCurve __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMultiCurve __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Dimension )( 
            IMultiCurve __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *dimension);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SpatialReference )( 
            IMultiCurve __RPC_FAR * This,
            /* [retval][out] */ ISpatialReference __RPC_FAR *__RPC_FAR *spatialRef);
        
        /* [propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_SpatialReference )( 
            IMultiCurve __RPC_FAR * This,
            /* [in] */ ISpatialReference __RPC_FAR *spatialRef);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsEmpty )( 
            IMultiCurve __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isEmpty);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEmpty )( 
            IMultiCurve __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsSimple )( 
            IMultiCurve __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isSimple);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Envelope )( 
            IMultiCurve __RPC_FAR * This,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *envelope);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IMultiCurve __RPC_FAR * This,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *newShape);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Project )( 
            IMultiCurve __RPC_FAR * This,
            /* [in] */ ISpatialReference __RPC_FAR *newSystem,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Extent2D )( 
            IMultiCurve __RPC_FAR * This,
            /* [out] */ double __RPC_FAR *minX,
            /* [out] */ double __RPC_FAR *minY,
            /* [out] */ double __RPC_FAR *maxX,
            /* [out] */ double __RPC_FAR *maxY);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_NumGeometries )( 
            IMultiCurve __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *numberOf);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Geometry )( 
            IMultiCurve __RPC_FAR * This,
            /* [in] */ long index,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *geometry);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Length )( 
            IMultiCurve __RPC_FAR * This,
            /* [retval][out] */ double __RPC_FAR *length);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsClosed )( 
            IMultiCurve __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isClosed);
        
        END_INTERFACE
    } IMultiCurveVtbl;

    interface IMultiCurve
    {
        CONST_VTBL struct IMultiCurveVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMultiCurve_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMultiCurve_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMultiCurve_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMultiCurve_get_Dimension(This,dimension)	\
    (This)->lpVtbl -> get_Dimension(This,dimension)

#define IMultiCurve_get_SpatialReference(This,spatialRef)	\
    (This)->lpVtbl -> get_SpatialReference(This,spatialRef)

#define IMultiCurve_putref_SpatialReference(This,spatialRef)	\
    (This)->lpVtbl -> putref_SpatialReference(This,spatialRef)

#define IMultiCurve_get_IsEmpty(This,isEmpty)	\
    (This)->lpVtbl -> get_IsEmpty(This,isEmpty)

#define IMultiCurve_SetEmpty(This)	\
    (This)->lpVtbl -> SetEmpty(This)

#define IMultiCurve_get_IsSimple(This,isSimple)	\
    (This)->lpVtbl -> get_IsSimple(This,isSimple)

#define IMultiCurve_Envelope(This,envelope)	\
    (This)->lpVtbl -> Envelope(This,envelope)

#define IMultiCurve_Clone(This,newShape)	\
    (This)->lpVtbl -> Clone(This,newShape)

#define IMultiCurve_Project(This,newSystem,result)	\
    (This)->lpVtbl -> Project(This,newSystem,result)

#define IMultiCurve_Extent2D(This,minX,minY,maxX,maxY)	\
    (This)->lpVtbl -> Extent2D(This,minX,minY,maxX,maxY)


#define IMultiCurve_get_NumGeometries(This,numberOf)	\
    (This)->lpVtbl -> get_NumGeometries(This,numberOf)

#define IMultiCurve_Geometry(This,index,geometry)	\
    (This)->lpVtbl -> Geometry(This,index,geometry)


#define IMultiCurve_get_Length(This,length)	\
    (This)->lpVtbl -> get_Length(This,length)

#define IMultiCurve_get_IsClosed(This,isClosed)	\
    (This)->lpVtbl -> get_IsClosed(This,isClosed)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IMultiCurve_get_Length_Proxy( 
    IMultiCurve __RPC_FAR * This,
    /* [retval][out] */ double __RPC_FAR *length);


void __RPC_STUB IMultiCurve_get_Length_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [propget] */ HRESULT STDMETHODCALLTYPE IMultiCurve_get_IsClosed_Proxy( 
    IMultiCurve __RPC_FAR * This,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isClosed);


void __RPC_STUB IMultiCurve_get_IsClosed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMultiCurve_INTERFACE_DEFINED__ */


#ifndef __IMultiSurface_INTERFACE_DEFINED__
#define __IMultiSurface_INTERFACE_DEFINED__

/* interface IMultiSurface */
/* [uuid][object] */ 


EXTERN_C const IID IID_IMultiSurface;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6A12403F-FE38-11d0-BECE-00805F7C4268")
    IMultiSurface : public IGeometryCollection
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Area( 
            /* [retval][out] */ double __RPC_FAR *area) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Centroid( 
            /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PointOnSurface( 
            /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *result) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMultiSurfaceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IMultiSurface __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IMultiSurface __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IMultiSurface __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Dimension )( 
            IMultiSurface __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *dimension);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SpatialReference )( 
            IMultiSurface __RPC_FAR * This,
            /* [retval][out] */ ISpatialReference __RPC_FAR *__RPC_FAR *spatialRef);
        
        /* [propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_SpatialReference )( 
            IMultiSurface __RPC_FAR * This,
            /* [in] */ ISpatialReference __RPC_FAR *spatialRef);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsEmpty )( 
            IMultiSurface __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isEmpty);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetEmpty )( 
            IMultiSurface __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_IsSimple )( 
            IMultiSurface __RPC_FAR * This,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isSimple);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Envelope )( 
            IMultiSurface __RPC_FAR * This,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *envelope);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Clone )( 
            IMultiSurface __RPC_FAR * This,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *newShape);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Project )( 
            IMultiSurface __RPC_FAR * This,
            /* [in] */ ISpatialReference __RPC_FAR *newSystem,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Extent2D )( 
            IMultiSurface __RPC_FAR * This,
            /* [out] */ double __RPC_FAR *minX,
            /* [out] */ double __RPC_FAR *minY,
            /* [out] */ double __RPC_FAR *maxX,
            /* [out] */ double __RPC_FAR *maxY);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_NumGeometries )( 
            IMultiSurface __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *numberOf);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Geometry )( 
            IMultiSurface __RPC_FAR * This,
            /* [in] */ long index,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *geometry);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Area )( 
            IMultiSurface __RPC_FAR * This,
            /* [retval][out] */ double __RPC_FAR *area);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Centroid )( 
            IMultiSurface __RPC_FAR * This,
            /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *PointOnSurface )( 
            IMultiSurface __RPC_FAR * This,
            /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *result);
        
        END_INTERFACE
    } IMultiSurfaceVtbl;

    interface IMultiSurface
    {
        CONST_VTBL struct IMultiSurfaceVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMultiSurface_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMultiSurface_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMultiSurface_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMultiSurface_get_Dimension(This,dimension)	\
    (This)->lpVtbl -> get_Dimension(This,dimension)

#define IMultiSurface_get_SpatialReference(This,spatialRef)	\
    (This)->lpVtbl -> get_SpatialReference(This,spatialRef)

#define IMultiSurface_putref_SpatialReference(This,spatialRef)	\
    (This)->lpVtbl -> putref_SpatialReference(This,spatialRef)

#define IMultiSurface_get_IsEmpty(This,isEmpty)	\
    (This)->lpVtbl -> get_IsEmpty(This,isEmpty)

#define IMultiSurface_SetEmpty(This)	\
    (This)->lpVtbl -> SetEmpty(This)

#define IMultiSurface_get_IsSimple(This,isSimple)	\
    (This)->lpVtbl -> get_IsSimple(This,isSimple)

#define IMultiSurface_Envelope(This,envelope)	\
    (This)->lpVtbl -> Envelope(This,envelope)

#define IMultiSurface_Clone(This,newShape)	\
    (This)->lpVtbl -> Clone(This,newShape)

#define IMultiSurface_Project(This,newSystem,result)	\
    (This)->lpVtbl -> Project(This,newSystem,result)

#define IMultiSurface_Extent2D(This,minX,minY,maxX,maxY)	\
    (This)->lpVtbl -> Extent2D(This,minX,minY,maxX,maxY)


#define IMultiSurface_get_NumGeometries(This,numberOf)	\
    (This)->lpVtbl -> get_NumGeometries(This,numberOf)

#define IMultiSurface_Geometry(This,index,geometry)	\
    (This)->lpVtbl -> Geometry(This,index,geometry)


#define IMultiSurface_get_Area(This,area)	\
    (This)->lpVtbl -> get_Area(This,area)

#define IMultiSurface_Centroid(This,result)	\
    (This)->lpVtbl -> Centroid(This,result)

#define IMultiSurface_PointOnSurface(This,result)	\
    (This)->lpVtbl -> PointOnSurface(This,result)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IMultiSurface_get_Area_Proxy( 
    IMultiSurface __RPC_FAR * This,
    /* [retval][out] */ double __RPC_FAR *area);


void __RPC_STUB IMultiSurface_get_Area_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMultiSurface_Centroid_Proxy( 
    IMultiSurface __RPC_FAR * This,
    /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *result);


void __RPC_STUB IMultiSurface_Centroid_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMultiSurface_PointOnSurface_Proxy( 
    IMultiSurface __RPC_FAR * This,
    /* [retval][out] */ IPoint __RPC_FAR *__RPC_FAR *result);


void __RPC_STUB IMultiSurface_PointOnSurface_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMultiSurface_INTERFACE_DEFINED__ */


#ifndef __ISpatialRelation_INTERFACE_DEFINED__
#define __ISpatialRelation_INTERFACE_DEFINED__

/* interface ISpatialRelation */
/* [uuid][object] */ 


EXTERN_C const IID IID_ISpatialRelation;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6A124040-FE38-11d0-BECE-00805F7C4268")
    ISpatialRelation : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Equals( 
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *equals) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Touches( 
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *touches) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Contains( 
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *contains) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Within( 
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *within) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Disjoint( 
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *disjoint) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Crosses( 
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *crosses) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Overlaps( 
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *overlaps) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Intersects( 
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *overlaps) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISpatialRelationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISpatialRelation __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISpatialRelation __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISpatialRelation __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Equals )( 
            ISpatialRelation __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *equals);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Touches )( 
            ISpatialRelation __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *touches);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Contains )( 
            ISpatialRelation __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *contains);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Within )( 
            ISpatialRelation __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *within);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Disjoint )( 
            ISpatialRelation __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *disjoint);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Crosses )( 
            ISpatialRelation __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *crosses);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Overlaps )( 
            ISpatialRelation __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *overlaps);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Intersects )( 
            ISpatialRelation __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *overlaps);
        
        END_INTERFACE
    } ISpatialRelationVtbl;

    interface ISpatialRelation
    {
        CONST_VTBL struct ISpatialRelationVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISpatialRelation_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISpatialRelation_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISpatialRelation_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISpatialRelation_Equals(This,other,equals)	\
    (This)->lpVtbl -> Equals(This,other,equals)

#define ISpatialRelation_Touches(This,other,touches)	\
    (This)->lpVtbl -> Touches(This,other,touches)

#define ISpatialRelation_Contains(This,other,contains)	\
    (This)->lpVtbl -> Contains(This,other,contains)

#define ISpatialRelation_Within(This,other,within)	\
    (This)->lpVtbl -> Within(This,other,within)

#define ISpatialRelation_Disjoint(This,other,disjoint)	\
    (This)->lpVtbl -> Disjoint(This,other,disjoint)

#define ISpatialRelation_Crosses(This,other,crosses)	\
    (This)->lpVtbl -> Crosses(This,other,crosses)

#define ISpatialRelation_Overlaps(This,other,overlaps)	\
    (This)->lpVtbl -> Overlaps(This,other,overlaps)

#define ISpatialRelation_Intersects(This,other,overlaps)	\
    (This)->lpVtbl -> Intersects(This,other,overlaps)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ISpatialRelation_Equals_Proxy( 
    ISpatialRelation __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *other,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *equals);


void __RPC_STUB ISpatialRelation_Equals_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpatialRelation_Touches_Proxy( 
    ISpatialRelation __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *other,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *touches);


void __RPC_STUB ISpatialRelation_Touches_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpatialRelation_Contains_Proxy( 
    ISpatialRelation __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *other,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *contains);


void __RPC_STUB ISpatialRelation_Contains_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpatialRelation_Within_Proxy( 
    ISpatialRelation __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *other,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *within);


void __RPC_STUB ISpatialRelation_Within_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpatialRelation_Disjoint_Proxy( 
    ISpatialRelation __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *other,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *disjoint);


void __RPC_STUB ISpatialRelation_Disjoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpatialRelation_Crosses_Proxy( 
    ISpatialRelation __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *other,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *crosses);


void __RPC_STUB ISpatialRelation_Crosses_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpatialRelation_Overlaps_Proxy( 
    ISpatialRelation __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *other,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *overlaps);


void __RPC_STUB ISpatialRelation_Overlaps_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpatialRelation_Intersects_Proxy( 
    ISpatialRelation __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *other,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *overlaps);


void __RPC_STUB ISpatialRelation_Intersects_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISpatialRelation_INTERFACE_DEFINED__ */


#ifndef __ISpatialRelation2_INTERFACE_DEFINED__
#define __ISpatialRelation2_INTERFACE_DEFINED__

/* interface ISpatialRelation2 */
/* [uuid][object] */ 


EXTERN_C const IID IID_ISpatialRelation2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6A124041-FE38-11d0-BECE-00805F7C4268")
    ISpatialRelation2 : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Relate( 
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [in] */ BSTR patternMatrix,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isRelated) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISpatialRelation2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISpatialRelation2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISpatialRelation2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISpatialRelation2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Relate )( 
            ISpatialRelation2 __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [in] */ BSTR patternMatrix,
            /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isRelated);
        
        END_INTERFACE
    } ISpatialRelation2Vtbl;

    interface ISpatialRelation2
    {
        CONST_VTBL struct ISpatialRelation2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISpatialRelation2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISpatialRelation2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISpatialRelation2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISpatialRelation2_Relate(This,other,patternMatrix,isRelated)	\
    (This)->lpVtbl -> Relate(This,other,patternMatrix,isRelated)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ISpatialRelation2_Relate_Proxy( 
    ISpatialRelation2 __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *other,
    /* [in] */ BSTR patternMatrix,
    /* [retval][out] */ VARIANT_BOOL __RPC_FAR *isRelated);


void __RPC_STUB ISpatialRelation2_Relate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISpatialRelation2_INTERFACE_DEFINED__ */


#ifndef __ISpatialOperator_INTERFACE_DEFINED__
#define __ISpatialOperator_INTERFACE_DEFINED__

/* interface ISpatialOperator */
/* [uuid][object] */ 


EXTERN_C const IID IID_ISpatialOperator;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6A124042-FE38-11d0-BECE-00805F7C4268")
    ISpatialOperator : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Distance( 
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [retval][out] */ double __RPC_FAR *distance) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Boundary( 
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *boundary) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Buffer( 
            /* [in] */ double distance,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ConvexHull( 
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Intersection( 
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Union( 
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Difference( 
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SymmetricDifference( 
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISpatialOperatorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISpatialOperator __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISpatialOperator __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISpatialOperator __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Distance )( 
            ISpatialOperator __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [retval][out] */ double __RPC_FAR *distance);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Boundary )( 
            ISpatialOperator __RPC_FAR * This,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *boundary);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Buffer )( 
            ISpatialOperator __RPC_FAR * This,
            /* [in] */ double distance,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ConvexHull )( 
            ISpatialOperator __RPC_FAR * This,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Intersection )( 
            ISpatialOperator __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Union )( 
            ISpatialOperator __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Difference )( 
            ISpatialOperator __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SymmetricDifference )( 
            ISpatialOperator __RPC_FAR * This,
            /* [in] */ IGeometry __RPC_FAR *other,
            /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result);
        
        END_INTERFACE
    } ISpatialOperatorVtbl;

    interface ISpatialOperator
    {
        CONST_VTBL struct ISpatialOperatorVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISpatialOperator_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISpatialOperator_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISpatialOperator_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISpatialOperator_Distance(This,other,distance)	\
    (This)->lpVtbl -> Distance(This,other,distance)

#define ISpatialOperator_Boundary(This,boundary)	\
    (This)->lpVtbl -> Boundary(This,boundary)

#define ISpatialOperator_Buffer(This,distance,result)	\
    (This)->lpVtbl -> Buffer(This,distance,result)

#define ISpatialOperator_ConvexHull(This,result)	\
    (This)->lpVtbl -> ConvexHull(This,result)

#define ISpatialOperator_Intersection(This,other,result)	\
    (This)->lpVtbl -> Intersection(This,other,result)

#define ISpatialOperator_Union(This,other,result)	\
    (This)->lpVtbl -> Union(This,other,result)

#define ISpatialOperator_Difference(This,other,result)	\
    (This)->lpVtbl -> Difference(This,other,result)

#define ISpatialOperator_SymmetricDifference(This,other,result)	\
    (This)->lpVtbl -> SymmetricDifference(This,other,result)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ISpatialOperator_Distance_Proxy( 
    ISpatialOperator __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *other,
    /* [retval][out] */ double __RPC_FAR *distance);


void __RPC_STUB ISpatialOperator_Distance_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpatialOperator_Boundary_Proxy( 
    ISpatialOperator __RPC_FAR * This,
    /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *boundary);


void __RPC_STUB ISpatialOperator_Boundary_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpatialOperator_Buffer_Proxy( 
    ISpatialOperator __RPC_FAR * This,
    /* [in] */ double distance,
    /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result);


void __RPC_STUB ISpatialOperator_Buffer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpatialOperator_ConvexHull_Proxy( 
    ISpatialOperator __RPC_FAR * This,
    /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result);


void __RPC_STUB ISpatialOperator_ConvexHull_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpatialOperator_Intersection_Proxy( 
    ISpatialOperator __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *other,
    /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result);


void __RPC_STUB ISpatialOperator_Intersection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpatialOperator_Union_Proxy( 
    ISpatialOperator __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *other,
    /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result);


void __RPC_STUB ISpatialOperator_Union_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpatialOperator_Difference_Proxy( 
    ISpatialOperator __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *other,
    /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result);


void __RPC_STUB ISpatialOperator_Difference_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpatialOperator_SymmetricDifference_Proxy( 
    ISpatialOperator __RPC_FAR * This,
    /* [in] */ IGeometry __RPC_FAR *other,
    /* [retval][out] */ IGeometry __RPC_FAR *__RPC_FAR *result);


void __RPC_STUB ISpatialOperator_SymmetricDifference_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISpatialOperator_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
