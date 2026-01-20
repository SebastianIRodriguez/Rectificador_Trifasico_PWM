%% COMPARACION TENSION DE FASE - RED VS CONVERTIDOR
v_puente_fase = simOut.v_puente_fase;
v_red_fase = simOut.v_red_cap;

figure;
plot(...
    v_red_fase.Time, v_red_fase.Data(:,1),...
    v_puente_fase.Time,v_puente_fase.Data(:,1)....
), grid on;
subtitle("Fase A")
%ylim([-50 50])
%xlim([0.26 0.30])
xlim([0 0.05])
ylim([-300 300])
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
figure;
plot(v_puente_fase.Time,v_puente_fase.Data(:,1),'g'), hold on;
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

%% EXPORTAR TENSION DE BUS CONMUTADO
v_bus = simOut.vdc_medida.Data;
t = simOut.vdc_medida.Time;
v_bus = downsample(v_bus, 10);
t = downsample(t, 10);
[x,y] = reducem(t,v_bus,1e-3);
%M = [ t v_bus.Data ;
%M = downsample(M, 10);
M = [round(x,3) round(y,3)];
plot(M(:,1), M(:,2));
T = array2table(M);
T.Properties.VariableNames(1:2) = {'tiempo_conmut','vdc_medida_conmut'};
writetable(T,'Para el informe/ensayo_setpoint_tension_conmutado1.csv')

%% EXPORTAR TENSION DE BUS
v_bus = simOut.vdc_medida.Data;
t = simOut.vdc_medida.Time;
v_bus = downsample(v_bus, 10);
t = downsample(t, 10);
[x,y] = reducem(t,v_bus,1e-3);
%M = [ t v_bus.Data ;
%M = downsample(M, 10);
M = [round(x,3) round(y,3)];
plot(M(:,1), M(:,2));
T = array2table(M);
T.Properties.VariableNames(1:2) = {'tiempo','vdc_medida'};
writetable(T,'Para el informe/ensayo_setpoint_tension1.csv')

%% EXPORTAR ENSAYO CONTROL DE CORRIENTE MODELO CONMUTADO
ensayo_id = simOut.id;
t = ensayo_id.Time;
idx = find(abs(t - 1) < 1e-14);
M = [ t(idx:end)-1 ensayo_id.Data(idx:end) ];
M = downsample(M, 200);
M = round(M, 4);
figure
plot(M(:,1),M(:,2))
T = array2table(M);
T.Properties.VariableNames(1:2) = {'tiempo_conmut','id_medida_conmut'};
writetable(T,'Para el informe/ensayo_setpoint_corriente_conmutado1.csv')

%% EXPORTAR ENSAYO CONTROL DE CORRIENTE
ensayo_id = simOut.id;
t = ensayo_id.Time;
idx = find(abs(t - 1) < 1e-14);
M = [ t(idx:end)-1 ensayo_id.Data(idx:end) ];
M = downsample(M, 200);
M = round(M, 4);
figure
plot(M(:,1),M(:,2))
T = array2table(M);
T.Properties.VariableNames(1:2) = {'tiempo','id_medida'};
writetable(T,'Para el informe/ensayo_setpoint_corriente1.csv')