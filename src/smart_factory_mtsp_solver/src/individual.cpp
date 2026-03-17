#include "../include/smart_factory_mtsp_solver/individual.hpp"

namespace smart_factory_mtsp_solver
{

Individual::Individual()
: cost(0.0), fitness(0.0)
{
}

std::vector<std::vector<int> > Individual::decode_routes(std::size_t robot_count) const
{
  std::vector<std::vector<int> > routes(robot_count);

  std::size_t cursor = 0;
  for (std::size_t robot = 0; robot < robot_count; ++robot) {
    const int count = genome.route_sizes[robot];
    for (int i = 0; i < count; ++i) {
      if (cursor < genome.goal_order.size()) {
        routes[robot].push_back(genome.goal_order[cursor]);
        ++cursor;
      }
    }
  }

  return routes;
}

void Individual::evaluate(
  const ProblemData & problem,
  const DistanceMatrix & distance_matrix,
  double unused_robot_penalty,
  double route_count_balance_penalty)
{
  cost = 0.0;

  const std::vector<std::vector<int> > routes = decode_routes(problem.robot_count());

  int unused_count = 0;
  int min_route_size = static_cast<int>(problem.goal_count());
  int max_route_size = 0;

  for (std::size_t robot = 0; robot < routes.size(); ++robot) {
    const int route_size = static_cast<int>(routes[robot].size());

    if (route_size == 0) {
      unused_count += 1;
      min_route_size = 0;
      continue;
    }

    if (route_size < min_route_size) {
      min_route_size = route_size;
    }
    if (route_size > max_route_size) {
      max_route_size = route_size;
    }

    cost += distance_matrix.robot_to_goal(static_cast<int>(robot), routes[robot][0]);

    for (std::size_t i = 1; i < routes[robot].size(); ++i) {
      cost += distance_matrix.goal_to_goal(routes[robot][i - 1], routes[robot][i]);
    }
  }

  cost += unused_robot_penalty * static_cast<double>(unused_count);

  if (!routes.empty()) {
    const int imbalance = max_route_size - min_route_size;
    cost += route_count_balance_penalty * static_cast<double>(imbalance);
  }

  fitness = 1.0 / (1.0 + cost);
}

}  // namespace smart_factory_mtsp_solver