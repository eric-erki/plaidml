#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "tile/lang/bignum.h"
#include "tile/lang/bilp/ilp_solver.h"

namespace vertexai {
namespace tile {
namespace lang {
namespace bilp {

TEST(BilpTest, TestTest) { EXPECT_EQ(0, 0); }

TEST(BilpTest, RationalTest) { EXPECT_EQ(Rational(0, 1), 0); }

TEST(BilpTest, BasicTableauTest) {
  std::vector<RangeConstraint> constraints;
  constraints.emplace_back(Polynomial("x") + 4, 4);
  Polynomial obj = Polynomial("x", 3);
  ILPSolver solver;
  Tableau t = solver.makeStandardFormTableau(constraints, obj);

  EXPECT_EQ(t.mat().size1(), 3);
  EXPECT_EQ(t.mat().size2(), 6);

  EXPECT_EQ(t.mat()(0, 0), 1);
  EXPECT_EQ(t.mat()(0, 1), -3);
  EXPECT_EQ(t.mat()(0, 2), 3);
  EXPECT_EQ(t.mat()(0, 3), 0);
  EXPECT_EQ(t.mat()(0, 4), 0);
  EXPECT_EQ(t.mat()(0, 5), 0);

  EXPECT_EQ(t.mat()(1, 0), 0);
  EXPECT_EQ(t.mat()(1, 1), -1);
  EXPECT_EQ(t.mat()(1, 2), 1);
  EXPECT_EQ(t.mat()(1, 3), 1);
  EXPECT_EQ(t.mat()(1, 4), 0);
  EXPECT_EQ(t.mat()(1, 5), 4);

  EXPECT_EQ(t.mat()(2, 0), 0);
  EXPECT_EQ(t.mat()(2, 1), -1);
  EXPECT_EQ(t.mat()(2, 2), 1);
  EXPECT_EQ(t.mat()(2, 3), 0);
  EXPECT_EQ(t.mat()(2, 4), -1);
  EXPECT_EQ(t.mat()(2, 5), 1);
}

TEST(BilpTest, OptimizeCanonicalTest) {
  std::vector<std::string> blank_var_names;
  Tableau t(3, 6, blank_var_names);
  t.mat()(0, 0) = 1;
  t.mat()(0, 1) = 5;
  t.mat()(0, 2) = -5;
  t.mat()(1, 1) = -2;
  t.mat()(1, 2) = 4;
  t.mat()(1, 3) = 1;
  t.mat()(1, 5) = 2;
  t.mat()(2, 1) = 9;
  t.mat()(2, 2) = 3;
  t.mat()(2, 4) = 1;
  t.mat()(2, 5) = 7;

  t.selectBasicVars();
  EXPECT_EQ(t.makeOptimal(true), true);

  EXPECT_EQ(t.mat()(0, 0), 1);
  EXPECT_EQ(t.mat()(0, 1), 0);
  EXPECT_EQ(t.mat()(0, 2), Rational(-20, 3));
  EXPECT_EQ(t.mat()(0, 3), 0);
  EXPECT_EQ(t.mat()(0, 4), Rational(-5, 9));
  EXPECT_EQ(t.mat()(0, 5), Rational(-35, 9));

  EXPECT_EQ(t.mat()(1, 0), 0);
  EXPECT_EQ(t.mat()(1, 1), 0);
  EXPECT_EQ(t.mat()(1, 2), Rational(14, 3));
  EXPECT_EQ(t.mat()(1, 3), 1);
  EXPECT_EQ(t.mat()(1, 4), Rational(2, 9));
  EXPECT_EQ(t.mat()(1, 5), Rational(32, 9));

  EXPECT_EQ(t.mat()(2, 0), 0);
  EXPECT_EQ(t.mat()(2, 1), 1);
  EXPECT_EQ(t.mat()(2, 2), Rational(1, 3));
  EXPECT_EQ(t.mat()(2, 3), 0);
  EXPECT_EQ(t.mat()(2, 4), Rational(1, 9));
  EXPECT_EQ(t.mat()(2, 5), Rational(7, 9));
}

TEST(BilpTest, SimpleOptimizeTest) {
  std::vector<RangeConstraint> constraints;
  constraints.emplace_back(Polynomial("x") + 4, 4);
  Polynomial obj = 3 * Polynomial("x");
  ILPSolver solver;
  Tableau t = solver.makeStandardFormTableau(constraints, obj);
  EXPECT_EQ(t.makeOptimal(), true);

  std::vector<Rational> soln = t.getSymbolicSolution();

  EXPECT_EQ(t.varNames()[0], "_x_pos");
  EXPECT_EQ(t.varNames()[1], "_x_neg");
  EXPECT_EQ(soln[0], 0);
  EXPECT_EQ(soln[1], 4);
}

TEST(BilpTest, OptimizeTest2D) {
  std::vector<RangeConstraint> constraints;
  constraints.emplace_back(Polynomial("x") + Polynomial("y") + 2, 4);
  constraints.emplace_back(Polynomial("x") + 1, 4);
  constraints.emplace_back(Polynomial("y") + 2, 5);
  Polynomial obj = -3 * Polynomial("x") + 2 * Polynomial("y");
  ILPSolver solver;
  Tableau t = solver.makeStandardFormTableau(constraints, obj);
  EXPECT_EQ(t.makeOptimal(), true);

  std::vector<Rational> soln = t.getSymbolicSolution();

  EXPECT_EQ(t.varNames()[0], "_x_pos");
  EXPECT_EQ(t.varNames()[1], "_x_neg");
  EXPECT_EQ(t.varNames()[2], "_y_pos");
  EXPECT_EQ(t.varNames()[3], "_y_neg");
  EXPECT_EQ(soln[0], 2);
  EXPECT_EQ(soln[1], 0);
  EXPECT_EQ(soln[2], 0);
  EXPECT_EQ(soln[3], 2);
}

TEST(BilpTest, TrivialILPTest) {
  std::vector<RangeConstraint> constraints;
  constraints.emplace_back(Polynomial("x") + Polynomial("y") + 2, 4);
  constraints.emplace_back(Polynomial("x") + 1, 4);
  constraints.emplace_back(Polynomial("y") + 2, 5);
  Polynomial obj = -3 * Polynomial("x") + 2 * Polynomial("y");
  ILPSolver solver;
  Tableau t = solver.makeStandardFormTableau(constraints, obj);
  ILPResult res = solver.solve(t);

  EXPECT_EQ(res.soln["x"], 2);
  EXPECT_EQ(res.soln["y"], -2);

  EXPECT_EQ(res.obj_val, -10);
}

TEST(BilpTest, ILPTest2D) {
  std::vector<RangeConstraint> constraints;
  constraints.emplace_back(2 * Polynomial("x") + Polynomial("y") + 2, 6);
  constraints.emplace_back(Polynomial("x") + 1, 4);
  constraints.emplace_back(Polynomial("y") + 2, 5);
  Polynomial obj = -3 * Polynomial("x") + 2 * Polynomial("y");
  ILPSolver solver;
  ILPResult res = solver.solve(constraints, obj);

  EXPECT_EQ(res.soln["x"], 2);
  EXPECT_EQ(res.soln["y"], -2);

  EXPECT_EQ(res.obj_val, -10);
}

TEST(BilpTest, Subdivision1D) {
  std::vector<RangeConstraint> constraints;
  constraints.emplace_back(Polynomial("i_0"), 2);
  constraints.emplace_back(Polynomial("i_0") + 2 * Polynomial("k_0"), 5);
  constraints.emplace_back(Polynomial("i_0") + Polynomial("i_1") + Polynomial("k_0"), 35);
  constraints.emplace_back(Polynomial("i_0") + 2 * Polynomial("i_1"), 70);

  std::vector<Polynomial> objectives;
  objectives.emplace_back(Polynomial("i_0"));
  objectives.emplace_back(-Polynomial("i_0"));
  objectives.emplace_back(Polynomial("i_1"));
  objectives.emplace_back(-Polynomial("i_1"));
  objectives.emplace_back(Polynomial("k_0"));
  objectives.emplace_back(-Polynomial("k_0"));
  ILPSolver solver;
  std::map<Polynomial, ILPResult> res = solver.batch_solve(constraints, objectives);

  EXPECT_EQ(res[Polynomial("i_0")].obj_val, 0);
  EXPECT_EQ(res[-Polynomial("i_0")].obj_val, -1);
  EXPECT_EQ(res[Polynomial("i_1")].obj_val, 0);
  EXPECT_EQ(res[-Polynomial("i_1")].obj_val, -34);
  EXPECT_EQ(res[Polynomial("k_0")].obj_val, 0);
  EXPECT_EQ(res[-Polynomial("k_0")].obj_val, -2);
}
}  // namespace bilp
}  // namespace lang
}  // namespace tile
}  // namespace vertexai
