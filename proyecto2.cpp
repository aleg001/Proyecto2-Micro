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
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <vector>

using namespace std;

int cantElementos = 20;

int CantidadThreads = 4;

// Se define variable condicional de tipo pthread
int Nums1[20] = {10, 13, 8, 9, 1, 4, 5, 2, 3, 6, 15, 20, 18, 17, 12, 11, 14, 19, 7, 16};

// Arrays con numeros aleatorios
int Nums2[20] = {47, 48, 49, 44, 45, 46, 55, 56, 57, 58, 59, 51, 52, 53, 54, 40, 41, 42, 43, 60};
int Nums3[20] = {21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40};
int Nums7[20] = {44, 45, 46, 47, 34, 35, 36, 41, 42, 43, 37, 38, 39, 40, 48, 49, 50, 31, 32, 33};
int Nums[20] = {63, 64, 65, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 66, 67, 68, 69, 70, 61, 62};

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
	int num;
	// Se le solicita al usuario que ingrese la cantidad de numeros
	cout << "Indique la cantidad de numeros que desea clasificar: ";
	cin >> n;

	//n = 20;
	cantElementos = n;

	// Ciclo for con la cantidad de elementos "n" definidos
	for (int i = 0; i <= n; i++)
	{
		Nums[i] = 1 + (rand() % 100);
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

	int hilo = 1;
	cout << endl
		 << "---------------------- LISTA INICIAL ----------------------" << endl
		 << endl;
	for (int i = 0; i < cantElementos; i++) {
		if (i == 0 || i == 5 || i == 10 || i == 15) {
			cout << "\nHilo: " << hilo << endl;
			hilo++;
		}
		cout << i << " -> " << Nums[i] << endl;
	}

    // Clasificación de números
    pthread_t threadID[cantThreads];
	int i = 0;
	int j = 0;

	for (i = 1; i <= cantElementos; i++) {

		// Verificar si i es par o impar
		if (i % 2 == 0) {

			// Es par
			// Creacion de hilos
			Variable_Temporal = 1;
			for (j = 0; j < cantThreads - 1; j++) {
				pthread_create(&threadID[j], NULL, ComparacionNum, (void *)Variable_Temporal);
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
				pthread_create(&threadID[j], NULL, ComparacionNum, (void *)Variable_Temporal);
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
		if (i == 0 || i == 5 || i == 10 || i == 15)
		{
			cout << "\nHilo: " << hilo << endl;
			hilo++;
		}

		cout << i << " -> " << Nums[i] << endl;
	}

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
			if (Nums[j] < Nums[j + 1])
			{ // Ordena el array de mayor a menor, cambiar el "<" a ">" para ordenar de menor a mayor
				temporal = Nums[j];
				Nums[j] = Nums[j + 1];
				Nums[j + 1] = temporal;
			}
		}
	}
}


void *ComparacionNum(void *arg)
{
	// Se define la variable de iteracion, con el valor de la variable temporal.
	long i = (long)arg;

	if (Nums[i] > Nums[i + 1])
	{ // Ordena el array de mayor a menor, cambiar el "<" a ">" para ordenar de menor a mayor
		long temporal = Nums[i];
		Nums[i] = Nums[i + 1];
		Nums[i + 1] = temporal;
	}

	return NULL;
}
