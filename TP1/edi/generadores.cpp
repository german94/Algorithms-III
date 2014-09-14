#include <queue>
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <ctime>

#include <cstdlib>


using namespace std;
 int main(){


    ofstream myfile;
    myfile.open ("entradaaleatoria.txt", ios::app);
	int cantInstancias = 200;
	int r = 100;
    for (int i = 10000; i < 10100; ++i)
    {	
	cout<<"generando instancia "<<i<<endl;
int cantEdi = i;
    	myfile<< cantEdi<< " ";
	
    	//no hace falta la semilla del clock, si la ponía me tiraba resultados iguales en cada instancia
        for (int j = 0; j < cantEdi; ++j)
        {		
        		int izq =rand() %r;
        		int der =rand() %r;

	        		if (izq>= der)
				{
					int aux = izq;
					izq = der;
					der = aux;
				}
	        		
	        		myfile <<izq<<" ";
	                	myfile <<1+rand() %r<<" ";
	        		myfile <<der;

	
	        		if(j!=cantEdi-1)
	        			myfile <<" ";
        }
        myfile << "\n";
    }

	myfile <<"0";
	myfile.close();

return 0;
}
