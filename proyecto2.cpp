#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <cstdlib>

using namespace std;

int Nums[20];
int n;

void ObtencionNum(void){
    int num;
    //cout<<"Indique la cantidad de numeros que desea clasificar";
    //cin>>n
    n=20;
    for (int i=0; i<=20; i++){
        Nums[i]=1+(rand()%100);
    }
}

void ClasificacionNum(void){

}

int main(int argc, char* argv[]) {
    ObtencionNum();
    n=20;
    for (int i=0; i<=n; i++){
        cout<<" -> "<<Nums[i]<<endl;
    }
	return 0;
}