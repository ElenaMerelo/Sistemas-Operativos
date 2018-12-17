# Módulo I. Administración de Linux.
## Sesión 3. Monitorización del sistema.
> Orden uptime
Muestra una línea con la siguiente información: la hora actual, el tiempo que lleva en marcha el
sistema, el número de usuarios conectados, y la carga media del sistema en los últimos 1, 5 y 15
minutos (es la misma información que devuelve la ejecución de la orden w, la cual muestra los
usuarios conectados y lo que están haciendo). La carga del sistema es el número de procesos en
la cola de ejecución del núcleo. Un sistema con un comportamiento normal debe mostrar una
carga igual o inferior a 1 4 , aunque se deben tener en cuenta la configuración y el tipo de
programas en ejecución

#### Actividad 3.1. Consulta de estadísticas del sistema
Responde a las siguientes cuestiones y especifica, para cada una, la opción que has utilizado
(para ello utiliza man y consulta las opciones de las órdenes anteriormente vistas:
a) ¿Cuánto tiempo lleva en marcha el sistema? 11 horas 24 minutos. Lo he visto poniendo `uptime` en la línea de órdenes sale una hora en la primera columna y luego up y el tiempo que lleva en marcha el sistema.
b) ¿Cuántos usuarios hay trabajando? Un usuario, también se ve con `uptime` tras la primera coma.
c) ¿Cuál es la carga media del sistema en los últimos 15 minutos? 0.00. Se ve con `w`, en la parte de `load average` te dice la carga media del sistema en los últimos uno, cinco y quince minutos.

##### Orden time
Mide el tiempo de ejecución de un programa y muestra un resumen del uso de los recursos del
sistema. Muestra el tiempo total que el programa ha estado ejecutándose (real), el tiempo que se
ha ejecutado en modo usuario (user) y el tiempo que se ha ejecutado en modo supervisor (sys),
de tal forma que se puede determinar que el tiempo de espera de un proceso es:
T_espera = real – user - sys

##### Órdenes nice y renice
Linux realiza una planificación por prioridades. Por defecto, un proceso hereda la prioridad de su
proceso padre. Se puede establecer el valor de prioridad de un proceso a un valor distinto del
que tendría por defecto mediante la orden nice. El rango de valores que permite establecer
como parámetro la orden nice es [-20,19]. Asignar un valor negativo aumenta la posibilidad de
ejecución de un proceso y solamente puede hacerlo el usuario root. Los usuarios sin privilegios
de administración solo pueden utilizar los valores positivos de este rango, desfavoreciendo así la
posibilidad de ejecución de un proceso. Ejemplos:
`#> nice -5 konqueror # Aumenta en 5 el valor de prioridad de la ejecución de konqueror`
`#> nice −−10 konqueror # Decrementa en 10 el valor de prioridad (sólo usuario root)`
La orden renice permite alterar el valor de prioridad de uno o más procesos en ejecución.
`#> renice 14 890 # Aumenta en 14 el valor de prioridad del proceso 890`

#### Actividad 3.2. Prioridad de los procesos
a) Crea un script o guión shell que realice un ciclo de un número variable de iteraciones en el
que se hagan dos cosas: una operación aritmética y el incremento de una variable. Cuando
terminen las iteraciones escribirá en pantalla un mensaje indicando el valor actual de la variable.
Este guión debe tener un argumento que es el número de iteraciones que va a realizar. Por
ejemplo, si el script se llama prueba_procesos, ejecutaríamos:
~~~
# prueba_procesos 1000
el valor de la variable es 1000
~~~
b) Ejecuta el guión anterior varias veces en background (segundo plano) y comprueba su
prioridad inicial. Cambia la prioridad de dos de ellos, a uno se la aumentas y a otro se la
disminuyes, ¿cómo se comporta el sistema para estos procesos? Para ello tendríamos que hacer:
`> ./prueba_procesos.sh 100 &` dos veces, y según el número de proceso que les sean asignados, digamos 1 y 2, entonces hacemos `renice 20 1` para aumentarle la prioridad a 20 (con lo que es menos prioritario) y `renice -5 2`, que podemos hacer si estamos en modo superusuario, al darle una prioridad grande.

> Orden pstree. Visualiza un árbol de procesos en ejecución. Tiene una serie de opciones, algunas de ellas son:
+ -a Muestra los argumentos de la línea de órdenes.
+ -A Usa caracteres ASCII para dibujar el árbol.
+ -G Usa los caracteres de VT100.
+ -h Resaltar el proceso actual y sus antepasados.
+ -H Igual que -h, pero para el proceso que se especifique.
+ -l Usa un formato largo, por defecto las líneas se truncan.
+ -n Ordena los procesos por el PID de su antecesor en vez de por el nombre (ordenación
numérica)
+ -p Desactiva el mostrar los PIDs entre paréntesis después del nombre de cada proceso.
+ -u Si el uid de un proceso difiere del uid de su padre, el nuevo uid se pone entre paréntesis después del nombre del proceso.
+ -V Visualiza información sobre la versión.
+ -Z (SELinux) Muestra el contexto de seguridad para cada proceso.

##### Orden ps
Esta orden se implementa usando el pseudo-sistema de archivos /proc. Muestra información
sobre los procesos en ejecución:
+ USER : usuario que lanzó el programa
+ PID : identificador del proceso
+ PPID : identificador del proceso padre
+ %CPU : porcentaje entre el tiempo usado realmente y el que lleva en ejecución
+ %MEM : fracción de memoria consumida (es una estimación)
+ VSZ : tamaño virtual del proceso (código+datos+pila) en KB
+ RSS : memoria real usada en KB
+ TTY : terminal asociado con el proceso
+ STAT : estado del proceso que puede ser:
      ++ R: en ejecución o listo (Running o Ready)
      ++ S: durmiendo (Sleeping)
      ++ T: parado (sTopped)
      ++ Z: proceso Zombie
      ++ D: durmiendo ininterrumpible (normalmente E/S)
      ++ N: prioridad baja (> 0)
      ++ <: prioridad alta (< 0)
      ++ s: líder de sesión
      ++ l: tiene multi-thread
      ++ +: proceso foreground
      ++ L: páginas bloqueadas en memoria
La orden ps normalmente se ejecuta con las operaciones -ef, ya que “e” significa que se
seleccione a todo proceso que esté en el sistema y “f” que se muestre información completa
(aunque con la opción “l” se imprime en pantalla más información). Sin argumentos muestra los
procesos lanzados por el usuario que ejecuta esta orden.
















>
