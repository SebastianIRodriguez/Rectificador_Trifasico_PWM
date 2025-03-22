%% Plotear respuesta al escalon: Transferencia Vd -> Id
time_of_step = 0.1;
id = id_vdc_step_response.signals(1).values;
vdc = id_vdc_step_response.signals(2).values;
t = id_vdc_step_response.time;
time_step_index = find(t == time_of_step);

figure(2)
hold on;
input = id;
output = vdc;
output = output - output(time_step_index);
subplot(2,1,1), plot(t, input,'LineWidth',1.5), grid on;
ylabel("I_D"), xlabel("t[s]");
subplot(2,1,2), plot(t, output,'LineWidth',1.5), grid on;
ylabel("V_D_C"), xlabel("t[s]");
xline(time_of_step,'r--','LineWidth',1.25)

%% Comparar plantas y calcular parametros del controlador
% Define los datos
delta_t = t(2) - t(1);
data = iddata(vdc, id, delta_t); % Crea un objeto iddata con el paso de tiempo

% Estima un modelo de transferencia de primer orden
opt = tfestOptions;
%opt.Display = 'on';
opt.SearchOptions.MaxIterations = 1000;
sys = tfest(data, 1, 0, opt); % Numerador de grado 1, denominador de grado 1

%Transferencia teorica
s = tf('s');
Z_load = R_load;
Z_cap = 1 / (s * C_filtro_CC);
Z_dc = Z_cap * Z_load / (Z_cap + Z_load);

% Muestra el modelo
ltiview(sys, Z_dc)

%% Compara respuesta al escalon medida vs estimada
% Calcular respuesta al escalon segun transferencia ideal y estimada
opt = RespConfig;
opt.Delay = time_of_step;
opt.Amplitude = id(end) - id(1);
[y_real, t_real] = step(sys, 0:delta_t:1, opt);
[y_ideal, t_ideal] = step(Z_dc, 0:delta_t:1, opt);

figure(2)

subplot(2,1,1), plot(t, input,'LineWidth',1.5), grid on;
ylabel("I_D"), xlabel("t[s]");
title( ...
    "Respuesta al Escalon: Bus de CC", ...
    "Punto de Trabajo: Vdc=300 V - Escalon del 2% en variable manipulada" ...
)

subplot(2,1,2);
plot(t, output, t_ideal, y_ideal, t_real, y_real, 'LineWidth',1.5), grid on;
ylabel("V_D_C [V]"), xlabel("t[s]"), hold on;
xline(time_of_step,'r--','LineWidth',1.25);
legend("Medido", "Teorico", "Estimado");