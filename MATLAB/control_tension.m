%feedback()
%minreal()

s = tf('s');

%Transferencia del lado de continua v_DC / i_DC
Z_load = R_load;
Z_cap = 1 / (s * C_filtro_CC);
Z_dc = Z_cap * Z_load / (Z_cap + Z_load);

%Control PI
C = Kp_DC + Ki_DC/s;

%Transferencia a lazo cerrado
H = C * Z_dc;
H_LC = H / (1 + H);

figure(25);
%bode(H, 1 / (tau_DC*s + 1)), grid on;
%bode(H_LC, 1 / (tau_DC * s + 1));
%legend("H", "Lo que espero")