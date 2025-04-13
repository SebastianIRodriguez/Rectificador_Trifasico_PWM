%% Tension de Red en plano dq
plot(...
    Vdq.Time, Vdq.Data(:,1),...
    Vdq.Time,Vdq.Data(:,2)...
);
grid on;
legend('Vd','Vq');

%% Entrada Trifasica vs Salida Trifasica del Puente
figure
subplot(2,1,1)
plot(...
    v_puente_fase.Time, v_puente_fase.Data(:,1),'r*',...
    v_puente_fase.Time,v_puente_fase.Data(:,2),'g*',...
    v_puente_fase.Time,v_puente_fase.Data(:,3),'b*'...
), hold on;
stairs(v_puente_fase.Time, v_puente_fase.Data(:,1),'r');
stairs(v_puente_fase.Time, v_puente_fase.Data(:,2),'g');
stairs(v_puente_fase.Time, v_puente_fase.Data(:,3),'b');
subtitle("Salida del Puente - Tensiones de Fase")

subplot(2,1,2)
plot(...
    v_red_fase.Time, v_red_fase.Data(:,1),...
    v_red_fase.Time,v_red_fase.Data(:,2),...
    v_red_fase.Time,v_red_fase.Data(:,3)...
);
subtitle("Entrada de la red - Tensiones de Fase")
legend("A","B","C")

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


%% Vector rotante
plot( ...
    Vabc_alphabeta(:,1), Vabc_alphabeta(:,2), ...
    Vabc_target_alphabeta(:,1), Vabc_target_alphabeta(:,2),...
    Vabc_puente_alphabeta(:,1), Vabc_puente_alphabeta(:,2)...
    )
legend('Red', 'Objetivo segun control', 'Logrado')
%%
figure
plot( ...
    v_puente_fase.Time,Vabc_target_alphabeta(:,1), ...
    v_puente_fase.Time,Vabc_target_alphabeta(:,2)...
    )
legend("alpha","beta")


%%
figure;
plot(...
    i_fase.time, i_fase.signals.values(:,1),...
    i_fase.time,i_fase.signals.values(:,2),...
    i_fase.time,i_fase.signals.values(:,3)...
);
subtitle("Corrientes de Fase")
legend("i_A","i_B","i_C")


%%
figure;
subplot(3,1,1)
plot(...
    v_red_fase.Time, v_red_fase.Data(:,1),...
    Vabc_target.time,Vabc_target.signals.values(:,1)....
);
subtitle("Fase A")
legend("Red","Convertidor")

subplot(3,1,2)
plot(...
    v_red_fase.Time, v_red_fase.Data(:,2),...
    Vabc_target.time,Vabc_target.signals.values(:,2)....
);
subtitle("Fase B")
legend("Red","Convertidor")

subplot(3,1,3)
plot(...
    v_red_fase.Time, v_red_fase.Data(:,3),...
    Vabc_target.time,Vabc_target.signals.values(:,3)....
);
subtitle("Fase C")
legend("Red","Convertidor")


%% COMPARACION TENSION DE FASE - RED VS CONVERTIDOR
v_puente_fase = simOut.v_puente_fase;
v_red_fase = simOut.v_red_fase;
fs = 1 / (v_puente_fase.Time(2) - v_puente_fase.Time(1));
va = lowpass(v_puente_fase.Data(:,1), 1000, fs,ImpulseResponse="iir",Steepness=0.95);

figure;
plot(...
    v_red_fase.Time, v_red_fase.Data(:,1),...
    v_puente_fase.Time,va....
), grid on;
subtitle("Fase A")
legend("Red","Convertidor")

%% SIN FILTRAR
fs = 1 / (v_puente_fase.Time(2) - v_puente_fase.Time(1));
va = v_puente_fase.Data(:,1);
vb = v_puente_fase.Data(:,2);
vc = v_puente_fase.Data(:,3);

figure;
subplot(3,1,1)
plot(...
    v_red_fase.Time, v_red_fase.Data(:,1),...
    v_puente_fase.Time,va....
);
subtitle("Fase A")
legend("Red","Convertidor")

subplot(3,1,2)
plot(...
    v_red_fase.Time, v_red_fase.Data(:,2),...
    v_puente_fase.Time,vb....
);
subtitle("Fase B")
legend("Red","Convertidor")

subplot(3,1,3)
plot(...
    v_red_fase.Time, v_red_fase.Data(:,3),...
    v_puente_fase.Time,vc....
);
subtitle("Fase C")
legend("Red","Convertidor")


%%
fs = 1 / (v_puente_fase.Time(2) - v_puente_fase.Time(1));
va = lowpass(v_puente_fase.Data(:,1), 1000, fs,ImpulseResponse="iir",Steepness=0.95);
vb = lowpass(v_puente_fase.Data(:,2), 1000, fs,ImpulseResponse="iir",Steepness=0.95);
vc = lowpass(v_puente_fase.Data(:,3), 1000, fs,ImpulseResponse="iir",Steepness=0.95);

