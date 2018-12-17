# Módulo I. Administración de Linux
## Sesión 2. Herramientas de administración del sistema de archivos
> Cuando creamos una partición es necesario asociarle una etiqueta que indica el tipo de SA que
va a alojar cuando posteriormente se formatee. Esta información se almacena mediante un
código numérico que determina el tipo de partición. Cada SO tiene sus propios códigos
numéricos para las particiones pero nosotros solamente nos vamos a centrar en los que se
utilizan en Linux. Por ejemplo, el código asociado a una partición que va a alojar un SA de tipo
ext2 es el 0x83, y el de una partición de intercambio (swap) es el 0x82. Para ver una lista de los
tipos de particiones soportados y sus códigos asociados se puede usar la siguiente orden:
/sbin/sfdisk -T. El número de particiones que podemos establecer en un dispositivo bajo una arquitectura Intel
está limitado debido a la “compatibilidad hacia atrás”. La primera
tabla de particiones (básicamente, es una tabla en la que cada entrada mantiene la
información asociada a una partición: donde comienza y finaliza la partición en el disco, tipo de
partición, partición de arranque (si/no), y algo más) se almacenaba como parte del sector de
arranque maestro (master boot record, MBR) y solamente tenía espacio para almacenar
cuatro entradas. Estas cuatro particiones se conocen en la actualidad como particiones
primarias.
Una partición primaria de disco puede a su vez dividirse. Estas subdivisiones se conocen con el
nombre de particiones lógicas. De esta forma podemos saltarnos la restricción histórica de
poder establecer solamente cuatro particiones por dispositivo de almacenamiento secundario.
La partición primaria que se usa para alojar las particiones lógicas se conoce como partición
extendida y tiene su propio tipo de partición: 0x05. A pesar de que a priori podríamos pensar que, de esta forma, el número de particiones
que podemos establecer en un dispositivo es “ilimitado”, esto no es así. Como casi siempre, los
SO imponen límites en el número de recursos disponibles y, por supuesto, el número de
particiones no iba a ser una excepción. Por ejemplo, Linux limita el número máximo de
particiones que se pueden realizar sobre un disco SCSI a 15, y a un total de 63 sobre un disco
IDE.

¿Cuántas particiones hago en mi dispositivo?
Para responder a esta pregunta vamos a establecer una clasificación de los dispositivos de
almacenamiento secundario, pues según si el dispositivo actuará como dispositivo de
arranque (Boot drive) o no, será necesario establecer una distribución de particiones u otra. Un
dispositivo de arranque va a ser el dispositivo que utilice en primer lugar la BIOS de nuestra
arquitectura para cargar en memoria el programa SO (¡o un programa cargador de SOs!).
Si queremos que nuestro SO arranque desde el dispositivo sobre el que vamos a realizar las
particiones necesitamos establecer la siguiente configuración de particiones:
+ Una partición primaria
+ Una o más particiones swap.
+ Ninguna, o las que quieras, partición(es) primaria(s) y lógica(s). Por supuesto que el
número de particiones está dentro de los límites que establece la compatibilidad hacia
atrás y el SO.
La configuración de particiones de cualquier dispositivo de almacenamiento secundario, que no
se vaya a utilizar como dispositivo de arranque sino simplemente para almacenar información,
será la siguiente:
+ Una o más particiones primarias o lógicas. Por supuesto siempre dentro de los límites
comentados anteriormente.
+ Ninguna, o las que quieras, partición(es) swap.

