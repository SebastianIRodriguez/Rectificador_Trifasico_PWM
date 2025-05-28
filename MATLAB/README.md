# Guia para simular el Rectificador Trifasico PWM

## Introduccion
En este repositorio se modela el Rectificador Trifasico PWM presente en el Laboratorio de Electronica de el edificio de Civil - Electronica en La Siberia.
La idea es poder realizar una simulación del equipo para poder realizar el ajuste de su sistema de control y posteriormente verificar su funcionamiento en distintas condiciones de trabajo.
El rectificador en si, consiste de un puente de IGBTs en conjunto con 3 inductores de desacople.
El sistema de control se realiza en el plano dq. Cuenta con dos lazos anidados de control, un control interno de corriente (en fase y cuadratura) y un control externo de tension del bus de continua.
Para modular el convertidor se utiliza un bloque de modulación vectorial.
La frecuencia de conmutacion elegida es de 10kHz. Para simular el convertidor, se utilizará su modelo promediado.

## Composicion del repositorio
A continuacion se detallan los distintos archivos presentes en el repositorio

### parametros.m
En este archivo se configuran las principales caracteristicas electricas del convertidor, asi como los parámetros de los controladores.

### simulacion.m
Aqui se encuentra el codigo que ejecuta la simulacion, pudiendose identificar 5 situaciones distintas.

- **Solo Feedforward:** El equipo replica la tension medida de la red, por lo que no hay transferencia de potencia. Del lado de continua se conecta una fuente de tension que mantiene la tension del bus constante. En cuanto al sistema de control, solo se encuentran activas las compensaciones feedforward de corriente y tension.

- **Lazo Cerrado - Controladores Ideales:** El equipo opera a lazo cerrado. Los parametros de los controles PI son calculados a partir del modelo teorico del equipo.

- **Lazo Abierto - Analisis de corriente:** Solo se habilitan las compensaciones de feedforward del sistema de control, desactivando los lazo de control anidados. En lo que serian las salidas de los PI de corriente, se configuran valores iniciales que corresponden con un determinado punto de trabajo. En determinado momento, se aplica un escalon en la salida correspondiente del controlador de id (lo que corresponderia a la componente en d de la tension que debe generarse). Se mide Vd e Id y se devuelve como salida de la simulacion. Esto posteriormente permite encontrar la transferencia Vd -> Id, que corresponderia al modelo de la planta utilizado para el ajuste de los controladores de corriente.

- **Lazo Abierto - Analisis de tension de bus** Similar al caso anterior, solo que aqui solo se desactiva el lazo mas externo, mantieniendo operativos el lazo interno de control de corriente junto con las compensaciones de feedforward. Inicialmente se aplica cierto valor en la referencia de id, que corresponde a cierto punto de trabajo. En determinado instante se aplica un escalon a esta referencia. Se mide este escalon junto con Vbus y se reporta como salida de la simulacion. Esto se utiliza para obtener la transferencia Id -> Vbus, que se toma como modelo de la planta para el ajuste del controlador de tension del bus.

- **Lazo Cerrado - Cambio en setpoint de corriente:** El lazo interno de control de corriente opera a lazo cerrado. El control de tension de bus se encuentra desactivado. Los parametros de los controles PI son calculados a partir del modelo experimental del equipo, obtenido a través de los ensayos de respuesta al escalón. En este modo, a partir de cierto tiempo, se aplica un cambio en el setpoint de la corriente id (el setpoint tiene un valor inicial seteado manualmente, al no estar activo el control de tension de bus). Asi, se puede evaluar el rendimiento del sistema de control corriente.

- **Lazo Cerrado - Cambio en setpoint de tension:** El equipo opera a lazo cerrado. Los parametros de los controles PI son calculados a partir del modelo experimental del equipo, obtenido a través de los ensayos de respuesta al escalón. En este modo, a partir de cierto tiempo, se aplica un cambio en el setpoint de la tension del bus de continua. Asi, se puede evaluar el rendimiento del sistema de control de tension.

### step_response_id.m
En este archivo se analizan los resultados de la simulacion del escalon en Vd. Se estima la transferencia Vd->Id y se contrasta con su forma teorica. Luego se calculan los parametros del controlador a partir del modelo obtenido experimentalmente.

### step_response_vdc.m
En este archivo se analizan los resultados de la simulacion del escalon en Id. Se estima la transferencia Id->Vbus y se contrasta con su forma teorica. Luego se calculan los parametros del controlador a partir del modelo obtenido experimentalmente.


## Criterio de ajuste de los controladores
Se realiza en base a 2 criterios: tiempo de respuesta y comportamiento a lazo cerrado.
### Tiempo de Respuesta
Se parte de la observación de que estamos limitados por la frecuencia de conmutacion de 10kHz.
A su vez, el PLL utilizado para realizar la sincronizacion con la red, tiene un tiempo de respuesta de aproximadamente 10ms.
Como criterio, se propone que la separacion entre los tiempos de respuesta de los controladores sea de una decada. Es decir, si el tau del PLL es de 10ms, el tau de los controladores de corriente será de 100ms y el del controlador de tension del bus será de 1000ms = 1s.
### Comportamiento a lazo cerrado
Se eligen los parametros de los PI (Kp y Ki), de forma que la respuesta a lazo cerrado sea de primer orden.

## Valores en Por Unidad (PU)
En general, todas las corrientes y tensiones se miden en el formato Por Unidad (abreviado PU). Es decir todos los valores medidos, se dividen por un valor base o de referencia. Por ejemplo, si se mide una tension de 40V en determinado momento y el valor base es de 80V, entonces esa tension expresada en pu es de 0.5pu.

## Comentarios adicionales
* A la hora de ejecutar la simulacion, asegurese de que, dentro del modelo de Simulink, en la sección "Simular", el modo de simulación este configurado como "Acelerador". Esto permite lograr tiempos de simulación razonables, ya que el modelo se transpila a lenguaje C, lo que permite obtener un código mucho más eficiente.
* A la hora de identificar la transferencia a partir de las respuestas al escalon, se utiliza la función "tfest" de MATLAB. Esta función es sensible a artefactos que puedan encontrarse sobre la respuesta a estimar. Por esto, a veces es necesario realizar distintos trabajos de post-proceso sobre la señal de forma de suavizar la curva y lograr mejores estimaciones.
* Tenga cuidado con la configuracion de los parametros del snubber de los IGBTs. Si los valores no son adecuados, pueden generarse oscilaciones inexplicables dentro del circuito.