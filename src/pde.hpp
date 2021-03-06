#pragma once
#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <typeinfo>
#include <vector>

#include "program_options.hpp"

#include "pde/pde_base.hpp"
#include "pde/pde_continuity1.hpp"
#include "pde/pde_continuity2.hpp"
#include "pde/pde_continuity3.hpp"
#include "pde/pde_continuity6.hpp"
#include "pde/pde_diffusion1.hpp"
#include "pde/pde_diffusion2.hpp"
#include "pde/pde_fokkerplanck1_4p1a.hpp"
#include "pde/pde_fokkerplanck1_4p2.hpp"
#include "pde/pde_fokkerplanck1_4p3.hpp"
#include "pde/pde_fokkerplanck1_4p4.hpp"
#include "pde/pde_fokkerplanck1_4p5.hpp"
#include "pde/pde_fokkerplanck2_complete.hpp"
#include "tensors.hpp"

//
// this file contains the PDE factory and the utilities to
// select the PDEs being made available by the included
// implementations
//

// ---------------------------------------------------------------------------
//
// A free function factory for making pdes. eventually will want to change the
// return for some of these once we implement them...
//
// ---------------------------------------------------------------------------

template<typename P>
std::unique_ptr<PDE<P>> make_PDE(parser const &cli_input)
{
  switch (cli_input.get_selected_pde())
  {
  case PDE_opts::continuity_1:
    return std::make_unique<PDE_continuity_1d<P>>(cli_input);
  case PDE_opts::continuity_2:
    return std::make_unique<PDE_continuity_2d<P>>(cli_input);
  case PDE_opts::continuity_3:
    return std::make_unique<PDE_continuity_3d<P>>(cli_input);
  case PDE_opts::continuity_6:
    return std::make_unique<PDE_continuity_6d<P>>(cli_input);
  case PDE_opts::fokkerplanck_1d_4p1a:
    return std::make_unique<PDE_fokkerplanck_1d_4p1a<P>>(cli_input);
  case PDE_opts::fokkerplanck_1d_4p2:
    return std::make_unique<PDE_fokkerplanck_1d_4p2<P>>(cli_input);
  case PDE_opts::fokkerplanck_1d_4p3:
    return std::make_unique<PDE_fokkerplanck_1d_4p3<P>>(cli_input);
  case PDE_opts::fokkerplanck_1d_4p4:
    return std::make_unique<PDE_fokkerplanck_1d_4p4<P>>(cli_input);
  case PDE_opts::fokkerplanck_1d_4p5:
    return std::make_unique<PDE_fokkerplanck_1d_4p5<P>>(cli_input);
  case PDE_opts::fokkerplanck_2d_complete:
    return std::make_unique<PDE_fokkerplanck_2d_complete<P>>(cli_input);
  case PDE_opts::diffusion_1:
    return std::make_unique<PDE_diffusion_1d<P>>(cli_input);
  case PDE_opts::diffusion_2:
    return std::make_unique<PDE_diffusion_2d<P>>(cli_input);
  default:
    std::cout << "Invalid pde choice" << std::endl;
    exit(-1);
  }
}

// shim for easy PDE creation in tests
template<typename P>
std::unique_ptr<PDE<P>>
make_PDE(PDE_opts const pde_choice, int const level = parser::NO_USER_VALUE,
         int const degree = parser::NO_USER_VALUE,
         double const cfl = parser::DEFAULT_CFL)
{
  return make_PDE<P>(parser(pde_choice, level, degree, cfl));
}
