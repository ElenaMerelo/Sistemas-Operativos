# Módulo I. Administración de Linux.
## Sesión 3. Monitorización del sistema.
> Orden uptime
Muestra una línea con la siguiente información: la hora actual, el tiempo que lleva en marcha el
sistema, el número de usuarios conectados, y la carga media del sistema en los últimos 1, 5 y 15
minutos (es la misma información que devuelve la ejecución de la orden w, la cual muestra los
usuarios conectados y lo que están haciendo). La carga del sistema es el número de procesos en
la cola de ejecución del núcleo. Un sistema con un comportamiento normal debe mostrar una
carga igual o inferior a 1, aunque se deben tener en cuenta la configuración y el tipo de
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

#### Actividad 3.3. Jerarquía e información de procesos
a) La orden pstree muestra el árbol de procesos que hay en ejecución. Comprueba que la
jerarquía mostrada es correcta haciendo uso de la orden ps y de los valores “PID" y “PPID" de
cada proceso. Para mostrar solo los PID Y PPID de los procesos en ejecución hacemos `ps -o pid,ppid`, y sería cuestión ya de ver que al especificar `pstree -p` hay algún proceso del árbol con los pids resultantes de ejecutar la primera orden.
b) Ejecuta la orden ps con la opción -A, ¿qué significa que un proceso tenga un carácter “?” en
la columna etiquetada como TTY? Que es un demonio, por lo que no tiene asociado terminal.

##### Orden top
Esta orden proporciona una visión continuada de la actividad del procesador en tiempo real,
muestra las tareas que más uso hacen de la CPU, y tiene una interfaz interactiva para manipular
procesos.
Las cinco primeras líneas muestran información general del sistema:
+ las estadísticas de la orden uptime
+ estadísticas sobre los procesos del sistema (número de procesos, procesos en
ejecución, durmiendo, parados o zombies)
+ el estado actual de la CPU (porcentaje en uso por usuarios, por el sistema, por
procesos con valor nice positivo, por procesos esperando E/S, CPU desocupada,
tratando interrupciones hardware o software, en espera involuntaria por
virtualización)
+ la memoria (memoria total disponible, usada, libre, cantidad usada en buffers y en
memoria caché de página)
+ el espacio de swap (swap total disponible, usada y libre)

Los datos de la parte inferior son similares a los del ps, excepto SHR que muestra la
cantidad de memoria compartida usada por la tarea.
Ordena los procesos mostrados en orden decreciente en base al uso de la CPU.
La lista se actualiza de forma interactiva, normalmente cada 5 segundos.
La orden top permite realizar una serie de acciones sobre los procesos de forma interactiva,
como por ejemplo:
+ Cambiar la prioridad de alguno utilizando la opción “r”.
+ Matar o enviar una señal con la opción “k”.
+ Ordenarlos según diferentes criterios (por PID con “N”, uso de CPU con “P”, tiempo con
“A”, etc.).
+ Con “n” se cambia el número de procesos que se muestran.
+ Para salir se utiliza la letra “q”.

##### Orden mpstat
Muestra estadísticas del procesador (o procesadores) del sistema junto con la media global de
todos los datos mostrados (tienes que tener instalado el paquete sysstat que se encuentra en el
directorio /fenix/depar/lsi/so/paquetes, tal y como se vió en la sesión anterior). Permite el
uso de parámetros para definir la cantidad de tiempo entre cada toma de datos y el número de
informes que se desean (mpstat time reports). La información de la cabecera hace referencia a:
+ CPU : número del procesador
+ %user : porcentaje de uso de la CPU con tareas a nivel de usuario
+ %nice : porcentaje de uso de la CPU con tareas a nivel de usuario con prioridad “nice” (>
0)
+ %sys : porcentaje de uso de la CPU para tareas del sistema (no incluye el tratamiento de
interrupciones) (modo núcleo)
+ %iowait : porcentaje de tiempo que la CPU estaba “desocupada” mientras que el sistema
tenía pendientes peticiones de E/S
+ %irq : porcentaje de tiempo que la CPU gasta con interrupciones hardware
+. %soft : porcentaje de tiempo que la CPU gasta con interrupciones software (la mayoría
son llamadas al sistema)
+. %idle : porcentaje de tiempo que la CPU estaba “desocupada” y el sistema no tiene
peticiones de disco pendientes
+ intr/s : número de interrupciones por segundo recibidas por el procesador
La sintaxis de la orden es:
`mpstat [intervalo] [número]` donde intervalo indica cada cuántos segundos debe mostrar los datos, y número, cuántos muestreos se solicitan.
..
#### Actividad 3.4. Estadísticas de recursos del sistema
Responde a las siguientes cuestiones y especifica, para cada una, la orden que has utilizado:
a) ¿Qué porcentaje de tiempo de CPU se ha usado para atender interrupciones hardware? `mpstat` mirando la columna %irq, en mi caso 0,00%
b) ¿Y qué porcentaje en tratar interrupciones software? Con `mpstat` en la columna %soft, en mi caso 0,42%
c) ¿Cuánto espacio de swap está libre y cuánto ocupado? Empleando la orden `top`.

