#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <ctime>
using namespace std;

void reacomodar(int vertices, int aristas, int particiones, vector<vector<int> > &solucion, float **pesos);
vector<vector<int> > crear_vecino( int indice, vector<vector<int> > &solucion, int particiones, float **pesos);

float suma_conjunto(const vector<int> &conjunto_actual, float **pesos)
{
	float suma = 0;
	int A= 0; //suma todos los vertices de un conjunto determinado
	while(A < conjunto_actual.size())
	{
		for(int B = A +1; B < conjunto_actual.size() ; B++ )
		{
			if(pesos[conjunto_actual[A] -1][conjunto_actual[B] -1] != -1) {suma = suma + pesos[conjunto_actual[A] -1][conjunto_actual[B] -1];}
		} 
		A++;
	}
	return suma;
}

float sumar_particion(const vector<vector<int> > &particion, float **pesos)
{
	float res = 0; //suma todo una particion 
	for(int i = 0; i < particion.size(); i++) { res = res +suma_conjunto(particion[i], pesos);}
	return res;
}

int seleccionar_vecino(const vector<vector<vector<int> > > &vecindad, float &posible_solucion, float **pesos)
{
	float menor = -1;
	int res; // va a sumar el peso de cada particion y quedarse con el minimo
	for(int i = 0; i < vecindad.size(); i++)
	{
		float peso = sumar_particion(vecindad[i], pesos);
		if(menor == -1 || menor > peso)
		{
			menor = peso;
			res = i;
		}
	}
	posible_solucion =  menor;
	return res;
}

void mostrar(float suma_solucion, vector <vector<int> > &solucion){//SOLO MUESTRA EL RESULTADO
	cout << "suma:" << suma_solucion << endl; 
	for (int t = 0; t < solucion.size(); t++){ 
		for (int j = 0; j < solucion[t].size(); j++){
			cout << t +1 << "_" << solucion[t][j] << " ";
		}
	}
	cout << endl;
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

void sacar(vector<int> &conjunto, int separo_A, int separo_B)
{
	vector<int> nuevo; //saca dos vertices determinafo de un conjunto
	for(int i = 0; i < conjunto.size(); i++)
	{
		if(conjunto[i] != separo_A && conjunto[i] != separo_B) {nuevo.push_back(conjunto[i]);}
	}
	conjunto = nuevo;
}

bool puedo_agregarlos(vector<int> &para_A, vector<int> &para_B, int A, int B, float &suma_minima, float **pesos)
{
	bool res = false;
	para_A.push_back(A); // se fija si al agregar dos nodos el costo es menor que algun anterior
	para_B.push_back(B);
	float agregado_A =suma_conjunto(para_A, pesos);
	float agregado_B =suma_conjunto(para_B, pesos);

	if(suma_minima > agregado_B + agregado_A)
	{
		suma_minima = agregado_A + agregado_B;
		res = true;
	}
	para_A.pop_back();
	para_B.pop_back();

	return res;
}