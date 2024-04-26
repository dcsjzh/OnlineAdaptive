# Online Submodular Maximization via Adaptive Thresholds
This repository contains source code of our paper "Online Submodular Maximization via Adaptive Thresholds" which introduces an approximation algorithm (OnlineAdaptive) for the online submodular maximization problem. In our experiments, our algorithm OnlineAdaptive and all evaluated baseline algorithms are implemented in C++.

# Datasets
This repository also provides five processed real-world datasets used in our experiments, as follows:

* ForestCover: a 10d dataset with 286,048 tuples. Reference: Fei Tony Liu, Kai Ming Ting, and Zhi-Hua Zhou. Isolation forest. In Proceedings of the IEEE Inter national Conference on Data Mining (ICDM), pages 413-422, 2008.

* CreditCardFraud: a 29d dataset with 284,807 tuples. Reference: Andrea Dal Pozzolo, Olivier Caelen, Reid A. Johnson, and Gianluca Bontempi. Calibrating probability with undersampling for unbalanced classification. In Proceedings of the IEEE Symposium Series on Computational Intelligence (SSCI), pages 159–166, 2015.

* KDDCup99: a 79d dataset with 48,113 tuples. Reference:  Guilherme Oliveira Campos, Arthur Zimek, J¨ org Sander, Ricardo J. G. B. Campello, Barbora Micenkov´a, Erich Schubert, Ira Assent, and Michael E. Houle. On the evaluation of unsupervised outlier detection: measures, datasets, and an empirical study. Data Mining and Knowledge Discovery, 30(4):891–927, 2016.

* YouTube: a 4d dataset with 9,010 tuples. Reference: Ehsan Kazemi, Marko Mitrovic, Morteza Zadimoghaddam, Silvio Lattanzi, and Amin Karbasi. Submodular streaming in all its glory: Tight approximation, minimum memory and low adaptive complexity. In Proceedings of the International Conference on Machine Learning (ICML), pages 3311–3320, 2019.

* Twitter: a dataset contains 42,104 tuples. Reference: Ehsan Kazemi, Marko Mitrovic, Morteza Zadimoghaddam, Silvio Lattanzi, and Amin Karbasi. Submodular streaming in all its glory: Tight approximation, minimum memory and low adaptive complexity. In Proceedings of the International Conference on Machine Learning (ICML), pages 3311–3320, 2019.

# How to reproduce the experiments in our paper
Our experiments were conducted on a machine running Ubuntu 20.04 LTS. We provide the explanations of the files as follows:

## Main function
- File "main.cpp": is used to conduct comparative experiments and output results.

## Datasets
- Folder "dataset": contains five processed real-world datasets as introduced above.

## Useful tools
- File "IOUtil.h": is used to load the datasets.
- File "Point.h": is used to represent the elements in the datasets and process some related calculations.

## Submodular functions
- File "SubmodularFunction.h": is the base class of the following three submodular functions.
- File "GauVecSubFunc.h": is the submodular function in the application "Online Kernel Prototype Selection". The corresponding datasets are "ForestCover", "CreditCardFraud", and "KDDCup99".
- File "LapVecSubFunc.h": is the submodular function used in the application "Online Video Summarization". The corresponding dataset is "YouTube".
- File "TweetTexSubFunc.h": is the submodular function used in the application "Online Text Summarization". The corresponding dataset is "Twitter".

## Evaluated algorithms
- File "SubsetSelectionAlgorithm.h": is the base class of the following six algorithms.
- File "OnlineAdaptive.h": is the online algorithm that we proposed.
- File "OnlineNonAdaptive.h": is the non-adaptive version of our OnlineAdaptive algorithm.
- File "Greedy.h": is the offline algorithm proposed in Ryan Gomes and Andreas Krause. Budgeted nonparametric learning from data streams. In Proceedings of the International Conference on Machine Learning (ICML), pages 391–398, 2010.
- File "IndependentSetImprovement.h": is the online algorithm proposed in Amit Chakrabarti and Sagar Kale. Submodular maximization meets streaming: matchings, matroids, and more. Mathematical Programming,154:225–247, 2015.
- File "StreamingGreedy.h": is the online algorithm proposed in Chandra Chekuri, Shalmoli Gupta, and Kent Quanrud. Streaming algorithms for submodular function maximization. In Proceedings of the International Colloquium on Automata, Languages, and Programming (ICALP), pages 318–330, 2015.
- File "Preemption.h": is the online algorithm proposed in Niv Buchbinder, Moran Feldman, and Roy Schwartz. Online submodular maximization with preemption. ACM Transactions on Algorithms, 15(3): 30:1–30:31, 2019.
- File "FreeDisposal.h": is the online algorithm proposed in T.-H. Hubert Chan, Zhiyi Huang, Shaofeng H.-C. Jiang, Ning Kang, and Zhihao Gavin Tang. Online submodular maximization with free disposal. ACM Transactions on Algorithms, 14(4): 56:1-56:29, 2018.
