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



// struct arista_costo
// {
// 	arista aris_;
// 	int costo_;
// 	bool operator< (const arista_costo& otra) const
// 	{
// 			return this->costo_ < otra.costo_; // es menor cuando el costo es mayor o igual

// 	}
// 	arista_costo operator = (const arista_costo& otra) const
// 	{
// 		arista_costo a;
// 		a.u_= otra.u_;
// 		a.w_= otra.w_;
// 		a.costo_= otra.costo_;
// 		return a;
// 	}

// };

struct arista_costo
{
	int u_;
	int w_;
	int costo_;
	bool operator< (const arista_costo& otra) const
	{
			return this->costo_ >= otra.costo_; // es menor cuando el costo es mayor o igual
	}

	bool operator!=(const arista_costo& otra) const
	{
		return this->costo_ != otra.costo_;
	}

	bool operator==(const arista_costo& otra) const
	{
		return this->costo_ == otra.costo_ &&  this->u_ == otra.u_ && this->w_ == otra.w_;
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
		for (int j = 1; j < n+1; ++j)
		{
			v[j]= -1;
		}
		grafo[i]=v; 
	}
	return grafo;
}


vector<vector<int> >  ponerPesoAlasAristas(int n, list<arista_costo> valores)
{	
	
	//armo el grafo
	vector<vector<int> > grafo = crearMatrizGrafo(n);

	// ahora voy a ir iterando la lista de valores para completar la matriz
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



arista_costo buscarMinima(list<arista_costo> aristas, vector<bool> usados)
{	

	arista_costo min;
	min.costo_=88888888; //MUCHO RUIDO
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

arista_costo buscarGeneraCiclo(list<arista_costo> aristasGrafo, list<arista> aristasArbol)
{	

	arista_costo min;
	min.costo_=88888888; //MUCHO RUIDO
	min.u_=0;
	min.w_=0;


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
				// tengo que refinar un poco esto, porque es muy borde
				// tengo que ponerme de acuerdo con que parte de la matriz voy a trabajar porque es simétrica.
				esta = true;
		}


		if ( !esta )	
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
		
		int u = buscarMinima(aristasGrafo, usados).u_;
		int w = buscarMinima(aristasGrafo, usados).w_;

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
	i++;

	}

	resAGM respuesta;

	respuesta.nodos_=nodos;	
	respuesta.aritas_= aristasArbol;
	
	respuesta.MinGeneraArbol_.u_=  buscarGeneraCiclo(aristasGrafo,aristasArbol).u_;
	respuesta.MinGeneraArbol_.w_= buscarGeneraCiclo(aristasGrafo,aristasArbol).w_;
	respuesta.MinGeneraArbol_.costo_= buscarGeneraCiclo(aristasGrafo,aristasArbol).costo_;

	
	// 	for (std::list<int>::iterator i = nodos.begin(); i != nodos.end(); ++i)
	// 	{
	// 		cout<< (*i)<<endl;
	// 	}
	// for (std::list<arista>::iterator i = aristasArbol.begin(); i != aristasArbol.end(); ++i)
	// {
	// 	cout<< (*i).u_<<","<<(*i).w_<<endl;
	// }

// cout<< buscarGeneraCiclo(aristasGrafo,aristasArbol).u_<<","<<buscarGeneraCiclo(aristasGrafo,aristasArbol).w_<<"costo:"<<buscarGeneraCiclo(aristasGrafo,aristasArbol).costo_<<endl;
	return respuesta;
}



