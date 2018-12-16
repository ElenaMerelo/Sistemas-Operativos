# Módulo 1: administración de Linux
## Sesión 1: Herramientas de administración básicas
### Gestión de usuarios
Un usuario (user) es una persona que trabaja en el sistema mediante una cuenta de usuario a la
que accede mediante una identificación. En Linux, un usuario se caracteriza por:
+ Su nombre de usuario, también conocido como username.
+ Su identificador de usuario (UID, del inglés User IDentifier) que es un número entero que
le asigna internamente el sistema y que lo representa (el sistema operativo no trabaja con
su nombre sino con su UID). El UID del root es el 0.
+ El grupo o grupos a los que pertenece (GID, del inglés Group IDentifier). Un usuario
tiene asignado un grupo principal (primary group) que es el grupo que aparece
especificado en el archivo /etc/passwd, pero puede pertenecer a más de un grupo. Los
grupos adicionales a los que puede pertenecer un usuario se denominan grupos
suplementarios (supplementary groups). Todos los grupos y los usuarios que pertenecen
a cada grupo están especificados en el archivo /etc/group. El GID principal del
superusuario es el 0.

> Creación de cuentas de usuario
Para añadir un nuevo usuario al sistema se han de realizar una serie de pasos que enumeramos a
continuación y que se llevan a cabo utilizando las órdenes que veremos más adelante:
1. Decidir el nombre de usuario (username) y los grupos a los que va a pertenecer el usuario
que utilizará esa cuenta (grupo principal y grupos suplementarios).
2. Introducir los datos en los archivos /etc/passwd y /etc/group, rellenando el campo
correspondiente a la contraseña (password) con el carácter “x”.
3. Asignar un password a la nueva cuenta de usuario.
4. Establecer los parámetros de envejecimiento de la cuenta.
5. Crear el directorio de inicio del nuevo usuario (HOME) normalmente en el directorio del
sistema /home, establecer el propietario y grupo correspondiente y los permisos
adecuados.
6. Copiar los archivos de inicialización (.bash_profile, .bashrc,...)
7. Establecer otras facilidades: quotas, mail, permisos para imprimir, etc.
8. Ejecutar cualquier tarea de inicialización propia del sistema.
9. Probar la nueva cuenta.

Las herramientas automáticas para la creación de cuentas de usuario suelen realizar todas las
tareas básicas del proceso anterior, a excepción de las específicas (quotas o impresión, etc.). Las
órdenes para la creación de cuentas de usuario son: `useradd` o `adduser`.

Se creará el usuario y su grupo principal, así como las entradas correspondientes en
/etc/passwd, /etc/shadow y /etc/group. También se creará el directorio de inicio,
normalmente en /home, y los archivos de configuración particulares para cada usuario que se
ubican dentro de este directorio y que se detallan más adelante. Si ejecutamos dichas órdenes
sin argumentos, nos muestran la lista de opciones por stderr (salida de error estándar).
Esta orden toma los valores por defecto que se le van a asignar al usuario (a su cuenta) a partir
de la información especificada en los archivos /etc/default/useradd y /etc/login.defs.

#### Actividad 1.2. Valores por omisión para nuevas cuentas
Visualiza el contenido de los dos archivos anteriores y comprueba cuáles son las opciones por
defecto que tendría un usuario que se creara en nuestro sistema. A continuación, crea una
cuenta de usuario y visualiza el contenido de los archivos /etc/passwd y /etc/group, y el
Guía Práctica de Sistemas Operativos-13directorio /home para comprobar que los nuevos datos se han rellenado conforme a la
especificación tomada de /etc/default/useradd y /etc/login.defs.

~~~shell
[root@localhost ~]# cat /etc/default/useradd
# useradd defaults file
GROUP=100
HOME=/home
INACTIVE=-1
EXPIRE=
SHELL=/bin/bash
SKEL=/etc/skel
CREATE_MAIL_SPOOL=yes

