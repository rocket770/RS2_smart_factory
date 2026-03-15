#include "../include/smart_factory_mtsp_solver/genome.hpp"

#include <algorithm>

namespace smart_factory_mtsp_solver
{

Genome::Genome()
{
}

void Genome::randomize(int num_goals, int num_robots, std::mt19937 & rng)
{
  goal_order.clear();
  route_sizes.clear();

  goal_order.resize(static_cast<std::size_t>(num_goals));
  for (int i = 0; i < num_goals; ++i) {
    goal_order[static_cast<std::size_t>(i)] = i;
  }

  std::shuffle(goal_order.begin(), goal_order.end(), rng);

  route_sizes.assign(static_cast<std::size_t>(num_robots), 0);

  for (int i = 0; i < num_goals; ++i) {
    std::uniform_int_distribution<int> robot_dist(0, num_robots - 1);
    int robot_index = robot_dist(rng);
    route_sizes[static_cast<std::size_t>(robot_index)] += 1;
  }
}

Genome Genome::crossover(const Genome & other, std::mt19937 & rng) const
{
  Genome child;

  const int n = static_cast<int>(goal_order.size());
  child.goal_order.assign(goal_order.size(), -1);

  if (n > 0) {
    std::uniform_int_distribution<int> cut_dist(0, n - 1);
    int start = cut_dist(rng);
    int end = cut_dist(rng);

    if (start > end) {
      std::swap(start, end);
    }

    for (int i = start; i <= end; ++i) {
      child.goal_order[static_cast<std::size_t>(i)] = goal_order[static_cast<std::size_t>(i)];
    }

    int insert_pos = 0;
    for (int i = 0; i < n; ++i) {
      int candidate = other.goal_order[static_cast<std::size_t>(i)];
      bool already_used = false;

      for (int j = start; j <= end; ++j) {
        if (child.goal_order[static_cast<std::size_t>(j)] == candidate) {
          already_used = true;
          break;
        }
      }

      if (!already_used) {
        while (child.goal_order[static_cast<std::size_t>(insert_pos)] != -1) {
          ++insert_pos;
        }
        child.goal_order[static_cast<std::size_t>(insert_pos)] = candidate;
      }
    }
  }

  child.route_sizes = route_sizes;

  if (other.route_sizes.size() == route_sizes.size() && !route_sizes.empty()) {
    std::uniform_int_distribution<int> parent_pick(0, 1);
    child.route_sizes.assign(route_sizes.size(), 0);

    for (std::size_t i = 0; i < child.route_sizes.size(); ++i) {
      int picked = (parent_pick(rng) == 0) ? route_sizes[i] : other.route_sizes[i];
      if (picked < 0) {
        picked = 0;
      }
      child.route_sizes[i] = picked;
    }

    int sum = 0;
    for (std::size_t i = 0; i < child.route_sizes.size(); ++i) {
      sum += child.route_sizes[i];
    }

    while (sum > n) {
      for (std::size_t i = 0; i < child.route_sizes.size() && sum > n; ++i) {
        if (child.route_sizes[i] > 0) {
          child.route_sizes[i] -= 1;
          sum -= 1;
        }
      }
    }

    while (sum < n) {
      std::uniform_int_distribution<int> robot_dist(
        0, static_cast<int>(child.route_sizes.size()) - 1);
      int robot_index = robot_dist(rng);
      child.route_sizes[static_cast<std::size_t>(robot_index)] += 1;
      sum += 1;
    }
  }

  return child;
}

void Genome::mutate(std::mt19937 & rng)
{
  mutate_goal_order(rng);
  mutate_route_sizes(rng);
}

void Genome::mutate_goal_order(std::mt19937 & rng)
{
  if (goal_order.size() < 2) {
    return;
  }

  std::uniform_int_distribution<int> index_dist(0, static_cast<int>(goal_order.size()) - 1);
  int a = index_dist(rng);
  int b = index_dist(rng);

  std::swap(goal_order[static_cast<std::size_t>(a)], goal_order[static_cast<std::size_t>(b)]);
}

void Genome::mutate_route_sizes(std::mt19937 & rng)
{
  if (route_sizes.size() < 2) {
    return;
  }

  std::uniform_int_distribution<int> index_dist(0, static_cast<int>(route_sizes.size()) - 1);
  int from = index_dist(rng);
  int to = index_dist(rng);

  if (from == to) {
    return;
  }

  if (route_sizes[static_cast<std::size_t>(from)] > 0) {
    route_sizes[static_cast<std::size_t>(from)] -= 1;
    route_sizes[static_cast<std::size_t>(to)] += 1;
  }
}

}  // namespace smart_factory_mtsp_solver