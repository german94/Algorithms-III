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
	arista operator = (const arista& otra) const
	{
		arista a;
		a.u_= otra.u_;
		a.w_= otra.w_;
		return a;
	}
};

struct arista_costo
{
	arista aris_;
	int costo_;
	bool operator< (const arista_costo& otra) const
	{
			return this->costo_ < otra.costo_;

	}
	arista_costo operator = (const arista_costo& otra) const
	{
		arista_costo a;
		a.aris_.u_= otra.aris_.u_;
		a.aris_.w_= otra.aris_.w_;
		a.costo_= otra.costo_;
		return a;
	}

};

struct resAGM //lo que retorna el arbol generador mínimo
{
	list<int> nodos_;
	list<arista> aritas_;
	arista_costo MinGeneraArbol_;
};


vector<vector<int> > crearMatrizGrafo(int n)
{
	vector<vector<int> > grafo(n+1);
	for (int i = 1; i < n+1; ++i)
	{	
		std::vector<int> v(n+1);
		for (int j = 1; j < n; ++j)
		{
			v[j]= -1;
		}
		grafo[i]=v; 
	}
}


vector<vector<int> >  ponerPesoAlasAristas(int n, list<arista_costo> valores)
{	
	//armo el grafo
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
	for (int i = 1; i < n+1; ++i)
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



resAGM arbolGeneradorMinimo(vector<vector<int> > grafo)
{	
	std::priority_queue< arista_costo > heap;

	int n = grafo.size();

	//para saber si un nodo está en uso ya, uso 
	//un vector
	std::vector<bool> usados(n+1);
	for (int i = 1; i < n+1; ++i)
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

	while (i<n+1)
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
	respuesta.MinGeneraArbol_= heap.top();
	return respuesta;
}

pair<list<arista>,list<int> > camino(int salida, int llegada, vector< vector<int> > grafo, vector<int> predecesores)
{
	int n = grafo.size();

	vector<int> tienenComoPredeASalida = grafo[salida];

	for (int i = 1; i < n+1; ++i)
	{
		if (tienenComoPredeASalida[i] != -1)
			predecesores[i]= salida;
	}

	for (int i = 1; i < n+1; ++i)
	{
		if (tienenComoPredeASalida[i] != llegada)
			camino(tienenComoPredeASalida[i],llegada,grafo,predecesores);
		else 
			{	// llegué al nodo del  final del camino, tengo que dar el camino.
				list<arista> aristas;
				list<int> nodos;

				int it = tienenComoPredeASalida[i]; // es llegada
				while(predecesores[it]=!0 )// es cuando llegué al primer nodo que que le pedí predecesor
				{
					arista a;
					a.u_=it;
					a.w_= predecesores[it];
					aristas.push_back(a);
					nodos.push_back(it);
					it= predecesores[it];
				}
				 //si terminé me falta agregar el nodo del principio del camino y la arista que cierra el circuido
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
	}




}

void anillo( vector< vector<int> > grafo)
{
	int n = grafo.size();


	list<int> nodos = arbolGeneradorMinimo(grafo).nodos_;
	list<arista> aristas = arbolGeneradorMinimo(grafo).aritas_;
	arista_costo minima = arbolGeneradorMinimo(grafo).MinGeneraArbol_;

	if (nodos.size()!=n)
		cout << "no";
	else
	{
		vector<int> predecesores(n+1);
		int desde = minima.aris_.u_;
		int hasta = minima.aris_.w_;
		predecesores[desde]=0; // para que saber cuando tengo que parar depedir predecesores

		list<arista> aristasAnillo = camino(desde, hasta, grafo, predecesores).first;
		list<int> nodosAnillo = camino(desde, hasta, grafo, predecesores).second;

		// ahora recorro las aristas del árbolGeneradoMinimo, si u y w están en los nodos del anillo entonces esa arista es parate
		// del anillo y no del resto de las computadoras

		// para saber rápido si pasa esto, meto los nodos del anillo en un vector de bool

		vector<bool> sonDeAnillo(n+1);
		for (int i = 1; i < n+1; ++i)
		{
			sonDeAnillo[i]=false;
		}

		for (std::list<int>::iterator it=nodosAnillo.begin(); it != nodosAnillo.end(); ++it)
		{
			sonDeAnillo[(*it)]=true;
		}


		list<arista> aristaNoDeAnillo; 
		for (std::list<arista>::iterator it=aristas.begin(); it != aristas.end(); ++it)
		{
			if (!sonDeAnillo[(*it).u_] && sonDeAnillo[(*it).w_]) // si no son parte del anillo
				aristaNoDeAnillo.push_back((*it));
		}

		// falta hacer el cout del resultado.
	}

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