# MongoDB Stress Test

Este programa permite realizar pruebas de estrés a un servidor de MongoDB mediante la inserción de registros utilizando conexiones concurrentes con un pool de conexiones y la librería Pthreads.

## Prerequisitos

* Un servidor de  MongoDB.
* El compilador `gcc`
* La biblioteca de funciones [`libmongoc`](http://mongoc.org/libmongoc/current/installing.html)

## Instucciones de uso

1. Decargue el repositorio a una carpeta de su computadora utilizando el comando `git clone`.
2. Cámbiese a la carpeta del proyecto.
3. Compile el código con el comando:

`gcc main.c -o test-mongo-c $(pkg-config --cflags --libs libmongoc-1.0) -pthread`
 
 o
 
 `gcc main.c -o test-mongo-c  -I/usr/local/include -lmongoc-1.0 -lbson-1.0 -pthread`
 
4. Ejecute el programa con el comando:

`./test-mongo-c [CONNECTION_STRING] [DATABASE_NAME] [COLLECTION_NAME]`
