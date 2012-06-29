/*
 * Compiz configuration system library
 *
 * Copyright (C) 2007  Dennis Kasprzyk <onestone@beryl-project.org>
 * Copyright (C) 2007  Danny Baumann <maniac@beryl-project.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _CSS_H
#define _CSS_H

#ifdef __cplusplus
extern "C"
{
#endif

#define D_NONE   0
#define D_NORMAL 1
#define D_FULL   2

#ifndef DEBUGLEVEL
# define DEBUGLEVEL D_FULL
#endif
#define D(x, fmt, args...)  { if (x <= DEBUGLEVEL) fprintf(stderr, fmt, ##args); }

#ifndef Bool
#define Bool int
#endif

#ifndef TRUE
#define TRUE ~0
#endif

#ifndef FALSE
#define FALSE 0
#endif

/**
 * list functions:
 * for each list there is a set of functions, explained using String as example
 *
 * ccsStringListAppend (list, item)
 * Adds an item at the end of the list. Returns the new list.
 *
 * ccsStringListPrepend (list, item)
 * Adds an item at the beginning of the list. Returns the new list.
 *
 * ccsStringListInsert (list, item, position)
 * Adds an item at a given position. Position is 0-based. If position is
 * larger than the amount of items in the list, the item is inserted at the
 * end of the list. Returns the new list.
 *
 * ccsStringListInsertBefore (list, sibling, item)
 * Inserts item before sibling into the list. If sibling is no list member,
 * item is inserted at the end. Returns the new list.
 *
 * ccsStringListLength (list)
 * Returns the amount of items in list.
 *
 * ccsStringListFind (list, item)
 * Finds and returns an item matching <item>. If nothing is found, returns NULL.
 *
 * ccsStringListGetItem (list, index)
 * Returns the list item at position <index>. If index is larger than the
 * amount of items in the list, returns NULL.
 *
 * ccsStringListRemove (list, item, freeObj)
 * Removes item from the list. If freeObj is TRUE, also frees the data item.
 * Returns the new list.
 *
 * ccsStringListFree (list, freeObj)
 * Frees the complete list. If freeObj is TRUE, also frees the data items.
 * Returns the new list (NULL).
 */
#define CCSLIST_HDR(type,dtype)		\
    typedef struct _CCS##type##List *	CCS##type##List;\
    struct _CCS##type##List	\
    {								\
	dtype   * data;			\
	CCS##type##List next;		\
    }; \
    CCS##type##List ccs##type##ListAppend (CCS##type##List list, dtype *data); \
    CCS##type##List ccs##type##ListPrepend (CCS##type##List list, dtype *data); \
    CCS##type##List ccs##type##ListInsert (CCS##type##List list, dtype *data, int position); \
    CCS##type##List ccs##type##ListInsertBefore (CCS##type##List list, CCS##type##List sibling, dtype *data); \
    unsigned int ccs##type##ListLength (CCS##type##List list); \
    CCS##type##List ccs##type##ListFind (CCS##type##List list, dtype *data); \
    CCS##type##List ccs##type##ListGetItem (CCS##type##List list, unsigned int index); \
    CCS##type##List ccs##type##ListRemove (CCS##type##List list, dtype *data, Bool freeObj); \
    CCS##type##List ccs##type##ListFree (CCS##type##List list, Bool freeObj);

typedef struct _CCSContext	  CCSContext;
typedef struct _CCSPlugin	  CCSPlugin;
typedef struct _CCSSetting	  CCSSetting;
typedef struct _CCSGroup	  CCSGroup;
typedef struct _CCSSubGroup	  CCSSubGroup;
typedef struct _CCSPluginCategory CCSPluginCategory;
typedef struct _CCSSettingValue	  CCSSettingValue;
typedef struct _CCSPluginConflict CCSPluginConflict;
typedef struct _CCSBackendInfo	  CCSBackendInfo;
typedef struct _CCSIntDesc	  CCSIntDesc;
typedef struct _CCSStrRestriction CCSStrRestriction;
typedef struct _CCSStrExtension   CCSStrExtension;
typedef struct _CCSString	  CCSString;

CCSLIST_HDR (Plugin, CCSPlugin)
CCSLIST_HDR (Setting, CCSSetting)
CCSLIST_HDR (String, CCSString)
CCSLIST_HDR (Group, CCSGroup)
CCSLIST_HDR (SubGroup, CCSSubGroup)
CCSLIST_HDR (SettingValue, CCSSettingValue)
CCSLIST_HDR (PluginConflict, CCSPluginConflict)
CCSLIST_HDR (BackendInfo, CCSBackendInfo)
CCSLIST_HDR (IntDesc, CCSIntDesc)
CCSLIST_HDR (StrRestriction, CCSStrRestriction)
CCSLIST_HDR (StrExtension, CCSStrExtension)

typedef struct _CCSInterface CCSInterface; /* Dummy typedef */
typedef struct _CCSPrivate CCSPrivate; /* Dummy typedef */
typedef struct _CCSObject CCSObject;

typedef void * (*reallocObjectProc) (void *, void *, size_t);
typedef void * (*mallocObjectProc) (void *, size_t);
typedef void * (*callocObjectProc) (void *, size_t, size_t);
typedef void (*freeObjectProc) (void *, void *);

typedef struct _CCSObjectAllocationInterface
{
    reallocObjectProc realloc_;
    mallocObjectProc  malloc_;
    callocObjectProc  calloc_;
    freeObjectProc    free_;
    void              *allocator;
} CCSObjectAllocationInterface;

extern CCSObjectAllocationInterface ccsDefaultObjectAllocator;

struct _CCSObject
{
    CCSPrivate *priv; /* Private pointer for object storage */

    const CCSInterface **interfaces; /* An array of interfaces that this object implements */
    int          *interface_types; /* An array of interface types */
    unsigned int n_interfaces;
    unsigned int n_allocated_interfaces;

    CCSObjectAllocationInterface *object_allocation;

    unsigned int refcnt; /* Reference count of this object */
};

Bool
ccsObjectInit_ (CCSObject *object, CCSObjectAllocationInterface *interface);

#define ccsObjectInit(o, interface) (ccsObjectInit_) (&(o)->object, interface)

Bool
ccsObjectAddInterface_ (CCSObject *object, const CCSInterface *interface, int interface_type);

#define ccsObjectAddInterface(o, interface, type) (ccsObjectAddInterface_) (&(o)->object, interface, type);

