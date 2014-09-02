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


struct Edificio
{
	int i_;
	int a_;
	int d_;
	bool operator< (const Edificio& otra) const
	{
			return this->a_ < otra.a_;
	}
	bool operator!=(const Edificio& otra) const
	{
		return this->a_ != otra.a_ && this->i_ != otra.i_ && this->d_ != otra.d_;
	}

};

std::ostream& operator<<(std::ostream& os, const Edificio& edi)
{
  os << "<" << edi.i_ << ", " << edi.a_ << ", " << edi.d_ << ">";

  return os;
}

bool porIzq(const Edificio& e1,const Edificio& e2)
{
	if (e1.i_ == e2.i_)
		return e1.a_ > e2.a_;
	else 
		return e1.i_ < e2.i_;
}

bool dentroDe(const Edificio& e1,const Edificio& e2)
{
	return (e1.i_ >= e2.i_ && e1.i_ <= e2.d_) && (e1.d_ <= e2.d_ && e1.d_ >= e2.i_);
}


void DarTiempo(int n, double t)
{
	ofstream myfile;
	myfile.open ("tiempos.txt", ios::app);
	myfile<<t<< "\n";
	myfile.close();
}


void resolverInstancia(vector<int> v1, int cant) //n es la cantidad
{

	//pasar a tuplas en un vector
	int length = cant;
	clock_t begin = clock();

	std::vector <Edificio>  v(cant);
	int pos=0;
	for (int j = 0; j < cant*3; j=j+3)
	{
		Edificio e;
		e.i_ = v1[j]; 
		e.a_ = v1[j+1];
		e.d_ = v1[j+2];	
		v[pos]=e;
		pos++;
	}

	// ordenar las tuplas
		//invertir Alt por Izq, esto lo hacemos porque < está en función de la altura para el heap.
/*		for (int i = 0; i < cant ; ++i)
		{
			Edificio e;
			e.i_ = v[i].a_; 
			e.a_ = v[i].i_;
			e.d_ = v[i].d_;
			v[i]=e;	

		}*/

		//ordenar por Izq que está en la posición de Alt.
		std::sort (v.begin(), v.begin()+cant,porIzq);

		for (int i = 0; i < cant; ++i)
		{
			cout<< v[i]<< " ";
		}

		// reestablecer las tuplas
/*		for (int i = 0; i < cant ; ++i)
		{
			Edificio e;
			e.i_ = v[i].a_; 
			e.a_ = v[i].i_;
			e.d_ = v[i].d_;
			v[i]=e;	

		}
*/
	// ya tengo las tuplas ordenadas por izquierda en un vectore de tuplas.
	//eliminar edificios metidos dentro de otro
	/*	std::vector <Edificio> v2(cant);
		int posV2=0;
		for (int i = 0; i < cant-1; ++i)
		{
			if (cant>=2)
				{
					if (!dentroDe (v[i], v[i+1]))
					{	
						Edificio e;
						e.i_ = v[i].i_; 
						e.a_ = v[i].a_;
						e.d_ = v[i].d_;	
						v2[posV2]=e;
						posV2++;
					};
				}
		}

		for (int i = 0; i < cant; ++i)
		{
			cout<< v2[i];
		}
*/

	std::priority_queue<Edificio> heap;
	list <int> res;
	int comparo = 0;
	int siguiente = 1; 

	res.push_back(v[comparo].i_); res.push_back(v[comparo].a_);
	Edificio e_comparo = v[comparo];
	Edificio e_siguiente = v[siguiente];
	while(siguiente < cant) 
	{			// se arruina caso edificios iguales mayor altura siguiente
			if (e_comparo.d_ >= e_siguiente.i_ ) //el edificio i corta al i+1 
 			{	//en este if había un problema, cuando las izqs eran iguales y la altura de siguiente es mayor
 				if(e_comparo < e_siguiente && e_comparo.i_ != e_siguiente.i_ ) // el siguiente edificio es mas a_o estrictamente
 				{	//y los izqs son distintos, entonces cambia la a_ura, es una solucion.
 				// if(e_comparo < e_siguiente) // el siguiente edificio es mas a_o estrictamente
 				// {	//y los izqs son distintos, entonces cambia la a_ura, es una solucion.    
 					res.push_back(e_siguiente.i_); res.push_back(e_siguiente.a_);
 					if(heap.empty())
 						{	
 							heap.push(e_comparo);
 						}
 						else
 						{
 							if (heap.top() != e_comparo) { heap.push(e_comparo);}
 						}	
 					comparo = siguiente;
 					e_comparo = v[comparo];
 				}	
 				else
 				{ // el siguiente edifio es mas chico lo sa_eo momentaneamente
 					if(e_comparo.a_ == e_siguiente.a_)
 					{
 						if(heap.empty())
 						{	
 							heap.push(e_comparo);
 						}
 						else
 						{
 							if (heap.top() != e_comparo) { heap.push(e_comparo);}
 						}	
 						comparo = siguiente;
 						e_comparo = v[comparo];
 					}
 					else
 					{	
 						heap.push(e_siguiente);
 					}
 				}
 				siguiente++;
 				e_siguiente = v[siguiente]; 
 			}
			else // el siguiente edificio no me corta, entonces puede que en el medio de estos dos
 	//	(comparar y siguiente) haya algun techo que quedo de antes es decir, el contorno
 	// baja y ese es un punto solucion 
 			{
 				for (; !heap.empty(); heap.pop())
 				{
 					if(heap.top().d_ > e_comparo.d_) 
 					{ // luego del comparo, la a_ura disminuye hasta la del proximo, esta es una solucion
 						res.push_back(e_comparo.d_); res.push_back(heap.top().a_);
 					 	e_comparo = heap.top(); //ahora voy a comparar el que estaba en el heap con siguiente
 						break;
 					}	// else si el que estaba proximo su lado d_echo es menor que el de comparo
 			//significa que quedo atras el del heap entonces lo desencolo y me olvido de ese
 				}
 				if (heap.empty()) //si sali del heap porque me quede sin edificios
 				{// entonces no hay nada entre ambos edificios la a_ura del siguiente es solucion
 					res.push_back(e_comparo.d_); res.push_back(0); 
 					//cambio en el if
 					if (siguiente != cant-1)
 							res.push_back(e_siguiente.i_); res.push_back(e_siguiente.a_);

 					/**/res.push_back(e_siguiente.i_); res.push_back(e_siguiente.a_);
 					comparo = siguiente; // no encolo nada porque es como si el ejercicio empezara de nuevo
 					siguiente++; //  
 					e_comparo = v[comparo];
 					e_siguiente = v[siguiente];
 				} // si el heap no esta vacio entonces el if de adentro del for ya dejo arreglado
 			}// quien es el comparar y siguiente;
 	}//comparo quedo dentro del limite 
 	if (!heap.empty()) //por si ya no hay sgtes pero si me quedaron algunos en el heap
 	{
 		for (;!heap.empty(); heap.pop())
 		{
 			if(heap.top().d_ > e_comparo.d_) 
 				{ // luego del comparo, la a_ura disminuye hasta la del proximo, esta es una solucion
 					if (heap.top() < e_comparo)
 					{	
 						res.push_back(e_comparo.d_);
						res.push_back(heap.top().a_);
					}
					e_comparo = heap.top();	// else si el que estaba proximo su lado d_echo es menor que el de comparo			
 				} 	//significa que quedo atras el del heap entonces lo desencolo y me olvido de ese
 		}
 	}
 	res.push_back(e_comparo.d_);
	res.push_back(0);


    clock_t end = clock();
    double elapsed_msecs = (double(end - begin) / CLOCKS_PER_SEC) * 1000;
    DarTiempo(length, elapsed_msecs);
	
	for (std::list<int>::iterator it=res.begin(); it != res.end(); ++it)
    std::cout << ' ' << *it;
	std::cout << '\n';

 }
