/******************************************************************************
 *
 * Component: OGDI Core C API
 * Purpose: Include file for all the OGDI (included by ecs.h).
 *          Contains most macros, and function prototypes, and structures
 *          that aren't transmitted over RPC (the rest are in ecs.h).
 * 
 ******************************************************************************
 * Copyright (C) 1995 Logiciels et Applications Scientifiques (L.A.S.) Inc
 * Permission to use, copy, modify and distribute this software and
 * its documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies, that
 * both the copyright notice and this permission notice appear in
 * supporting documentation, and that the name of L.A.S. Inc not be used 
 * in advertising or publicity pertaining to distribution of the software 
 * without specific, written prior permission. L.A.S. Inc. makes no
 * representations about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 ******************************************************************************
 *
 * $Log: ecs_util.h,v $
 * Revision 1.19  2005/03/01 00:49:27  warmerda
 * Updated to 3.1.5.
 *
 * Revision 1.18  2004/10/26 20:45:52  warmerda
 * Uupdated to 3.1.4.
 *
 * Revision 1.17  2004/10/04 15:53:12  warmerda
 * Updated to version 3.1.3.
 *
 * Revision 1.16  2003/12/15 18:11:21  warmerda
 * Updated version to 3.1.2.
 *
 * Revision 1.15  2003/01/29 15:46:45  warmerda
 * fixed spelling of length
 *
 * Revision 1.14  2002/12/18 14:31:47  warmerda
 * provide more precision for PI and use M_PI if available
 *
 * Revision 1.13  2002/03/28 16:09:07  warmerda
 * updated version/release info
 *
 * Revision 1.12  2001/09/28 18:56:20  warmerda
 * added OGDI_VERSION and OGDI_RELEASEDATE
 *
 * Revision 1.11  2001/07/18 03:55:09  warmerda
 * Fixed case where DISABLE_CVSID is defined.
 *
 * Revision 1.10  2001/06/22 16:28:24  warmerda
 * fixed typo in ECS_SETGEOMIMAGEVALUE
 *
 * Revision 1.9  2001/04/12 19:25:39  warmerda
 * added RGB<->Pixel functions
 *
 * Revision 1.8  2001/04/12 18:15:25  warmerda
 * added/finished capabilities support
 *
 * Revision 1.7  2001/04/12 05:29:47  warmerda
 * added ecs_LayerCapabilities structure, and additions to ecs_Client
 *
 * Revision 1.6  2001/04/09 15:04:35  warmerda
 * applied new source headers
 *
 */

#ifndef ECS_UTIL
#define ECS_UTIL 1

#define OGDI_VERSION     315
#define OGDI_RELEASEDATE 20050228

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rpc/rpc.h>
#include <math.h>
#include <memory.h>
#include <ctype.h>

#ifdef _WINDOWS
#include <windows.h>
#include <winsock.h>
#else
#include <dlfcn.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>

#define PVALUE PVALUE_PROJ4
#include "projects.h"

/* Ensure we are compatible with PROJ.4.4.x and PROJ.4.3.x */
#ifndef USE_PROJUV
#  define projUV UV
#endif

#include "ecs.h"

#ifdef _SCO
#include <sys/fs/nfs/time.h>
#endif

#ifdef _WINDOWS
#include <signal.h>
#define alarm(a) printf("")
#endif


#ifndef __INT32DEF__
#define __INT32DEF__
#ifdef _ALPHA
typedef int int32;
typedef unsigned int uint32;
#else
typedef long int32;
typedef unsigned long uint32;
#endif
#endif

/***********************************************************************/

/* Global definitions */

#undef _ANSI_ARGS_
#undef CONST
#if ((defined(__STDC__) || defined(SABER)) && !defined(NO_PROTOTYPE)) || defined(__cplusplus)
#   define _USING_PROTOTYPES_ 1
#   define _ANSI_ARGS_(x)	x
#   define CONST const
#   ifdef __cplusplus
#       define VARARGS(first) (first, ...)
#   else
#       define VARARGS(first) ()
#   endif
#else
#   define _ANSI_ARGS_(x)	()
#   define CONST
#endif

#ifdef __cplusplus
#   define EXTERN extern "C"
#else
#   define EXTERN extern
#endif

/*
 * Macro to use instead of "void" for arguments that must have
 * type "void *" in ANSI C;  maps them to type "char *" in
 * non-ANSI systems.
 */

#ifndef VOID
#   ifdef __STDC__
#       define VOID void
#   else
#       define VOID char
#   endif
#endif

/*
 * Miscellaneous declarations (to allow Tcl to be used stand-alone,
 * without the rest of Sprite).
 */

#ifndef NULL
#define NULL 0
#endif

#define ECSTOLERANCE 0.05

#define PROJ_UNKNOWN "unknown"
#define PROJ_LONGLAT "+proj=longlat"

#define ECSGETJ(s,i1,j1) (int)(((s->rasterconversion.coef.coef_val[0]*j1 + s->rasterconversion.coef.coef_val[1]*i1 + s->rasterconversion.coef.coef_val[6]) / (s->rasterconversion.coef.coef_val[4]*j1 + s->rasterconversion.coef.coef_val[5]*i1 + 1)) + 0.5)
#define ECSGETI(s,i1,j1) (int)(((s->rasterconversion.coef.coef_val[2]*j1 + s->rasterconversion.coef.coef_val[3]*i1 + s->rasterconversion.coef.coef_val[7]) / (s->rasterconversion.coef.coef_val[4]*j1 + s->rasterconversion.coef.coef_val[5]*i1 + 1)) + 0.5)

#ifndef PI
#  ifdef M_PI
#    define PI M_PI
#  else
#    define PI 3.1415926535897932384626433832795029
#  endif
#endif

/***********************************************************************
 * Define ECS_CVSID() macro.  It can be disabled during a build by
 * defining DISABLE_CVSID in the compiler options.
 *
 * The cvsid_aw() function is just there to prevent reports of ecs_cvsid()
 * being unused.
 */

#ifndef DISABLE_CVSID
#  define ECS_CVSID(string)	static char ecs_cvsid[] = string; \
static char *cvsid_aw() { return( cvsid_aw() ? ((char *) NULL) : ecs_cvsid ); }
#else
#  define ECS_CVSID(string)
#endif

/***********************************************************************/

/* defines for defaults file */

#define MAX_DEF_LINE_LENGTH 1024
#define DEFAULTS_INDEX_FILE "ogdidef.idx"


/***********************************************************************/

/* server.c declarations */

#define OGDIID -1
#define OGDICAT -2
#define OGDIDESC -3
#define OGDILAYERINC 32

typedef ecs_Result *dynfunc();
typedef int attrfunc();

/*********************************************************************

  STRUCTURE_INFORMATION
  
  NAME
     ecs_AttributeLink

  DESCRIPTION
     Description of a possible link between a geographic layer and a attribute driver
  END_DESCRIPTION

  ATTRIBUTES
     char *url: URL of the geographic layer
     char *layer: Geographic layer selection string
     ecs_Family family: Family of the geographic objects selected by this layer
     char *DriverType: The kind of attribute driver to be link
     char *InformationSource: Information source of the link
     char *UserDescription: User description
     char *AutorizationDescription: Autorization description
     char *SelectionRequest: The request to construct the link.
  END_ATTRIBUTES

  END_STRUCTURE_INFORMATION

  ********************************************************************/

typedef struct {
  char *url;
  char *layer;
  ecs_Family family;
  char *DriverType;
  char *InformationSource;
  char *UserDescription;
  char *AutorizationDescription;
  char *SelectionRequest;
} ecs_AttributeLink;


