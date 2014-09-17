#include <iostream>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <ctime>
#include <list>

using namespace std;

bool esValida(pair<int, int> p, int n)
{
	return ((p.first >= 0 && p.first <= n - 1) && (p.second >= 0 && p.second <= n - 1));
}

vector<pair<int, int> > PosicionesASaltar(pair<int, int> p, int n)
{
	vector<pair<int, int> > aSaltar;

	if(esValida(pair<int, int>(p.first - 2, p.second - 1), n))
		aSaltar.push_back(pair<int, int>(p.first - 2, p.second - 1));

	if(esValida(pair<int, int>(p.first - 1, p.second - 2), n))
		aSaltar.push_back(pair<int, int>(p.first - 1, p.second - 2));

	if(esValida(pair<int, int>(p.first - 2, p.second + 1), n))
		aSaltar.push_back(pair<int, int>(p.first - 2, p.second + 1));

	if(esValida(pair<int, int>(p.first - 1, p.second + 2), n))
		aSaltar.push_back(pair<int, int>(p.first - 1, p.second + 2));

	if(esValida(pair<int, int>(p.first + 2, p.second - 1), n))
		aSaltar.push_back(pair<int, int>(p.first + 2, p.second - 1));

	if(esValida(pair<int, int>(p.first + 1, p.second - 2), n))
		aSaltar.push_back(pair<int, int>(p.first + 1, p.second - 2));

	if(esValida(pair<int, int>(p.first + 2, p.second + 1), n))
		aSaltar.push_back(pair<int, int>(p.first + 2, p.second + 1));

	if(esValida(pair<int, int>(p.first + 1, p.second + 2), n))
		aSaltar.push_back(pair<int, int>(p.first + 1, p.second + 2));


	return aSaltar;
}

vector<pair<int, int> > FiltrarMarcadas(vector<vector<int> > M, vector<pair<int, int> > l)
{
	vector<pair<int, int> > lNoMarcadas;
	for(int i=0; i<l.size(); i++)
	{
		if(M[l[i].first][l[i].second] == -1)
			lNoMarcadas.push_back(l[i]);
	}

	return lNoMarcadas;
}

void LlenarTablero(vector<vector<int> >& M, int n, pair<int, int> pos)
{
	int c = 0;
	vector<pair<int, int> > listaActual;
	listaActual.push_back(pos);

	while(listaActual.size() > 0)
	{
		vector<pair<int, int> > listaNueva;
		for(int j = 0; j < listaActual.size(); j++)
		{
			vector<pair<int, int> > l = PosicionesASaltar(listaActual[j], n);
			l = FiltrarMarcadas(M, l);
			listaNueva.insert(listaNueva.end(), l.begin(), l.end());
		}

		for(int i = 0; i < listaNueva.size(); i++)
			M[listaNueva[i].first][listaNueva[i].second] = c + 1;


		listaActual = listaNueva;
			
		c++;

	}
}

void ResolverTablero(vector<vector<vector<int> > > tableros, vector<pair<int, int> > caballos)
{
	int length = caballos.size();
	int n = tableros[0].size();
	int movsMinimos = -1;
	pair<int, int> posMinima;
	for(int i = 0; i < n; i++)
	{
		for(int j = 0; j < n; j++)
		{
			int movs = 0;
			for(int c = 0; c < length; c++)
				movs += tableros[c][i][j];
			if(movs < movsMinimos || movsMinimos == -1)
			{
				movsMinimos = movs;
				posMinima = pair<int, int>(i, j);

			}
		}
	}
	cout<<posMinima.first<<" "<<posMinima.second<<" "<<movsMinimos<<endl;
}


int main()
{
	vector<vector<vector<int> > > tableros;
	vector<pair<int, int> > caballos;
	string l;
	std::getline(cin, l);
	istringstream ss(l);
	string token;
	std::getline(ss, token, ' ');
	int k, n;
	n = atoi(token.c_str());
	std::getline(ss, token, ' ');
	k = atoi(token.c_str());
	int i = k;

	while(i > 0)
	{
		std::getline(cin, l);
		istringstream sse(l);
		int x, y;
		std::getline(sse, token, ' ');
		x = atoi(token.c_str());
		std::getline(sse, token, ' ');
		y = atoi(token.c_str());
		caballos.push_back(pair<int, int>(x, y));
		vector<vector<int> > tablero;
		for(int z=0; z<n; z++)
		{
			vector<int> f;
			for(int h=0; h<n; h++)
				f.push_back(-1);
			tablero.push_back(f);
		}
		tablero[x][y] = 0;
		LlenarTablero(tablero, n, pair<int, int>(x, y));
		tableros.push_back(tablero);

		i--;
	}

	ResolverTablero(tableros, caballos);


	return 0;
}