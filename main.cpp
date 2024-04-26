#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <cmath>
#include <map>
#include <algorithm>
#include <list>

#include "Point.h"
#include "IOUtil.h"
#include "GauVecSubFunc.h"
#include "LapVecSubFunc.h"
#include "TweetTexSubFunc.h"
#include "Greedy.h"
#include "IndependentSetImprovement.h"
#include "StreamingGreedy.h"
#include "Preemption.h"
#include "FreeDisposal.h"
#include "OnlineAdaptive.h"
#include "OnlineNonAdaptive.h"

using namespace std;

/**
 * @brief Evaluate the performance of an algorithm on a dataset
 * @param alg: The algorithm to be evaluated
 * @param Dataset: The dataset where the algorithm to be evaluated 
 * @return Tuple: function value of solution set, running time, size of the solution set, total oracles
*/
auto evaluate_algorithm(SubsetSelectionAlgorithm &alg, const vector<Point> &Dataset)
{
    auto start = chrono::steady_clock::now();
    alg.run(Dataset);
    auto end = chrono::steady_clock::now();
    chrono::duration<double> runtime_seconds = end - start;
    double fval = alg.fval;
    int query = alg.f.query;
    return make_tuple(fval, runtime_seconds.count(),alg.solution.size(),query);
}

/**
 * @brief Compare different algorithms on a dataset to maximize a submodular function
 * @param f: The submodular function to be maximized
 * @param Dataset: The dataset where different algorithms to be compared
*/
void run_algorithms(SubmodularFunction &f, const vector<Point> &Dataset)
{
    //Record the results of each algorithm: function value of solution set, running time, size of solution set, total oracles
    tuple<double, double,size_t,int> res;  

    //Output path of algorithm result 
    vector<string> outfiles = {
        "result/test/test.txt",
    };

    ofstream outfile;
    
    //Cardinality constraint
    vector<size_t> ks = {10,20,30,40,50};
    // vector<size_t> ks = {30};
    for(auto k : ks)
    {
        //Open output file
        outfile.open(outfiles[0], ios::out|ios::app);

        // Greedy
        Greedy my_Greedy(k, f);
        res = evaluate_algorithm(my_Greedy, Dataset);
        cout << "Greedy:\t Selecting " << k << " points" << "\t fval:\t" << get<0>(res) << "\t runtime:\t" << get<1>(res) << "\t queries:\t"<< get<3>(res)<< endl;
        outfile << "Greedy:\t Selecting " << k << " points" << "\t fval:\t" << get<0>(res) << "\t runtime:\t" << get<1>(res) << "\t queries:\t"<< get<3>(res)<< endl;    

        // IndependentSetImprovement
        IndependentSetImprovement my_IndependentSetImprovement(k, f);
        res = evaluate_algorithm(my_IndependentSetImprovement, Dataset);
        cout << "IndependentSetImprovement:\t Selecting " << k << " points" << "\t fval:\t" << get<0>(res) << "\t runtime:\t" << get<1>(res) <<"\t queries:\t"<< get<3>(res)<< endl;
        outfile << "IndependentSetImprovement:\t Selecting " << k << " points" << "\t fval:\t" << get<0>(res) << "\t runtime:\t" << get<1>(res) <<"\t queries:\t"<< get<3>(res)<< endl;     

        // StreamingGreedy
        StreamingGreedy my_StreamingGreedy(k, f);
        res = evaluate_algorithm(my_StreamingGreedy, Dataset);
        cout << "StreamingGreedy:\t Selecting " << k << " points" << "\t fval:\t" << get<0>(res) << "\t runtime:\t" << get<1>(res) <<"\t queries:\t"<< get<3>(res)<< endl;
        outfile << "StreamingGreedy:\t Selecting " << k << " points" << "\t fval:\t" << get<0>(res) << "\t runtime:\t" << get<1>(res) <<"\t queries:\t"<< get<3>(res)<< endl; 

        // Preemption
        auto c_Preemption = {1.0};
        for(auto c: c_Preemption)
        {
            Preemption my_Preemption(k, f, c);
            res = evaluate_algorithm(my_Preemption, Dataset);
            cout << "Preemption:\t Selecting " << k << " points with c = " << c << "\t fval:\t" << get<0>(res) << "\t runtime:\t" << get<1>(res) <<"\t queries:\t"<< get<3>(res)<< endl;
            outfile << "Preemption:\t Selecting " << k << " points with c = " << c << "\t fval:\t" << get<0>(res) << "\t runtime:\t" << get<1>(res) <<"\t queries:\t"<< get<3>(res)<< endl;
        }
        
        //FreeDisposal
        FreeDisposal my_FreeDisposal(k, f);
        res = evaluate_algorithm(my_FreeDisposal, Dataset);
        cout << "FreeDisposal:\t Selecting " << k <<"->"<<get<2>(res)<< " points \t fval:\t" << get<0>(res) << "\t runtime:\t" << get<1>(res) <<"\t queries:\t"<< get<3>(res)<< endl;
        outfile << "FreeDisposal:\t Selecting " << k <<"->"<<get<2>(res)<< " points \t fval:\t" << get<0>(res) << "\t runtime:\t" << get<1>(res) <<"\t queries:\t"<< get<3>(res)<< endl;
   
        //OnlineAdaptive
        auto r_OnlineAdaptive = {1.0, 3.0, 5.0, 7.0, 9.0, 1.0*k};
        for(auto r: r_OnlineAdaptive)
        {
            OnlineAdaptive my_OnlineAdaptive(k, f, r);
            res = evaluate_algorithm(my_OnlineAdaptive, Dataset);
            cout << "OnlineAdaptive:\t Selecting " << k <<"->"<<get<2>(res)<< " points with r = " << r << "\t fval:\t" << get<0>(res) << "\t runtime:\t" << get<1>(res)  <<"\t queries:\t"<< get<3>(res)<< endl;
            outfile << "OnlineAdaptive:\t Selecting " << k <<"->"<<get<2>(res)<< " points with r = " << r << "\t fval:\t" << get<0>(res) << "\t runtime:\t" << get<1>(res)  <<"\t queries:\t"<< get<3>(res)<< endl;   
        }       

        //OnlineNonAdaptive
        auto r_OnlineNonAdaptive = {1.0, 3.0, 5.0, 7.0, 9.0};
        for(auto r: r_OnlineNonAdaptive)
        {
            OnlineNonAdaptive my_OnlineNonAdaptive(k, f, r);
            res = evaluate_algorithm(my_OnlineNonAdaptive, Dataset);
            cout << "OnlineNonAdaptive:\t Selecting " << k <<"->"<<get<2>(res)<< " points with r = " << r << "\t fval:\t" << get<0>(res) << "\t runtime:\t" << get<1>(res)  <<"\t queries:\t"<< get<3>(res)<< endl;
            outfile << "OnlineNonAdaptive:\t Selecting " << k <<"->"<<get<2>(res)<< " points with r = " << r << "\t fval:\t" << get<0>(res) << "\t runtime:\t" << get<1>(res)  <<"\t queries:\t"<< get<3>(res)<< endl;   
        }  


        cout << endl;
        outfile << endl;
        if(k == ks.back())
        {
            cout << endl;
            outfile << endl;
        }
        outfile.close();
    }
}

