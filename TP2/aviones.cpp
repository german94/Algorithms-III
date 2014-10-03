#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include "aviones.h" 
#include <list>
#include <ctime>
#include <sstream>
#include <fstream>

using namespace std;

list<int> desde_origen(vector<string> ciudades, vector<int> horarios, string desde, string hasta);

std::pair<int, list<int> > Buscar_Llegada(vector<vuelo>& vuelos, int vuelos_disponibles[], vuelo vuelo_actual, int posicion_de_salida, int llegada, vector<list<int> >& donde, bool ya_lo_use[]);

void DarTiempo(double t)
{
	ofstream myfile;
	myfile.open ("tiemposmejor.txt", ios::app);
	myfile <<t << "\n";
	myfile.close();
}

int main()
{
	/*vector<string> ciudades; //va a contener las ciudades de los vuelos
	vector<int> horarios; // va a contener los horarios de los vuelos
	int cantidad; //CANTIDAD DE VUELOS DISPONIBLES
	string origen; // CIUDAD ORIGEN DE LA INSTANCIA
	string destino;// CIUDAD DESTINO DE LA INSTANCIA
	cin >> origen >> destino >> cantidad;

	ciudades.resize(cantidad*2);
	horarios.resize(cantidad*2);

	for(int i = 0; i < cantidad*2; i= i+2)
	{
		string lu_salida;
		cin >> lu_salida;
		ciudades[i] = lu_salida; //voy armando un vector con la ciudad origen y destino de cada vuelo
		string lu_llegada;
		cin >> lu_llegada;
		ciudades[i+1] = lu_llegada; // armo otro vector con la hora de salida y llegada de cada vuelo
		int ho_salida;
		cin >> ho_salida;
		horarios[i] = ho_salida;
		int ho_llegada;
		cin >> ho_llegada;
		horarios[i+1] = ho_llegada;
	}*/

	string l;
	std::getline(cin, l);
	int cantInstancias = atoi(l.c_str());
	for(int j = 0; j < cantInstancias; j++)
	{
		vector<string> ciudades; //va a contener las ciudades de los vuelos
		vector<int> horarios; // va a contener los horarios de los vuelos
		int cantidad; //CANTIDAD DE VUELOS DISPONIBLES
		string origen; // CIUDAD ORIGEN DE LA INSTANCIA
		string destino;// CIUDAD DESTINO DE LA INSTANCIA
		std::getline(cin, l);
		istringstream ss(l);
		std::getline(ss, origen, ' ');
		std::getline(ss, destino, ' ');
		std::getline(ss, l, ' ');
		cantidad = atoi(l.c_str());

		for(int i = 0; i < cantidad; i++)
		{
			std::getline(cin, l);
			istringstream sse(l);
			string lu_salida;
			std::getline(sse, lu_salida, ' ');
			string lu_llegada;
			std::getline(sse, lu_llegada, ' ');
			string c;
			std::getline(sse, c, ' ');
			int ho_salida = atoi(c.c_str());
			std::getline(sse, c, ' ');
			int ho_llegada = atoi(c.c_str());

			ciudades.push_back(lu_salida);
			ciudades.push_back(lu_llegada);
			horarios.push_back(ho_salida);
			horarios.push_back(ho_llegada);
		}


		clock_t begin = clock();

		list<int> resul = desde_origen(ciudades, horarios, origen, destino);//funcion principal recibe los paises, los horarios y desde donde hasta donde quiero ir
		
		clock_t end = clock();
    	double elapsed_msecs = (double(end - begin) / CLOCKS_PER_SEC) *1000;
        DarTiempo(elapsed_msecs);

		mostrar (resul); // muestro el resultado
   	}
    return 0;
}

