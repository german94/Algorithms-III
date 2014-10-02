#include <iostream>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <ctime>
#include <list>
#include <queue>

#include <algorithm>


using namespace std;

struct arista{int u_; int w_;};

struct arista_costo
{int u_; int w_; int costo_;
	bool operator< (const arista_costo& otra) const
		{return this->costo_ >= otra.costo_;} // es menor cuando el costo es mayor o igual, para que el heap sea un minheap
};

struct resAGM //lo que retorna el arbol generador mínimo
{list<int> nodos_;	list<arista> aristas_; arista_costo MinGeneraArbol_; // es la arista que genera el ciclo, de costo mínimo entre las aristas no usadas
};

struct tuplaPila
{int prioridad_; int vertice_; list<int > adyacentesAVertice_;
	bool operator< (const tuplaPila& otra) const
		{return this->prioridad_ >= otra.prioridad_;} // es menor cuando el costo es mayor o igual, para que el heap sea un minheap
};

void DarTiempo(float t)
{
	ofstream myfile;
	myfile.open ("tiempos.txt", ios::app);
	myfile<<t<< "\n";
	myfile.close();
}

vector<vector<int> > crearMatrizGrafo(int n)
{
	vector<vector<int> > grafo(n+1); // es n+1 porque las computadoras están numeradas de 1 a n, y la indexación del vector es de 0 a n-1
	for (int i = 0; i < n+1; ++i)
	{
		std::vector<int> v(n+1);
		for (int j = 0; j < n+1; ++j)
			v[j]= -1;

		grafo[i]=v;
	}
	return grafo;
}

vector<vector<int> >  ponerPesoAlasAristas(int n, list<arista_costo>& valores)
{
	vector<vector<int> > grafo = crearMatrizGrafo(n);
	for (std::list<arista_costo>::iterator it=valores.begin(); it != valores.end(); ++it)
	{
		int i = (*it).u_; int j = (*it).w_; int costo = (*it).costo_;
		grafo[i][j]=costo; grafo[j][i]=costo; // pues es simétrico
	}
	return grafo;
}

arista_costo buscarGeneraCiclo(vector <vector<int> >& grafo, list<arista>& aristasArbol)
{
	int n= grafo.size(); // n es la cantidad de nodos +1
	vector<vector<int> > arbol = crearMatrizGrafo(n);
	for (std::list<arista>::iterator it=aristasArbol.begin(); it != aristasArbol.end(); ++it)
	{
		int i = (*it).u_; int j = (*it).w_; int costo = grafo[(*it).u_][(*it).w_];
		arbol[i][j]=costo; arbol[j][i]=costo; // pues es simétrico
	}

	int acum =0;
	for (int i = 1; i < n; ++i)
		for (int j = 1; j <n; ++j)
			acum = acum+ grafo[i][j];

	arista_costo min;
	min.costo_=acum+1;  min.u_=0; min.w_=0;

	for (int i = 1; i < n; ++i)
	{
		for (int j = 1; j < n; ++j)
		{
			if (grafo[i][j] !=-1)
			{	// si no están ambos nodos usados y el costo es más chico que el actual, cambio 
				if ( arbol[i][j]==-1 && grafo[i][j] < min.costo_)
					{
						min.costo_=grafo[i][j]; 
						min.u_=i;
						min.w_=j;
					}
			}
		}
	}
	//si nunca cambió min.u_ y quedó en 0, no hay solución.
	return min;
}

