#include "auxiliares.h"

void heuristica_golosa(float* const * mAdy, vector<vector<int> >& particion, int n)
{
	list<int> cola;
	float pesoMax = -1;
	int a;
	int b;
	int nodoActual;
	vector<bool> marcados(n, false);		//aca voy a tener como true a los nodos que ya puse en algun conjunto de la particion
	vector<bool> encolados(n, false);		//aca voy a tener como true a los nodos que estan encolados o alguna vez fueron encolados
	pesoMax = aristaDePesoMaximo(n, mAdy, a, b);		//tomo la arista cuyo peso es maximo sobre todas las demas, y guardo sus vertices en a y b

	//si en pesoMax (el peso de la arista maxima) tengo -1, quiere decir que no existe una arista de peso maximo, por lo tanto no existe 
	//una arista, entonces el grafo no tiene aristas y puedo meter todo en el primer conjunto de la particion y terminar
	if(pesoMax == -1)
	{
		for(int i = 0; i < n; i++)
			particion[0].push_back(i);

		return;
	}

	//si k = 1 entonces directamente no llamamos a esta funcion y metemos todo en ese conjunto
	particion[0].push_back(a);
	particion[1].push_back(b);
	marcados[a] = true;
	marcados[b] = true;
	encolarAdyacentesNoAgregados(a, n, mAdy, marcados, cola, encolados);

	nodoActual = b;

	while(!cola.empty() || nodoActual != -1)
	{	
		int adyacente;

		if(tieneAdyacenteNoAgregado(nodoActual, n, mAdy, marcados))
		{
			adyacente = dameAdyacenteNoMarcado(nodoActual, n, mAdy, marcados);
			marcados[adyacente] = true;
			encolarAdyacentesNoAgregados(nodoActual, n, mAdy, marcados, cola, encolados);
		}
		else
		{
			while(marcados[adyacente] && !cola.empty())
			{
				adyacente = cola.front();
				cola.pop_front();
			}

			if(marcados[adyacente] && cola.empty())
				return;
		}

		
		int par = -1;
		if(tieneAdyacenteNoAgregado(adyacente, n, mAdy, marcados))
			par = dameParNoMarcado(adyacente, n, mAdy, marcados);

		if(par != -1)
		{
			meterParDeVerticesEnParticion(adyacente, par, mAdy, particion);
			marcados[adyacente] = true;
			marcados[par] = true;
			encolarAdyacentesNoAgregados(adyacente, n, mAdy, marcados, cola, encolados);
			nodoActual = par;
		}
		else
		{
			meterVerticeEnParticion(adyacente, mAdy, particion);
			nodoActual = -1;
			if(!cola.empty())
				nodoActual = cola.front();
		}

		cout<<"_______________"<<endl;
	}
}

int main()
{
	int vertices;
	int aristas;
	int particiones;
	cin >> vertices;
	cin >> aristas;
	cin >> particiones;
	vector<vector<int> > particion(particiones, vector<int>());
	float** pesos = new float *[vertices];
	for(int i = 0; i < vertices; i++)  
	{
		pesos[i] = new float[vertices];
		for(int j = 0; j < vertices; j++)		//inicializo los pesos todos con -1 para que despues si dos nodos no son adyacentes no haya fruta
			pesos[i][j] = -1;
	}
	int vertice_a;
	int vertice_b;
	float peso; 

	for(int i = 0; i < aristas; i++){
		cin >> vertice_a;
		cin >> vertice_b;
		cin >> peso;
		pesos[vertice_a -1][ vertice_b -1] = peso;
		pesos[vertice_b -1][ vertice_a -1] = peso;
	} //tengo una matriz con todos los pesos

	heuristica_golosa(pesos, particion, vertices);
	mostrar(pesoParticion(particion, pesos), particion);

	for(int i = 0; i < vertices; i++) {free (pesos[i]);} //LIBERO MEMORIA
	free (pesos);	

    return 0;
}