/*
 * nich.cpp
 *
 *  Created on: 28/05/2013
 *      Author: Mauricio
 */

#include "GeneticAlgorithm.h"
#include "problem.h"

#include <algorithm>
#include <assert.h>
#include "toolbox.h"

GeneticAlgorithm::GeneticAlgorithm(int n_pop_initial, string name) {
	// TODO Auto-generated constructor stub
	Problem* p = Problem::instance();
	n_pop = n_pop_initial;
	for(int i=0; i<n_pop_initial;i++){
		pop.push_back(p->NewSolution());
	}
	this->name = name;
//	cout << "Cantidad de chromosomas del nicho: " << pop.size() << endl;
}

GeneticAlgorithm::~GeneticAlgorithm() {
	// TODO Auto-generated destructor stub
}

void GeneticAlgorithm::setName(string name){
	this->name = name;
}

bool myfunction (Solution* i,Solution*  j) { return (i->getFitness()>j->getFitness()); }

void GeneticAlgorithm::nextPopulation(){

	Problem *p = Problem::instance();
	toolbox *tb = toolbox::instance();
	list<Solution*> newPopulation;
//	list<Solution*> *aux;
	/*we gonna use elitism */
//	cout << "Obteniendo el primer mejor chromosoma ..." <<endl;
//	Solution * elite = get_chrom_max_fitness();
//	newPopulation.push_back(elite);
//	double bestChromfit = elite->getFitness();
//	double secondbest = 0;
	list<Solution*>::iterator it1;
	list<Solution*>::iterator it2;

/*	for(it1=pop.begin(); it1!=pop.end();it1++){
		cout << (*it1)->getFitness() << " " ;
	}
	cout << endl;
	cout << "ordenando de mayor a menor" << endl;*/
	pop.sort(myfunction);
	/*for(it1=pop.begin(); it1!=pop.end();it1++){
			cout << (*it1)->getFitness() << " " ;
		}
	cout << endl;*/
/*
	pop.reverse();
	for(it1=pop.begin(); it1!=pop.end();it1++){
			cout << (*it1)->getFitness() << " " ;
		}
	cout << endl;*/

	double elitism = tb->pval_double("ELITISM")*n_pop;
//	cout << "Elitismo = " << elitism << endl;
	int i = 0;
	for(it1 = pop.begin();it1!= pop.end();it1++){
		if(i < elitism){
			newPopulation.push_back(*it1);
			i++;
		}
	}
/*	cout << "agregados a nueva poblacion " << newPopulation.size() << endl;

	for(it2=newPopulation.begin(); it2!=newPopulation.end();it2++){
		cout << (*it2)->getFitness() << " " ;
	}
	cout << endl;*/
//	cout << "Obteniendo el segund mejor chromosoma ..." << endl;
/*	for(it1 = pop.begin();it1!= pop.end();it1++)
		if((*it1)->getFitness()>secondbest && (*it1)->getFitness()<bestChromfit){
			it2 = it1;
			secondbest =(*it1)->getFitness();
		}
	newPopulation.push_back(*it2); */

//	cout << "Elitismo aplicado!" << endl;
//	cout << "Inciando proceso de cruzamiento ..." << endl;
	while((int)newPopulation.size() < n_pop){
//		cout <<"Cantidad nueva generacion: " << newPopulation.size() << endl;
		Solution * parentA;
		Solution * parentB;
//		cout << "Seleccionando primer padre para cruzamiento... " << endl;
		parentA = rouletteSelection();
//		cout << "Padre A seleccionado" << endl;
		do{
			parentB = rouletteSelection();
		}while(parentA == parentB);
//		cout << "Padre B seleccionado" << endl;
//		cout << "Realizando cruzamiento ..." << endl;
		/* 0.0 <= random <= 1 */
		double random = randomInclusive(1);
		double crossoverProbability = tb->pval_double("CROSS_PROB");
//		cout << "Probabilidad de cruzamiento: " << crossoverProbability << endl;
		if (random > crossoverProbability) {
			//		offspring = sol1;
			vector<int>offspring = crossover(parentA, parentB);
//			cout << offspring.at(0) << endl;
//			for (unsigned int i = 0; i < offspring.size(); i++)
//				cout << offspring[i] << " ";
//			cout << endl;
//			cout << "Cruzamiento realizado ..." << endl;
//			cout << "Iniciando mutación del hijo obtenido... " << endl;
			mutate(&offspring);
//			cout << "Agregando chromosoma hijo a la nueva generacion..." << endl;
//			cout << "Largo del chromosoma: " << offspring.size() << endl;
			Solution* newSol = p->NewSolution(offspring);
//			cout << "Special Flag" << endl;
			newPopulation.push_back(newSol);
//			cout << "El chromosoma obtenido se ha agregado a la nueva generacion" << endl;
/*			for (unsigned int i = 0; i < offspring.size(); i++)
							cout << offspring[i] << " ";
						cout << endl;
//			exit(0);*/
		}
	}
//	aux = &pop;
	pop = newPopulation;
//	cout << "Nueva poblacion obtenida!" << endl;
//	delete(aux);
}
/**
* Método de Cruzamiento OX
*/
vector<int> GeneticAlgorithm::crossover(Solution * solution1, Solution * solution2){

//	cout << "Iniciando cruzamiento ..." << endl;
//	toolbox *tb = toolbox::instance();
	Problem *ptr = Problem::instance();
	vector<int> sol1 = * (vector<int> *)(solution1->getValues());
	vector<int> sol2 = * (vector<int> *)(solution2->getValues());
//	cout << "largo del padre y madre: " << sol1.size() << " " << sol2.size() << endl;

	/* Impresion del padre1*/
/*	for (unsigned int i = 0; i < sol1.size(); i++) {
		cout << sol1[i] << " ";
	}
	cout << endl;*/

	/* Impresion del padre1*/
/*	for (unsigned int i = 0; i < sol1.size(); i++) {
		cout << sol2[i] << " ";
	}
	cout << endl;*/

//	cout << "Cruzamiento aprobado" << endl;
//	cout << "dimension: " << ptr->getDimension() << endl;
	vector<int> offspring(ptr->getDimension(),0);
//	cout << "offspring.size(): " << offspring.size() << endl;
	int index1 = (int)randomInclusive((int)ptr->getDimension() -1);
	int index2;
	do{
		index2 = (int)randomInclusive((int)ptr->getDimension() -1);
	}while(index1 == index2 || (index1 == 0 && index2 == (int)ptr->getDimension() -1) || (index2 == 0 && index1 == (int)ptr->getDimension() -1));
//	cout << "Indices obtenidos son " << index1 << " " << index2 << endl;
	int start;
	int end;
	if(index1 < index2){
		start = index1;
		end= index2;
	}else{
		start = index2;
		end = index1;
	}
//	cout << "Start y End " << start << " " << end << endl;
	vector<int> map_sol1;
	vector<int> check((int)ptr->getDimension(),0);
	for(int i = start ;i <= end ; i++){
		map_sol1.push_back(sol1[i]);
		offspring[i] = sol1[i];
		check[i] = 1;
	}
	bool aux;
	int k = 0;
	for(int i=0; i<(int)ptr->getDimension();i++){
		aux = true;
		for(int j=0; j < (int)map_sol1.size();j++)
			if(sol2[i] == map_sol1[j])
				aux = false;
		if(aux){
			while(check[k] == 1){
				k++;
			}
			offspring[k] = sol2[i];
			k++;
		}
	}

/*	for (unsigned int i = 0; i < sol1.size(); i++) {
		cout << offspring[i] << " ";
	}
	cout << endl;*/

	return offspring;

}

