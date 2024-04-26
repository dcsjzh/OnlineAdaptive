#ifndef SUBMODULARFUNCTION_H
#define SUBMODULARFUNCTION_H

#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>
#include <functional>
#include <cassert>
#include <iostream>

#include "Point.h"

using namespace std;

//Pure virtual class, the base class of various submodular functions
class SubmodularFunction {
public:

    //The set of all points that was added to the solution set
    vector<Point> A;
    //Whether the algorithm needs to store A
    bool store_A;
    //Value of A
    double fval_A;

    //Query times of the submodular algorithm
    int query;
    
    /**
     * @brief Calculate the value of the solution set
     * @param cur_solution : Current solution set
     * @return Value of the solution set
     */
    virtual double operator()(const vector<Point> &cur_solution)=0;

    /**
     * @brief Calculate the value after adding point to the solution set
     * @param cur_solution : Current solution set
     * @param cur_point : Point that needs to be added to the current solution set
     * @param position : Position to be added
     * @return Value after adding point
     */
    virtual double peek(const vector<Point> &cur_solution, const Point &cur_point, size_t position)=0;

    /**
     * @brief Update solution set
     * @param cur_solution : Current solution set
     * @param cur_point : Point that needs to be added to the current solution set
     * @param position : Position to be added
     */
    virtual void update(vector<Point> &cur_solution, const Point &cur_point, size_t position)=0;

    /**
     * @brief Arrange the solution set in descending order of fdelta of data points
     * @param cur_solution : Current solution set
     */
    virtual void sort_descend_fdelta(vector<Point> &cur_solution)=0;

    /**
     * @brief Create a new submodular function
     * @return Reference to new submodular function
     */
    virtual SubmodularFunction& new_object()=0;

    /**
     * @brief Only used by FreeDisposal: Calculate the delta_value after adding point to A
     * @param cur_point : Point that needs to be added to the current solution set
     * @return Value after adding point
     */
    virtual double peek_delta_A(const Point &cur_point)=0;

    /**
     * @brief Only used by FreeDisposal: Calculate the delta_value after adding point to A \cap S
     * @param cur_solution : Current solution set
     * @param cur_point : Point that needs to be added to the current solution set
     * @return Value after adding point
     */
    virtual double peek_delta_A_cap_S(const vector<Point> &cur_solution, const Point &cur_point)=0;
    
    /**
     * @brief Destructor
     */
    virtual ~SubmodularFunction() {}
};

#endif // SUBMODULARFUNCTION_H
