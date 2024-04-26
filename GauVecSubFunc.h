#ifndef GAUVECSUBFUNC_H
#define GAUVECSUBFUNC_H

#include <mutex>
#include <vector>
#include <functional>
#include <cmath>
#include <cassert>
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Core>

#include "SubmodularFunction.h"
#include "Point.h"

using namespace std;
using namespace Eigen;

/**
 * @brief This submodular function is applicable to the following datasets: “ForestCover”，“CreditCardFraud”，“KDDCup99”
 */

class GauVecSubFunc : public SubmodularFunction
{  
public:

    //Parameters
    double l;
    double a;

    //Data stored for acceleration to avoid repeated calculations
    //The matrix composed of the current solution set
    Matrix<double,Dynamic,Dynamic> M;
    //The position of the point with id in the current solution set
    map<size_t,int> id_to_position;
    //Value of the current solution set
    double fval;

    //The matrix composed of A
    Matrix<double,Dynamic,Dynamic> M_A;

    /**
     * @brief Constructor
     * @param dimension : Vector dimension
     */
    GauVecSubFunc(int dimension)
    {
        l = 1/(2*sqrt(dimension));
        a = 1;
        fval = 0;
        store_A=false;
        query=0;
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
 
        return fval;
    }

    /**
     * @brief Calculate the value after adding a point to the solution set
     * @param cur_solution : Current solution set
     * @param cur_point : Point that is added to the current solution set
     * @param position : Position for the point to be added
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
            return log(1+a)/2;
        }

        if(position == cur_solution.size())
        {          
            //Append

            //Update M_temp
            Matrix<double,Dynamic,Dynamic> M_temp(M.rows()+1,M.cols()+1);//Expand one row and one column
            //Copy original M to M_temp
            for(int i = 0; i < position; ++i)
            {   
                M_temp(i,i) = 1+a;
                for(int j = i+1; j < position; ++j)
                {
                    M_temp(i,j) = M(i,j);
                    M_temp(j,i) = M_temp(i,j);
                }
            }
            //Add
            M_temp(position,position) = 1+a;
            for(int i = 0; i < position; ++i)
            {                  
                double distance = cur_solution[i].distance_to(cur_point);
                M_temp(i,position) = a*exp(-(distance*distance)/(2*l*l));
                M_temp(position,i) = M_temp(i,position);  
            }

            return log(M_temp.determinant())/2;
        }
        else
        {
            //Replace

            //Update M_temp
            Matrix<double,Dynamic,Dynamic> M_temp(M);//Size unchanged
            for(int i = 0; i < cur_solution.size(); ++i)
            {   
                if(i != position)
                {
                    double distance = cur_solution[i].distance_to(cur_point);
                    M_temp(i,position) = a*exp(-(distance*distance)/(2*l*l));
                    M_temp(position,i) = M_temp(i,position);  
                }
            }

            return log(M_temp.determinant())/2;
        }
    }

    /**
     * @brief Update the solution set
     * @param cur_solution : Current solution set
     * @param cur_point : Point that needs to be added to the current solution set
     * @param position : Position to be added
     */
    void update(vector<Point> &cur_solution, Point const &cur_point, size_t position) 
    {
        if(position > cur_solution.size())
        {
            cout<<"The specified position is out of range!!!"<<endl;
            exit(1);
        }

        if(position == cur_solution.size())
        {
            //Append

            //Update id_to_position
            id_to_position[cur_point.id] = position;

            //Update the solution set
            cur_solution.push_back(cur_point);        

            //Update M
            //Save original data of M 
            Matrix<double,Dynamic,Dynamic> M_temp(M);
            //M expands one row and one column, resulting in loss of original data
            M.resize(M.rows()+1,M.cols()+1);
            //Copy original data to M
            for(int i = 0; i < position; ++i)
            {   
                M(i,i) = 1+a;
                for(int j = i+1; j < position; ++j)
                {
                    M(i,j) = M_temp(i,j);
                    M(j,i) = M(i,j);
                }
            }
            //Add
            M(position,position) = 1+a;
            for(int i = 0; i < position; ++i)
            {                  
                double distance = cur_solution[i].distance_to(cur_point);
                M(i,position) = a*exp(-(distance*distance)/(2*l*l));
                M(position,i) = M(i,position);  
            }
        }
        else
        {
            //Replace

            //Update id_to_position
            id_to_position.erase(cur_solution[position].id);
            id_to_position[cur_point.id] = position;

            //Update the solution set
            cur_solution[position] = cur_point;  

            //Update M
            for(int i=0; i<cur_solution.size(); ++i)
            {   
                if(i != position)
                {
                    double distance = cur_solution[i].distance_to(cur_point);
                    M(i,position) = a*exp(-(distance*distance)/(2*l*l));
                    M(position,i) = M(i,position);  
                }
            }
        }

        //Update fval
        fval = log(M.determinant())/2;

        if(store_A)
        {
            //Update A
            A.push_back(cur_point);

            //Update M_A
            //Save original data of M_A 
            Matrix<double,Dynamic,Dynamic> M_A_temp(M_A);
            //M_A expands one row and one column, resulting in loss of original data
            M_A.resize(M_A.rows()+1,M_A.cols()+1);
            //Copy original data to M_A
            for(int i = 0; i < M_A.rows()-1; ++i)
            {   
                M_A(i,i) = 1+a;
                for(int j = i+1; j < M_A.rows()-1; ++j)
                {
                    M_A(i,j) = M_A_temp(i,j);
                    M_A(j,i) = M_A(i,j);
                }
            }
            //Add
            M_A(M_A.rows()-1,M_A.rows()-1) = 1+a;
            for(int i = 0; i < M_A.rows()-1; ++i)
            {                  
                double distance = A[i].distance_to(cur_point);
                M_A(i,M_A.rows()-1) = a*exp(-(distance*distance)/(2*l*l));
                M_A(M_A.rows()-1,i) = M_A(i,M_A.rows()-1);  
            }

            //Update fval_A
            fval_A = log(M_A.determinant())/2;
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

        //Update M
        Matrix<double,Dynamic,Dynamic> M_temp(M);
        int S_size = cur_solution.size();
        for(int i = 0; i < S_size; ++i)
        {   
            M(i,i) = 1+a;
            for(int j = i+1; j < S_size; ++j)
            {
                M(i,j) = M_temp(id_to_position[cur_solution[i].id],id_to_position[cur_solution[j].id]);
                M(j,i) = M(i,j);
            }
        }

        //Update id_to_position
        for(int i = 0; i < S_size; ++i)
        {
            id_to_position[cur_solution[i].id] = i;
        }
    }

    /**
     * @brief Create a new submodular function
     * @return Reference to the new submodular function
     */
    SubmodularFunction& new_object()
    {
        GauVecSubFunc *f = new GauVecSubFunc(*this);
        return *f;
    }

    /**
     * @brief Calculate the delta_value after adding a point to A
     * @param cur_point : Point that is added to the current solution set
     * @return The delta_value after adding the point
     */
    double peek_delta_A(const Point &cur_point)
    {
        ++query;

        if(!store_A)
        {
            cout << "Did not store A!!! \n";
            exit(1);
        }

        //Append

        //Update M_A_temp
        int M_A_size = M_A.rows();
        Matrix<double,Dynamic,Dynamic> M_A_temp(M_A_size+1,M_A_size+1);//Expand one row and one column
        //Copy original M_A to M_A_temp
        for(int i=0;i<M_A_size;++i)
        {   
            M_A_temp(i,i) = 1+a;
            for(int j = i+1; j < M_A_size; ++j)
            {
                M_A_temp(i,j) = M_A(i,j);
                M_A_temp(j,i) = M_A_temp(i,j);
            }
        }
        //Add
        M_A_temp(M_A_size,M_A_size) = 1+a;
        for(int i = 0; i < M_A_size; ++i)
        {                  
            double distance = A[i].distance_to(cur_point);
            M_A_temp(i,M_A_size) = a*exp(-(distance*distance)/(2*l*l));
            M_A_temp(M_A_size,i) = M_A_temp(i,M_A_size);  
        }

        return log(M_A_temp.determinant())/2 - fval_A;
    }

    /**
     * @brief Calculate the delta_value after adding point to A \cap S
     * @param cur_solution : Current solution set
     * @param cur_point : Point that is added to the current solution set
     * @return Value after adding the point
     */
    double peek_delta_A_cap_S(const vector<Point> &cur_solution, const Point &cur_point)
    {
        ++query;

        if(id_to_position.count(cur_point.id) == 0)
        {
            cout << "This point dosen't in the solution!!! \n";
            exit(1);
        }

        //Find points in A \cap S
        vector<size_t> A_cap_S_position;//The positions in S of points in A \cap S
        int cur_id = cur_point.id;//The position of cur_point in A
        for(auto p : cur_solution)
        {
            if(p.id < cur_id)
            {
                A_cap_S_position.push_back(id_to_position[p.id]);//The positions in S
            }
        }

        //Calculate value of A \cap S
        int M_S_cap_A_size = A_cap_S_position.size();
        Matrix<double,Dynamic,Dynamic> M_S_cap_A(M_S_cap_A_size,M_S_cap_A_size);
        for(int i = 0; i < M_S_cap_A_size; ++i)
        {      
            M_S_cap_A(i,i) = 1+a;
            for(int j = i+1; j < M_S_cap_A_size; ++j)
            {
                M_S_cap_A(i,j) = M(A_cap_S_position[i],A_cap_S_position[j]);
                M_S_cap_A(j,i) = M_S_cap_A(i,j);
            }
        }

        double fval_A_cap_S = log(M_S_cap_A.determinant())/2;

        //Add point 
        Matrix<double,Dynamic,Dynamic> M_S_cap_A_temp(M_S_cap_A_size+1,M_S_cap_A_size+1);//Expand one row and one column
        //Copy original M_S_cap_A to M_S_cap_A_temp
        for(int i = 0; i < M_S_cap_A_size; ++i)
        {   
            M_S_cap_A_temp(i,i) = 1+a;
            for(int j = i+1; j < M_S_cap_A_size; ++j)
            {
                M_S_cap_A_temp(i,j) = M_S_cap_A(i,j);
                M_S_cap_A_temp(j,i) = M_S_cap_A_temp(i,j);
            }
        }
        //Add
        M_S_cap_A_temp(M_S_cap_A_size,M_S_cap_A_size) = 1+a;
        for(int i = 0; i < M_S_cap_A_size; ++i)
        {                  
            M_S_cap_A_temp(i,M_S_cap_A_size) = M(A_cap_S_position[i],id_to_position[cur_point.id]);
            M_S_cap_A_temp(M_S_cap_A_size,i) = M_S_cap_A_temp(i,M_S_cap_A_size);  
        }

        return log(M_S_cap_A_temp.determinant())/2 - fval_A_cap_S;
    }

    /**
     * @brief Destructor
     */
    ~GauVecSubFunc() {}
};

#endif // GASVECSUBFUNC_H