decimation = 1;
%% Obtener punto de trabajo
sim_mode = 1;
model_name = 'rectificador_pwm.slx';
%%
tic 

simOut = sim(model_name,"StopTime","1");%0.05
%steady_state = get(simOut, "xFinal");

toc
%% Transferencia Vgd -> Id
sim_mode = 0;

step_time = 10;
% Valores: -7.2
vgd_step_start_value = 0;%36.4545
vgd_step_percentage = 40;
vgd_step_time = step_time;
% Valores: -15.6
vgq_step_start_value = 0;%-18.2863
vgq_step_percentage = 0;
vgq_step_time = step_time;
%%
tic

simIn = Simulink.SimulationInput(model_name);
%simIn = simIn.setModelParameter("LoadInitialState","on", "InitialState", steady_state);
%simIn = setInitialState(simIn,steady_state);%0.005
simOut = sim(model_name,"StopTime","1");
ensayo_i = simOut.ensayo_i;

toc
%%
figure(2)
hold on;
input = ensayo_i.Data(:,1);
output = ensayo_i.Data(:,2)-ensayo_i.Data(1,2);
subplot(2,1,1), plot(ensayo_i.Time, input,'LineWidth',1.5), grid on;
ylabel("V_G_D"), xlabel("t[s]");
subplot(2,1,2), plot(ensayo_i.Time, output,'LineWidth',1.5), grid on;
ylabel("I_D"), xlabel("t[s]");
xline(step_time,'r--','LineWidth',1.25)
%%
figure(45)
input = ensayo_i.Data(:,1);
output = ensayo_i.Data(:,2)-ensayo_i.Data(1,2);
output = movmedian(output,10000);
subplot(2,1,1), plot(ensayo_i.Time, input,'LineWidth',1.5), grid on;
ylabel("V_G_D"), xlabel("t[s]");
subplot(2,1,2), plot(ensayo_i.Time, output,'LineWidth',1.5), grid on;
ylabel("I_D"), xlabel("t[s]");
xline(step_time,'r--','LineWidth',1.25)

%%
vgd = ensayo_i.Data(:,1);
avg_vgd = movmean(vgd(end-1000:end),100);
figure(3);
%plot(ensayo_id.Time, ensayo_id.Data(:,1), ensayo_id.Time, avg_vgd), grid on;
%legend("Original", "Average")
plot(ensayo_i.Time(end-1000:end), avg_vgd), grid on;