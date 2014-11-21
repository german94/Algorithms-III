#ifndef _VECINDAD_1_H_
#define _VECINDAD_1_H_

#include <list>
#include <vector>
#include <iostream>

#include <algorithm>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <ctime>
#include <string>
#include <cstdlib>


using namespace std;
typedef std::vector<vector<int> > solucion;

float costo_solu(float *const*pesos, solucion &solu);
void vecindad1(float*const* pesos,solucion& mejor, float suma_solucion);
vector<pair<solucion,float> > crear_vecindad(float *const*pesos, float suma_solucion, solucion &solu);
float nuevo_costo(float *const*pesos, int elem, int sacar, int poner, solucion &solu, float suma_solucion);
int seleccionar_vecino(vector< pair<solucion,float> >& vecinos);
#endif