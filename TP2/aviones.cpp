#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <assert.h>
#include <algorithm>
#include <sstream>
#include "aviones.h" 
using namespace std;

std::pair<int, vector<int> > salida(vector<vuelo> vuelos, int inicio, int llegada, vector<vector<int> > donde);

std::pair<int, vector<int> > Buscar_Llegada(vector<vuelo> vuelos, int vuelos_disponibles[], vuelo salida, int posicion_de_salida, int llegada, vector<vector<int> > donde);

int main()
{
	string l;
	std::getline(cin, l);
	int cantInstancias = atoi(l.c_str());
	for(int j = 0; j < cantInstancias; j++)
	{
		vector<string> a_mapear; //va a contener las ciudades de los vuelos
		vector<int> horas; // va a contener los horarios de los vuelos
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

			a_mapear.push_back(lu_salida);
			a_mapear.push_back(lu_llegada);
			horas.push_back(ho_salida);
			horas.push_back(ho_llegada);
		}

		vector<string> mapeados =mapear(a_mapear); //cada ciudad distinta recibe un numero, la numeración va de
		//0 .. n-1 (siendo n la cantidad de ciudades distintas)
		vector<vuelo> vuelos = construir_vuelos(mapeados, a_mapear, horas); //construyo un vector de vuelos
		// cada vuelo es <ori, des, ini, fin> solo que ori y des no son del tipo string sino int segun la funcion mapear 
		std::sort (vuelos.begin(), vuelos.end(), por_llegada);//ordeno por menor costo

		vector<vector<int> > donde = de_donde_salen(vuelos, mapeados.size()); //cada posicion de "donde"
		//representa una ciudad, mapeada a un int, y en cada una esta la posición donde esa ciudad es una ciudad de origen en el vector "vuelos"
		int origen_mapeado, destino_mapeado;
		bool existe = false; //si la ciudad origen no esta en ningun vuelo no hay solución
		for(int i = 0; i < mapeados.size(); i++)
		{
			if(mapeados[i] == origen) {origen_mapeado = i; existe = true;}
			if(mapeados[i] == destino) {destino_mapeado = i;}
		}//busco a la ciudad de ORIGEN y DESTINO de la instancia y devuelvo su equivalente en int
		if(!existe) cout << "no" <<endl;
		else
		{	
			std::pair<int, vector<int> > resul = salida(vuelos, origen_mapeado, destino_mapeado, donde);//funcion principal
			mostrar (resul); // muestro el resultado
	   	}
	}
    return 0;
}

std::pair<int, vector<int> > salida(vector<vuelo> vuelos, int origen, int destino, vector<vector<int> > donde)
{
	int n = vuelos.size();
	int vuelos_disponibles[n];//arreglo donde indico si ya analice ese vuelo y todos aquellos que puedo analizar siguiendo este vuelo 
	for (int i = 0; i < n ; i++ ) { vuelos_disponibles[i] = 0;} //inicializo cada posicion
	for (int i = 0; i < n ; i++ )
	{
		vuelos_disponibles[vuelos[i].lugar_de_salida]++; //indico cuantos vuelos de salida hay hacia esa ciudad;
	}
	vector<int> posiciones = donde[origen];
	if (posiciones.size() == 0)
	{
		std::pair<int, vector<int> > no_hay_solucion;
		no_hay_solucion.first = -1;
		return no_hay_solucion; // no hay vuelos que salgan desde "origen"
	}
	else
	{	
		vector <std::pair<int, vector<int> > > soluciones; // la segunda componente contiene 
		//una lista de vuelos, según la aparición en la entrada, cuya combinación llega a destino, la primera la hora de llegada
		for (int i = 0; i< posiciones.size(); i++)
		{
			std::pair<int, vector<int> > s = Buscar_Llegada(vuelos, vuelos_disponibles, vuelos[posiciones[i]], posiciones[i], destino, donde);
			soluciones.push_back(s);			
		}
		if (No_Hay_Camino(soluciones))
		{
			std::pair<int, vector<int> > no_hay_solucion;
			no_hay_solucion.first = -1;
			return no_hay_solucion; // no hay vuelos que salgan desde "origen"
		}
		else {return minimo_camino(soluciones);}
	}
}			

std::pair<int, vector<int> > Buscar_Llegada(vector<vuelo> vuelos, int vuelos_disponibles[], vuelo salida, int posicion_en_vuelos, int destino, vector< vector<int> > donde)
{
	std::pair <int , vector<int> > itinerario; 
	if (vuelos_disponibles[salida.lugar_de_salida] != 0)//no verifique ese vuelo
	{
		if (salida.lugar_de_llegada == destino)
		{
			vector<int> camino; //acá voy a tener la lista con todos los vuelos que me ayudan 
			//a llegar a destino
			camino.push_back(posicion_en_vuelos); 
			itinerario.first = salida.hora_llegada; //la hora que llego a destino
			itinerario.second = camino; // los vuelos para llegar
			vuelos_disponibles[salida.lugar_de_salida]--; //si otro vuelo hace uso de este vuelo ya tengo la información que necesito y no vuelvo a calcularl
		}
		else 
		{
			vector<int> posiciones = donde[salida.lugar_de_llegada];
		
			vector <std::pair<int, vector<int> > > soluciones;
			for(int i =0; i< posiciones.size(); i++)//analizo todos los vuelos que salen desde esta ciudad
			{
				if(vuelos[posiciones[i]].hora_salida - salida.hora_llegada >= 2)
				{
					std::pair<int, vector<int> >  s = Buscar_Llegada(vuelos, vuelos_disponibles, vuelos[posiciones[i]], posiciones[i], destino, donde);
					soluciones.push_back(s);
				}
					vuelos_disponibles[salida.lugar_de_salida]--;
					
			}
			if (No_Hay_Camino(soluciones)) {itinerario.first = -1;}
			else
			{
				itinerario =  minimo_camino(soluciones);//hay minimo una combinacion de vuelos para llegar  a destino elijo el que llega antes
				itinerario.second.push_back(posicion_en_vuelos); //agrego este vuelo ya que me sirve para llefar a destino
			}	
		}
	}
	else {itinerario.first = -1;}//si ya analice este vuelo y sus sgtes no vuelo a analizar
	
	return itinerario;
}