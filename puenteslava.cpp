#include <iostream>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <ctime>

using namespace std;

void DarTiempo(int n, double t)
{
	ofstream myfile;
	myfile.open ("tiempos.txt", ios::app);
	myfile << n << " " <<t << ".\n";
	myfile.close();
}

void resolverInstancia(vector<int> p, int c)
{
	/*clock_t begin = clock();
	int length = p.size();

	int rotosSeguidos = 0;
	for(int i=0; i<length; i++)
	{
		if(p[i] == 1)
		{
			rotosSeguidos++;
			if(rotosSeguidos > c)
			{
				cout<<"no";

				clock_t end = clock();
  				double elapsed_secs = double(end - begin);
				DarTiempo(length, elapsed_secs);

				return;
			}
		}
		else
			rotosSeguidos = 0;
	}

	vector<int> s;
	int i = c + 1;
	while(i - 1 < length)
	{
		if(p[i - 1] == 0)
		{
			s.push_back(i);
			i = i + c + 1;
		}
		else
		{
			int j = i - 1;
			while(p[j] == 1)
				j--;

			i = j + 1;
		}
	}
*/


	clock_t begin = clock();
	int length = p.size();
    int ultimosano = -1;
    int saltados = 0;
    vector<int> s;
    for(int i=0; i<p.size(); i++)
    {
    	saltados++;
    	if(p[i] == 0)
    		ultimosano = i;

    	if(saltados == c + 1)	//si pase todos los tablones que puedo llegar a saltar desde el ultimo tablon sano en donde estaba parado:
    	{
    		//si la distancia del tablon donde estoy ahora al ultimo tablon sano (no necesariamente donde estaba parado antes ya que lo voy pisando) es mayor que lo que puedo
    		//llegar a saltar, entonces quiere decir que no hay ningun tablon sano en el medio, o sea que tengo mas de c tablones rotos
    		if(i - ultimosano > c - 1 || ultimosano == -1)
    		{
    			cout<<"no";
    			return;
    		}

    		//si eso de arriba no pasa, entonces el tablon a donde voy a saltar va a ser el ultimo tablon sano
    		s.push_back(ultimosano + 1);
    		//y ahora tengo que repetir el proceso, pero no puedo saltar c tablones a lo sumo desde la posicion i actual, ya que no necesariamente estoy parado en la posicion i,
    		//puedo estar parado mucho antes, en tal caso voy a poder saltar menos de c tablones desde donde estoy, porque significa que los tablones entre el tablon donde estoy
    		//parado y el tablon i estan todos rotos, entonces a los saltados hasta ahora, le tengo que restar los saltados anteriores a donde estoy parado que es el ultimo tablon
    		//sano por donde pase
    		saltados -= ultimosano + 1;
    		if(saltados < 0)
    			saltados = 0;
    	}   	
    }

    //como los tablones de la solucion se agregan cuando llegue al tablon c + 1 a partir de donde estaba parado, podria pasar que caiga en un tablon sano y termine de recorrer el
    //puente antes de que saltados llegue a c + 1, entonces en ese caso, el tablon donde estaba parado antes de pasar al final del puente nunca se agregaria.
    //La manera de chequear esto es ver si desde el ultimo tablon que agregue puedo pasar al final del puente, si puedo quiere decir que lo agregue, si no quiere decir que me falta
    //agregar el ultimo sano.
    if(p.size() - s[s.size() - 1] > c)
    	s.push_back(ultimosano + 1);
    s.push_back(p.size() + 1);

    clock_t end = clock();
    double elapsed_secs = (double(end - begin) / CLOCKS_PER_SEC)*1000000;
    DarTiempo(length, elapsed_secs);

    vector<int>::const_iterator ci;
    for(ci=s.begin(); ci!=s.end(); ci++)
   	    cout << *ci << " ";
}

int main()
{
	vector<vector<int> > v;
	string l;
	getline(cin, l);
	while(l.at(0) != '0')
	{
		vector<int> vec;
		istringstream ss(l);
		string token;
		while(std::getline(ss, token, ' ')) 
		    vec.push_back(atoi(token.c_str()));

		v.push_back(vec);
		getline(cin, l);
	}

	cout<<"___________________________\n"<<"Soluciones: "<<endl;
	for(int i=0; i<v.size(); i++)
	{
		int c = v[i][1];
		v[i].erase(v[i].begin(), v[i].begin()+2);
		resolverInstancia(v[i], c);
		cout<<endl;
	}
}