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

#### Actividad 2.2. Creación de sistemas de archivos
El objetivo es simplemente formatear lógicamente las particiones creadas con anterioridad de
forma consistente con el tipo de SA que se estableció que iba a ser alojado. En la primera
partición crearemos un SA de tipo ext3 y en la segunda un ext4.
La orden que permite establecer un SA de los reconocidos dentro del sistema Linux sobre una
partición de disco es mke2fs.
El resultado de la ejecución de esta orden es el formateo lógico de la partición escogida
utilizando el SA que se ha seleccionado. mke2fs es la
orden genérica para creación de sistemas de archivos. Como requisito es necesario que
establezcas dos etiquetas de volumen para los SAs: LABEL_ext3 para la primera partición y
LABEL_ext4 para la segunda.

>If mke2fs is run as mkfs.XXX (i.e., mkfs.ext2, mkfs.ext3, or mkfs.ext4)
       the option -t XXX is implied; so mkfs.ext3 will create  a  file  system
       for  use  with  ext3,  mkfs.ext4 will create a file system for use with
       ext4, and so on.
Usage: mke2fs [-c|-l filename] [-b block-size] [-f fragment-size]
	[-i bytes-per-inode] [-I inode-size] [-J journal-options]
	[-G meta group size] [-N number-of-inodes]
	[-m reserved-blocks-percentage] [-o creator-os]
	[-g blocks-per-group] [-L volume-label] [-M last-mounted-directory]
	[-O feature[,...]] [-r fs-revision] [-E extended-option[,...]]
	[-T fs-type] [-U UUID] [-jnqvFKSV] device [blocks-count]

Para ello habríamos de especificar: `mke2fs -L LABEL_ext3 -t ext3 -c /dev/loop0` ó `mkfs.ext3 -m1 -L LABEL_ext3 -c /dev/loop0` (Formatear la partición /dev/loop0 con el sistema de archivos ext3, con un espacio libre (desperdiciado, podemos incluso darle un 0, útil si no tenemos un sistema operativo en esta partición) reducido al 1% y con una etiqueta "LABEL_ext3") y `mke2fs -L LABEL_ext4 -t ext4 -c /dev/loop1` o equivalentemente `mkfs.ext4 -L LABEL_ext4 -c /dev/loop1`.

>Una vez que tenemos disponibles nuestros nuevos sistemas de archivos podemos utilizar tune2fs, que nos permite ajustar determinados parámetros de los sistemas de archivos
ext2/ext3/ext4. Se puede utilizar la opción -l para obtener un listado por pantalla que nos
muestre la información relevante de un determinado SA. Algunas opciones interesantes de la orden tune2fs:
+ -l <dispositivo> Muestra por pantalla el contenido del superbloque del SA.
+ -c max-mount-counts <dispositivo > Establece el número máximo de montajes que se puede llegar
a realizar sin que se realice una comprobación de la
consistencia del SA.
+ -L label <dispositivo> Poner una etiqueta al SA.

Con el tiempo, las estructuras de metainformación del SA pueden llegar a corromperse. Esta
situación podría provocar degradación en el rendimiento del sistema e incluso situaciones de
pérdida de información. Para solucionarlo Linux automatiza el proceso de comprobación de la
consistencia del sistema de archivos en base al número de montajes que se han realizado. No
obstante, pueden ocurrir situaciones en las que sea necesario que el administrador ejecute
manualmente las comprobaciones y repare las posibles inconsistencias. Linux proporciona la
herramienta fsck para llevar a cabo esta labor.

#### Actividad 2.3. Personalización de los metadatos del SA
Consultando el manual en línea para la orden tune2fs responde a las siguientes preguntas:
(a) ¿Cómo podrías conseguir que en el siguiente arranque del sistema se ejecutara
automáticamente e2fsck sin que se haya alcanzado el máximo número de montajes? Poniendo `tune2fs -c -1`, ya que según el manual:
~~~shell
 -c max-mount-counts
              Adjust  the  number of mounts after which the filesystem will be
              checked by e2fsck(8).  If max-mount-counts is 0 or -1, the  num‐
              ber  of  times  the filesystem is mounted will be disregarded by
              e2fsck(8) and the kernel.
