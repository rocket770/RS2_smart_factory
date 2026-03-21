#ifndef SMART_FACTORY_MTSP_SOLVER__PATH_COST_PROVIDER_HPP_
#define SMART_FACTORY_MTSP_SOLVER__PATH_COST_PROVIDER_HPP_

#include "problem_data.hpp"

namespace smart_factory_mtsp_solver
{

class PathCostProvider
{
public:
  virtual ~PathCostProvider() = default;

  virtual double compute_cost(
    const Point2D & start,
    const Point2D & goal) = 0;
};

}  // namespace smart_factory_mtsp_solver

#endif  // SMART_FACTORY_MTSP_SOLVER__PATH_COST_PROVIDER_HPP_