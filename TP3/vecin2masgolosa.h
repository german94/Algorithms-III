#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <ctime>
#include <list>

using namespace std;


void reacomodar(int vertices, int aristas, int particiones, vector<vector<int> > &solucion, float **pesos);
float crear_vecino(int indice, vector<pair <float, vector<int> > > &solucion, vector<pair <float, vector<int> > > &nuevo_vecino, float **pesos); 

void mostrar(float suma_solucion, vector <pair<float, vector<int> > > &solucion)
{//SOLO MUESTRA EL RESULTADO
	cout << "suma:" << suma_solucion << endl; 
	for (int t = 0; t < solucion.size(); t++){ 
		for (int j = 0; j < solucion[t].second.size(); j++){
			cout << t +1 << "_" << solucion[t].second[j] << " ";
		}
	}
	cout << endl;
}	

float suma_conjunto(const vector<int> &conjunto_actual, float **pesos)
{
	float suma = 0; //suma todos los vertices de un conjunto determinado
	for(int A= 0; A < conjunto_actual.size(); A++)
	{
		for(int B = A +1; B < conjunto_actual.size() ; B++ )
		{
			if(pesos[conjunto_actual[A] -1][conjunto_actual[B] -1] != -1) {suma = suma + pesos[conjunto_actual[A] -1][conjunto_actual[B] -1];}
		} 
	}
	return suma;
}

vector<pair<float, vector<int> > > obtener_pesos(vector<vector<int> > &presolucion, float **pesos)
{
	vector<pair<float, vector<int> > > res;
	for(int i = 0; i < presolucion.size(); i++)
	{
		pair<float, vector<int> > elemento;
		elemento.second = presolucion[i];
		elemento.first = suma_conjunto(presolucion[i], pesos);
		res.push_back(elemento); 
	}
	return res;
}

void peso_agregado(vector<float> &mas_peso, int a_agregar, float **pesos, vector<pair <float, vector<int> > > &nuevo_vecino)
{
	for(int i = 0; i < nuevo_vecino.size(); i++)
	{
		float res = 0;
		for(int j = 0; j < nuevo_vecino[i].second.size(); j++)
		{
			if(pesos[a_agregar -1][nuevo_vecino[i].second[j] -1] != -1) {res = res + pesos[a_agregar -1][nuevo_vecino[i].second[j] -1];}		
		}
	mas_peso.push_back(res);
	}
}

float sumar_particion(const vector<pair<float, vector<int> > > &solucion )
{
	float res = 0; //suma todo una particion 
	for(int i = 0; i < solucion.size(); i++) { res = res + solucion[i].first;}
	return res;
}

float sumar_adyacentes(int nodo_A, int nodo_B, const vector<int> &conjunto_actual, float **pesos )
{
	float suma = 0; //dados dos vertices adyacentes, le suma los adyacentes a estos
	for(int i =0; i < conjunto_actual.size(); i++)
	{
		if(pesos[nodo_A -1][conjunto_actual[i] -1] != -1) {suma = suma + pesos[nodo_A -1][conjunto_actual[i]-1];}
		if(pesos[nodo_B -1][conjunto_actual[i] -1] != -1) {suma = suma + pesos[nodo_B -1][conjunto_actual[i]-1];}
	}
	return suma - pesos[nodo_A -1][nodo_B-1]; //ya que lo sume dos veces
}

void sacar(pair<float, vector<int> > &conjunto, int separo_A, int separo_B, float **pesos)
{
	vector<int> nuevo; //saca dos vertices determinafo de un conjunto
	for(int i = 0; i < conjunto.second.size(); i++)
	{
		if(conjunto.second[i] != separo_A && conjunto.second[i] != separo_B) {nuevo.push_back(conjunto.second[i]);}
	}
	conjunto.first = suma_conjunto(nuevo, pesos);
	conjunto.second = nuevo;
}



int dameAlgunoQueFalte(const vector<bool>& marcados)
{
	for(int i = 0; i < marcados.size(); i++)
	{
		if(!marcados[i])
			return i;
	}
}

bool faltanAgregar(const vector<bool>& marcados)
{
	for(int i = 0; i < marcados.size(); i++)
	{
		if(!marcados[i])
			return true;
	}

	return false;
}

int dameAdyacenteNoMarcado(int vertice, int n, float* const * mAdy, vector<bool>& yaAgregados)
{
	for(int i = 0; i < n; i++)
	{
		if(mAdy[vertice][i] != -1 && !yaAgregados[i])
			return i;
			
	}
}

