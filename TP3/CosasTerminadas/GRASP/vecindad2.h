#ifndef _VECINDAD2_H_
#define _VECINDAD2_H_
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <ctime>
#include <algorithm>
using namespace std;

void vecindad2(int particiones, vector<vector<int> > &solucion, float *const*pesos);
float crear_vecino(int indice, vector<pair <float, vector<int> > > &solucion, vector<pair <float, vector<int> > > &nuevo_vecino, float *const*pesos); 
float suma_conjunto(const vector<int> &conjunto_actual, float *const*pesos);
vector<pair<float, vector<int> > > obtener_pesos(vector<vector<int> > &presolucion, float *const*pesos);
void peso_agregado(vector<float> &mas_peso, int a_agregar, float *const*pesos, vector<pair <float, vector<int> > > &nuevo_vecino);
float sumar_particion(const vector<pair<float, vector<int> > > &solucion );
float sumar_adyacentes(int nodo_A, int nodo_B, const vector<int> &conjunto_actual, float *const*pesos );
void sacar(pair<float, vector<int> > &conjunto, int separo_A, int separo_B, float *const*pesos);

#endif