#ifndef SMART_FACTORY_MTSP_SOLVER_POPULATION_HPP
#define SMART_FACTORY_MTSP_SOLVER_POPULATION_HPP

#include <random>
#include <vector>
#include "individual.hpp"
#include "problem_data.hpp"
#include "distance_matrix.hpp"

namespace smart_factory_mtsp_solver
{

class Population
{
public:
  Population();

  void initialize(
    int population_size,
    int num_goals,
    int num_robots,
    std::mt19937 & rng);

  void evaluate_all(
    const ProblemData & problem,
    const DistanceMatrix & distance_matrix,
    double unused_robot_penalty,
    double route_count_balance_penalty);

  void set_best();
  Individual get_best() const;

  void evolve(
    std::mt19937 & rng,
    const ProblemData & problem,
    const DistanceMatrix & distance_matrix,
    double mutation_rate,
    double unused_robot_penalty,
    double route_count_balance_penalty);

  int generation;

private:
  std::vector<Individual> individuals_;
  int best_index_;
  double fitness_sum_;

  void calculate_fitness_sum();
  const Individual & select_parent(std::mt19937 & rng) const;
};

}  // namespace smart_factory_mtsp_solver

#endif