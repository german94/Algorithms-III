#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <ctime>
#include <algorithm>
#include "vecindad2.h" 
using namespace std;

void reacomodar(int vertices, int aristas, int particiones, vector<vector<int> > &solucion, float **pesos);
float crear_vecino(int indice, vector<pair <float, vector<int> > > &solucion, vector<pair <float, vector<int> > > &nuevo_vecino, float **pesos); 

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

void reacomodar(int vertices, int aristas, int particiones, vector<vector<int> > &presolucion, float **pesos)
{	
	vector<pair<float, vector<int> > > solucion = obtener_pesos(presolucion, pesos);
	float suma_solucion = sumar_particion(solucion); //inicialmente la suma solucion es la de la presolucion

	if(solucion.size() != particiones) //si la presolucion tiene menos de k conjuntos le agrego los restantes
	{
		for(int i = solucion.size(); i < particiones; i++)
		{
			float valor_inicial = 0;
			vector<int> vector_inicial;
			pair<float, vector<int> > rellenar;
			rellenar.first = valor_inicial;
			rellenar.second = vector_inicial;
			solucion.push_back(rellenar);
		}
	}

	float posible_solucion = -1; 
	float peso_vecino;
	vector<pair<float, vector<int> > > vecino_solucion;
	do 
	{ 
		for(int i = 0; i < solucion.size(); i ++) //modificadas 
		{
			if(solucion[i].second.size() > 1) 
			{
				vector<pair<float, vector<int> > > nuevo_vecino;
				peso_vecino = crear_vecino(i, solucion, nuevo_vecino, pesos);
				if(posible_solucion ==-1 || posible_solucion > peso_vecino)
				{
					posible_solucion = peso_vecino;
					vecino_solucion = nuevo_vecino;
				}
			} 
		}// va ir obteniendo la "c.c" mas pesada de cada conjunto que haya
		if(posible_solucion != -1)
		{	
			if (posible_solucion < suma_solucion) // si el vecino mas optimo es mejor que la solucion que tenia lo reemplazo 
			{
				suma_solucion = posible_solucion;
				posible_solucion = -1; //decremente para seguir iterando ya que ahora posible solucion == suma_solucion
				solucion = vecino_solucion;
			}
		} else {posible_solucion = suma_solucion;}	
	} while (posible_solucion < suma_solucion); // no me sirve poner <= porque podria no terminar, por eso antes decremente a psibles_solucion

	mostrar(suma_solucion, solucion); //cuando ya no encuentra nada mejor termina
}

float crear_vecino(int indice, vector<pair <float, vector<int> > > &solucion, vector<pair <float, vector<int> > > &nuevo_vecino, float **pesos) 
{
	float mayor_peso = -1;//tengo a la cosa mas pesada
	int nodo_a_separarA, nodo_a_separarB;
	int separo_A, separo_B;
<<<<<<< HEAD
	for(int nodo_A = 0; nodo_A < solucion[indice].second.size() -1; nodo_A++)
	{		//ambos while sirven para agarrar todo PAR de nodo
		for(int nodo_B = nodo_A +1; nodo_B < solucion[indice].second.size(); nodo_B++)
=======
	for(int nodo_A = 0; nodo_A < solucion[indice].size() -1; nodo_A++)
	{		//ambos while sirven para agarrar todo PAR de nodo
		for(int nodo_B = nodo_A +1; nodo_B < solucion[indice].size(); nodo_B++)
>>>>>>> ac4d79745afac6c766d87d964402e2135681b3c8
		{	
			nodo_a_separarA = solucion[indice].second[nodo_A];
			nodo_a_separarB = solucion[indice].second[nodo_B];

			if(pesos[nodo_a_separarA -1][nodo_a_separarB -1] != -1) //Si ese par es adyacente veo cuanto suma con sus adyancentes
			{
				float peso = sumar_adyacentes(nodo_a_separarA, nodo_a_separarB, solucion[indice].second, pesos);
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
	float suma_minima = solucion[indice].first; //va a ser la suma del conjunto luego de 
	nuevo_vecino = solucion;

<<<<<<< HEAD
	if (mayor_peso != -1) // si antes no encontre nodos adyacentes entonces mayor_peso quedo en -1, no tiene sentido seguir iterando
	{
		vector<float> mas_peso_de_A; //este va a ser el vecino con las modificaciones
		vector<float> mas_peso_de_B;

		sacar(nuevo_vecino[indice], separo_A, separo_B, pesos);
		
		peso_agregado(mas_peso_de_A, separo_A, pesos, nuevo_vecino);
		peso_agregado(mas_peso_de_B, separo_B, pesos, nuevo_vecino);

		int ubicar_A = indice;
		int ubicar_B = indice;
		for(int i = 0; i < nuevo_vecino.size() && suma_minima!= 0; i++ )
		{
			for(int j = 0; j < nuevo_vecino.size() && suma_minima !=0; j++ )
			{//ambos for me sirven para ver todas las combinaciones de donde meter a los nodos mas pesados
				int a = mas_peso_de_A[i] + mas_peso_de_B[j];
				if (i == j) {a = a + pesos[separo_A -1][separo_B -1];}

				if(suma_minima > a)
				{
					suma_minima = a;
=======
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
>>>>>>> ac4d79745afac6c766d87d964402e2135681b3c8
					ubicar_A = i;
					ubicar_B = j;
				}
				nuevo_vecino[j].pop_back();
			}
			nuevo_vecino[i].pop_back();
		}
		nuevo_vecino[ubicar_A].second.push_back(separo_A); //los agrego donde me conviene
		nuevo_vecino[ubicar_A].first = nuevo_vecino[ubicar_A].first + mas_peso_de_A[ubicar_A]; 
		nuevo_vecino[ubicar_B].second.push_back(separo_B);
		nuevo_vecino[ubicar_B].first = nuevo_vecino[ubicar_B].first + mas_peso_de_B[ubicar_B]; 
		if(ubicar_A == ubicar_B) {nuevo_vecino[ubicar_A].first += pesos[separo_A -1][separo_B -1];}
	}
<<<<<<< HEAD
	return sumar_particion(nuevo_vecino); //inicialmente la suma solucion es la de la presolucion
=======
	return nuevo_vecino; // devuelvo el vecino modificado
>>>>>>> ac4d79745afac6c766d87d964402e2135681b3c8
}