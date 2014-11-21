#include "vecindad2.h" 

float suma_conjunto(const vector<int> &conjunto_actual, float *const*pesos)
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

vector<pair<float, vector<int> > > obtener_pesos(vector<vector<int> > &presolucion, float *const*pesos)
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

void peso_agregado(vector<float> &mas_peso, int a_agregar, float *const*pesos, vector<pair <float, vector<int> > > &nuevo_vecino)
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

float sumar_adyacentes(int nodo_A, int nodo_B, const vector<int> &conjunto_actual, float *const*pesos )
{
	float suma = 0; //dados dos vertices adyacentes, le suma los adyacentes a estos
	for(int i =0; i < conjunto_actual.size(); i++)
	{
		if(pesos[nodo_A -1][conjunto_actual[i] -1] != -1) {suma = suma + pesos[nodo_A -1][conjunto_actual[i]-1];}
		if(pesos[nodo_B -1][conjunto_actual[i] -1] != -1) {suma = suma + pesos[nodo_B -1][conjunto_actual[i]-1];}
	}
	return suma - pesos[nodo_A -1][nodo_B-1]; //ya que lo sume dos veces
}

void sacar(pair<float, vector<int> > &conjunto, int separo_A, int separo_B, float *const*pesos)
{
	vector<int> nuevo; //saca dos vertices determinafo de un conjunto
	for(int i = 0; i < conjunto.second.size(); i++)
	{
		if(conjunto.second[i] != separo_A && conjunto.second[i] != separo_B) {nuevo.push_back(conjunto.second[i]);}
	}
	conjunto.first = suma_conjunto(nuevo, pesos);
	conjunto.second = nuevo;
}


void vecindad2(int vertices, int aristas, int particiones, vector<vector<int> > &presolucion, float *const*pesos)
{		
	vector<pair<float, vector<int> > > solucion = obtener_pesos(presolucion, pesos);
	float suma_solucion = sumar_particion(solucion); //inicialmente la suma solucion es la de la presolucion

	if(solucion.size() != particiones) //si la presolucion tiene menos de k conjuntos le agrego los restantes
	{
		for(int i = solucion.size(); i < particiones; i++)
		{
			float valor_inicial = 0;
			vector<int> vector_inicial;
			pair<float, vector<int> > rellenar;
			rellenar.first = valor_inicial;
			rellenar.second = vector_inicial;
			solucion.push_back(rellenar);
		}
	}

	float posible_solucion = -1; 
	float peso_vecino;
	vector<pair<float, vector<int> > > vecino_solucion;
	do 
	{ 
		for(int i = 0; i < solucion.size(); i ++) //modificadas 
		{
			if(solucion[i].second.size() > 1) 
			{
				vector<pair<float, vector<int> > > nuevo_vecino;
				peso_vecino = crear_vecino(i, solucion, nuevo_vecino, pesos);
				if(posible_solucion ==-1 || posible_solucion > peso_vecino)
				{
					posible_solucion = peso_vecino;
					vecino_solucion = nuevo_vecino;
				}
			} 
		}// va ir obteniendo la "c.c" mas pesada de cada conjunto que haya
		if(posible_solucion != -1)
		{	
			if (posible_solucion < suma_solucion) // si el vecino mas optimo es mejor que la solucion que tenia lo reemplazo 
			{
				suma_solucion = posible_solucion;
				posible_solucion = -1; //decremente para seguir iterando ya que ahora posible solucion == suma_solucion
				solucion = vecino_solucion;
			}
		} else {posible_solucion = suma_solucion;}	
	} while (posible_solucion < suma_solucion); // no me sirve poner <= porque podria no terminar, por eso antes decremente a psibles_solucion

	vector<vector<int> > restauro;
	for(int i = 0; i < solucion.size() ; i++){
		vector<int> a = solucion[i].second;
		restauro.push_back(a);
	}
	presolucion = restauro;
}

float crear_vecino(int indice, vector<pair <float, vector<int> > > &solucion, vector<pair <float, vector<int> > > &nuevo_vecino, float *const*pesos) 
{
	float mayor_peso = -1;//tengo a la cosa mas pesada
	int nodo_a_separarA, nodo_a_separarB;
	int separo_A, separo_B;
	for(int nodo_A = 0; nodo_A < solucion[indice].second.size() -1; nodo_A++)
	{		//ambos while sirven para agarrar todo PAR de nodo
		for(int nodo_B = nodo_A +1; nodo_B < solucion[indice].second.size(); nodo_B++)
		{	
			nodo_a_separarA = solucion[indice].second[nodo_A];
			nodo_a_separarB = solucion[indice].second[nodo_B];

			if(pesos[nodo_a_separarA -1][nodo_a_separarB -1] != -1) //Si ese par es adyacente veo cuanto suma con sus adyancentes
			{
				float peso = sumar_adyacentes(nodo_a_separarA, nodo_a_separarB, solucion[indice].second, pesos);
				if(peso > mayor_peso || mayor_peso == -1)
				{
					mayor_peso = peso; // como lo voy a hacer con todos los pares me guardo la suma de adyacencia que mas sumo. y me guardo los nodos que tendria que separar
					separo_A = nodo_a_separarA;
					separo_B = nodo_a_separarB;
				}
			}
		}
	} //tengo a los nodos que quiero separar y el peso
	//En esta parte voy a ubicar a los nodos donde mas me convenga
	float suma_minima = solucion[indice].first; //va a ser la suma del conjunto luego de 
	nuevo_vecino = solucion;

	if (mayor_peso != -1) // si antes no encontre nodos adyacentes entonces mayor_peso quedo en -1, no tiene sentido seguir iterando
	{
		vector<float> mas_peso_de_A; //este va a ser el vecino con las modificaciones
		vector<float> mas_peso_de_B;

		sacar(nuevo_vecino[indice], separo_A, separo_B, pesos);
		
		peso_agregado(mas_peso_de_A, separo_A, pesos, nuevo_vecino);
		peso_agregado(mas_peso_de_B, separo_B, pesos, nuevo_vecino);

		int ubicar_A = indice;
		int ubicar_B = indice;
		for(int i = 0; i < nuevo_vecino.size() && suma_minima!= 0; i++ )
		{
			for(int j = 0; j < nuevo_vecino.size() && suma_minima !=0; j++ )
			{//ambos for me sirven para ver todas las combinaciones de donde meter a los nodos mas pesados
				int a = mas_peso_de_A[i] + mas_peso_de_B[j];
				if (i == j) {a = a + pesos[separo_A -1][separo_B -1];}

				if(suma_minima > a)
				{
					suma_minima = a;
					ubicar_A = i;
					ubicar_B = j;
				}
			}
		}
		nuevo_vecino[ubicar_A].second.push_back(separo_A); //los agrego donde me conviene
		nuevo_vecino[ubicar_A].first = nuevo_vecino[ubicar_A].first + mas_peso_de_A[ubicar_A]; 
		nuevo_vecino[ubicar_B].second.push_back(separo_B);
		nuevo_vecino[ubicar_B].first = nuevo_vecino[ubicar_B].first + mas_peso_de_B[ubicar_B]; 
		if(ubicar_A == ubicar_B) {nuevo_vecino[ubicar_A].first += pesos[separo_A -1][separo_B -1];}
	}
	return sumar_particion(nuevo_vecino); //inicialmente la suma solucion es la de la presolucion
}