~~~
Otra opción posible sería empleando
~~~shell
-C mount-count
              Set  the  number  of  times  the  filesystem  has been
              mounted.  If set to a  greater  value  than  the  max-
              mount-counts parameter set by the -c option, e2fsck(8)
              will check the filesystem at the next reboot.
~~~
Es decir, estableciendo con esa opción un número suficientemente grande (número más grande de veces que el sistema de archivos es montado) en el próximo arranque se ejecutará e2fsck.


(b) ¿Cómo podrías conseguir reservar para uso exclusivo de un usuario username un número de
bloques del sistema de archivos? `tune2fs -r <number> -u username`, incluso sin especificar -r diría yo que va.
~~~shell
-r reserved-blocks-count
              Set the number of reserved filesystem blocks.
-u user
              Set the user  who  can  use  the  reserved  filesystem
              blocks.   user  can be a numerical uid or a user name.
              If a user name is given, it is converted to a  numeri‐
              cal uid before it is stored in the superblock.
~~~

#### Actividad 2.4. Montaje de sistemas de archivos
Utiliza el manual en línea para descubrir la forma de montar nuestros SAs de manera que
cumplas los siguientes requisitos:
3. El SA etiquetado como LABEL_ext3 debe estar montado en el directorio /mnt/SA_ext3 y
en modo de solo lectura.
4. El SA etiquetado como LABEL_ext4 debe estar montado en el directorio /mnt/LABEL_ext4
y debe tener sincronizadas sus operaciones de E/S de modificación de directorios.

~~~shell
The standard form of the mount command is:

              mount -t type device dir

       This  tells  the kernel to attach the filesystem found on device (which
       is of type type) at the directory dir.  The option -t type is optional.
The following command lists all mounted filesystems (of type type):

             mount [-l] [-t type]
Most  devices  are indicated by a filename (of a block special device),
      like /dev/sda1

It is
also possible to indicate a block special device using  its  filesystem
label or UUID (see the -L and -U options below), or its partition label
or UUID.
~~~
Basándonos en lo anterior, para hacer el apartado 3 debemos especificar:
`mount -r -L "LABEL_ext3" /dev/loop0 /mnt/SA_ext3` (habiendo hecho antes `mkdir /mnt/SA_ext3`), y para 4: `mount -L "LABEL_ext4" -o dirsync /dev/loop1 /mnt/LABEL_ext4`, habiendo creado previamente el directorio en el que vamos a montarlo.

##### Formato del archivo /etc/fstab
Como vimos en la sesión anterior, /etc/fstab es el archivo de configuración que contiene la
información sobre todos los sistemas de archivos que se pueden montar y de las zonas de
intercambio a activar. El formato del archivo se describe a continuación:
<file system> <mount point> <type> <options> <dump> <pass>

+ <file system>, es el número que identifica el archivo especial de bloques .

+ <mount point>, es el directorio que actua como punto de montaje.

+ <type>, tipo de sistema de archivos (ext2, ext3, ext4, vfat, iso9660, swap, nfs, etc.)

+ <options>, son las opciones que se utilizarán en el proceso de montaje. Se especifican
como una lista separada por comas y sin espacios.

+ <dump>, normalmente no se usa, pero si su valor es distinto de 0 indica la frecuencia con
la que se realizará una copia de seguridad del SA.

+ <pass> , durante el arranque del sistema este campo especifica el orden en el que la
orden fsck realizará las comprobaciones sobre los SAs.

A continuación se muestran las posibles opciones que pueden especificarse en el campo
<options>:
+ rw. Lectura-escritura

+ ro. Sólo lectura

+ suid/nosuid. Permitido el acceso en modo SUID, o no permitido

+ auto/noauto. Montar automáticamente o no montar automáticamente (ni ejecutando
mount -a)

