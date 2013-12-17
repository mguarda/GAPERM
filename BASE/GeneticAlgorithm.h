/*
 * nich.h
 *
 *  Created on: 28/05/2013
 *      Author: Mauricio
 */

#ifndef NICH_H_
#define NICH_H_

#include <list>
#include <vector>
#include <cstring>
#include "solution.h"
using namespace std;

class GeneticAlgorithm {
private:
	int n_pop;
	list<Solution*> pop;

	double getAverageDistance();
	double randomInclusive(double max);
	double randomExclusive(double max);
	Solution* rouletteSelection();
	vector<int> crossover(Solution * solution1, Solution * solution2);
	void mutate(vector<int> *offspring);
public:
	string name;
	GeneticAlgorithm(int n_pop_initial, string name);
	virtual ~GeneticAlgorithm();
	void setName(string name);

	Solution * get_chrom_max_fitness();
	void nextPopulation();
};

#endif /* NICH_H_ */
