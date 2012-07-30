#ifndef _COMPIZ_GSETTINGS_UTIL_H
#define _COMPIZ_GSETTINGS_UTIL_H

#include <glib.h>
#include <glib-object.h>
#include <ccs.h>

gchar *
getSchemaNameForPlugin (const char *plugin);

char *
truncateKeyForGSettings (const char *gsettingName);

char *
translateUnderscoresToDashesForGSettings (const char *truncated);

void
translateToLowercaseForGSettings (char *name);

gchar *
translateKeyForGSettings (const char *gsettingName);

gchar *
translateKeyForCCS (const char *gsettingName);

gboolean
compizconfigTypeHasVariantType (CCSSettingType t);

gboolean
decomposeGSettingsPath (const char *path,
			char **pluginName,
			unsigned int *screenNum);

gboolean
variantIsValidForCCSType (GVariant *gsettingsValue,
			  CCSSettingType settingType);

Bool
appendToPluginsWithSetKeysList (const gchar    *plugin,
				GVariant       *writtenPlugins,
				char	       ***newWrittenPlugins,
				gsize	       *newWrittenPluginsSize);

GObject *
findObjectInListWithPropertySchemaName (const gchar *schemaName,
					GList	    *iter);

CCSSettingList
filterAllSettingsMatchingType (CCSSettingType type,
			       CCSSettingList settingList);

CCSSettingList
filterAllSettingsMatchingPartOfStringIgnoringDashesUnderscoresAndCase (const gchar *keyName,
								       CCSSettingList sList);

CCSSetting *
attemptToFindCCSSettingFromLossyName (CCSSettingList settingList, const gchar *lossyName, CCSSettingType type);

GList *
variantTypeToPossibleSettingType (const gchar *vt);

gchar *
makeCompizProfilePath (const gchar *profilename);

gchar *
makeCompizPluginPath (const gchar *profileName, const gchar *pluginName);

#endif