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
{list<int> nodos_;	list<arista> aristas_; arista_costo MinGeneraArbol_;}; // es la arista que genera el ciclo, de costo mínimo entre las aristas no usadas


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
			{	
				if(grafo[i][j]!=-1)
					acum = acum+ grafo[i][j];
			}

	arista_costo min;
	min.costo_=acum;  min.u_=0; min.w_=0;

	for (int i = 1; i < n; ++i)
	{
		for (int j = 1; j < n; ++j)
		{
			if (grafo[i][j] !=-1 && arbol[i][j]==-1 && grafo[i][j] <= min.costo_ ) //si está definido, si no está en el árbol usados y el costo es más chico que el actual, cambio 	
			{min.costo_=grafo[i][j]; min.u_=i; min.w_=j;}
		}
	}
	//si nunca cambió min.u_ y quedó en 0, no hay solución.
	return min;
}

resAGM arbolGeneradorMinimo(vector<vector<int> >& grafo)
{
	int n = grafo.size();
	std::priority_queue<arista_costo> siguienteArista;
	list<int> nodos;
	list<arista> aristas;

	std::vector<bool> usados(n);
	for (int i = 0; i < n; ++i) // inicializo todos en false
		usados[i]= false;

	for (int i = 1; i < n; ++i) // empiezo con un nodo cualquiera, en este caso 1.
	{
		if (grafo[i][1] != -1)
		{	
			arista_costo a; a.u_=1; a.w_=i; a.costo_= grafo[i][1]; 
			siguienteArista.push(a);
		}
	} // ya tengo encolados los adyacentes a 1.
	usados[1] = true;
	nodos.push_back(1);	

	while(!siguienteArista.empty() )
	{
		int nodo = siguienteArista.top().w_;
		int adyacenteANodo = siguienteArista.top().u_;
		siguienteArista.pop();
		if (!usados[nodo])
		{
				usados[nodo]= true;
				nodos.push_back(nodo);
				arista a; a.w_=nodo; a.u_= adyacenteANodo;
				aristas.push_back(a);

			for (int i = 1; i < n; ++i)
			{
				if (grafo[i][nodo] != -1 && !usados[i]) // para que no vuelva a encolar nodos usados.
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
	res.MinGeneraArbol_= buscarGeneraCiclo(grafo,aristas);

	return res;
}

pair<list<arista>,list<int> > camino(int salida, int llegada, vector< vector<int> >& grafo)
{
	int n = grafo.size(); // n es la cantidad de nodos +1
	std::priority_queue< tuplaPila > heap;
	int prioridad=1;

	vector<bool> usados(n);
	for (int i = 0; i < n; ++i)	// inicializo las posiciones del vector con false
		usados[i]=false;

	vector<int> predecesores(n);
	predecesores[salida] = 0;

	list<int> adyacentes ;	//buscar los adyacentes de salida
	for (int i = 1; i < n; ++i)
		if (grafo[i][salida] != -1)
			adyacentes.push_back(i);

	tuplaPila t; t.vertice_ = salida; t.prioridad_= prioridad; t.adyacentesAVertice_= adyacentes;
	heap.push(t);
	usados[salida]= true;

	while(!heap.empty())
	{
		if (heap.top().adyacentesAVertice_.empty()) // si el vertice del tope de la pila no tiene más adyacentes
			heap.pop();
		else
		{
			int predecesor= heap.top().vertice_;

			list<int> adyacentes =heap.top().adyacentesAVertice_;
			int nodoParaAgregarNuevosAdyacentes = (heap.top().adyacentesAVertice_).front();
			adyacentes.pop_front();	// borro el primer nodo de la lista de adyacentes
			// como no puedo modificar la tupla porque top es const, tengo que crear una nueva tupla y meter los datos cambiados

			tuplaPila modificada;
			modificada.vertice_ = heap.top().vertice_;
			modificada.prioridad_= heap.top().prioridad_;
			modificada.adyacentesAVertice_= adyacentes;
			heap.pop(); 
			heap.push(modificada);

			list<int> adyacentesDelNuevoNodo;
			for (int i = 1; i < n; ++i)
				if (grafo[i][nodoParaAgregarNuevosAdyacentes] != -1 && !usados[i])
					adyacentesDelNuevoNodo.push_back(i);

			tuplaPila primeroDeAdyacentes;
			primeroDeAdyacentes.vertice_ = nodoParaAgregarNuevosAdyacentes;
			prioridad++; 
			primeroDeAdyacentes.prioridad_= prioridad;
			primeroDeAdyacentes.adyacentesAVertice_= adyacentesDelNuevoNodo;
			heap.push(primeroDeAdyacentes);

			usados[nodoParaAgregarNuevosAdyacentes]=true;
			predecesores[nodoParaAgregarNuevosAdyacentes]= predecesor;
		}
	}

		list<arista> aristas;
		list<int> nodos;

		int it = llegada;

		while(it!=salida )
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