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
* Modf.: 2021/09/25
*
* References:
* "Intro to Parallel Programming (Pacheco) Ch3.7.pdf"
* "Introduction to Parallel Computing 2Ed (Grama,Gupta,Karypis) Ch9.3.pdf"
*
* ---------------------------------------
* Entrega preliminar.
*----------------------------------------
*/

// Se llaman las librerias a utilizar
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <cstdlib>

using namespace std;

// Se define variable condicional de tipo pthread
pthread_cond_t lista_vacia = PTHREAD_COND_INITIALIZER;
int cantThreads = 4;
int n;
int Nums [20] = {18, 14, 12, 2, 5, 10, 15, 3, 6, 9, 1, 7, 8, 4, 11, 13, 16, 20, 19, 17};

/*
* ObtencionNum
* Por medio de este método, se obtiene el número a
* ingresar en el ordenamiento
*/
void ObtencionNum(void)
{
    int num;
    cout<<"Indique la cantidad de numeros que desea clasificar";
    cin>>n;

    n = 20;
    // Ciclo for con la cantidad de elementos "n" definidos
    for (int i = 0; i <= 20; i++)
    {
        Nums[i] = 1 + (rand() % 100);
    }
}

/*
* Bubblesort
* Este método se encarga de ordenar por medio
* del algoritmo BubbleSort
*/
void Bubblesort(int max, int min)
{
    for (int i = min; i <max ; i++) {
        for (int j = min; max-i-1; j++){
            if (Nums[j] > Nums[j+1]) {
                int jTemp = Nums[j];
                int jMasTemp = Nums[j+1];
                Nums[j+1] = jTemp;
                Nums[j] = jMasTemp;
            }
        }
    }
    
}

/*
* ClasificacionNum
* Este método se utiliza para realizar los procesos correspondientes a
* el ordenamiento de los elementos de la lista.
*/

void* ClasificacionNum(void *iValue)
{
    // Casting de identificador
    long id = (long) iValue;
    
    // División de dominio
    long nTotal = 20/cantThreads; // 20 por la cantidad de elementos en la lista
    long min = id*nTotal; // Primera iteración realizada
    long max = id + nTotal; // Última iteración realizada

    for (int i = min; i <= max; i++)
    {
        // Llamar BubbleSort
        Bubblesort(max, min);
    }
    
    return NULL;
}

/*
* Main
* Este método es utilizado como parta principal del código, en el cual
* se realizan los procesos llamando a la obtencionNum y ClasificacionNum
*/

int main(void)
{
    // Llamado al método de Obtencion Num
    pthread_t tid[cantThreads];
    ObtencionNum();
    // Declaración de variables a usar
    int i = 0;
    int err;
    n = 20;
    
    // Crear hilos
    while(i < 4)													
    {
        err = pthread_create(&(tid[i]), NULL, &ClasificacionNum, (void *)&i); 	//creacion de hilos sin paso de parametros
        if (err != 0)
            printf("\nERROR: Can't create thread");	//impresion de mensaje si el hilo no se crea correctamente
            return 0;
        i++;
    }


    // Imprimir lista
    for (int i = 0; i <= n; i++)
    {
        cout << " -> " << Nums[i] << endl;

    }

    while(i < 4)													
    {
        pthread_join(tid[i], NULL);
        i++;
    }

    return 0;
}