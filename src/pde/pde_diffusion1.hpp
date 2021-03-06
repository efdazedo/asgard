#pragma once
#include "pde_base.hpp"

template<typename P>
class PDE_diffusion_1d : public PDE<P>
{
public:
  PDE_diffusion_1d(parser const &cli_input)
      : PDE<P>(cli_input, num_dims_, num_sources_, num_terms_, dimensions_,
               terms_, sources_, exact_vector_funcs_, exact_scalar_func_,
               get_dt_, do_poisson_solve_, has_analytic_soln_)
  {}

private:
  // these fields will be checked against provided functions to make sure
  // everything is specified correctly

  static int constexpr num_dims_           = 1;
  static int constexpr num_sources_        = 1;
  static int constexpr num_terms_          = 1;
  static bool constexpr do_poisson_solve_  = false;
  static bool constexpr has_analytic_soln_ = true;

  static fk::vector<P>
  initial_condition_dim0(fk::vector<P> const &x, P const t = 0)
  {
    static double p     = -2.0 * PI * PI;
    P const coefficient = std::exp(p * t);

    fk::vector<P> fx(x.size());
    std::transform(x.begin(), x.end(), fx.begin(),
                   [coefficient](P const x_value) -> P {
                     return coefficient * std::cos(PI * x_value);
                   });

    return fx;
  }

  /* Define the dimension */
  inline static dimension<P> const dim_0 =
      dimension<P>(0, 1, 2, 2, initial_condition_dim0, "x");

  inline static std::vector<dimension<P>> const dimensions_ = {dim_0};

  /* Define the g_functions */
  static P g_func_0(P const x, P const time)
  {
    // suppress compiler warnings
    ignore(x);
    ignore(time);
    return 1.0;
  }

  inline static const partial_term<P> partial_term_0 = partial_term<P>(
      coefficient_type::grad, partial_term<P>::null_gfunc, flux_type::upwind,
      boundary_condition::neumann, boundary_condition::neumann);

  static fk::vector<P> bc_func(fk::vector<P> const x, P const t)
  {
    ignore(t);

    fk::vector<P> fx(x.size());
    std::transform(x.begin(), x.end(), fx.begin(),
                   [](P const x_value) -> P { return std::cos(PI * x_value); });

    return fx;
  }

  static P bc_time_func(P const t)
  {
    /* e^(-2 * pi^2 * t )*/
    static double const p = -2.0 * PI * PI;
    return std::exp(p * t);
  }

  inline static const partial_term<P> partial_term_1 = partial_term<P>(
      coefficient_type::grad, g_func_0, flux_type::downwind,
      boundary_condition::dirichlet, boundary_condition::dirichlet,
      homogeneity::inhomogeneous, homogeneity::inhomogeneous, {bc_func},
      bc_time_func, {bc_func}, bc_time_func);

  inline static term<P> const term_0 =
      term<P>(true,            // time-dependent
              fk::vector<P>(), // additional data vector
              "",              // name
              dim_0,           // owning dim
              {partial_term_0, partial_term_1});

  inline static std::vector<term<P>> const terms_0 = {term_0};
  inline static term_set<P> const terms_           = {terms_0};

  /* Create sources */
  static fk::vector<P> source_0_x(fk::vector<P> const x, P const t)
  {
    ignore(t);
    static double const neg_pi_squared = -1.0 * PI * PI;

    fk::vector<P> fx(x.size());
    std::transform(x.begin(), x.end(), fx.begin(), [](P const x_value) -> P {
      return neg_pi_squared * std::cos(PI * x_value);
    });

    return fx;
  }

  static P source_0_t(P const t)
  {
    static double neg_two_pi_squared = -2.0 * PI * PI;

    return std::exp(neg_two_pi_squared * t);
  }

  inline static source<P> const source_0 = source<P>({source_0_x}, source_0_t);

  inline static std::vector<source<P>> const sources_ = {source_0};

  /* exact solutions */
  static fk::vector<P> exact_solution_0(fk::vector<P> const x, P const t = 0)
  {
    ignore(t);
    fk::vector<P> fx(x.size());
    std::transform(x.begin(), x.end(), fx.begin(),
                   [](P const &x) { return std::cos(PI * x); });
    return fx;
  }

  static P exact_time(P const time) { return std::sin(time); }

  inline static std::vector<vector_func<P>> const exact_vector_funcs_ = {
      exact_solution_0};

  /* This is not used ever */
  inline static scalar_func<P> const exact_scalar_func_ = source_0_t;

  static P get_dt_(dimension<P> const &dim)
  {
    /* (1/2^level)^2 = 1/4^level */
    /* return dx; this will be scaled by CFL from command line */
    return std::pow(0.25, dim.get_level());
  }
};