figure;
subplot(3,1,1)
plot(...
    v_puente_fase.Time,va....
);
xlim([0.5 1])
subtitle("Fase A")
legend("Red","Convertidor")

subplot(3,1,2)
plot(...
    v_puente_fase.Time,vb....
);
xlim([0.5 1])
subtitle("Fase B")
legend("Red","Convertidor")

subplot(3,1,3)
plot(...
    v_puente_fase.Time,vc....
);
xlim([0.5 1])
subtitle("Fase C")
legend("Red","Convertidor")
%%

figure;
subplot(3,1,1)
plot(...
    v_red_fase.Time, v_red_fase.Data(:,1),...
    v_puente_fase.Time,va....
);
subtitle("Fase A")
legend("Red","Convertidor")

subplot(3,1,2)
plot(...
    v_red_fase.Time, v_red_fase.Data(:,2),...
    v_puente_fase.Time,vb....
);
subtitle("Fase B")
legend("Red","Convertidor")

subplot(3,1,3)
plot(...
    v_red_fase.Time, v_red_fase.Data(:,3),...
    v_puente_fase.Time,vc....
);
subtitle("Fase C")
legend("Red","Convertidor")


%% SIN FILTRAR
fs = 1 / (v_puente_fase.Time(2) - v_puente_fase.Time(1));
va = lowpass(v_puente_fase.Data(:,1), 1000, fs,ImpulseResponse="iir",Steepness=0.95);
vb = lowpass(v_puente_fase.Data(:,2), 1000, fs,ImpulseResponse="iir",Steepness=0.95);
vc = lowpass(v_puente_fase.Data(:,3), 1000, fs,ImpulseResponse="iir",Steepness=0.95);

figure;
subplot(3,1,1)
plot(...
    v_red_linea.Time, v_red_linea.Data(:,1),...
    v_puente_fase.Time,va....
);
subtitle("Fase A")
legend("Red","Convertidor")

subplot(3,1,2)
plot(...
    v_red_linea.Time, v_red_linea.Data(:,2),...
    v_puente_fase.Time,vb....
);
subtitle("Fase B")
legend("Red","Convertidor")

subplot(3,1,3)
plot(...
    v_red_linea.Time, v_red_linea.Data(:,3),...
    v_puente_fase.Time,vc....
);
subtitle("Fase C")
legend("Red","Convertidor")

%% TENSIONES V_AN y V_AB con FILTRO PASABAJOS 1kHz
fs = sim_sample_time;
va_fase_filtr = lowpass(v_puente_fase.Data(:,1), 1000, fs,ImpulseResponse="iir",Steepness=0.85);
va_linea_filtr = lowpass(v_puente_linea.Data(:,1), 1000, fs,ImpulseResponse="iir",Steepness=0.85);
va_fase = v_puente_fase.Data(:,1);
va_linea = v_puente_linea.Data(:,1);


figure;
subplot(2,1,1)
plot(...
    v_puente_fase.Time,va_fase,....
    v_puente_linea.Time,va_linea....
);
%ylim([-100 100])
grid on;
subtitle("Tensiones - SIN FILTRAR")
legend("V_A_N", "V_A_B")

subplot(2,1,2)
plot(...
    v_puente_fase.Time,va_fase_filtr,....
    v_puente_linea.Time,va_linea_filtr....
);
%ylim([-100 100])
grid on;
subtitle("Tensiones - FILTRO 1kHz")
legend("V_A_N", "V_A_B")


%% TENSIONES V_AN y V_AB con FILTRO PASABAJOS 1kHz
fs = 1/sim_sample_time;
va_fase_filtr = lowpass(va_puente_fase.Data(:,1), 1000, fs,ImpulseResponse="iir",Steepness=0.85);
va_linea_filtr = lowpass(v_puente_linea.Data(:,1), 1000, fs,ImpulseResponse="iir",Steepness=0.85);
va_fase = va_puente_fase.Data(:,1);
va_linea = v_puente_linea.Data(:,1);


figure;
subplot(2,1,1)
plot(...
    v_puente_fase.Time,va_fase,....
    v_puente_linea.Time,va_linea....
);
%ylim([-100 100])
grid on;
subtitle("Tensiones - SIN FILTRAR")
legend("V_A_N", "V_A_B")

subplot(2,1,2)
plot(...
    v_puente_fase.Time,va_fase_filtr,....
    v_puente_linea.Time,va_linea_filtr....
);
%ylim([-100 100])
grid on;
subtitle("Tensiones - FILTRO 1kHz")
legend("V_A_N", "V_A_B")