/*
 * Copyright (C) 2003, 2004, 2005 Free Software Foundation
 *
 * Author: Nikos Mavroyanopoulos
 *
 * This file is part of GNUTLS.
 *
 * The GNUTLS library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301
 * USA
 *
 */

#ifndef GNUTLS_PKCS12_H
# define GNUTLS_PKCS12_H

#ifdef __cplusplus
extern "C" {
#endif

#include <gnutls/x509.h>

/* PKCS12 structures handling 
 */
struct gnutls_pkcs12_int;
typedef struct gnutls_pkcs12_int* gnutls_pkcs12_t;

struct gnutls_pkcs12_bag_int;
typedef struct gnutls_pkcs12_bag_int* gnutls_pkcs12_bag_t;

int gnutls_pkcs12_init(gnutls_pkcs12_t * pkcs12);
void gnutls_pkcs12_deinit(gnutls_pkcs12_t pkcs12);
int gnutls_pkcs12_import(gnutls_pkcs12_t pkcs12, const gnutls_datum_t * data,
    gnutls_x509_crt_fmt format, unsigned int flags);
int gnutls_pkcs12_export( gnutls_pkcs12_t pkcs12,
    gnutls_x509_crt_fmt format, void* output_data, size_t* output_data_size);

int gnutls_pkcs12_get_bag(gnutls_pkcs12_t pkcs12, 
    int indx, gnutls_pkcs12_bag_t bag);
int gnutls_pkcs12_set_bag(gnutls_pkcs12_t pkcs12, gnutls_pkcs12_bag_t bag);

int gnutls_pkcs12_generate_mac(gnutls_pkcs12_t pkcs12, const char* pass);
int gnutls_pkcs12_verify_mac(gnutls_pkcs12_t pkcs12, const char* pass);

int gnutls_pkcs12_bag_decrypt(gnutls_pkcs12_bag_t bag, const char* pass);
int gnutls_pkcs12_bag_encrypt(gnutls_pkcs12_bag_t bag, const char* pass, unsigned int flags);

typedef enum gnutls_pkcs12_bag_type_t {
    GNUTLS_BAG_EMPTY = 0,

    GNUTLS_BAG_PKCS8_ENCRYPTED_KEY=1,
    GNUTLS_BAG_PKCS8_KEY,
    GNUTLS_BAG_CERTIFICATE,
    GNUTLS_BAG_CRL,
    GNUTLS_BAG_ENCRYPTED=10,
    GNUTLS_BAG_UNKNOWN=20
} gnutls_pkcs12_bag_type_t;

gnutls_pkcs12_bag_type_t gnutls_pkcs12_bag_get_type(gnutls_pkcs12_bag_t bag, int indx);
int gnutls_pkcs12_bag_get_data(gnutls_pkcs12_bag_t bag, int indx, gnutls_datum_t* data);
int gnutls_pkcs12_bag_set_data(gnutls_pkcs12_bag_t bag, gnutls_pkcs12_bag_type_t type,
        const gnutls_datum_t* data);
int gnutls_pkcs12_bag_set_crl(gnutls_pkcs12_bag_t bag, gnutls_x509_crl_t crl);
int gnutls_pkcs12_bag_set_crt(gnutls_pkcs12_bag_t bag, gnutls_x509_crt_t crt);

int gnutls_pkcs12_bag_init(gnutls_pkcs12_bag_t * bag);
void gnutls_pkcs12_bag_deinit(gnutls_pkcs12_bag_t bag);
int gnutls_pkcs12_bag_get_count(gnutls_pkcs12_bag_t bag);

int gnutls_pkcs12_bag_get_key_id(gnutls_pkcs12_bag_t  bag, int indx, 
    gnutls_datum_t* id);
int gnutls_pkcs12_bag_set_key_id(gnutls_pkcs12_bag_t  bag, int indx, 
    const gnutls_datum_t* id);

int gnutls_pkcs12_bag_get_friendly_name(gnutls_pkcs12_bag_t  bag, int indx, 
    char **name);
int gnutls_pkcs12_bag_set_friendly_name(gnutls_pkcs12_bag_t  bag, int indx, 
    const char* name);

#ifdef __cplusplus
}
#endif
#endif /* GNUTLS_PKCS12_H */
