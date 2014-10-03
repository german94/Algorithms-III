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