Bool
ccsObjectRemoveInterface_ (CCSObject *object, int interface_type);

#define ccsObjectRemoveInterface(o, interface_type) (ccsObjectRemoveInterface_) (&(o)->object, interface_type);

const CCSInterface * ccsObjectGetInterface_ (CCSObject *object, int interface_type);

#define ccsObjectGetInterface(o, interface_type) (ccsObjectGetInterface_) (&(o)->object, interface_type)

#define ccsObjectRef(o) \
    do { ((o)->object).refcnt++; } while (FALSE)

#define ccsObjectUnref(o, freeFunc) \
    do \
    { \
	((o)->object).refcnt--; \
	if (!((o)->object).refcnt) \
	    freeFunc (o); \
    } while (FALSE)

CCSPrivate *
ccsObjectGetPrivate_ (CCSObject *object);

#define ccsObjectGetPrivate(o) (ccsObjectGetPrivate_) (&(o)->object)

void
ccsObjectSetPrivate_ (CCSObject *object, CCSPrivate *priv);

#define ccsObjectSetPrivate(o, priv) (ccsObjectSetPrivate_) (&(o)->object, priv)

void
ccsObjectFinalize_ (CCSObject *object);

#define ccsObjectFinalize(o) (ccsObjectFinalize_) (&(o)->object)

unsigned int
ccsAllocateType ();

#define GET_INTERFACE_TYPE(Interface) \
    ccs##Interface##GetType ()

#define INTERFACE_TYPE(Interface) \
    unsigned int ccs##Interface##GetType () \
    { \
	static unsigned int   type_id = 0; \
	if (!type_id) \
	    type_id = ccsAllocateType (); \
	 \
	return type_id; \
    }

#define GET_INTERFACE(CType, o) (CType *) ccsObjectGetInterface (o, GET_INTERFACE_TYPE(CType))

/**
 * reference counting
 * 
 * ccsSettingRef
 * References the settings object so it can be kept in a list and
 * unreferenced later with freeObj (mixed with objects that need
 * to be freed and need not be freed)
 * 
 * ccsSettingUnref
 * Unreferences the settings object, when the reference count reaches
 * zero, the object is freed
 * 
 */

#define CCSREF_HDR(type,dtype) \
	void ccs##type##Ref (dtype *);  \
	void ccs##type##Unref (dtype *);

CCSREF_HDR (Plugin, CCSPlugin)
CCSREF_HDR (Setting, CCSSetting)
CCSREF_HDR (String, CCSString)
CCSREF_HDR (Group, CCSGroup)
CCSREF_HDR (SubGroup, CCSSubGroup)
CCSREF_HDR (SettingValue, CCSSettingValue)
CCSREF_HDR (PluginConflict, CCSPluginConflict)
CCSREF_HDR (BackendInfo, CCSBackendInfo)
CCSREF_HDR (IntDesc, CCSIntDesc)
CCSREF_HDR (StrRestriction, CCSStrRestriction)
CCSREF_HDR (StrExtension, CCSStrExtension)

typedef struct _CCSInterfaceTable CCSInterfaceTable;
typedef struct _CCSContextInterface CCSContextInterface;
typedef struct _CCSPluginInterface CCSPluginInterface;
typedef struct _CCSSettingInterface CCSSettingInterface;

struct _CCSInterfaceTable
{
    const CCSContextInterface *contextInterface;
    const CCSPluginInterface *pluginInterface;
    const CCSSettingInterface *settingInterface;
};

extern const CCSInterfaceTable ccsDefaultInterfaceTable;

/* CCSContext interface */
typedef CCSPluginList (*CCSContextGetPluginsProc) (CCSContext *context);
typedef CCSPluginCategory * (*CCSContextGetCategories) (CCSContext *context);
typedef CCSSettingList (*CCSContextGetChangedSettings) (CCSContext *context);
typedef unsigned int (*CCSContextGetScreenNum) (CCSContext *context);
typedef Bool (*CCSContextAddChangedSetting) (CCSContext *context, CCSSetting *setting);
typedef Bool (*CCSContextClearChangedSettings) (CCSContext *context);
typedef CCSSettingList (*CCSContextStealChangedSettings) (CCSContext *context);
typedef void * (*CCSContextGetPrivatePtr) (CCSContext *context);
typedef void (*CCSContextSetPrivatePtr) (CCSContext *context, void *ptr);
typedef Bool (*CCSContextLoadPlugin) (CCSContext *context, char *name);
typedef CCSPlugin * (*CCSContextFindPlugin) (CCSContext *context, const char *name);
typedef Bool (*CCSContextPluginIsActive) (CCSContext *context, char *name);
typedef CCSPluginList (*CCSContextGetActivePluginList) (CCSContext *context);
typedef CCSStringList (*CCSContextGetSortedPluginStringList) (CCSContext *context);
typedef Bool (*CCSContextSetBackend) (CCSContext *context, char *name);
typedef char * (*CCSContextGetBackend) (CCSContext *context);
typedef void (*CCSContextSetIntegrationEnabled) (CCSContext *context, Bool value);
typedef void (*CCSContextSetProfile) (CCSContext *context, char *name);
typedef void (*CCSContextSetPluginListAutoSort) (CCSContext *context, Bool value);
typedef char * (*CCSContextGetProfile) (CCSContext *context);
typedef Bool (*CCSContextGetIntegrationEnabled) (CCSContext *context);
typedef Bool (*CCSContextGetPluginListAutoSort) (CCSContext *context);
typedef void (*CCSContextProcessEvents) (CCSContext *context, unsigned int flags);
typedef void (*CCSContextReadSettings) (CCSContext *context);
typedef void (*CCSContextWriteSettings) (CCSContext *context);
typedef void (*CCSContextWriteChangedSettings) (CCSContext *context);
typedef Bool (*CCSContextExportToFile) (CCSContext *context, const char *fileName, Bool skipDefaults);
typedef Bool (*CCSContextImportFromFile) (CCSContext *context, const char *fileName, Bool overwriteNonDefault);
typedef CCSPluginConflictList (*CCSContextCanEnablePlugin) (CCSContext *context, CCSPlugin *plugin);
typedef CCSPluginConflictList (*CCSContextCanDisablePlugin) (CCSContext *context, CCSPlugin *plugin);
typedef CCSStringList (*CCSContextGetExistingProfiles) (CCSContext *context);
typedef void (*CCSContextDeleteProfile) (CCSContext *context, char *name);
typedef Bool (*CCSContextCheckForSettingsUpgrade) (CCSContext *context);
typedef void (*CCSContextLoadPlugins) (CCSContext *context);