/*********************************************************************

  STRUCTURE_INFORMATION
  
  NAME
     ecs_Layer

  DESCRIPTION
     Contain all the layer selection information.
  END_DESCRIPTION

  ATTRIBUTES
     ecs_LayerSelection sel: Layer Selection Information
     int index: For GetNextObject, the current object extracted
     int nbfeature: The number of features in a layer. Optionnal.
     void *priv: The private geographic information of the geographic driver for a geographic layer.
     ecs_AttributeLink *AttributeDriverLinkPtr: The item in the list of possible odbc link that's in use. If null, no ODBC link
     void *attribute_priv: The private geographic information of the geographic driver.
     attrfunc *InitializeDBLinkFuncPtr: The pointer to the function dyn_InitializeDBLink in the attribute driver
     attrfunc *DeinitializeDBLinkFuncPtr: The pointer to the function dyn_DeinitializeDBLink in the attribute driver
     attrfunc *GetColumnsInfoFuncPtr: The pointer to the function dyn_GetColumnsInfo in the attribute driver
     attrfunc *SelectAttributes: The pointer to the function dyn_SelectAttributes in the attribute driver
     attrfunc *IsSelectedFuncPtr: The pointer to the function dyn_IsSelected in the attribute driver
     attrfunc *GetSelectedAttributesFuncPtr: The pointer to the function GetSelectedAttributes in the attribute driver
     void *AttributeDriverHandle: The dynamic library handle for the attribute driver
     int SelectionAttributeListQty: The quantity of elements in the SelectionAttributeList
     int *SelectionAttributeList: The list of positions of the objects attributes in use in the IsValueIsSelected of the attribute driver
     char *AttrRequest: The attribute request with the "?code?" replaced with "?"
  END_ATTRIBUTES

  END_STRUCTURE_INFORMATION

  ********************************************************************/

typedef struct {
  ecs_LayerSelection sel;
  int index;
  int nbfeature;
  void *priv;
  ecs_AttributeLink *AttributeDriverLinkPtr;
  void *attribute_priv;
  attrfunc *InitializeDBLinkFuncPtr;
  attrfunc *DeinitializeDBLinkFuncPtr;
  attrfunc *GetColumnsInfoFuncPtr;
  attrfunc *SelectAttributesFuncPtr;
  attrfunc *IsSelectedFuncPtr;
  attrfunc *GetSelectedAttributesFuncPtr;
  void *AttributeDriverHandle;
  int SelectionAttributeListQty;
  int *SelectionAttributeList;
  char *AttrRequest;
} ecs_Layer;


/*********************************************************************

  STRUCTURE_INFORMATION
  
  NAME
     ecs_Server

  DESCRIPTION
     Contain all the geographic driver information
  END_DESCRIPTION

  ATTRIBUTES
     void *priv: The private geographic information of the geographic driver
     ecs_Layer *layer: The table of the layer in use in the driver
     int nblayer: Quantity of layer in layer.
     int layer_tablesize: The table size of the layer table
     int currentLayer: The current layer in use in the driver
     void *handle: The dynamic library handle of the geographic driver
     ecs_Region currentRegion: The current region of the geographic driver
     ecs_Region globalRegion: The global region of the geographic driver
     char *projection: The projection string of the contain of the geographic driver
     char *hostname: The hostname extracted from the URL
     char *server_type: The server type extracted from the URL
     char *pathname: The path name extracted from the URL
     int isRemote: Indicate if the server is a remote server or not
     ecs_RasterConversion rasterconversion: Used to convert rasters in the driver
     ecs_Result result: Returned structure to the client
     int AttributeListQty: Quantity of links in AttributeList
     ecs_AttributeList *AttributeList: The list of attribute links available
     dynfunc *createserver: Pointer to the function dyn_CreateServer
     dynfunc *destroyserver: Pointer to the function dyn_DestroyServer
     dynfunc *selectlayer: Pointer to the function dyn_SelectLayer
     dynfunc *releaselayer: Pointer to the function dyn_ReleaseLayer
     dynfunc *closelayer: Pointer to the function dyn_ReleaseLayer
     dynfunc *selectregion: Pointer to the function dyn_SelectRegion
     dynfunc *getdictionary: Pointer to the function dyn_GetDictionary
     dynfunc *getattrformat: Pointer to the function dyn_GetAttributesFormat
     dynfunc *getnextobject: Pointer to the function dyn_GetNextObject
     dynfunc *getrasterinfo: Pointer to the function dyn_GetRasterInfo
     dynfunc *getobject: Pointer to the function dyn_GetObject
     dynfunc *getobjectid: Pointer to the function dyn_GetObjectIdFromCoord
     dynfunc *updatedictionary: Pointer to the function dyn_UpdateDictionary
     dynfunc *getserverprojection: Pointer to the function dyn_GetServerProjection
     dynfunc *getglobalbound: Pointer to the function dyn_GetGlobalBound
     dynfunc *setserverlanguage: Pointer to the function dyn_SetServerLanguage
     dynfunc *setserverprojection: Pointer to the function dyn_SetServerProjection
     dynfunc *setrasterconversion: Pointer to the function dyn_SetRasterConversion
  END_ATTRIBUTES

  END_STRUCTURE_INFORMATION

  ********************************************************************/

typedef struct {
  /* Specific information for dynamic part of the server */

  void *priv;

  ecs_Layer *layer;
  int nblayer;
  int currentLayer;
  int layer_tablesize;

  /* dynamic library handle */
  
  void *handle;

  /* Regions of the server */

  ecs_Region currentRegion;
  ecs_Region globalRegion;

  /* Server projection */

  char *projection;

  /* Extracted information from URL */

  char *url;
  char *hostname;
  char *server_type;
  char *pathname;

  /* Indicate if the server is a remote server or not */

  int isRemote;

  /* Indicate if the client is local or not */

  int localClient;

  /* RasterConversion structure */
  
  ecs_RasterConversion rasterconversion;

  /* Structure returned to client */

  ecs_Result result;
  
  /* Compression parameters */

  ecs_Compression compression;

  /* pointers to functions */

  dynfunc *createserver;
  dynfunc *destroyserver;
  dynfunc *selectlayer;
  dynfunc *releaselayer;
  dynfunc *closelayer;
  dynfunc *selectregion;
  dynfunc *getdictionary;
  dynfunc *getattrformat;
  dynfunc *getnextobject;
  dynfunc *getrasterinfo;
  dynfunc *getobject;
  dynfunc *getobjectid;
  dynfunc *updatedictionary;
  dynfunc *getserverprojection;
  dynfunc *getglobalbound;
  dynfunc *setserverlanguage;
  dynfunc *setserverprojection;
  dynfunc *setrasterconversion;
  dynfunc *setcompression;

  int AttributeListQty;
  ecs_AttributeLink *AttributeList;
} ecs_Server;


ecs_Result *svr_CreateServer _ANSI_ARGS_((ecs_Server *s, char *Request, int isLocal));
ecs_Result *svr_DestroyServer _ANSI_ARGS_((ecs_Server *s));
ecs_Result *svr_SelectLayer _ANSI_ARGS_((ecs_Server *s, ecs_LayerSelection *ls));
ecs_Result *svr_ReleaseLayer _ANSI_ARGS_((ecs_Server *s, ecs_LayerSelection *ls));
void svr_BroadCloseLayers _ANSI_ARGS_((ecs_Server *s));
void svr_CloseLayer _ANSI_ARGS_((ecs_Server *s));
ecs_Result *svr_SelectRegion _ANSI_ARGS_((ecs_Server *s, ecs_Region *gr));
ecs_Result *svr_GetDictionary _ANSI_ARGS_((ecs_Server *s));
ecs_Result *svr_GetAttributesFormat _ANSI_ARGS_((ecs_Server *s));
ecs_Result *svr_GetNextObject _ANSI_ARGS_((ecs_Server *s));
ecs_Result *svr_GetRasterInfo _ANSI_ARGS_((ecs_Server *s));
ecs_Result *svr_GetObject _ANSI_ARGS_((ecs_Server *s, char *Id));
ecs_Result *svr_GetObjectIdFromCoord _ANSI_ARGS_((ecs_Server *s, ecs_Coordinate *coord));
ecs_Result *svr_UpdateDictionary _ANSI_ARGS_((ecs_Server *s, char *info));
ecs_Result *svr_GetServerProjection _ANSI_ARGS_((ecs_Server *s));
ecs_Result *svr_GetGlobalBound _ANSI_ARGS_((ecs_Server *s));
ecs_Result *svr_SetServerLanguage _ANSI_ARGS_((ecs_Server *s, u_int language));
ecs_Result *svr_SetCompression _ANSI_ARGS_((ecs_Server *s, ecs_Compression *compression));
ecs_Result *svr_SetServerProjection _ANSI_ARGS_((ecs_Server *s, char *projection));
ecs_Result *svr_SetRasterConversion _ANSI_ARGS_((ecs_Server *s,
						 ecs_RasterConversion *rc));

