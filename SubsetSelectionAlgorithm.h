#ifndef SUBSETSELECTIONALGORITHM_H
#define SUBSETSELECTIONALGORITHM_H

#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>
#include <functional>
#include <cassert>
#include <optional>
#include <iostream>

#include "SubmodularFunction.h"

using namespace std;

//The base class of various submodular algorithms
class SubsetSelectionAlgorithm
{

public:
    //Cardinality constraint
    size_t k;

    //Submodular function
    SubmodularFunction &f;

    //Solution set
    vector<Point> solution;

    //The value of the current solution set
    double fval;

   /**
     * @brief Constructor
     * @param k: Cardinality constraint
     * @param f: Submodular function
     */
    SubsetSelectionAlgorithm(size_t k, SubmodularFunction &f) : k(k),f(f.new_object())
    {
        fval = 0;
    }

    /**
     * @brief Run the submodular algorithm
     * @param Dataset: Dataset
     * @param iterations: Maximum number of times to traverse the dataset when running the Greedy algorithm
     */
    virtual void run(const vector<Point> &Dataset, unsigned int iterations = 1) = 0;

    /**
     * @brief Process streaming data
     * @param cur_point: The current point in the data flow
     */
    virtual void next(const Point &cur_point) = 0;

    /**
     * @brief Destructor
     */
    virtual ~SubsetSelectionAlgorithm()
    {
        delete &f;
    }
};

#endif // SUBSETSELECTIONALGORITHM_H
