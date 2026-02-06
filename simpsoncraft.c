#include <stdio.h>
#include <stdbool.h>
#include "simpsoncraft.h"
#include <stdlib.h>
#include "utiles.h"
#include <time.h>

#define HOMERO 'H'
#define MOE 'M'
#define RECETA 'R'
#define TELARAÑA 'A'
#define TIERRA 'T'
#define VIGAS 'V'
#define ESCALERA 'E'
#define MURCIELAGOS 'B'
#define CABLES 'C'
#define MADERA 'W'
#define SOPLETE 'S'

const int MADERA_INICIAL =15;
const int SOPLETES_INICIAL=2;
const int RESISTENCIA_TELARAÑA=1;
const int RESISTENCIA_TIERRA=2;
const int RESISTENCIA_VIGAS=-1;
const int RESISTENCIA_ESCALERA=1;
const int INICIO_HOMERO=0;
const int MAX_MOVIMIENTOS=100;
const int MAX_TELARAÑAS=100;
const int MAX_TIERRA=80;
const int MAX_CABLE=25;
const int MAX_MURCIELAGOS=20;
const int MAX_MADERAS=20;
const int MAX_SOPLETES=10;
const int RECUPERA_MADERAS=2;
const int CREAR_ESCALERA=3;
const int DISTANCIA_SOPLETE=2;
const int RESTAR_MADERA=3;
const int RESTAR_MOVIMIENTO=5;
const char *EMOJI_HOMERO="👨";
const char *EMOJI_MOE="🍻";
const char *EMOJI_RECETA="📜";
const char *EMOJI_TIERRA="🟫";
const char *EMOJI_TELARAÑA="🕸️ ";
const char *EMOJI_VIGAS="⬜";
const char *EMOJI_ESCALERA="🪜";
const char *EMOJI_MURCIELAGOS="🦇";
const char *EMOJI_CABLES="🔌";
const char *EMOJI_MADERA="🪵";
const char *EMOJI_SOPLETE="💨";

