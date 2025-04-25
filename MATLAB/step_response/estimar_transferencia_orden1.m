function transf = estimar_transferencia_orden1( ...
    input, output, delta_t, mostrar_calculos ...
)
    % Estima un modelo de transferencia de primer orden

    % Crea un objeto iddata con el paso de tiempo
    data = iddata(output, input, delta_t);
    
    opt = tfestOptions;
    if(mostrar_calculos)
        opt.Display = 'on';
    end
    opt.SearchOptions.Tolerance = 1e-6;
    opt.SearchOptions.MaxIterations = 1000;
    
    % Estimo transferencia con numerador de grado 1, denominador de grado 1
    transf = tfest(data, 1, 0, opt);
end