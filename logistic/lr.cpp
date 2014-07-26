#include <iostream>
#include <assert.h>
#include <fstream>
#include <string>
#include <sstream>
#include <set>
#include <map>

#include <stdio.h>
#include <stdlib.h>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>

// refer to matrix row
#include <boost/numeric/ublas/matrix_proxy.hpp>

#include "util.hpp"
#include "data_loader.hpp"


using namespace std;
using namespace boost::numeric::ublas;


bool debug = true;

//
double sigmoid(double x) {
    double e = 2.718281828;

    return 1.0 / (1.0 + pow(e, -x));
}


// target: max { sum {log f(y(i)z(i)}} for i in (1, n) where f(x) = 1/1+e**(-x)
// and z(i) = sum(w(k) * x(i)(k)) for k in (1, l) where i denotes the ith training instance
// and k denotes the kth feature.
// The gradient of the log-likehood with respect to the kth weight is:
// gra = sum{y(i)x(i)(k)f(-y(i)z(i))}, then we know how to update the weight in each iteration:
// w(k)(t+1) = w(k)(t) + e * gra
void lr_without_regularization(boost::numeric::ublas::matrix<double>& x,
        boost::numeric::ublas::vector<double>& y
        ) {

    // the convergence rate
    double epsilon = 0.0001;
    // the learning rate
    double step = 0.995;
    double gamma = 0.005;
    int max_iters = 100;
    int iter = 0;

    // init
    boost::numeric::ublas::vector<double> weight_old(x.size2());
    for (size_t i=0; i<weight_old.size(); ++i) {
        weight_old(i) = 0;
    }


    boost::numeric::ublas::vector<double> weight_new(x.size2());
    for (size_t i=0; i<weight_new.size(); ++i) {
        weight_new(i) = 0;
    }

    while (true) {
        // update each weight
        for (size_t k=0; k<weight_new.size(); ++k) {
            double gradient = 0;
            for (size_t i=0; i<x.size1(); ++i) {
                double z_i = 0;
                for (size_t j=0; j<weight_old.size(); ++j) {
                    z_i += weight_old(j) * x(i,j);
                }
                gradient += y(i) * x(i,k) * sigmoid(-y(i) * z_i);
            }
            weight_new(k) = weight_old(k) + gamma * gradient;
        }

        double dist = norm(weight_new, weight_old);
        if (dist < epsilon) {
            break;
        }
        else {
            weight_old.swap(weight_new);
           // weight_old = weight_new;
        }

		gamma *= step;
        iter += 1;
        if (iter >= max_iters) {
            break;
        }

    }
	for (unsigned i = 0; i<weight_new.size(); i++){
		cout<<weight_new[i]<<' ';
	}
	cout<<endl;

}

int main(int argc, char* argv[]) {
	assert(argc == 4);

    int record_num ;
    int dim_num ;
	sscanf(argv[2], "%d", &record_num);
	sscanf(argv[3], "%d", &dim_num);
	dim_num++;

    boost::numeric::ublas::vector<double> y(record_num);
    boost::numeric::ublas::matrix<double> x(record_num, dim_num);
    SimpleDataLoader loader(record_num, dim_num);
    loader.load_file(argv[1], y, x);

    // lr_method
    lr_without_regularization(x, y);
    return 0;
}