void GeneticAlgorithm::mutate(vector<int> *offspring){
	toolbox *tb = toolbox::instance();
	Problem *ptr = Problem::instance();
	/* 0.0 <= random <= 1 */

	double random = randomInclusive(1);
	/* Nope, didn't happen */
	double mutationProbability = tb->pval_double("MUTA_PROB");
//	cout << "Probabilidad de mutacion: " << mutationProbability << endl;
	if (random <= mutationProbability){
//		cout << "Flag tmp" << endl;
		int tmp;
		int random2;
//		cout << "Obteniendo indices a intercambiar" << endl;
		int random1 = (int) randomExclusive((int) ptr->getDimension() - 1);
		do {
			random2 = (int) randomExclusive((int) ptr->getDimension() - 1);
		} while (random1 == random2);
//		cout << "Indices " << random1 << " " << random2 << endl;
		tmp = (*offspring)[random1];
		(*offspring)[random1] = (*offspring)[random2];
		(*offspring)[random2] = tmp;
//		cout << "Mutacion realizada" << endl;

	}else{
//		cout << "No hubo mutacion" << endl;
	}


}

double GeneticAlgorithm::getAverageDistance()
{
		list<Solution*>::iterator it;
        double distance = 0;
        for(it = pop.begin(); it != pop.end(); it++)
        	distance += 1/(*it)->getFitness();
        return distance/pop.size();
}

Solution* GeneticAlgorithm::rouletteSelection()
{
		list<Solution*>::iterator it;
		double sum = 0;
        /* Calculate sum of all chromosome fitnesses in population */
		for(it = pop.begin(); it != pop.end(); it++)
        {
                sum += (*it)->getFitness();
        }

        /* 0.0 <= random <= sum */
        double random = randomInclusive(sum);

        sum = 0;
        /* Go through the population and sum fitnesses from 0 to sum s.
         * When the sum s is greater or equal to r; stop and return the chromosome where you are */
        for(it = pop.begin(); it != pop.end(); it++)
        {
                sum += (*it)->getFitness();
                if(sum >= random)
                {
                        return (*it);
                }
        }
        assert(false && "A chromosone should have been picked by now");
//        return(NULL);
}

double GeneticAlgorithm::randomInclusive(double max)
{
        /* Generate random number r, 0.0 <= r <= max */
        //return ((double)rand() / (double)RAND_MAX * max);
//		cout << "RandomInclusive ... "<<endl;
        return ((double)rand() * max) / (double)RAND_MAX;
}

double GeneticAlgorithm::randomExclusive(double max)
{
        /* Generate random number r, 0.0 <= r < max */
        //return ((double)rand() / ((double)RAND_MAX + 1) * max);
        return ((double)rand() * max) / ((double)RAND_MAX + 1);
}

Solution* GeneticAlgorithm::get_chrom_max_fitness(){
	list<Solution*>::iterator it;
	list<Solution*>::iterator best;
	double fitness = 0;
	for(it=pop.begin(); it!= pop.end();it++)
		if((*it)->getFitness()>fitness){
			fitness = (*it)->getFitness();
			best = it;
		}
	return *best;
}
