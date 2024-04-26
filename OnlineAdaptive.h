#ifndef ONLINEADAPTIVE_H
#define ONLINEADAPTIVE_H

#include <iostream>
#include <algorithm>
#include <numeric>
#include <random>
#include <math.h> 
#include <unordered_set>
#include <unordered_map>

#include "SubsetSelectionAlgorithm.h"

using namespace std;

class OnlineAdaptive : public SubsetSelectionAlgorithm
{
public:
    //Parameters that determine the threshold
    double beta;
    double eta;

    //Weighted sum of marginal gains in the solution set
    double tau = 0;

    //As the solution set increases, parameter alpha expands to "r" times its original size
    double r;

    /**
     * @brief Constructor
     * @param k: Cardinality constraint
     * @param f: Submodular function
     * @param r: Relaxation parameter
     */
    OnlineAdaptive(size_t k, SubmodularFunction &f, double r) : SubsetSelectionAlgorithm(k, f), r(r)
    {
        //eta is the positive root of the equation $(1+x)^{(k+1)}=kx+x+2$
        //Store some values of eta in advance
        map<int,double> k_to_eta;
        k_to_eta[10] = 0.112823;
        k_to_eta[20] = 0.0568559;
        k_to_eta[30] = 0.0380041;
        k_to_eta[40] = 0.0285408;
        k_to_eta[50] = 0.0228508;
        k_to_eta[60] = 0.0190525;
        k_to_eta[70] = 0.0163369;
        k_to_eta[80] = 0.0142988;
        k_to_eta[90] = 0.0127129;
        k_to_eta[100] = 0.0114436;
        k_to_eta[110] = 0.0104048;
        k_to_eta[120] = 0.0095389;
        k_to_eta[130] = 0.00880604;
        k_to_eta[140] = 0.00817776;
        k_to_eta[150] = 0.00763315;

        eta=k_to_eta[k];

        if(eta == 0.0)
        {
            cout<<"The parameter value corresponding to k has not been specified yet!!!"<<endl;
            exit(1);
        }
        beta=(1+k*eta) / (pow(1+eta, k)-1);
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

        if(fdelta < beta*tau/k)
        {
            return;
        }

        if (solution.size() < k)
        {
            //Add to the solution set directly 
            f.update(solution, t, solution.size());   
            fval = f.operator()(solution);
        }
        else
        {
            //Replace the point at the end
            f.update(solution, t, solution.size()-1); 
            fval = f.operator()(solution);
        }
        double exponent = log(log(k)/log(1.2))/log(2);
        double alpha = min(exp(pow(solution.size(), exponent)*log(r)/pow(k,exponent))*eta,1.0);
        beta = (1+k*alpha) / (pow(1+alpha, k)-1);
        
        //Descend order
        f.sort_descend_fdelta(solution);
        
        //Update tau
        tau = 0;
        for(size_t i = 0 ; i < solution.size(); ++i)
        {
            tau += pow(1+alpha, i) * solution[i].fdelta;
        }
    }

    /**
     * @brief Destructor
     */
    ~OnlineAdaptive() {}
};

#endif