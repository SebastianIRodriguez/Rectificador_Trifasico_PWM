%% Parametros

% Lado de Alterna
f = 50;
f_sw = 2000;
Ts = 1 / f_sw;
w = 2 * pi * f;
R = 1e-3;
ron = 1e-3;
L = 500e-6;
C = 100e-6;

% Lado de Continua
Vdc = 200;
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
%% Maxima tension lograble en el lado de continua si la salida del trafo es 28V
Vdc_max = 28 * sqrt(2) / 0.577
Vac_ef_max = Vdc_max * 0.577 / sqrt(2)

%% Tension de Red en plano dq
plot(...
    Vdq.Time, Vdq.Data(:,1),...
    Vdq.Time,Vdq.Data(:,2)...
);
grid on;
legend('Vd','Vq');

%% Entrada Trifasica vs Salida Trifasica del Puente
subplot(2,1,1)
plot(...
    v_puente_fase.Time, v_puente_fase.Data(:,1),...
    v_puente_fase.Time,v_puente_fase.Data(:,2),...
    v_puente_fase.Time,v_puente_fase.Data(:,3)...
);
subtitle("Salida del Puente - Tensiones de Fase")

subplot(2,1,2)
plot(...
    v_red_fase.Time, v_red_fase.Data(:,1),...
    v_red_fase.Time,v_red_fase.Data(:,2),...
    v_red_fase.Time,v_red_fase.Data(:,3)...
);
subtitle("Entrada de la red - Tensiones de Fase")

%% Entrada Trifasica vs Salida Trifasica del Puente
% Comparacion fase a fase
subplot(3,1,1)
plot(...
    v_puente_fase.Time, v_puente_fase.Data(:,1),...
    v_red_fase.Time,v_red_fase.Data(:,1)...
);
subtitle("Fase A")

subplot(3,1,2)
plot(...
    v_puente_fase.Time, v_puente_fase.Data(:,2),...
    v_red_fase.Time,v_red_fase.Data(:,2)...
);
subtitle("Fase B")

subplot(3,1,3)
plot(...
    v_puente_fase.Time, v_puente_fase.Data(:,3),...
    v_red_fase.Time,v_red_fase.Data(:,3)...
);
subtitle("Fase C")

%% Ajuste de controladores de corriente
s = tf('s');

ron = 0;

Hp = 1 / (L*s + (R + ron)); % Hp = Vgdq / Idq
bode(Hp), grid on;


%% Vector rotante
sim("rectificador_pwm.slx");
%%
plot( ...
    Vabc_alphabeta(:,1), Vabc_alphabeta(:,2), ...
    Vabc_target_alphabeta(:,1), Vabc_target_alphabeta(:,2),...
    Vabc_puente_alphabeta(:,1), Vabc_puente_alphabeta(:,2)...
    )
legend('Red', 'Objetivo segun control', 'Logrado')