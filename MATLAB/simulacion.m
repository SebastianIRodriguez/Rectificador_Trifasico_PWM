decimation = 1;
scope_sample_time = 1e-4;
sim_mode = 0;
model_name = "rectificador_pwm.slx";
%% Simulacion a Lazo Cerrado
tic 
sim_mode = 0;
simOut = sim(model_name,"StopTime","1");
steady_state = get(simOut, "xFinal");

toc
%% Simulacion a Lazo Abierto: Analisis de corriente
%% Transferencia Vgd -> Id
sim_mode = 1;
step_time = 0.4;

% 09/03/2025: Parece ser -0.11
vgd_step.start_value = -0.11;
vgd_step.percentage = 15;
vgd_step.end_value = vgd_step.start_value * (1 + vgd_step.percentage/100);
vgd_step.time = step_time;


% 09/03/2025: Parece ser 0
vgq_step.start_value = 0;
vgq_step.percentage = 0;
vgq_step.time = step_time;

tic
sim_mode = 1;
simIn = Simulink.SimulationInput("rectificador_pwm");
simIn = simIn.setModelParameter("StopTime", "1.5");
simIn = setInitialState(simIn, steady_state);
simOut = sim(simIn);
vd_id_step_response = simOut.vd_id_step_response;

toc

%% Simulacion a Lazo Abierto: Analisis tension de bus
tic

step_time = 0.1;
vdc_step.start_value = 10.05;
vdc_step.percentage = 2;
vdc_step.end_value = vdc_step.start_value * (1 + vdc_step.percentage / 100);
vdc_step.time = step_time;

sim_mode = 2;
simIn = Simulink.SimulationInput("rectificador_pwm");
simIn = simIn.setModelParameter("StopTime", "1");
simIn = setInitialState(simIn, steady_state);
simOut = sim(simIn);

toc
id_vdc_step_response = simOut.id_vdc_step_response;









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