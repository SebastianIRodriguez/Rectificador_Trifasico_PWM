%% Datos brutos
time_of_step = 0.2;
t = vd_id_step_response.time;
t_final = t(end);
idx = find(t == 0.18);

vgd_med = vd_id_step_response.signals(1).values;
id_med = vd_id_step_response.signals(2).values;
t_med = vd_id_step_response.time;

vgd = vgd_med(idx:end);
id = id_med(idx:end);
id = lowpass(id, 200, 1 / delta_t, ImpulseResponse="iir",Steepness=0.85);
t = t_med(idx:end);
delta_t = t(2) - t(1);

figure(1);
subplot(2,1,1), plot(t, vgd,'LineWidth',1.5), grid on;
ylabel("V_G_D"), xlabel("t[s]");
title("Datos brutos")
subplot(2,1,2), plot(t, id,'LineWidth',1.5), grid on;
ylabel("I_D"), xlabel("t[s]");
xline(time_of_step,'r--','LineWidth',1.25)

%% Obtencion de transferencias

% TRANSFERENCIA REAL A PARTIR DE RESPUESTA AL ESCALON
sys = estimar_transferencia_orden1(vgd, id, delta_t, false);

% TRANSFERENCIA TEORICA
s = tf("s");
H = 1 / (config.vsc.L*s + config.vsc.R);

%% Comparativa frecuencial entre transferencia teorica vs experimental
ltiview(sys, H)

%% Compara respuesta al escalon medida vs estimada

idx_step = find(t == time_of_step);
id_0 = mean(id(1:idx_step));

%Simular respuestas al escalon
opt = RespConfig;
%opt.InputOffset = vgd(1);
opt.Delay = time_of_step;
opt.Amplitude = vgd(end) - vgd(1);
[y_real, t_real] = step(sys, 0:delta_t:t_final, opt);
[y_ideal, t_ideal] = step(H, 0:delta_t:t_final, opt);


% Plotear comparativa
figure(3)
subplot(1,2,1), plot(t, vgd,'LineWidth',1.5), grid on;
ylabel("V_G_D"), xlabel("t[s]");
title( ...
    "Respuesta al Escalon: Corriente I_D", ...
    "Punto de Trabajo: I_D=-50 A - Escalon del 15% en variable manipulada" ...
)

subplot(1,2,2);
plot( ...
    t_med, id_med, ...
    t_ideal, y_ideal + id_0, ...
    t_real, y_real  + id_0, ...
    'LineWidth',1.5), grid on;
ylabel("I_D"), xlabel("t[s]");
xline(time_of_step,'r--','LineWidth',1.25);
legend("Medido", "Teorico", "Estimado");


%% Parametros del controlador
den = sys.Denominator / sys.Numerator;
L_exp = den(1);
R_exp = den(2);

ContI_Exp.tau = tau_limitante * 10;
ContI_Exp.Kp = L_exp / ContI_Exp.tau;
ContI_Exp.Ki = R_exp / ContI_Exp.tau;

ContI_Ideal
ContI_Exp