struct _CCSContextInterface
{
    CCSContextGetPluginsProc contextGetPlugins;
    CCSContextGetCategories contextGetCategories;
    CCSContextGetChangedSettings contextGetChangedSettings;
    CCSContextGetScreenNum contextGetScreenNum;
    CCSContextAddChangedSetting contextAddChangedSetting;
    CCSContextClearChangedSettings contextClearChangedSettings;
    CCSContextStealChangedSettings contextStealChangedSettings;
    CCSContextGetPrivatePtr contextGetPrivatePtr;
    CCSContextSetPrivatePtr contextSetPrivatePtr;
    CCSContextLoadPlugin contextLoadPlugin;
    CCSContextFindPlugin contextFindPlugin;
    CCSContextPluginIsActive contextPluginIsActive;
    CCSContextGetActivePluginList contextGetActivePluginList;
    CCSContextGetSortedPluginStringList contextGetSortedPluginStringList;
    CCSContextSetBackend contextSetBackend;
    CCSContextGetBackend contextGetBackend;
    CCSContextSetIntegrationEnabled contextSetIntegrationEnabled;
    CCSContextSetProfile contextSetProfile;
    CCSContextSetPluginListAutoSort contextSetPluginListAutoSort;
    CCSContextGetProfile contextGetProfile;
    CCSContextGetIntegrationEnabled contextGetIntegrationEnabled;
    CCSContextGetPluginListAutoSort contextGetPluginListAutoSort;
    CCSContextProcessEvents contextProcessEvents;
    CCSContextReadSettings contextReadSettings;
    CCSContextWriteSettings contextWriteSettings;
    CCSContextWriteChangedSettings contextWriteChangedSettings;
    CCSContextExportToFile contextExportToFile;
    CCSContextImportFromFile contextImportFromFile;
    CCSContextCanEnablePlugin contextCanEnablePlugin;
    CCSContextCanDisablePlugin contextCanDisablePlugin;
    CCSContextGetExistingProfiles contextGetExistingProfiles;
    CCSContextDeleteProfile contextDeleteProfile;
    CCSContextCheckForSettingsUpgrade contextCheckForSettingsUpgrade;
    CCSContextLoadPlugins contextLoadPlugins;
};

unsigned int ccsCCSContextInterfaceGetType ();

/* CCSContext accessor functions */
CCSPluginList ccsContextGetPlugins (CCSContext *);
CCSPluginCategory * ccsContextGetCategories (CCSContext *);
CCSSettingList ccsContextGetChangedSettings (CCSContext *);
unsigned int ccsContextGetScreenNum (CCSContext *);
Bool ccsContextAddChangedSetting (CCSContext *context, CCSSetting *setting);
Bool ccsContextClearChangedSettings (CCSContext *context);
CCSSettingList ccsContextStealChangedSettings (CCSContext *context);
void * ccsContextGetPrivatePtr (CCSContext *context);
void ccsContextSetPrivatePtr (CCSContext *context, void *ptr);

/* only for bindings */
void * ccsContextGetPluginsBindable (CCSContext *context);
void * ccsContextStealChangedSettingsBindable (CCSContext *context);
void * ccsContextGetChangedSettingsBindable (CCSContext *context);

struct _CCSContext
{
    CCSObject object;
};

struct _CCSBackendInfo
{
    char *name;              /* name of the backend */
    char *shortDesc;         /* backend's short description */
    char *longDesc;          /* backend's long description */
    Bool integrationSupport; /* does the backend support DE integration? */
    Bool profileSupport;     /* does the backend support profiles? */
    unsigned int refCount;   /* reference count */
};

/* CCSPluginInterface */
typedef char * (*CCSPluginGetName) (CCSPlugin *plugin);
typedef char * (*CCSPluginGetShortDesc) (CCSPlugin *plugin);
typedef char * (*CCSPluginGetLongDesc) (CCSPlugin *plugin);
typedef char * (*CCSPluginGetHints) (CCSPlugin *plugin);
typedef char * (*CCSPluginGetCategory) (CCSPlugin *plugin);

typedef CCSStringList (*CCSPluginGetLoadAfter) (CCSPlugin *plugin);
typedef CCSStringList (*CCSPluginGetLoadBefore) (CCSPlugin *plugin);
typedef CCSStringList (*CCSPluginGetRequiresPlugins) (CCSPlugin *plugin);
typedef CCSStringList (*CCSPluginGetConflictPlugins) (CCSPlugin *plugin);
typedef CCSStringList (*CCSPluginGetProvidesFeatures) (CCSPlugin *plugin);
typedef CCSStringList (*CCSPluginGetRequiresFeatures) (CCSPlugin *plugin);

typedef void * (*CCSPluginGetPrivatePtr) (CCSPlugin *plugin);
typedef void (*CCSPluginSetPrivatePtr) (CCSPlugin *plugin, void *);

typedef CCSContext * (*CCSPluginGetContext) (CCSPlugin *plugin);

typedef CCSSetting * (*CCSPluginFindSetting) (CCSPlugin *plugin, const char *name);
typedef CCSSettingList (*CCSPluginGetPluginSettings) (CCSPlugin *plugin);
typedef CCSGroupList (*CCSPluginGetPluginGroups) (CCSPlugin *plugin);
typedef void (*CCSPluginReadPluginSettings) (CCSPlugin *plugin);
typedef CCSStrExtensionList (*CCSPluginGetPluginStrExtensions) (CCSPlugin *plugin);

unsigned int ccsCCSPluginInterfaceGetType ();

struct _CCSPluginInterface
{
    CCSPluginGetName pluginGetName;
    CCSPluginGetShortDesc pluginGetShortDesc;
    CCSPluginGetLongDesc pluginGetLongDesc;
    CCSPluginGetHints pluginGetHints;
    CCSPluginGetCategory pluginGetCategory;
    CCSPluginGetLoadAfter pluginGetLoadAfter;
    CCSPluginGetLoadBefore pluginGetLoadBefore;
    CCSPluginGetRequiresPlugins pluginGetRequiresPlugins;
    CCSPluginGetConflictPlugins pluginGetConflictPlugins;
    CCSPluginGetProvidesFeatures pluginGetProvidesFeatures;
    CCSPluginGetRequiresFeatures pluginGetRequiresFeatures;
    CCSPluginGetPrivatePtr pluginGetPrivatePtr;
    CCSPluginSetPrivatePtr pluginSetPrivatePtr;
    CCSPluginGetContext pluginGetContext;
    CCSPluginFindSetting pluginFindSetting;
    CCSPluginGetPluginSettings pluginGetPluginSettings;
    CCSPluginGetPluginGroups pluginGetPluginGroups;
    CCSPluginReadPluginSettings pluginReadPluginSettings;
    CCSPluginGetPluginStrExtensions pluginGetPluginStrExtensions;
};

