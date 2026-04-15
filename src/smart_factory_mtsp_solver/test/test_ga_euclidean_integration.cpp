#include <gtest/gtest.h>

#include "../include/smart_factory_mtsp_solver/genetic_algorithm.hpp"
#include "../include/smart_factory_mtsp_solver/problem_data.hpp"
#include "../include/smart_factory_mtsp_solver/point2d.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

namespace s = smart_factory_mtsp_solver;

namespace
{

double euclidean(const s::Point2D & a, const s::Point2D & b)
{
  const double dx = a.x - b.x;
  const double dy = a.y - b.y;
  return std::sqrt(dx * dx + dy * dy);
}

double recompute_solution_cost(
  const s::ProblemData & problem,
  const s::Solution & solution,
  double unused_robot_penalty,
  double route_count_balance_penalty)
{
  double cost = 0.0;
  int unused_count = 0;
  int min_route_size = static_cast<int>(problem.goal_count());
  int max_route_size = 0;

  for (std::size_t robot = 0; robot < solution.routes.size(); ++robot) {
    const auto & route = solution.routes[robot];
    const int route_size = static_cast<int>(route.size());

    if (route_size == 0) {
      ++unused_count;
      min_route_size = 0;
      continue;
    }

    if (route_size < min_route_size) {
      min_route_size = route_size;
    }
    if (route_size > max_route_size) {
      max_route_size = route_size;
    }

    cost += euclidean(problem.robot_starts[robot], problem.goals[static_cast<std::size_t>(route[0])]);

    for (std::size_t i = 1; i < route.size(); ++i) {
      cost += euclidean(
        problem.goals[static_cast<std::size_t>(route[i - 1])],
        problem.goals[static_cast<std::size_t>(route[i])]);
    }
  }

  cost += unused_robot_penalty * static_cast<double>(unused_count);

  if (!solution.routes.empty()) {
    const int imbalance = max_route_size - min_route_size;
    cost += route_count_balance_penalty * static_cast<double>(imbalance);
  }

  return cost;
}

std::vector<int> flatten_routes(const std::vector<std::vector<int>> & routes)
{
  std::vector<int> flat;
  for (const auto & route : routes) {
    flat.insert(flat.end(), route.begin(), route.end());
  }
  return flat;
}

s::ProblemData make_problem()
{
  s::ProblemData problem;
  problem.robot_starts = {
    s::Point2D(0.0, 0.0),
    s::Point2D(10.0, 0.0)
  };
  problem.goals = {
    s::Point2D(1.0, 0.0),
    s::Point2D(2.0, 0.0),
    s::Point2D(9.0, 0.0),
    s::Point2D(10.0, 1.0)
  };
  return problem;
}

}  // namespace

TEST(GeneticAlgorithmEuclideanIntegrationTest, ReturnsValidDeterministicSolution)
{
  const s::ProblemData problem = make_problem();

  s::GAParams params;
  params.population_size = 40;
  params.generations = 60;
  params.mutation_rate = 0.10;
  params.seed = 12345U;
  params.unused_robot_penalty = 5.0;
  params.route_count_balance_penalty = 1.5;
  params.distance_backend = s::DistanceBackend::EUCLIDEAN;

  s::GeneticAlgorithm ga1;
  s::GeneticAlgorithm ga2;

  const s::Solution solution1 = ga1.solve(problem, params);
  const s::Solution solution2 = ga2.solve(problem, params);

  ASSERT_EQ(solution1.routes.size(), problem.robot_count());
  ASSERT_EQ(solution2.routes.size(), problem.robot_count());

  const std::vector<int> flat1 = flatten_routes(solution1.routes);
  const std::vector<int> flat2 = flatten_routes(solution2.routes);

  ASSERT_EQ(flat1.size(), problem.goal_count());
  ASSERT_EQ(flat2.size(), problem.goal_count());

  std::vector<int> sorted1 = flat1;
  std::vector<int> sorted2 = flat2;
  std::sort(sorted1.begin(), sorted1.end());
  std::sort(sorted2.begin(), sorted2.end());

  std::vector<int> expected_goal_indices{0, 1, 2, 3};

  EXPECT_EQ(sorted1, expected_goal_indices);
  EXPECT_EQ(sorted2, expected_goal_indices);

  const double recomputed1 = recompute_solution_cost(
    problem,
    solution1,
    params.unused_robot_penalty,
    params.route_count_balance_penalty);

  const double recomputed2 = recompute_solution_cost(
    problem,
    solution2,
    params.unused_robot_penalty,
    params.route_count_balance_penalty);

  EXPECT_TRUE(std::isfinite(solution1.total_cost));
  EXPECT_TRUE(std::isfinite(solution2.total_cost));

  EXPECT_NEAR(solution1.total_cost, recomputed1, 1e-6);
  EXPECT_NEAR(solution2.total_cost, recomputed2, 1e-6);

  EXPECT_NEAR(solution1.total_cost, solution2.total_cost, 1e-9);
  EXPECT_EQ(solution1.routes, solution2.routes);
}