##### Orden free
Aunque algunas de las órdenes anteriores (como top) muestran información acerca del uso de
memoria del sistema, en caso de que queramos centrar la monitorización únicamente en el uso
de memoria, sin distraer la atención con otra información adicional, podemos utilizar órdenes
específicas para esta labor. Esta subsección describe una orden que es capaz de llevar a cabo
esta tarea de forma eficiente.
La orden free consume menos recursos (CPU y memoria) que por ejemplo la orden top. Así
pues, free es una orden muy ligera (lightweight) que se utiliza para visualizar el uso actual de
memoria. Dicha orden informa del consumo de:
7. Memoria real o principal (RAM) instalada en la computadora.
8. Memoria de espacio de intercambio (swap) – esto es, del uso del espacio de intercambio
en la partición de almacenamiento en disco correspondiente. Los sistemas operativos
utilizan espacio de intercambio cuando necesitan alojar parte del espacio virtual de
memoria de un proceso.

#### Actividad 3.5. Utilización de las órdenes free y watch
Explora las opciones de las que consta la orden free prestando especial atención a las diferentes
unidades de medida según las que puede informar acerca de memoria. Además, compare los
resultados con los obtenidos usando la orden watch.
~~~
free [options]
OPTIONS
       -b, --bytes
              Display the amount  of  memory  in
              bytes.

       -k, --kibi
              Display  the  amount  of memory in
              kibibytes.  This is the default.

       -m, --mebi
              Display the amount  of  memory  in
              mebibytes.

       -g, --gibi
              Display  the  amount  of memory in
              gibibytes.

       --tebi Display the amount  of  memory  in
              tebibytes.

       --pebi Display  the  amount  of memory in
              pebibytes.

       --kilo Display the amount  of  memory  in
              kilobytes. Implies --si.

       --mega Display  the  amount  of memory in
              megabytes. Implies --si.

       --giga Display the amount  of  memory  in
              gigabytes. Implies --si.

       --tera Display  the  amount  of memory in
              terabytes. Implies --si.

      --peta Display the amount  of  memory  in
                    petabytes. Implies --si.

       -h, --human
              Show  all  output fields automati‐
              cally  scaled  to  shortest  three
              digit  unit  and display the units
              of print out.  Following units are
              used.

                B = bytes
                K = kibibyte
                M = mebibyte
                G = gibibyte
                T = tebibyte
                P = pebibyte

              If  unit  is missing, and you have
              exbibyte of RAM or swap, the  num‐
              ber  is  in  tebibytes and columns
              might not be aligned with header.

       -w, --wide
              Switch to the wide mode. The  wide
              mode produces lines longer than 80
              characters. In this  mode  buffers
              and cache are reported in two sep‐
              arate columns.

       -c, --count count
              Display the  result  count  times.
              Requires the -s option.

       -l, --lohi
              Show  detailed low and high memory
              statistics.

      -s, --seconds delay
                   Continuously  display  the  result
                   delay   seconds  apart.   You  may
                   actually  specify   any   floating
                   point   number   for  delay  using
                   either . or , for  decimal  point.
                   usleep(3)  is used for microsecond
                   resolution delay times.

            --si   Use kilo, mega, giga etc (power of
                   1000)  instead of kibi, mebi, gibi
                   (power of 1024).

            -t, --total
                   Display a line showing the  column
                   totals.

            --help Print help.

            -V, --version
                   Display version information.
~~~              
La orden watch es distinta, sirve para monitorizar una orden, podemos hacer `watch free` y muestra el uso de la memoria, refrescándolo cada dos segundos por defecto, si queremos cambiarlo a cuatro segundos podemos poner `watch -n 4 free`, y equivalentemente `free -s 4`