[root@localhost ~]# cat /etc/login.defs
# *REQUIRED*
#   Directory where mailboxes reside, _or_ name of file, relative to the
#   home directory.  If you _do_ define both, MAIL_DIR takes precedence.
#   QMAIL_DIR is for Qmail
#
#QMAIL_DIR	Maildir
MAIL_DIR	/var/spool/mail
#MAIL_FILE	.mail

# Password aging controls:
#
#	PASS_MAX_DAYS	Maximum number of days a password may be used.
#	PASS_MIN_DAYS	Minimum number of days allowed between password changes.
#	PASS_MIN_LEN	Minimum acceptable password length.
#	PASS_WARN_AGE	Number of days warning given before a password expires.
#
PASS_MAX_DAYS	99999
PASS_MIN_DAYS	0
PASS_MIN_LEN	5
PASS_WARN_AGE	7

#
# Min/max values for automatic uid selection in useradd
#
UID_MIN			  500
UID_MAX			60000

#
# Min/max values for automatic gid selection in groupadd
#
GID_MIN			  500
GID_MAX			60000

#
# If defined, this command is run when removing a user.
# It should remove any at/cron/print jobs etc. owned by
# the user to be removed (passed as the first argument).
#
#USERDEL_CMD	/usr/sbin/userdel_local

#
# If useradd should create home directories for users by default
# On RH systems, we do. This option is overridden with the -m flag on
# useradd command line.
#
CREATE_HOME	yes

# The permission mask is initialized to this value. If not specified,
# the permission mask will be initialized to 022.
UMASK           077

# This enables userdel to remove user groups if no members exist.
#
USERGROUPS_ENAB yes

# Use SHA512 to encrypt password.
ENCRYPT_METHOD SHA512

~~~
Hago `useradd newuser` y al visualizar el archivo passwd con `cat /etc/passwd` en la última línea obtenemos:
`newuser:x:500:500::/home/newuser:/bin/bash` y en el archivo group (`cat /etc/group`):
`newuser:x:500:`.

>Para modificar los valores asociados a una cuenta, disponemos de las siguientes órdenes:
+ `usermod` modifica una cuenta de usuario ya existente
+ `userdel` elimina una cuenta de usuario (por defecto no borra el directorio
HOME)
+ `newusers` crea cuentas de usuarios utilizando la información introducida en
un archivo de texto, que ha de tener el formato del archivo
/etc/passwd
+ `system-config-users` herramienta en modo gráfico

En el directorio `/etc/skel` se guardan unos archivos de configuración del shell, los cuales se
copian al directorio HOME asignado cuando se crea una cuenta de usuario. Posteriormente, cada
usuario podrá personalizar su copia. Estos archivos son guiones shell que realizan determinadas
tareas como inicializar variables, ejecutar funciones específicas, establecer los alias, etc. Estos
archivos dependen del intérprete de órdenes seleccionado y en el caso del bash son:
+ `.bash_profile` se ejecuta al hacer el login (conectarnos al sistema) y en él podremos indicar
alias, variables, configuración del entorno, etc. que deseamos iniciar al
principio de la sesión.
+ `.bashrc` su contenido se ejecuta cada vez que se ejecuta una shell, tradicionalmente en
este archivo se indican los programas o scripts a ejecutar.
+ `.bash_logout` se ejecuta al salir el usuario del sistema y en él podremos indicar acciones,
programas, scripts, etc., que deseemos ejecutar al salirnos de la sesión.

