#include "hgolosaAleatorizada.h"

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

int dameParNoMarcadoRCL(int vertice, int n, float* const *mAdy, vector<bool>& yaAgregados, float alfa)
{
	int pesoMax = mAdy[dameParNoMarcado(vertice, n, mAdy, yaAgregados)][vertice];
	float cercania = (pesoMax * alfa) / 100;

	vector<int> rcl;
	for(int i = 0; i < n; i++)
	{
		if(mAdy[vertice][i] + alfa >= pesoMax && mAdy[vertice][i] != -1)
			rcl.push_back(i);
	}

	int iAleatorio = rand() % rcl.size();

	return iAleatorio;
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
	if(mAdy[v1][v2] < 0)
		return 0;

	return mAdy[v1][v2];
}

float sumaEnConjunto(int v, float* const * mAdy, const vector<int>& conj)
{
	float ret = 0;

	for(int i = 0; i < conj.size(); i++)
	{
		if(peso(v, conj[i], mAdy)< 0)
			cout<< "FALLA ACA 107 -> "<<peso(v, conj[i], mAdy)<<endl;
		ret += peso(v, conj[i], mAdy);
	}

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

pair<float, vector<float> > mejorPesoAlAgregarParDeVerticesEnParticion(int v1, int v2, float* const * mAdy, const vector<vector<int> >& particion)
{
	int dondePonerAv1;
	int dondePonerAv2;
	float pesoMejor = -1;
	vector<float> pesos_Precalculados_De_v2(particion.size(), 0);

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
	pair<float, vector<float> > ret;
	ret.first = pesoMejor;
	ret.second = pesos_Precalculados_De_v2;

	return ret;
}

void meterParDeVerticesEnParticionRCL(int v1, int v2, float* const * mAdy, vector<vector<int> >& particion, float alfa)
{
	pair<float, vector<float> > mejorPesoInfo = mejorPesoAlAgregarParDeVerticesEnParticion(v1, v2, mAdy, particion);
	float cercania = (mejorPesoInfo.first * alfa) / 100;
	vector<pair<int, int> > rcl;

	for(int i = 0; i < particion.size(); i++)
	{
		float nuevoPeso = sumaEnConjunto(v1, mAdy, particion[i]);

		for(int j = 0; j < particion.size(); j++)
		{
			float _peso = nuevoPeso + mejorPesoInfo.second[j];
			if(i == j)
				_peso += peso(v1, v2, mAdy);

			if(_peso - cercania <= mejorPesoInfo.first)
				rcl.push_back(pair<int, int>(i, j));
		}
	}

	int iAleatorio = rand() % rcl.size();

	particion[rcl[iAleatorio].first].push_back(v1);
	particion[rcl[iAleatorio].second].push_back(v2);
}

void meterParDeVerticesEnParticion(int v1, int v2, float* const * mAdy, vector<vector<int> >& particion)
{
	int dondePonerAv1;
	int dondePonerAv2;
	float pesoMejor = -1;
	vector<float> pesos_Precalculados_De_v2(particion.size(), 0);

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

float mejorPesoParaVerticeEnParticion(int v, float* const * mAdy, const vector<vector<int> >& particion)
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

	return pesoMejor;
}

void meterVerticeEnParticionRCL(int v, float* const * mAdy, vector<vector<int> >& particion, float alfa)
{
	int dondePonerAv;
	float pesoMejor = mejorPesoParaVerticeEnParticion(v, mAdy, particion);
	float cercania = (pesoMejor * alfa) / 100;
	vector<int> rcl;

	for(int i = 0; i < particion.size(); i++)
	{
		float nuevoPeso = sumaEnConjunto(v, mAdy, particion[i]);

		if(nuevoPeso - cercania <= pesoMejor)
			rcl.push_back(i);
	}

	int iAleatorio = rand() % rcl.size();

	particion[iAleatorio].push_back(v);
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

float aristaDePesoMaximoValor(int n, float* const * mAdy)
{
	float pesoMax = -1;

	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			if(mAdy[i][j] > pesoMax)
				pesoMax = mAdy[i][j];
		}
	}

	return pesoMax;
}

