# scheduling Grupo N°24
## Integrantes
 - Ariel Agustin Alvaro `ariel.alvaro@mi.unc.edu.ar`
 - Andres David Doctors `andresdaviddoctors@mi.unc.edu.ar`
 - Nicolas Alejandro Greiff `alejandro.greiff@mi.unc.edu.ar`
## Introduccion
Al implementar mybash tuvimos como objetivo:  
- Investigar sobre la **política de planificación** de xv6
- Analizar el benchmark (comparativas de rendimiento) sobre diferentes procesos para ver la respuesta de entrada/salida y de poder de cómputo 
- Implementar la política de planificación MLFQ (Multilevel Feedback Queue Scheduling)

## Modularizacion 
Seguimos la modularización propuesta por xv6
## Tecnicas de programacion
Se respeta el las convenciones que tiene XV6
## Herramientas de programación
Usamos editores de texto para el desarrollo del código (Visual Studio, Notepad++,etc), para la compilación xv6 nos proporciona un MakeFile para compilar atraves de gcc (GNU Compiler Collection). Para el debugging utilizamos impresiones por pantalla de las variables y GDB(GNU debugger)

## Desarrollo (Actividades) 

### Parte 1 :

XV6 Selecciona un proceso si tiene el estado RUNNABLE. Esto se puede ver en la funcion scheduler() en proc.c y en trap.c se ve que cada cierto tiempo se fuerza a los procesos a devolver la CPU, lo que implica junto al punto anterior que xv6 usa RR (si no fuese por esto xv6 podría estar usando la política FIFO, por ejemplo)



En lapic.c se definen los macros de los registros y TICR tiene la siguiente descripción: "Timer initial Count" donde en la función lapicinit() nos indica que el timer se va decrementando con la frecuencia del bus (que es de 1,000,000,000 revoluciones/segundo) y cuando llega a 0 se produce una interrupción, después se escribe en TICR el valor 10,000,000 (100 veces menos que la frecuencia del bus), por lo que se produce una interrupción 100 veces por segundo, lo que nos da un quantum de 0.01 segs = 10 ms. Para cambiar el quantum basta con reemplazar el valor escrito en TICR.


### Parte 2 :
Graficos:
[CPU RR](https://imgur.com/H5w9G3I)
[I/O RR](https://imgur.com/sVR2Ojg)


### Parte 4 :
Graficos:
[CPU MLFQ](https://imgur.com/FgeQjTJ)
[I/O MLFQ](https://imgur.com/tVtu1Tk)

¿Se puede producir *starvation* en el nuevo planificador? Justifique su respuesta.

Sí, si siempre está disponible algún proceso de alta prioridad con estado RUNNABLE nunca llegan a ejecutarse los procesos de baja prioridad.


Además de los objetivos planteados en el enunciado se agrego también el programa de usuario 'analysis' que ejecuta simultaneamente varias instancias de cpubranch y iobranch (con ligeras modificaciones), así como también las salidas por pantalla de llamar al programa con quantums 1, 10, 100 y 1000 veces mas cortos con el scheduler MLFQ y RR (úbicados en quentumMLFQ y quentumRR respectivamente).