#### Actividad 2.1. Partición de un dispositivo: “USB pen drive” o “memory stick”
##### Preparación previa a la partición de un dispositivo simulado mediante un archivo especial de dispositivo.
Vamos a utilizar un dispositivo simulado mediante un archivo /dev/loop?. Estos archivos
permiten crear un dispositivo de almacenamiento virtual cuyo espacio de almacenamiento viene
soportado por un archivo asociado. A continuación se describen los pasos para construir este
dispositivo simulado:
+ Crea los archivos /dev/loop0 y /dev/loop1, si no se encuentran en el sistema, utilizando las
siguientes órdenes:
~~~
#> mknod /dev/loop0 b 7 0
#> mknod /dev/loop1 b 7 1
~~~
Con `mknod` creamos archivos especiales de bloques o caracteres. En este caso serán
con el nombre /dev/loop?, con b se especifica que queremos que sea un archivos especial
de bloques (buffered). Por haber especificado la opción b, hemos de indicar major y minor.

+ Crea un archivo de 20 MB y otro de 30 MB en tu sistema de archivos con las siguientes
órdenes:
~~~
#> dd if=/dev/zero of=/root/archivo_SA20 bs=2k count=10000
#> dd if=/dev/zero of=/root/archivo_SA30 bs=3k count=10000
~~~
Con `dd` copiamos un fichero, convertiéndolo y formateándolo de manera acorde a los
operandos especificados. Con bs=BYTES indicamos que se pueden leer y escribir hasta
BYTES bytes de una vez. Con count=N que solo se pueden copiar N input blocks, con if=FILE
que se lea desde FILE en vez de desde stdin, y con of=FILE que se escriba a FILE en vez
de a stdout.

+ Ahora vamos a asociar un archivo de dispositivo loop a cada uno de los archivos que acabamos
de crear. De esta forma el “disco virtual” que representa el archivo pasará a estar asociado al
archivo de dispositivo /dev/loop0 y /dev/loop1. Para ello ejecutamos las siguientes órdenes:
~~~
#> losetup /dev/loop0 /root/archivo_SA20
#> losetup /dev/loop1 /root/archivo_SA30
~~~

+ Por último, podemos comprobar la configuración de los “discos virtuales” mediante la siguiente orden:
~~~
#> fdisk -l /dev/loop0 /dev/loop1
~~~
Devuelve:
~~~
Disk /dev/loop0: 20 MB, 20480000 bytes
255 heads, 63 sectors/track, 2 cylinders, total 40000 sectors
Units = sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disk identifier: 0x00000000

Disk /dev/loop0 doesn't contain a valid partition table

Disk /dev/loop1: 30 MB, 30720000 bytes
255 heads, 63 sectors/track, 3 cylinders, total 60000 sectors
Units = sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disk identifier: 0x00000000

Disk /dev/loop1 doesn't contain a valid partition table
~~~
##### Creación de la partición
Ahora podemos proceder a crear la tabla de
particiones mediante fdisk, ya sea en el pen drive o en los “discos virtuales” /dev/loop0 Y
/dev/loop1. Una vez que hayamos finalizado el proceso podemos comprobar si la tabla de
particiones es correcta con la orden última.

Para crear la tabla de particiones hacemos primeramente `fdisk /dev/loop0`, especificando
la opción n( in order to add a new partition), elijo luego que sea una primary partition 2,
de primer sector 3000, y de último 3512, por poner unos números. Para comprobar que
se ha creado la partición correctamente indico v (verify the partition table), obteniendo:
~~~
Command (m for help): v
Partition 2: previous sectors 3512 disagrees with total 12040
Remaining 39486 unallocated 512-byte sectors
~~~
Para ver la tabla de la partición:
~~~
Command (m for help): p

Disk /dev/loop0: 20 MB, 20480000 bytes
255 heads, 63 sectors/track, 2 cylinders, total 40000 sectors
Units = sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disk identifier: 0x20b3d82e

      Device Boot      Start         End      Blocks   Id  System
/dev/loop0p2            3000        3512         256+  83  Linux
~~~
Por último, pongo w para que se escriba la tabla en el disco y salgamos de fdisk,
devolviéndonos:
~~~
The partition table has been altered!

Calling ioctl() to re-read partition table.