##### Orden vmstat
Sirve para supervisar el sistema mostrando información de memoria pero también
acerca de procesos, E/S y CPU. Normalmente su primera salida proporciona una media desde el
último arranque del sistema operativo y se pueden obtener informes sobre el uso durante el
periodo de tiempo actual, indicando el periodo de tiempo en segundos y número de iteraciones
deseadas. Por ejemplo, para ejecutar 20 iteraciones de la orden mostrando la información cada 2
segundos, los informes de memoria y procesos son instantáneas en este caso, ejecutaremos `#> vmstat 2 20`
Ciertas columnas muestran porcentaje de tiempo de CPU tratando con :
+ Programas o peticiones de usuario (columna us).
+ Tareas del sistema (columna sy), tal como esperando E/S, actualizando estadísticas del
sistema, gestionando prioridades, etc.
+ No haciendo nada en absoluto (columna id).

Por ejemplo, una alta demanda de memoria puede provocar en el sistema utilice intensivamente
el espacio de intercambio transfiriendo continuamente información de memoria a disco y
viceversa. Esta situación podría estar indicando que se están comenzando a ejecutar en un
momento dado un número importante de procesos, ello podría quedar reflejado en la salida
iterativa de la orden vmstat de la siguiente manera:
+ La columna r mostraría cuántos procesos están actualmente en cola de ejecución.
+ La columna wa podría indicar que no hay procesos en espacio de intercambio, esto sería
un buen indicador en general deseable en cualquier circunstancia.
+ La columna so indicaría que se está incrementando el uso del espacio de intercambio,
es decir, se lleva información de memoria principal a espacio de intercambio.
+ La columna free indicaría que la memoria principal libre se puede estar agotando.

Después de que dichos procesos se hayan lanzado el sistema tenderá a estabilizarse liberando
algo de memoria principal llevando más información a dispositivo de intercambio. El problema
real llegaría a ser más evidente cuando se empieza a tocar techo en la memoria de intercambio y
la actividad reflejada con respecto a la transferencia de información entre memoria principal y
espacio de intercambio (columnas si y so) no cesa.

#### Actividad 3.6. Utilización de vmstat
Intente reproducir el escenario justo descrito anteriormente supervisando la actividad del
sistema mediante la ejecución periódica de vmstat tal cual se ha descrito, y proporcione como
muestra la salida almacenada en un archivo de texto. Para ello he ejecutado un programa que se queda interbloqueado, y creado un fichero vmstat.md, al cual he redirigido la salida de vmstat: `vmstat 2 20 > vmstat.md`. Ahí se observará el resultado.

##### Órdenes útiles para la consulta de metadatos de archivo.
+ `ls -l` Print a long listing format of file metadata for each file of the specified
directory(-ies).
+ `ls -n` Print a long listing format but list numeric user and group IDs.
+ `ls -la` Like ls -l but do not ignore directory entries starting with “.” character
(Hidden entries.)
+ `ls -li` Print a long listing format adding the inode number field.
+ `ls -lh` Print a long listing format of file metadata but size fields are printed in
Kbytes, Mbytes o Gbytes (human readable format).

#### Actividad 3.7. Consulta de metadatos de archivo
Anota al menos dos nombres de archivo de dispositivo de bloques y dos nombres de dispositivo
de caracteres de tu sistema UML. Anota los nombres de los archivos ocultos de tu directorio de
inicio como usuario root que tienen relación con el intérprete de órdenes que tienes asignado
por defecto. Ahora efectúa la misma tarea pero en una consola de terminal del sistema Ubuntu
que arrancas inicialmente en el laboratorio de prácticas.

Para ello ejecuto `ls -la` en ambos sitios, y los que tengan una b son dispositivos de bloques, en los que aparezca una c de caracteres, y para ver lo de los archivos ocultos como he puesto la opción a aparecen, y coger los que tienen relación con el intérprete de órdenes sería buscar los que ponga bash.

##### Opciones básicas para ordenación que proporciona ls. Especialmente indicadas para aplicarlas en “long listing format”.

+ `ls -X` Sort alphabetically by directory entry extension.
+ `ls -t` Sort by modification time.
+ `ls -u` Sort by access time.
+ `ls -c` Sort by ctime (time of last modification of file status information, e.d. file
metadata.)

#### Actividad 3.8. Listados de metadatos de archivos: ls
Conocemos la sintaxis de la orden para obtener un listado en formato largo (“long listing
format”). Manteniendo la opción de listado largo añade las opciones que sean necesarias para
obtener un listado largo con las siguientes especificaciones:
• Que contenga el campo “access time” de los archivos del directorio especificado y que
esté ordenado por dicho campo.
• Que contenga el campo “ctime” de los archivos del directorio especificado y que esté
ordenado por dicho campo.

