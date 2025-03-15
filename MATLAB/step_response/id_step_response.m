%% Datos
time_of_step = 0.4;
t = vd_id_step_response.time;
idx = find(t == 0.39);
%idx = 1;
vgd = vd_id_step_response.signals(1).values(idx:end);
id = vd_id_step_response.signals(2).values(idx:end);
t = vd_id_step_response.time(idx:end);
delta_t = t(2) - t(1);

figure(1);
subplot(2,1,1), plot(t, vgd,'LineWidth',1.5), grid on;
ylabel("V_G_D"), xlabel("t[s]");
subplot(2,1,2), plot(t, id,'LineWidth',1.5), grid on;
ylabel("I_D"), xlabel("t[s]");
xline(step_time,'r--','LineWidth',1.25)

%% Plotear respuesta al escalon: Transferencia Vd -> Id

figure(2)
hold on;
input = -vgd;
output = id;
output = -output;
output = lowpass(output, 20, 1 / delta_t, ImpulseResponse="iir",Steepness=0.85);
output = movmean(output,100);
output = output - output(find(t==time_of_step));

subplot(2,1,1), plot(t, input,'LineWidth',1.5), grid on;
ylabel("V_G_D"), xlabel("t[s]");
subplot(2,1,2), plot(t, output,'LineWidth',1.5), grid on;
ylabel("I_D"), xlabel("t[s]");
xline(step_time,'r--','LineWidth',1.25)

%% Comparar plantas y calcular parametros del controlador
% Define los datos
data = iddata(id, vgd, delta_t); % Crea un objeto iddata con el paso de tiempo

% Estima un modelo de transferencia de primer orden
opt = tfestOptions;
%'gn' | 'gna' | 'lm' | 'grad' | 'lsqnonlin' | 'fmincon'
%opt.SearchMethod = 'grad';
opt.Display = 'on';
opt.SearchOptions.Tolerance = 0.000000001;
opt.SearchOptions.MaxIterations = 1000;
opt.InputOffset = vgd(1);
opt.OutputOffset = id(find(t==time_of_step));
%'iv' (default) | 'svf' | 'gpmf' | 'n4sid' | 'all'
%opt.InitializeMethod = 'svf';
init_sys = idtf(H);
%opt.ErrorThreshold = 1.6;
sys = tfest(data, 1, 0, opt) % Numerador de grado 1, denominador de grado 1

s = tf('s');
G = 1 / (L*s + (R + ron)); % Hp = Vgdq / Idq

% Muestra el modelo
%ltiview(sys, G)
%%
den = sys.Denominator / sys.Numerator;
L_exp = den(1);
R_exp = den(2);

ContI_Exp.tau = max(tau_conmut * 10, tau_pll * 10);
ContI_Exp.Kp = L_exp / ContI_Exp.tau;
ContI_Exp.Ki = R_exp / ContI_Exp.tau;


% COMPARACION DE RESULTADOS
% tau = 0.01e-3
%
% ContI = 
%     tau: 1.5915e-04
%      Kp: 3.1416
%      Ki: 12.5664
% 
% ContI_Exp = 
%     tau: 1.5915e-04
%      Kp: 2.6924
%      Ki: 13.5332

%% Compara respuesta al escalon medida vs estimada
% Calcular respuesta al escalon segun transferencia ideal y estimada
H = 140 / (0.15*s + 1);
opt = RespConfig;
%opt.InputOffset = vgd(1);
opt.Delay = time_of_step;
opt.Amplitude = vgd(end) - vgd(1);
[y_real, t_real] = step(sys, 0:delta_t:1.5, opt);
[y_ideal, t_ideal] = step(H, 0:delta_t:1.5, opt);

figure(2)

subplot(1,2,1), plot(t, vgd,'LineWidth',1.5), grid on;
ylabel("V_G_D"), xlabel("t[s]");
title( ...
    "Respuesta al Escalon: Corriente I_D", ...
    "Punto de Trabajo: I_D=-50 A - Escalon del 10% en variable manipulada" ...
)

subplot(1,2,2);
plot(t, id, t_ideal, y_ideal + id(find(t==time_of_step)), t_real, y_real  + id(find(t==time_of_step)), 'LineWidth',1.5), grid on;
ylabel("I_D"), xlabel("t[s]"), hold on;
xline(time_of_step,'r--','LineWidth',1.25);
legend("Medido", "Teorico", "Estimado");