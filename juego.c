#include <stdio.h>
#include <stdbool.h>
#include "simpsoncraft.h"
#include <stdlib.h>
#include "utiles.h"
#include <time.h>

const int JUGANDO=0;
const int GANO=1;
const int PERDIO=-1;
const char VACIO=' ';

/*
Pre:
Post:Da una introduccion al usuario respecto a como funciona el juego
*/
void presentacion(){
    printf("Bienvenido a SimpsonCraft\n");
    printf("Las instrucciones son las siguientes....\n");
    printf("Tu objetivo es recuperar la receta secreta que Moe te robo,ir a la fiesta que organizo y revelarsela al publico\n");
    printf("Para poder hacerlo deberas moverte por el mapa,romper tierras,telarañas,evitar los obstaculos y recolectar herramientas que te ayudaran en el camino\n");
    printf("Las vigas no podes romperlas, no sos un superheroe :v\n");
    printf("Las instrucciones del juego son las siguientes:\n");
    printf("D para moverte a la derecha\n");
    printf("A para moverte a la izquierda\n");
    printf("E para usar una escalera y subir(necesitas 3 maderas para crearla)\n");
    printf("O para usar el soplete(este elimina todos los bloques que esten a 2 casillas de distancia de homero(las vigas NO :())\n");
    printf("S para moverte hacia abajo\n");
    printf("Cuando rompes un bloque adentro existe la posibilidad de que encuentres un objeto o herramienta\n");
    printf("Homero es representado por 👨\nMoe por 🍻\nLa receta por 📜\n");
    printf("La telaraña es representada por 🕸️ (golpeandola una vez la rompes)\nLa tierra por 🟫(tenes que golpearlo dos veces para romperla)\nLas vigas por ⬜\nLos murcielagos por 🦇\nLos cables por 🔌\nLa escalera es 🪜\nLas maderas por 🪵\nY los sopletes por 💨\n");
    printf("Tenes 100 movimientos para conseguir la receta y despues llegar a Moe, sino bueno, Moe te robo la idea y fama ;(\n");
    printf("Buenas suerte :D\n");
    printf("Presiona cualquier tecla para empezar\n");
    char comenzar_juego;
    scanf(" %c", &comenzar_juego);
}
/*
Pre:
Post:Devuelve true si el movimiento es valido, false si  es invalido
*/
bool es_movimiento_valido(char movimiento){
    return (movimiento=='A' || movimiento=='D' || movimiento=='S' || movimiento=='E' || movimiento=='O' ||
            movimiento=='a' || movimiento=='d' || movimiento=='s' || movimiento=='e' || movimiento=='o');
}
int main (){
    srand((unsigned) time(NULL));
    juego_t juego;
    presentacion();
    inicializar_juego(&juego);
    system ("clear");
    char movimiento = VACIO;
    int sigue_juego=0;
    while(sigue_juego==0){
        printf("\n");
        mostrar_juego(juego);
        printf("Estado del juego: %d\n", sigue_juego);
        printf("Ingrese el movimiento que desea hacer:\n");
        scanf(" %c", &movimiento);
        while(!es_movimiento_valido(movimiento)){
            printf("Movimiento invalido, ingrese un movimiento valido (A,D,S,O,E):\n");
            scanf(" %c", &movimiento);
            system ("clear");
            mostrar_juego(juego);
            printf("Estado del juego: %d\n", sigue_juego);
        }
        system ("clear");
        realizar_jugada(&juego, movimiento);
        sigue_juego=estado_juego(juego);
        if(sigue_juego==GANO){
            printf("Yei, ganaste, felicidades :D\n");
        }else if(sigue_juego==PERDIO){
            printf("Se acabo el juego. Intenta devuelta :)\n");
        }
    }
    return 0;
}