Para lo primero sería simplemente poner `ls -lu`, l para que sea long listing, u para que se ordene por access time, y para el segundo punto `ls -lc`.

>La orden `df` permite visualizar, para cada SA montado, información sobre su capacidad de
almacenamiento total, el espacio usado para almacenamiento y el espacio libre restante, y el
punto de montaje en la jerarquía de directorios para cada SA. Utilizando la orden df -i podemos visualizar la información sobre los inodos de cada SA
montado.

> Para poder ver el espacio en disco que gasta un directorio de la jerarquía de directorios, y todo
el subárbol de la jerarquía que comienza en él, se utiliza la orden du. Esta orden proporciona el
número de bloques de disco asignados a todos los archivos (incluidos directorios) que “cuelgan”
del directorio especificado. La última línea de la salida por pantalla muestra la cantidad total de bloques de disco utilizados por el subárbol. du contabiliza el número de bloques de disco asignados estén o no completamente ocupados.

#### Actividad 3.9. Metadatos del sistema de archivos: df y du
Resuelve las siguientes cuestiones relacionadas con la consulta de metadatos del sistema de
archivos:
1. Comprueba cuántos bloques de datos está usando la partición raíz del sistema UML del
laboratorio. Ahora obtén la misma información pero expresada en “human readable format”:
Megabytes o Gigabytes. Para ello consulta en detalle el manual en línea.
2. ¿Cuántos inodos se están usando en la partición raíz? ¿Cuántos nuevos archivos se podrían
crear en esta partición?
3. ¿Cuál es el tamaño del directorio /etc? ¿Y el del directorio /var? Compara estos tamaños con
los de los directorios /bin, /usr y /lib. Anota brevemente tus conclusiones.
4. Obtén el número de bloques de tamaño 4 KB que utiliza la rama de la estructura jerárquica de
directorios que comienza en el directorio /etc. En otras palabras, los bloques de tamaño 4 KB del
subárbol cuya raíz es /etc. ¿Cuál es el tamaño de bloque, por omisión, utilizado en el SA?

**Solución**
1. Ejecuto `du /usr` en UML, obteniendo que UML usa 303308 bloques de datos. Para que aparezca en formato human readable simplemente especificamos la opción -h y devuelve 297M.

2. Para que muestre los inodos en vez de los bloques ponemos `du --inodes /usr` que no sé por qué no funciona en el UML pero en un terminal normal sí. Para que funcione en el UML poniendo `df -i` va, nos dice que tiene 65536 Inodes, de los cuales 14687 son usados y 50849 están libres.

3. Para ver los tamaños hay que poner `df -H <directorio>`.

4. Ejecutamos lo siguiente:
~~~shell
[root@localhost ~]# df -B 4 /etc
Filesystem           4B-blocks      Used Available Use% Mounted on
LABEL=ROOT           264214528 105880576 144912384  43% /
~~~
Por defecto se usa de tamaño 1K (podemos verlo no solo en el manual, sino poniendo: `tune2fs -l /dev/sda1 | grep -i "Block size"`)

> Para crear enlaces duros o enlaces simbólicos sobre un archivo creado previamente se utiliza la
orden ln. Como argumentos básico debemos proporcionarle el nombre del archivo a enlazar
(link target) y el nuevo nombre de archivo (link name).
Los números que aparecen en la columna inmediatamente anterior al username propietario del
archivo (owner) son valores del campo contador de enlaces. Este contador mantiene el número
de enlaces duros a archivos con el objetivo de poder liberar el inodo cuando todos los nombres
de archivo que utilizan dicho inodo hayan sido eliminados de la estructura de directorios y nunca
antes, pues si se liberase el inodo con un contador de enlaces mayor que 0, se podría intentar acceder al archivo y se produciría una inconsistencia entre el espacio de nombres y los
metadatos de archivo.

#### Actividad 3.10. Creación de enlaces con la orden ln
Construye los mismos enlaces, duros y simbólicos, que muestra la salida por pantalla anterior.
Para ello crea los archivos archivo.txt y target_hardLink2.txt y, utilizando el manual en línea
para ln, construye los enlaces softLink, hardLink y hardLink2. Anota las órdenes que has
utilizado.
¿Por qué el contador de enlaces del archivo archivo.txt vale 2 si sobre él existen un enlace
duro hardLink y un enlace simbólico softLink? Porque los enlaces blandos no cuentan en el contador de enlaces, tan sólo los duros, que
son realmente referencias reales al metadato, los blandos tan sólo al nombre del archivo.

