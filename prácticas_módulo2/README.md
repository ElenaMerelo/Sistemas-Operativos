### Contenidos del módulo 2:
+ Sesión 1- Llamadas al sistema para el sistema de archivos. **`open`, `write`, `lseek`, `lstat`**.

    + Programa en el que se pasa el nombre de un archivo, se abre y se escriben sucesivamente hasta el fin del mismo en otro archivo bloques de 80 bytes. (`ejer2.c`)

    + Programa que dice si el archivo pasado como argumento es regular, un directorio, un dispositivo de caracteres, un bloque, FIFO,  enlace o socket. (`tarea2.c`)

    + Programa en el que se define una macro con la misma funcionalidad que `S_ISREG(mode)`. (`ejer4.c`)

+ Sesión 2- Llamadas al sistema para el SA (parte II). **`umask`, `chmod`, `opendir`, `readdir`, `nftw`**.

    + Programa al que se le pasa el pathname de un directorio y un número octal de 4 dígitos, y tiene que usar dicho número para cambiar los permisos de todos los archivos que se encuentren en el directorio indicado en el primer argumento, proporcionando una línea en la salidad estándar para cada archivo con su nombre, permisos antiguos y permisos nuevos.(`ejer2,2.c`).

    + Programa que recorre la jerarquía de subdirectorios existentes a partir de uno dado como argumento (o el actual) y devuelve la cuenta de todos aquellos archivos regulares que tengan permiso de ejecución para el grupo y others. Además del nombre de los archivos encontrados, devolver inode y suma total de espacio ocupado.(`ejer3.c`).

    + Programa igual al anterior pero usando nftw.(`ejer4_antonio.c`)

    + Contenido de `struct stat` y `struct dirent`.

+ Sesión 3- Llamadas al sistema para el control de procesos.
**`getpid`, `getppid`, `wait`, `fork`, `write`, `setvbuf`, familia de llamadas al sistema `exec`.**.

    + Programa que tenga como argumento un entero y cree un proceso hijo que se encargue de comprobar si es par o impar, y el proceso padre de si es divisible por 4. Ambos informan de si lo es o no por la salida estándar.(`ejer1.c`)

    + Programa que crea un hijo que incrementa dos variables, una global y otra local. (`tarea4.c`)

    + Programa que crea 20 hijos, cada uno que crea es el hijo del anterior, y otro 20 hijos en el mismo nivel con el mismo padre.(`ejer3.c`).

    + Programa que lanza 5 procesos hijo, los cuales se identifican en la salida estándar, y el padre tiene que esperar la finalización de todos ellos, detectando cada vez que termine uno.(`ejer4.c`).

    + Programa como el anterior pero que espera primero a los hijos impares y luego a los pares. (`ejer5.c`).

    + Programa que ejecuta desde `/usr/bin/ldd` `tarea5`, mostrando con `ldd` las bibliotecas dinámicas. (`tarea5.c`).

    + Programa que acepta como argumentos el nombre de un programa, sus argumentos y opcionalmente la cadena bg, y ejecuta el programa en foreground si no se ha puesto la anterior cadena o en background en caso contrario. (`ejer7.c`).

+ Sesión 4- Comunicación entre procesos usando cauces.
**`mknod`, `mkfifo`, `pipe`, `dup`, `dup2`**.

    + Programa que modela el problema del productor consumidor, acepta datos del productor hasta que mande la cadena fin. (`consumidorFIFO.c`, `productorFIFO.c`).

    + Programa que crea un hijo, el cual manda un mensaje a su padre a través de un cauce sin nombre. (`tarea6.c`).

    + Programa que realiza la misma funcionalidad que `ls|sort`, con un cauce sin nombre entre un proceso hijo y su padre, redireccionando la salida y entrada estándar con `close` y `dup` o con `dup2`. (`tarea7.c`, `tarea8.c`).

    + Programa maestro al que se le pasan los extremos de un intervalo y divide el intervalo en dos, asignando cada mitad a otro programa esclavo que ha de calcular los primos que hay en el mismo. (`maestro.c`, `esclavo.c`).

+ Sesión 5. Llamadas al sistema para gestión y control de señales.**`sigaction`, `kill`,`sigemptyset`, `sigsuspend`, `sigprocmask`, `sigfillset`, `sigdelset`, `memset`**.

    + Programa `send_signal.c` que permite el envío de una señal a un proceso identificado por su PID. `get_signal.c` se ejecuta en background y permite la recepción de señales.

    + Programa contador tal que cada vez que recibe una señal que se puede manejar, la muestra por pantalla y el número de veces que se ha recibido ese tipo de señal. (`contador.c`).

    + Programa que suspende la ejecución del programa actual hasta que reciba la señal `SIGUSR1`. (`ejer3.c`).

    + Programa que establece un conjunto de señales bloqueadas, desactiva la señal `SIGTERM` durante 10 segundos y restablece la máscara original. (`tarea12.c`)

    + Programa en el que se suspende la ejecución del proceso actual hasta que reciba una señal distinta a `SIGUSR1`. (`tarea11.c`).

+ Sesión 6. Control de archivos y archivos proyectados en memoria. **`fcntl`,**.

    + Programa que admite una orden de linux, el símbolo '<' ó '>' y el nombre de un archivo, y ejecutará la orden especificada en el primer argumento con la redirección indicada en el segundo argumento hacia el archivo. (`ejer1.c`).

    + Programa que implementa el encaucamiento de dos órdenes linux, como el hecho para `ls|sort`, pero admitiendo cualesquiera dos órdenes. (`ejer2.c`).

    + Programa que verifica que el kernel comprueba que puede darse una situación de interbloqueo en el bloqueo de archivos. (`ejer3.c`).

    + Programa que se asegura de que solo hay una instancia de él en ejecución en un momento dado. (`ejer4.c`).

    + Programa  que crea un archivo denominado Archivo y los rellena con nulos, creando una proyección compartida del archivo para que los cambios se mantengan. Una vez establecida la proyección, copia en la memoria asignada a la misma el mensaje “Hola Mundo\n”. Tras finalizar el programa, podemos visualizar el archivo para ver cual es el contenido: la cadena “Hola Mundo\n”. (`tarea14.c`).

    + Programa que se usa para visualizar un archivo completo, que pasamos como primer argumento (similar a la orden cat) y muestra el contenido del byte cuyo desplazamiento se pasa como segundo argumento. Vemos así cómo, una vez establecida la proyección, podemos acceder a los datos que contiene con cualquier mecanismo para leer de memoria. (`tarea15.c`).

    + Programa donde un proceso padre crea una proyección que se utiliza para almacenar un valor. El padre asignará valor a cnt y el hijo solo leerá el valor asignado por el padre (lo hacemos así -solo lectura en el hijo- para evitar condiciones de carrera y así evitarnos tener que introducir un mecanismo de sincronización para acceder al contador). También podemos ver cómo el archivo contiene el valor modificado del padre. (`tarea16.c`).

    + Programa que implementa una proyección anónima. (`tarea17.c`).

    + Proyección anónima con `/dev/zero`. (`tarea18.c`).

    + Programa que muestra la forma habitual de manejar un archivo que está creciendo: especifica una proyección mayor que el archivo, tiene en cuenta su tamaño actual (asegurándonos de no hacer referencias a posiciones posteriores al fin de archivo), y deja que se incremente el tamaño del archivo conforme se escribe en él. (`tarea19.c`).
