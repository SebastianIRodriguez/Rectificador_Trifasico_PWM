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


%% COMPARACION TENSION DE FASE - RED VS CONVERTIDOR
v_puente_fase = simOut.v_puente_fase;
v_red_fase = simOut.v_red_fase;

figure;
plot(...
    v_red_fase.Time, v_red_fase.Data(:,1),...
    v_puente_fase.Time,v_puente_fase.Data(:,1)....
), grid on;
subtitle("Fase A")
ylim([-50 50])
xlim([0.26 0.30])
legend("Red","Convertidor")

%% CORRIENTE
i_fase = simOut.i_fase;

figure;
plot(i_fase.Time, i_fase.Data(:,1)), grid on;
subtitle("Fase A")
%ylim([-50 50])
%xlim([0.16 0.20])
legend("Red","Convertidor")

%% SIN FILTRAR
fs = 1 / (v_puente_fase.Time(2) - v_puente_fase.Time(1));
va = v_puente_fase.Data(:,1);
vb = v_puente_fase.Data(:,2);
vc = v_puente_fase.Data(:,3);

figure;
plot(v_puente_fase.Time,va,'g'), hold on;
plot(v_red_fase.Time, v_red_fase.Data(:,1), 'r', 'LineWidth',1.25), grid on;
xlim([0.16 0.20])
subtitle("Fase A")
legend("Red","Convertidor")


%% COMPARACION TENSION DE FASE - RED VS CONVERTIDOR
v_puente_fase = simOut.v_puente_fase;
v_red_fase = simOut.v_red_fase;

figure;
plot(v_red_fase.Time, v_red_fase.Data(:,1) - v_puente_fase.Data(:,1)), grid on;
subtitle("Fase A")
legend("Red","Convertidor")


%% EXPORTAR FEEDFORWARD TENSION
t1 = find(abs(v_puente_fase.Time - 0.16) < 1e-8);
t2 = find(abs(v_puente_fase.Time - 0.18) < 1e-8);
M = [v_puente_fase.Time(t1:t2) v_red_fase.Data(t1:t2, 1) v_puente_fase.Data(t1:t2, 1)];
M = downsample(M, 100);
T = array2table(M);
T.Properties.VariableNames(1:3) = {'tiempo','entrada','convertidor'};
writetable(T,'Para el informe/feedforward_tension_filtrada.csv')

%% EXPORTAR FEEDFORWARD COMPLETO
t1 = find(abs(v_puente_fase.Time - 0.16) < 1e-8);
t2 = find(abs(v_puente_fase.Time - 0.18) < 1e-8);
M = [v_puente_fase.Time(t1:t2) v_red_fase.Data(t1:t2, 1) v_puente_fase.Data(t1:t2, 1)];
%M = downsample(M, 100);
T = array2table(M);
T.Properties.VariableNames(1:3) = {'tiempo','entrada','convertidor'};
writetable(T,'Para el informe/feedforward_completo_filtrada.csv')

%% EXPORTAR TENSION DE BUS
v_bus = simOut.vdc_medida;
t = v_bus.Time;
M = [ t v_bus.Data ];
M = downsample(M, 100);
T = array2table(M);
T.Properties.VariableNames(1:2) = {'tiempo','vdc_medida'};
writetable(T,'Para el informe/ensayo_setpoint_tension.csv')

%% EXPORTAR ENSAYO CONTROL DE CORRIENTE
ensayo_id = simOut.id;
t = ensayo_id.Time;
M = [ t ensayo_id.Data ];
M = downsample(M, 100);
T = array2table(M);
T.Properties.VariableNames(1:2) = {'tiempo','id_medida'};
writetable(T,'Para el informe/ensayo_setpoint_corriente.csv')