+ exec/noexec. Permitir la ejecución de ficheros, o no permitir

+ usrquota, grpquota. Cuotas de usuario y de grupo

+ defaults = rw,suid,dev,exec,auto,nouser,async

+ user, users, owner. Permitir a los usuarios montar un sistema de archivos

+ uid=500, gid=100. Propietario y grupo propietario de los archivos del SA.

+ umask. Máscara para aplicar los permisos a los archivos.

#### Actividad 2.5. Automontaje de Sistemas de Archivos
Escribe las dos líneas necesarias en el archivo /etc/fstab para que se monten automáticamente
nuestros dos SA en el arranque del sistema con los mismos requisitos que se han pedido en la
Actividad 2.4.

Habría que añadir las siguientes líneas:
~~~shell
/dev/loop0 /mnt/SA_ext3 ext3 -ro,-L "LABEL_ext3", auto 0 0  # o loop0  /dev/loop0 auto defaults 0 0
/dev/loop1 /mnt/LABEL_ext4 ext4 -L "LABEL_ext4", -o dirsync, auto 0 0 # o loop1  /dev/loop1 auto defaults 0 0
~~~

##### APT y YUM
APT (Advanced Packaging Tool) fue originalmente desarrollado por Debian Linux y modificado
para su uso también con paquetes RPM. Tanto APT, como su interfaz gráfico Synaptic, son fáciles
de utilizar. Existen varios proveedores principales de paquetes y repositorios para APT, por
ejemplo http://www.freshrmps.net, pero no se deben utilizar simultáneamente porque a veces
existen conflictos entre versiones.
Algunos desarrolladores piensan que para gestionar paquetes RPM es mejor herramienta YUM
(Yellow dog Updater, Modified - http://yum.baseurl.org) que APT. Además, parece que APT
contiene más código innecesario que se utiliza realmente para los paquetes .deb. Las siguientes
órdenes son muy útiles al usar YUM:

+ `yum list` Lista los paqu etes disponibles en los repositorios para su instalación
+ `yum list installed` Lista los paquetes actualmente instalados
+ `yum list updates` Muestra todos los paquetes con actualizaciones disponibles en los
repositorios para su instalación
+` yum install <nombre-paquete>` Instala el paquete cuyo nombre es especificado
+ `yum update` Se actualizan todos los paquetes instalados
+ `yum remove <nombre-paquete>` Elimina el paquete cuyo nombre es especificado, así como los paquetes
que dependen de éste

Existen varios interfaces gráficos que utilizan directamente YUM tales como los mencionados
anteriormente: PackageKit y gnome-packagekit. Aunque YUM sólo proporciona una interfaz para
línea de órdenes, resulta muy cómodo y fácil de usar. Puede ejecutar la siguiente órden en el
shell para obtener un listado más completo de las órdenes y opciones disponibles en YUM: `#> yum –-help | more`

#### Actividad 2.7. Trabajo con el gestor de paquetes YUM
Encuentra los archivos de configuración de YUM y explora las distintas órdenes disponibles en
YUM ejecutándolas. En concreto, lista todos los paquetes instalados y disponibles, elimina el
paquete instalado que te indique el profesor de prácticas, y a continuación vuelve a instalar el
mismo paquete haciendo uso de los paquetes que se encuentran disponibles en
/fenix/depar/lsi/so/paquetes. Para obtener acceso a este directorio del sistema de archivos
anfitrión ejecute la siguiente órden de montaje una vez lanzado el sistema operativo User Mode
Linux (UML):
`#> mount none /<directorio-punto-montaje> /fenix/depar/lsi/so/paquetes -t hostfs -o`
Tenga en cuenta que algunas órdenes de YUM pueden no funcionar bien debido a que no pueden
acceder a los sitios web que mantienen repositorios de paquetes, ya que no existe conexión a
Internet en la configuración actual del sistema operativo UML. Para descargar paquetes de uno
de los principales sitios web encargados de mantener repositorios de paquetes en código binario
y fuente, con el navegador Web acceda a https://admin.fedoraproject.org/pkgdb. Puede
realizar la búsqueda de un paquete binario específico introduciendo parte del nombre y pulsando
el botón BUILD, o puede acceder al listado completo de paquetes pulsando sobre Builds en el
menú de navegación que se encuentra a la izquierda.

+ Para mostrar una lista de los paquetes instalados: `yum list installed`
+ Para mostrar una lista de los paquetes que están disponibles para ser instalados: `yum list avalaible`
+ Para mostrar una lista de los paquetes disponibles e instalados: `yum list all`

+ Para borrar cualquier paquete: `yum remove paquete` (ó equivalentemente `yum erase paquete`)

+ Para luego volver a instalarlo: `yum install paquete`

##### RPM
El gestor de paquetes RPM desciende del primer software de gestión de paquetes Linux.
Accediendo al manual de RPM o ejecutando `rpm --help`, se puede obtener  una descripción detallada de las opciones disponibles.
El formato general para la línea de órdenes se muestra a continuación:
`#> rpm <opciones> <nombres-paquetes>`

>Clasificación de tipos de operaciones en el gestor RPM y órdenes más comunes.

+ Instalación de nuevos paquetes: `rpm -i <nombre-archivo-paquete>`. Si la operación tiene éxito no se mostrará ningún mensaje.

+ Eliminación de paquetes instalados: `rpm -e <nombre-paquete>`. Si la operación tiene éxito no se mostrará ningún mensaje.

+ Actualización de paquetes instalados: `rpm -U <nombre-archivo-paquete>`. La actualización se consigue descargando el paquete que corresponde a la nueva versión y ejecutando RPM con la opción -U, que además incluye la eliminación automática de la versión del paquete previamente instalada. También `rpm -F <nombre-servidor-HTTP/FTP>`. Se buscará el paquete en el servidor designado en Internet y se preparará la correspondiente actualización.

+ Obtención de información sobre paquetes software: `rpm -qa | grep <parte-nombre- paquete-buscado> | sort`. Esta línea de órdenes puede utilizarse para buscar paquetes instalados por su nombre o parte de éste. También `rpm -qi <nombre-paquete>` Muestra información precisa del
paquete instalado que se especifica.

+ Verificación e integridad de la instalación: `rpm -V <nombre_paquete  ` Consulta en la base de datos para verificar la instalación de un paquete recientemente instalado. Si la instalación se ha realizado correctamente, la orden no produce información de salida.

#### Actividad 2.8. Trabajo con el gestor de paquetes rpm
En primer lugar deseamos mostrar cierta metainformación acerca de uno o más paquetes ya
instalados. Para ello debes utilizar la orden rpm con las opciones adecuadas.
1. Muestra la información general (nombre, versión, arquitectura, grupo, descripción, etc.) y
lista los archivos que contiene un paquete ya instalado haciendo uso de la orden rpm y un
único conjunto de opciones.
2. Ídem que el anterior pero mostrando únicamente los archivos de configuración que
contiene el paquete.
3. Escribe una orden que muestre los paquetes requeridos por un paquete determinado que
se encuentre instalado en el sistema. Escriba la orden que devuelva el mismo resultado
pero para un paquete no instalado en el sistema.
4. Instala el paquete quota que encontrarás en el directorio de software de la asignatura
(directorio que ya has montado en la Actividad 2.7).
5. Instala y desinstala el paquete sysstat mostrando en pantalla también la máxima
información posible acerca del propio proceso de eliminación del paquete.
**Solución**

1. Para mostrar la información general de todos los paquetes instalados habría que poner `rpm -a -v -q -i`. Pongo el -v para que sea verbose, si no no solo muestra los nombres de los paquetes, y la opción -q -i es la que muestra información del paquete, incluyendo nombre, versión y descripción.

2. Para mostrar los archivos de configuración que tiene el paquete: `rpm -a -v -q -c`, -a para que muestre el de todos los paquetes, -v para que muestre toda la información, -q porque es una query y -c para que liste solo los archivos de configuración.

3. `rpm -a -v -q -R`, en este cambia que ahora usamos -R, el cual según el manual: `List  capabilities  on  which   this   package depends.`. Para los paquetes que no están instalados: `rpm -p * -v -q -R`, no devuelve nada, pero tiene sentido que sea así, en vez de especificar all le digo -p, que, según el manual:
~~~
-p, --package PACKAGE_FILE
              Query an (uninstalled)  package  PACKAGE_FILE.
              The PACKAGE_FILE may be specified as an ftp or
              http style URL,  in  which  case  the  package
              header  will  be  downloaded and queried.  See
              FTP/HTTP  OPTIONS  for  information  on  rpm's
              internal  ftp  and  http  client  support. The
              PACKAGE_FILE  argument(s),  if  not  a  binary
              package, will be interpreted as an ASCII pack‐
              age manifest  unless  --nomanifest  option  is
              used.   In  manifests, comments are permitted,
              starting with a '#', and each line of a  pack‐
              age manifest file may include white space sep‐
              arated glob expressions, including URL\'s, that
              will be expanded to paths that are substituted
              in place of the package manifest as additional
              PACKAGE_FILE arguments to the query.
~~~

y con \* que sean todos los paquetes que no está instalados(?).

4. `rpm -i quota`

5. Sería `rpm -i sysstat` y luego `rpm -e -vv sysstat` (la opción -vv muestra ugly debugging ). Otra cosa que también podría ser posible es `rpm -e -a -q --changes sysstat`, porque --changes: *Displays change  information  for  the  package with full time stamps.*

#### Actividad 2.9. Sistema de cuotas para el sistema de archivos tipo ext3
En esta actividad se van a presentar los pasos que necesitas llevar a cabo para establecer el
sistema de cuotas de disco en Linux. El objetivo será activar el sistema de cuotas sobre el
sistema de archivos tipo ext3 que has creado con anterioridad.
1. Editar el archivo /etc/fstab y activar el sistema de cuotas de usuario para el SA tipo ext3.
Busca cómo se especifica esta opción en el manual en línea. Una ayuda para la búsqueda es que
la realices sobre la orden mount y recuerdes que las opciones de montaje vienen especificadas
en los apartados: FILESYSTEM INDEPENDENT MOUNT OPTIONS y FILESYSTEM SPECIFIC
MOUNT OPTIONS.
2. Montar de nuevo el SA en el espacio de nombres para que se active la opción previamente
establecida. Usa la siguiente orden:
`#> mount -o remount <directorio_punto_de_montaje>`
3. Crear el archivo que permite llevar el control de cuotas de usuario para el SA. El nombre de
este archivo es aquota.user. Para ello utiliza la siguiente orden:
`#> quotacheck -nm <directorio_punto_de_montaje>`
4. Ahora procedemos a activar el sistema de control de cuotas de usuario. Para ello ejecuta la
orden:
`#> quotaon -a`
5. Ahora solo falta editar la cuota para cada usuario del sistema mediante la siguiente orden. En
este caso, establece los parámetros para cada usuario existente. Puede ser buena idea utilizar el
archivo /etc/passwd para localizar los nombres.
`#> edquota username`
6. Para finalizar estableceremos el periodo de gracia para el límite soft.
`#> edquota -t`

>Órdenes útiles para el trabajo con cuotas.
+ `quota username` Asignación de las cuotas para un usuario.
+ `repquota <SA>` Estadística de las cuotas para todos los usuarios .

#### Actividad 2.10. Establecer límites sobre recursos de un SA
Establece los límites de bloques e i-nodos para un par de usuarios del sistema UML sobre el que
trabajas en el laboratorio:
`setquota -u <usuario_al_que_le_quiero_cambiar_las_quotas> block-softlimit block-hardlimit  inode-softlimit inode-hardlimit`


> Para montar el sistema de archivos a partir de una carpeta: `mount none /mnt/ -t hostfs -o <ruta desde home a donde tengo el paquete que quiero instalar>`
