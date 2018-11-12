## Sesión 6. Control de archivos y archivos proyectados en memoria.
### Actividad 6.1 Trabajo con la llamada al sistema fcntl
**Ejercicio 1** Implementa un programa que admita t argumentos. El primer argumento será una
orden de Linux; el segundo, uno de los siguientes caracteres “<” o “>”, y el tercero el nombre de
un archivo (que puede existir o no). El programa ejecutará la orden que se especifica como
argumento primero e implementará la redirección especificada por el segundo argumento hacia
el archivo indicado en el tercer argumento. Por ejemplo, si deseamos redireccionar la entrada
estándar de sort desde un archivo temporal, ejecutaríamos:
`$> ./mi_programa sort "<" temporal`
Nota. El carácter redirección (<) aparece entre comillas dobles para que no los interprete el
shell sino que sea aceptado como un argumento del programa mi_programa.












### Extra:
Para modificar las banderas, primero invocamos a `fcntl` para obtener una copia de la bandera
existente. A continuación, modificamos el bit correspondiente, y finalmente hacemos una nueva
invocación de `fcntl` para modificarla. Por ejemplo, para habilitar la bandera `O_APPEND` podemos
escribir el siguiente código:
~~~c
int bandera;
bandera = fcntl(fd, F_GETFL);
if (bandera == -1)
  perror(“fcntl”);

bandera |= O_APPEND;
if (fcntl(fd, F_SETFL, bandera) == -1)
  perror(“fcntl”);
~~~
Uno de los usos de la función permite recuperar o modificar el modo de acceso y las banderas de
estado (las especificadas en `open`) de un archivo abierto. Para recuperar los valores, utilizamos la
orden `F_GETFL`:
~~~c
int banderas, ModoAcceso;
banderas=fcntl(fd, F_GETFL);
if (banderas == -1)
perror(“fcntl error”);
~~~
Tras lo cual, podemos comprobar si el archivo fue abierto para escrituras sincronizadas como
se indica:
~~~c
if (banderas & O_SYNC)
printf (”Las escrituras son sincronizadas \n”);
~~~
Comprobar el modo de acceso es algo más complicado ya que las constantes `O_RDONLY`,
`O_WRONLY` y `O_RDWR` no se corresponden con un único bit de la bandera de estado. Por ello,
utilizamos la máscara `O_ACCMODE` y comparamos la igualdad con una de las constantes:
~~~c
ModoAcceso=banderas & O_ACCMODE;
if (ModoAcceso == O_WRONLY || ModoAcceso == O_RDWR)
printf (”El archivo permite la escritura \n);
~~~
La orden `F_DUPFD` de `fcntl` permite duplicar un descriptor, es decir, que cuando tiene éxito,
tendremos en nuestro proceso dos descriptores apuntando al mismo archivo abierto con el
mismo modo de acceso y compartiendo el mismo puntero de lectura-escritura, es decir,
compartiendo la misma sesión de trabajo, como vimos en una sesión anterior (ordenes `dup` y
`dup2`).
Veamos un fragmento de código que nos permite redireccionar la salida estándar de un proceso
hacia un archivo (tal como hacíamos con `dup`, `dup2` o `dup3`):
~~~c
int fd = open (“temporal”, O_WRONLY);
close (1);

if (fcntl(fd, F_DUPFD, 1) == -1 ) perror (“Fallo en fcntl”);

char bufer[256];
int cont = write (1, bufer, 256);
~~~
La primera línea abre el archivo al que queremos redireccionar la salida estándar, en nuestro
ejemplo, temporal. A continuación, cerramos la salida estándar asignada al proceso llamador. De
esta forma, nos aseguramos que el descriptor donde se va a duplicar está libre. Ya podemos
realizar la duplicación de fd en el descriptor 1. Recordad que el tercer argumento de fcntl
especifica que el descriptor duplicado es mayor o igual que el valor especificado. En nuestro
ejemplo, como hemos realizado un close(1), el descriptor duplicado es el 1. Tras definir el búfer
de escritura, realizamos una operación write(1, ...) que escribe en el archivo temporal, ya que
este descriptor apunta ahora al archivo abierto por open.
