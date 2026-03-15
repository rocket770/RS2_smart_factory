#ifndef SMART_FACTORY_MTSP_SOLVER_GENETIC_ALGORITHM_HPP
#define SMART_FACTORY_MTSP_SOLVER_GENETIC_ALGORITHM_HPP

#include <functional>
#include <random>
#include <vector>

#include "problem_data.hpp"
#include "distance_matrix.hpp"
#include "population.hpp"

namespace smart_factory_mtsp_solver
{

struct GAParams
{
  int population_size;
  int generations;
  double mutation_rate;
  unsigned int seed;
  double unused_robot_penalty;
  double route_count_balance_penalty;

  GAParams()
  : population_size(100),
    generations(200),
    mutation_rate(0.10),
    seed(42U),
    unused_robot_penalty(0.0),
    route_count_balance_penalty(0.0)
  {
  }
};

struct Solution
{
  double total_cost;
  std::vector<std::vector<int> > routes;
};

struct ProgressState
{
  int generation;
  double total_cost;
  std::vector<std::vector<int> > routes;
};

using ProgressCallback = std::function<void(const ProgressState &)>;

class GeneticAlgorithm
{
public:
  GeneticAlgorithm();

  Solution solve(
    const ProblemData & problem,
    const GAParams & params,
    ProgressCallback progress_callback = ProgressCallback());

private:
  std::mt19937 rng_;
};

}  // namespace smart_factory_mtsp_solver

#endif