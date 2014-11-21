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

void vecindad2(int particiones, vector<vector<int> > &solucion, float **pesos);
float crear_vecino(int indice, vector<pair <float, vector<int> > > &solucion, vector<pair <float, vector<int> > > &nuevo_vecino, float **pesos); 

int main()
{
	string l;
	std::getline(cin, l);
	while(l.at(0) != '0')
	{	
		istringstream ss(l);
		string token;
		int vertices, aristas, particiones;
		std::getline(ss, token, ' ');
		vertices = atoi(token.c_str());
		std::getline(ss, token, ' ');
		aristas = atoi(token.c_str());
		std::getline(ss, token, ' ');
		particiones = atoi(token.c_str());
				
		float **pesos = new float *[vertices];
		for(int i = 0; i < vertices; i++)  {pesos[i] = new float[vertices];}
		for (int i = 0; i < vertices; ++i)
		{
			for (int j = 0; j < vertices; ++j)
			{
				pesos[i][j]=-1;
				pesos[j][i]=-1;
			}
		}
		for(int i=0; i < aristas; i++)
		{
			std::getline(cin, l);
			int vertice_a;
			int vertice_b;
			float peso; 
			istringstream ssl(l);
			string token1;
			std::getline(ssl, token1, ' ');
			vertice_a = atoi(token1.c_str());
			std::getline(ssl, token1, ' ');
			vertice_b = atoi(token1.c_str());
			std::getline(ssl, token1, ' ');
			peso = atoi(token1.c_str());
			pesos[vertice_a -1][ vertice_b -1] = peso;
			pesos[vertice_b -1][ vertice_a -1] = peso;
		} 

			vector<vector<int> > presolucion;
			
			vecindad2(particiones, presolucion, pesos);//FUNCION PRINCIPAL
			
			mostrar(vertices, presolucion); //cuando ya no encuentra nada mejor termina

			for(int i = 0; i < vertices; i++) {free (pesos[i]);} //LIBERO MEMORIA
				free (pesos);
			std::getline(cin, l);
	}
    return 0;
}

void vecindad2(int particiones, vector<vector<int> > &presolucion, float **pesos)
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
	vector<pair<float, vector<int> > > vecino_solucion; // va a tener al vecino mas optimo de la vecindad
	do 
	{ 
		for(int i = 0; i < solucion.size(); i ++) //modificadas 
		{
			if(solucion[i].second.size() > 1) 
			{
				vector<pair<float, vector<int> > > nuevo_vecino; //genero un vecino
				peso_vecino = crear_vecino(i, solucion, nuevo_vecino, pesos);
				if(posible_solucion ==-1 || posible_solucion > peso_vecino)
				{
					posible_solucion = peso_vecino;
					vecino_solucion = nuevo_vecino;
				}
			} 
		}
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

	vector<vector<int> > restauro;
	for(int i = 0; i < solucion.size() ; i++){
		vector<int> a = solucion[i].second;
		restauro.push_back(a);
	}
	presolucion = restauro;

}

float crear_vecino(int indice, vector<pair <float, vector<int> > > &solucion, vector<pair <float, vector<int> > > &nuevo_vecino, float **pesos) 
{
	float mayor_peso = -1;//va a tener el peso de los adyacentes con sus adyacentes
	int nodo_a_separarA, nodo_a_separarB;
	int separo_A, separo_B;
	for(int nodo_A = 0; nodo_A < solucion[indice].second.size() -1; nodo_A++)
	{		//ambos for sirven para agarrar todo PAR de nodo
		for(int nodo_B = nodo_A +1; nodo_B < solucion[indice].second.size(); nodo_B++)
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
					ubicar_A = i;
					ubicar_B = j;
				}
			}
		}
		nuevo_vecino[ubicar_A].second.push_back(separo_A); //los agrego donde me conviene
		nuevo_vecino[ubicar_A].first = nuevo_vecino[ubicar_A].first + mas_peso_de_A[ubicar_A]; 
		nuevo_vecino[ubicar_B].second.push_back(separo_B);
		nuevo_vecino[ubicar_B].first = nuevo_vecino[ubicar_B].first + mas_peso_de_B[ubicar_B]; 
		if(ubicar_A == ubicar_B) {nuevo_vecino[ubicar_A].first += pesos[separo_A -1][separo_B -1];}
	}
	return sumar_particion(nuevo_vecino); 
}
