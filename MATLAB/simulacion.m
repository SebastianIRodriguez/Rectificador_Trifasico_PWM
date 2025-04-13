%% SIMULACION A LAZO CERRADO

%Parametros
config.mode = 0;
%sim.ContI = ContI_Exp;

%Configurar simulacion
simIn = Simulink.SimulationInput(config.model_name);
simIn = simIn.setModelParameter("StopTime", "0.5");
%simIn = setInitialState(simIn, steady_state);

%Ejecutar simulacion
tic
simOut = sim(simIn);
toc

%Estado final de todas las variables 
%(usado cuando se quieren obterner los valores de regimen del convertidor)
steady_state = get(simOut, "xFinal");

%% SIMULACION A LAZO ABIERTO: ANALISIS DE CORRIENTE

% Parametros
config.mode = 1;
config.step.vgd = crear_configuracion_escalon(-0.11, 15, 0.4);
config.step.vgq = crear_configuracion_escalon(0,0,0);

% Configurar simulacion
simIn = Simulink.SimulationInput(config.model_name);
simIn = simIn.setModelParameter("StopTime", "1.5");
simIn = setInitialState(simIn, steady_state);

%Ejecutar simulacion
tic
simOut = sim(simIn);
toc

%Resultados del ensayo
vd_id_step_response = simOut.vd_id_step_response;


%% SIMULACION A LAZO ABIERTO: ANALISIS TENSION DE BUS

%Parametros
sim_mode = 2;
config.step.id = crear_configuracion_escalon(10.05, 2, 0.1);

%Configurar simulacion
simIn = Simulink.SimulationInput("rectificador_pwm");
simIn = simIn.setModelParameter("StopTime", "1");
simIn = setInitialState(simIn, steady_state);

%Ejecutar simulacion
tic
simOut = sim(simIn);
toc

%Resultados del ensayo
id_vdc_step_response = simOut.id_vdc_step_response;