int main()
{
    //When adding a new dataset, it is necessary to add both the reading method and the definition of the submodular function
    //Just as examples to illustrate the format of the datasets, each of these datasets with "_sampled" only contain 50 elements sampled from the original dataset
    vector<string> file_paths = {
            "dataset/ForestCover_sampled.txt",
            "dataset/CreditCardFraud_sampled.txt",
            "dataset/KDDCup99_sampled.txt",
            "dataset/YouTube_sampled.txt",
            "dataset/Twitter_sampled.txt",
    };
    for (size_t count = 0; count < file_paths.size(); ++count)
    {
        //Read datasets
        string file_path = file_paths[count];
        size_t dim;
        vector<Point> Dataset;
        cout << "Loading data from " << file_path << endl;
        if(
            file_path == "dataset/ForestCover_sampled.txt"||
            file_path == "dataset/CreditCardFraud_sampled.txt"||
            file_path == "dataset/KDDCup99_sampled.txt"||
            file_path == "dataset/YouTube_sampled.txt"
        )
        {
            IOUtil::read_vectors(file_path.c_str(), dim, Dataset);
            cout << "The dimension of Dataset is " << dim << endl;
            cout << "The size of Dataset is " << Dataset.size() << endl;
            cout << endl;
        }
        else if(file_path == "dataset/Twitter_sampled.txt")
        {
            IOUtil::read_words(file_path.c_str(), Dataset);
            cout << "The size of Dataset is " << Dataset.size() << endl;
            cout << endl;
        }
        else
        {
            cout << "Reading method for dataset " << file_path << " is not specified!!!" << endl;
            exit(1);
        }
        
        //Specify submodular functions
        if(
            file_path == "dataset/ForestCover_sampled.txt"||
            file_path == "dataset/CreditCardFraud_sampled.txt"||
            file_path == "dataset/KDDCup99_sampled.txt"
        )
        {
            GauVecSubFunc f(dim);
            run_algorithms(f, Dataset);
        }
        else if(file_path == "dataset/YouTube_sampled.txt")
        {
            LapVecSubFunc f;
            run_algorithms(f, Dataset);
        }
        else if(file_path == "dataset/Twitter_sampled.txt")
        {
            TweetTexSubFunc f;
            run_algorithms(f, Dataset);
        }
        else
        {
            cout << "Submodular function for dataset " << file_path << " is not specified!!!" << endl;
            exit(1);
        }
    }
}