#ifndef TWEETTEXSUBFUNC_H
#define TWEETTEXSUBFUNC_H

#include <mutex>
#include <vector>
#include <functional>
#include <cmath>
#include <cassert>
#include <iostream>
#include <map>

#include "SubmodularFunction.h"
#include "Point.h"

using namespace std;

/**
 * @brief This submodular function is applicable to the following dataset: “Twitter”
 */

class TweetTexSubFunc : public SubmodularFunction
{  
public:

    //Count the forwarding times of each word in A
    map<string,double> word_to_retweets_A;

    /**
     * @brief Constructor
     */
    TweetTexSubFunc()
    {
        store_A = false;
        query = 0;
    }

    /**
     * @brief Calculate the value of the solution set
     * @param cur_solution : Current solution set
     * @return Value of the solution set
     */
    double operator()(const vector<Point> &cur_solution)
    {
        if(cur_solution.empty())
        {
            return 0;
        }

        //Count the forwarding times of each word
        map<string,double> word_to_retweets;
        for(auto p : cur_solution)
        {
            for(auto word : p.words)
            {
                if(word_to_retweets.count(word))
                {
                    word_to_retweets[word] += p.retweets;
                }
                else
                {
                    word_to_retweets[word] = p.retweets;
                }   
            }
        }

        //Accumulate
        double retweets_all = 0;
        for(auto pair : word_to_retweets)
        {
            retweets_all += sqrt(pair.second);
        }
        return retweets_all;
    }

   /**
     * @brief Calculate the value after adding point to the solution set
     * @param cur_solution : Current solution set
     * @param cur_point : Point that needs to be added to the current solution set
     * @param position : Position to be added
     * @return Value after adding point
     */
    double peek(const vector<Point> &cur_solution, const Point &cur_point, size_t position)
    {
        ++query;

        if(position > cur_solution.size())
        {
            cout<<"The specified position is out of range!!!"<<endl;
            exit(1);
        }

        if(cur_solution.size() == 0)
        {
            return this->operator()({cur_point});
        }

        vector<Point> tmp_solution(cur_solution);
        
        if(position == cur_solution.size())
        {
            //Append
            tmp_solution.push_back(cur_point);              
        }
        else
        {
            //Replace
            tmp_solution[position] = cur_point;
        }

        return this->operator()(tmp_solution);
    }

    /**
     * @brief Update solution set
     * @param cur_solution : Current solution set
     * @param cur_point : Point that needs to be added to the current solution set
     * @param position : Position to be added
     */
    void update(vector<Point> &cur_solution, const Point &cur_point, size_t position) 
    {
        if(position > cur_solution.size())
        {
            cout<<"The specified position is out of range!!!"<<endl;
            exit(1);
        }

        
        if(position == cur_solution.size())
        {
            //Append
            cur_solution.push_back(cur_point);
        }
        else
        {
            //Replace
            cur_solution[position] = cur_point;
        }

        if(store_A)
        {
            //Update A
            A.push_back(cur_point);

            //Update fval_A
            for(auto word : cur_point.words)
            {
                if(word_to_retweets_A.count(word))
                {
                    word_to_retweets_A[word] += cur_point.retweets;
                }
                else
                {
                    word_to_retweets_A[word] = cur_point.retweets;
                }   
            }

            double retweets_all_A = 0;
            for(auto pair : word_to_retweets_A)
            {
                retweets_all_A += sqrt(pair.second);
            }
            fval_A = retweets_all_A;
        }
    }

    /**
     * @brief Arrange the solution set in descending order of fdelta of data points
     * @param cur_solution : Current solution set
     */
    void sort_descend_fdelta(vector<Point> &cur_solution)
    {
        sort(cur_solution.begin(),cur_solution.end(),[&](const Point &a,const Point &b)->bool{
            return a.fdelta > b.fdelta;
        });
    }

    /**
     * @brief Create a new submodular function
     * @return Reference to new submodular function
     */
    SubmodularFunction& new_object()
    {
        TweetTexSubFunc *f = new TweetTexSubFunc(*this);
        return *f;
    }

     /**
     * @brief Only used by FreeDisposal: Calculate the delta_value after adding point to A
     * @param cur_point : Point that needs to be added to the current solution set
     * @return Value after adding point
     */
    double peek_delta_A(const Point &cur_point)
    {
        ++query;

        if(!store_A)
        {
            cout << "Did not store A!!! \n";
            exit(1);
        }

        map<string,double> temp_word_to_retweets_A(word_to_retweets_A);

        for(auto word : cur_point.words)
        {
            if(temp_word_to_retweets_A.count(word))
            {
                temp_word_to_retweets_A[word] += cur_point.retweets;
            }
            else
            {
                temp_word_to_retweets_A[word] = cur_point.retweets;
            }   
        }

        double retweets_all = 0;
        for(auto pair : temp_word_to_retweets_A)
        {
            retweets_all += sqrt(pair.second);
        }

        return retweets_all - fval_A;
    }

    /**
     * @brief Calculate the delta_value after adding point to A \cap S
     * @param cur_solution : Current solution set
     * @param cur_point : Point that needs to be added to the current solution set
     * @return Value after adding point
     */
    double peek_delta_A_cap_S(const vector<Point> &cur_solution, const Point &cur_point)
    {
        ++query;

        //Find points in A \cap S
        vector<Point> A_cap_S;//The positions in S of points in A \cap S
        int cur_id = cur_point.id;//The position of cur_point in A
        for(auto p : cur_solution)
        {
            if(p.id<cur_id)
            {
                A_cap_S.push_back(p);
            }
        }

        double before = this->operator()(A_cap_S);
        A_cap_S.push_back(cur_point);
        double after = this->operator()(A_cap_S);

        return after-before;
    }

    /**
     * @brief Destructor
     */
    ~TweetTexSubFunc() {}
};

#endif // TWEETTEXSUBFUNC_H