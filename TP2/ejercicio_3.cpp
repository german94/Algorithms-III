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
	arista aris_;
	int costo_;
	bool operator< (const arista_costo& otra) const
	{
			return this->costo_ < otra.costo_;
	}

};

struct resAGM
{
	list<int> nodos_;
	list<arista> aritas_;
	bool aristaValida_;
	arista_costo MinGeneraArbol_;
};


vector<vector<int> > crearMatrizGrafo(int n)
{
	vector<vector<int> > grafo(n+1);
	for (int i = 1; i < n+1; ++i)
	{	
		std::vector<int> v;
		for (int j = 1; j < n; ++j)
		{
			v[j]= -1;
		}
		grafo[i]=v; 
	}
}


vector<vector<int> >  ponerPesoAlasAristas(int n, list<arista_costo> valores)
{	
	//armo la matriz con todas posiciones sin  aritas
	vector<vector<int> > grafo = crearMatrizGrafo(n);

	// ahora voy a ir iterando la lista de valores para completar la matriz
	for (std::list<arista_costo>::iterator it=valores.begin(); it != valores.end(); ++it)
	{
		int i = (*it).aris_.u_;
		int j = (*it).aris_.w_;
		int costo = (*it).costo_;
		grafo[i][j]=costo;
		grafo[j][i]=costo; // pues es simétrico

	}

	return grafo;

}


void guardarAdyacentes(vector<vector<int> > grafo, int j, std::priority_queue< arista_costo > heap, vector<bool> usados)
{
	int n = grafo.size();
	for (int i = 1; i <= n; ++i)
	{
		if (grafo[i][j] != -1 && !usados[i]) // hay arista entre i j y no genero ciclos.
			{	
				arista aux;
				arista_costo aCosto;
				aux.u_= i;
				aux.w_= j;
				aCosto.costo_= grafo[i][j];
				aCosto.aris_= aux;

				heap.push(aCosto);
			}

	}
}
// voy a utilizar una matriz de aristas
// la coordenada a,b es el costo de la arista a,b


resAGM arbolGeneradorMinimo(vector<vector<int> > grafo)
{	
	std::priority_queue< arista_costo > heap;

	int n = grafo.size();

	//para saber si un nodo está en uso ya, uso 
	//un vector
	std::vector<bool> usados(n);
	for (int i = 0; i < n; ++i)
	{
		usados[i]= false;
	}

	// cualquier nodo

	int cualquiera = 1;
	// encolo sus adyacentes
	usados[1]= true;
	guardarAdyacentes(grafo, cualquiera, heap, usados);

	std::list <int> nodos;
	nodos.push_back(cualquiera);

	std::list <arista> aristas;
	int i=1;

	while (i<n)
	{
		//guardo la nueva arista, que está en el tope del heap
		aristas.push_back(heap.top().aris_); 
		// actualizo los nodos usados, si un lado de la arista nueva no está usado tengo que ponerlo como usado
		// y guardarme los adyacentes
		if (usados[heap.top().aris_.u_] )
		{
			usados[heap.top().aris_.w_] = true;
			guardarAdyacentes(grafo, heap.top().aris_.w_ , heap, usados);
			nodos.push_back(heap.top().aris_.w_);
		}
		else 
		{
			usados[heap.top().aris_.u_] = true;
			guardarAdyacentes(grafo, heap.top().aris_.u_ , heap, usados);
			nodos.push_back(heap.top().aris_.u_);

		}

	heap.pop();
	i++;

	}

	resAGM respuesta;

	respuesta.nodos_=nodos;	
	respuesta.aritas_= aristas;
	respuesta.aristaValida_= heap.empty();
	// si está vacio quiere decir que era un árbol el dato de entrada y no hay solución
	if (!heap.empty())
		respuesta.MinGeneraArbol_= heap.top();
	else // busco
	{	
		arista_costo a;
		a.costo_= 0;
		arista aux;
		aux.u_=0;
		aux.w_=0;
		a.aris_= aux;
		respuesta.MinGeneraArbol_= a;
	}

	return respuesta;
}


void anillo( vector< vector<int> > grafo)
{
	int n = grafo.size();

	// *_*_*_*_*__*_*_*_*_***_
	//buscar mínimo de cada uno;
	for (int i = 0; i < n; ++i)
	{
		
	}


	list<int> nodos = arbolGeneradorMinimo(grafo).nodos_;
	// list<arista> = arbolGeneradorMinimo(grafo).aristas aritas_;
	bool valida = arbolGeneradorMinimo(grafo).aristaValida_ ;
	arista_costo minima = arbolGeneradorMinimo(grafo).MinGeneraArbol_;

	if (!valida)
		cout << "no";
	else // agrego la arista para que se forme el árbol.
	{	
		int u= minima.aris_.u_;
		int w= minima.aris_.w_;
		grafo[u][w]= minima.costo_;
		grafo[w][u]= minima.costo_; // porque son simétricos

		// ahora tengo que buscar el circuito.

		// list<arista> = buscarRecorridoMinimo(grafo, u,w);

	}

	//_*_*_*_*_*_*_*
	// falta evolver el resultado




	// list<int> nodos = arbolGeneradorMinimo(grafo).first;
	// list<arista> aristas = arbolGeneradorMinimo(grafo).second;

	// // ahora tengo que ver si el generador usó todos los nodos
	// if (nodos.size() != n+1)
	// 	cout << "no":
	// else
	// {

	// }

}




int main()
{

	string l;
	std::getline(cin, l);
	istringstream ss(l);
	string token;
	std::getline(ss, token, ' ');
	int n, m;
	n = atoi(token.c_str());
	std::getline(ss, token, ' ');
	m = atoi(token.c_str());
	std::getline(cin, l);


	list<arista_costo> valores;

	for (int i = 0; i < m; ++i)		
	{
		istringstream ss(l);
		string token;
		std::getline(ss, token, ' ');
		int u, w, costo;
		u = atoi(token.c_str());
		std::getline(ss, token, ' ');
		w = atoi(token.c_str());
		std::getline(ss, token, ' ');
		costo = atoi(token.c_str());

		arista_costo a;
		a.aris_.u_= u;
		a.aris_.w_= w;
		a.costo_= costo;

		valores.push_back(a);

			
		getline(cin, l);
	}
	cout<<"___________________________\n"<<"Solucion" <<endl;

	
	vector<vector<int> > grafo = ponerPesoAlasAristas(n, valores);

	anillo(grafo);



	return 0;
}
//_*_*_*_*_*_*_*_*_*_*_*
//pseuso:


// 1)filtrar todas las conexiones iguales quedandonos con el mínimo de cada una
// 2)arbol generador mínimo.
// 3) el arbol generador mínimo no usa todos los nodos?
// 	a) no, entonces no hay solución
// 	b) si, agregamos una arista al arbol generador mínimo, 
// 		(la mínima de las que no usa (a,b))
// 4) guardamos el recorrido de a-b y junto con la arista a agregar ese es el 
// 	anillo de servidores
// 5) devolver el anillo y las demás computadoras