pair<list<arista>,list<int> > camino(int salida, int llegada, vector< vector<int> > grafo, vector<int> predecesores, vector<bool> usados)
{	


	if (llegada == salida)
	{
		list<arista> aristas;
		list<int> nodos;

		int it = llegada; 

		while(predecesores[it]!=0 )// es cuando llegué al primer nodo y  le pedí predecesor
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
	else
	{	

		int n = grafo.size();

		// armo los predecesores
		list<int> tienenComoPredeASalida;

		for (int i = 1; i < n; ++i)
		{
			if (grafo[salida][i] != -1 && i !=llegada)
				tienenComoPredeASalida.push_back(i);
		}	

		// para esa lista que armé su predecesor es salida
		for (std::list<int>::iterator i = tienenComoPredeASalida.begin(); i != tienenComoPredeASalida.end(); ++i)	
		{	
			if ((*i)!=llegada)
				predecesores[(*i)]= salida;
		}

		// si no llegué a donde quería aplicar recursión con la lista, hacer cadena de predecesores.
		for (std::list<int>::iterator i = tienenComoPredeASalida.begin(); i != tienenComoPredeASalida.end(); ++i)	
		{
			if (!usados[(*i)]) 
				usados[(*i)]=true;
				camino((*i),llegada,grafo,predecesores, usados);

		}

	}
}



void anillo( vector< vector<int> > grafo)
{
	int n = grafo.size(); // es la cantidad de nodos +1

	list<int> nodos = arbolGeneradorMinimo(grafo).nodos_;
	list<arista> aristas = arbolGeneradorMinimo(grafo).aritas_;

	if (nodos.size() < n-1 || arbolGeneradorMinimo(grafo).MinGeneraArbol_.costo_ == -1) // si no usó todos los nodo o recibió basura en el mínimo, no hay solución 
		cout << "no";
	else
	{
		vector<int> predecesores(n);
		int desde = arbolGeneradorMinimo(grafo).MinGeneraArbol_.u_;
		int hasta = arbolGeneradorMinimo(grafo).MinGeneraArbol_.w_;
		predecesores[desde]=0; // para que saber cuando tengo que parar de pedir predecesores
		
		vector<bool> usados(n);
		usados[desde] =true; 

		list<arista> aristasAnillo = camino(desde, hasta, grafo, predecesores, usados).first;
		list<int> nodosAnillo = camino(desde, hasta, grafo, predecesores, usados).second;

		// ahora recorro las aristas del árbolGeneradoMinimo, si u y w están en los nodos del anillo entonces esa arista es parate
		// del anillo y no del resto de las computadoras

		// para saber rápido si pasa esto, meto los nodos del anillo en un vector de bool

		vector<bool> sonDeAnillo(n);
		for (int i = 1; i < n; ++i)
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

		//costo de las conexiones
		int costo;
		for (std::list<arista>::iterator it=aristas.begin(); it != aristas.end(); ++it)
		{	
			int u = (*it).u_;
			int w = (*it).w_;			
			costo = costo + grafo[u][w];
		}

		for (std::list<arista>::iterator it=aristasAnillo.begin(); it != aristasAnillo.end(); ++it)
		{	
			int u = (*it).u_;
			int w = (*it).w_;			
			costo = costo + grafo[u][w];
		}

		// tengo el costo de todas las conexiones en costo;	

		// cout << costo <<endl;

		// for (std::list<arista>::iterator it=aristasAnillo.begin(); it != aristasAnillo.end(); ++it)
		// {
		// 	cout << (*it).u_ <<" "<< (*it).w_<< endl;
		// }

		// for (std::list<arista>::iterator it=aristas.begin(); it != aristas.end(); ++it)
		// {
		// 	cout << (*it).u_ <<" "<< (*it).w_<< endl;
		// }

	}

}




int main()
{

	// string l;
	// std::getline(cin, l);
	// istringstream ss(l);
	// string token;
	// std::getline(ss, token, ' ');
	// int n, m;
	// n = atoi(token.c_str());
	// std::getline(ss, token, ' ');
	// m = atoi(token.c_str());
	// std::getline(cin, l);
	// getline(cin, l);



	// list<arista_costo> valores;

	// for (int i = 0; i < m; ++i)		
	// {
	// 	istringstream ss(l);
	// 	string token;
	// 	std::getline(ss, token, ' ');
	// 	int u, w, costo;
	// 	u = atoi(token.c_str());
	// 	std::getline(ss, token, ' ');
	// 	w = atoi(token.c_str());
	// 	std::getline(ss, token, ' ');
	// 	costo = atoi(token.c_str());

	// 	arista_costo a;
	// 	a.u_= u;
	// 	a.w_= w;
	// 	a.costo_= costo;

	// 	valores.push_back(a);

			
	// 	getline(cin, l);
	// }
	// cout<<"___________________________\n"<<"Solucion" <<endl;

	
	// vector<vector<int> > grafo = ponerPesoAlasAristas(n, valores);

	// anillo(grafo);



	int n=4;

	arista_costo a;

		a.u_= 1;
		a.w_= 2;
		a.costo_= 2;

	arista_costo a1;
		a1.u_= 2;
		a1.w_= 3;
		a1.costo_= 3;

	arista_costo a2;
		a2.u_= 2;
		a2.w_= 4;
		a2.costo_= 4;

	arista_costo a3;
		a3.u_= 1;
		a3.w_= 3;
		a3.costo_= 3;

	// arista_costo a4;
	// 	a4.u_= 6;
	// 	a4.w_= 7;
	// 	a4.costo_= 2;

	// arista_costo a5;
	// 	a5.u_= 7;
	// 	a5.w_= 8;
	// 	a5.costo_= 1;


	list<arista_costo> valores;
	valores.push_back(a);
	valores.push_back(a1);
	valores.push_back(a2);
	valores.push_back(a3);

	vector<vector<int> > grafo = ponerPesoAlasAristas(4, valores);
	// vector<vector<int> > grafo = crearMatrizGrafo(3)

	// for (int i = 1; i < grafo.size(); ++i)
	// {
	// 	for (int j = 1; j < grafo.size(); ++j)
	// 	{

	// 		cout << grafo[i][j]<<"("<<i<<","<<j<<")"<<endl;	
	// 	}
	// }
	arbolGeneradorMinimo(grafo);

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