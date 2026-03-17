#include "../include/smart_factory_mtsp_solver/population.hpp"

#include <limits>
#include <random>
#include <vector>

namespace smart_factory_mtsp_solver
{

  Population::Population()
  : generation(0), best_index_(0), fitness_sum_(0.0)
  {
  }

  void Population::initialize(
    int population_size,
    int num_goals,
    int num_robots,
    std::mt19937 & rng)
  {
    individuals_.clear();
    individuals_.resize(static_cast<std::size_t>(population_size));

    // start off with random paths
    for (int i = 0; i < population_size; ++i) {
      individuals_[static_cast<std::size_t>(i)].genome.randomize(num_goals, num_robots, rng);
    }

    generation = 0;
    best_index_ = 0;
    fitness_sum_ = 0.0;
  }

  void Population::evaluate_all(
    const ProblemData & problem,
    const DistanceMatrix & distance_matrix,
    double unused_robot_penalty,
    double route_count_balance_penalty)
  {
    for (std::size_t i = 0; i < individuals_.size(); ++i) {
      individuals_[i].evaluate(
        problem,
        distance_matrix,
        unused_robot_penalty,
        route_count_balance_penalty);
    }
  }

  void Population::set_best()
  {
    if (individuals_.empty()) {
      best_index_ = 0;
      return;
    }

    double best_cost = std::numeric_limits<double>::max();
    int index = 0;

    for (std::size_t i = 0; i < individuals_.size(); ++i) {
      if (individuals_[i].cost < best_cost) {
        best_cost = individuals_[i].cost;
        index = static_cast<int>(i);
      }
    }

    best_index_ = index;
  }

  Individual Population::get_best() const
  {
    return individuals_.at(static_cast<std::size_t>(best_index_));
  }

  void Population::calculate_fitness_sum()
  {
    fitness_sum_ = 0.0;
    for (std::size_t i = 0; i < individuals_.size(); ++i) {
      fitness_sum_ += individuals_[i].fitness;
    }
  }

  const Individual & Population::select_parent(std::mt19937 & rng) const
  {
    std::uniform_real_distribution<double> dist(0.0, fitness_sum_);
    const double pick = dist(rng);

    double running_sum = 0.0;
    for (std::size_t i = 0; i < individuals_.size(); ++i) {
      running_sum += individuals_[i].fitness;
      if (running_sum >= pick) {
        return individuals_[i];
      }
    }

    return individuals_.back();
  }

  void Population::evolve(
    std::mt19937 & rng,
    const ProblemData & problem,
    const DistanceMatrix & distance_matrix,
    double mutation_rate,
    double unused_robot_penalty,
    double route_count_balance_penalty)
  {
    evaluate_all(
      problem,
      distance_matrix,
      unused_robot_penalty,
      route_count_balance_penalty);
    set_best();
    calculate_fitness_sum();

    std::vector<Individual> next_generation(individuals_.size());
    next_generation[0] = get_best();

    std::uniform_real_distribution<double> mutation_dist(0.0, 1.0);

    for (std::size_t i = 1; i < next_generation.size(); ++i) {
      const Individual & parent_a = select_parent(rng);
      const Individual & parent_b = select_parent(rng);

      Individual child;
      child.genome = parent_a.genome.crossover(parent_b.genome, rng);

      if (mutation_dist(rng) < mutation_rate) {
        child.genome.mutate(rng);
      }

      next_generation[i] = child;
    }

    individuals_ = next_generation;
    generation += 1;
  }

}  // namespace smart_factory_mtsp_solver