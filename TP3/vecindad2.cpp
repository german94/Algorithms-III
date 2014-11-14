#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <ctime>
#include "vecindad2.h" 
using namespace std;

void reacomodar(int vertices, int aristas, int particiones, vector<vector<int> > &solucion, float **pesos);
vector<vector<int> > crear_vecino( int indice, vector<vector<int> > &solucion, int particiones, float **pesos);

int main()
{
	int vertices;
	int aristas;
	int particiones;
	cin >> vertices;
	cin >> aristas;
	cin >> particiones;
	float **pesos = new float *[vertices];
	for(int i = 0; i < vertices; i++)  {pesos[i] = new float[vertices];}
	int vertice_a;
	int vertice_b;
	float peso; 

	for(int i = 0; i < vertices; i++){
		for (int j = 0; j < vertices ; j++){
				pesos[i][j] = -1; //inicialmente dejo todos en -1 para los que no sean adyacentes
		}
	} //tengo una matriz con todos los pesos
		
	for(int i = 0; i < aristas; i++){
		cin >> vertice_a;
		cin >> vertice_b;
		cin >> peso;
		pesos[vertice_a -1][ vertice_b -1] = peso;
		pesos[vertice_b -1][ vertice_a -1] = peso;
	} //tengo una matriz con todos los pesos

vector<vector<int> > presolucion;	//ARMAR UNA PRESOLUCION
/*
vector<int> a;
vector<int> b;
vector<int> c;
vector<int> d;
a.push_back(1);
b.push_back(2);
c.push_back(3);
d.push_back(4);
a.push_back(5);
b.push_back(6);
c.push_back(7);
d.push_back(8);
a.push_back(9);
b.push_back(10);
presolucion.push_back(a);
presolucion.push_back(b);
presolucion.push_back(c);
presolucion.push_back(d);
*/

reacomodar(vertices, aristas, particiones, presolucion, pesos);//FUNCION PRINCIPAL

	for(int i = 0; i < vertices; i++) {free (pesos[i]);} //LIBERO MEMORIA
	free (pesos);	

    return 0;
}

void reacomodar(int vertices, int aristas, int particiones, vector<vector<int> > &solucion, float **pesos)
{
	float suma_solucion = sumar_particion(solucion, pesos); //inicialmente la suma solucion es la de la presolucion
	if(solucion.size() != particiones) //si la presolucion tiene menos de k conjuntos le agrego los restantes
	{
		for(int i = solucion.size(); i < particiones; i++)
		{
			vector<int> rellenar;
			solucion.push_back(rellenar);
		}
	}
	
	float posible_solucion; 
	do 
	{ 
		vector <vector<vector<int> > > vecindad; //voy a crear mi vecindad con las kparticiones
		for(int i = 0; i < solucion.size(); i ++) //modificadas 
		{
			if(solucion[i].size() > 1) {vecindad.push_back(crear_vecino(i, solucion, particiones, pesos));} 
		}// va ir obteniendo la "c.c" mas pesada de cada conjunto que haya

		int j = seleccionar_vecino(vecindad, posible_solucion, pesos);// me quedo con el vecino mas optimo

		if (posible_solucion < suma_solucion) // si el vecino mas optimo es mejor que la solucion que tenia lo reemplazo 
		{
			solucion = vecindad[j]; //tengo una nueva solucion
			suma_solucion = posible_solucion;
			posible_solucion--; //decremente para seguir iterando ya que ahora posible solucion == suma_solucion
		}
	} while (posible_solucion < suma_solucion); // no me sirve poner <= porque podria no terminar, por eso antes decremente a psibles_solucion

	mostrar(suma_solucion, solucion); //cuando ya no encuentra nada mejor termina
}

vector<vector<int> > crear_vecino( int indice, vector<vector<int> > &solucion, int particiones, float **pesos) 
{
	float mayor_peso = -1;//tengo a la cosa mas pesada
	int nodo_a_separarA, nodo_a_separarB;
	int separo_A, separo_B;
	for(int nodo_A = 0; nodo_A < solucion[indice].size() -1; nodo_A++)
	{		//ambos while sirven para agarrar todo PAR de nodo
		for(int nodo_B = nodo_A +1; nodo_B < solucion[indice].size(); nodo_B++)
		{	
			nodo_a_separarA = solucion[indice][nodo_A];
			nodo_a_separarB = solucion[indice][nodo_B];

			if(pesos[nodo_a_separarA -1][nodo_a_separarB -1] != -1) //Si ese par es adyacente veo cuanto suma con sus adyancentes
			{
				float peso = sumar_adyacentes(nodo_a_separarA, nodo_a_separarB, solucion[indice], pesos);
				if(peso > mayor_peso || mayor_peso == -1)
				{
					mayor_peso = peso; // como lo voy a hacer con todos los pares me guardo la suma de adyacencia que mas sumo. y me guardo los nodos que tendria que separar
					separo_A = nodo_a_separarA;
					separo_B = nodo_a_separarB;
				}
			}
		}
	} //tengo a los nodos que quiero separar y el peso
	//En esta parte voy a ubicar a los nodos donde mas me convenga
	float suma_minima = suma_conjunto(solucion[indice], pesos); //es el peso del conjunto sin los vertices

	int ubicar_A = indice; //si no lo dejo como esta
	int ubicar_B = indice;
	vector<vector<int> > nuevo_vecino = solucion; //este va a ser el vecino con las modificaciones
	
	if (mayor_peso != -1) // si antes no encontre nodos adyacentes entonces mayor_peso quedo en -1, no tiene sentido seguir iterando
	{
		
		sacar(nuevo_vecino[indice], separo_A, separo_B);
		for(int i = 0; i < particiones && suma_minima!= 0; i++ )
		{
			nuevo_vecino[i].push_back(separo_A); // se fija si al agregar dos nodos el costo es menor que algun anterior
			float agregado_A =suma_conjunto(nuevo_vecino[i], pesos);

			for(int j = 0; j < particiones && suma_minima !=0; j++ )
			{//ambos for me sirven para ver todas las combinaciones de donde meter a los nodos mas pesados
			
				nuevo_vecino[j].push_back(separo_B); // se fija si al agregar dos nodos el costo es menor que algun anterior
				float agregado_B = suma_conjunto(nuevo_vecino[j], pesos);

				if(suma_minima > agregado_B + agregado_A)
				{
					suma_minima = agregado_A + agregado_B;
					ubicar_A = i;
					ubicar_B = j;
				}
				nuevo_vecino[j].pop_back();
			}
			nuevo_vecino[i].pop_back();
		}
		nuevo_vecino[ubicar_A].push_back(separo_A); //los agrego donde me conviene
		nuevo_vecino[ubicar_B].push_back(separo_B);
	}
	return nuevo_vecino; // devuelvo el vecino modificado
}