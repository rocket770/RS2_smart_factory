#include "../include/smart_factory_mtsp_solver/distance_matrix.hpp"

#include <cmath>
#include <stdexcept>

namespace smart_factory_mtsp_solver
{

namespace
{

double euclidean_distance(const Point2D & a, const Point2D & b)
{
  const double dx = a.x - b.x;
  const double dy = a.y - b.y;
  return std::sqrt(dx * dx + dy * dy);
}

}  // namespace

DistanceMatrix::DistanceMatrix()
{
}

void DistanceMatrix::build_euclidean(const ProblemData & problem)
{
  const std::size_t num_robots = problem.robot_starts.size();
  const std::size_t num_goals = problem.goals.size();

  robot_to_goal_.assign(num_robots, std::vector<double>(num_goals, 0.0));
  goal_to_goal_.assign(num_goals, std::vector<double>(num_goals, 0.0));

  for (std::size_t r = 0; r < num_robots; ++r) {
    for (std::size_t g = 0; g < num_goals; ++g) {
      robot_to_goal_[r][g] = euclidean_distance(problem.robot_starts[r], problem.goals[g]);
    }
  }

  for (std::size_t i = 0; i < num_goals; ++i) {
    for (std::size_t j = 0; j < num_goals; ++j) {
      goal_to_goal_[i][j] = euclidean_distance(problem.goals[i], problem.goals[j]);
    }
  }
}

double DistanceMatrix::robot_to_goal(int robot_index, int goal_index) const
{
  return robot_to_goal_.at(static_cast<std::size_t>(robot_index)).at(static_cast<std::size_t>(goal_index));
}

double DistanceMatrix::goal_to_goal(int from_goal_index, int to_goal_index) const
{
  return goal_to_goal_.at(static_cast<std::size_t>(from_goal_index)).at(static_cast<std::size_t>(to_goal_index));
}

}  // namespace smart_factory_mtsp_solver