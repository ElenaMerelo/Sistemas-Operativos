### Things to remember
#### struct stat
~~~c
struct stat {
dev_t st_dev; /*no de dispositivo (filesystem)*/
dev_t st_rdev; /*no de dispositivo para archivos especiales*/
ino_t st_ino; /*no de inodo*/
mode_t st_mode; /*tipo de archivo y mode (permisos*/
nlink_t st_nlink; /*número de enlaces duros (hard)*/
uid_t st_uid; /*UID del usuario propietario (owner)*/
gid_t st_gid; /*GID del usuario propietario (owner)*/
off_t st_size; /*tamaño total en bytes para archivos regulares*/
unsigned long st_blksize; /*tamaño bloque E/S para el sistema de archivos*/
unsigned long st_blocks; /*número de bloques asignados*/
time_t st_atime; /*hora último acceso*/
time_t st_mtime; /*hora última modificación*/
time_t st_ctime; /*hora último cambio*/
};
~~~
#### Macros POSIX para comprobar el tipo del fichero.

S_ISLNK(st_mode) Verdadero si es un enlace simbólico (soft)
S_ISREG(st_mode) Verdadero si es un archivo regular
S_ISDIR(st_mode) Verdadero si es un directorio
S_ISCHR(st_mode) Verdadero si es un dispositivo de caracteres
S_ISBLK(st_mode) Verdadero si es un dispositivo de bloques
S_ISFIFO(st_mode) Verdadero si es una cauce con nombre (FIFO)
S_ISSOCK(st_mode) Verdadero si es un socket

#### struct dirent
~~~c
struct dirent {
	long d_ino; /*número i-nodo*/
	char d_name[256]; /*nombre del archivo*/
	off_t          d_off;       /*offset to the next dirent*/
	unsigned short d_reclen;    /*length of this record*/
	unsigned char  d_type;      //type of file; not supported
};
~~~
