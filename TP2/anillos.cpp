#include "anillos.h"
#include "time.h"
void comunidadDelAnillo( int cantNodos, list<arista_costo>& valores)
{	
	clock_t begin = clock();
	vector<vector<int> > grafo = ponerPesoAlasAristas(cantNodos, valores);

	int n = grafo.size(); // es la cantidad de nodos +1

	resAGM res = arbolGeneradorMinimo(grafo);
	 list<int> nodos = res.nodos_;
	list<arista> aristas = res.aristas_;
	int desde = res.MinGeneraArbol_.u_;
	int hasta = res.MinGeneraArbol_.w_;

	if (nodos.size() < n-1 ||  hasta== 0 ) 
	// si no usó todos los nodo o recibió basura, no hay solución
		cout << "no";
	else
	{

		//creo un grafo con las aristas del arbol generador minimo
		// pero ponerPesoAlaAristas toma una lista de arista_costo, voy a poner costo a las aristas del arbol
		list<arista_costo> arisCostoArbol;
		for (std::list<arista>::iterator i = aristas.begin(); i != aristas.end(); ++i)
		{
			arista_costo a;
			a.u_= (*i).u_; a.w_= (*i).w_; a.costo_= 0;
			arisCostoArbol.push_back(a);
		}

		vector< vector<int> > grafoArbol = ponerPesoAlasAristas(nodos.size(), arisCostoArbol);

		list<arista> aristasAnillo = camino(desde, hasta, grafoArbol).first;
		list<int> nodosAnillo = camino(desde, hasta, grafoArbol).second;

		// ahora recorro las aristas del árbolGeneradoMinimo, si u y w están en los nodos del anillo entonces esa arista es parte
		// del anillo y no del resto de las computadoras

		// para saber rápido si pasa esto, meto los nodos del anillo en un vector de bool
		// int cantNodosAnillo = nodosAnillo.size();
		vector<bool> sonDeAnillo(n);
		// inicializo el vector con todo false
		for (int i = 0; i < n; ++i)
			sonDeAnillo[i]=false;

		
		// ahora le pongo true en los nodos que son del anillo
		 for (std::list<int>::iterator it=nodosAnillo.begin(); it != nodosAnillo.end(); ++it)
		 {
		 	// cout<<*it<<" "<<sonDeAnillo.size()<<endl;
		 	sonDeAnillo[(*it)]=true;

}
		list<arista> aristaNoDeAnillo;
		 for (std::list<arista>::iterator it=aristas.begin(); it != aristas.end(); ++it)
		 {
		 	if (!sonDeAnillo[(*it).u_] || !sonDeAnillo[(*it).w_] ) // si no son parte del anillo
				aristaNoDeAnillo.push_back((*it));
		 }
		// costo de las conexiones, es el costo de todas las conexiones, anillo y demas.
		int costo=0;
		for (std::list<arista>::iterator it=aristaNoDeAnillo.begin(); it != aristaNoDeAnillo.end(); ++it)
			costo = costo + grafo[(*it).u_][(*it).w_];

		for (std::list<arista>::iterator it=aristasAnillo.begin(); it != aristasAnillo.end(); ++it)
			costo = costo + grafo[(*it).u_][(*it).w_];

		cout << "costo:"<<costo <<endl;
		cout << "anillo:"<<endl;
		for (std::list<arista>::iterator it=aristasAnillo.begin(); it != aristasAnillo.end(); ++it)
			cout << (*it).u_ <<" "<< (*it).w_<< endl;

		cout<<endl;
		cout<< "demas:";
		for (std::list<arista>::iterator it=aristaNoDeAnillo.begin(); it != aristaNoDeAnillo.end(); ++it)
			cout << (*it).u_ <<" "<< (*it).w_<< endl;
	}

	clock_t end = clock();
    float elapsed_msecs = (float(end - begin) / CLOCKS_PER_SEC) * 1000;
    DarTiempo(elapsed_msecs);

}

