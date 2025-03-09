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
L = 0.5e-3; %0.5 mH
C = 100e-6; %uF

% Lado de Continua
Vdc = 300;
R_load = 30; %30 -> 3kW , 300 -> 0.3kW
C_filtro_CC = 4 * 470e-6 + 0.47e-6;

%% Parametros del controlador de corriente
tau_conmut = 1 / (2 * pi * f_sw);
tau_pll = 0.01e-3;
% Controlador: Lazo de Corriente - Potencia Activa - Potencia Reactiva
%tau_i = ; %Una decada detras de la f de conmut
tau_i = max(tau_conmut * 10, tau_pll * 10);
Kp = L / tau_i;
Ki = R_tot / tau_i;

%% Parametros del controlador de tension de bus
% Utilizo Kp y Ki para cancelar el polo propio de la planta
% Luego ubico el polo de la respuesta a lazo cerrado, una decada antes
% del polo de la respuesta a lazo cerrado de la corriente id
tau_DC = tau_i * 10;
Ki_DC = 1 / (R_load * tau_DC);
Kp_DC = Ki_DC * R_load * C_filtro_CC;

%% Configuracion de la simulacion
sim_mode = 1;
vgd_step_start_value = 0;
vgd_step_percentage = 0;
vgd_step_time = 0;
vgq_step_start_value = 0;
vgq_step_percentage = 0;
vgq_step_time = 0;


%%
sim_sample_time = 1e-7; %%USAR 1e-7
%%
Vn = 28 * sqrt(3);
Pn = 3000;
Cs_max = Pn / (1000 * 2 * pi * f * Vn^2)
Rs_min = 2 * sim_sample_time / Cs_max

%%
Cs = 1e-6;
Rs = 10;
%%
Rs = 1e5;
Cs = inf;