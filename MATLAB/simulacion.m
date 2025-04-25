%% FEEDFORWARD - POTENCIA NULA

%Parametros
config.mode = 1;
config.usar_fuente_lado_DC = 1;
config.step.vgd = crear_configuracion_escalon(0, 0, 0);
config.step.vgq = crear_configuracion_escalon(0,0,0);

%Configurar simulacion
simIn = Simulink.SimulationInput(config.model_name);
simIn = simIn.setModelParameter("StopTime", "0.25", "LoadInitialState","off");

%Ejecutar simulacion
tic
sim(simIn);
toc


%% SIMULACION A LAZO CERRADO - CONTROLADORES IDEALES

load("Para el informe\steady_state_baja_potencia.mat");

%Parametros
config.mode = 0;
config.usar_fuente_lado_DC = false;
config.ContI = ContI_Ideal;
contfig.ContDC = ContDC_Ideal;

%Configurar simulacion
simIn = Simulink.SimulationInput(config.model_name);
simIn = simIn.setModelParameter("StopTime", "0.5");
simIn = setInitialState(simIn, steady_state);

%Ejecutar simulacion
tic
simOut = sim(simIn);
toc

%Estado final de todas las variables 
%(usado cuando se quieren obtener los valores de regimen del convertidor)
steady_state = get(simOut, "xFinal");

%% SIMULACION A LAZO ABIERTO: ANALISIS DE CORRIENTE

% Para 3000 W
% vgd = -0.11, vgq = 0
% Para  180 W
% vgd = -0.55, vgq = 0

% Parametros
config.mode = 1;
config.usar_fuente_lado_DC = 1;
config.step.vgd = crear_configuracion_escalon(-0.55, 15, 0.2);
config.step.vgq = crear_configuracion_escalon(0,0,0);

% Configurar simulacion
simIn = Simulink.SimulationInput(config.model_name);
simIn = simIn.setModelParameter("StopTime", "0.4");
simIn = setInitialState(simIn, steady_state);

%Ejecutar simulacion
tic
simOut = sim(simIn);
toc

%Resultados del ensayo
vd_id_step_response = simOut.vd_id_step_response;


%% SIMULACION A LAZO ABIERTO: ANALISIS TENSION DE BUS

config.ContI = ContI_Exp;

% Para 3000 W
% ud = 10.05
% Para  180 W
% ud = 0.617

%Parametros
config.mode = 2;
config.usar_fuente_lado_DC = 0;
config.step.id = crear_configuracion_escalon(0.617, 10, 0.25);

%Configurar simulacion
simIn = Simulink.SimulationInput("rectificador_pwm");
simIn = simIn.setModelParameter("StopTime", "5");
simIn = setInitialState(simIn, steady_state);

%Ejecutar simulacion
tic
simOut = sim(simIn);
toc

%Resultados del ensayo
id_vdc_step_response = simOut.id_vdc_step_response;

%% SIMULACION A LAZO CERRADO - CONTROLADORES EXPERIMENTALES

load("Para el informe\steady_state_baja_potencia.mat");

%Parametros
config.mode = 0;
config.usar_fuente_lado_DC = 0;
config.perturbacion_tension_habilitada = true;
config.perturbacion_tension_ti = 0.1; % segundos
config.perturbacion_tension_amp = -5; % por ciento 
config.ContI = ContI_Exp;
config.ContDC = ContDC_Exp;

%Configurar simulacion
simIn = Simulink.SimulationInput(config.model_name);
simIn = simIn.setModelParameter("StopTime", "1");
simIn = setInitialState(simIn, steady_state);

config.perturbacion_tension_ampl_habilitada = false;

%Ejecutar simulacion
tic
simOut = sim(simIn);
toc