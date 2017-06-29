/*******************************************************************************
*
*   ejecutivociclico.c: Planificacion con ejecutivo ciclico
*
* 	Autor: J.A. Holgado
*	Ultima modificacion: 20/12/2014
*	Plataforma: POSIX
*	Asignatura: Sistemas Concurrentes y Distribuido
*	Departamento: Lenguajes y Sistemas Informaticos
* 	Universidad de Granada
*
* 	compilacion: gcc ejecutivociclico.c utilRT.c -o ejecutivo
*
*   Nota: En linux es necesario enlazar la libreria POSIX al final a�adiendo -lrt
*
*******************************************************************************/


#include <time.h>		//para  clock_gettime()
#include <stdio.h>		//para printf()
#include <errno.h>		//para perror
#include "utilRT.h"


// Se define una variable global que marca la referencia de tiempo:
// es el instante de tiempo absoluto (tiempo desde 1 de enero de 1970)
// en el cual comienza la primera iteración del ciclo principal (inicio de la
// simulación)
struct timespec origenTiempo;

///////////////////////////////////////////////////////////////
//Definicion de tareas
// Nota: Si las tareas tuvieran el mismo comportamiento se pueden parametrizar
void TareaA(void);
void TareaB(void);
void TareaC(void);
void TareaD(void);


// *****************************************************************************
//
// Programa principal

int main(int argc, char **argv)
{
	//Parametros relacionados con el ejecutivo ciclico
	struct timespec activacionTiempo;  //define el nuevo instante de activacion
	struct timespec cicloMarco;  //define el intervalo de tiempo

	//Parametros relacionados con el ejecutivo ciclico
	int nciclos; //Numero de ciclos secundarios respecto al marco primario ---Modificar----
	int marco;   //especifica el ciclo secundario actual
	marco = 0;   //se inicializa el marco, que varia entre 0 y nCiclos-1


	//Establecer el numero de ciclos secundarios nciclos, asi como la inicializacion el marco
	nciclos = 4;

	/////////////////////////////////////////////////////////////////////////////////
	//Etapa 1: Inicializacion y Preparacion del Ejecutivo Ciclico
	//
	/////////////////////////////////////////////////////////////////////////////////


	//Establecer el Ciclo Secundario (--- usar getTimespec ---)
	cicloMarco = getTimespec(5);

	//Calcula la marca de tiempo para determinar el instante de referencia
	clock_gettime(CLOCK_REALTIME, &origenTiempo);

    //A partir de este instante, determina la siguiente activacion del ejecutivo ciclico ( --- usar sumaTiempos ---)
	activacionTiempo = sumaTiempos(&origenTiempo, &cicloMarco);	

	/////////////////////////////////////////////////////////////////////////////////
	//Etapa 2: Implementacion del ejecutivo ciclico
	/////////////////////////////////////////////////////////////////////
	// Aqui esta el planificador ciclico
	 while (1) {
	     switch(marco)
	     {
		       case 0: TareaA(); TareaB(); TareaC(); break;
		       case 1: TareaA(); TareaB(); TareaD(); break;
			   case 2: TareaA(); TareaB(); TareaC(); break;
			   case 3: TareaA(); TareaB(); break;
	     }
	     marco = (marco +1)% nciclos;
	
		 //suspender hasta nueva activacion utiiizando retardos absolutos  (--- usar clock_nanosleep ---) y definir la activacion del siguiente ciclo
		 clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &activacionTiempo, NULL);
		 activacionTiempo = sumaTiempos(&activacionTiempo, &cicloMarco);
		 
	 }
	/////////////////////////////////////////////////////////////////////////////////

  return 0;
}

// *****************************************************************************
//
// TareaA

void TareaA(void)
{
	// Variables de tiempo
	struct timespec
		tInicio,     // tiempo de inicio de esta ejecución de la tarea (relativo a 'origenTiempo')
		tFinal,      // tiempo final de esta ejecución de la tarea (relativo a 'origenTiempo')
		tActivacion, // instante absoluto en el cual la tarea debe de despertarse:
		             //    (se debe calcular sumando el instante absoluto de inicio y la duración (tComputo))
		tComputo,    // duración del intervalo en el que se duerme la hebra, simulando hacer algún trabajo útil.
		             //    (se debe de construir usando el valor de C expresado en segundos)
		tiempo;      // variable temporal usada en los cálculos
	static int
	  i = 0 ;      // número de la activación actual de la tarea

	//Establece el tiempo de computo tComputo (--- usar getTimespec ---)
	tComputo = getTimespec(1);

	//Marca el tiempo de inicio
	clock_gettime(CLOCK_REALTIME,&tiempo);
	tInicio = restaTiempos(&tiempo,&origenTiempo);
	printf("Tarea A: activacion = %d, tiempo activacion = %.6f seg\n",
		i,getSeconds(&tInicio));

	//Simula el consumo con otro retardo absoluto o relativo  (--- usar clock_nanosleep ---)
	tActivacion = sumaTiempos(&tiempo, &tComputo);
	clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &tActivacion, NULL);	

	//Marca el tiempo de fin
	clock_gettime(CLOCK_REALTIME,&tiempo);
	tFinal = restaTiempos(&tiempo,&origenTiempo);
	printf("Tarea A: activacion = %d, tiempo finalizacion = %.6f seg\n",
		i,getSeconds(&tFinal));

	// incrementa contador de activaciones
	i++;
}

// *****************************************************************************
//
// TareaB

