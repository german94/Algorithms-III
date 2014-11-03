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

int dameAdyacenteNoMarcado(int vertice, int n, float* const * mAdy, vector<bool>& yaAgregados)
{
	for(int i = 0; i < n; i++)
	{
		if(mAdy[vertice][i] != -1 && !yaAgregados[i])
			return i;
			
	}
}

int dameParNoMarcado(int vertice, int n, float* const * mAdy, const vector<bool>& yaAgregados)
{
	int pesoMax = -1;
	int ret;

	for(int i = 0; i < n; i++)
	{
		if(mAdy[vertice][i] > pesoMax && !yaAgregados[i])
		{
			ret = i;
			pesoMax = mAdy[vertice][i];
		}			
	}

	return ret;
}

bool tieneAdyacenteNoAgregado(int vertice, int n, float* const * mAdy, const vector<bool>& yaAgregados)
{
	for(int i = 0; i < n; i++)
	{
		if(mAdy[vertice][i] != -1 && !yaAgregados[i])
			return true;
	}

	return false;
}

void encolarAdyacentesNoAgregados(int vertice, int n, float* const * mAdy, const vector<bool>& yaAgregados, list<int>& cola, vector<bool>& yaEncolados)
{
	for(int i = 0; i < n; i++)
	{
		if(mAdy[vertice][i] != -1 && !yaAgregados[i] && !yaEncolados[i])
		{
			cola.push_back(i);
			yaEncolados[i] = true;
		}
	}
}

//necesito esta funcion porque no puedo ponerle peso 0 en la matriz a dos vertices no adyacentes, ya que
//tengo que tener alguna manera de diferenciar cuando dos vertices son (>= 0) o no (-1) adyacentes
float peso(int v1, int v2, float* const * mAdy)
{
	if(mAdy[v1][v2] == -1)
		return 0;

	return mAdy[v1][v2];
}

float sumaEnConjunto(int v, float* const * mAdy, const vector<int>& conj)
{
	float ret = 0;

	for(int i = 0; i < conj.size(); i++)
		ret += peso(v, conj[i], mAdy);

	return ret;
}

float pesoConjunto(const vector<int>& conj, float* const * mAdy)
{
	float ret = 0;

	for(int i = 0; i < conj.size(); i++)
	{
		for(int j = i + 1; j < conj.size(); j++)
			ret += peso(conj[i], conj[j], mAdy);
	}

	return ret;
}

float pesoParticion(const vector<vector<int> >& particion, float* const * mAdy)
{
	float ret = 0;

	for(int i = 0; i < particion.size(); i++)
		ret += pesoConjunto(particion[i], mAdy);

	return ret;
}

void meterParDeVerticesEnParticion(int v1, int v2, float* const * mAdy, vector<vector<int> >& particion)
{
	int dondePonerAv1;
	int dondePonerAv2;
	float pesoMejor = -1;
	for(int i = 0; i < particion.size(); i++)
	{
		for(int j = 0; j < particion.size(); j++)
		{
			float nuevoPeso = sumaEnConjunto(v1, mAdy, particion[i]) + sumaEnConjunto(v2, mAdy, particion[j]);
			if(i == j)
				nuevoPeso += peso(v1, v2, mAdy);

			if(nuevoPeso < pesoMejor || pesoMejor == -1)
			{
				pesoMejor = nuevoPeso;
				dondePonerAv1 = i;
				dondePonerAv2 = j;
			}
		}
	}

	particion[dondePonerAv1].push_back(v1);
	particion[dondePonerAv2].push_back(v2);
}

void meterVerticeEnParticion(int v, float* const * mAdy, vector<vector<int> >& particion)
{
	int dondePonerAv;
	float pesoMejor = -1;

	for(int i = 0; i < particion.size(); i++)
	{
		float nuevoPeso = sumaEnConjunto(v, mAdy, particion[i]);

		if(nuevoPeso < pesoMejor || pesoMejor == -1)
		{
			pesoMejor = nuevoPeso;
			dondePonerAv = i;
		}
	}

	particion[dondePonerAv].push_back(v);
}

float aristaDePesoMaximo(int n, float* const * mAdy, int& a, int& b)
{
	float pesoMax = -1;

	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			if(mAdy[i][j] > pesoMax)
			{
				pesoMax = mAdy[i][j];
				a = i;
				b = j;
			}
		}
	}

	return pesoMax;
}

void mostrar(float suma_solucion, const vector <vector<int> > &solucion){//SOLO MUESTRA EL RESULTADO
	cout << "suma:" << suma_solucion << endl; 
	for (int t = 0; t < solucion.size(); t++){ 
		for (int j = 0; j < solucion[t].size(); j++){
			cout << t +1 << "_" << solucion[t][j] + 1 << " ";
		}
	}
	cout << endl;
}	