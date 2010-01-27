/*
** i18n.h: the i18n header file.
**
** Copyright (C) 2007-2008 SKY-MOBI AS.  All rights reserved.
**
** This file is part of the simple gui library.
** It may not be redistributed under any circumstances.
*/

#ifndef _SGL_I18N_H
#define _SGL_I18N_H

#include "types.h"


#ifdef __cplusplus
extern "C"{
#endif  /* __cplusplus */


#define SGL_MAX_LNG_COUNT		39


//include the string ids
#include "res_str.h"


/**
 * \defgroup resource Resource Management
 * @{
 */

/**
  \defgroup i18n Internationalization and Strings Management.

  Strings management sub system includes two componets:
  - Excel managemented string resource (see the image).
	Fill all the application strings resource in the excel, and give each of them a ID.
	The language columns can removed or added as needed. After edit the excel,
	Click the button "CodeGen" to generate the code.
  - Some apis used by the application.
	  \code
		#include "i18n.h"
		...
		//has string id "STR_OK"
		PWCSTR str = SGL_LoadString(STR_OK);
	  \endcode

  <br><br>
  \image html strings.gif "Excel Manangement String Resource"

  @ingroup resource
  @{
 */
	 
/**
 * \brief Initialize the i18n sub system.
 *
 * This function must be called before any other i18n functions. The framework will call for you when system bring up.
 * so do not care about it.
 */
VOID GUIAPI SGL_I18nInitialize(VOID);

/**
 * \brief Set the current language, this does not affect the strings have loaded by SGL_LoadString.
 *
 * \param lang the new language identifier
 * \return 
 * 	- TRUE on success
 * 	- FALSE otherwise
 * \sa SGL_LoadString SGL_GetCurrentLanguage
 */
BOOL GUIAPI SGL_SetCurrentLanguage(DWORD lang);

/**
 * \brief Get the current language in use.
 *
 * \return the current language
 * \sa SGL_SetCurrentLanguage
 */
DWORD GUIAPI SGL_GetCurrentLanguage(VOID);

/**
 * \brief Get the string by the string id acordding to the current language setting.
 *
 * \param str the string id
 * \return 
 * 	- the data of the string in unicode formatted
 * 	- NULL when the str invalid
 */
PCWSTR GUIAPI SGL_LoadString(DWORD str);

/**
 * \brief Release the memory resource for language lang
 *
 * \param lang the language
 * \return TRUE on success, FALSE otherwise
 */
BOOL GUIAPI SGL_ReleaseResource(DWORD lang);

/** @} end of i18n  */

/** @} end of resource */

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _SGL_I18N_H */
