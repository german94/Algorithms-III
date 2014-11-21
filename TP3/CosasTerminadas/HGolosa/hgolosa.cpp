#include "auxiliares.h"

void heuristica_golosa(float* const * mAdy, vector<vector<int> >& particion, int n)
{
	clock_t begin = clock();

	list<int> cola;
	float pesoMax = -1;
	int a;
	int b;
	int nodoActual;
	int agregados;
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

	//si k = 1 entonces metemos todo en el mismo conjunto y terminamos (k <= 0 no tendria sentido)
	if(particion.size() == 1)
	{
		for(int i = 0; i < n; i++)
			particion[0].push_back(i);

		return;
	}

	particion[0].push_back(a);
	particion[1].push_back(b);
	marcados[a] = true;
	marcados[b] = true;
	encolarAdyacentesNoAgregados(a, n, mAdy, marcados, cola, encolados);

	agregados = 2;
	nodoActual = b;
	while(agregados < n)
	{	
		int adyacente = -1;

		if(nodoActual != -1 && tieneAdyacenteNoAgregado(nodoActual, n, mAdy, marcados))
		{
			adyacente = dameAdyacenteNoMarcado(nodoActual, n, mAdy, marcados);
			marcados[adyacente] = true;
			encolarAdyacentesNoAgregados(nodoActual, n, mAdy, marcados, cola, encolados);
		}
		else
		{
			while((adyacente == -1 || marcados[adyacente]) && !cola.empty())
			{
				adyacente = cola.front();
				cola.pop_front();
			}

			if(adyacente == -1 || marcados[adyacente])
				adyacente = dameAlgunoQueFalte(marcados);
		}

		if(tieneAdyacenteNoAgregado(adyacente, n, mAdy, marcados))
		{
			int par = dameParNoMarcado(adyacente, n, mAdy, marcados);
			meterParDeVerticesEnParticion(adyacente, par, mAdy, particion);
			marcados[par] = true;
			encolarAdyacentesNoAgregados(adyacente, n, mAdy, marcados, cola, encolados);
			agregados += 2;
			nodoActual = par;
		}
		else
		{
			meterVerticeEnParticion(adyacente, mAdy, particion);
			nodoActual = -1;
			agregados++;
		}

		marcados[adyacente] = true;
	}

	clock_t end = clock();
    double elapsed_msecs = (double(end - begin) / CLOCKS_PER_SEC) * 1000;
    DarTiempo(elapsed_msecs);
}

int main()
{/*
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
*/

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

		vector<vector<int> > particion(particiones, vector<int>());
				
		float **pesos = new float *[vertices];
		for(int i = 0; i < vertices; i++)  {pesos[i] = new float[vertices];}
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
			// cout << i<<endl;
		} 
			// cout << "paso3"<<endl;
			heuristica_golosa(pesos, particion, vertices);
			mostrar(vertices, particion);

			for(int i = 0; i < vertices; i++) {free (pesos[i]);} //LIBERO MEMORIA
				free (pesos);
			std::getline(cin, l);

	}
    return 0;
}