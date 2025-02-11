
%% Maxima tension lograble en el lado de continua si la salida del trafo es 28V
Vdc_max = 28 * sqrt(2) / 0.577
Vac_ef_max = Vdc_max * 0.577 / sqrt(2)

%% Ajuste de controladores de corriente
figure(2)
s = tf('s');

Hp = 1 / (L*s + (R + ron)); % Hp = Vgdq / Idq

Cont = Kp + Ki / s;

Loop = Cont * Hp;

ClosedLoop = Loop / (Loop + 1);

ClosedLoopNoControl = Hp / (Hp + 1);

bode(Hp, Loop, ClosedLoop, ClosedLoopNoControl), grid on;
legend("Planta", "Ganancia de Lazo", "Lazo Cerrado", "LC Sin control")

%%ltiview(ClosedLoop, ClosedLoopNoControl)
%%legend("Lazo Cerrado", "LC Sin control")

%% Lazo de Control de Tension de Bus
I0 = 1;
R0 = Vdc / I0;

C = Kp_dc + Ki_dc / s;
G = sqrt(3) / 2 * R0 / (1 - R0 * C_filtro_CC * s);