/* CCSPlugin accessor functions */
char * ccsPluginGetName (CCSPlugin *plugin);
char * ccsPluginGetShortDesc (CCSPlugin *plugin);
char * ccsPluginGetLongDesc (CCSPlugin *plugin);
char * ccsPluginGetHints (CCSPlugin *plugin);
char * ccsPluginGetCategory (CCSPlugin *plugin);

CCSStringList ccsPluginGetLoadAfter (CCSPlugin *plugin);
CCSStringList ccsPluginGetLoadBefore (CCSPlugin *plugin);
CCSStringList ccsPluginGetRequiresPlugins (CCSPlugin *plugin);
CCSStringList ccsPluginGetConflictPlugins (CCSPlugin *plugin);
CCSStringList ccsPluginGetProvidesFeatures (CCSPlugin *plugin);
CCSStringList ccsPluginGetRequiresFeatures (CCSPlugin *plugin);

void * ccsPluginGetPrivatePtr (CCSPlugin *plugin);
void ccsPluginSetPrivatePtr (CCSPlugin *plugin, void *ptr);

CCSContext * ccsPluginGetContext (CCSPlugin *plugin);

void * ccsPluginGetProvidesFeaturesBindable (CCSPlugin *plugin);

struct _CCSPlugin
{
    CCSObject object;
};

typedef enum _CCSSettingType
{
    /* This needs to be in the same order as CompOptionType for consistency */
    TypeBool,
    TypeInt,
    TypeFloat,
    TypeString,
    TypeColor,
    TypeAction,
    TypeKey,
    TypeButton,
    TypeEdge,
    TypeBell,
    TypeMatch,
    TypeList,
    TypeNum
} CCSSettingType;

struct _CCSSubGroup
{
    char           *name;    /* sub group name in current locale */
    CCSSettingList settings; /* list of settings in this sub group */
    unsigned int   refCount;	   /* reference count */
};

struct _CCSGroup
{
    char            *name;     /* group name in current locale */
    CCSSubGroupList subGroups; /* list of sub groups in this group */
    unsigned int    refCount;	   /* reference count */
};

typedef enum _CCSPluginConflictType
{
    /* produced on plugin activation */
    ConflictRequiresPlugin,
    ConflictRequiresFeature,
    ConflictFeature,
    ConflictPlugin,
    /* produced on plugin deactivation */
    ConflictFeatureNeeded,
    ConflictPluginNeeded,
    ConflictPluginError,
} CCSPluginConflictType;

struct _CCSPluginConflict
{
    char *                value;   /* item (plugin / feature) name that
				      caused the conflict */
    CCSPluginConflictType type;    /* type of the conflict */
    CCSPluginList         plugins; /* list of conflicting plugins */
    unsigned int 	  refCount;	   /* reference count */	
};

union _CCSSettingInfo;

struct _CCSString
{
    char 	 *value;
    unsigned int refCount;
};

struct _CCSIntDesc
{
    int  value; /* value the description is assigned to */
    char *name; /* description */
    unsigned int refCount;	   /* reference count */
};

struct _CCSStrRestriction
{
    char *value; /* value the restriction is assigned to */
    char *name;  /* description */
    unsigned int refCount;	   /* reference count */
};

struct _CCSStrExtension
{
    char *basePlugin;           /* plugin this extension extends */
    CCSStringList baseSettings; /* list of settings this extension extends */
    CCSStrRestrictionList restriction; /* list of added restriction items */
    unsigned int refCount;	   /* reference count */
};

typedef struct _CCSSettingIntInfo
{
    int            min;  /* minimum value for this setting */
    int            max;  /* maximum value */
    CCSIntDescList desc; /* list of item descriptions */
} CCSSettingIntInfo;

typedef struct _CCSSettingFloatInfo
{
    float min;       /* minimum value for this setting */
    float max;       /* maximum value */
    float precision; /* precision (allowed increment) */
} CCSSettingFloatInfo;

typedef struct _CCSSettingStringInfo
{
    CCSStrRestrictionList restriction;  /* list of restriction items */
    int                sortStartsAt; /* the restriction index to start sorting
					at (defaults to -1 for no sorting) */
    Bool               extensible;   /* whether extension is allowed for
					this setting */	
} CCSSettingStringInfo;

typedef struct _CCSSettingListInfo
{
    CCSSettingType        listType;  /* type of setting this list contains */
    union _CCSSettingInfo *listInfo; /* list of settings */
} CCSSettingListInfo;

typedef struct _CCSSettingActionInfo
{
    Bool internal; /* is this binding global or plugin internal*/
} CCSSettingActionInfo;

typedef union _CCSSettingInfo
{
    CCSSettingIntInfo    forInt;
    CCSSettingFloatInfo  forFloat;
    CCSSettingStringInfo forString;
    CCSSettingListInfo   forList;
    CCSSettingActionInfo forAction;
} CCSSettingInfo;

typedef struct _CCSSettingColorValueColor
{
    unsigned short red;
    unsigned short green;
    unsigned short blue;
    unsigned short alpha;
}

CCSSettingColorValueColor;

typedef struct _CCSSettingColorValueArray
{
    unsigned short array[4];
}

CCSSettingColorValueArray;

typedef union _CCSSettingColorValue
{
    CCSSettingColorValueColor color;
    CCSSettingColorValueArray array;
} CCSSettingColorValue;


typedef struct _CCSSettingKeyValue
{
    int          keysym;
    unsigned int keyModMask;
} CCSSettingKeyValue;

typedef struct _CCSSettingButtonValue
{
    int          button;
    unsigned int buttonModMask;
    unsigned int edgeMask;
} CCSSettingButtonValue;