void TareaB(void)
{
	// Variables de tiempo
	struct timespec
		tInicio,     // tiempo de inicio de esta ejecución de la tarea (relativo a 'origenTiempo')
		tFinal,      // tiempo final de esta ejecución de la tarea (relativo a 'origenTiempo')
		tActivacion, // instante absoluto en el cual la tarea debe de despertarse:
								 //    (se debe calcular sumando el instante absoluto de inicio y la duración (tComputo))
		tComputo,    // duración del intervalo en el que se duerme la hebra, simulando hacer algún trabajo útil.
								 //    (se debe de construir usando el valor de C expresado en segundos)
		tiempo;      // variable temporal usada en los cálculos
	static int
		i = 0 ;      // número de la activación actual de la tarea

	//Establece el tiempo de computo tComputo
	tComputo = getTimespec(1.5);

	//Marca el tiempo de inicio
	clock_gettime(CLOCK_REALTIME,&tiempo);
	tInicio = restaTiempos(&tiempo,&origenTiempo);
	printf("Tarea B: activacion = %d, tiempo activacion = %.6f seg\n",
		i,getSeconds(&tInicio));

	//Simula el consumo con otro retardo absoluto o relativo
	tActivacion = sumaTiempos(&tiempo, &tComputo);
	clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &tActivacion, NULL);	

	//Marca el tiempo de fin
	clock_gettime(CLOCK_REALTIME,&tiempo);
	tFinal = restaTiempos(&tiempo,&origenTiempo);
	printf("Tarea B: activacion = %d, tiempo finalizacion = %.6f seg\n",
		i,getSeconds(&tFinal));

	// incrementa contador de activaciones
	i++;
}

// *****************************************************************************
//
// TareaC

void TareaC(void)
{
	// Variables de tiempo
	struct timespec
		tInicio,     // tiempo de inicio de esta ejecución de la tarea (relativo a 'origenTiempo')
		tFinal,      // tiempo final de esta ejecución de la tarea (relativo a 'origenTiempo')
		tActivacion, // instante absoluto en el cual la tarea debe de despertarse:
								 //    (se debe calcular sumando el instante absoluto de inicio y la duración (tComputo))
		tComputo,    // duración del intervalo en el que se duerme la hebra, simulando hacer algún trabajo útil.
								 //    (se debe de construir usando el valor de C expresado en segundos)
		tiempo;      // variable temporal usada en los cálculos
	static int
		i = 0 ;      // número de la activación actual de la tarea

	//Establece el tiempo de computo tComputo
	tComputo = getTimespec(2);

	//Marca el tiempo de inicio
	clock_gettime(CLOCK_REALTIME,&tiempo);
	tInicio = restaTiempos(&tiempo,&origenTiempo);
	printf("Tarea C: activacion = %d, tiempo activacion = %.6f seg\n",
		i,getSeconds(&tInicio));

	//Simula el consumo con otro retardo absoluto o relativo
	tActivacion = sumaTiempos(&tiempo, &tComputo);
	clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &tActivacion, NULL);	


	//Marca el tiempo de fin
	clock_gettime(CLOCK_REALTIME,&tiempo);
	tFinal = restaTiempos(&tiempo,&origenTiempo);
	printf("Tarea C: activacion = %d, tiempo finalizacion = %.6f seg\n",
		i,getSeconds(&tFinal));

	// incrementa contador de activaciones
	i++;
}

// *****************************************************************************
//
// TareaD

void TareaD(void)
{
	// Variables de tiempo
	struct timespec
		tInicio,     // tiempo de inicio de esta ejecución de la tarea (relativo a 'origenTiempo')
		tFinal,      // tiempo final de esta ejecución de la tarea (relativo a 'origenTiempo')
		tActivacion, // instante absoluto en el cual la tarea debe de despertarse:
								 //    (se debe calcular sumando el instante absoluto de inicio y la duración (tComputo))
		tComputo,    // duración del intervalo en el que se duerme la hebra, simulando hacer algún trabajo útil.
								 //    (se debe de construir usando el valor de C expresado en segundos)
		tiempo;      // variable temporal usada en los cálculos
	static int
		i = 0 ;      // número de la activación actual de la tarea

	//Establece el tiempo de computo tComputo
	tComputo = getTimespec(2.4);	

	//Marca el tiempo de inicio
	clock_gettime(CLOCK_REALTIME,&tiempo);
	tInicio = restaTiempos(&tiempo,&origenTiempo);
	printf("Tarea D: activacion = %d, tiempo activacion = %.6f seg\n",
		i,getSeconds(&tInicio));

	//Simula el consumo con otro retardo absoluto o relativo
	tActivacion = sumaTiempos(&tiempo, &tComputo);
	clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &tActivacion, NULL);	


	//Marca el tiempo de fin
	clock_gettime(CLOCK_REALTIME,&tiempo);
	tFinal = restaTiempos(&tiempo,&origenTiempo);
	printf("Tarea D: activacion = %d, tiempo finalizacion = %.6f seg\n",
		i,getSeconds(&tFinal));

	// incrementa contador de activaciones
	i++;
}

//RESPUESTA:
/* Al modificar el tiempo de cómputo de D, obtenemos que ya no termina en su correspondiente ciclo secundario, si
no que se come parte del siguiente. En consecuencia A empieza un poco más tarde, sin embargo al utilizar retardos
absolutos, hacemos que aún así el siguiente ciclo acabe dentro de los límites correspondientes.
*/
