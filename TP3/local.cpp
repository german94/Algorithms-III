
#include "local.h"

float costo_solu(float **pesos, solucion &solu){
	int suma_solucion = 0;
	int k = solu.size();
	for (int i = 0; i < k; ++i)
	{
		for (std::vector<int>::iterator elem1 = solu[i].begin(); elem1 != solu[i].end(); ++elem1)
		{
			for (std::vector<int>::iterator elem2 = elem1 ; elem2 != solu[i].end(); ++elem2)
			{
				suma_solucion += pesos[(*elem1)][(*elem2)];
			}
		}
		
	}
	return suma_solucion;
}

void mostrar(float ** pesos, solucion &solu){//SOLO MUESTRA EL RESULTADO
	int suma_solucion = costo_solu(pesos, solu);

	cout << "suma:" << suma_solucion << endl; 
	for (int t = 0; t < solu.size(); t++){ 
		for (int j = 0; j < solu[t].size(); j++){
			cout << t +1 << "_" << solu[t][j] + 1 << " ";
		}
	}
	cout << endl;	
}

vector<pair<solucion,float> > crear_vecindad(float **pesos, float suma_solucion, solucion &solu) {
	vector<pair<solucion,float> > vecinos;
	int cantDeConjuntos = solu.size();

	for(int i = 0; i < cantDeConjuntos ; i++) //tomo un conjunto 
	{	
		for (std::vector<int>::iterator elem = solu[i].begin(); elem != solu[i].end(); ++elem)// tomo un elemento del conjunto
		{
			for (int k = 0; k < cantDeConjuntos; ++k) // tomo el elemento y lo meto en algún conjunto distinto al que estaba
			{	
				if(k!=i) // para meter elem en otra caja distinta a donde estaba
				{
					solucion nuevo = solu;
					for (std::vector<int>::iterator it = nuevo[i].begin(); it != nuevo[i].end(); ++it) // busco en la solución que copié el elemento a borrar
					{
						if((*it) == (*elem))
						{
							nuevo[i].erase(it);
							break;
						}
					}
					nuevo[k].push_back((*elem));
					int nuevoCosto = costo_solu(pesos, nuevo);
					if (nuevoCosto < suma_solucion) // si hay una majora es un vecino
					{
						pair <solucion, float> vecino(nuevo, nuevoCosto);
						vecinos.push_back(vecino);
					}		
				}
			}	
					
		}
	}
	return vecinos;
}

int seleccionar_vecino(vector< pair<solucion,float> >& vecinos) { // busco cual tiene el menor costo de los vecinos.
	int ret = -1;
	int cantVecinos = vecinos.size();
	for(int i = 0; i < cantVecinos; i++)
	{
		if(ret == -1 || ((vecinos[i]).second > ret) )
			ret = i;
	}
	return ret;
}

void kpmp_busqueda_local(float** pesos, solucion& mejor, float suma_solucion) {
	vector<pair<solucion,float> > vecinos = crear_vecindad(pesos, suma_solucion, mejor); // para cada vecino en vecinos first es el vecino y second es el costo de él.
	solucion posible_mejor = vecinos[seleccionar_vecino(vecinos)].first;
	float posible_suma_mejor = vecinos[seleccionar_vecino(vecinos)].second;
	// mejor = posible_mejor;

	while(suma_solucion > posible_suma_mejor)
	{	
		mejor = posible_mejor;
		suma_solucion = posible_suma_mejor;
		vecinos = crear_vecindad(pesos, suma_solucion, mejor);
		int nuevoVecino = seleccionar_vecino(vecinos);
		if(nuevoVecino >= 0) // porque si no hay vecinos seleccionar da -1
		{
			posible_mejor = vecinos[nuevoVecino].first;
			posible_suma_mejor = vecinos[nuevoVecino].second;		
		}
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
	float **pesos = new float *[vertices];
	for(int i = 0; i < vertices; i++)  {pesos[i] = new float[vertices];}
	int vertice_a;
	int vertice_b;
	float peso; 
	int i = 0;

	for(i=0; i < aristas; i++){
		cin >> vertice_a;
		cin >> vertice_b;
		cin >> peso;
		pesos[vertice_a -1][ vertice_b -1] = peso;
		pesos[vertice_b -1][ vertice_a -1] = peso;
	} //tengo una matriz con todos los pesos
	solucion  solu(particiones, vector<int>());

	for (int i = 0; i < vertices; ++i) // pongo todos en 1
	 	solu[0].push_back(i);

	int costo_solucion = costo_solu(pesos,solu);
	kpmp_busqueda_local(pesos,solu,costo_solucion);
	mostrar(pesos,solu);
		
	for(int i = 0; i < vertices; i++) {free (pesos[i]);} //LIBERO MEMORIA
	free (pesos);
    return 0;
}