typedef union _CCSSettingValueUnion
{
    Bool		  asBool;
    int			  asInt;
    float		  asFloat;
    char *		  asString;
    char *		  asMatch;
    CCSSettingColorValue  asColor;
    CCSSettingValueList   asList;
    CCSSettingKeyValue    asKey;
    CCSSettingButtonValue asButton;
    unsigned int	  asEdge;
    Bool		  asBell;
} CCSSettingValueUnion;

struct _CCSSettingValue
{
    CCSSettingValueUnion value;
    CCSSetting *	 parent;
    Bool		 isListChild;
    unsigned int	 refCount;	   /* reference count */
};

/* Interface for CCSSetting */
char * ccsSettingGetName (CCSSetting *setting);
char * ccsSettingGetShortDesc (CCSSetting *setting);
char * ccsSettingGetLongDesc (CCSSetting *setting);
CCSSettingType ccsSettingGetType (CCSSetting *setting);
CCSSettingInfo * ccsSettingGetInfo (CCSSetting *setting);
char * ccsSettingGetGroup (CCSSetting *setting);
char * ccsSettingGetSubGroup (CCSSetting *setting);
char * ccsSettingGetHints (CCSSetting *setting);
CCSSettingValue * ccsSettingGetDefaultValue (CCSSetting *setting);
CCSSettingValue *ccsSettingGetValue (CCSSetting *setting);
Bool ccsSettingGetIsDefault (CCSSetting *setting);
CCSPlugin * ccsSettingGetParent (CCSSetting *setting);
void * ccsSettingGetPrivatePtr (CCSSetting *setting);
void ccsSettingSetPrivatePtr (CCSSetting *setting, void *ptr);

/* Setting getters. Returns TRUE if the setting value was successfully
   copied into <data>, FALSE otherwise. */
Bool ccsGetInt (CCSSetting *setting,
		int        *data);
Bool ccsGetFloat (CCSSetting *setting,
		  float      *data);
Bool ccsGetBool (CCSSetting *setting,
		 Bool       *data);
Bool ccsGetString (CCSSetting *setting,
		   char       **data);
Bool ccsGetColor (CCSSetting           *setting,
		  CCSSettingColorValue *data);
Bool ccsGetMatch (CCSSetting *setting,
		  char       **data);
Bool ccsGetKey (CCSSetting         *setting,
		CCSSettingKeyValue *data);
Bool ccsGetButton (CCSSetting            *setting,
		   CCSSettingButtonValue *data);
Bool ccsGetEdge (CCSSetting  *setting,
		 unsigned int *data);
Bool ccsGetBell (CCSSetting *setting,
		 Bool       *data);
Bool ccsGetList (CCSSetting          *setting,
		 CCSSettingValueList *data);

/* Setting setters. Set <setting> to value <data>. Return TRUE if new value
   matches data. If the new value doesn't match the old value, the setting
   is added to the context's changedSettings list. */
Bool ccsSetInt (CCSSetting *setting,
		int        data,
		Bool	   processChanged);
Bool ccsSetFloat (CCSSetting *setting,
		  float      data,
		  Bool	     processChanged);
Bool ccsSetBool (CCSSetting *setting,
		 Bool       data,
		 Bool	    processChanged);
Bool ccsSetString (CCSSetting *setting,
		   const char *data,
		   Bool	      processChanged);
Bool ccsSetColor (CCSSetting           *setting,
		  CCSSettingColorValue data,
		  Bool		       processChanged);
Bool ccsSetMatch (CCSSetting *setting,
		  const char *data,
		  Bool	     processChanged);
Bool ccsSetKey (CCSSetting         *setting,
		CCSSettingKeyValue data,
		Bool		   processChanged);
Bool ccsSetButton (CCSSetting            *setting,
		   CCSSettingButtonValue data,
		   Bool			 processChanged);
Bool ccsSetEdge (CCSSetting   *setting,
		 unsigned int data,
		 Bool	      processChanged);
Bool ccsSetBell (CCSSetting *setting,
		 Bool       data,
		 Bool	    processChanged);
Bool ccsSetList (CCSSetting          *setting,
		 CCSSettingValueList data,
		 Bool	 processChanged);
Bool ccsSetValue (CCSSetting      *setting,
		  CCSSettingValue *data,
		  Bool		  processChanged);

/* Reset all settings to defaults. Settings that were non-default
   previously are added to the changedSettings list of the context. */
void ccsResetToDefault (CCSSetting * setting, Bool processChanged);

/* Checks if a given setting is integrated in the desktop environment. */
Bool ccsSettingIsIntegrated (CCSSetting *setting);

/* Checks if a given setting is read-only. */
Bool ccsSettingIsReadOnly (CCSSetting *setting);

