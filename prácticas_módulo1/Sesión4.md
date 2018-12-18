# Módulo I. Administración de Linux.
## Sesión 4. Automatización de tareas.

#### Actividad 4.1. Consulta de información sobre procesos demonio
A partir de la información proporcionada por la orden ps encuentre los datos asociados a los
demonios atd y crond, en concreto: quién es su padre, qué terminal tienen asociado y cuál es su
usuario.

Para que me salgan daemons pongo la opción --ppid 1, ya que ellos tienen como padre al proceso init, y obtengo:
~~~
[root@localhost ~]# ps --ppid 1
  PID TTY          TIME CMD
 1082 ?        00:00:00 auditd
 1100 ?        00:00:00 rsyslogd
 1140 ?        00:00:00 sshd
 1169 ?        00:00:00 sendmail
 1180 ?        00:00:00 crond
 1195 ?        00:00:00 login
~~~
Luego atd no está instalado.

##### Ejecutar tareas a una determinada hora: demonio atd
Con el demonio atd podemos provocar la ejecución de una orden en un momento de tiempo
especificado. Como usuarios de este servicio interaccionamos con atd con las siguientes órdenes:
+ at : ordenar la ejecución de órdenes a una determinada hora
+ atq: consultar la lista de órdenes
+ atrm: eliminar órdenes
+ batch: ordenar la ejecución de órdenes que se ejecutarán cuando la carga del sistema sea baja.

##### Orden at
Su sintaxis completa es:
`at [-q queue] [-f <script>] [-mldbv] TIME`

La orden at lee órdenes de la entrada estándar o del archivo <script> y provoca su ejecución a
la hora especificada en TIME (una sola vez), usando /bin/sh. TIME admite muchos formatos,
por ejemplo HH:MM. Para una descripción de las posibilidades para expresar la hora vea la
ayuda de at y el contenido del archivo /usr/share/doc/at/timespec.
Con el ejemplo siguiente, a una determinada hora (17:10) se generará la lista de archivos del
directorio home en un archivo de nombre listahome:
~~~shell
at 17:10
at> ls ~ > listahome
at> <Ctrl-D>
~~~
Con lo que me devuelve `job 12 at Tue Dec 18 17:10:00 2018`. Comprobado que funciona, para que no se me olvide que hace eso todos los días y generar trabajo tonto elimino la tarea: `atrm 12`.

#### Actividad 4.2. Ejecución postergada de órdenes con at (I)
Crea un archivo genera-apunte que escriba la lista de hijos del directorio home en un archivo
de nombre listahome-`date +%Y-%j-%T-$$`, es decir, la yuxtaposición del literal “listahome” y
el año, día dentro del año, la hora actual y pid (consulte la ayuda de date).
Lanza la ejecución del archivo genera-apunte un minuto más tarde de la hora actual.
¿En qué directorio se crea el archivo de salida?  
~~~shell
!#/bin/bash
# Fichero genera_apunte.sh
ls ~ > listahome-`date +%Y-%j-%T-$$`

~~~shell
>at now + 1 minute
warning: commands will be executed using /bin/sh
at> ./genera-apunte.sh
at> <EOT>
job 13 at Tue Dec 18 10:02:00 2018
~~~
Aparece en el directorio desde donde he lanzado el trabajo, al hacer `ls` me ha aparecido listahome-2018-352-10:02:07-3690.

#### Actividad 4.3. Ejecución postergada de órdenes con at (II)
Lanza varias órdenes at utilizando distintas formas de especificar el tiempo como las siguientes:
a) a medianoche de hoy: `at midnight`

b) un minuto después de la medianoche de hoy: `at midnight + 1 minute`

c) a las 17 horas y 30 minutos de mañana: `at 5:30pm tomorrow`

d) a la misma hora en que estemos ahora pero del día 25 de diciembre del presente
año: `at 10:14 Dec 25`

e) a las 00:00 del 1 de enero del presente año: `at 00:00 Jan 1 2018` devuelve, como tiene sentido, `at: refusing to create job destined in the past`.

Para todos los apartados le he puesto que haga `ls`. Para ver ahora todos los trabajos pendientes que hemos creado empleamos `atq`, y de paso comprobamos que están bien fijados:
~~~shell
> atq
17	Wed Dec 19 17:30:00 2018 a root
16	Wed Dec 19 00:01:00 2018 a root
18	Tue Dec 25 10:14:00 2018 a root
15	Wed Dec 19 00:00:00 2018 a root
~~~
Ahora borro todos ellos: `atrm 15` y así hasta 18.

