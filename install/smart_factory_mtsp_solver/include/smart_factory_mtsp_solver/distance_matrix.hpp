#ifndef SMART_FACTORY_MTSP_SOLVER__DISTANCE_MATRIX_HPP_
#define SMART_FACTORY_MTSP_SOLVER__DISTANCE_MATRIX_HPP_

#include "path_cost_provider.hpp"
#include "problem_data.hpp"

#include <vector>

namespace smart_factory_mtsp_solver
{

class DistanceMatrix
{
public:
  DistanceMatrix();

  void build_euclidean(const ProblemData & problem);
  void build_from_provider(const ProblemData & problem, PathCostProvider & provider);

  double robot_to_goal(int robot_index, int goal_index) const;
  double goal_to_goal(int from_goal_index, int to_goal_index) const;

private:
  std::vector<std::vector<double>> robot_to_goal_;
  std::vector<std::vector<double>> goal_to_goal_;
};

}  // namespace smart_factory_mtsp_solver

#endif  // SMART_FACTORY_MTSP_SOLVER__DISTANCE_MATRIX_HPP_