/* These functions will be called by dynamic servers */

int ecs_SetLayer  _ANSI_ARGS_((ecs_Server *s, ecs_LayerSelection *sel));
int ecs_GetLayer  _ANSI_ARGS_((ecs_Server *s, ecs_LayerSelection *sel));
void ecs_FreeLayer _ANSI_ARGS_((ecs_Server *s, int layer));
int ecs_RemoveDir _ANSI_ARGS_((char *path));

/* These functions are internally used in server.c */

int ecs_GetLateralDBConnectionCtrlFile _ANSI_ARGS_((ecs_Server *s));
int ecs_SetAttributeLinkWithRequest _ANSI_ARGS_((ecs_Server *s, char *request, ecs_Family family));
int ecs_RemoveAttributeLinkWithRequest _ANSI_ARGS_((ecs_Server *s, char *request, ecs_Family family));
int ecs_SetAttributeQuery _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l, char **error));
int ecs_ReleaseAttributeQuery _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l, char **error));
int ecs_UnstackRequest _ANSI_ARGS_((ecs_Server *s, char **request));
int AddAttributeLink _ANSI_ARGS_((ecs_Server *s, char *url, char *layer, char *family, char *drivertype, char *infosource, char *userdesc, char *autorization, char *request));
int ecs_SetBindListForVector _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l, ecs_Result *msg, char ***BindList, char **error));
int ecs_SetBindListForMatrix _ANSI_ARGS_((ecs_Server *s,ecs_Layer *l,ecs_Category *ptr,char ***BindList,char **error));
int ecs_AddAttributeLink _ANSI_ARGS_((ecs_Server *s,char *url,char *layer,ecs_Family family,char *drivertype,char *infosource,
				      char *userdesc,char *autorization,char *request));
int ecs_ExtractRequestInformation _ANSI_ARGS_((char *request,char **ExtractRequest,char **DriverType,char **InformationSource,
					       char **UserDescription,char **AutorizationDescription, char **SelectionRequest));

/***********************************************************************/

/* ecs_dyna.c declarations */

void *ecs_OpenDynamicLib _ANSI_ARGS_((char *libname));
void *ecs_GetDynamicLibFunction _ANSI_ARGS_((void *handle,char *functionname));
void ecs_CloseDynamicLib _ANSI_ARGS_((void *handle));

/***********************************************************************/

/* ecsregex.c declarations */

#define NSUBEXP  50
typedef struct ecs_regexp {
	char *startp[NSUBEXP];
	char *endp[NSUBEXP];
	char regstart;		/* Internal use only. */
	char reganch;		/* Internal use only. */
	char *regmust;		/* Internal use only. */
	int regmlen;		/* Internal use only. */
	char program[1];	/* Unwarranted chumminess with compiler. */
} ecs_regexp;

ecs_regexp *EcsRegComp _ANSI_ARGS_((char *exp));
int EcsRegExec _ANSI_ARGS_((ecs_regexp *prog, char *string, char *start));
void EcsRegError _ANSI_ARGS_((char *msg));
char *EcsGetRegError _ANSI_ARGS_((void));

/***********************************************************************/

/* ecsdist.c declarations */


double ecs_DistanceObject _ANSI_ARGS_((ecs_Object *obj, double X, double Y));
double ecs_DistanceObjectWithTolerance _ANSI_ARGS_((ecs_Object *obj, double X, double Y));
double ecs_SetTolerance _ANSI_ARGS_((ecs_Region *reg));
double ecs_DistanceMBR _ANSI_ARGS_((double xl, double yl, double xu, double yu, double dx, double dy));
double ecs_DistanceSegment _ANSI_ARGS_((double xl, double yl, double xu, double yu, double dx, double dy));
int ecs_IsPointInPolygon _ANSI_ARGS_((int npoints, ecs_Coordinate *poly,double x,double y));

/***********************************************************************/
/* ecsinfo.c declaration  */
 
int ecs_GetDefaultInfo _ANSI_ARGS_((char* url, char* key, char** result));

/***********************************************************************/

/* ecsassoc.c declarations */

/* WARNING: These define do more than one operation.
   Please, don't consider these as functions.
   */


/*
  ----------------------------------------------------------------
  ECSRESULTTYPE: This macro indicate the object type of ecs_Result
  ----------------------------------------------------------------
  */

#define ECSRESULTTYPE(result) ((result)->res.type)

/*
  ----------------------------------------------------------------
  ECSRESULTTYPE: This macro access the union in ecs_Result and 
  facilitate ecs_Result structures access.
  ----------------------------------------------------------------
  */

#define ECSRESULT(result) ((result)->res.ecs_ResultUnion_u)

/*
  ----------------------------------------------------------------
  ECSRESULTTYPE: This macro indicate the geographical object type 
  contain in ecs_Result
  ----------------------------------------------------------------
  */

#define ECSGEOMTYPE(result) ((result)->res.ecs_ResultUnion_u.dob.geom.family)

/*
  ----------------------------------------------------------------
  ECSGEOM: This macro access the union contain in the geographical
  object in ecs_Result.
  ----------------------------------------------------------------
  */

#define ECSGEOM(result) ((result)->res.ecs_ResultUnion_u.dob.geom.ecs_Geometry_u)

/*
  ----------------------------------------------------------------
  ECSAREARING: This macro access the area ring in the geographic
  object of type Area
  ----------------------------------------------------------------
  */

#define ECSAREARING(result,pos) ((result)->res.ecs_ResultUnion_u.dob.geom.ecs_Geometry_u.area.ring.ring_val[(pos)])

/*
  ----------------------------------------------------------------
  ECSERROR: Indicate if ecs_Result contain an error code
  ----------------------------------------------------------------
  */

#define ECSERROR(r) ((r)->error != 0)

/*
  ----------------------------------------------------------------
  ECSSUCCESS: Indicate if ecs_Result contain an success code
  ----------------------------------------------------------------
  */

#define ECSSUCCESS(r) ((r)->error == 0)

/*
  ----------------------------------------------------------------
  ECSEOF: Indicate if ecs_Result contain EOF message. Mainly
  use in cln_GetNextObject function.
  ----------------------------------------------------------------
  */

#define ECSEOF(r) ((r)->error == 2)

/*
  ----------------------------------------------------------------
  ECSPROJ: Indicate if ecs_Result contain PROJ error message. Mainly
  use in cln_GetNextObject function.
  ----------------------------------------------------------------
  */

#define ECSPROJ(r) ((r)->error == 3)

/*
  ----------------------------------------------------------------
  ECSMESSAGE: Return the error message contain in ecs_Result (an
  string).
  ----------------------------------------------------------------
  */

#define ECSMESSAGE(r) ((r)->message)

