%% SIMULACION A LAZO CERRADO
tic

sim_mode = 0;
ContI = ContI_Exp;
simIn = Simulink.SimulationInput("rectificador_pwm");
simIn = simIn.setModelParameter("StopTime", "0.5");
simIn = setInitialState(simIn, steady_state);
simOut = sim(simIn);
steady_state = get(simOut, "xFinal");

toc

%% SIMULACION A LAZO ABIERTO: ANALISIS DE CORRIENTE
%% Parametros
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
%% Simulacion
tic

sim_mode = 1;
simIn = Simulink.SimulationInput("rectificador_pwm");
simIn = simIn.setModelParameter("StopTime", "1.5");
simIn = setInitialState(simIn, steady_state);
simOut = sim(simIn);
vd_id_step_response = simOut.vd_id_step_response;

toc


%% SIMULACION A LAZO ABIERTO: ANALISIS TENSION DE BUS
%% Parametros
step_time = 0.1;
vdc_step.start_value = 10.05;
vdc_step.percentage = 2;
vdc_step.end_value = vdc_step.start_value * (1 + vdc_step.percentage / 100);
vdc_step.time = step_time;

%% Simulacion
tic

sim_mode = 2;
simIn = Simulink.SimulationInput("rectificador_pwm");
simIn = simIn.setModelParameter("StopTime", "1");
simIn = setInitialState(simIn, steady_state);
simOut = sim(simIn);

toc
id_vdc_step_response = simOut.id_vdc_step_response;