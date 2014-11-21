#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <ctime>
using namespace std;

void vecindad2(int particiones, vector<vector<int> > &solucion, float **pesos);
float crear_vecino(int indice, vector<pair <float, vector<int> > > &solucion, vector<pair <float, vector<int> > > &nuevo_vecino, float **pesos); 

void mostrar(int n, const vector <vector<int> > &solucion)
{	
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < solucion.size(); j++)
		{
			for(int k = 0; k < solucion[j].size(); k++)
			{
				if(solucion[j][k] == i+1)
					cout<<j+1<<" ";
			}
		}
	}
	cout<<endl;
}


float suma_conjunto(const vector<int> &conjunto_actual, float **pesos)
{
	float suma = 0; //suma todos los vertices de un conjunto determinado
	for(int A= 0; A < conjunto_actual.size(); A++)
	{
		for(int B = A +1; B < conjunto_actual.size() ; B++ )
		{
			if(pesos[conjunto_actual[A] -1][conjunto_actual[B] -1] != -1) {suma = suma + pesos[conjunto_actual[A] -1][conjunto_actual[B] -1];}
		} 
	}
	return suma;
}

vector<pair<float, vector<int> > > obtener_pesos(vector<vector<int> > &presolucion, float **pesos)
{
	vector<pair<float, vector<int> > > res;
	for(int i = 0; i < presolucion.size(); i++)
	{
		pair<float, vector<int> > elemento;
		elemento.second = presolucion[i];
		elemento.first = suma_conjunto(presolucion[i], pesos);
		res.push_back(elemento); 
	}
	return res;
}

void peso_agregado(vector<float> &mas_peso, int a_agregar, float **pesos, vector<pair <float, vector<int> > > &nuevo_vecino)
{
	for(int i = 0; i < nuevo_vecino.size(); i++)
	{
		float res = 0;
		for(int j = 0; j < nuevo_vecino[i].second.size(); j++)
		{
			if(pesos[a_agregar -1][nuevo_vecino[i].second[j] -1] != -1) {res = res + pesos[a_agregar -1][nuevo_vecino[i].second[j] -1];}		
		}
	mas_peso.push_back(res);
	}
}

float sumar_particion(const vector<pair<float, vector<int> > > &solucion )
{
	float res = 0; //suma todo una particion 
	for(int i = 0; i < solucion.size(); i++) { res = res + solucion[i].first;}
	return res;
}

float sumar_adyacentes(int nodo_A, int nodo_B, const vector<int> &conjunto_actual, float **pesos )
{
	float suma = 0; //dados dos vertices adyacentes, le suma los adyacentes a estos
	for(int i =0; i < conjunto_actual.size(); i++)
	{
		if(pesos[nodo_A -1][conjunto_actual[i] -1] != -1) {suma = suma + pesos[nodo_A -1][conjunto_actual[i]-1];}
		if(pesos[nodo_B -1][conjunto_actual[i] -1] != -1) {suma = suma + pesos[nodo_B -1][conjunto_actual[i]-1];}
	}
	return suma - pesos[nodo_A -1][nodo_B-1]; //ya que lo sume dos veces
}

void sacar(pair<float, vector<int> > &conjunto, int separo_A, int separo_B, float **pesos)
{
	vector<int> nuevo; //saca dos vertices determinafo de un conjunto
	for(int i = 0; i < conjunto.second.size(); i++)
	{
		if(conjunto.second[i] != separo_A && conjunto.second[i] != separo_B) {nuevo.push_back(conjunto.second[i]);}
	}
	conjunto.first = suma_conjunto(nuevo, pesos);
	conjunto.second = nuevo;
}
