%% Parametros

% Lado de Alterna
f = 50;
f_sw = 10000;
Ts = 1 / f_sw;
w = 2 * pi * f;
R = 1e-3;
ron = 1e-3;
R_tot = R + ron;
%R_tot = 0.2;
L = 1e-3; %0.5 mH
C = 100e-6; %uF

% Lado de Continua
Vdc = 300;
R_load = 30; %30 -> 3kW , 300 -> 0.3kW
C_filtro_CC = 4 * 470e-6 + 0.47e-6;

% Tiempos de respuesta
tau_conmut = 1 / (2 * pi * f_sw);
tau_pll = 10e-3;

%% Parametros del controlador de corriente

ContI_Ideal.tau = max(tau_conmut * 10, tau_pll * 10);
ContI_Ideal.Kp = L / ContI_Ideal.tau;
ContI_Ideal.Ki = R_tot / ContI_Ideal.tau;
ContI = ContI_Ideal;

%% Parametros del controlador de tension de bus
% Utilizo Kp y Ki para cancelar el polo propio de la planta
% Luego ubico el polo de la respuesta a lazo cerrado, una decada antes
% del polo de la respuesta a lazo cerrado de la corriente id
ContDC.tau = ContI.tau * 10;
ContDC.Ki = 1 / (R_load * ContDC.tau);
ContDC.Kp = ContDC.Ki * R_load * C_filtro_CC;

%% Configuracion por default de escalones

vgd_step.start_value = 0;
vgd_step.end_value = 0;
vgd_step.time = 0;

vgq_step.start_value = 0;
vgq_step.percentage = 0;
vgq_step.time = 0;

vdc_step.time = 0;
vdc_step.start_value = 0;
vdc_step.end_value = 0;


%%
sim_sample_time = 1e-7; %%USAR 1e-7
decimation = 1;
scope_sample_time = 1e-4;
sim_mode = 0;
model_name = "rectificador_pwm.slx";

%% Parametros sugeridos por MATLAB para el snubber (NO FUNCIONAN BIEN)
Vn = 28 * sqrt(3);
Pn = 3000;
Cs_max = Pn / (1000 * 2 * pi * f * Vn^2);
Rs_min = 2 * sim_sample_time / Cs_max;

%%
Rs = 1e5;
Cs = inf;