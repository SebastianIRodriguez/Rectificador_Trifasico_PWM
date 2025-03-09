function [tita,tao,K] = cohen_coon(t,y,z,t0)
    % Programa que calcula los parametros de la funcion de transferencia 
    % a partir del metodo de Cohen - Coon.
    % t     vector de tiempos de la simulacion
    % y     vector de la respuesta de la variable en estudio
    % z     vector de la variable excitada
    % t0    tiempo en que se excita la variable de entrada
    
    %Graficas
    figure(1);
    subplot(2,2,1), plot(t,z), title('Variable modificada'), grid on;
    subplot(2,2,2), plot(t,y), title('Respuesta de la variable en estudio'), grid on;
    
    %Calculo de la recta tangente
    dy = diff(y) ./ diff(t); %Derivada de la respuesta respecto del tiempo. 

    if y(1) - y(length(y)) < 0
        tg = max(dy); %Tangente en el punto de inflexion cuando la respuesta es inversa.
    else
        tg = min(dy); %Tangente en el punto de inflexion cuando la respuesta es directa.
    end
    aux1 = find(dy==tg); %Ubicacion de la tg en el vector dy.
    aux1 = aux1(1);
    t1 = t(aux1+1); %Valor del tiempo en el punto de inflexion.
    y1 = y(aux1+1); %Valor de la respuesta en el punto de inflexion.
    aux2 = y1 - tg * t1; %Ordenada al origen.
    r = tg * t + aux2;

    subplot(2,2,3), plot(dy), hold on, plot(y), grid on;
    subplot(2,2,4), plot(t,y,'b',t,r,'r',t1,y1,"*"), grid on;
    legend('Derivada de la respuesta','Recta tangente al punto de inflexion')
    text(t1,y1,'Punto Inflexion')
    axis([0 max(t) min(y) max(y)])
    
    %Calculo de los parametros
    tita = ( y(1) - aux2 ) / tg - t0;
    final = length(y);
    tao = ( y(final) - aux2 )/ tg - ( tita + t0 );
    K = ( y(length(y) )- y(1) ) / ( z(length(z)) - z(1) );
    
    disp('Parametros de funcion de transferencia: tpo muerto(tita), tau y ganancia (K)')