list<int> desde_origen(vector<string> ciudades, vector<int> horarios, string desde, string hasta){

	list<string> mapeados = mapear(ciudades); //cada ciudad distinta recibe un numero, la numeración va de
	//0 .. n-1 (siendo n la cantidad de ciudades distintas)
	vector<vuelo> vuelos_sin_ord = construir_vuelos(mapeados, ciudades, horarios); //construyo un vector de vuelos
	// cada vuelo es <ori, des, ini, fin> solo que ori y des no son del tipo string sino int segun la funcion mapear 
	int origen, destino;
	bool existe_origen = false; //si la ciudad origen no esta en ningun vuelo no hay solución
	bool existe_destino = false;
	list<int> resultado;  
	int i = 0;
	for(std::list<string>::iterator it = mapeados.begin(); it != mapeados.end(); ++it, i++)
	{
		if(*it == desde) {origen = i; existe_origen = true;}
		if(*it == hasta) {destino = i; existe_destino = true;}
	}//busco a la ciudad de ORIGEN y DESTINO de la instancia y devuelvo su equivalente en int
	if(!existe_origen || !existe_destino) {return resultado;} // lo devuelvo vacio
	else
	{	
		vector<vuelo> vuelos = vuelos_sin_ord;
		std::sort (vuelos.begin(), vuelos.end(), por_llegada);//ordeno por llegada
		vector<list<int> > donde = de_donde_salen(vuelos, mapeados.size()); //cada posicion de "donde"
		bool ya_lo_use[vuelos.size()];
		for (int i = 0; i < vuelos.size(); i++) {ya_lo_use[i] = false;} //me va a decir si ya analice ese vuelo
		//representa una ciudad, mapeada a un int, y en cada una esta la posición donde esa ciudad es una ciudad de origen en el vector "vuelos"
		int n = donde.size(); //ya que donde es del tamaño de la cantidad de ciudades distintas. Podria usar mapeados pero es una lista y size es lineal
		int vuelos_disponibles[n];//arreglo donde indico si ya analice ese vuelo y todos aquellos que puedo analizar siguiendo este vuelo 
		for (int i = 0; i < n ; i++ ) { vuelos_disponibles[i] = 0;} //inicializo cada posicion
		for (int i = 0; i < n ; i++ )
		{
			vuelos_disponibles[vuelos[i].lugar_de_salida]++; //indico cuantos vuelos de salida hay hacia esa ciudad;
		}
		
		list<int> posiciones = donde[origen];
		if (posiciones.empty())
		{
			return resultado; // no hay vuelos que salgan desde "origen"
		}
		else
		{	
			list <std::pair<int, list<int> > > soluciones; // la segunda componente contiene 
			//una lista de vuelos, según la aparición en la entrada, cuya combinación llega a destino, la primera la hora de llegada
			for(std::list<int>::iterator it = posiciones.begin(); it != posiciones.end(); ++it)
			{
				std::pair<int, list<int> > s = Buscar_Llegada(vuelos, vuelos_disponibles, vuelos[*it], *it, destino, donde, ya_lo_use);
				soluciones.push_back(s);		
			}
			if (No_Hay_Camino(soluciones)) { return resultado;}
			else 
			{
				std::pair<int, list<int> > camino_minimo =  minimo_camino(soluciones);
				return buscar_pos(camino_minimo, vuelos_sin_ord, vuelos); 
			}
		}
	}	
}			

std::pair<int, list<int> > Buscar_Llegada(vector<vuelo>& vuelos, int vuelos_disponibles[], vuelo vuelo_actual, int posicion_en_vuelos, int destino, vector< list<int> >& donde, bool ya_lo_use[]){
		
	std::pair <int , list<int> > itinerario; 
	if (vuelo_actual.lugar_de_llegada == destino)
	{
		list<int> camino; //acá voy a tener la lista con todos los vuelos que me ayudan 
		//a llegar a destino
		camino.push_back(posicion_en_vuelos); 
		itinerario.first = vuelo_actual.hora_llegada; //la hora que llego a destino
		itinerario.second = camino; // los vuelos para llegar
	}
	else 
	{ 
		if (vuelos_disponibles[vuelo_actual.lugar_de_llegada] != 0)
		{		
			list<int> posiciones = donde[vuelo_actual.lugar_de_llegada];
		
			list <std::pair<int, list<int> > > soluciones;
			for(std::list<int>::iterator it = posiciones.begin(); it != posiciones.end(); ++it) //analizo todos los vuelos que salen desde esta ciudad
			{
				if(vuelos[*it].hora_salida - vuelo_actual.hora_llegada >= 2 && !ya_lo_use[*it])
				{
					std::pair<int, list<int> >  s = Buscar_Llegada(vuelos, vuelos_disponibles, vuelos[*it], *it, destino, donde, ya_lo_use);
					soluciones.push_back(s);
					vuelos_disponibles[vuelo_actual.lugar_de_salida]--;
					ya_lo_use[*it] = true;
				}					
			}
			if (No_Hay_Camino(soluciones)) {itinerario.first = -1;}
			else
			{
				itinerario =  minimo_camino(soluciones);//hay minimo una combinacion de vuelos para llegar  a destino elijo el que llega antes
				itinerario.second.push_back(posicion_en_vuelos); //agrego este vuelo ya que me sirve para llefar a destino
			}	
		}
		else { itinerario.first = -1;}
	}
	return itinerario;
}
