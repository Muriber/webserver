Este proyecto consiste en codificar un servidor HTTP en C++98. 

El ejecutable funciona de la siguiente manera: 

./webserv [configuration file]

## Características:

- El programa toma un archivo de configuración como argumento, o usa la una ruta predeterminada en caso de que no se indique archivo.
- El servidor no bloqueará y el cliente será rechazado correctamente si es necesario.
- Es no bloqueante y usa solo 1 poll() para todas las operaciones de E/S entre el cliente y el servidor (escucha incluida).
- El poll() verifica la lectura y la escritura al mismo tiempo y todas las operaciones de lectura o escritura pasan por poll().
- El servidor es compatible con el navegador web Chrome ( y otros).
- Se contemplan códigos de estado de respuesta HTTP precisos así como páginas de error predeterminadas (en caso de que no se proporcionasen).
- Tan solo contempla sitios webs totalmente estáticos.
- Es posible que los clientes suban archivos.
- Implementa los métodos GET, POST y DELETE.
- Además, el servidor puede escuchar en múltiples puertos.

## Archivo de configuración.

Se encuentra inspirado en la parte 'server' del archivo de configuración de NGINX. En el archivo de configuración, es posible:
- Elegir el puerto y host de cada 'server'. 
- Configurar los server_names o no.
- El primer servidor para un host es el predeterminado para este host (lo que significa que responderá a todas las solicitudes que no pertenezcan a otro servidor).
- Configuración de páginas de error predeterminadas.
- Limitar el tamaño del cuerpo del cliente.
- Configurar rutas con las siguientes reglas/configuración:
	* Definición de una lista de métodos HTTP aceptados para la ruta.
	* Definición de una redirección HTTP.
	* Definición de un directorio o archivo desde donde se debe buscar el archivo. 
	* Activar o desactivar la lista de directorios.
	* Establecer un archivo predeterminado para responder si la solicitud es un directorio.
	* Ejecutar CGI basado en ciertas extensiones de archivo (por ejemplo, .php).
	◦ Hacer que funcione con los métodos POST y GET.
