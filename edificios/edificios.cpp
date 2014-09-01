#include <queue>
#include <string>
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>  


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



list<int> edificios(vector<int> v1, int cant) //n es la cantidad
{

	//pasar a tuplas en un vector
	

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
		for (int i = 0; i < cant ; ++i)
		{
			Edificio e;
			e.i_ = v[i].a_; 
			e.a_ = v[i].i_;
			e.d_ = v[i].d_;
			v[i]=e;	

		}

		//ordenar por Izq que está en la posición de Alt.
		std::sort (v.begin(), v.begin()+cant);


		// reestablecer las tuplas
		for (int i = 0; i < cant ; ++i)
		{
			Edificio e;
			e.i_ = v[i].a_; 
			e.a_ = v[i].i_;
			e.d_ = v[i].d_;
			v[i]=e;	

		}

	// ya tengo las tuplas ordenadas por izquierda en un vectore de tuplas.


	std::priority_queue<Edificio> heap;
	list <int> res;
	int comparo = 0;
	int siguiente = 1; 

	res.push_back(v[comparo].i_); res.push_back(v[comparo].a_);
	Edificio e_comparo = v[comparo];
	Edificio e_siguiente = v[siguiente];
	while(siguiente < cant) 
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
	
	int a[] = {0,3,8,1,6,5,2,4,6,4,2,7,9,6,10};
	int cant = 5;
	vector<int> entrada;

	for (int i = 0; i < cant*3; ++i)
	{
		entrada.push_back(a[i]);
	}
	
	list <int> res = edificios(entrada, cant);

	for (std::list<int>::iterator it=res.begin(); it != res.end(); ++it)
    std::cout << ' ' << *it;

  std::cout << '\n';
 	return 0;
 }