resAGM arbolGeneradorMinimo(vector<vector<int> >& grafo)
{
	int n = grafo.size();

	std::vector<bool> usados(n);
	for (int i = 0; i < n; ++i) // inicializo todos en false
		usados[i]= false;

	std::priority_queue<arista_costo> siguienteArista;

	int nodo = 1;
	for (int i = 1; i < n; ++i)
	{
		if (grafo[i][nodo] != -1)
		{	
			arista_costo a; a.u_=nodo; a.w_=i; a.costo_= grafo[i][nodo]; 
			siguienteArista.push(a);
		}
	} // ya tengo encolados los adyacentes a 1.
	usados[nodo] = true;

	list<int> nodos;
	list<arista> aristas;
	int cantNodos = 0;
	nodos.push_back(nodo);
	cantNodos++;	

	while(!siguienteArista.empty() && cantNodos !=n-1)
	{
		int nodo = siguienteArista.top().w_;
		int adyacenteANodo = siguienteArista.top().u_;
		siguienteArista.pop();
		if (!usados[nodo])
		{
				usados[nodo]= true;
				nodos.push_back(nodo);
				cantNodos++;
				arista a; a.u_=nodo; a.w_= adyacenteANodo;
				aristas.push_back(a);

			for (int i = 1; i < n; ++i)
			{
				if (grafo[i][nodo] != -1 && !usados[i]) // para que no vuelva a encolar cosas usadas.
				{	
					arista_costo a; a.u_=nodo; a.w_=i; a.costo_= grafo[i][nodo]; 
					siguienteArista.push(a);
				}
			} // ya tengo encolados los adyacentes a nodo
		}	

	}

	resAGM res;
	res.aristas_=aristas;
	res.nodos_=nodos;

	res.MinGeneraArbol_.u_= buscarGeneraCiclo(grafo,aristas).u_;
	res.MinGeneraArbol_.w_= buscarGeneraCiclo(grafo,aristas).w_;
	res.MinGeneraArbol_.costo_= buscarGeneraCiclo(grafo,aristas).costo_;

	return res;
}

pair<list<arista>,list<int> > camino(int salida, int llegada, vector< vector<int> >& grafo)
{
	int n = grafo.size();
	vector<bool> usados(n);
	// inicializo las posiciones del vector con false
	for (int i = 0; i < n; ++i)
		usados[i]=false;

	vector<int> predecesores(n);
	predecesores[salida]=0; // para que saber cuando tengo que parar de pedir predecesores

	std::priority_queue< tuplaPila > heap;

	int prioridad=1;
	//buscar los adyacentes de salida
	list<int> adyacentes ;
	for (int i = 1; i < n; ++i)
		if (grafo[i][salida] != -1)
			adyacentes.push_back(i);

	tuplaPila t;
	t.vertice_ = salida; t.prioridad_= prioridad; t.adyacentesAVertice_= adyacentes;

	heap.push(t);
	usados[salida]= true;
	predecesores[salida]= 0; // para saber cuando tengo que dejar de pedir predecesores

	while(!heap.empty())
	{
		if (heap.top().adyacentesAVertice_.empty()) // si el vertice del tope de la pila no tiene más adyacentes
		{heap.pop();}
		else
		{
			int predecesor= heap.top().vertice_;

			// borro el nodo de la lista de adyacentes
			list<int> adyacentes =heap.top().adyacentesAVertice_;
			int agregarNuevosAdyacentes = (heap.top().adyacentesAVertice_).front();
			adyacentes.pop_front();
			// como no puedo modificar la tupla porque top es const, tengo que crear una nueva tupla y meter los datos cambiados

			tuplaPila t;
			t.vertice_ = heap.top().vertice_;
			t.prioridad_= heap.top().prioridad_;
			t.adyacentesAVertice_= adyacentes;
			heap.pop(); 
			heap.push(t);

			list<int> adyacentesDelNuevoNodo;
			for (int i = 1; i < n; ++i)
				if (grafo[i][agregarNuevosAdyacentes] != -1 && !usados[i])
					adyacentesDelNuevoNodo.push_back(i);

			tuplaPila t1;
			t1.vertice_ = agregarNuevosAdyacentes;
			t1.prioridad_= prioridad;
			t1.adyacentesAVertice_= adyacentesDelNuevoNodo;
			prioridad++; 
			heap.push(t1);

			usados[agregarNuevosAdyacentes]=true;
			predecesores[agregarNuevosAdyacentes]= predecesor;
		}
	}

	// terminé de poner todos los predecesores.
	// ahora tengo que devolver la lista de nodos y aristas del recorrido con la arista que cierra el ciclo.

		list<arista> aristas;
		list<int> nodos;

		int it = llegada;

		while(predecesores[it]!=0 )// es cuando llegué a salida y  le pedí predecesor
		{
			arista a; a.u_=it; a.w_= predecesores[it];
			aristas.push_back(a);
			nodos.push_back(it);
			it= predecesores[it];
		}
		 //si terminé me falta agregar el nodo del principio del camino y la arista que cierra el circuito
		nodos.push_back(it);
		arista a; a.u_=it; a.w_= llegada;
		aristas.push_back(a);

		pair<list<arista>,list<int> > res;
		res.first = aristas;
		res.second = nodos;
		
		return res;
}