Para construir el enlace softLink: `ln -s -T archivo.txt softLink`, -T indicando que el target es archivo.txt y -s para que construya un enlace simbólico y no duro. Para construir el enlace duro hardLink sobre archivo.txt: `ln -T archivo.txt hardLink`, ídem para crear el hard link sobre target_hardLink2.txt: `ln -T target_hardLink2.txt hardLink2`. Y ahora, al hacer `ls -la`, entre otras cosas, me muestra:
~~~shell
ls -la
total 192560
drwxr-xr-x 3 elena elena      4096 dic 18 09:20  .
drwxr-x--- 4 elena elena      4096 dic 16 11:21  ..
-rw-r--r-- 2 elena elena         0 dic 18 09:13  archivo.txt
-rw-r--r-- 2 elena elena         0 dic 18 09:13  hardLink
-rw-r--r-- 2 elena elena         0 dic 18 09:13  hardLink2
lrwxrwxrwx 1 elena elena        11 dic 18 09:19  softLink -> archivo.txt
~~~
#### Actividad 3.11. Trabajo con enlaces
Proporciona las opciones necesarias de la orden ls para obtener la información de metadatos de
los archivos de un directorio concreto en los dos casos siguientes:
+ En el caso de que haya archivos de tipo enlace simbólico, la orden debe mostrar la
información del archivo al que enlaza cada enlace simbólico y no la del propio archivo de
tipo enlace simbólico.
+ En el caso de enlaces simbólicos debe mostrar la información del enlace en sí, no del
archivo al cual enlaza. En el caso de directorios no debe mostrar su contenido sino los
metadatos del directorio.

~~~
-d, --directory
             list directories themselves, not their contents

-H, --dereference-command-line
              follow symbolic links listed on the command line

-L, --dereference
            when showing file information for a symbolic link, show informa‐
            tion for the file the link references rather than for  the  link
            itself

~~~
Luego, para mostrar la info del archivo que enlaza cada enlace simbólico y no la del propio archivo de tipo enlace simbólico habría que poner `ls -lL`, para que muestre información del enlace en sí `ls -lH`, en el caso del directorio `ls -ld`.

>Los dispositivos de nuestro sistema se representan en un SO tipo UNIX mediante archivos
especiales de dispositivo. Existen dos tipos principales de archivos especiales de dispositivo: de
bloques y de caracteres. Los archivos especiales de bloque representan a dispositivos de
bloques, que normalmente coinciden con los dispositivos de almacenamiento persistente, los
ramdisks y los dispositivos loop. Los archivos especiales de caracteres representan a dispositivos
de caracteres del tipo puertos serie, paralelo y USB, consola virtual (console), audio, los
dispositivos de terminal (tty*), y muchos más. Podemos crear archivos especiales de dispositivo, tanto de bloques (buffered) como de
caracteres (unbuffered) utilizando la orden mknod. Esta orden permite especificar el nombre del
archivo y los números principal (major) y secundario (minor). Estos números permiten
identificar a los dispositivos en el kernel, concretamente en la Tabla de Dispositivos. El número
principal determina el controlador al que está conectado el dispositivo y el número secundario al
Guía Práctica de Sistemas Operativos-62dispositivo en sí. Es necesario consultar el convenio de nombres dado que la asignación de estos
números va a depender de la distribución.

#### Actividad 3.12. Creación de archivos especiales
Consulta el manual en línea para la orden mknod y crea un dispositivo de bloques y otro de
caracteres. Anota las órdenes que has utilizado y la salida que proporciona un ls -li de los dos
archivos de dispositivo recién creados. Puedes utilizar las salidas por pantalla mostradas en esta
sección del guión para ver el aspecto que debe presentar la información de un archivo de
dispositivo.

Para crear un dispositivo de bloques hay que usar `mknod disp_bloques b 5 0`, y para que sea de caracteres `mknod disp_caracteres c 5 1`. Ahora, al hacer `ls -li`:
~~~shell
> ls -li
total 192556
13240719 brw-r--r-- 1 root  root       5, 0 dic 18 09:39  disp_bloques
13240720 crw-r--r-- 1 root  root       5, 1 dic 18 09:39  disp_caracteres
~~~











>