typedef char * (*CCSSettingGetName) (CCSSetting *);
typedef char * (*CCSSettingGetShortDesc) (CCSSetting *);
typedef char * (*CCSSettingGetLongDesc) (CCSSetting *);
typedef CCSSettingType (*CCSSettingGetType) (CCSSetting *);
typedef CCSSettingInfo * (*CCSSettingGetInfo) (CCSSetting *);
typedef char * (*CCSSettingGetGroup) (CCSSetting *);
typedef char * (*CCSSettingGetSubGroup) (CCSSetting *);
typedef char * (*CCSSettingGetHints) (CCSSetting *);
typedef CCSSettingValue * (*CCSSettingGetDefaultValue) (CCSSetting *setting);
typedef CCSSettingValue * (*CCSSettingGetValue) (CCSSetting *setting);
typedef Bool (*CCSSettingGetIsDefault) (CCSSetting *);
typedef CCSPlugin * (*CCSSettingGetParent) (CCSSetting *);
typedef void * (*CCSSettingGetPrivatePtr) (CCSSetting *);
typedef void (*CCSSettingSetPrivatePtr) (CCSSetting *, void *);
typedef Bool (*CCSSettingSetInt) (CCSSetting *setting, int data, Bool processChanged);
typedef Bool (*CCSSettingSetFloat) (CCSSetting *setting, float data, Bool processChanged);
typedef Bool (*CCSSettingSetBool) (CCSSetting *setting, Bool data, Bool processChanged);
typedef Bool (*CCSSettingSetString) (CCSSetting *setting, const char * data, Bool processChanged);
typedef Bool (*CCSSettingSetColor) (CCSSetting *setting, CCSSettingColorValue data, Bool processChanged);
typedef Bool (*CCSSettingSetMatch) (CCSSetting *setting, const char * data, Bool processChanged);
typedef Bool (*CCSSettingSetKey) (CCSSetting *setting, CCSSettingKeyValue data, Bool processChanged);
typedef Bool (*CCSSettingSetButton) (CCSSetting *setting, CCSSettingButtonValue data, Bool processChanged);
typedef Bool (*CCSSettingSetEdge) (CCSSetting *setting, unsigned int data, Bool processChanged);
typedef Bool (*CCSSettingSetBell) (CCSSetting *setting, Bool data, Bool processChanged);
typedef Bool (*CCSSettingSetList) (CCSSetting *setting, CCSSettingValueList data, Bool processChanged);
typedef Bool (*CCSSettingSetValue) (CCSSetting *setting, CCSSettingValue *data, Bool processChanged);
typedef Bool (*CCSSettingGetInt) (CCSSetting *setting, int *data);
typedef Bool (*CCSSettingGetFloat) (CCSSetting *setting, float *data);
typedef Bool (*CCSSettingGetBool) (CCSSetting *setting, Bool *data);
typedef Bool (*CCSSettingGetString) (CCSSetting *setting, char **data);
typedef Bool (*CCSSettingGetColor) (CCSSetting *setting, CCSSettingColorValue *data);
typedef Bool (*CCSSettingGetMatch) (CCSSetting *setting, char **data);
typedef Bool (*CCSSettingGetKey) (CCSSetting *setting, CCSSettingKeyValue *data);
typedef Bool (*CCSSettingGetButton) (CCSSetting *setting, CCSSettingButtonValue *data);
typedef Bool (*CCSSettingGetEdge) (CCSSetting *setting, unsigned int *data);
typedef Bool (*CCSSettingGetBell) (CCSSetting *setting, Bool *data);
typedef Bool (*CCSSettingGetList) (CCSSetting *setting, CCSSettingValueList *data);
typedef void (*CCSSettingResetToDefault) (CCSSetting *setting, Bool processChanged);
typedef Bool (*CCSSettingIsIntegrated) (CCSSetting *setting);
typedef Bool (*CCSSettingIsReadOnly) (CCSSetting *setting);

unsigned int ccsCCSSettingInterfaceGetType ();

struct _CCSSettingInterface
{
    CCSSettingGetName settingGetName;
    CCSSettingGetShortDesc settingGetShortDesc;
    CCSSettingGetLongDesc settingGetLongDesc;
    CCSSettingGetType settingGetType;
    CCSSettingGetInfo settingGetInfo;
    CCSSettingGetGroup settingGetGroup;
    CCSSettingGetSubGroup settingGetSubGroup;
    CCSSettingGetHints settingGetHints;
    CCSSettingGetDefaultValue settingGetDefaultValue;
    CCSSettingGetValue settingGetValue;
    CCSSettingGetIsDefault settingGetIsDefault;
    CCSSettingGetParent settingGetParent;
    CCSSettingGetPrivatePtr settingGetPrivatePtr;
    CCSSettingSetPrivatePtr settingSetPrivatePtr;
    CCSSettingSetInt settingSetInt;
    CCSSettingSetFloat settingSetFloat;
    CCSSettingSetBool settingSetBool;
    CCSSettingSetString settingSetString;
    CCSSettingSetColor settingSetColor;
    CCSSettingSetMatch settingSetMatch;
    CCSSettingSetKey settingSetKey;
    CCSSettingSetButton settingSetButton;
    CCSSettingSetEdge settingSetEdge;
    CCSSettingSetBell settingSetBell;
    CCSSettingSetList settingSetList;
    CCSSettingSetValue settingSetValue;
    CCSSettingGetInt settingGetInt;
    CCSSettingGetFloat settingGetFloat;
    CCSSettingGetBool settingGetBool;
    CCSSettingGetString settingGetString;
    CCSSettingGetColor settingGetColor;
    CCSSettingGetMatch settingGetMatch;
    CCSSettingGetKey settingGetKey;
    CCSSettingGetButton settingGetButton;
    CCSSettingGetEdge settingGetEdge;
    CCSSettingGetBell settingGetBell;
    CCSSettingGetList settingGetList;
    CCSSettingResetToDefault settingResetToDefault;
    CCSSettingIsIntegrated settingIsIntegrated;
    CCSSettingIsReadOnly settingIsReadOnly;
};

struct _CCSSetting
{
    CCSObject object;
};

struct _CCSPluginCategory
{
    const char *name;      /* plugin category name */
    const char *shortDesc; /* plugin category short description */
    const char *longDesc;  /* plugin category long description */

    CCSStringList plugins; /* list of plugins in this category */
    unsigned int refCount;	   /* reference count */
};

/* set basic metadata to TRUE and no additional
   metadata informations will be parsed */
void ccsSetBasicMetadata (Bool value);

/* Creates a new context for the given screen.
   All plugin settings are automatically enumerated. */
CCSContext* ccsContextNew (unsigned int screenNum, const CCSInterfaceTable *);

/* Creates a new context without auto-enumerating any plugin or setting.
   Behaves otherwise exactly like ccsContextNew. */
CCSContext* ccsEmptyContextNew (unsigned int screenNum, const CCSInterfaceTable *);

/* Destroys the allocated context. */
void ccsContextDestroy (CCSContext * context);

/* Load the plugin and setting metadata for a given plugin.
   Returns TRUE on success, FALSE otherwise. */
Bool ccsLoadPlugin (CCSContext *context,
		    char       *name);

/* Searches for a plugin identified by its name in the context.
   Returns the plugin struct if it could be found, NULL otherwise. */
CCSPlugin* ccsFindPlugin (CCSContext *context,
			  const char *name);

/* Searches for a setting in a plugin.
   Returns the setting struct if the search was successful (setting with
   name <name> found), NULL otherwise. */
CCSSetting* ccsFindSetting (CCSPlugin    *plugin,
			    const char   *name);

/* Returns TRUE if the named plugin is in the context and marked as currently
   active in Compiz, FALSE otherwise. */
Bool ccsPluginIsActive (CCSContext *context,
			char       *name);

void ccsFreeContext (CCSContext *context);
void ccsFreePlugin (CCSPlugin *plugin);
void ccsFreeSetting (CCSSetting *setting);
void ccsFreeGroup (CCSGroup *group);
void ccsFreeSubGroup (CCSSubGroup *subGroup);
void ccsFreeSettingValue (CCSSettingValue *value);
void ccsFreePluginConflict (CCSPluginConflict *value);
void ccsFreeBackendInfo (CCSBackendInfo *value);
void ccsFreeIntDesc (CCSIntDesc *value);
void ccsFreeStrRestriction (CCSStrRestriction *restriction);
void ccsFreeStrExtension (CCSStrExtension *extension);
void ccsFreeString (CCSString *str);



