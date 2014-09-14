#include <queue>
#include <string>
#include <iostream>
#include <vector>

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

vector<int> edificios(vector<Edificio> v, int total_de_edificios) //n es la cantidad
{

	std::priority_queue<Edificio> heap;
	vector <int> res;
	int comparo = 0;
	int siguiente = 1; 

	res.push_back(v[comparo].i_); res.push_back(v[comparo].a_);
	Edificio e_comparo = v[comparo];
	Edificio e_siguiente = v[siguiente];
	while(siguiente < total_de_edificios) 
	{
			if (e_comparo.d_ >= e_siguiente.i_) //el edificio i corta al i+1 
 			{
 				if(e_comparo < e_siguiente) // el siguiente edificio es mas a_o estrictamente
 				{	//entonces cambia la a_ura, es una solucion.  
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
 					res.push_back(e_comparo.d_); res.push_back(0); res.push_back(e_siguiente.i_); res.push_back(e_siguiente.a_);
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
	return res;
 }

	
int main()
{
	
	int a[] = {1, 2, 10, 2, 3, 11, 3, 4, 5, 4, 5, 6, 6, 5, 7, 8, 6, 9};
	int tam = 18;
	int cant= tam/3;
	vector<Edificio> entrada;

	for (int j = 0; j < tam; j=j+3)
	{
		Edificio e;
		e.i_ = a[j]; 
		e.a_ = a[j+1];
		e.d_ = a[j+2];	
		entrada.push_back(e);
	}
	
	vector <int> res = edificios(entrada, cant);
	for(int j = 0; j < res.size(); j++)
	{
		cout << res[j] << " ";
	}
	
 	return 0;
 }
