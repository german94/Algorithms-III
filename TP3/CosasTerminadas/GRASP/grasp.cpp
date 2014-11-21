#include "hgolosaAleatorizada.h"
#include "vecindad1.h"
#include "vecindad2.h"

void grasp(vector<vector<int> >& particion, float *const* mAdy, bool usarVecindad2, int cuantasParaTerminar, int n, int m, float alfa, float beta, float gamma)
{
	clock_t begin = clock();
	float mejorPeso = -1;
	int repeticionesSeguidasSinMejorar = 0;
	vector<vector<int> > mejorParticion;

	while(repeticionesSeguidasSinMejorar != cuantasParaTerminar)
	{
		vector<vector<int> > iParticion = vector<vector<int> >(particion.size(), vector<int>());

		heuristica_golosa_aleatorizada(mAdy, iParticion, n, alfa, beta, gamma);

		float peso = pesoTotalParticion(iParticion, mAdy, false);

		if(usarVecindad2)
		{
			convertirIndices(iParticion);
			vecindad2(iParticion.size(), iParticion, mAdy);

		}
		else
			vecindad1(mAdy, iParticion, pesoTotalParticion(iParticion, mAdy, false)); 

		peso = pesoTotalParticion(iParticion, mAdy, usarVecindad2);
		if(peso >= mejorPeso && mejorPeso != -1)
			repeticionesSeguidasSinMejorar++;
		else
		{
			repeticionesSeguidasSinMejorar = 0;
			mejorPeso = peso;
			mejorParticion = iParticion;
		}

	}

	particion = mejorParticion;
	clock_t end = clock();
    double elapsed_msecs = (double(end - begin) / CLOCKS_PER_SEC) * 1000;
    DarTiempo(elapsed_msecs, mejorPeso);
	mostrar(n, particion, usarVecindad2);
}

int main()
{
	bool usarVecindad2 = false;
	int cuantasParaTerminar = 10;
	float alfa = 20;
	float beta = 20;
	float gamma = 20;
	int ta = 1;
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
		for(int i = 0; i < vertices; i++)
		{
			for(int j = 0; j < vertices; j++)
				pesos[i][j] = -1;
		}
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
			grasp(particion, pesos, usarVecindad2, cuantasParaTerminar, vertices, aristas, alfa, beta, gamma);
			cout<<"termino la instancia "<<ta<<endl;
			ta++;

			for(int i = 0; i < vertices; i++) {free (pesos[i]);} //LIBERO MEMORIA
				free (pesos);
			std::getline(cin, l);

	}
    return 0;
}
