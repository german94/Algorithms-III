OrdenarEdificios (e_1, e_2, e_3, ......, e_n) {

heap(edificios) heap = crear();

ordeno_edificios_de_izq_a_der (e_1, e_2,....,e_n );
int comparo = 1;
int siguiente = 2; 

imprimir_primer_punto (e_1.izq, e_1.alt)

while(siguiente < total_de_edificios) 
{

	if (edificio_comparo.der >= edificio_siguiente.izq) //el edificio i corta al i+1 
 	{
 		if(edificio_comparo.alt < edificio_siguiente.alt) // el siguiente edificio es mas alto estrictamente
 		{	//entonces cambia la altura, es una solucion.  
 			imprimir (edificio_siguiente.izq, edificio_siguiente.alt);
 			encolar(edificio_comparo);
 			comparo = siguiente;
 			siguiente++;
 		}	
 		else
 		{ // el siguiente edifio es mas chico lo salteo momentaneamente
 			encolar(edificio_siguiente)
 			siguiente++; // ahora comparo con el siguiente al siguiente, con el cual voy a comparar se mantiene
 		}
 	}
 	else // el siguiente edificio no me corta, entonces puede que en el medio de estos dos
 	//	(comparar y siguiente) haya algun techo que quedo de antes es decir, el contorno
 	// baja y ese es un punto solucion 
 	{
 		
 		for (; !vacio(heap), heap.desencolar)
 		{
 			if(heap.proximo.der > edificio_comparo.der) 
 			{ // luego del comparo, la altura disminuye hasta la del proximo, esta es una solucion
 				imprimir(edificio_comparo.der, heap.proximo.altura)
 				comparo = heap.proximo //ahora voy a comparar el que estaba en el heap con siguiente
 				heap.desencolar //no encolo nada porque el que era comparo ya quedo atras
 				break for 
 			}	// else si el que estaba proximo su lado derecho es menor que el de comparo
 			//significa que quedo atras el del heap entonces lo desencolo y me olvido de ese
 		}
 		if (vacio(heap)) //si sali del heap porque me quede sin edificios
 		{// entonces no hay nada entre ambos edificios la altura del siguiente es solucion
 			//ya que termina el edificio_comparo, luego la altura es la del piso y asciendo con
 			// la altura del edificio siguiente
 			imprimir(edificio_siguiente.izq, edificio_siguiente.alt)
 			comparo = siguiente; // no encolo nada porque es como si el ejercicio empezara de nuevo
 			siguiente++ //  
 		} // si el heap no esta vacio entonces el if de adentro del for ya dejo arreglado
 	}// quien es el comparar y siguiente;
 }	

 }



 	















} 












} 