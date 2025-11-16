# Entregable final Sistemas operativos

## Integrantes:

* Santiago Sanchez Carvajal
* Samuel Velez Gaviria
* El maxi MC

## Resumen

Este proyecto implementa un sistema de encriptación y compresión de archivos desarrollado en C/C++, diseñado para ejecutarse en entornos Linux y orientado a aplicar conceptos vistos en la asignatura de sistemas operativos. El objetivo principal es permitir el procesamiento eficiente de múltiples archivos mediante paralelismo, utilizando técnicas como:

* Encriptación y desencriptación de archivos mediante el algoritmo de Vigenère modificado para bytes.
* Compresión y descompresión usando Huffman, con serialización completa del árbol.
* Manejo manual de archivos, buffers y operaciones de bajo nivel (open, read, write).
* Procesamiento paralelo, permitiendo ejecutar en simultáneo operaciones sobre varios archivos.
* Gestión eficiente de memoria y estructuras dinámicas.

El programa ofrece seis operaciones principales:

* Encriptar archivos
* Desencriptar archivos
* Comprimir archivos
* Descomprimir archivos
* Encriptar + Comprimir
* Descomprimir + Desencriptar

Estas funciones permiten crear una herramienta flexible para el manejo seguro y eficiente de información, optimizada para procesar múltiples archivos en paralelo mediante hilos.


## Requisitos

Es recomendable emplear un sistema operativo Linux como Ubuntu, un compilador de C y la librería de pthreads.


## Copilar y ejecutar

Para compilar el código es necesario ejecutar el siguiente comando:

`g++ main.cpp -o programa`

Esto garantiza que se importen las librerías de pthread para trabajar con hilos.

Finalmente, ejecuta el siguiente comando para correr el programa:

`./programa`

## Ejemplo de uso

### Paso 1: Ejecutar
<img width="810" height="306" alt="image" src="https://github.com/user-attachments/assets/cbda093e-6cfd-4172-b951-0d8521c6b44d" />
<img width="973" height="184" alt="image" src="https://github.com/user-attachments/assets/45902fee-b445-4ffb-a8ac-2131285beba6" />


### Importante: el archivo a encriptar debe ser un .txt

## Paso 2: Encriptar y Comprimir

<img width="456" height="394" alt="image" src="https://github.com/user-attachments/assets/3ab04a2e-dc2f-443a-bd99-a27b6d2c59f1" />

En este paso creamos el archivo encriptado y lo comprimimos con una key la cual creamos en el arbol de huff

<img width="1370" height="162" alt="image" src="https://github.com/user-attachments/assets/9d920b5b-9646-4418-b1aa-06bab411fcad" />


## Paso 3: Descomprimir y Desencriptar

<img width="546" height="386" alt="image" src="https://github.com/user-attachments/assets/15978c9e-e903-4e74-b7a4-34f93edb147c" />

Importante: Usar la misma clave que usamos para el arbol

<img width="1589" height="184" alt="image" src="https://github.com/user-attachments/assets/38cd8d1c-2e49-4789-9dca-c47ab8285f11" />


## Resultado

### prueba.txt:
<img width="1904" height="864" alt="image" src="https://github.com/user-attachments/assets/3e55d8e2-e729-4493-8dc2-23ef803554a5" />

### resultado.txt:

<img width="1914" height="875" alt="image" src="https://github.com/user-attachments/assets/7cf78501-b12d-4ae1-bdfe-97d84f14f7c3" />



