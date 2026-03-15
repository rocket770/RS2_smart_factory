#ifndef SMART_FACTORY_MTSP_SOLVER_PROBLEM_DATA_HPP
#define SMART_FACTORY_MTSP_SOLVER_PROBLEM_DATA_HPP

#include <vector>
#include "point2d.hpp"

namespace smart_factory_mtsp_solver
{

struct ProblemData
{
  std::vector<Point2D> robot_starts;
  std::vector<Point2D> goals;

  std::size_t robot_count() const
  {
    return robot_starts.size();
  }

  std::size_t goal_count() const
  {
    return goals.size();
  }
};

}  // namespace smart_factory_mtsp_solver

#endif