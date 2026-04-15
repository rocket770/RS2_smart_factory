#include <gtest/gtest.h>

#include "../include/smart_factory_mtsp_solver/distance_matrix.hpp"
#include "../include/smart_factory_mtsp_solver/individual.hpp"
#include "../include/smart_factory_mtsp_solver/problem_data.hpp"
#include "../include/smart_factory_mtsp_solver/point2d.hpp"

#include <cmath>
#include <vector>

namespace s = smart_factory_mtsp_solver;

namespace
{

s::ProblemData make_problem()
{
  s::ProblemData problem;
  problem.robot_starts = {
    s::Point2D(0.0, 0.0),
    s::Point2D(10.0, 0.0)
  };
  problem.goals = {
    s::Point2D(3.0, 4.0),   // goal 0
    s::Point2D(6.0, 8.0),   // goal 1
    s::Point2D(10.0, 3.0)   // goal 2
  };
  return problem;
}

double compute_expected_cost_from_routes(
  const s::ProblemData & problem,
  const std::vector<std::vector<int>> & routes,
  double unused_robot_penalty,
  double route_count_balance_penalty)
{
  auto euclidean = [](const s::Point2D & a, const s::Point2D & b) {
    const double dx = a.x - b.x;
    const double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
  };

  double cost = 0.0;
  int unused_count = 0;
  int min_route_size = static_cast<int>(problem.goal_count());
  int max_route_size = 0;

  for (std::size_t robot = 0; robot < routes.size(); ++robot) {
    const int route_size = static_cast<int>(routes[robot].size());

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

    cost += euclidean(problem.robot_starts[robot], problem.goals[static_cast<std::size_t>(routes[robot][0])]);

    for (std::size_t i = 1; i < routes[robot].size(); ++i) {
      cost += euclidean(
        problem.goals[static_cast<std::size_t>(routes[robot][i - 1])],
        problem.goals[static_cast<std::size_t>(routes[robot][i])]);
    }
  }

  cost += unused_robot_penalty * static_cast<double>(unused_count);

  if (!routes.empty()) {
    const int imbalance = max_route_size - min_route_size;
    cost += route_count_balance_penalty * static_cast<double>(imbalance);
  }

  return cost;
}

}  // namespace

TEST(DistanceMatrixEuclideanTest, BuildsRobotToGoalAndGoalToGoalDistances)
{
  const s::ProblemData problem = make_problem();

  s::DistanceMatrix matrix;
  matrix.build_euclidean(problem);

  EXPECT_NEAR(matrix.robot_to_goal(0, 0), 5.0, 1e-6);
  EXPECT_NEAR(matrix.robot_to_goal(0, 1), 10.0, 1e-6);
  EXPECT_NEAR(matrix.robot_to_goal(1, 2), 3.0, 1e-6);

  EXPECT_NEAR(matrix.goal_to_goal(0, 0), 0.0, 1e-6);
  EXPECT_NEAR(matrix.goal_to_goal(0, 1), 5.0, 1e-6);
  EXPECT_NEAR(matrix.goal_to_goal(1, 0), 5.0, 1e-6);
}

TEST(IndividualTest, DecodeRoutesSplitsGoalOrderUsingRouteSizes)
{
  s::Individual individual;
  individual.genome.goal_order = {2, 0, 1};
  individual.genome.route_sizes = {2, 1};

  const auto routes = individual.decode_routes(2);

  ASSERT_EQ(routes.size(), 2u);
  EXPECT_EQ(routes[0], (std::vector<int>{2, 0}));
  EXPECT_EQ(routes[1], (std::vector<int>{1}));
}

TEST(IndividualTest, EvaluateComputesEuclideanCostWithoutPenalties)
{
  const s::ProblemData problem = make_problem();

  s::DistanceMatrix matrix;
  matrix.build_euclidean(problem);

  s::Individual individual;
  individual.genome.goal_order = {0, 1, 2};
  individual.genome.route_sizes = {2, 1};

  individual.evaluate(problem, matrix, 0.0, 0.0);

  const auto routes = individual.decode_routes(problem.robot_count());
  const double expected_cost = compute_expected_cost_from_routes(problem, routes, 0.0, 0.0);
  const double expected_fitness = 1.0 / (1.0 + expected_cost);

  EXPECT_NEAR(individual.cost, expected_cost, 1e-6);
  EXPECT_NEAR(individual.fitness, expected_fitness, 1e-6);
}

TEST(IndividualTest, EvaluateAddsUnusedRobotAndBalancePenalties)
{
  s::ProblemData problem;
  problem.robot_starts = {
    s::Point2D(0.0, 0.0),
    s::Point2D(10.0, 0.0),
    s::Point2D(20.0, 0.0)
  };
  problem.goals = {
    s::Point2D(3.0, 4.0),
    s::Point2D(6.0, 8.0),
    s::Point2D(10.0, 3.0)
  };

  s::DistanceMatrix matrix;
  matrix.build_euclidean(problem);

  s::Individual individual;
  individual.genome.goal_order = {0, 1, 2};
  individual.genome.route_sizes = {3, 0, 0};

  const double unused_robot_penalty = 7.5;
  const double route_count_balance_penalty = 2.0;

  individual.evaluate(
    problem,
    matrix,
    unused_robot_penalty,
    route_count_balance_penalty);

  const auto routes = individual.decode_routes(problem.robot_count());
  const double expected_cost = compute_expected_cost_from_routes(
    problem,
    routes,
    unused_robot_penalty,
    route_count_balance_penalty);

  EXPECT_NEAR(individual.cost, expected_cost, 1e-6);

  // total penalty contribution should be 2 * 7.5 + 3 * 2.0 = 21.0
  const double base_cost = compute_expected_cost_from_routes(problem, routes, 0.0, 0.0);
  EXPECT_NEAR(individual.cost - base_cost, 21.0, 1e-6);
}