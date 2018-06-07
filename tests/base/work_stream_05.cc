// ---------------------------------------------------------------------
//
// Copyright (C) 2008 - 2017 by the deal.II authors
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


// a test for WorkStream where we really do write conflicting entries
// into a global vector

#include <deal.II/base/work_stream.h>

#include <deal.II/lac/vector.h>

#include "../tests.h"


Vector<double> result(100);


struct ScratchData
{};


struct CopyData
{
  unsigned int computed;
};


void
worker(const std::vector<unsigned int>::iterator &i,
       ScratchData &,
       CopyData &ad)
{
  ad.computed = *i * 2;
}

void
copier(const CopyData &ad)
{
  // write into the five elements of 'result' starting at
  // ad.computed%result.size()
  for (unsigned int j = 0; j < 5; ++j)
    result((ad.computed + j) % result.size()) += ad.computed;
}



void
test()
{
  std::vector<unsigned int> v;
  for (unsigned int i = 0; i < 200; ++i)
    v.push_back(i);

  WorkStream::run(
    v.begin(), v.end(), &worker, &copier, ScratchData(), CopyData());

  // now simulate what we should have gotten
  Vector<double> comp(result.size());
  for (unsigned int i = 0; i < v.size(); ++i)
    {
      const unsigned int ad_computed = v[i] * 2;
      for (unsigned int j = 0; j < 5; ++j)
        comp((ad_computed + j) % result.size()) += ad_computed;
    }


  // and compare
  for (unsigned int i = 0; i < result.size(); ++i)
    AssertThrow(result(i) == comp(i), ExcInternalError());

  for (unsigned int i = 0; i < result.size(); ++i)
    deallog << result(i) << std::endl;
}



int
main()
{
  initlog();

  test();
}