float aristaDePesoMaximoRCL(int n, float* const * mAdy, int& a, int& b, float alfa)
{
	float pesoMax = aristaDePesoMaximoValor(n, mAdy);
	if(pesoMax == -1)
		return -1;

	float cercania = (pesoMax * alfa) / 100;
	vector<pair<int, int> > rcl;

	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			if(mAdy[i][j] + cercania >= pesoMax && mAdy[i][j] != -1)
				rcl.push_back(pair<int, int>(i, j));
		}
	}

	int iAleatorio = rand() % rcl.size();

	a = rcl[iAleatorio].first;
	b = rcl[iAleatorio].second;

	return mAdy[a][b];
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

void mostrar(float suma_solucion, const vector <vector<int> > &solucion){//SOLO MUESTRA EL RESULTADO
	cout << "suma:" << suma_solucion << endl; 
	for (int t = 0; t < solucion.size(); t++){ 
		for (int j = 0; j < solucion[t].size(); j++){
			cout << t +1 << "_" << solucion[t][j] + 1 << " ";
		}
	}
	cout << endl;
}	

float pesoTotalParticion(const vector<vector<int> >& particion, float *const* mAdy, bool desde1)
{
	float pesoTotal = 0;

	for(int i = 0; i < particion.size(); i++)
	{
		for(int j = 0; j < particion[i].size(); j++)
		{
			for(int k = j + 1; k < particion[i].size(); k++)
			{
				if(desde1)	
				{
					if(mAdy[particion[i][j] - 1][particion[i][k] - 1] != -1)
						pesoTotal += mAdy[particion[i][j] - 1][particion[i][k] - 1];
				}
				else
				{
					if(mAdy[particion[i][j]][particion[i][k]] != -1)
						pesoTotal += mAdy[particion[i][j]][particion[i][k]];
				}
			}
		}
	}

	return pesoTotal;
}

void mostrar(int n, const vector <vector<int> > &solucion, bool desde1)
{
	int correrIndice = desde1 ? 1 : 0;
	
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < solucion.size(); j++)
		{
			for(int k = 0; k < solucion[j].size(); k++)
			{
				if(solucion[j][k] - correrIndice == i)
					cout<<j+1<<" ";
			}
		}
	}

	cout<<endl;
}	

void convertirIndices(vector<vector<int> >& solucion)
{	
	for(int i = 0; i < solucion.size(); i++){
		for(int j = 0; j < solucion[i].size(); j++){
			solucion[i][j]++;
		} 
	}
}

void DarTiempo(double t)
{
	ofstream myfile;
	myfile.open ("tiempos.txt", ios::app);
	myfile<<t<< "\n";
	myfile.close();
}

void heuristica_golosa_aleatorizada(float* const * mAdy, vector<vector<int> >& particion, int n, float alfa, float beta, float gamma)
{
	//clock_t begin = clock();

	list<int> cola;
	float pesoMax = -1;
	int a;
	int b;
	int nodoActual;
	int agregados;
	vector<bool> marcados(n, false);		//aca voy a tener como true a los nodos que ya puse en algun conjunto de la particion
	vector<bool> encolados(n, false);		//aca voy a tener como true a los nodos que estan encolados o alguna vez fueron encolados
	pesoMax = aristaDePesoMaximoRCL(n, mAdy, a, b, alfa);		//tomo la arista cuyo peso es maximo sobre todas las demas, y guardo sus vertices en a y b

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
			int par = dameParNoMarcadoRCL(adyacente, n, mAdy, marcados, beta);
			meterParDeVerticesEnParticionRCL(adyacente, par, mAdy, particion, gamma);
			marcados[par] = true;
			encolarAdyacentesNoAgregados(adyacente, n, mAdy, marcados, cola, encolados);
			agregados += 2;
			nodoActual = par;
		}
		else
		{
			meterVerticeEnParticionRCL(adyacente, mAdy, particion, gamma);
			nodoActual = -1;
			agregados++;
		}

		marcados[adyacente] = true;
	}
}

