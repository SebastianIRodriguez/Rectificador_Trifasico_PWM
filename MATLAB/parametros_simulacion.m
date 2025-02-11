%% Parametros

% Lado de Alterna
f = 50;
f_sw = 10000;
Ts = 1 / f_sw;
w = 2 * pi * f;
R = 1e-3;
ron = 1e-3;
L = 500e-6;
C = 100e-6;

% Lado de Continua
Vdc = 30;
C_filtro_CC = 4 * 470e-6 + 0.47e-6;

% Controlador: Lazo de Corriente - Potencia Activa - Potencia Reactiva
tau_i = 1 / (0.1 * (2 * pi * f_sw));
%Kp = 0.6325;
Kp = L / tau_i;
%Ki = 0.261;
Ki = (R+ron) / tau_i;

% Controlador: Tension de Bus
phi = deg2rad(15); % Propongo margen de fase de 15°
a = (1 + cos(phi)) / sin(phi);

Kp_dc = C_filtro_CC / (2 * sqrt(3) * a * Ts);
Ki_dc = Kp / (a^2*3*Ts);