WARNING: Re-reading the partition table failed with error 22: Invalid argument.
The kernel still uses the old table. The new table will be used at
the next reboot or after you run partprobe(8) or kpartx(8)
Syncing disks.
~~~
Procedo de manera análoga para `/dev/loop1`, poniendo otros sectores y tipo de partición:
~~~
Command (m for help): n
Command action
   e   extended
   p   primary partition (1-4)
p
Partition number (1-4, default 1): 3
First sector (2048-59999, default 2048): 4321
Last sector, +sectors or +size{K,M,G} (4321-59999, default 59999): 5678

Command (m for help): v
Partition 3: previous sectors 5678 disagrees with total 2130
Remaining 58641 unallocated 512-byte sectors

Command (m for help): p

Disk /dev/loop1: 30 MB, 30720000 bytes
255 heads, 63 sectors/track, 3 cylinders, total 60000 sectors
Units = sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disk identifier: 0x48d96247

      Device Boot      Start         End      Blocks   Id  System
/dev/loop1p3            4321        5678         679   83  Linux

Command (m for help): w
The partition table has been altered!

Calling ioctl() to re-read partition table.

WARNING: Re-reading the partition table failed with error 22: Invalid argument.
The kernel still uses the old table. The new table will be used at
the next reboot or after you run partprobe(8) or kpartx(8)
Syncing disks.
~~~
Ahora, al poner el comando anterior ya se parece a lo que sale en el guión de prácticas:
~~~
[root@localhost ~]# fdisk -l /dev/loop0 /dev/loop1

Disk /dev/loop0: 20 MB, 20480000 bytes
56 heads, 48 sectors/track, 14 cylinders, total 40000 sectors
Units = sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disk identifier: 0x20b3d82e

      Device Boot      Start         End      Blocks   Id  System
/dev/loop0p2            3000        3512         256+  83  Linux

Disk /dev/loop1: 30 MB, 30720000 bytes
91 heads, 9 sectors/track, 73 cylinders, total 60000 sectors
Units = sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disk identifier: 0x48d96247

      Device Boot      Start         End      Blocks   Id  System
/dev/loop1p3            4321        5678         679   83  Linux
~~~

#### Asignación de un Sistema de Archivos a una partición (formateo lógico)
Una vez que disponemos de las particiones en nuestro dispositivo de almacenamiento secundario
debemos proceder a asignar el sistema de archivos adecuado a cada una de las particiones. En
Linux, aparte del SA específico para las particiones especialmente dedicadas a intercambio
(swap), se utilizan normalmente tres sistemas de archivos: ext2, ext3 y ext4:
+ ext2. Es el sistema de archivos de disco de alto rendimiento usado en Linux para discos
duros, memorias flash y medios extraíbles. El second extended file system se diseñó como
una extensión del extended file system (ext). ext2 ofrece el mejor rendimiento en términos
de velocidad de transferencia de E/S y uso de CPU de entre todos los sistemas de archivos
que soporta Linux.
+ ext3. Es una versión de ext2 que incluye “registro por diario” (journaling). El journaling
es un mecanismo por el cual un sistema informático puede implementar transacciones. Se
basa en llevar un journal o registro de diario en el que se almacena la información
necesaria para restablecer los datos afectados por la transacción en caso de que ésta
falle. La aplicación del journaling en los sistemas de archivos modernos permite evitar la
corrupción de las estructuras de datos que soportan la información del sistema de
archivos: estructura de directorios, estructura de bloques libres de disco y estructuras
que soportan los atributos de los archivos.
+ ext4. Es el estándar de facto actual de las distribuciones Linux. Este SA tiene unas
estructuras similares a las del ext3 pero, además, presenta las siguientes mejoras: Extensiones: permiten describir un conjunto de bloques de disco
contiguos, mejorando de esta forma el rendimiento de E/S al trabajar con archivos de
gran tamaño y reduciendo la fragmentación de disco. Asignación retardada de espacio en disco (allocate-on-flush): Esta técnica
permite postergar en el tiempo la asignación de bloques de disco hasta el momento
real en el que se va a realizar la escritura.


















#