#### Actividad 1.3. Creación de usuarios
1. Utiliza el manual en línea para leer la sintaxis completa de la utilidad para creación de
cuentas y crea dos o tres usuarios en tu sistema cambiando alguno de los valores por
defecto.
2. Elimina alguno de ellos y comprueba que “rastro” ha dejado la cuenta recién eliminada en
el sistema.
3. Entra (orden su) en el sistema como uno de estos usuarios que has creado y mira qué
archivos tiene en su directorio home. La orden sudo permite cambiar el modo de trabajo a
modo root específicamente para ejecutar una orden con privilegios de supervisor y tras su
ejecución continuar con los privilegios del usuario que abrió la sesión.
~~~shell
adduser  [options]  [--home  DIR]  [--shell  SHELL]  [--no-create-home]
      [--uid ID] [--firstuid ID] [--lastuid ID] [--ingroup GROUP | --gid  ID]
      [--disabled-password]      [--disabled-login]      [--gecos      GECOS]
      [--add_extra_groups] [--encrypt-home] user

      adduser  --system  [opciones]  [--home  DIRECTORIO]  [--shell  CONSOLA]
      [--no-create-home]  [--uid  ID]  [--group | --ingroup GRUPO | --gid ID]
      [--disabled-password] [--disabled-login] [--gecos GECOS] USUARIO

      addgroup [opciones] [--gid ID] grupo

      addgroup --system [opciones] [--gid ID] grupo

      adduser [opciones] usuario grupo
~~~
Creo el usuario u1 con directorio base otro_basedir, otro_comment, otro directorio home,
fecha de expiración y días de inactividad de la contraseña que pueden pasar antes
de que se elimine la cuenta: `useradd -b /home/u1/otro_basedir -c otro_comment -d /home/otro_home -e 2018-12-16 -f 2 u1`
Vemos que salen cosas distintas que cuando creé el usuario newuser simplemente
haciendo useradd newuser:
~~~shell
newuser:x:500:500::/home/newuser:/bin/bash
u1:x:501:501:otro_comment:/home/otro_home:/bin/bash
~~~
Creo también el usuario u2 que tiene como grupo 500, especifico la opción -N para que
no ponga el mismo grupo que UID, como shell le pongo arch, y de uid 502:
`useradd -g 500 -N -s /bin/arch -u 502 u2`. Esta creación consta en `etc/passwd` de la siguiente manera:
`u2:x:502:500::/home/u2:/bin/arch`.

Elimino newuser: `userdel newuser`, lo que me devuelve
*userdel: group newuser is the primary group of another user and is not removed.*
Al hacer:
`[root@localhost ~]# su newuser` me dice *su: user newuser does not exist*, como queríamos.
No obstante, al mirar su directorio home sigue existiendo:
`[root@localhost ~]# ls /home/newuser/`.

Ahora me meto en el usuario u1, miro los archivos que tiene en su directorio home,
que era /home/otro_home y no la opción por defecto /home/u1 como comprobamos, mas
no aparece ninguno: son todo archivos ocultos, para verlos especifico otra opción
y obtenemos:
~~~shell
[root@localhost ~]# su u1
[u1@localhost root]$ ls /home/otro_home/
[u1@localhost root]$ ls /home/u1
ls: cannot access /home/u1: No such file or directory
[u1@localhost root]$ ls -la /home/otro_home/
total 20
drwx------ 2 u1   u1   4096 Dec 16 06:16 .
drwxr-xr-x 5 root root 4096 Dec 16 06:24 ..
-rw-r--r-- 1 u1   u1     18 Jun 22  2011 .bash_logout
-rw-r--r-- 1 u1   u1    176 Jun 22  2011 .bash_profile
-rw-r--r-- 1 u1   u1    124 Jun 22  2011 .bashrc
~~~

>Cambio de contraseña (password)
Para asignar una contraseña a un usuario se puede usar la orden passwd. Esta orden también se
utiliza para cambiar la contraseña del usuario que la ejecuta si no se le indica ningún argumento.
El usuario administrador puede cambiar las contraseñas de todos los usuarios del sistema,
incluyendo la suya propia, pero, por razones obvias, un usuario normal solamente podrá cambiar
la contraseña asociada a su propia cuenta. A continuación se muestra la orden:
`$> passwd <nombre_usuario>`
Podemos asignar o cambiar el intérprete de órdenes por defecto que usará el usuario cuando se
conecte al sistema. En el último campo del archivo /etc/passwd se establece para cada usuario
el intérprete de órdenes que se ejecuta por defecto cada vez que entra al sistema. En el archivo
/etc/shells se indican los shells permitidos, es decir los que están instalados en el sistema. Con
la orden chsh el usuario puede cambiar su shell (el nuevo ha de estar entre los permitidos). Si un
usuario no tiene asignado ningún intérprete de órdenes, se usará el shell por defecto,
representado por el archivo /bin/sh. Si se desea que el usuario no pueda entrar al sistema se le
puede asignar al campo que indica el shell los nombres de archivo /bin/false o /sbin/nologin.
También se puede establecer como shell un archivo ejecutable, de forma que cuando el usuario
entre al sistema se ejecutará dicho archivo y al finalizar su ejecución se acabará la sesión de
trabajo del usuario.

