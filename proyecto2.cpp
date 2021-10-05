/*
*----------------------------------------
* Proyecto.2 cpp
* ---------------------------------------
* UNIVERSIDAD DEL VALLE DE GUATEMALA
* CC3056 - Programacion de Microprocesadores
* Autor: Ana Paola de León Molina 
* Autor: Alejandro José Gómez Hernández
* Autor: Gabriela Paola Contreras Guerra
* Autor: Cristian Eduardo Aguirre Duarte
* Ver.: 1.0
* Fecha: 2021/09/25
* Modf.: 2021/10/4
*
* References:
* "Intro to Parallel Programming (Pacheco) Ch3.7.pdf"
* "Introduction to Parallel Computing 2Ed (Grama,Gupta,Karypis) Ch9.3.pdf"
*
* ---------------------------------------
* Entrega final.
*----------------------------------------
*/

// Se llaman las librerias a utilizar
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <pthread.h>

using namespace std;

const int cantElementos = 20; // Cantidad de elementos contenidos en lista, variar según cantidad de datos requeridos
int CantidadThreads = 4;

// Se define variable condicional de tipo pthread
int Numss[cantElementos];
// Se define la mutex a utilizar
pthread_mutex_t bloqueado;

pthread_cond_t lista_vacia = PTHREAD_COND_INITIALIZER;
int cantThreads = (cantElementos + 1) / 2;
long Valores[4];
int n;

long Variable_Temporal;

// Se define una estructura con los parametros a implementar
struct parameters
{
	long id;
	long min;
	long max;
};

// Declarar funciones vacias
void ObtencionNum(void);
void *ComparacionNum(void *arg);
void Bubblesort(int max, int min);
void ClasificacionNum(pthread_t threadsID[]);


/*
* ObtencionNum
* Por medio de este método, se obtiene el número a
* ingresar en el ordenamiento
* 
*/
void ObtencionNum(void)
{
	int valide = 0;
	int numValidado = 0;

	while ( valide == 1 ) {

		// Se le solicita al usuario que ingrese la cantidad de numeros
		cout << "Indique la cantidad de numeros que desea clasificar: ";
		cin >> n;

		try
		{
			numValidado = (int) n;
			valide++;
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}

	}

	// Ciclo for con la cantidad de elementos "n" definidos
	for (int i = 0; i <= numValidado; i++)
	{
		Numss[i] = 1 + (rand() % 100);
	}
}

/*
		Main: -void-
Declaracion del metodo main
Se declaran funciones principales y
llamado a subrutinas definidas

*/

int main(void)
{
	//pthread_t tid[cantThreads];
	pthread_t threads[cantThreads];
	long min = 0;
	long max = 0;

	//ObtencionNum();
	//int valide = 0;
	int numValidado = 0;

	// Ciclo for con la cantidad de elementos "n" definidos
	for (int i = 0; i <= cantElementos; i++)
	{
		Numss[i] = 1 + (rand() % 100);
	}
	
	// Inicialización de mutex
	if (pthread_mutex_init(&bloqueado, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    } 

	int hilo = 1;
	cout << endl
		 << "---------------------- LISTA INICIAL ----------------------" << endl
		 << endl;
	for (int i = 0; i < cantElementos; i++) {
		cout << i << " -> " << Numss[i] << endl;
	}

    // Clasificación de números
    pthread_t threadID[cantThreads];
	int i = 0;
	int j = 0;
	int err;

	for (i = 1; i <= cantElementos; i++) {

		// Verificar si i es par o impar
		if (i % 2 == 0) {

			// Es par
			// Creacion de hilos
			Variable_Temporal = 1;
			for (j = 0; j < cantThreads - 1; j++) {
				err = pthread_create(&threadID[j], NULL, ComparacionNum, (void *)Variable_Temporal);
				if (err != 0)
                {
                    printf("\nError al crear thread! \n");	//impresion de mensaje si el hilo no se crea correctamente
                    return 0;
                }
				Variable_Temporal = Variable_Temporal + 2;
			}

			for (j = 0; j < cantThreads - 1; j++) {
				pthread_join(threadID[j], NULL);
			}
		}
		if (i % 2 == 1) {

			// Es impar
			// Creacion de hilos
			Variable_Temporal = 0;
			for (j = 0; j < cantThreads; j++) {
				err = pthread_create(&threadID[j], NULL, ComparacionNum, (void *)Variable_Temporal);
				if (err != 0)
                {
                    printf("\nError al crear thread! \n");	//impresion de mensaje si el hilo no se crea correctamente
                    return 0;
                }
				Variable_Temporal = Variable_Temporal + 2;
			}

			for (j = 0; j < cantThreads; j++) {
				pthread_join(threadID[j], NULL);
			}
		}
	}

	hilo = 1;
	cout << endl
		 << "---------------------- LISTA ORDENADA ----------------------" << endl
		 << endl;
	for (int i = 0; i < cantElementos; i++) {
		if (cantElementos == 20) {
			if (i == 0 || i == 5 || i == 10 || i == 15)
			{
				cout << "\nHilo: " << hilo << endl;
				hilo++;
			}
		}

		cout << i << " -> " << Numss[i] << endl;
	}

	// Destrucción de mutex
	pthread_mutex_destroy(&bloqueado);

	return 0;
}


/*
* Bubblesort
* Este método se encarga de ordenar por medio
* del algoritmo BubbleSort
*/
void Bubblesort(int max, int min)
{
	//cout<<"Entro al Bubblesort"<<endl;
	float temporal;

	for (int i = min; i <= max; i++)
	{
		for (int j = min; j <= max - 1; j++)
		{
			if (Numss[j] < Numss[j + 1])
			{ // Ordena el array de mayor a menor, cambiar el "<" a ">" para ordenar de menor a mayor
				temporal = Numss[j];
				Numss[j] = Numss[j + 1];
				Numss[j + 1] = temporal;
			}
		}
	}
}


void *ComparacionNum(void *arg)
{
	// Se define la variable de iteracion, con el valor de la variable temporal.
	long i = (long)arg;

	if ( Numss[i] > Numss[i + 1] )
	{ // Ordena el array de mayor a menor, cambiar el "<" a ">" para ordenar de menor a mayor
		pthread_mutex_lock(&bloqueado); // bloqueo mutex
		long temporal = Numss[i];
		Numss[i] = Numss[i + 1];
		Numss[i + 1] = temporal;
		pthread_mutex_unlock(&bloqueado); // desbloqueo mutex
	}

	return NULL;
}