/*
  ----------------------------------------------------------------
  ECSREGION: Return the geographical region contain in ecs_Result
  (if it's the case). The structure returned is a ecs_Region.
  ----------------------------------------------------------------
  */

#define ECSREGION(r) (ECSRESULT(r).gr)

/*
  ----------------------------------------------------------------
  ECSTEXT: Return the text string contain in ecs_Result (if 
  it's the case). The structure returned is a string.
  ----------------------------------------------------------------
  */

#define ECSTEXT(r) (ECSRESULT(r).s)

/*
  ----------------------------------------------------------------
  ECSRASTERINFO: Return the raster information contain in ecs_Result
  (if it's the case). The structure return is a ecs_RasterInfo.
  ----------------------------------------------------------------
  */

#define ECSRASTERINFO(r) (ECSRESULT(r).ri)

/*
  ----------------------------------------------------------------
  ECSRASTERINFONB: Return the number of categories in the
  ecs_RasterInfo structure contain in ecs_Result.
  ----------------------------------------------------------------
  */

#define ECSRASTERINFONB(r) (ECSRASTERINFO(r).cat.cat_len)

/*
  ----------------------------------------------------------------
  ECSRASTERINFOCAT: Return the category number "c" contain in
  ecs_RasterInfo in ecs_Result. The structure returned is a
  ecs_Category.
  ----------------------------------------------------------------
  */

#define ECSRASTERINFOCAT(r,c) (ECSRASTERINFO(r).cat.cat_val[(c)])

/*
  ----------------------------------------------------------------
  ECSOBJECT: Return the geographic object contain in ecs_Result
  (if it's the case). The structure return is a ecs_Object.
  ----------------------------------------------------------------
  */

#define ECSOBJECT(r) (ECSRESULT(r).dob)

/*
  ----------------------------------------------------------------
  ECSOBJECTID: Return the "Id" attribute contain the ecs_Object structure
  contain in ecs_Result.
  ----------------------------------------------------------------
  */

#define ECSOBJECTID(r) (ECSOBJECT(r).Id)

/*
  ----------------------------------------------------------------
  ECSOBJECTATTR: Return the "attr" attribute contain the ecs_Object structure
  contain in ecs_Result.
  ----------------------------------------------------------------
  */

#define ECSOBJECTATTR(r) (ECSOBJECT(r).attr)

/*
  ----------------------------------------------------------------
  ECSRASTER: Return the raster line table contain in ecs_Object.
  ----------------------------------------------------------------
  */

#define ECSRASTER(r) (ECSOBJECT(r).geom.ecs_Geometry_u.matrix.x.x_val)

/*
  ----------------------------------------------------------------
  ECS_SETGEOMBOUNDINGBOX: This macro will put in the geographical
  object contain in "result" the bounding rectangle (lxmin, lymin,
  lxmax, lymax).
  ----------------------------------------------------------------
  */

#define ECS_SETGEOMBOUNDINGBOX(result,lxmin,lymin,lxmax,lymax) { \
    {if ((result)->res.type==Object) { \
	(result)->res.ecs_ResultUnion_u.dob.xmin=(lxmin); \
	(result)->res.ecs_ResultUnion_u.dob.ymin=(lymin); \
	(result)->res.ecs_ResultUnion_u.dob.xmax=(lxmax); \
	(result)->res.ecs_ResultUnion_u.dob.ymax=(lymax); \
    }} \
}

/*
  ----------------------------------------------------------------
  ECSGEOMLINECOORD: This macro put a point (lx,ly) at the position
  "position" in a line geographical object. To entirely set the
  line object, ecs_SetGeomLine must be call first and for each point,
  ECS_SETGEOMLINECOORD must be call.
  ----------------------------------------------------------------
  */

#define ECS_SETGEOMLINECOORD(result,position,lx,ly) \
{ \
  (result)->res.ecs_ResultUnion_u.dob.geom.ecs_Geometry_u.line.c.c_val[position].x = (lx); \
  (result)->res.ecs_ResultUnion_u.dob.geom.ecs_Geometry_u.line.c.c_val[position].y = (ly); \
}

/*
  ----------------------------------------------------------------
  ECSGEOMAREACOORD: This macro put a point (lx,ly) at the position
  "position" of the ring "ringpos" in a area geographical object. To 
  entirely set the area object, ecs_SetGeomArea must be call first 
  and for each ring, ecs_SetGeomAreaRing must be call. In each of 
  this ring, ECS_SETGEOMAREACOORD must be call define the polygons 
  points.
  ----------------------------------------------------------------
  */

#define ECS_SETGEOMAREACOORD(result,ringpos,position,lx,ly) \
{ \
(result)->res.ecs_ResultUnion_u.dob.geom.ecs_Geometry_u.area.ring.ring_val[ringpos].c.c_val[position].x = lx; \
(result)->res.ecs_ResultUnion_u.dob.geom.ecs_Geometry_u.area.ring.ring_val[ringpos].c.c_val[position].y = ly; \
}

/*
  ----------------------------------------------------------------
  ECSGEOMMATRIXCOORD: This macro put a value at the position 
  "position" in a matrix geographical object. To entirely set the 
  matrix object, ecs_SetGeomMatrix must be call first and for each 
  column, ECS_SETGEOMMATRIXCOORD must be call.
  ----------------------------------------------------------------
  */

#define ECS_SETGEOMMATRIXVALUE(result,lpos,lval) \
{(result)->res.ecs_ResultUnion_u.dob.geom.ecs_Geometry_u.matrix.x.x_val[lpos] = (lval);}

/*
  ----------------------------------------------------------------
  ECSGEOMIMAGECOORD: This macro put a value at the position 
  "position" in a image geographical object. To entirely set the 
  image object, ecs_SetGeomImage must be call first and for each 
  column, ECS_SETGEOMIMAGECOORD must be call.
  ----------------------------------------------------------------
  */

#define ECS_SETGEOMIMAGEVALUE(result,lpos,lval) \
{(result)->res.ecs_ResultUnion_u.dob.geom.ecs_Geometry_u.image.x.x_val[lpos] = (lval);}

/********************************/

int ecs_SetError _ANSI_ARGS_((ecs_Result *r,
			      int errorcode, char *error_message));
int ecs_SetSuccess _ANSI_ARGS_((ecs_Result *r));
int ecs_AdjustResult _ANSI_ARGS_((ecs_Result *r));
int ecs_SetGeoRegion _ANSI_ARGS_((ecs_Result *r,
				  double north, double south, double east,
				  double west, double nsres, double ewres));
int ecs_SetText _ANSI_ARGS_((ecs_Result *r,char *text));
int ecs_AddText _ANSI_ARGS_((ecs_Result *r,char *text));
int ecs_SetRasterInfo _ANSI_ARGS_((ecs_Result *r, int width, int height));
int ecs_AddRasterInfoCategory _ANSI_ARGS_((ecs_Result *r, long no_cat,
					   unsigned int red, 
					   unsigned int green,
					   unsigned int blue, char *label,
					   unsigned long qty));
int ecs_SetObjAttributeFormat _ANSI_ARGS_((ecs_Result *r));
int ecs_AddAttributeFormat _ANSI_ARGS_((ecs_Result *r, char *name,
					ecs_AttributeFormat type,
					int length, int precision,
					int nullable));
int ecs_SetGeomPoint _ANSI_ARGS_((ecs_Result *r, double x, double y));
int ecs_SetGeomText _ANSI_ARGS_((ecs_Result *r, double x, 
				 double y, char *desc));
int ecs_SetGeomLine _ANSI_ARGS_((ecs_Result *r, unsigned int length));
int ecs_SetGeomArea _ANSI_ARGS_((ecs_Result *r, unsigned int length));
int ecs_SetGeomAreaRing _ANSI_ARGS_((ecs_Result *r, int position, 
				     unsigned int length,
				     double centroid_x, double centroid_y));