#### Actividad 1.5. Archivo /etc/shadow
Visualiza el archivo /etc/shadow desde un usuario distinto al root ¿Te da algún problema?
¿Sabes por qué? Intenta averiguarlo. Da problema ya que solo se puede visualizar
estando en modo root:
~~~shell
> cat /etc/shadow
cat: /etc/shadow: Permiso denegado

> ls -lF /etc/shadow
-rw-r----- 1 root shadow 1672 dic 16 12:09 /etc/shadow
~~~

> Parámetros de configuración de una cuenta.
Para las cuentas de los usuarios se pueden establecer restricciones de tiempo, también llamadas
de envejecimiento, respecto a la validez de la cuenta o de la contraseña. Los valores se guardan
en el archivo /etc/shadow. La siguiente tabla muestra los valores posibles.
+ changed fecha del último cambio de contraseña
+ minlife número de días que han de pasar para poder cambiar la contraseña
+ maxlife número de días máximo que puede estar con la misma contraseña sin cambiarla
+ warn cuántos días antes de que la contraseña expire (maxlife) será informado sobre ello,
indicándole que tiene que cambiarla
+ inactive número de días después de que la contraseña expire que la cuenta se deshabilitará
de forma automática si no ha sido cambiada
+ expired fecha en la que la cuenta expira y se deshabilita de forma automática
Los valores los establece el administrador con las órdenes chage o con passwd. Recordemos que
el archivo /etc/login.defs tiene los valores por defecto. La siguiente tabla muestra algunas
opciones y argumentos útiles para la orden chage.
+ chage -d ult_día usuario fecha del último cambio de password
+ chage -m min_días usuario no de días que han de pasar para poder cambiar la
contraseña
+ chage -M max_días usuario no de días máximo que puede estar con la misma
contraseña sin cambiarla
+ chage -W warn_días usuario cuántos días antes de que la contraseña expire (maxlife)
será avisado de ello, indicándole que tiene que cambiarla
+ chage -I inac_días usuario no de días después de que la contraseña expire que la
cuenta se deshabilitará de forma automática si la
contraseña no ha sido cambiada
+ chage -E exp_días usuario fecha en la que la cuenta expira y se deshabilita de forma
automática

###### Gestión de grupos
Un grupo es un conjunto de usuarios que comparten recursos o archivos del sistema. Con los
grupos se pueden garantizar permisos concretos para un conjunto de usuarios, sin tener que
repetirlos cada vez que se desee aplicarlos.
Un grupo se caracteriza por:
+ Nombre del grupo, o groupname
+ Identificador del grupo (GID, del inglés Group Identifier) que es un número que permite
al sistema identificar al grupo (ver sección 4.2).
+ Archivo de configuración /etc/group. Cada línea de este archivo presenta el siguiente
formato: nombre: x:gid:lista de usuarios
>Órdenes relacionadas con la gestión de grupos.
+ groupadd grupo crea un nuevo grupo
+ groupmod grupo modifica un grupo existente
+ groupdel grupo elimina un grupo
+ newgrp grupo cambia de grupo activo (lanza un shell con ese grupo)
+ gpasswd grupo asigna una contraseña a un grupo
+ gpasswd -a user grupo añade un usuario a un grupo
+ groups [usuario] informa de los grupos a los que pertenece un usuario
+ id [usuario] lista el identificador del usuario y los grupos a los que
pertenece
+ grpck comprueba la consistencia del archivo de grupos

