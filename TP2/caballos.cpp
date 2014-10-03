#include <iostream>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <ctime>
#include <list>
#include <algorithm>

using namespace std;

void DarTiempo(double t)
{
	ofstream myfile;
	myfile.open ("tiemposCaballos.txt", ios::app);
	myfile <<t << "\n";
	myfile.close();
}

bool esValida(pair<int, int> p, vector<vector<int> >& M)
{
	return ((p.first >= 0 && p.first <= M.size() - 1) && (p.second >= 0 && p.second <= M.size() - 1)) && (M[p.first][p.second] == -1);
}

void PosicionesASaltar(pair<int, int> p, vector<vector<int> >& M, list<pair<int, int> >& aSaltar)
{
	if(esValida(pair<int, int>(p.first - 2, p.second - 1), M))
		aSaltar.push_back(pair<int, int>(p.first - 2, p.second - 1));

	if(esValida(pair<int, int>(p.first - 1, p.second - 2), M))
		aSaltar.push_back(pair<int, int>(p.first - 1, p.second - 2));

	if(esValida(pair<int, int>(p.first - 2, p.second + 1), M))
		aSaltar.push_back(pair<int, int>(p.first - 2, p.second + 1));

	if(esValida(pair<int, int>(p.first - 1, p.second + 2), M))
		aSaltar.push_back(pair<int, int>(p.first - 1, p.second + 2));

	if(esValida(pair<int, int>(p.first + 2, p.second - 1), M))
		aSaltar.push_back(pair<int, int>(p.first + 2, p.second - 1));

	if(esValida(pair<int, int>(p.first + 1, p.second - 2), M))
		aSaltar.push_back(pair<int, int>(p.first + 1, p.second - 2));

	if(esValida(pair<int, int>(p.first + 2, p.second + 1), M))
		aSaltar.push_back(pair<int, int>(p.first + 2, p.second + 1));

	if(esValida(pair<int, int>(p.first + 1, p.second + 2), M))
		aSaltar.push_back(pair<int, int>(p.first + 1, p.second + 2));
}

void FiltrarMarcadas(vector<vector<int> > M, list<pair<int, int> > l, list<pair<int, int> >& lNoMarcadas)
{
	for (std::list<pair<int,int> >::iterator it=l.begin(); it != l.end(); ++it)
	{
		if(M[(*it).first][(*it).second] == -1)
			lNoMarcadas.push_back((*it));
	}
}

void LlenarTablero(vector<vector<int> >& M, int n, pair<int, int> pos)
{ 
	int c = 0;
	list<pair<int, int> > listaActual;
	listaActual.push_back(pos);

	while(!listaActual.empty())
	{
		list<pair<int, int> > listaNueva;
		
		for (std::list<pair<int,int> >::iterator it=listaActual.begin(); it != listaActual.end(); ++it)
		{
			list<pair<int, int> > l;
			PosicionesASaltar((*it), M, l);
			for (std::list<pair<int,int> >::iterator it2=l.begin(); it2!= l.end(); ++it2)
			{
				M[(*it2).first][(*it2).second] = c + 1;
				listaNueva.push_back((*it2));
			}
		}

		listaActual = listaNueva;
			
		c++;
	}	
}

void ResolverTablero(vector<vector<vector<int> > > tableros)
{
	int length = tableros.size();
	int n = tableros[0].size();
	int movsMinimos = -1;
	pair<int, int> posMinima;
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			int movs = 0;
			for(int c = 0; c < length; c++)
			{
				if(tableros[c][i][j] == -1)
				{
					movs = -1;
					break;
				}
				movs += tableros[c][i][j];
			}
			if((movs < movsMinimos || movsMinimos == -1) && movs != -1)
			{
				movsMinimos = movs;
				posMinima = pair<int, int>(i, j);

			}
		}
	}
	if(movsMinimos == -1)
		cout<<"no"<<endl;
	else
		cout<<posMinima.first + 1<<" "<<posMinima.second + 1<<" "<<movsMinimos<<endl;
}


int main()
{ 	
	string l;
	vector<vector<vector<int> > > tableros;
	vector<pair<int, int> > caballos;
	std::getline(cin, l);
	istringstream ss(l);
	string token;
	std::getline(ss, token, ' ');
	int k, n;
	n = atoi(token.c_str());
	std::getline(ss, token, ' ');
	k = atoi(token.c_str());
	tableros.resize(k);
	caballos.resize(k);
	int i = k;
	if(k == 0 || n == 0)
		cout<< "no"<<endl;
	else
	{

		while(i > 0)
		{
			std::getline(cin, l);
			istringstream sse(l);
			int x, y;
			std::getline(sse, token, ' ');
			x = atoi(token.c_str()) - 1;
			std::getline(sse, token, ' ');
			y = atoi(token.c_str()) - 1;
			caballos[k - i] = pair<int, int>(x, y);
			i--;
		}

		//clock_t begin = clock();

		for(int c = 0; c < k; c++)
		{
			vector<vector<int> > tablero;
			tablero.resize(n);
			for(int z=0; z<n; z++)
				tablero[z] = vector<int>(n, -1);

			tablero[caballos[c].first][caballos[c].second] = 0;
			LlenarTablero(tablero, n, caballos[c]);

			tableros[c] = tablero;
		}
		
		ResolverTablero(tableros);

	// clock_t end = clock();
//       double elapsed_msecs = (double(end - begin) / CLOCKS_PER_SEC) * 1000;
//       DarTiempo(elapsed_msecs);
	}


	return 0;
}