int ecs_SetGeomMatrix _ANSI_ARGS_((ecs_Result *r, int size));
int ecs_SetGeomMatrixWithArray _ANSI_ARGS_((ecs_Result *r, int size, unsigned int *array));
int ecs_SetGeomImage _ANSI_ARGS_((ecs_Result *r, int size));
int ecs_SetGeomImageWithArray _ANSI_ARGS_((ecs_Result *r, int size, unsigned int *array));
int ecs_SetObjectId _ANSI_ARGS_((ecs_Result *r,char *id));
int ecs_SetObjectAttr _ANSI_ARGS_((ecs_Result *r,char *attr));
int ecs_CleanUp _ANSI_ARGS_((ecs_Result *r));
int ecs_CleanUpObject _ANSI_ARGS_((ecs_Object *r));
int ecs_CleanUpResultUnion _ANSI_ARGS_((ecs_ResultUnion *r));
int ecs_ResultInit _ANSI_ARGS_((ecs_Result *r));
int ecs_CalcObjectMBR _ANSI_ARGS_((ecs_Server *s, ecs_Object *obj));
int ecs_CopyResult _ANSI_ARGS_((ecs_Result *source, ecs_Result **copy));
int ecs_CopyResultFromUnion _ANSI_ARGS_((ecs_ResultUnion *source, ecs_Result **copy));
int ecs_CopyResultUnion _ANSI_ARGS_((ecs_ResultUnion *source, ecs_ResultUnion **copy));
int ecs_CopyResultUnionWork _ANSI_ARGS_((ecs_ResultUnion *source, ecs_ResultUnion *copy));
int ecs_CopyObject _ANSI_ARGS_((ecs_Object *source, ecs_Object *copy));
int ecs_CopyGeometry _ANSI_ARGS_((ecs_Object *source, ecs_Object *copy));
int ecs_CopyArea _ANSI_ARGS_((ecs_Area *source, ecs_Area *copy));
int ecs_CopyLine _ANSI_ARGS_((ecs_Line *source, ecs_Line *copy));
int ecs_CopyPoint _ANSI_ARGS_((ecs_Point *source, ecs_Point *copy));
int ecs_CopyText _ANSI_ARGS_((ecs_Text *source, ecs_Text *copy));
int ecs_CopyMatrix _ANSI_ARGS_((ecs_Matrix *source, ecs_Matrix *copy));
int ecs_CopyImage _ANSI_ARGS_((ecs_Image *source, ecs_Image *copy));
void ecs_FreeObject _ANSI_ARGS_((ecs_Object *obj));


/***********************************************************************/

/* ecs_split.c declarations */

void ecs_freeSplitURL _ANSI_ARGS_((char **type,char **machine,char **path));
int ecs_GetRegex _ANSI_ARGS_((ecs_regexp *reg,int index,char **chaine));
int ecs_SplitURL _ANSI_ARGS_((char *url,char **machine,char **server,char **path));


/***********************************************************************/

/* ecs_list.c declarations */

char ecs_Backslash _ANSI_ARGS_((char *src, int *readPtr));
int ecs_FindElement _ANSI_ARGS_((register char *list,char **elementPtr, char **nextPtr, int *sizePtr, int *bracePtr));
void ecs_CopyAndCollapse _ANSI_ARGS_((int count,register char *src,register char *dst));
int ecs_SplitList _ANSI_ARGS_((char *list,int *argcPtr,char ***argvPtr));

/***********************************************************************/

/* dynamic library server declarations */

ecs_Result *dyn_CreateServer _ANSI_ARGS_((ecs_Server *s, char *Request));
ecs_Result *dyn_DestroyServer _ANSI_ARGS_((ecs_Server *s));
ecs_Result *dyn_SelectLayer _ANSI_ARGS_((ecs_Server *s, ecs_LayerSelection *ls));
ecs_Result *dyn_ReleaseLayer _ANSI_ARGS_((ecs_Server *s, ecs_LayerSelection *ls));
ecs_Result *dyn_SelectRegion _ANSI_ARGS_((ecs_Server *s, ecs_Region *gr));
ecs_Result *dyn_GetDictionary _ANSI_ARGS_((ecs_Server *s));
ecs_Result *dyn_GetAttributesFormat _ANSI_ARGS_((ecs_Server *s));
ecs_Result *dyn_GetNextObject _ANSI_ARGS_((ecs_Server *s));
ecs_Result *dyn_GetRasterInfo _ANSI_ARGS_((ecs_Server *s));
ecs_Result *dyn_GetObject _ANSI_ARGS_((ecs_Server *s, char *Id));
ecs_Result *dyn_GetObjectIdFromCoord _ANSI_ARGS_((ecs_Server *s, ecs_Coordinate *coord));
ecs_Result *dyn_UpdateDictionary _ANSI_ARGS_((ecs_Server *s, char *info));
ecs_Result *dyn_GetServerProjection _ANSI_ARGS_((ecs_Server *s));
ecs_Result *dyn_GetGlobalBound _ANSI_ARGS_((ecs_Server *s));
ecs_Result *dyn_SetServerLanguage _ANSI_ARGS_((ecs_Server *s, u_int language));
ecs_Result *dyn_SetServerProjection _ANSI_ARGS_((ecs_Server *s, char *projection));
ecs_Result *dyn_SetRasterConversion _ANSI_ARGS_((ecs_Server *s,
						 ecs_RasterConversion *rc));
ecs_Result *dyn_SetCompression _ANSI_ARGS_((ecs_Server *s, ecs_Compression *compression));

/***********************************************************************/

/* dynamic library attribute driver declarations */

int dyn_InitializeDBLink _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l, char **error));
int dyn_DeinitializeDBLink _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l, char **error));
int dyn_GetColumnsInfo _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l, int *columns_qty,  ecs_ObjAttribute **attr, char **error));
int dyn_SelectAttributes _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l, int attribute_qty, char **attribute_list, char **error));
int dyn_IsSelected _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l, short *isSelected, char **error));
int dyn_GetSelectedAttributes _ANSI_ARGS_((ecs_Server *s, ecs_Layer *l, char **attributes, char **error));

/***********************************************************************/

/* dynamic library attribute driver declarations */

int dyn_nad_init _ANSI_ARGS_((void **privtableinfo, char *table));
int dyn_nad_close _ANSI_ARGS_((void *privtableinfo));
int dyn_nad_forward _ANSI_ARGS_((void *privtableinfo, double *x, double *y));
int dyn_nad_reverse _ANSI_ARGS_((void *privtableinfo, double *x, double *y));

/***********************************************************************/


/*
 * client.h --
 *
 * Control dispatch of locals client. Also control cache management and
 * projection changes.
 *
 * Copyright (c) 1995 Logiciels et Applications Scientifiques (L.A.S.) Inc.
 * Il est strictement interdit de publier ou de devoiler le contenu de ce
 * programme sans avoir prealablement obtenu la permission de L.A.S. Inc.
 */

#define CACHEINITSIZE 100
#define COMPARETOLERANCE 0.000000001    /* For projection conversion, tolerance factor during projection compare */

#define ECS_TTOS 0  /* define the conversion direction (target to source) or (source to target) */
#define ECS_STOT 1


typedef struct ecs_CtlPoint {
  double e1,n1,e2,n2;
  double errorx,errory;
} ecs_CtlPoint;


typedef struct ecs_CtlPoints {
  int nbpts;
  ecs_CtlPoint *pts;
} ecs_CtlPoints;

 
typedef struct ecs_Cache {
  ecs_LayerSelection coverage;   /* coverage descriptor */

  int size;         /* logical cache size */
  int startpos;	    /* beginning position of cache */
  int currentpos;   /* current position in cache for getnextobject */
  int allocatedSize;   /* physical size of cache */  
  ecs_Result **o;   /* the cache itself */

  struct ecs_Cache *next; /* linked list successor, NULL if end of list */
  struct ecs_Cache *previous; /* linked list predecessor */
} ecs_Cache;