#### Actividad 4.4. Cuestiones sobre at
El proceso nuevo que se lanza al cumplirse el tiempo que se especificó en la orden at....
+ ¿qué directorio de trabajo tiene inicialmente? ¿hereda el que tenía el proceso que invocó
a at o bien es el home, directorio inicial por omisión? Hereda el que tenía el proceso que invocó a at.
+ ¿qué máscara de creación de archivos umask tiene? ¿es la heredada del padre o la que se
usa por omisión? La máscara que tiene es 022, la que hereda del padre.
+ ¿hereda las variables locales del proceso padre? No.

#### Actividad 4.5. Relación padre-hijo con órdenes ejecutadas mediante at
El proceso nuevo que se lanza al cumplirse el tiempo que se especificó en la orden at.... ¿de
quién es hijo? Investiga lanzando la ejecución retardada de un script que muestre la información
completa sobre los procesos existentes y el pid del proceso actual; el script podría contener lo
que sigue:
~~~
nombrearchivo=`date +%Y-%j-%T`
ps -ef > $nombrearchivo
echo Mi pid = $$ >> $nombrearchivo
~~~
Pongo eso en el fichero `script.sh`, le doy permiso de ejecución y le pongo que lo ejecute en un minuto con `at now + 1 minute`, con lo que me aparece un archivo con la fecha de hoy que contiene al final del todo, después del listado de trabajos: `Mi pid = 4714`. Los procesos anteriores son:
~~~
daemon    4712   943  0 10:27 ?        00:00:00 /usr/sbin/atd -f
root      4713  4712  0 10:27 ?        00:00:00 sh
root      4714  4713  0 10:27 ?        00:00:00 /bin/bash ./script.sh
root      4716  4714  0 10:27 ?        00:00:00 ps -ef
~~~

Luego es hijo del proceso atd.

> En el caso de lanzar la ejecución de órdenes que generan muchos mensajes en la salida estándar
o salida de error estándar podríamos redirigirlas a /dev/null para que se “pierda” y no inunde el
buzón de correo:
~~~
at now + 1 minute
at> tar cvf /backups/backup.tar . 1>> ~/salidas 2> /dev/null
at> <ctrl-D>
~~~
También puede ser conveniente, como se ve arriba, redirigir la salida estándar del programa a
un archivo para que pueda consultarse en caso de necesidad.

#### Actividad 4.6. Script para orden at
Construye un script que utilice la orden find para generar en la salida estándar los archivos
modificados en las últimas 24 horas (partiendo del directorio home y recorriéndolo en
profundidad), la salida deberá escribirse el archivo de nombre “modificados_<año><día><hora>” (dentro del directorio home). Con la orden at provoque que se ejecute dentro de un día a partir de este momento.

Ejecuto mi script:
~~~
at now + 1 minute
warning: commands will be executed using /bin/sh
at> ./actividad4_6.sh
at> <EOT>
job 22 at Tue Dec 18 10:42:00 2018
~~~

##### Orden batch
La orden batch es equivalente a at excepto que no especificamos la hora de ejecución, sino que
el trabajo especificado se ejecutará cuando la carga de trabajos del sistema esté bajo cierto valor
umbral que se especifica a la hora de lanzar el demonio atd.

#### Actividad 4.7. Trabajo con la orden batch
Lanza los procesos que sean necesarios para conseguir que exista una gran carga de trabajo
para el sistema de modo que los trabajos lanzados con la orden batch no se estén ejecutando
(puede simplemente construir un script que esté en un ciclo infinito y lanzarla varias veces en
segundo plano). Utiliza las órdenes oportunas para manejar este conjunto de procesos (la orden
jobs para ver los trabajos lanzados, kill para finalizar un trabajo, ...y tal vez también las
órdenes fg, bg para pasar de segundo a primer plano y viceversa, <Ctrl-Z> para suspender el
proceso en primer plano actual, etc). Experimenta para comprobar cómo al ir disminuyendo la
carga de trabajos habrá un momento en que se ejecuten los trabajos lanzados a la cola batch.










>