/*
Pre:Numero_aleatorio debe ser mayor a 0
Post:Devuelve un numero aleatorio entre 0 y el numero_aleatorio-1
*/
int generar_nro_aleatorio(int numero_aleatorio){
    return rand()%numero_aleatorio;
}
/*
Pre:Filas y columnas debe ser mayor a 0
Post:Devuelve una posicion aleatoria dentro del rango de filas y columnas
*/
coordenada_t posicion_aleatoria(int total_filas, int total_columnas){
    coordenada_t posicion;
    posicion.fil=generar_nro_aleatorio(total_filas);
    posicion.col=generar_nro_aleatorio(total_columnas);
    return posicion;
}
/*
Pre:
Post:Le muestra al usuario la posicion actual de los objetos principales del juego,y los movimientos validos
*/
void recordar(juego_t juego){
    printf("Movimientos: A,D,S,E,O\n");
    printf("Tu posicion: fila %d, columna %d\n",juego.homero.posicion.fil+1,juego.homero.posicion.col+1);
    printf("Posicion de Moe: fila %d, columna %d\n",juego.posicion_moe.fil+1,juego.posicion_moe.col+1);
    if(juego.homero.recolecto_receta==true){
        printf("Recuperaste la receta\n");
    }else{
    printf("Posicion de la receta: fila %d, columna %d\n",juego.posicion_receta.fil+1,juego.posicion_receta.col+1);
    }
    printf("Movimientos restantes: %d\n", juego.movimientos_restantes);
    printf("Cantidad de maderas: %d\n", juego.homero.cantidad_madera);
    printf("Cantidad de sopletes: %d\n", juego.homero.cantidad_sopletes);
}
/*
Pre:
Post:Inicializa a homero
*/
void inicializar_homero (personaje_t *homero, int *movimientos){
    homero->tipo=HOMERO;
    homero->posicion.fil=INICIO_HOMERO;
    homero->posicion.col=INICIO_HOMERO;
    homero->cantidad_madera=MADERA_INICIAL;
    homero->cantidad_sopletes=SOPLETES_INICIAL;
    homero->recolecto_receta=false;
    *movimientos=MAX_MOVIMIENTOS;
}
/*
Pre:El tope de bloques debe estar inicializado en 0
Post:Inicializa las vigas en el juego mediante obtener mapa y se le asigna resistencia y tipo
*/
void inicializar_vigas(bloque_t bloques[],int *tope_bloques){
    coordenada_t coordenada_viga[MAX_VIGAS];
    int tope_vigas=0;
    obtener_mapa(coordenada_viga, &tope_vigas);
    for(int i=0;i<tope_vigas;i++){
        bloques[i].tipo=VIGAS;
        bloques[i].resistencia=RESISTENCIA_VIGAS;
        bloques[i].posicion=coordenada_viga[i];
    }
    *tope_bloques =tope_vigas;
}
/*
Pre:Vigas debe estar inicializado
Post:Devuelve una posicion aleatoria en cualquier columna de la ultima, que no choque con ningun bloque
*/
void inicializar_moe(coordenada_t *posicion_moe, bloque_t bloques[], int tope_bloques){
    bool posicion_moe_valida=false;
    while(!posicion_moe_valida){
        posicion_moe->fil=MAX_FILAS-1;
        posicion_moe->col = generar_nro_aleatorio(MAX_COLUMNAS);
        posicion_moe_valida=true;
        int i=0;
        while(i<tope_bloques && !posicion_moe_valida){
            if(bloques[i].posicion.fil==posicion_moe->fil && bloques[i].posicion.col==posicion_moe->col && bloques[i].tipo==VIGAS){
                posicion_moe_valida=false;
            }
        i++;
        }
    }
}
/*
Pre:Tope_bloques debe ser mayor a 0
Post:Devuelve true si colisiona con algun bloques,homero,moe o la receta, sino devuelve false
*/
bool chequear_posicion_bloque(bloque_t bloques[], int tope_bloques, coordenada_t posicion,juego_t *juego){
    bool colisiona=false;
    int i=0;
    while(i<tope_bloques && !colisiona){
        if(juego->bloques[i].posicion.fil==posicion.fil && juego->bloques[i].posicion.col==posicion.col){
            colisiona=true;
        }
        i++;
    }
    if(posicion.fil==juego->homero.posicion.fil && posicion.col==juego->homero.posicion.col){
        colisiona=true;
    }else if(posicion.fil==juego->posicion_moe.fil && posicion.col==juego->posicion_moe.col){
        colisiona=true;
    }else if(posicion.fil==juego->posicion_receta.fil && posicion.col==juego->posicion_receta.col){
        colisiona=true;
    }
    return colisiona;
}
/*
Pre:Tope_objetos debe ser mayor a 0
Post:Devuelve true si colisiona con algun objeto, sino devuelve false
*/
bool chequear_posicion_objeto(objeto_t objetos[], int tope_objetos,objeto_t otro_objeto[],int tope_otro_objeto, coordenada_t posicion,juego_t *juego){
    bool colisiona=false;
    int i=0;
    int j=0;
    while(i<tope_objetos && !colisiona){
        if(objetos[i].posicion.fil==posicion.fil && objetos[i].posicion.col==posicion.col){
            colisiona=true;
        }
        i++;
    }
    if(posicion.fil==juego->homero.posicion.fil && posicion.col==juego->homero.posicion.col){
        colisiona=true;
    }else if(posicion.fil==juego->posicion_moe.fil && posicion.col==juego->posicion_moe.col){
        colisiona=true;
    }else if(posicion.fil==juego->posicion_receta.fil && posicion.col==juego->posicion_receta.col){
        colisiona=true;
    }
    while(j<tope_otro_objeto && !colisiona){
        if(otro_objeto[j].posicion.fil==posicion.fil && otro_objeto[j].posicion.col==posicion.col){
            colisiona=true;
        }
        j++;
    }
    return colisiona;
}
/*
Pre:Los bloques deben estar inicializados
Post:Devuelve true cuando la posicion de la receta es valida, sino devuelve false
*/
bool chequear_posicion_receta(bloque_t bloques[], int tope_bloques, coordenada_t posicion,coordenada_t posicion_homero,coordenada_t posicion_moe){
    bool colisiona=false;
    bool bloque_encima=false;
    int i=0;
    while(i<tope_bloques && !colisiona){
        if(bloques[i].posicion.fil==posicion.fil && bloques[i].posicion.col==posicion.col){
            colisiona=true;
        }else if(bloques[i].posicion.fil==posicion.fil+1 && bloques[i].posicion.col==posicion.col){
        bloque_encima=true;
        }
        i++;
    }
    if(posicion.fil==posicion_homero.fil && posicion.col==posicion_homero.col){
        colisiona=true;
    }else if(posicion.fil==posicion_moe.fil && posicion.col==posicion_moe.col){
        colisiona=true;
    }
    return !colisiona && !bloque_encima;
}
/*
Pre:Deben estar inicializados moe,homero y los bloques
Post:Devuelve una posicion aleatoria que no sea debajo de un bloque ni que colisione con algun elemento del juego
*/
void inicializar_receta(coordenada_t *posicion_receta,bloque_t bloques[],int tope_bloques,coordenada_t posicion_homero,coordenada_t posicion_moe){
    bool posicion_receta_valida=false;
    while(!posicion_receta_valida){
        *posicion_receta=posicion_aleatoria(MAX_FILAS, MAX_COLUMNAS);
        posicion_receta_valida= chequear_posicion_receta(bloques, tope_bloques,*posicion_receta,posicion_homero,posicion_moe);
    }
}
/*
Pre:El juego debe estar inicializado
Post:Devuelve true si hay un bloque arriba de homero, sino devuelve false
*/
bool hay_bloque_arriba(juego_t juego){
    bool hay_bloque=false;
    int i=0;
    while(i<juego.tope_bloques && !hay_bloque){
        if(juego.bloques[i].posicion.fil==juego.homero.posicion.fil-1 && juego.bloques[i].posicion.col==juego.homero.posicion.col){
            hay_bloque=true;
        }
        i++;
    }
    return hay_bloque;
}
/*
Pre:Tope_bloques debe ser mayor o igual a 0
Post:Inicializa los bloques y aumenta su tope
*/
void generar_bloques(juego_t *juego, int cantidad, char tipo, int resistencia ){
    int i=0;
    do{
        coordenada_t bloque_generado= posicion_aleatoria(MAX_FILAS, MAX_COLUMNAS);
        bool es_viga=false;
        int j=0;
        while(j<juego->tope_bloques && !es_viga){
            if(juego->bloques[j].posicion.fil==bloque_generado.fil && juego->bloques[j].posicion.col==bloque_generado.col && juego->bloques[j].tipo==VIGAS){
                es_viga=true;
            }
            j++;
        }
        if(!chequear_posicion_bloque(juego->bloques, juego->tope_bloques, bloque_generado, juego)){
            juego->bloques[juego->tope_bloques].tipo=tipo;
            juego->bloques[juego->tope_bloques].resistencia=resistencia;
            juego->bloques[juego->tope_bloques].posicion=bloque_generado;
            (juego->tope_bloques)++;
            i++;
            }       
    }while(i<cantidad);
}
/*
Pre:Tope_obstaculos debe ser mayor o igual a 0
Post:Inicializa los obstaculos y aumenta su tope
*/
void generar_obstaculos(juego_t *juego, int cantidad, char tipo ) {
    int i = 0;
    do{
        int bloque_aleatorio=rand()%juego->tope_bloques;
        coordenada_t obstaculo_generado=juego->bloques[bloque_aleatorio].posicion;
        bool es_viga=false;
        int j=0;
        while(j<juego->tope_bloques && !es_viga){
            if(juego->bloques[j].posicion.fil==obstaculo_generado.fil && juego->bloques[j].posicion.col==obstaculo_generado.col && juego->bloques[j].tipo==VIGAS){
                es_viga=true;
            }
            j++;
        }        
        if(!es_viga && !chequear_posicion_objeto(juego->obstaculos, juego->tope_obstaculos,juego->herramientas,juego->tope_herramientas,obstaculo_generado,juego)){
            juego->obstaculos[juego->tope_obstaculos].tipo = tipo;
            juego->obstaculos[juego->tope_obstaculos].posicion = obstaculo_generado;
            (juego->tope_obstaculos)++;
            i++;
        }
    } while(i<cantidad);
}
/*
Pre:Tope_herramientas debe ser mayor o igual a 0
Post:Inicializa las herramientas y aumenta su tope
*/
void generar_herramientas(juego_t *juego, int cantidad, char tipo) {
    int i=0;
    do{
        int bloque_aleatorio=generar_nro_aleatorio(juego->tope_bloques);
        coordenada_t herramienta_generada=juego->bloques[bloque_aleatorio].posicion;
        bool es_viga=false;
        int j=0;
        while(j<juego->tope_bloques && !es_viga){
            if(juego->bloques[j].posicion.fil==herramienta_generada.fil && juego->bloques[j].posicion.col==herramienta_generada.col && juego->bloques[j].tipo==VIGAS){
                es_viga=true;
            }
            j++;
        }
        if (!es_viga && !chequear_posicion_objeto(juego->herramientas, juego->tope_herramientas,juego->obstaculos,juego->tope_obstaculos, herramienta_generada,juego)){
            juego->herramientas[juego->tope_herramientas].tipo = tipo;
            juego->herramientas[juego->tope_herramientas].posicion = herramienta_generada;
            (juego->tope_herramientas)++;
            i++;
        }
    } while(i<cantidad);
}
/*
Pre:La posicion de homero debe ser valida,deben estar inicializadas las herramientas
Post:Homero reacciona segun la herramienta con la que se choca, despues la saca del vector
*/
void chocar_con_herramienta(juego_t *juego){
    int pos_eliminar=0;
    for(int i=0 ;i<juego->tope_herramientas;i++){
        if (juego->homero.posicion.fil==juego->herramientas[i].posicion.fil && juego->homero.posicion.col==juego->herramientas[i].posicion.col){
            pos_eliminar=i;
            switch(juego->herramientas[i].tipo){
                case 'W':
                    printf("Encontraste madera!\n");
                    juego->homero.cantidad_madera ++;
                    break;
                case 'S':
                    printf("Conseguiste un soplete\n");
                    juego->homero.cantidad_sopletes++;
                    break;
            }
        for(int j=pos_eliminar;j<juego->tope_herramientas;j++){
            juego->herramientas[j]=juego->herramientas[j+1];
        }
        juego->tope_herramientas--;
        }
    }
}
/*
Pre:La posicion de homero debe ser valida, deben estar inicializadas los osbtaculos
Post:Homero reacciona segun el obstaculo con el que se choca
*/
void chocar_con_obstaculo(objeto_t obstaculos[], int tope_obstaculos,personaje_t *homero, int *movimientos_restantes){
    for(int i=0;i<tope_obstaculos;i++){
        if(homero->posicion.fil==obstaculos[i].posicion.fil && homero->posicion.col == obstaculos[i].posicion.col){
            switch(obstaculos[i].tipo){
                case 'B':
                    printf("Un murcielago te asusto y perdiste 3 maderas\n");
                    if(homero->cantidad_madera >= CREAR_ESCALERA){
                        homero->cantidad_madera -=RESTAR_MADERA;
                    }else{
                        homero->cantidad_madera=0;
                    }
                    break;
                case 'C':
                    printf("Pisaste un cable y te dio una descarga electrica, perdiste 5 movimientos\n");
                    if(*movimientos_restantes <= 5){
                        *movimientos_restantes=0;
                    }else{
                        *movimientos_restantes -=RESTAR_MOVIMIENTO;
                    }
                    break;
            }
        }
    }
}
/*
Pre:El juego debe estar inicializado
Post:Verifica el movimiento de homero, y reacciona con los objetos que encuentra en el camino, y con la receta
*/
void interactuar_con_entorno(juego_t *juego){
    if (juego->homero.posicion.fil==juego->posicion_receta.fil && juego->homero.posicion.col==juego->posicion_receta.col){
        juego->homero.recolecto_receta=true;
        printf("Homero recupero la receta secreta\n");
        juego->posicion_receta.fil=juego->homero.posicion.fil;
        juego->posicion_receta.col=juego->homero.posicion.col;
    }
    if(juego->homero.recolecto_receta){
        juego->posicion_receta.fil=juego->homero.posicion.fil;
        juego->posicion_receta.col=juego->homero.posicion.col;
    }
    chocar_con_obstaculo(juego->obstaculos,juego->tope_obstaculos,&juego->homero,&juego->movimientos_restantes);
    chocar_con_herramienta(juego);
}
/*
Pre:El juego debe estar inicializado y homero "en el aire"
Post:Homero cae hasta el proximo bloque o el final del mapa
*/
void dejar_caer_homero(juego_t *juego){
    bool en_aire=false;
    int fila_homero=juego->homero.posicion.fil;
    int ultima_fila=MAX_FILAS-1;
    while (!en_aire && fila_homero<ultima_fila){
        int i=0;
        while(i<juego->tope_bloques && !en_aire){
            if(juego->bloques[i].posicion.fil==fila_homero+1 && juego->bloques[i].posicion.col==juego->homero.posicion.col){
                en_aire=true;
            }
            i++;
        }
        if (!en_aire){
            fila_homero++;
            juego->homero.posicion.fil=fila_homero;
            interactuar_con_entorno(juego);
        }
    }
    juego->homero.posicion.fil=fila_homero;
}
/*
Pre:Homero debe tener 3 maderas
Post:Crea la escalera y la agrega a bloques, e incrementa el tope de bloques
*/
void agregar_escalera(juego_t *juego, bloque_t escalera){
    if(juego->tope_bloques<MAX_BLOQUES){
        juego->bloques[juego->tope_bloques]=escalera;
        juego->tope_bloques++;
    }
}
/*
Pre:La resistencia del bloque debe ser 0 para romperlo
Post:Elimina el bloque del vector bloques,disminuye el tope bloques, y si es telaraña la reubica, si es escalera le devuelve 2 maderas a homero
*/
void romper_bloque(juego_t *juego,int pos_eliminar){
    if(juego->bloques[pos_eliminar].tipo==TELARAÑA){
        coordenada_t nueva_telaraña;
        do{
        nueva_telaraña=posicion_aleatoria(MAX_FILAS, MAX_COLUMNAS);
        juego->bloques[pos_eliminar].posicion=nueva_telaraña;
        }while(!chequear_posicion_bloque(juego->bloques,juego->tope_bloques,nueva_telaraña, juego));
        juego->bloques[pos_eliminar].resistencia=RESISTENCIA_TELARAÑA;
    }else if(juego->bloques[pos_eliminar].tipo==ESCALERA){
        for(int j=pos_eliminar;j<juego->tope_bloques -1;j++){
        juego->bloques[j]=juego->bloques[j+1];
        }
        juego->tope_bloques--;
        juego->homero.cantidad_madera+=2;
    }else{
        for(int j=pos_eliminar;j<juego->tope_bloques -1;j++){
        juego->bloques[j]=juego->bloques[j+1];
        }
        juego->tope_bloques--;
    }
    dejar_caer_homero(juego);
   
}
/*
Pre:Debe ser un movimiento valido
Post:Ejecuta el movimiento y reacciona con los elementos del juego en caso de chocar con alguno
*/
void verificar_movimiento(juego_t *juego, int movimiento_fila, int movimiento_columna){
    bool choca = false;
    int i=0;
    bool choca_con_viga=false;
    while(i < juego->tope_bloques && !choca){
        if(juego->bloques[i].posicion.fil == movimiento_fila &&
            juego->bloques[i].posicion.col == movimiento_columna){
            choca = true;
            if(juego->bloques[i].tipo==VIGAS){
                choca_con_viga=true;
                printf("Homero, las vigas no se pueden romper :v\n");
            }else{
                juego->bloques[i].resistencia--;
                if(juego->bloques[i].resistencia == 0){
                    int bloque_roto=i;
                    romper_bloque(juego,bloque_roto);
                }
            }
        }
        i++;
        }
    if(!choca){
        juego->homero.posicion.fil = movimiento_fila;
        juego->homero.posicion.col = movimiento_columna;
        interactuar_con_entorno(juego);
        dejar_caer_homero(juego);
    }
    if(!choca_con_viga && juego->movimientos_restantes>0){
        juego->movimientos_restantes--;
    }
}
/*
Pre:Debe tener mas de 3 maderas y en la posicion arriba de homero no debe haber nada con lo que choque
Post:Crea una escalera
*/
void usar_escalera(juego_t *juego){
            juego->homero.cantidad_madera-=RESTAR_MADERA;
            bloque_t escalera;
            escalera.posicion=juego->homero.posicion;
            escalera.tipo=ESCALERA;
            escalera.resistencia=RESISTENCIA_ESCALERA;
            agregar_escalera(juego, escalera);
}
/*
Pre:Homero debe tener al menos 1 soplete
Post:Elimina todos los bloques de tierra y telarañas a 2 casillas de distancia de homero,las telarañas las reubica,las vigas no las rompe
*/
void usar_soplete(juego_t *juego){
    int fila_homero=juego->homero.posicion.fil;
    int col_homero=juego->homero.posicion.col;
    int tope_bloques_no_rotos=0;
    for(int i=0; i<juego->tope_bloques;i++){
        int soplete_fila=abs(fila_homero - juego->bloques[i].posicion.fil);
        int soplete_col=abs(col_homero - juego->bloques[i].posicion.col);
        if((soplete_fila + soplete_col)<=DISTANCIA_SOPLETE){
            if(juego->bloques[i].tipo==TELARAÑA){
                coordenada_t nueva_telaraña;
                do{
                    nueva_telaraña=posicion_aleatoria(MAX_FILAS, MAX_COLUMNAS);
                    juego->bloques[i].posicion=nueva_telaraña;
                }while(!chequear_posicion_bloque(juego->bloques,juego->tope_bloques,nueva_telaraña, juego));
                juego->bloques[i].resistencia=RESISTENCIA_TELARAÑA;
                juego->bloques[tope_bloques_no_rotos]=juego->bloques[i];
                tope_bloques_no_rotos++;
            }else if(juego->bloques[i].tipo==VIGAS){
                printf("Homero, el soplete tampoco puede con las vigas :v\n");
                juego->bloques[tope_bloques_no_rotos]=juego->bloques[i];
                tope_bloques_no_rotos++;
            }else if(juego->bloques[i].tipo==ESCALERA){
                juego->homero.cantidad_madera+=RECUPERA_MADERAS;
            }
        }else{
            juego->bloques[tope_bloques_no_rotos]=juego->bloques[i];
            tope_bloques_no_rotos++;
        }
    }
    juego->tope_bloques = tope_bloques_no_rotos;
}
/*
Pre:Bloques debe estar inicialzado
Post:Ordena los bloques por fila y columna de menor a mayor
*/
void ordenar_bloques(bloque_t bloques[], int tope_bloques) {
    for (int i = 0; i < tope_bloques - 1; i++) {
        for (int j = 0; j < tope_bloques - i - 1; j++) {
            if (bloques[j].posicion.fil > bloques[j + 1].posicion.fil ||
                (bloques[j].posicion.fil == bloques[j + 1].posicion.fil &&
                 bloques[j].posicion.col > bloques[j + 1].posicion.col)) {
                bloque_t auxiliar = bloques[j];
                bloques[j] = bloques[j + 1];
                bloques[j + 1] = auxiliar;
            }
        }
    }
}
/*
Pre:El juego debe estar inicializado correctamente
Post:Devuelve un emoji segun el tipo de elemento
*/
const char *cambiar_a_emojis(char tipo){
    switch(tipo){
        case HOMERO:
            return EMOJI_HOMERO;
        case MOE:
            return EMOJI_MOE;
        case RECETA:
            return EMOJI_RECETA;
        case TIERRA:
            return EMOJI_TIERRA;
        case TELARAÑA:
            return EMOJI_TELARAÑA;
        case VIGAS:
            return EMOJI_VIGAS;
        case ESCALERA:
            return EMOJI_ESCALERA;
        case MURCIELAGOS:
            return EMOJI_MURCIELAGOS;
        case CABLES:
            return EMOJI_CABLES;
        case MADERA:
            return EMOJI_MADERA;
        case SOPLETE:
            return EMOJI_SOPLETE;
        default:
            return " ";
    }
}
void inicializar_juego(juego_t *juego){
    juego->tope_bloques=0;
    juego->tope_herramientas=0;
    juego->tope_obstaculos=0;
    inicializar_homero(&juego->homero,&juego->movimientos_restantes);
    inicializar_vigas(juego->bloques,&juego->tope_bloques);
    inicializar_moe(&juego->posicion_moe,juego->bloques,juego->tope_bloques);
    generar_bloques(juego,MAX_TIERRA,TIERRA,RESISTENCIA_TIERRA);
    generar_bloques(juego,MAX_TELARAÑAS,TELARAÑA,RESISTENCIA_TELARAÑA);
    generar_obstaculos(juego,MAX_CABLE,CABLES);
    generar_obstaculos(juego,MAX_MURCIELAGOS,MURCIELAGOS);
    generar_herramientas(juego,MAX_MADERAS,MADERA);
    generar_herramientas(juego,MAX_SOPLETES,SOPLETE);
    inicializar_receta( &juego->posicion_receta,juego->bloques,juego->tope_bloques,juego->homero.posicion,juego->posicion_moe);
    ordenar_bloques(juego->bloques, juego->tope_bloques);
}
void realizar_jugada(juego_t *juego, char movimiento){
    switch (movimiento){
    case 'D':
    case 'd':
        if(juego->homero.posicion.col >= MAX_COLUMNAS - 1){
            printf("Homero intenta salir del mapa\n");
        }else{
            int mov_derecha_col = juego->homero.posicion.col +1;
            int mov_derecha_fil = juego->homero.posicion.fil;
            verificar_movimiento(juego, mov_derecha_fil, mov_derecha_col);   
        }
        break;

    case 'A':
    case 'a':
        if(juego->homero.posicion.col <= 0){
            printf("Homero intenta salir del mapa\n");   
        }else{
            int mov_izquierda_col = juego->homero.posicion.col -1;
            int mov_izquierda_fil = juego->homero.posicion.fil;
            verificar_movimiento(juego, mov_izquierda_fil,mov_izquierda_col);
        }
        break;
    case 'S':
    case 's':
        if(juego->homero.posicion.fil>=MAX_FILAS-1){
            printf("Homero intenta salir del mapa\n");
        }else{
            int mov_abajo_fil=juego->homero.posicion.fil+1;
            int mov_abajo_col = juego->homero.posicion.col;
            verificar_movimiento(juego, mov_abajo_fil,mov_abajo_col);
        }
        break;
    case 'O':
    case 'o':
        if(juego->homero.cantidad_sopletes>0){
            usar_soplete(juego);
            dejar_caer_homero(juego);
            juego->homero.cantidad_sopletes--;
        }else{
            printf("No tenes sopletes para usar\n");
        }
        break;
    case  'E':
    case 'e':
        if(juego->homero.cantidad_madera>=CREAR_ESCALERA){
            if(juego->homero.posicion.fil <=0){
                printf("Intestas salirte del mapa\n");
            }else if(!hay_bloque_arriba(*juego)){
                usar_escalera(juego);
                juego->homero.posicion.fil--;
                interactuar_con_entorno(juego);
            }else{
                printf("No puedes poner una escalera si hay un bloque arriba\n");
            }
        }else{
            printf("No hay madera para hacer la escalera");
        }
        break;
    }
}
void mostrar_juego(juego_t juego){
    const char *tablero[MAX_FILAS][MAX_COLUMNAS];
    for(int i=0; i<MAX_FILAS;i++){
        for(int j=0;j<MAX_COLUMNAS; j++){
            tablero[i][j] = "  " ;
        }
    }
    for(int i=0; i<juego.tope_obstaculos; i++){
        tablero[juego.obstaculos[i].posicion.fil][juego.obstaculos[i].posicion.col] = cambiar_a_emojis(juego.obstaculos[i].tipo);
    }
    for(int i=0; i<juego.tope_herramientas; i++){
        tablero[juego.herramientas[i].posicion.fil][juego.herramientas[i].posicion.col] = cambiar_a_emojis(juego.herramientas[i].tipo);
    }
    for(int i=0; i<juego.tope_bloques; i++){
        tablero[juego.bloques[i].posicion.fil][juego.bloques[i].posicion.col] = cambiar_a_emojis(juego.bloques[i].tipo);
    }
    tablero[juego.posicion_moe.fil][juego.posicion_moe.col]=EMOJI_MOE;
    tablero[juego.posicion_receta.fil][juego.posicion_receta.col]=EMOJI_RECETA;
    tablero[juego.homero.posicion.fil][juego.homero.posicion.col]=EMOJI_HOMERO;

    recordar(juego);
    for(int i=0; i<MAX_FILAS;i++){
        printf("\n");
        for(int j=0;j<MAX_COLUMNAS; j++){
            printf("\033[48;5;52m|%s|\033[0m",tablero[i][j]);
        }
    }
    printf("\n");

}
int estado_juego (juego_t juego){
    if(juego.homero.recolecto_receta && (juego.homero.posicion.fil==juego.posicion_moe.fil && juego.homero.posicion.col==juego.posicion_moe.col)){
        return 1;
    }
    if(juego.movimientos_restantes==0){
        return -1;
    }
    return 0;
}