void resolverInstancia(vector<int> p, int c)
{
	int length = p.size();
    int ultimosano = -1;
    int saltados = 0;
    std::list<int> s;
    
    for(int i=0; i<p.size(); i++)
    {
    	saltados++;
    	if(p[i] == 0)
    		ultimosano = i;

    	if(saltados == c + 1)	//si pase todos los tablones que puedo llegar a saltar desde el ultimo tablon sano en donde estaba parado:
    	{
    		//si la distancia del tablon donde estoy ahora al ultimo tablon sano (no necesariamente donde estaba parado antes ya que lo voy pisando) es mayor que lo que puedo
    		//llegar a saltar, entonces quiere decir que no hay ningun tablon sano en el medio, o sea que tengo mas de c tablones rotos
    		if(i - ultimosano > c || ultimosano == -1)
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
    		saltados = i - ultimosano;
    	}   	
    }

    s.push_back(p.size() + 1);

    list<int>::const_iterator ci;
    for(ci=s.begin(); ci!=s.end(); ci++)
   	    cout << *ci << " ";
}