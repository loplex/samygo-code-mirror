/* { dg-do compile { target { powerpc_spe_ok } } } */
/* { dg-options "-mcpu=8540 -mspe -mabi=spe -mfloat-gprs=single -O0" } */

/* (Test with -O0 so we don't optimize any of them away).  */


typedef float __attribute__((vector_size(8))) __ev64_fs__;

__ev64_opaque__ Foo (void);

void Bar ()
{
  __ev64_fs__ fs = Foo ();
}