//no fuciona porque genera edificios no válidos, por ejemplo 7 2 1 siempre izq es menor que derechar
//por enunciado, traté de solucionarlo pero entra en un while infinito :/
void generar(int cantEdi, int cantInstancias, int r)
{   
    ofstream myfile;
    myfile.open ("entrada.txt", ios::app);

    for (int i = 0; i < cantInstancias; ++i)
    {	
    	myfile<< cantEdi<< " ";
    	//no hace falta la semilla del clock, si la ponía me tiraba resultados iguales en cada instancia
        for (int j = 0; j < cantEdi; ++j)
        {		
        		int izq =rand() %r;
        		int der =rand() %r;

	        		if (izq< der)
	        			{
	        				myfile <<izq<<" ";
	                		myfile <<1+rand() %r<<" ";
	        				myfile <<der;
	        			}
	        		else
	        		{	
	        			while(izq>= der)
	        			{	
	        				izq =rand() %r;
	        				der =rand() %r;
	        			}
        				myfile <<izq<<" ";
                		myfile <<1+rand() %r<<" ";
        				myfile <<der;
	        		}
	        		if(j!=cantEdi-1)
	        			myfile <<" ";
        }
        myfile << "\n";
    };

    myfile.close(); 



}

	
int main()
{	
	//generados de instancias
	std::ofstream file("entrada.txt", std::ios::trunc); // eliminar viejas instancias.
	for(int i=2; i>0; i--)
    {
    	
    	generar(i*10000,1,9);
	}
	ofstream myfile;
	myfile.open ("entrada.txt", ios::app);
	myfile <<"0";
	myfile.close();

	//generar una isntancia
/*	generar(1,1);

	ofstream myfile;
	myfile.open ("entrada.txt", ios::app);
	myfile <<"0";
	myfile.close();*/


	vector<vector<int> > v;
	string l;
	getline(cin, l); // leer una linea (cuanto escribo hasta enter)y almacenarla en l
	while(l.at(0) != '0')
	{
		vector<int> vec;
		istringstream ss(l);//eso mete el string en un "stream", donde de ahi podes llamar a una funcion donde podes itearar el resultado y el resultado va a ser cada "substring" que este separada por el espacio
							//si tenes "asdf ffff ddd" en una iteracion vas a tener asdf, en otra fff y en otra ddd
		string token;
		while(std::getline(ss, token, ' ')) //le decis bueno, tengo mi string en un stream ss
											//quiero delimitar los substrings por el caracter ' '
											//y quiero que cada substring se guarde en el string token
		    vec.push_back(atoi(token.c_str())); // convierte el string token en entero

		v.push_back(vec);
		getline(cin, l);
	}
	cout<<"___________________________\n"<<"Solucion" <<endl;

	
	for(int i=0; i<v.size(); i++)
	{
		int c = v[i][0];
		v[i].erase(v[i].begin(), v[i].begin()+1); // elimino del vector el 1 que es la cantidad de edificios
		resolverInstancia(v[i], c);
	}


/*
	int a[] = {0,3,8,1,6,5,2,4,6,4,2,7,9,6,10};
	int cant = 5;
	vector<int> entrada;

	for (int i = 0; i < cant*3; ++i)
	{
		entrada.push_back(a[i]);
	}
	
	list <int> res = edificios(entrada, cant);*/
/*
	Edificio e;
		e.i_ = 1;
		e.a_ = 5;
		e.d_ = 3;
	Edificio e1;
		e1.i_ = 1;
		e1.a_ = 2;
		e1.d_ = 3;

	cout<< dentroDe(e,e1);*/


	return 0;
    
 }
