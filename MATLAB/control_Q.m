%Kp_Q = 1.9318;
%Ki_Q = 1.9318 * 6283;
Cont_Q = Kp_Q + Ki_Q / s;

Lazo_Q = (Cont_Q * ClosedLoop) / (1 + Cont_Q * ClosedLoop);
Lazo_Q_sin_control = ClosedLoop / (1 + ClosedLoop);

figure(1)
bode(Cont_Q, Cont_Q * ClosedLoop, Lazo_Q, Lazo_Q_sin_control), grid on;
legend("Controlador", "Lazo Abierto", "Lazo Cerrado", "Lazo Cerrado - Sin Control")

figure(2)
bode(ClosedLoop, Lazo_Q), grid on;
legend("Lazo Corriente", "Lazo Potencia")

%% Eleccion de parametros
tau_Q = tau_i * 10;
Ki_Q = 1 / tau_Q;
Kp_Q = Ki_Q * tau_i;