#### Actividad 1.6. Creación de grupos
1. Crea un par de grupos y asignáselos a algunos de los usuarios de tu sistema.
2. ¿Qué información devuelve la orden id si estás conectado como root?
~~~shell
[root@localhost ~]# groupadd nuevogrupi
[root@localhost ~]# groups
root bin daemon sys adm disk wheel
[root@localhost ~]# id
uid=0(root) gid=0(root) groups=0(root),1(bin),2(daemon),3(sys),4(adm),6(disk),10(wheel)
[root@localhost ~]# man groupadd
-bash: man: command not found
[root@localhost ~]# cat /etc/group
[...]
nuevogrupi:x:502:
~~~
Le asigno el nuevo grupo a u1:
~~~shell
[root@localhost ~]# usermod -g nuevogrupi u1
[root@localhost ~]# su u1
[u1@localhost root]$ groups
nuevogrupi
~~~

#### Actividad 1.7. Archivo del kernel de Linux
Utilizando la orden (find) que ya conoces para la búsqueda de archivos en el sistema de
archivos, anota el nombre absoluto del archivo del kernel de Linux que se ha cargado en
el sistema operativo que estás usando en el laboratorio de prácticas para acceso modo
root.
~~~shell
[root@localhost ~]# whereis Fedora
Fedora:
[root@localhost ~]# find / -name Fedora
/usr/share/icons/Fedora
[root@localhost ~]# find / -name Fedora14
[root@localhost ~]# find / -name kernel32
~~~

#### Actividad 1.8. Organización del SA
Un programa que se ejecuta en modo root, ¿dónde podría guardar la información temporal de
forma que ésta se mantuviese entre arranques del sistema?
En `/sbin` ya que el tipo de información que almacena en el estándar FHS es:
Programas de utilidad fundamentales para ser utilizados por el usuario root.

>Acceso a información del SO relativa a sistemas de archivos.
Si pensamos en acceso a información del sistema entonces sabemos que, según el estándar FHS,
algo deberíamos encontrar en el directorio /etc. Efectivamente, aquí disponemos de dos
archivos fundamentales para obtener información de los sistemas de archivos: /etc/fstab y
/etc/mtab.

#### Actividad 1.9. Información de los SAs
Los archivos /etc/fstab y /etc/mtab muestran información sobre los sistemas de archivos que
se encuentran montados en el sistema. ¿Cuál es la diferencia entre la información que muestra
cada uno de ellos?
El archivo `/etc/fstab` muestra la lista de discos y
particiones disponibles, indicando cómo montar cada dispositivo y qué
configuración utilizar. El archivo `/etc/mtab` es un archivo de información del sistema, comúnmente en
Unix. Este archivo lista todos los sistemas de ficheros montados junto
con sus opciones de inicialización. mtab tiene mucho en común con fstab, pero la
diferencia principal es que este último enumera todos los sistemas de archivos
disponibles.

>La información que muestra el archivo /etc/fstab es muy útil para comprender las opciones de
montaje que se han realizado para cada uno de los sistemas de archivos que tenemos accesibles
en nuestro sistema. A continuación se muestran algunas de las opciones que refleja este archivo:
+ Modo de acceso a los archivos del sistema de archivos: {rw|ro}, lectura/escritura o sólo
lectura.
+ Modo de acceso SUID: {suid|nosuid}, si/no.
+ Montaje automático: {auto|noauto}, se permite o no el montaje automático. En el caso de
no permitirlo no se realizará el montaje ni utilizando la orden mount -a.
+ Ejecución de archivos: {exec|noexec}, si/no.
+ Cuotas de usuario y de grupo: usrquota, grpquota.
+ Valores por defecto de montaje (defaults): rw, suid, dev, exec, auto, nouser,
async
+ Permitir a los usuarios montar un sistema de ficheros : user, users, owner.
+ Propietario y grupo propietario de los ficheros del SA : uid=500, gid=100
+ Máscara a aplicar en los permisos de los archivos de nueva creación: umask=022




















#
