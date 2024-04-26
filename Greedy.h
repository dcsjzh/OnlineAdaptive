#ifndef GREEDY_H
#define GREEDY_H

#include <iostream>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <optional>
#include <list>

#include "SubsetSelectionAlgorithm.h"

using namespace std;

class Greedy : public SubsetSelectionAlgorithm
{
public:

    /**
     * @brief Constructor
     * @param k: Cardinality constraint
     * @param f: Submodular function 
     */
    Greedy(size_t k, SubmodularFunction &f) : SubsetSelectionAlgorithm(k, f)
    {

    }

    /**
     * @brief Run the submodular algorithm
     * @param Dataset: Dataset
     * @param iterations: Maximum number of times to traverse the dataset when running the Greedy algorithm
     */
    void run(const vector<Point> &Dataset, unsigned int iterations = 1)
    {
        //Initialize the remaining points in the original dataset, with a range of [0, Dataset.size()-1]
        list<size_t> Dataset_remaining(Dataset.size());
        int id_temp = 0;
        for (auto it = Dataset_remaining.begin(); it != Dataset_remaining.end(); ++it)
        {
            *it = id_temp;
            ++id_temp;
        }

        //Start multiple rounds of traversal
        while (solution.size() < k && Dataset_remaining.size() > 0)
        {
            //Record the information of the point with maximum marginal gain among the current remaining points
            double fval_max = 0;//marginal gain     
            size_t fval_max_id = Dataset_remaining.front();//The position in the original dataset 
            auto fval_max_it = Dataset_remaining.begin();//Iterator 

            //Traverse the remaining dataset
            for (auto it = Dataset_remaining.begin(); it != Dataset_remaining.end(); ++it)
            {
                double fval_temp = f.peek(solution, Dataset[*it], solution.size());

                //Update the information
                if(fval_temp > fval_max)
                {
                    fval_max = fval_temp;
                    fval_max_id = *it;
                    fval_max_it = it;
                }
            }

            //Add the point with maximum marginal gain to the solution set
            f.update(solution, Dataset[fval_max_id], solution.size());

            //Remove this point from the remaining dataset
            Dataset_remaining.erase(fval_max_it);

            //Update the value of the current solution set
            fval = fval_max;
        }
    }

    /**
     * @brief Process streaming data, but Greedy does not support it and will throw an exception
     * @param cur_point: The current point in the data flow
     */
    void next(const Point &cur_point)
    {
        throw runtime_error("Greedy does not support streaming data, please use run().");
    }

    /**
     * @brief Destructor
     */
    ~Greedy() {}
};

#endif // GREEDY_H
