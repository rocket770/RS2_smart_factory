#ifndef SMART_FACTORY_MTSP_SOLVER_DISTANCE_MATRIX_HPP
#define SMART_FACTORY_MTSP_SOLVER_DISTANCE_MATRIX_HPP

#include <vector>
#include "problem_data.hpp"

namespace smart_factory_mtsp_solver
{

class DistanceMatrix
{
public:
  DistanceMatrix();

  void build_euclidean(const ProblemData & problem);

  double robot_to_goal(int robot_index, int goal_index) const;
  double goal_to_goal(int from_goal_index, int to_goal_index) const;

private:
  std::vector<std::vector<double> > robot_to_goal_;
  std::vector<std::vector<double> > goal_to_goal_;
};

}  // namespace smart_factory_mtsp_solver

#endif