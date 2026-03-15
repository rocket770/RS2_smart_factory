#include "../include/smart_factory_mtsp_solver/genome.hpp"

#include <algorithm>

namespace smart_factory_mtsp_solver
{

Genome::Genome()
{
}

void Genome::repair_route_sizes(int total_goals)
{
  const int num_robots = static_cast<int>(route_sizes.size());
  if (num_robots <= 0) {
    return;
  }

  // how many goals each robot gets
  for (std::size_t i = 0; i < route_sizes.size(); ++i) {
    if (route_sizes[i] < 0) {
      route_sizes[i] = 0;
    }
  }

  // if there enough goals 
  if (total_goals >= num_robots) {
    for (std::size_t i = 0; i < route_sizes.size(); ++i) {
      if (route_sizes[i] == 0) {
        route_sizes[i] = 1;
      }
    }
  }

  int sum = 0;
  for (std::size_t i = 0; i < route_sizes.size(); ++i) {
    sum += route_sizes[i];
  }

  // reduce assignments if the total is too large
  if (sum > total_goals) {
    if (total_goals >= num_robots) {
      while (sum > total_goals) {
        bool changed = false;
        for (std::size_t i = 0; i < route_sizes.size() && sum > total_goals; ++i) {
          if (route_sizes[i] > 1) {
            route_sizes[i] -= 1;
            sum -= 1;
            changed = true;
          }
        }
        if (!changed) {
          break;
        }
      }
    } else {
      while (sum > total_goals) {
        bool changed = false;
        for (std::size_t i = 0; i < route_sizes.size() && sum > total_goals; ++i) {
          if (route_sizes[i] > 0) {
            route_sizes[i] -= 1;
            sum -= 1;
            changed = true;
          }
        }
        if (!changed) {
          break;
        }
      }
    }
  } else if (sum < total_goals) {

    // add missing goals to the robot with the smallest route
    while (sum < total_goals) {
      std::size_t best_index = 0;
      int smallest = route_sizes[0];

      for (std::size_t i = 1; i < route_sizes.size(); ++i) {
        if (route_sizes[i] < smallest) {
          smallest = route_sizes[i];
          best_index = i;
        }
      }

      route_sizes[best_index] += 1;
      sum += 1;
    }
  }
}

void Genome::randomize(int num_goals, int num_robots, std::mt19937 & rng)
{
  goal_order.clear();
  route_sizes.clear();

  goal_order.resize(static_cast<std::size_t>(num_goals));
  for (int i = 0; i < num_goals; ++i) {
    goal_order[static_cast<std::size_t>(i)] = i;
  }

  // create a random visiting order for all goals
  std::shuffle(goal_order.begin(), goal_order.end(), rng);

  route_sizes.assign(static_cast<std::size_t>(num_robots), 0);

  if (num_robots <= 0) {
    return;
  }

  if (num_goals >= num_robots) {
    // start by giving each robot one goal
    for (int i = 0; i < num_robots; ++i) {
      route_sizes[static_cast<std::size_t>(i)] = 1;
    }

    const int remaining = num_goals - num_robots;
    for (int i = 0; i < remaining; ++i) {
      std::uniform_int_distribution<int> robot_dist(0, num_robots - 1);
      int robot_index = robot_dist(rng);
      route_sizes[static_cast<std::size_t>(robot_index)] += 1;
    }
  } else {
    for (int i = 0; i < num_goals; ++i) {
      route_sizes[static_cast<std::size_t>(i)] = 1;
    }
  }

  repair_route_sizes(num_goals);
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

    // copy one segment from this parent, then fill the rest from the other parent
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

    // pick each route size from one of the two parents
    for (std::size_t i = 0; i < child.route_sizes.size(); ++i) {
      int picked = (parent_pick(rng) == 0) ? route_sizes[i] : other.route_sizes[i];
      if (picked < 0) {
        picked = 0;
      }
      child.route_sizes[i] = picked;
    }

    child.repair_route_sizes(n);
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

  // mutate by swapping two goals
  std::swap(goal_order[static_cast<std::size_t>(a)], goal_order[static_cast<std::size_t>(b)]);
}

void Genome::mutate_route_sizes(std::mt19937 & rng)
{
  if (route_sizes.size() < 2) {
    return;
  }

  const int total_goals = static_cast<int>(goal_order.size());
  const int num_robots = static_cast<int>(route_sizes.size());

  std::uniform_int_distribution<int> index_dist(0, num_robots - 1);
  int from = index_dist(rng);
  int to = index_dist(rng);

  if (from == to) {
    return;
  }

  // move one goal from one robot to another when allowed
  if (total_goals >= num_robots) {
    if (route_sizes[static_cast<std::size_t>(from)] > 1) {
      route_sizes[static_cast<std::size_t>(from)] -= 1;
      route_sizes[static_cast<std::size_t>(to)] += 1;
    }
  } else {
    if (route_sizes[static_cast<std::size_t>(from)] > 0) {
      route_sizes[static_cast<std::size_t>(from)] -= 1;
      route_sizes[static_cast<std::size_t>(to)] += 1;
    }
  }

  repair_route_sizes(total_goals);
}

}  // namespace smart_factory_mtsp_solver