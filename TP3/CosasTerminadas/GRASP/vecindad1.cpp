
#include "vecindad1.h"

float costo_solu(float *const*pesos, solucion &solu){
	int suma_solucion = 0;
	int k = solu.size();
	for (int i = 0; i < k; ++i)
	{
		for (std::vector<int>::iterator elem1 = solu[i].begin(); elem1 != solu[i].end(); ++elem1)
		{
			for (std::vector<int>::iterator elem2 = elem1 ; elem2 != solu[i].end(); ++elem2)
			{
				if(pesos[(*elem1)][(*elem2)] >= 0)
					suma_solucion += pesos[(*elem1)][(*elem2)];
			}
		}
		
	}
	return suma_solucion;
}

vector<pair<solucion,float> > crear_vecindad(float *const*pesos, float suma_solucion, solucion &solu) {
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

void vecindad1(float*const* pesos, solucion& mejor, float suma_solucion) {
	// // inciar reloj:
	// clock_t begin = clock();

	solucion posible_mejor;
	float posible_suma_mejor = suma_solucion;
	vector<pair<solucion,float> > vecinos = crear_vecindad(pesos, suma_solucion, mejor); // para cada vecino en vecinos first es el vecino y second es el costo de él.
	if (vecinos.size() >=1)
	{
		posible_mejor = vecinos[seleccionar_vecino(vecinos)].first;
		posible_suma_mejor = vecinos[seleccionar_vecino(vecinos)].second;
		
	}
	// 	// finalizar reloj:
	// clock_t end = clock();
 //    double elapsed_msecs = (double(end - begin) / CLOCKS_PER_SEC) * 1000;
 //    DarTiempo(elapsed_msecs,suma_solucion);
    
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