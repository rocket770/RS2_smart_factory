#ifndef SMART_FACTORY_MTSP_SOLVER_GENOME_HPP
#define SMART_FACTORY_MTSP_SOLVER_GENOME_HPP

#include <random>
#include <vector>

namespace smart_factory_mtsp_solver
{

class Genome
{
public:
  std::vector<int> goal_order;
  std::vector<int> route_sizes;

  Genome();

  void randomize(int num_goals, int num_robots, std::mt19937 & rng);
  Genome crossover(const Genome & other, std::mt19937 & rng) const;
  void mutate(std::mt19937 & rng);

private:
  void mutate_goal_order(std::mt19937 & rng);
  void mutate_route_sizes(std::mt19937 & rng);
};

}  // namespace smart_factory_mtsp_solver

#endif