/* Compares two setting values. Returns TRUE if values match,
   FALSE otherwise. */
Bool ccsIsEqualColor (CCSSettingColorValue c1,
		      CCSSettingColorValue c2);
Bool ccsIsEqualKey (CCSSettingKeyValue c1,
		    CCSSettingKeyValue c2);
Bool ccsIsEqualButton (CCSSettingButtonValue c1,
		       CCSSettingButtonValue c2);

/* Retrieves a list of settings in a plugin */
CCSSettingList ccsGetPluginSettings (CCSPlugin *plugin);

/* Retrieves a list of setting groups in a plugin */
CCSGroupList ccsGetPluginGroups (CCSPlugin *plugin);

/* Converts a string list into a list of string settings.
   Return value needs to be freed by the caller. */
CCSSettingValueList ccsGetValueListFromStringList (CCSStringList list,
						   CCSSetting    *parent);
/* Converts a string setting value list into a string list.
   Return value needs to be freed by the caller. */
CCSStringList ccsGetStringListFromValueList (CCSSettingValueList list);

/* Converts a string list into a string array. If the return value is not
   NULL, the item count is copied into <num>. Return value needs to be freed
   by the caller. */
char** ccsGetStringArrayFromList (CCSStringList list,
				  int           *num);
/* Converts a string array with <num> items into a string list. Return value
   needs to be freed by the caller. */
CCSStringList ccsGetListFromStringArray (char **array,
					 int  num);

/* Converts a setting value list into an array of the setting item data type.
   Behaves similar to ccsGetStringArrayFromList. */
char** ccsGetStringArrayFromValueList (CCSSettingValueList list,
				       int                 *num);
char** ccsGetMatchArrayFromValueList (CCSSettingValueList list,
				      int                 *num);

int* ccsGetIntArrayFromValueList (CCSSettingValueList list,
				  int                 *num);
float* ccsGetFloatArrayFromValueList (CCSSettingValueList list,
				      int                 *num);
Bool * ccsGetBoolArrayFromValueList (CCSSettingValueList list,
				     int                 *num);
CCSSettingColorValue* ccsGetColorArrayFromValueList (CCSSettingValueList list,
	       					     int                 *num);

/* Converts an array of data items to a setting value list. Behaves similar
   to ccsGetListFromStringArray */
CCSSettingValueList ccsGetValueListFromStringArray (char       **array,
						    int        num,
						    CCSSetting *parent);
CCSSettingValueList ccsGetValueListFromMatchArray (char       **array,
						   int        num,
						   CCSSetting *parent);
CCSSettingValueList ccsGetValueListFromIntArray (int        *array,
						 int        num,
						 CCSSetting *parent);
CCSSettingValueList ccsGetValueListFromFloatArray (float      *array,
						   int        num,
						   CCSSetting *parent);
CCSSettingValueList ccsGetValueListFromBoolArray (Bool       *array,
						  int        num,
						  CCSSetting *parent);
CCSSettingValueList ccsGetValueListFromColorArray (CCSSettingColorValue *array,
						   int                  num,
						   CCSSetting           *parent);

/* Retrieves a list of plugins marked as active in Compiz for this context */
CCSPluginList ccsGetActivePluginList (CCSContext *context);

/* Retrieves a list of plugin names which are active in Compiz for a given
   context, sorted as needed according to load after/before/etc. rules */
CCSStringList ccsGetSortedPluginStringList (CCSContext *context);

/* Switches the backend for a context. Returns TRUE on successful switch,
   FALSE otherwise. */
Bool ccsSetBackend (CCSContext *context,
		    char       *name);
/* Retrieves the name of the backend active for the context. */
char * ccsGetBackend (CCSContext *context);

/* Enable/disable DE integration for a context. */
void ccsSetIntegrationEnabled (CCSContext *context,
			       Bool       value);

/* Sets the profile for a context. */
void ccsSetProfile (CCSContext *context,
		    char       *name);

/* Set plugin list autosort for a context. */
void ccsSetPluginListAutoSort (CCSContext *context,
			       Bool       value);

/* Retrieve current profile of the context. */
char * ccsGetProfile (CCSContext *context);

/* Retrieves current DE integration status for a context */
Bool ccsGetIntegrationEnabled (CCSContext *context);

/* Retrieves the autosort setting for a context. */
Bool ccsGetPluginListAutoSort (CCSContext *context);

/* Changes the plugin activeness status in compiz. If plugin list autosort
   is enabled, automatically writes a new sorted plugin list to the
   active_plugins setting. If autosort is disabled, it's up to the caller
   to do that. */
Bool ccsPluginSetActive (CCSPlugin *plugin,
			 Bool      value);

/* functions parsing/creating an action string -
   the returned strings must be free'd after usage! */

char * ccsModifiersToString (unsigned int modMask);

char * ccsEdgesToString (unsigned int edge);

char * ccsEdgesToModString (unsigned int edge);

char * ccsKeyBindingToString (CCSSettingKeyValue *key);

char * ccsButtonBindingToString (CCSSettingButtonValue *button);

char * ccsColorToString (CCSSettingColorValue *color);

unsigned int ccsStringToModifiers (const char *binding);

unsigned int ccsStringToEdges (const char *edge);

unsigned int ccsModStringToEdges (const char *edge);

Bool ccsStringToKeyBinding (const char         *binding,
			    CCSSettingKeyValue *key);

Bool ccsStringToButtonBinding (const char            *binding,
			       CCSSettingButtonValue *button);

Bool ccsStringToColor (const char           *value,
		       CCSSettingColorValue *color);

/* flag values for ccsProcessEvents */
#define ProcessEventsNoGlibMainLoopMask (1 << 0)

void ccsProcessEvents (CCSContext   *context,
		       unsigned int flags);

/* Read all setting values from disk */
void ccsReadSettings (CCSContext *context);

/* Read setting values for a given plugin */
void ccsReadPluginSettings (CCSPlugin *plugin);

/* Write all settings to disk */
void ccsWriteSettings (CCSContext *context);

/* Write changed settings to disk */
void ccsWriteChangedSettings (CCSContext *context);

