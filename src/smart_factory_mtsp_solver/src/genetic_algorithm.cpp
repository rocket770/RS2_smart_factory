#include "../include/smart_factory_mtsp_solver/genetic_algorithm.hpp"
#include <stdexcept>
#include <memory>
#include <iostream>

namespace smart_factory_mtsp_solver
{

GeneticAlgorithm::GeneticAlgorithm()
: rng_(42U)
{
}

Solution GeneticAlgorithm::solve(
  const ProblemData & problem,
  const GAParams & params,
  ProgressCallback progress_callback,
  PathCostProvider * path_cost_provider)
{
  rng_.seed(params.seed);

  DistanceMatrix distance_matrix;

  switch (params.distance_backend) {
    case DistanceBackend::EUCLIDEAN:
      distance_matrix.build_euclidean(problem);
      break;

    case DistanceBackend::NAV2:
      if (path_cost_provider == nullptr) {
        throw std::runtime_error(
          "NAV2 distance backend selected, but path_cost_provider was nullptr");
      }
      distance_matrix.build_from_provider(problem, *path_cost_provider);
      break;

    default:
      throw std::runtime_error("Unknown distance backend");
  }


  Population population;
  population.initialize(
    params.population_size,
    static_cast<int>(problem.goal_count()),
    static_cast<int>(problem.robot_count()),
    rng_);

  population.evaluate_all(
    problem,
    distance_matrix,
    params.unused_robot_penalty,
    params.route_count_balance_penalty);
  population.set_best();

  if (progress_callback) {
    const Individual best = population.get_best();

    ProgressState progress;
    progress.generation = 0;
    progress.total_cost = best.cost;
    progress.routes = best.decode_routes(problem.robot_count());

    progress_callback(progress);
  }

  for (int generation = 0; generation < params.generations; ++generation) {
    population.evolve(
      rng_,
      problem,
      distance_matrix,
      params.mutation_rate,
      params.unused_robot_penalty,
      params.route_count_balance_penalty);

    population.evaluate_all(
      problem,
      distance_matrix,
      params.unused_robot_penalty,
      params.route_count_balance_penalty);
    population.set_best();

    if (progress_callback) {
      const Individual best = population.get_best();

      ProgressState progress;
      progress.generation = generation + 1;
      progress.total_cost = best.cost;
      progress.routes = best.decode_routes(problem.robot_count());

      progress_callback(progress);
    }
  }

  population.evaluate_all(
    problem,
    distance_matrix,
    params.unused_robot_penalty,
    params.route_count_balance_penalty);
  population.set_best();

  const Individual best = population.get_best();

  Solution solution;
  solution.total_cost = best.cost;
  solution.routes = best.decode_routes(problem.robot_count());

  return solution;
}

}  // namespace smart_factory_mtsp_solver