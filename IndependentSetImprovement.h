#ifndef INDEPENDENTSETIMPROVEMENT_H
#define INDEPENDENTSETIMPROVEMENT_H

#include <iostream>
#include <algorithm>
#include <numeric>
#include <random>
#include <math.h> 
#include <map> 

#include "SubsetSelectionAlgorithm.h"

using namespace std;

class IndependentSetImprovement : public SubsetSelectionAlgorithm
{
public:

    /**
     * @brief Constructor
     * @param k: Cardinality constraint
     * @param f: Submodular function
     */
    IndependentSetImprovement(size_t k, SubmodularFunction &f) : SubsetSelectionAlgorithm(k, f)
    {
    
    }

    /**
     * @brief Run the submodular algorithm
     * @param Dataset: Dataset
     * @param iterations: Maximum number of times to traverse the dataset when running the Greedy algorithm
     */
    void run(const vector<Point> &Dataset, unsigned int iterations = 1)
    {
        for (auto &cur_point : Dataset)
        {
            next(cur_point);
        }
    }

    /**
     * @brief Process streaming data
     * @param cur_point: The current point in the data flow
     */
    void next(const Point &cur_point)
    {
        Point t(cur_point);
        double fdelta = f.peek(solution, t, solution.size()) - fval;
        t.fdelta = fdelta;

        if (solution.size() < k)
        {
            //Add to the solution set directly 
            f.update(solution, t, solution.size());   
            fval = f.operator()(solution);

            //Descend order
            f.sort_descend_fdelta(solution);
        }
        else
        {
            if(fdelta > 2*solution.back().fdelta)
            {
                //Replace the point at the end
                f.update(solution, t, solution.size()-1); 
                fval = f.operator()(solution);

                //Descend order
                f.sort_descend_fdelta(solution);
            }
        }
    }

    /**
     * @brief Destructor
     */
    ~IndependentSetImprovement() {}
};

#endif