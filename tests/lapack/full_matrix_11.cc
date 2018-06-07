// ---------------------------------------------------------------------
//
// Copyright (C) 2014 - 2017 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE.md at
// the top level directory of deal.II.
//
// ---------------------------------------------------------------------


// Tests LAPACKFullMatrix::TmTmult

#include <deal.II/lac/full_matrix.h>
#include <deal.II/lac/lapack_full_matrix.h>
#include <deal.II/lac/vector.h>

#include <iostream>

#include "../tests.h"



void
test()
{
  const unsigned int       m = 2;
  const unsigned int       n = 3;
  const unsigned int       k = 4;
  FullMatrix<double>       A(k, m), B(n, k), C(m, n), OC(m, n);
  LAPACKFullMatrix<double> AL(k, m), BL(n, k), CL(m, n);
  for (unsigned int i = 0; i < k; ++i)
    for (unsigned int j = 0; j < m; ++j)
      A(i, j) = AL(i, j) = random_value<double>();
  for (unsigned int i = 0; i < n; ++i)
    for (unsigned int j = 0; j < k; ++j)
      B(i, j) = BL(i, j) = random_value<double>();

  A.TmTmult(C, B);
  AL.TmTmult(CL, BL);
  AL.TmTmult(OC, BL);
  for (unsigned int i = 0; i < m; ++i)
    for (unsigned int j = 0; j < n; ++j)
      {
        Assert(std::abs(C(i, j) - CL(i, j)) < 1e-13, ExcInternalError());
        Assert(std::abs(C(i, j) - OC(i, j)) < 1e-13, ExcInternalError());
      }

  deallog << "OK" << std::endl;
}

int
main()
{
  const std::string logname = "output";
  std::ofstream     logfile(logname.c_str());
  logfile.precision(3);
  deallog.attach(logfile);

  test();
}
