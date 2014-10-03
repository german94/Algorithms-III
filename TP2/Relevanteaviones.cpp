std::pair<int, list<int> > desde_origen(vector<string> ciudades, vector<int> horarios, string desde, string hasta){

	list<string> mapeados = mapear(ciudades); //cada ciudad distinta recibe un numero, la numeración va de
	//0 .. 2n-1 (siendo 2n a lo sumala cantidad de ciudades distintas)
	vector<vuelo> vuelos = construir_vuelos(mapeados, ciudades, horarios); //construyo un vector de vuelos
	// cada vuelo es <ori, des, ini, fin> solo que ori y des no son del tipo string sino int segun la funcion mapear 
	int origen, destino;
	bool existe_origen = false; //si la ciudad origen o destino no esta en ningun vuelo no hay solución
	bool existe_destino = false;
	std::pair<int, list<int> > resultado;  
	int i = 0;
	for(std::list<string>::iterator it = mapeados.begin(); it != mapeados.end(); ++it, i++)
	{
		if(*it == desde) {origen = i; existe_origen = true;}
		if(*it == hasta) {destino = i; existe_destino = true;}
	}//busco a la ciudad de ORIGEN y DESTINO de la instancia y devuelvo su equivalente en int
	if(!existe_origen || !existe_destino) {resultado.first = -1; return resultado;} // devuelvo -1
	else
	{	
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
		if (posiciones.empty()) { resultado.first = -1; return resultado; }
		else
		{	
			list <std::pair<int, list<int> > > soluciones; // la segunda componente contiene 
			//una lista de vuelos, según la aparición en la entrada, cuya combinación llega a destino, la primera la hora de llegada
			for(std::list<int>::iterator it = posiciones.begin(); it != posiciones.end(); ++it)
			{
				std::pair<int, list<int> > s = Buscar_Llegada(vuelos, vuelos_disponibles, vuelos[*it], *it, destino, donde, ya_lo_use);
				soluciones.push_back(s);		
			}
			if (No_Hay_Camino(soluciones)) {resultado.first = -1; return resultado;}
			else { return  minimo_camino(soluciones);}
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
				itinerario.second.push_back(posicion_en_vuelos); //agrego este vuelo ya que me sirve para llegar a destino
			}	
		}
		else { itinerario.first = -1;}
	}
	return itinerario;
}
