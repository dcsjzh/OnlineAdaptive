#ifndef FREEDISPOSAL_H
#define FREEDISPOSAL_H

#include <iostream>
#include <algorithm>
#include <numeric>
#include <random>
#include <math.h> 
#include <map> 

#include "SubsetSelectionAlgorithm.h"

using namespace std;

class FreeDisposal : public SubsetSelectionAlgorithm
{
public:

    //Parameters that determine the thresholds
    double alpha, beta, gamma;

    /**
     * @brief Constructor
     * @param k: Cardinality constraint
     * @param f: Submodular function
     */
    FreeDisposal(size_t k, SubmodularFunction &f) : SubsetSelectionAlgorithm(k, f)
    {
        this->f.store_A=true;
        //alpha is the unique root in the interval (3, 4) of the equation: $x = (1+\frac{x - 2}{k+1})^{k+1}$
        //Store some values of alpha in advance
        map<int,double> k_to_alpha;
        k_to_alpha[10] = 3.24105;
        k_to_alpha[20] = 3.19397;
        k_to_alpha[30] = 3.17813;
        k_to_alpha[40] = 3.17017;
        k_to_alpha[50] = 3.16539;
        k_to_alpha[60] = 3.16220;
        k_to_alpha[70] = 3.15992;
        k_to_alpha[80] = 3.15821;
        k_to_alpha[90] = 3.15687;
        k_to_alpha[100] = 3.15581;

        alpha=k_to_alpha[k];

        if(alpha == 0.0)
        {
            cout<<"The parameter value corresponding to k has not been specified yet!!!"<<endl;
            exit(1);
        }

        beta=1+(alpha-2)/(k+1);
        gamma=k*(beta-1)/(1-pow(beta,-k));
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
     * @param cur_point: The current point in the data stream
     */
    void next(const Point &cur_point)
    {
        double W=0;

        //W is equal to f(S)-f({})
        W=fval-f.operator()({});

        double fdelta=f.peek_delta_A(cur_point);
        if(fdelta >= gamma/k*W)
        {
            if (solution.size()<k)
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

                    //Update the information on the replaced point
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

                //Perform the replacement
                f.update(solution, cur_point, fdelta_min_position);
                fval = f.operator()(solution);          
            }
        }
    }

    /**
     * @brief Destructor
     */
    ~FreeDisposal() {}
};

#endif