/*********************************************************************

  STRUCTURE_INFORMATION
  
  NAME
     ecs_Datum

  DESCRIPTION
     Description of all the possible states of a projection about datum.
  END_DESCRIPTION

  END_STRUCTURE_INFORMATION

  ********************************************************************/

enum ecs_Datum {
	nodatum = 1,
	nad27 = 2,
	nad83 = 3
};
typedef enum ecs_Datum ecs_Datum;

typedef int dtfunc();

/*********************************************************************

  STRUCTURE_INFORMATION
  
  NAME
     ecs_LayerCapabilities

  DESCRIPTION
     Information about a layer gleaned from OGDI_Capabilities document.
  END_DESCRIPTION

  ATTRIBUTES
     char *name: The default layer name, as per the <Name> element.
  END_ATTRIBUTES

  END_STRUCTURE_INFORMATION

  ********************************************************************/

#define MAX_FAMILIES 20

typedef struct {
  char  *name;
  char  *title;
  char  *srs;
  int   families[MAX_FAMILIES];
  char  **parents;
  char  **extensions;

  int    ll_bounds_set;
  double ll_north;
  double ll_south;
  double ll_east;
  double ll_west;

  double srs_north;
  double srs_south;
  double srs_east;
  double srs_west;
  double srs_nsres;
  double srs_ewres;

  int    query_expression_set;
  char   *qe_prefix;
  char   *qe_suffix;
  char   *qe_format;
  char   *qe_description;
} ecs_LayerCapabilities;

/*********************************************************************

  STRUCTURE_INFORMATION
  
  NAME
     ecs_Client

  DESCRIPTION
     Description of the attributes of the c interface object (client.c).
  END_DESCRIPTION

  ATTRIBUTES
     char *url: Client URL, mostly used to see if a client already exist
     ecs_Cache *cache: linked list of all cached coverage
     ecs_Cache *selectCache: A pointer to the cache selected by SelectLayer
     ecs_Region cacheRegion: mbr region of cache
     ecs_Region currentRegion: mbr of current region
     ecs_Family currentSelectionFamily: Current layer selection type
     char *tclprocname: attribute callback procedure for tcl
     char *target_proj: Projection descriptor
     PJ *target: target (c interface) projection descriptors
     PJ *source: source (driver) projection descriptors
     ecs_Datum targetdatum: target datum information
     ecs_Datum sourcedatum: source datum information
     void *dthandle: Handle to the datum driver
     void *privdatuminfo: The private datum information pointer. Used to specify the object.
     dtfunc *nad_init: Pointer to the nad_init function in the datum driver
     dtfunc *nad_forward: Pointer to the nad_forward function in the datum driver
     dtfunc *nad_reverse: Pointer to the nad_reverse function in the datum driver
     dtfunc *nad_close: Pointer to the nad_close function in the datum driver
     char datumtable[10]: Datum table name
     int isSourceLL: Indicate if the source is a longlat projection
     int isTargetLL: Indicate if the target is a longlat projection
     int isProjEqual: Indicate if the projections are the same
     int isCurrentRegionSet: Indicate if the current region is set
     double target_azimuth: The azimuth angle to apply to the target projection
     double sinazimuth: The azimuth sinus
     double cosazimuth: The azimuth cosinus
     ecs_Server s: The ecs_Server structure is hold here.
     ecs_FeatureRing *mask: Polygon mask
     int isMaskInclusive: Indicate if the mask is inclusive (all the objects are completelly inside) or not.
  END_ATTRIBUTES

  END_STRUCTURE_INFORMATION

  ********************************************************************/

typedef struct {
  char *url;              /* Client URL, mostly used to see if a client already exist */

  ecs_Cache *cache;       /* linked list of all cached coverage */
  ecs_Cache *selectCache; /* A pointer to the cache selected by SelectLayer */
  ecs_Cache *autoCache;   /* Automatic cache when using with remote servers */
  ecs_Region cacheRegion; /* mbr region of cache */
  ecs_Region currentRegion; /* mbr of current region */
  ecs_Family currentSelectionFamily; /* Current layer selection type */
  char *tclprocname;      /* attribute callback procedure for tcl */
  char *target_proj;
  PJ *target;             /* source and target projection descriptors */
  PJ *source;
  int isSourceLL;         /* flags to avoid unnecessary computation */
  int isTargetLL;
  int isProjEqual;
  int isCurrentRegionSet;
  double target_azimuth;
  double sinazimuth;
  double cosazimuth;
  ecs_Datum targetdatum;
  ecs_Datum sourcedatum;
  char datumtable[10];
  void *privdatuminfo;
  void *dthandle;
  dtfunc *nad_init;
  dtfunc *nad_forward;
  dtfunc *nad_reverse;
  dtfunc *nad_close;

  ecs_Server s;

  ecs_FeatureRing *mask;
  ecs_Region maskregion;
  int isMaskInclusive;

  int           have_server_capabilities;				       
  char		server_version_str[32];
  int           server_version; /* version times 1000, ie. 3100 for 3.1 */

  char          **global_extensions;

  int           have_capabilities;
  int           layer_cap_count;
  ecs_LayerCapabilities **layer_cap;
    
} ecs_Client;

#define MAXCLIENT 32

/* Functions used for allocation and deallocation. */

void        cln_FreeClient           _ANSI_ARGS_((ecs_Client **c));
int         cln_AllocClient          _ANSI_ARGS_((char *URL,int *error));

/* API functions */

ecs_Result *cln_CreateClient         _ANSI_ARGS_((int *ClientID,char *url));
ecs_Result *cln_DestroyClient        _ANSI_ARGS_((int ClientID));
ecs_Result *cln_SelectLayer          _ANSI_ARGS_((int ClientID, ecs_LayerSelection *ls));
ecs_Result *cln_ReleaseLayer         _ANSI_ARGS_((int ClientID, ecs_LayerSelection *ls));
void cln_BroadCloseLayers();
ecs_Result *cln_SelectMask _ANSI_ARGS_((int ClientID,ecs_FeatureRing *mask,int isInclusive));
ecs_Result *cln_UnSelectMask _ANSI_ARGS_((int ClientID));
ecs_Result *cln_SelectRegion         _ANSI_ARGS_((int ClientID, ecs_Region *gr));
ecs_Result *cln_GetDictionary        _ANSI_ARGS_((int ClientID));
ecs_Result *cln_GetAttributesFormat  _ANSI_ARGS_((int ClientID));
ecs_Result *cln_GetNextObject        _ANSI_ARGS_((int ClientID));
ecs_Result *cln_GetRasterInfo        _ANSI_ARGS_((int ClientID));
ecs_Result *cln_GetObject            _ANSI_ARGS_((int ClientID, char *Id));
ecs_Result *cln_GetObjectIdFromCoord _ANSI_ARGS_((int ClientID, ecs_Coordinate *coord));
ecs_Result *cln_UpdateDictionary     _ANSI_ARGS_((int ClientID, char *info));
ecs_Result *cln_GetGlobalBound       _ANSI_ARGS_((int ClientID));
ecs_Result *cln_SetServerLanguage    _ANSI_ARGS_((int ClientID, u_int language));
ecs_Result *cln_SetCompression       _ANSI_ARGS_((int ClientID, ecs_Compression *compression));
ecs_Result *cln_GetServerProjection  _ANSI_ARGS_((int ClientID));
ecs_Result *cln_SetServerProjection  _ANSI_ARGS_((int ClientID, char *projection));
ecs_Result *cln_SetClientProjection  _ANSI_ARGS_((int ClientID, char *projection));
void cln_SetTclProc                  _ANSI_ARGS_((int ClientID, char *tclproc));
char *cln_GetTclProc                 _ANSI_ARGS_((int ClientID));


