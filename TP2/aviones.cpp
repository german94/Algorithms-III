#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <assert.h>
#include <algorithm>
#include "aviones.h" 

using namespace std;

std::pair<int, vector<int> > salida(vector<vuelo> vuelos, int inicio, int llegada, vector<vector<int> > donde);

std::pair<int, vector<int> > Buscar_Llegada(vector<vuelo> vuelos, bool verifico[], vuelo salida, int posicion_de_salida, int llegada, vector<vector<int> > donde);

int main()
{
	vector<string> a_mapear; //va a contener las ciudades de los vuelos
	vector<int> horas; // va a contener los horarios de los vuelos
	int cantidad; //CANTIDAD DE VUELOS DISPONIBLES
	string origen; // CIUDAD ORIGEN DE LA INSTANCIA
	string destino;// CIUDAD DESTINO DE LA INSTANCIA
	cin >> origen >> destino >> cantidad;

	for(int i = 0; i < cantidad; i++)
	{
		string lu_salida;
		cin >> lu_salida;
		a_mapear.push_back(lu_salida); //voy armando un vector con la ciudad origen y destino de cada vuelo
		string lu_llegada;
		cin >> lu_llegada;
		a_mapear.push_back(lu_llegada); // armo otro vector con la hora de salida y llegada de cada vuelo
		int ho_salida;
		cin >> ho_salida;
		horas.push_back(ho_salida);
		int ho_llegada;
		cin >> ho_llegada;
		horas.push_back(ho_llegada);
	}

	vector<string> mapeados =mapear(a_mapear); //cada ciudad distinta recibe un numero, la numeración va de
	//0 .. n-1 (siendo n la cantidad de ciudades distintas)
	vector<vuelo> vuelos = construir_vuelos(mapeados, a_mapear, horas); //construyo un vector de vuelos
	// cada vuelo es <ori, des, ini, fin> solo que ori y des no son del tipo string sino int segun la funcion mapear 
	vector<vector<int> > donde = de_donde_salen(vuelos, mapeados.size()); //cada posicion de "donde"
	//representa una ciudad, mapeada a un int, y en cada una esta la posición donde esa ciudad es una ciudad de origen en el vector "vuelos"
	int origen_mapeado;
	int destino_mapeado;
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
    return 0;
}


std::pair<int, vector<int> > salida(vector<vuelo> vuelos, int origen, int destino, vector<vector<int> > donde)
{
	int n = vuelos.size();
	bool verifico[n];//arreglo donde indico si ya analice ese vuelo y todos aquellos que puedo analizar siguiendo este vuelo 
	for (int i = 0; i <n ; i++ )
	{
		verifico[i] = false; //inicializo con false indicando que todavia no analice ningun vuelo
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
			std::pair<int, vector<int> > s = Buscar_Llegada(vuelos, verifico, vuelos[posiciones[i]], posiciones[i], destino, donde);
			soluciones.push_back(s);			
		}
		if (No_Hay_Camino(soluciones))
		{
			std::pair<int, vector<int> > no_hay_solucion;
			no_hay_solucion.first = -1;
			return no_hay_solucion; // no hay vuelos que salgan desde "origen"
		}
		else
		{
			return minimo_camino(soluciones);
		}
	}
}			

std::pair<int, vector<int> > Buscar_Llegada(vector<vuelo> vuelos, bool verifico[], vuelo salida, int posicion_en_vuelos, int destino, vector< vector<int> > donde)
{
	std::pair <int , vector<int> > itinerario; 
	if (verifico[posicion_en_vuelos] == false)//no verifique ese vuelo
	{
		if (salida.lugar_de_llegada == destino)
		{
			vector<int> camino; //acá voy a tener la lista con todos los vuelos que me ayudan 
			//a llegar a destino
			camino.push_back(posicion_en_vuelos); 
			itinerario.first = salida.hora_llegada; //la hora que llego a destino
			itinerario.second = camino; // los vuelos para llegar
			verifico[posicion_en_vuelos] = true; //si otro vuelo hace uso de este vuelo ya tengo la información que necesito y no vuelvo a calcularl
		}
		else 
		{
			vector<int> posiciones = donde[salida.lugar_de_llegada];
			if (posiciones.size() == 0)
			{ 
				verifico[posicion_en_vuelos] = true; //no llegue a destino y desde esta ciudad no hay mas vuelos este camino no sirve
				itinerario.first = -1;
			}
			else
			{
				vector <std::pair<int, vector<int> > > soluciones;
				for(int i =0; i< posiciones.size(); i++)//analizo todos los vuelos que salen desde esta ciudad
				{
					if(vuelos[posiciones[i]].hora_salida - salida.hora_llegada >= 2)
					{
						std::pair<int, vector<int> >  s = Buscar_Llegada(vuelos, verifico, vuelos[posiciones[i]], posiciones[i], destino, donde);
						soluciones.push_back(s);
					}	
				}
				if (No_Hay_Camino(soluciones))
				{
					verifico[posicion_en_vuelos] = true; //no llegue a destino  y desde esta ciudad y sus sgtes vuelos no sirven
					itinerario.first = -1;
				}
				else
				{
					itinerario =  minimo_camino(soluciones);//hay minimo una combinacion de vuelos para llegar  a destino elijo el que llega antes
					itinerario.second.push_back(posicion_en_vuelos); //agrego este vuelo ya que me sirve para llefar a destino
					verifico[posicion_en_vuelos] = true; // actualizo verifico
				}	
			}	
		}
	}
	else
	{
		itinerario.first = -1;//si ya analice este vuelo y sus sgtes no vuelo a analizar
	}		
	return itinerario;
}

