#ifndef SMART_FACTORY_MTSP_SOLVER_INDIVIDUAL_HPP
#define SMART_FACTORY_MTSP_SOLVER_INDIVIDUAL_HPP

#include <vector>
#include "genome.hpp"
#include "problem_data.hpp"
#include "distance_matrix.hpp"

namespace smart_factory_mtsp_solver
{

class Individual
{
public:
  Genome genome;
  double cost;
  double fitness;

  Individual();

  void evaluate(const ProblemData & problem, const DistanceMatrix & distance_matrix);
  std::vector<std::vector<int> > decode_routes(std::size_t robot_count) const;
};

}  // namespace smart_factory_mtsp_solver

#endif