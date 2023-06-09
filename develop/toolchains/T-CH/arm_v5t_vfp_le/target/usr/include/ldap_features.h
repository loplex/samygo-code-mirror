/* include/ldap_features.h.  Generated automatically by configure.  */
/* $OpenLDAP: pkg/ldap/include/ldap_features.h.in,v 1.13.2.1 2003/02/09 17:02:17 kurt Exp $ */
/*
 * Copyright 1998-2003 The OpenLDAP Foundation, Redwood City, California, USA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted only as authorized by the OpenLDAP
 * Public License.  A copy of this license is available at
 * http://www.OpenLDAP.org/license.html or in file LICENSE in the
 * top-level directory of the distribution.
 */

/* 
 * LDAP Features
 */

#ifndef _LDAP_FEATURES_H
#define _LDAP_FEATURES_H 1

/* OpenLDAP API version macros */
#define LDAP_VENDOR_VERSION 20130
#define LDAP_VENDOR_VERSION_MAJOR 2
#define LDAP_VENDOR_VERSION_MINOR 1
#define LDAP_VENDOR_VERSION_PATCH 30

/*
** WORK IN PROGRESS!
**
** OpenLDAP reentrancy/thread-safeness should be dynamically
** checked using ldap_get_option().
**
** The -lldap implementation is not thread-safe.
**
** The -lldap_r implementation is:
**		LDAP_API_FEATURE_THREAD_SAFE (basic thread safety)
** but also be:
**		LDAP_API_FEATURE_SESSION_THREAD_SAFE
**		LDAP_API_FEATURE_OPERATION_THREAD_SAFE
**
** The preprocessor flag LDAP_API_FEATURE_X_OPENLDAP_THREAD_SAFE
** can be used to determine if -lldap_r is available at compile
** time.  You must define LDAP_THREAD_SAFE if and only if you
** link with -lldap_r.
**
** If you fail to define LDAP_THREAD_SAFE when linking with
** -lldap_r or define LDAP_THREAD_SAFE when linking with -lldap,
** provided header definations and declarations may be incorrect.
**
*/

/* is -lldap_r available or not */
#define LDAP_API_FEATURE_X_OPENLDAP_THREAD_SAFE 1

/* LDAP v2 Kerberos Bind */
/* #undef LDAP_API_FEATURE_X_OPENLDAP_V2_KBIND */

/* LDAP v2 Referrals */
/* #undef LDAP_API_FEATURE_X_OPENLDAP_V2_REFERRALS */

/* LDAP Server Side Sort. */
#define LDAP_API_FEATURE_SERVER_SIDE_SORT 1000

/* LDAP Virtual List View. */ 
#define LDAP_API_FEATURE_VIRTUAL_LIST_VIEW 1000

#endif /* LDAP_FEATURES */