void generadorDeInsatancias (int cantPC, int r)
{	
	srand (time (NULL));
	list<arista_costo> valores;

	for (int i = 1; i < cantPC; ++i)
	{
		for (int j = i+1; j <= cantPC; ++j)
				{
					arista_costo a;
					a.u_= i; a.w_= j; a.costo_= rand() %r;
					valores.push_back(a);
				}		
	}
	// for (std::list<arista_costo>::iterator i = valores.begin(); i != valores.end(); ++i)
	// {
	// 		cout<< "("<<(*i).u_<< "," << (*i).w_ << "," << (*i).costo_ << ")" << endl;
	// }

	comunidadDelAnillo(cantPC, valores);

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
	// getline(cin, l);

	// list<arista_costo> valores;

	// for (int i = 0; i < m; ++i) //por qué tengo que poner m-1?
	// {
	// 	istringstream ss2(l);
	// 	string token;
	// 	std::getline(ss2, token, ' ');
	// 	int u, w, costo;
	// 	u = atoi(token.c_str());
	// 	std::getline(ss2, token, ' ');
	// 	w = atoi(token.c_str());
	// 	std::getline(ss2, token, ' ');
	// 	costo = atoi(token.c_str()); 

	// 	arista_costo a;
	// 	a.u_= u; a.w_= w; a.costo_= costo;
	// 	valores.push_back(a);
	// 	getline(cin, l);
	// }
	// cout<<"___________________________\n"<<"Solucion" <<endl;

	
	// comunidadDelAnillo(n,valores);
	// for (int i = 1; i < 10000; ++i)
	// {
		generadorDeInsatancias(3, 50);
		/* code */
	// }

	return 0;
}
// int n=9;

// 	arista_costo a;

// 		a.u_= 1;
// 		a.w_= 2;
// 		a.costo_= 4;

// 	arista_costo a1;
// 		a1.u_= 1;
// 		a1.w_= 8;
// 		a1.costo_= 8;

// 	arista_costo a2;
// 		a2.u_= 2;
// 		a2.w_= 3;
// 		a2.costo_= 8;

// 	arista_costo a3;
// 		a3.u_= 3;
// 		a3.w_= 4;
// 		a3.costo_= 6;

// 	arista_costo a4;
// 		a4.u_= 4;
// 		a4.w_= 5;
// 		a4.costo_= 9;

// 	arista_costo a5;
// 		a5.u_= 5;
// 		a5.w_= 6;
// 		a5.costo_= 10;
	
// 	arista_costo a6;
// 		a6.u_= 6;
// 		a6.w_= 7;
// 		a6.costo_= 3;

// 	arista_costo a7;
// 		a7.u_= 7;
// 		a7.w_= 8;
// 		a7.costo_= 1;	

// 	arista_costo a8;
// 		a8.u_= 8;
// 		a8.w_= 9;
// 		a8.costo_= 6;	

// 	arista_costo a9;
// 		a9.u_= 2;
// 		a9.w_= 8;
// 		a9.costo_= 12;	

// 	arista_costo a10;
// 		a10.u_= 3;
// 		a10.w_= 9;
// 		a10.costo_= 3;	

// 	arista_costo a11;
// 		a11.u_= 9;
// 		a11.w_= 7;
// 		a11.costo_= 5;	

// 	arista_costo a12;
// 		a12.u_= 3;
// 		a12.w_= 6;
// 		a12.costo_= 4;	

// 	arista_costo a13;
// 		a13.u_= 4;
// 		a13.w_= 6;
// 		a13.costo_= 13;



// 	list<arista_costo> valores;
// 	valores.push_back(a);
// 	valores.push_back(a1);
// 	valores.push_back(a2);
// 	valores.push_back(a3);
// 	valores.push_back(a4);
// 	valores.push_back(a5);
// 	valores.push_back(a6);
// 	valores.push_back(a7);
// 	valores.push_back(a8);
// 	valores.push_back(a9);
// 	valores.push_back(a10);
// 	valores.push_back(a11);
// 	valores.push_back(a12);
// 	valores.push_back(a13);

// 	vector<vector<int> > grafo = ponerPesoAlasAristas(n, valores);
// 	anillo(grafo);

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
