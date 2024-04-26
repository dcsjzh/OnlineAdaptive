#ifndef PREEMPTION_H
#define PREEMPTION_H

#include <iostream>
#include <algorithm>
#include <numeric>
#include <random>
#include <unordered_set>

#include "SubsetSelectionAlgorithm.h"

using namespace std;

class Preemption : public SubsetSelectionAlgorithm
{
public:

    //Parameter that determines the threshold
    double c;

    /**
     * @brief Constructor
     * @param k: Cardinality constraint
     * @param f: Submodular function
     * @param c: Parameter that determine the threshold
     */
    Preemption(size_t k, SubmodularFunction &f, double c) : SubsetSelectionAlgorithm(k, f), c(c)
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
        if (solution.size() < k)
        {
            //Add the first k points directly to the solution set
            f.update(solution, cur_point, solution.size());
            fval = f.operator()(solution);
        }
        else
        {
            //Record the information of the replaced point
            double fval_max = 0;//Value     
            size_t fval_max_position = 0;//The position in the solution set

            //Traverse the solution set
            for (size_t i = 0; i < k; ++i)
            {
                double fval_temp = f.peek(solution, cur_point, i);

                //Update information of the replaced point
                if(fval_temp > fval_max)
                {
                    fval_max = fval_temp;
                    fval_max_position = i;
                }
            }

            if (fval_max - fval >= c * fval / k)
            {
                //Perform the replacement
                f.update(solution, cur_point, fval_max_position);
                fval = fval_max;
            }
        }
    }

    /**
     * @brief Destructor
     */
    ~Preemption() {}
};

#endif