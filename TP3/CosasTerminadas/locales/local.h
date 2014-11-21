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

void kpmp_busqueda_local(float** pesos,solucion& mejor, float suma_solucion);
vector<pair<solucion,float> > crear_vecindad(float **pesos, float suma_solucion, solucion &solu);
float nuevo_costo(float **pesos, int elem, int sacar, int poner, solucion &solu, float suma_solucion);
int seleccionar_vecino(vector< pair<solucion,float> >& vecinos);
void mostrar(float ** pesos, solucion &solu);

void DarTiempo(double t, float suma)
{
	ofstream myfile;
	myfile.open ("tiempos1.txt", ios::app);
	myfile<<t<< "\n";
	myfile.close();

	ofstream myfile2;
	myfile2.open ("sumas1.txt", ios::app);
	myfile2<<suma<< "\n";
	myfile2.close();
}

void DarTiempo2(double t, float suma)
{
	ofstream myfile;
	myfile.open ("tiemposGolo1.txt", ios::app);
	myfile<<t<< "\n";
	myfile.close();

	ofstream myfile2;
	myfile2.open ("sumasGolo1.txt", ios::app);
	myfile2<<suma<< "\n";
	myfile2.close();
}
