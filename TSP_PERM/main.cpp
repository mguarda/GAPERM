//============================================================================
// Name        : GA_Tsp.cpp
// Author      : Mauricio Guarda
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "GeneticAlgorithm.h"
#include <vector>
#include <list>
#include "problem.h"
#include "toolbox.h"
#include <locale>
#include <time.h>
using namespace std;

int main(int argc , char **argv){

	if(argc > 1){
		typedef vector<int> fila;
		typedef vector<fila> matriz;
		toolbox *tb = toolbox::instance();
		tb->clockstart();
		tb->readparamfile("./setup.conf");
		list<GeneticAlgorithm*> niches;
		list<GeneticAlgorithm*>::iterator it_niches;
		list<GeneticAlgorithm*>::iterator paux_niches;

		tb->storeVariable("IDExp",tb->filenameFromPath(argv[1]));
		tb->storeVariable("filepath",argv[1]);
		tb->storeVariable("problemType","TSP");

		Problem* p = Problem::instance();

		time_t rawtime;
		struct tm * timeinfo;
		char buffer[80];

		time(&rawtime);
		timeinfo = localtime(&rawtime);
		strftime(buffer, 80, "%c", timeinfo);
		string date_experiment(buffer);
		tb->storeVariable("date", date_experiment);

		string IDExp = tb->pval_string("IDExp");
		ofstream salida_fitness (("./Results/"+tb->pval_string("problemType")+"/"+ IDExp + "_fitness"+"("+buffer+")"+".csv").c_str(),ofstream::out);
		if(salida_fitness){
			cout << "se ha creado el archivo numcolonias.dat" << endl;
		}else{
			cout << "Error al crear el archivo numcolonias.dat" << endl;
			exit(0);
		}
		locale mylocale("");
		salida_fitness.imbue(mylocale);
		salida_fitness << "Generacion | Fitness | Costo | Solucion | Suma Distancias | Fitness Prom " << endl;

		for(int i=0; i<tb->pval_int("POP"); i++){
			niches.push_back(new GeneticAlgorithm(tb->pval_int("N_CHROM"),"Niche_"+tb->int2string(i+1)));
		}
		int n_generations = tb->pval_int("MAXGEN");
		list<Solution*> bestChromosones;
		for(int i=0; i< n_generations;i++){
//			cout << "===============Generacion Nº " << i+1 << "=================" <<endl;
			for(it_niches = niches.begin(); it_niches != niches.end();it_niches++){
//				cout << "--------------------"<<(*it_niches)->name <<"--------------------" << endl;
//				cout << "Generando nueva poblacion " << endl;
				(*it_niches)->nextPopulation();
	//			cout << "Obteniendo el mejor fitness" << endl;
				double fitness = (*it_niches)->get_chrom_max_fitness()->getFitness();
//				cout << "Mejor fitness de la generacion de: "<< (*it_niches)->name<< " " << fitness << endl;
			}
			double b_fitness = 0;
			for(it_niches = niches.begin(); it_niches != niches.end();it_niches++)
				if((*it_niches)->get_chrom_max_fitness()->getFitness() > b_fitness){
					b_fitness = (*it_niches)->get_chrom_max_fitness()->getFitness();
					paux_niches = it_niches;
				}
			salida_fitness << (i+1) << " | ";
			(*paux_niches)->get_chrom_max_fitness()->show_solution(salida_fitness);
			//salida_fitness << endl;
//---------------------------Obtencion de individuos del patio---------------------------
			list<Solution*> chrom_aux;
			list<Solution*> chromosones;
			list<Solution*>::iterator it_chrom;
			list<Solution*>::iterator it_chrom2;
			double fit = 0;
			int distancias = 0;
			for (it_niches = niches.begin(); it_niches != niches.end();	it_niches++) {
				chrom_aux = (*it_niches)->get_chromosones();
				for (it_chrom = chrom_aux.begin(); it_chrom != chrom_aux.end(); it_chrom++) {
					chromosones.push_back(*it_chrom);
				}
			}
//--------Calculo de la susma de distancias y suma de fitness de todos los individuos---------------
			for (it_chrom = chromosones.begin(); it_chrom != chromosones.end();	it_chrom++) {
				fit += (*it_chrom)->getFitness();
				for (it_chrom2 = chromosones.begin();
						it_chrom2 != chromosones.end(); it_chrom2++) {
					distancias += (*it_chrom)->Distance((*it_chrom2));
				}
			}
			double fitness_prom = fit
					/ (tb->pval_int("POP") * tb->pval_int("N_CHROM"));
//===================Se agregan al archivo de salida======================================
			salida_fitness << " | " << distancias << " | " << fitness_prom << endl;
		}

		ofstream resultado (("./Results/"+tb->pval_string("problemType")+"/"+ IDExp + "_results"+"("+buffer+")"+".dat").c_str(),ofstream::out);
		if(resultado){
			cout << "se ha creado el archivo results.dat" << endl;
		}else{
			cout << "Error al crear el archivo results.dat" << endl;
			exit(0);
		}
		resultado.imbue(mylocale);
		resultado << "Fitness | Costo | Solucion" << endl;
		for(it_niches = niches.begin(); it_niches != niches.end();it_niches++){
			(*it_niches)->get_chrom_max_fitness()->show_solution(resultado);
			/*resultado << (*it_niches)->get_chrom_max_fitness()->getFitness() << " ";
			(*it_niches)->get_chrom_max_fitness()->show_solution(resultado);*/
			resultado << endl;
		}

		double seg = tb->clockstop() / 1000000;
		time_t seconds(seg);
		tm *pt = localtime(&seconds);
		cout << "el tiempo de ejecucion ha sido de " << pt->tm_min << " min "<< pt->tm_sec << "seg " << endl;
		salida_fitness.close();
		resultado.close();
	}else{
		//	cout << "Invocación: ./EJECUTABLE_TSP_PERM <Experiment_ID>" << endl;
			cout << "Se necesita referencia a archivo de problema TSP" << endl;
			exit(0);
		}

	return 0;
}
