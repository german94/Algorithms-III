#ifndef BUSQUEDA_LOCAL_H_
#define BUSQUEDA_LOCAL_H_
#include <list>
#include <vector>
#include <iostream>

#include <algorithm>

using namespace std;
typedef std::vector<vector<int> > solucion;

void kpmp_busqueda_local(float** pesos,solucion& mejor, float suma_solucion);
vector<pair<solucion,float> > crear_vecindad(float **pesos, float suma_solucion, solucion &solu);
float nuevo_costo(float **pesos, int elem, int sacar, int poner, solucion &solu, float suma_solucion);
int seleccionar_vecino(vector< pair<solucion,float> >& vecinos);
// void mostrar(float ** pesos, solucion &solu)

#endif /* BUSQUEDA_LOCAL_H_ */
