%% Plotear respuesta al escalon: Transferencia Id -> Vdc
time_of_step = 0.25;

t = id_vdc_step_response.time;
time_step_index = find(t == time_of_step);
t_final = t(end);
idx = find(t == 0.00);
t = t(idx:end);

id = id_vdc_step_response.signals(1).values(idx:end);
vdc = id_vdc_step_response.signals(2).values(idx:end);

input = id;
output = vdc;
output = output - output(1);

figure(20)
subplot(2,1,1), plot(t, input,'LineWidth',1.5), grid on;
ylabel("I_D"), xlabel("t[s]");
subplot(2,1,2), plot(t, output,'LineWidth',1.5), grid on;
ylabel("V_D_C"), xlabel("t[s]");
xline(time_of_step,'r--','LineWidth',1.25)

%% Obtencion de transferencias
% TRANSFERENCIA REAL A PARTIR DE RESPUESTA AL ESCALON

% Define los datos
delta_t = t(2) - t(1);

% Estima un modelo de transferencia de primer orden
sys = estimar_transferencia_orden1(id, vdc, delta_t, true);

% TRANSFERENCIA TEORICA
s = tf('s');
Z_load = config.vsc.R_load;
Z_cap = 1 / (s * config.vsc.C);
Z_dc = Z_cap * Z_load / (Z_cap + Z_load);

%% Comparacion de bodes de ambas transferencias
% Muestra el modelo
ltiview(sys, Z_dc)

%% Comparar respuestas al escalon
% Calcular respuesta al escalon segun transferencia ideal y estimada
opt = RespConfig;
opt.Delay = time_of_step;
opt.Amplitude = id(end) - id(1);
[y_real, t_real] = step(sys, 0:delta_t:t_final, opt);
[y_ideal, t_ideal] = step(Z_dc, 0:delta_t:t_final, opt);

figure(20)
subplot(2,1,1), plot(t, input,'LineWidth',1.5), grid on;
ylabel("I_D"), xlabel("t[s]");
title( ...
    "Respuesta al Escalon: Bus de CC", ...
    "Punto de Trabajo: Vdc=300 V - Escalon del 10% en variable manipulada" ...
)
subplot(2,1,2);
plot(t, output, t_ideal, y_ideal, t_real, y_real, 'LineWidth',1.5), grid on;
ylabel("V_D_C [V]"), xlabel("t[s]"), hold on;
xline(time_of_step,'r--','LineWidth',1.25);
legend("Medido", "Teorico", "Estimado");

%% Parametros del controlador
den = sys.Denominator / sys.Numerator;
C_exp = den(1);
R_exp = 1 / den(2);

ContDC_Exp.tau = ContI_Exp.tau * 10;
ContDC_Exp.Ki = ContDC_Exp.tau / R_exp;
ContDC_Exp.Kp = C_exp * ContDC_Exp.tau;

ContDC_Ideal
ContDC_Exp

%% GUARDAR DATOS DEL CONTROLADOR
save("datos/parametros_control_tension_experimental","ContDC_Exp")

%% EXPORTAR FEEDFORWARD TENSION
M = [t output y_ideal];
M = downsample(M,100);
T = array2table(M);
T.Properties.VariableNames(1:3) = {'tiempo','vdc_medido', 'vdc_teorico'};
writetable(T,'Para el informe/respuesta_escalon_tension_bus.csv')