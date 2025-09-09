# Steps y Aclaraciones del funcionamiento, compilacion y ejecucion de la Shell y el Monitor 

## Proceso para compilar y Ejecutar el programa principal MyShell:

### Uso de Conan, CMake y Make para la compilación del proyecto

Para compilar un proyecto utilizando Conan, CMake y Make:

1. **Conan**: Se encarga de gestionar las dependencias del proyecto.
2. **CMake**: Se utiliza para configurar y gestionar el proceso de compilación, indicando cómo deben compilarse los archivos y qué dependencias usar.
3. **Make**: Se usa finalmente para realizar la compilación, siguiendo las instrucciones de CMake.

#### **Archivos CMakeLists.txt**

El proyecto tiene **dos archivos CMakeLists.txt**:
- **CMakeLists.txt principal**: Ubicado en el directorio raíz del proyecto.
- **CMakeLists.txt en subdirectorio /test**: Se usan para configurar la compilación de los tests.

### **Dependencias en Conan**

En el archivo **conanfile.txt**, se definen todas las dependencias necesarias para el proyecto. Luego, en los archivos **CMakeLists.txt**, se integran estas dependencias y se indica cómo deben ser utilizadas en la compilación. En este caso las dependencias son Unity y cJSON.

### **Configuración del Proyecto**

Para configurar el proyecto se debe ejecutar los siguientes comandos en la terminal:

```bash

conan install . --output-folder=build --build=missing

```
Lo cual crea la carpeta build donde no solo contiene las dependencias descargadas y compiladas, sino también otros archivos necesarios para la configuración de la compilación.

```bash

cd build

cmake .. -DCMAKE_TOOLCHAIN_FILE=./build/Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release

make -j$(nproc)

```

Lo que genera un ejecutable que se puede correr con el comando: 

```bash
./MyShell

```
## Compilacion y ejecucion de los tests:

Para esto se crea el CMakeLists.txt donde se especifica como van a compilarse, el codigo fuente y se declaran los tests en ctest. Desde el directorio **build** se corren los siguientes comandos:

```bash
cmake .. -DRUN_TESTS=ON

make -j$(nproc)

ctest
```

## Comunicacion entre La shell y el Monitor:

El proyecto del monitor se agrego como un submodulo. Este es objeto de una interaccion con la Shell por medio de dos archivos: internCommand.c y shell.c. 

El primero tiene 3 métodos **start_monitor**, **stop_monitor** y **status_monito** los cuales inician, detienen y devuelven el estado del programa de monitoreo por medio de llamadas de sistema y signals. Estas interacciones se engloban en el metodo **shell_interaction** para ser ejecutadas.

En el segundo, primero se leen las configuraciones de parametros en el **config.json**, son guardados en un args[] y luego se ejecuta el programa metrics pasandole esos argumentos.

## Compilacion y ejecucion del Monitor:

El proyecto de monitoreo tiene un MakeFile conteniendo los pasos necesarios para su compilación. Por lo que si se quiere compilar solo, desde la raiz del proyecto se utilizan los siguentes comandos:

```bash
cd submodulos/'Tp1 SO1'

make 
```

Y para su ejecucion se tienen 2 opciones:

1) Ejecutar sin argumentos: esto le indica al programa que se quiere crear y exponer todas las metricas posibles.

2) Ejecutar con argumentos: pasarle parametros para su configuracion.
    El 1° es el **sample_time** que le indica cada cuanto tiempo se toman los datos. Por default esta se realiza cada 5 segundos.
    El 2° es para aclarar si se quiere o no la metrica de memory_total ya que esta se crea una sola vez. Acepta valores booleanos 0 si no y 1 si se quiere.
    El 3° es un arreglo de los nombres de las metricas que se quieren crear y exponer.
    Ejemplo:

    ```bash
    ./metrics 10 1 cpu_usage disk_writes IO_reads
    ```