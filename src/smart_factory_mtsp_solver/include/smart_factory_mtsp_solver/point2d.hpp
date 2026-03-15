#ifndef SMART_FACTORY_MTSP_SOLVER_POINT2D_HPP
#define SMART_FACTORY_MTSP_SOLVER_POINT2D_HPP

namespace smart_factory_mtsp_solver
{

struct Point2D
{
  double x;
  double y;

  Point2D() : x(0.0), y(0.0) {}
  Point2D(double x_in, double y_in) : x(x_in), y(y_in) {}
};

}  // namespace smart_factory_mtsp_solver

#endif