int dameParNoMarcado(int vertice, int n, float* const * mAdy, const vector<bool>& yaAgregados)
{
	int pesoMax = -1;
	int ret;

	for(int i = 0; i < n; i++)
	{
		if(mAdy[vertice][i] > pesoMax && !yaAgregados[i])
		{
			ret = i;
			pesoMax = mAdy[vertice][i];
		}			
	}

	return ret;
}

bool tieneAdyacenteNoAgregado(int vertice, int n, float* const * mAdy, const vector<bool>& yaAgregados)
{
	for(int i = 0; i < n; i++)
	{
		if(mAdy[vertice][i] != -1 && !yaAgregados[i])
			return true;
	}

	return false;
}

void encolarAdyacentesNoAgregados(int vertice, int n, float* const * mAdy, const vector<bool>& yaAgregados, list<int>& cola, vector<bool>& yaEncolados)
{
	for(int i = 0; i < n; i++)
	{
		if(mAdy[vertice][i] != -1 && !yaAgregados[i] && !yaEncolados[i])
		{
			cola.push_back(i);
			yaEncolados[i] = true;
		}
	}
}

//necesito esta funcion porque no puedo ponerle peso 0 en la matriz a dos vertices no adyacentes, ya que
//tengo que tener alguna manera de diferenciar cuando dos vertices son (>= 0) o no (-1) adyacentes
float peso(int v1, int v2, float* const * mAdy)
{
	if(mAdy[v1][v2] == -1)
		return 0;

	return mAdy[v1][v2];
}

float sumaEnConjunto(int v, float* const * mAdy, const vector<int>& conj)
{
	float ret = 0;

	for(int i = 0; i < conj.size(); i++)
		ret += peso(v, conj[i], mAdy);

	return ret;
}

float pesoConjunto(const vector<int>& conj, float* const * mAdy)
{
	float ret = 0;

	for(int i = 0; i < conj.size(); i++)
	{
		for(int j = i + 1; j < conj.size(); j++)
			ret += peso(conj[i], conj[j], mAdy);
	}

	return ret;
}

float pesoParticion(const vector<vector<int> >& particion, float* const * mAdy)
{
	float ret = 0;

	for(int i = 0; i < particion.size(); i++)
		ret += pesoConjunto(particion[i], mAdy);

	return ret;
}

void meterParDeVerticesEnParticion(int v1, int v2, float* const * mAdy, vector<vector<int> >& particion)
{
	int dondePonerAv1;
	int dondePonerAv2;
	float pesoMejor = -1;
	vector<int> pesos_Precalculados_De_v2(particion.size(), 0);

	for(int j = 0; j < particion.size(); j++)
		pesos_Precalculados_De_v2[j] = sumaEnConjunto(v2, mAdy, particion[j]);

	for(int i = 0; i < particion.size(); i++)
	{
		float nuevoPeso = sumaEnConjunto(v1, mAdy, particion[i]);

		for(int j = 0; j < particion.size(); j++)
		{
			float _peso = nuevoPeso + pesos_Precalculados_De_v2[j];
			if(i == j)
				_peso += peso(v1, v2, mAdy);

			if(_peso < pesoMejor || pesoMejor == -1)
			{
				pesoMejor = _peso;
				dondePonerAv1 = i;
				dondePonerAv2 = j;
			}
		}
	}

	particion[dondePonerAv1].push_back(v1);
	particion[dondePonerAv2].push_back(v2);
}

void meterVerticeEnParticion(int v, float* const * mAdy, vector<vector<int> >& particion)
{
	int dondePonerAv;
	float pesoMejor = -1;

	for(int i = 0; i < particion.size(); i++)
	{
		float nuevoPeso = sumaEnConjunto(v, mAdy, particion[i]);

		if(nuevoPeso < pesoMejor || pesoMejor == -1)
		{
			pesoMejor = nuevoPeso;
			dondePonerAv = i;
		}
	}

	particion[dondePonerAv].push_back(v);
}

float aristaDePesoMaximo(int n, float* const * mAdy, int& a, int& b)
{
	float pesoMax = -1;

	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			if(mAdy[i][j] > pesoMax)
			{
				pesoMax = mAdy[i][j];
				a = i;
				b = j;
			}
		}
	}

	return pesoMax;
}

void heuristica_golosa(float* const * mAdy, vector<vector<int> >& particion, int n)
{
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
	cout<<a+1<<" "<<b+1<<endl;
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
}