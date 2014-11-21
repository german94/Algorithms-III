#ifndef HGOLOSA_A_H_
#define HGOLOSA_A_H_

#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <ctime>
#include <list>
using namespace std;

void heuristica_golosa_aleatorizada(float* const * mAdy, vector<vector<int> >& particion, int n, float alfa, float beta, float gamma);
int dameAlgunoQueFalte(const vector<bool>& marcados);
bool faltanAgregar(const vector<bool>& marcados);
int dameAdyacenteNoMarcado(int vertice, int n, float* const * mAdy, vector<bool>& yaAgregados);
int dameParNoMarcado(int vertice, int n, float* const * mAdy, const vector<bool>& yaAgregados);
int dameParNoMarcadoRCL(int vertice, int n, float* const *mAdy, vector<bool>& yaAgregados, float alfa);
bool tieneAdyacenteNoAgregado(int vertice, int n, float* const * mAdy, const vector<bool>& yaAgregados);
void encolarAdyacentesNoAgregados(int vertice, int n, float* const * mAdy, const vector<bool>& yaAgregados, list<int>& cola, vector<bool>& yaEncolados);
float peso(int v1, int v2, float* const * mAdy);
float sumaEnConjunto(int v, float* const * mAdy, const vector<int>& conj);
float pesoConjunto(const vector<int>& conj, float* const * mAdy);
float pesoParticion(const vector<vector<int> >& particion, float* const * mAdy);
pair<float, vector<float> > mejorPesoAlAgregarParDeVerticesEnParticion(int v1, int v2, float* const * mAdy, const vector<vector<int> >& particion);
void meterParDeVerticesEnParticionRCL(int v1, int v2, float* const * mAdy, vector<vector<int> >& particion, float alfa);
void meterParDeVerticesEnParticion(int v1, int v2, float* const * mAdy, vector<vector<int> >& particion);
float mejorPesoParaVerticeEnParticion(int v, float* const * mAdy, const vector<vector<int> >& particion);
void meterVerticeEnParticionRCL(int v, float* const * mAdy, vector<vector<int> >& particion, float alfa);
void meterVerticeEnParticion(int v, float* const * mAdy, vector<vector<int> >& particion);
float aristaDePesoMaximoValor(int n, float* const * mAdy);
float aristaDePesoMaximoRCL(int n, float* const * mAdy, int& a, int& b, float alfa);
float aristaDePesoMaximo(int n, float* const * mAdy, int& a, int& b);
float pesoTotalParticion(const vector<vector<int> >& particion, float *const* mAdy, bool desde1);
void convertirIndices(vector<vector<int> >& solucion);
void mostrar(int n, const vector <vector<int> > &solucion, bool desde1);
#endif
