// 2004-04-30  Paolo Carlini  <pcarlini@suse.de>

// Copyright (C) 2004 Free Software Foundation
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING.  If not, write to the Free
// Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
// USA.

// 22.2.2.2.1  num_put members

#include <locale>
#include <sstream>
#include <testsuite_hooks.h>

// On Solaris 10 x86, this test crashes in libc.  Inside libstdc++,
// we call sprintf like so:
//   sprintf (buffer, "%.*f", 1000, 1.0)
// which crashes.
// { dg-do run { xfail { i?86*-*-solaris2.10 } } }

// libstdc++/14220
void test01()
{
  using namespace std;
  bool test __attribute__((unused)) = true;
  
  wostringstream oss;
  const num_put<wchar_t>& np = use_facet<num_put<wchar_t> >(oss.getloc());

  const int precision = 1000;

  oss.precision(precision);
  oss.setf(ios_base::fixed);
  np.put(oss.rdbuf(), oss, L'+', 1.0);
  const wstring result = oss.str();
  VERIFY( result.size() == precision + 2 );
}

int main()
{
  test01();
  return 0;
}