/* Projection conversion functions */

PJ *cln_ProjInit                     _ANSI_ARGS_((char *d));
int cln_CompareProjections           _ANSI_ARGS_((int ClientID));
int cln_UpdateMaxRegion              _ANSI_ARGS_((int ClientID, double x, double y, ecs_Region *gr, int sens, int first));
int cln_ConvRegion                   _ANSI_ARGS_((int ClientID, ecs_Region *gr, int sens));
int cln_ConvTtoS                     _ANSI_ARGS_((int ClientID, double *X, double *Y));
int cln_ConvStoT                     _ANSI_ARGS_((int ClientID, double *X, double *Y));
int cln_ChangeProjection             _ANSI_ARGS_((int ClientID, ecs_Object *obj));
int cln_ChangeProjectionArea         _ANSI_ARGS_((int ClientID, ecs_Area *obj));
int cln_ChangeProjectionLine         _ANSI_ARGS_((int ClientID, ecs_Line *obj));
int cln_ChangeProjectionPoint        _ANSI_ARGS_((int ClientID, ecs_Point *obj));
int cln_ChangeProjectionMatrix       _ANSI_ARGS_((int ClientID, ecs_Matrix *obj));
int cln_ChangeProjectionImage        _ANSI_ARGS_((int ClientID, ecs_Image *obj));
int cln_ChangeProjectionText         _ANSI_ARGS_((int ClientID, ecs_Text *obj));
int cln_PointValid                   _ANSI_ARGS_((int ClientID, double x, double y));
ecs_Datum cln_GetDatumInfo           _ANSI_ARGS_((char *projection));

/* Matrix conversion functions */

int cln_CalcCtlPoint _ANSI_ARGS_((int ClientID, ecs_Region *server_region,
				  int SI, int SJ, ecs_CtlPoint *pt));
int cln_CalcCtlPoints _ANSI_ARGS_((int ClientID, ecs_CtlPoints **pts,
				   char **error_message));
int cln_SetRasterConversion          _ANSI_ARGS_((int ClientID,
						  ecs_CtlPoints **pts,
						  ecs_Resampling resampling,
						  ecs_Transformation trans,
						  char **error_message));
						  

/* URL manipulation */

int cln_GetClientIdFromURL           _ANSI_ARGS_((char *url));
int cln_GetURLList                   _ANSI_ARGS_((char **urllist));

/* Cache functions */

int cln_SetRegionCaches              _ANSI_ARGS_((int ClientID, ecs_Region *GR, char **error_message));
int cln_LoadCache                    _ANSI_ARGS_((int ClientID, ecs_LayerSelection *ls, char **error_message));
int cln_ReleaseCache                 _ANSI_ARGS_((int ClientID, ecs_LayerSelection *ls, char **error_message));
void cln_FreeCache                   _ANSI_ARGS_((ecs_Cache *cache));
ecs_Cache *cln_NewCache              _ANSI_ARGS_((int size));
ecs_Cache *cln_FoundCache            _ANSI_ARGS_((int ClientID, ecs_LayerSelection *ls));

/* The rest */

void cln_BlockOGDI();
void cln_UnBlockOGDI();
int cln_IsGeoObjectInsideMask        _ANSI_ARGS_((int ClientID,ecs_Result *obj));
int cln_ConvMBR                      _ANSI_ARGS_((int ClientID,double *xl,double *yl,double *xu,double *yu,int direction));

/* capabilities */

ecs_Result *cln_GetVersion( int ClientID );
int         cln_CheckExtension( int ClientID, const char *extension,
                                const char *layer_name );
const ecs_LayerCapabilities *
            cln_GetLayerCapabilities( int ClientID, int layer_index );

ecs_Result *cln_LoadCapabilities( int ClientID, const char *arg, int err );
void ecs_ParseCapabilities( ecs_Client *cln, const char *cap_doc, 
                            ecs_Result *result );

/***********************************************************************/


/*
 * ecsgeo --
 *
 * Make geometric calculations
 *
 * Copyright (c) 1995 Logiciels et Applications Scientifiques (L.A.S.) Inc.
 * Il est strictement interdit de publier ou de devoiler le contenu de ce
 * programme sans avoir prealablement obtenu la permission de L.A.S. Inc.
 */

void ecs_begin_ellipsoid_polygon_area _ANSI_ARGS_((double a, double e2));
double ecs_Q _ANSI_ARGS_((double x));
double ecs_Qbar _ANSI_ARGS_((double x));
double ecs_planimetric_polygon_area _ANSI_ARGS_((int n,ecs_Coordinate *coord));
double ecs_ellipsoid_polygon_area _ANSI_ARGS_((int n,ecs_Coordinate *coord));
double ecs_geodesic_distance _ANSI_ARGS_((double lon1, double lat1, double lon2, double lat2));
double ecs_distance_meters _ANSI_ARGS_((char *projection, double X1, double Y1, double X2, double Y2));
int ecs_CalculateCentroid _ANSI_ARGS_((int nb_segment, ecs_Coordinate *coord,ecs_Coordinate *centroid));


/***********************************************************************/

/*
 * Structure definition for an entry in a hash table.  No-one outside
 * ecs should access any of these fields directly;  use the macros
 * defined below.
 */

typedef struct ecs_HashEntry {
    struct ecs_HashEntry *nextPtr;	/* Pointer to next entry in this
					 * hash bucket, or NULL for end of
					 * chain. */
    struct ecs_HashTable *tablePtr;	/* Pointer to table containing entry. */
    struct ecs_HashEntry **bucketPtr;	/* Pointer to bucket that points to
					 * first entry in this entry's chain:
					 * used for deleting the entry. */
    int clientData;  		        /* Application stores something here
					 * with ecs_SetHashValue. */
    union {				/* Key has one of these forms: */
	char *oneWordValue;		/* One-word value for key. */
	int words[1];			/* Multiple integer words for key.
					 * The actual size will be as large
					 * as necessary for this table's
					 * keys. */
	char string[4];			/* String for key.  The actual size
					 * will be as large as needed to hold
					 * the key. */
    } key;				/* MUST BE LAST FIELD IN RECORD!! */
} ecs_HashEntry;

/*
 * Structure definition for a hash table.  Must be in ecs.h so clients
 * can allocate space for these structures, but clients should never
 * access any fields in this structure.
 */

#define ECS_SMALL_HASH_TABLE 4
typedef struct ecs_HashTable {
    ecs_HashEntry **buckets;		/* Pointer to bucket array.  Each
					 * element points to first entry in
					 * bucket's hash chain, or NULL. */
    ecs_HashEntry *staticBuckets[ECS_SMALL_HASH_TABLE];
					/* Bucket array used for small tables
					 * (to avoid mallocs and frees). */
    int numBuckets;			/* Total number of buckets allocated
					 * at **bucketPtr. */
    int numEntries;			/* Total number of entries present
					 * in table. */
    int rebuildSize;			/* Enlarge table when numEntries gets
					 * to be this large. */
    int downShift;			/* Shift count used in hashing
					 * function.  Designed to use high-
					 * order bits of randomized keys. */
    int mask;				/* Mask value used in hashing
					 * function. */
    int keyType;			/* Type of keys used in this table. 
					 * It's either ECS_STRING_KEYS,
					 * ECS_ONE_WORD_KEYS, or an integer
					 * giving the number of ints in a
					 */
    ecs_HashEntry *(*findProc) _ANSI_ARGS_((struct ecs_HashTable *tablePtr,
	    char *key));
    ecs_HashEntry *(*createProc) _ANSI_ARGS_((struct ecs_HashTable *tablePtr,
	    char *key, int *newPtr));
} ecs_HashTable;

