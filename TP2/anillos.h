#include <iostream>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <ctime>
#include <list>
#include <queue>

using namespace std;


struct arista
{
	int u_;
	int w_;
};

struct arista_costo
{
	int u_;
	int w_;
	int costo_;
	bool operator< (const arista_costo& otra) const
	{
			return this->costo_ >= otra.costo_; // es menor cuando el costo es mayor o igual, para que el heap sea un minheap
	}
};

struct resAGM //lo que retorna el arbol generador mínimo
{
	list<int> nodos_;
	list<arista> aritas_;
	arista_costo MinGeneraArbol_; // es la arista que genera el ciclo, de costo mínimo entre las aristas no usadas
};

struct tuplaPila
{
	int prioridad_;
	int vertice_;
	list<int > adyacentesAVertice_;

	bool operator< (const tuplaPila& otra) const
	{
			return this->prioridad_ >= otra.prioridad_; // es menor cuando el costo es mayor o igual, para que el heap sea un minheap
	}

};

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

vector<vector<int> >  ponerPesoAlasAristas(int n, list<arista_costo> valores)
{

	vector<vector<int> > grafo = crearMatrizGrafo(n);
	for (std::list<arista_costo>::iterator it=valores.begin(); it != valores.end(); ++it)
	{
		int i = (*it).u_;
		int j = (*it).w_;
		int costo = (*it).costo_;
		grafo[i][j]=costo;
		grafo[j][i]=costo; // pues es simétrico

	}

	return grafo;

}

arista_costo buscarMinima(list<arista_costo> aristas, vector<bool> usados, int max)
{

	arista_costo min;
	min.costo_=max+1;
	min.u_=0;
	min.w_=0;


	for (std::list<arista_costo>::iterator i = aristas.begin(); i != aristas.end(); ++i)
	{
		if ( (usados[(*i).u_] && !usados[(*i).w_]) || (usados[(*i).w_] && !usados[(*i).u_]) )
		{
			if((*i).costo_ < min.costo_)
			{
				min.costo_=(*i).costo_;
				min.u_= (*i).u_;
				min.w_=(*i).w_;
			}

		}
	}

	return min;
}

arista_costo buscarGeneraCiclo(list<arista_costo> aristasGrafo, list<arista> aristasArbol, int max)
{

	arista_costo min;
	min.costo_=max+1; 
	min.u_=0;
	min.w_=0;

	// busco una arista del grafo que no sea parte del arbol y sea mínima
	for (std::list<arista_costo>::iterator i = aristasGrafo.begin(); i != aristasGrafo.end(); ++i)
	{
			arista_costo busco;
				busco.costo_=(*i).costo_;
				busco.u_= (*i).u_;
				busco.w_=(*i).w_;

			bool esta = false;

		for (std::list<arista>::iterator it = aristasArbol.begin(); it != aristasArbol.end(); ++it)
		{
			if ((busco.u_ == (*it).u_ && busco.w_ == (*it).w_) || (busco.w_ == (*it).u_ && busco.u_ == (*it).w_))
				// si la arista que agarré o su simétrica está en el arbol
				esta = true;
		}


		if ( !esta ) // si la arista no era parte del arbol quiero verificar si es mínima.
		{
			if(busco.costo_ < min.costo_)
			{
				min.costo_=busco.costo_;
				min.u_= busco.u_;
				min.w_=busco.w_;
			}
		}
	}


	return min;
}



resAGM arbolGeneradorMinimo(vector<vector<int> > grafo)
{


	int n = grafo.size();
	int max=0;
	//armar la lista de las aristas
	list<arista_costo> aristasGrafo;


	for (int i = 1; i < n; ++i)
	{
		for (int j = 1; j < n; ++j)
		{
			if (grafo[i][j] != -1)
			{
				arista_costo a;
				a.u_= i;
				a.w_= j;
				a.costo_= grafo[i][j];
				aristasGrafo.push_back(a);
				max= max + grafo[i][j];
			}
		}
	}

	//para saber si un nodo está en uso ya, uso un vector
	std::vector<bool> usados(n);
	for (int i = 1; i < n; ++i)
	{
		usados[i]= false;
	}

	int cualquiera = 1;
	usados[cualquiera]= true;

	std::list <int> nodos;
	nodos.push_back(cualquiera);

	std::list <arista> aristasArbol;
	int i=2;

	while (i<n)
	{

		int u = buscarMinima(aristasGrafo, usados, max).u_;
		int w = buscarMinima(aristasGrafo, usados, max).w_;

		if(u!=0) // si no me dió basura, proceso la arista a agregar
		{
			arista a;
			a.u_=u;
			a.w_=w;
			aristasArbol.push_back(a);

			if (usados[u] )
			{
				if (!usados[w])
				{
					usados[w] = true;
					nodos.push_back(w);
				}
			}
			if (usados[w])
			{
				if (!usados[u])
				{
					usados[u] = true;
					nodos.push_back(u);
				}

			}
		}
		i++;


	}

	resAGM respuesta;
	respuesta.nodos_=nodos;
	respuesta.aritas_= aristasArbol;

	respuesta.MinGeneraArbol_.u_=  buscarGeneraCiclo(aristasGrafo,aristasArbol, max).u_;
	respuesta.MinGeneraArbol_.w_= buscarGeneraCiclo(aristasGrafo,aristasArbol, max).w_;
	respuesta.MinGeneraArbol_.costo_= buscarGeneraCiclo(aristasGrafo,aristasArbol, max).costo_;

	return respuesta;
}



pair<list<arista>,list<int> > camino(int salida, int llegada, vector< vector<int> > grafo)
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
	t.vertice_ = salida;
	t.prioridad_= prioridad;
	t.adyacentesAVertice_= adyacentes;

	heap.push(t);
	usados[salida]= true;
	predecesores[salida]= 0; // para saber cuando tengo que dejar de pedir predecesores

	while(!heap.empty())
	{
		if (heap.top().adyacentesAVertice_.empty()) // si el vertice del tope de la pila no tiene más adyacentes
		{
			heap.pop();
		}
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
			arista a;
			a.u_=it;
			a.w_= predecesores[it];
			aristas.push_back(a);
			nodos.push_back(it);
			it= predecesores[it];
		}
		 //si terminé me falta agregar el nodo del principio del camino y la arista que cierra el circuito
		nodos.push_back(it);
		arista a;
		a.u_=it;
		a.w_= llegada;
		aristas.push_back(a);

		pair<list<arista>,list<int> > res;
		res.first = aristas;
		res.second = nodos;
		
		return res;
}