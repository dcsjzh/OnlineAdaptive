#ifndef __POINT_H__
#define __POINT_H__

#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>

using namespace std;

class Point
{
public:

    //0 represents a numerical vector, 1 represents a non numerical vector
    int type;

    //Dimension
    size_t dimension;

    //The position in the dataset and it counts from 0
    size_t id;

    //Used for numerical vector
    //Numerical components
    vector<double> coordinates;

    //Marginal gain of the point when it is accepted
    double fdelta = 0;

    //Used for non numerical vector，such as the "Twitter" dataset 
    //Non numerical components
    vector<string> words;
    //Retweets
    double retweets;

    /**
     * @brief Constructor, default to a numerical vector
     */
    Point()
    {
        this->type = 0;
    }

    /**
     * @brief Constructor. Default to a numerical vector
     * @param dimension: The dimension of the vector 
     */
    Point(size_t dimension)
    {
        this->dimension = dimension;
        this->type = 0;
        for(size_t i = 0; i < dimension; ++i)
        {
            coordinates.push_back(0);
        }
    }

    /**
     * @brief Constructor for a numerical vectors
     * @param id: The position in the dataset
     * @param dimension: The dimension of the vector 
     * @param coordinates: The numerical components of the vector
     */
    Point(size_t id, size_t dimension, vector<double>& coordinates)
    {
        assert(dimension == coordinates.size());

        this->id = id;
        this->dimension = dimension;
        this->coordinates = coordinates;
        this->type = 0;
    }

    /**
     * @brief Constructor for a non numerical vector
     * @param id: The position in the dataset
     * @param dimension: The dimension of the vector/The number of words
     * @param words: The non numerical components of the vector
     * @param retweets: Retweets
     */
    Point(size_t id, size_t dimension, vector<string>& words, double retweets)
    {
        assert(dimension == words.size());

        this->id = id;
        this->dimension = dimension;
        this->words = words;
        this->retweets = retweets;
        this->type = 1;
    }

    /**
     * @brief Vectors' dot multiplication
     * @param other_point: Another point
     * @return The result of dot multiplication
     */
    double dotP(const Point &other_point) const
    {
        assert(dimension == other_point.dimension);
        assert(type == other_point.type);

        double dotp = 0.0;
        for(size_t i = 0; i < dimension; ++i)
        {
            dotp += coordinates[i] * other_point.coordinates[i];
        }

        return dotp;
    }

    /**
     * @brief Determine whether a vector dominates another
     * @param other_point: Another point
     * @return bool
     */
    bool dominates(const Point &other_point) const
    {
        assert(dimension == other_point.dimension);
        assert(type == other_point.type);

        bool at_least_one = false;

        for(size_t i = 0; i < dimension; ++i)
        {
            if(coordinates[i] < other_point.coordinates[i])
            {
                return false;
            }

            if(coordinates[i] > other_point.coordinates[i])
            {
                at_least_one = true;
            }
        }

        return at_least_one;
    }

    /**
     * @brief Scale the length of the first norm of a vector to 'len'
     * @param len: The first norm length
     */
    void scale_to_length(double len)
    {
        assert(len >= 0);
        assert(type == 0);

        double mylen = length();    //mylen = |p|

        assert(len == 0 || mylen > 0);

        double factor = (mylen > 0) ? (len / mylen) : 0;

        for(size_t i = 0; i < dimension; ++i)
        {
            coordinates[i] *= factor;
            coordinates[i] *= coordinates[i];
        }    
    }

    /**
     * @brief Vectors' subtraction
     * @param other_point: Another point
     * @return The result of subtraction
     */
    Point operator-(const Point& other_point) const
    {
        assert(other_point.dimension == dimension);
        assert(type == other_point.type);

        Point diff(dimension);

        for(size_t i = 0; i < dimension; ++i)
        {
            diff.coordinates[i] = coordinates[i] - other_point.coordinates[i];
        }

        return diff;
    }

    /**
     * @brief A vector multiplied by an constant
     * @param factor: The constant
     * @return The result of multiplication
     */
    Point operator*(const double factor) const
    {
        assert(type == 0);

        Point mult(dimension);

        for(size_t i = 0; i < dimension; ++i)
        {
            mult.coordinates[i] = coordinates[i] * factor;
        }

        return mult;
    }

    /**
     * @brief Calculate the second norm distance between vectors
     * @param other_point: Another point
     * @return Second norm distance
     */
    double distance_to(const Point &other_point) const
    {
        assert(type == other_point.type);
        assert(type == 0);

        Point diff = (*this) - other_point;

        return diff.length();
    }

    /**
     * @brief Calculate the length of the second norm of a vector
     * @return Second norm length
     */
    double length() const
    {
        assert(type == 0);

        double length = 0;

        for(size_t i = 0; i < dimension; ++i)
        {
            length += coordinates[i] * coordinates[i];
        }

        return sqrt(length);
    }

    /**
     * @brief Output 'prefix + each component + suffix'“
     * @param prefix: prefix
     * @param suffix: suffix
     */
    void dump(const char *prefix, const char *suffix) const
    {
        if(type == 0)
        {
            cout << prefix;
            cout << "[ ";
            for(size_t i = 0; i < dimension-1; ++i)
            {
                cout << coordinates[i] << ", ";
            }
            cout << coordinates[dimension - 1];
            cout << " ]";
            cout << suffix;
            cout << endl;
        }

        if(type == 1)
        {
            cout << prefix;
            cout << "[ ";
            for(size_t i = 0; i < dimension-1; ++i)
            {
                cout << words[i] << ", ";
            }
            cout << words[dimension - 1];
            cout << " ]";
            cout << suffix;
            cout << endl;
        }
    }

    /**
     * @brief Take absolute values of numerical components of a vector
     * @param other_point: A point
     */
    static Point abs(const Point &other_point)
    {
        assert(other_point.type == 0);

        Point p(other_point.dimension);
        for(size_t i = 0; i < other_point.dimension; ++i)
        {
            p.coordinates[i] = std::abs(other_point.coordinates[i]);
        }

        return p;
    }

};

#endif