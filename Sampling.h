#pragma once

double qaussian(double x);

void metropolis_gasstings(const int num_iter, double *thetha);//theta structur

double uniform_random(double mm, double nn);

Thetha get_prev_iter_with_probabilities(Posterior *posterior);