/* Exports a profile to a file. If skipDefaults is TRUE, only exports
   non-default settings. Returns TRUE on successful export, FALSE otherwise. */
Bool ccsExportToFile (CCSContext *context,
		      const char *fileName,
		      Bool skipDefaults);

/* Imports a profile from a file. If overwriteNonDefault is TRUE, also
   overwrites settings that were non-default before. Returns TRUE on success,
   FALSE otherwise. */
Bool ccsImportFromFile (CCSContext *context,
			const char *fileName,
			Bool       overwriteNonDefault);

/* File watch stuff */

typedef void (*FileWatchCallbackProc) (unsigned int watchId, void *closure);

unsigned int ccsAddFileWatch (const char            *fileName,
			      Bool                  enable,
			      FileWatchCallbackProc callback,
			      void                  *closure);

void ccsRemoveFileWatch (unsigned int watchId);
void ccsDisableFileWatch (unsigned int watchId);
void ccsEnableFileWatch (unsigned int watchId);

/* INI file stuff
 * FIXME: This should not be part of the
 * public API */

typedef struct _dictionary_
{
    /** Number of entries in dictionary */
    int n;
    /** Storage size */
    int size;
    /** List of string values */
    char **val;
    /** List of string keys */
    char **key ;
    /** List of hash values for keys */
    unsigned *hash;
} IniDictionary;

IniDictionary* ccsIniNew (void);
IniDictionary* ccsIniOpen (const char *fileName);
void ccsIniClose (IniDictionary *dictionary);
void ccsIniSave (IniDictionary *dictionary,
		 const char    *fileName);

Bool ccsCreateDirFor (const char *fileName);

Bool ccsIniParseString (char        *str,
			char	    **value);
Bool ccsIniGetString (IniDictionary *dictionary,
		      const char    *section,
		      const char    *entry,
		      char          **value);

Bool ccsIniParseInt (const char  *str,
		     int	 *value);
Bool ccsIniGetInt (IniDictionary *dictionary,
		   const char    *section,
		   const char    *entry,
		   int           *value);

Bool ccsIniParseFloat (const char  *str,
		       float	   *value);
Bool ccsIniGetFloat (IniDictionary *dictionary,
		     const char    *section,
		     const char    *entry,
		     float         *value);

Bool ccsIniParseBool (const char  *str,
		      Bool	  *value);
Bool ccsIniGetBool (IniDictionary *dictionary,
		    const char    *section,
		    const char    *entry,
		    Bool          *value);

Bool ccsIniParseColor (const char 	    *str,
		       CCSSettingColorValue *value);
Bool ccsIniGetColor (IniDictionary        *dictionary,
		     const char           *section,
		     const char           *entry,
		     CCSSettingColorValue *value);

Bool ccsIniParseKey (const char		*str,
		     CCSSettingKeyValue *value);
Bool ccsIniGetKey (IniDictionary        *dictionary,
		   const char           *section,
		   const char           *entry,
		   CCSSettingKeyValue   *value);

Bool ccsIniParseButton (const char	      *str,
			CCSSettingButtonValue *value);
Bool ccsIniGetButton (IniDictionary         *dictionary,
		      const char            *section,
		      const char            *entry,
		      CCSSettingButtonValue *value);

Bool ccsIniParseEdge (const char  *str,
		      unsigned int *value);
Bool ccsIniGetEdge (IniDictionary *dictionary,
		    const char    *section,
		    const char    *entry,
		    unsigned int  *value);
	
Bool ccsIniGetBell (IniDictionary *dictionary,
		    const char    *section,
		    const char    *entry,
		    Bool          *value);

Bool ccsIniParseList (const char	  *str,
		      CCSSettingValueList *value,
		      CCSSetting	  *parent);
Bool ccsIniGetList (IniDictionary       *dictionary,
		    const char          *section,
		    const char          *entry,
		    CCSSettingValueList *value,
		    CCSSetting          *parent);

void ccsIniSetString (IniDictionary *dictionary,
		      const char    *section,
		      const char    *entry,
		      char          *value);
void ccsIniSetInt (IniDictionary *dictionary,
		   const char    *section,
		   const char    *entry,
		   int           value);
void ccsIniSetFloat (IniDictionary *dictionary,
		     const char    *section,
		     const char    *entry,
		     float         value);
void ccsIniSetBool (IniDictionary *dictionary,
		    const char    *section,
		    const char    *entry,
		    Bool          value);
void ccsIniSetColor (IniDictionary        *dictionary,
		     const char           *section,
		     const char           *entry,
		     CCSSettingColorValue value);
void ccsIniSetKey (IniDictionary      *dictionary,
		   const char         *section,
		   const char         *entry,
		   CCSSettingKeyValue value);
void ccsIniSetButton (IniDictionary         *dictionary,
		      const char            *section,
		      const char            *entry,
		      CCSSettingButtonValue value);
void ccsIniSetEdge (IniDictionary *dictionary,
		     const char   *section,
		     const char   *entry,
		     unsigned int value);
void ccsIniSetBell (IniDictionary *dictionary,
		    const char    *section,
		    const char    *entry,
		    Bool          value);
void ccsIniSetList (IniDictionary       *dictionary,
		    const char          *section,
		    const char          *entry,
		    CCSSettingValueList value,
		    CCSSettingType      listType);

void ccsIniRemoveEntry (IniDictionary *dictionary,
			const char    *section,
			const char    *entry);

/* Checks if a plugin can be enabled. Returns a list of conflicts that
   would occur when loading the plugin. A return value of NULL means that
   the plugin can be enabled without problems. */
CCSPluginConflictList ccsCanEnablePlugin (CCSContext *context,
					  CCSPlugin  *plugin);

/* Checks if a plugin can be disabled. The meaning of the return value is the
   same as for ccsCanEnablePlugin */
CCSPluginConflictList ccsCanDisablePlugin (CCSContext *context,
					   CCSPlugin *plugin);

/* Enumerates the available profiles for the current backend. */
CCSStringList ccsGetExistingProfiles (CCSContext * context);

/* Deletes the profile with the given name. */
void ccsDeleteProfile (CCSContext *context,
		       char       *name);

/* Enumerates the available backends. */
CCSBackendInfoList ccsGetExistingBackends (void);

CCSStrExtensionList ccsGetPluginStrExtensions (CCSPlugin *plugin);

#ifdef __cplusplus
}
#endif

#endif