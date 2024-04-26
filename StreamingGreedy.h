#ifndef STREAMINGGREEDY_H
#define STREAMINGGREEDY_H

#include <iostream>
#include <algorithm>
#include <numeric>
#include <random>
#include <math.h> 
#include <map> 

#include "SubsetSelectionAlgorithm.h"

using namespace std;

class StreamingGreedy : public SubsetSelectionAlgorithm
{
public:

    /**
     * @brief Constructor
     * @param k: Cardinality constraint
     * @param f: Submodular function
     */
    StreamingGreedy(size_t k, SubmodularFunction &f) : SubsetSelectionAlgorithm(k, f)
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
            double fdelta_min;//Value     
            size_t fdelta_min_position;//The position in the solution set
            //Traverse the solution set
            for (size_t i = 0; i < solution.size(); ++i)
            {
                double fdelta_temp = f.peek_delta_A_cap_S(solution, solution[i]);

                //Update information of the replaced point
                if(i == 0)
                {
                    fdelta_min = fdelta_temp;
                    fdelta_min_position = i;
                }
                else if(fdelta_temp < fdelta_min)
                {
                    fdelta_min = fdelta_temp;
                    fdelta_min_position = i;
                }
            }

            double fdelta = f.peek(solution, cur_point, solution.size()) - fval;
            if(fdelta >= 2*fdelta_min)
            {
                //Perform the replacement
                f.update(solution, cur_point, fdelta_min_position);
                fval = f.operator()(solution);
            }           
        }
    }

    /**
     * @brief Destructor
     */
    ~StreamingGreedy() {}
};

#endif