/*
 * Structure definition for information used to keep track of searches
 * through hash tables:
 */

typedef struct ecs_HashSearch {
    ecs_HashTable *tablePtr;		/* Table being searched. */
    int nextIndex;			/* Index of next bucket to be
					 * enumerated after present one. */
    ecs_HashEntry *nextEntryPtr;	/* Next entry to be enumerated in the
					 * the current bucket. */
} ecs_HashSearch;

/*
 * Acceptable key types for hash tables:
 */

#define ECS_STRING_KEYS		0
#define ECS_ONE_WORD_KEYS	1

/*
 * Macros for clients to use to access fields of hash entries:
 */

#define ecs_GetHashValue(h) ((h)->clientData)
#define ecs_SetHashValue(h, value) ((h)->clientData = (ClientData) (value))
#define ecs_GetHashKey(tablePtr, h) \
    ((char *) (((tablePtr)->keyType == ECS_ONE_WORD_KEYS) ? (h)->key.oneWordValue \
						: (h)->key.string))

/*
 * Macros to use for clients to use to invoke find and create procedures
 * for hash tables:
 */

#define ecs_FindHashEntry(tablePtr, key) \
	(*((tablePtr)->findProc))(tablePtr, key)
#define ecs_CreateHashEntry(tablePtr, key, newPtr) \
	(*((tablePtr)->createProc))(tablePtr, key, newPtr)

void ecs_InitHashTable _ANSI_ARGS_((ecs_HashTable *tablePtr, int keyType));
void ecs_DeleteHashEntry _ANSI_ARGS_((ecs_HashEntry *entryPtr));
void ecs_DeleteHashTable _ANSI_ARGS_((ecs_HashTable *tablePtr));
ecs_HashEntry * ecs_FirstHashEntry _ANSI_ARGS_((ecs_HashTable *tablePtr,
						ecs_HashSearch *searchPtr));
ecs_HashEntry * ecs_NextHashEntry _ANSI_ARGS_((ecs_HashSearch *searchPtr));
char *ecs_HashStats _ANSI_ARGS_((ecs_HashTable *tablePtr));


/***********************************************************************/

typedef int tile_func();

/*
  ********************************************************************

   STRUCTURE_INFORMATION

        NAME
	        ecs_TileID
        DESCRIPTION
	        Identifier for a tile
        END_DESCRIPTION

        ATTRIBUTES
	        int x:        the horizontal tile number
	        int y:        the vertical tile number
		short none:   0 if invalid tile
        END_ATTRIBUTES

   END_STRUCTURE_INFORMATION

   ********************************************************************
*/

typedef struct ecs_TileID {
  int x;
  int y;
  short none;  /* 1 if valid, 0 if uninitialized */
} ecs_TileID;

/*
  ********************************************************************

   STRUCTURE_INFORMATION

        NAME
	        ecs_TileBufferLine
        DESCRIPTION
	        A line in the tile buffer.  Stores categories
		that have been retrieved from the file.
        END_DESCRIPTION

        ATTRIBUTES
	        int *linebuffer: ptr to array of values
		                 (length is stored in ecs_Tile).
		int index:       the y-matrix position of this line.
		int last:        the last filled point.
	        ecs_TileBufferLinePtr *next: pointer to next line.
        END_ATTRIBUTES

   END_STRUCTURE_INFORMATION

   ********************************************************************
*/

typedef struct ecs_TileBufferLine {
  int *linebuffer;
  int index;
  int last;
  struct ecs_TileBufferLine *next;
} ecs_TileBufferLine;

/*
  ********************************************************************

   STRUCTURE_INFORMATION

        NAME
	        ecs_Tile
        DESCRIPTION
	        structure for storing information about tiling
        END_DESCRIPTION

        ATTRIBUTES
	        int x: the number of horizontal tiles

                int y: the number of vertical tiles

		ecs_Region region: the "global" region of the tile

		ecs_Result *callback(); the callback to get the i,j
		value

		int offValue; the value for a "no data" raster value

		int uninitializedValue the value for an uninitialized
		matrix value

		ecs_TileBufferLine *linebuffer; a pointer to a linked
		list of buffered lines

		int index; index of the top line in the buffer (y
		matrix value)

		int nb_lines the number of lines

		int linelength; the length of a line
		
		ecs_TileID currentTile; the current tile: -1 if none

		void *priv; private data for tile.
        END_ATTRIBUTES

   END_STRUCTURE_INFORMATION

   ********************************************************************
*/

typedef struct ecs_TileStructure {
  int xtiles;                            /* the number of horizontal tiles */
  int ytiles;                            /* the number of vertical tiles */
  tile_func *callback;                   /* the callback to get the i,j value */
  tile_func *tileDimCallback;            /* a callback to get the tile dimensions */
  int offValue;                          /* the value for a "no data" raster value */
  int uninitializedValue;                /* the value for an uninitialized matrix value */

  ecs_Region region;
  int regionwidth;
  int regionheight;
  
  ecs_TileBufferLine *linebuffer;        /* a pointer to a linked list of buffered lines */
  int index;                             /* the index of the top line in the buffer */
  int nb_lines;                          /* the number of lines */
  int linelength;                        /* the length of a line */

  int width;                             /* the width of a tile */
  int height;                            /* the height of a tile */

  ecs_TileID currentTile;                /* the current tile: -1 if none */
  void *priv;
} ecs_TileStructure;

/* public commands */
int ecs_TileInitialize _ANSI_ARGS_((ecs_Server *s, ecs_TileStructure *t, ecs_Region *region, int xtiles, int ytiles, int tilewidth, int tileheight, tile_func *callbackFunc, tile_func *globalRegionFunc));
int ecs_TileGetLine _ANSI_ARGS_((ecs_Server *s, ecs_TileStructure *t, ecs_Coordinate *start, ecs_Coordinate *end));
void ecs_TileClearBuffer _ANSI_ARGS_((ecs_TileStructure *t));

/* private commands */
int ecs_TileFind _ANSI_ARGS_((ecs_Server *s, ecs_TileID *id, ecs_Coordinate *coord));
int ecs_TileFindBuffer _ANSI_ARGS_((ecs_TileStructure *t, int bufptr, ecs_TileBufferLine **tbuf));
int ecs_TileGetCurPos _ANSI_ARGS_((ecs_Server *s, ecs_TileStructure *t, int i, int j, int *cat));


int ecs_TileFill _ANSI_ARGS_((ecs_Server *s, ecs_TileStructure *t, int index, ecs_TileID *tile_id));
int ecs_TileAddLine _ANSI_ARGS_((ecs_TileStructure *tile, int length, int index, ecs_TileBufferLine **tbuf));
int ecs_TileDeleteLine _ANSI_ARGS_((ecs_TileStructure *tile));
void ecs_TileDeleteAllLines _ANSI_ARGS_((ecs_TileStructure *tile));
int ecs_GetTileId _ANSI_ARGS_((ecs_Server *s, ecs_TileStructure *t, ecs_Coordinate *coord, ecs_TileID *tile_id)); 
int ecs_GetTileIdFromPos _ANSI_ARGS_((ecs_Server *s, ecs_TileStructure *t, int x, int y, ecs_TileID *tile_id)); 
int ecs_TileCompare _ANSI_ARGS_((ecs_TileID *id1, ecs_TileID *id2));
int ecs_SetTile _ANSI_ARGS_((ecs_TileID *destination, ecs_TileID *source));

/* RGB Image functions */

void ecs_GetRGBFromPixel( unsigned int pixel, 
                          unsigned char *transparent, 
                          unsigned char *r, 
                          unsigned char *g, 
                          unsigned char *b );

unsigned int ecs_GetPixelFromRGB( int trans, int r, int g, int b );

#endif /* ECS_UTIL */


