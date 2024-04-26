#ifndef __IOUTIL_H__
#define __IOUTIL_H__

#include <cstdlib>
#include <vector>
#include <iostream>
#include <set>
#include <iomanip>
#include <fstream>
#include "Point.h"

using namespace std;

class IOUtil
{
public:
    /**
     * @brief Read files with vectors
     * @param file_path : File path
     * @param dim : The dimension of vectors
     * @param Dataset : Dataset
     */
    static void read_vectors(const char *file_path, size_t &dim, vector<Point> &Dataset)
    {
        ifstream fin;
        fin.open(file_path,std::ios::in);

        if (!fin)
        {
            cout << "Cannot open file " << file_path << " for reading \n";
            exit(1);
        }

        char first_line[1024]={0};
        fin.getline(first_line,sizeof(first_line));
        stringstream first_line_stream(first_line);
        int D_size;
        first_line_stream >> D_size;
        first_line_stream >> dim;

        char line[1024]={0};
        size_t id = 0;
        while(fin.getline(line,sizeof(line)))
        {               
            Point p;
            p.id = id;
            ++id;
            p.dimension = dim;
            p.type = 0;
            //Process each line
            stringstream line_stream(line);
            double num;
            while(line_stream >> num)
            {
                p.coordinates.push_back(num);
            }
            Dataset.push_back(p);
        }

        fin.close();
    }

    /**
     * @brief Read files with words
     * @param file_path : File path
     * @param Dataset : Dataset
     */
    static void read_words(const char *file_path, vector<Point> &Dataset)
    {
        ifstream fin;
        fin.open(file_path,std::ios::in);

        if (!fin)
        {
            cout << "Cannot open file " << file_path << " for reading \n";
            exit(1);
        }

        char first_line[1024]={0};
        fin.getline(first_line,sizeof(first_line));
        stringstream first_line_stream(first_line);
        int D_size;
        first_line_stream >> D_size;

        char line[1024]={0};
        size_t id = 0;
        while(fin.getline(line,sizeof(line)))
        {
            Point p;
            p.id = id;
            ++id;
            p.type = 1;
            //Process each line
            stringstream line_stream(line);
            line_stream>>p.retweets;
            string word;
            int dim = 0;
            while(line_stream >> word)
            {
                ++dim;
                p.words.push_back(word);
            }
            p.dimension = dim;
            Dataset.push_back(p);
        }

        fin.close();
    }
};
#endif
