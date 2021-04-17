# SOR-Semáforos-1S-2021
## Trabajo Práctico Semáforos primer semestre año 2021
***
En la cocina de Subway Argento compiten cuatro equipos de cocineros
para ganarse el puesto de trabajo en el local. ¿Quién será el primero en
entregar 1 sándwich de milanesa terminado, de 15 cm, al dueño del
local?
La dificultad de la competencia es que la cocina no cuenta con el
equipamiento individual para cada equipo, sino que algunos de ellos son
compartidos. Se tienen que ordenar para cocinar las milanesas ya que
solo tienen un sartén para todos los equipos y este cocina una milanesa
a la vez, existe solo un horno que cocina dos panes a la vez y un solo
salero.

¿Cómo se arma un sándwich de milanesa?
Para armar un sándwich necesito cortar 2 dientes de ajo y
un puñado de perejil, para luego mezclarlos con 1 huevo.
Después utilizar sal a gusto, agregar la carne y luego empanar
las milanesas. Una vez que tengo todo armado se cocinan por 5’
en el sartén bien caliente.
Como corresponde una buena milanesa tiene panes recién
horneados (por 10’ aproximadamente) junto a lechuga fresca,
tomate, cebolla morada y pepino recién cortados.
Finalmente puedo armar el sándwich con todos los ingredientes.
***
-----------------------------------------------------------------------
### Implementacion
-------------
para simular la competencia entre 4 equipos, se utiliza la implementacion de 4 hilos 
los cuales se ejecutaran paralelamente siendo el scheduler el encargado de decidir el orden
de ejecucuion de los mismos. Por otro lado cada equipo tiene una serie de tareas por realizar
las cuales se simulan atraves de las distintas funciones del programa. 
No todas las funciones se pueden ejecutar al mismo tiempo ya que algunas dependen de la terminacion de la anterior
ej. no se puede cocinar la milanesa si no fue empanada previamente.
por lo cual se utilizaran semaforos para controlar el acceso de los diferentes hilos a las secciones
criticas de cada tarea.

------------
### Pseudocodigo
-----------

### funciones:
---------
ejecutarReceta()
cortar()
mezclar()
salar()
empanar()
cocinar()
hornar()
preparar verduras()

### semaforos:
--------
sem_cortar=1
sem_mezclar=0
sem_salar=0
sem_salarAux=1
sem_empanar=0
sem_cocinar=0
sem_cocinarAux=1
sem_hornearAux=2
sem_prepararVerduras=1

main()
    hiloEquipo1(ejecutarReceta())
    hiloEquipo2(ejecutarReceta())
    hiloEquipo3(ejecutarReceta())
    hiloEquipo4(ejecutarReceta())

ejecutarReceta()
    hilo1(cortar())
    hilo2(mezclar())
    hilo3(salar())
    hilo4(empanar())
    hilo1(cocinar())
    hilo2(hornear())
    hilo3(prepararVerduras())

cortar()
    P(sem_cortar)
    //codigo
    V(sem_mezclar)

mezclar()
    P(sem_mezclar)
    //codigo
    V(sem_salar)

salar()
    P(sem_salar)
    P(sem_salarAux)
    //codigo
    V(sem_salarAux)
    V(sem_empanar)

empanar()
    P(sem_empanar)
    //codigo
    V(sem_cocinar)

cocinar()
    P(sem_cocinar)
    p(sem_cocinarAux)
    //codigo
    V(sem_cocinarAux)

cocinar()
    p(sem_hornearAux)
    //codigo
    V(sem_hornearAux)

prepararVerduras()
    p(sem_prepararVerduras)
    //codigo
    V(sem_prepararVerduras)

-----------------------------------------------
### Orden de ejecucion por cada hiloEquipo
-----------------------------------------------
                        
sem_cortar               |   1   |   0    |   0    |   0   |   0   |   0   |   0
sem_mezclar              |   0   |   1    |   0    |   0   |   0   |   0   |   0
sem_salar                |   0   |   0    |   1    |   0   |   0   |   0   |   0
sem_empanar              |   0   |   0    |   0    |   1   |   0   |   0   |   0
sem_cocinar              |   0   |   0    |   0    |   0   |   1   |   0   |   0
sem_salarAux             |   1   |   1    |   1    |   0   |   1   |   1   |   1
sem_cocinarAux           |   1   |   1    |   1    |   1   |   1   |   0   |   1
sem_hornearAux           |   2   |   1    |   2    |   2   |   2   |   2   |   2
sem_prepararVerduras     |   1   |   0    |   0    |   0   |   0   |   0   |   0
                                  cortar  | mezclar| salar |empanar|cocinar
                                  prepVerd|
                                  hornear |

Tras este analisis podemos evaluar, con los semaforos declarados, cual es el orden de ejecucuion de cada hiloEquipo.
Generamos que tareas que dependan de la terminacion de otras no se ejecuten simultaneamente.
Por otro lado los semaforos con terminacion aux permiten que varios hilosEquipos NO accedan al mismo tiempo a la 
seccion critica de las funciones salar, hornear y cocinar, ya que en terminos del ejercicio planteado 
solo se cuenta con un salero, una sarten y se puede hornear hasta dos panes en el desarrollo de la competencia.

-----------------------------------------------------------------------------------------------------------------------------
Dificultades encontradas:
Dentro de las dificultades podemos destacar que en las consignas se debia leer un archivo con la receta de la competencia
para luego interactuar con la misma durante el programa. El hecho de leer un archivo no presento mayores dificultades, 
sino realizar el algoritmo adecuado para incorporar los datos dentro de una matriz.

-----------------------------------------------------------------------------------------------------------------------------




