%% FEEDFORWARD - POTENCIA NULA

%Parametros
config.mode = ModoControl.SOLO_FEEDFORWARD;
config.usar_fuente_lado_DC = true;
config.step.vgd = crear_configuracion_escalon(0, 0, 0);
config.step.vgq = crear_configuracion_escalon(0,0,0);

%Configurar simulacion
simIn = Simulink.SimulationInput(config.model_name);
simIn = simIn.setModelParameter("StopTime", "0.5", "LoadInitialState","off");

%Ejecutar simulacion
tic
simOut = sim(simIn);
toc


%% SIMULACION A LAZO CERRADO - CONTROLADORES IDEALES

%load(config.steady_state_file);

%Parametros
config.mode = ModoControl.SISTEMA_COMPLETO;
config.usar_fuente_lado_DC = false;
config.ContI = ContI_Ideal;
contfig.ContDC = ContDC_Ideal;

%Configurar simulacion
simIn = Simulink.SimulationInput(config.model_name);
%simIn = simIn.setModelParameter("StopTime", "1");
%simIn = setInitialState(simIn, steady_state);

simIn = simIn.setModelParameter("StopTime", "10", "LoadInitialState","off");

%Ejecutar simulacion
tic
simOut = sim(simIn);
toc

%Estado final de todas las variables 
%(usado cuando se quieren obtener los valores de regimen del convertidor)
steady_state = get(simOut, "xFinal");

%% SIMULACION A LAZO ABIERTO: ANALISIS DE CORRIENTE

load(config.steady_state_file);

% Parametros
config.mode = ModoControl.SOLO_FEEDFORWARD;
config.usar_fuente_lado_DC = true;
config.step.vgd = crear_configuracion_escalon(-3.7716, 15, 0.2);
config.step.vgq = crear_configuracion_escalon(-0.6738, 0, 0);

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
save("datos\id_vdc_step_response.mat", "vd_id_step_response")


%% SIMULACION A LAZO ABIERTO: ANALISIS TENSION DE BUS

load(config.steady_state_file);

config.ContI = ContI_Exp;

%Parametros
config.mode = ModoControl.FF_Y_PI_CORRIENTE_Y_COMP_VDC;
config.usar_fuente_lado_DC = false;
config.step.id = crear_configuracion_escalon(15.08, 1, 0.25);

%Configurar simulacion
simIn = Simulink.SimulationInput("rectificador_pwm");
simIn = simIn.setModelParameter("StopTime", "10");
simIn = setInitialState(simIn, steady_state);

%Ejecutar simulacion
tic
simOut = sim(simIn);
toc

%Resultados del ensayo
id_vdc_step_response = simOut.id_vdc_step_response;
save("datos\id_vdc_step_response.mat", "id_vdc_step_response")

%% ENSAYO A LAZO CERRADO - CAMBIO EN SETPOINT DE CORRIENTE

load(config.steady_state_file);

%Parametros
config.mode = ModoControl.FF_Y_PI_CORRIENTE;
config.usar_fuente_lado_DC = true;
config.step_setpoint_corriente = 1; % por ciento

config.ContI = ContI_Exp;

%Configurar simulacion
simIn = Simulink.SimulationInput(config.model_name);
simIn = simIn.setModelParameter("StopTime", "2");
simIn = setInitialState(simIn, steady_state);

%Ejecutar simulacion
tic
simOut = sim(simIn);
toc

%% ENSAYO A LAZO CERRADO - CAMBIO EN SETPOINT DE TENSION

load(config.steady_state_file);

%Parametros
config.mode = ModoControl.SISTEMA_COMPLETO;
config.usar_fuente_lado_DC = false;

config.cambio_tension_referencia = true;
config.step_setpoint_tension = 1; % en porcentaje

config.ContI = ContI_Exp;
config.ContDC = ContDC_Exp;

%Configurar simulacion
simIn = Simulink.SimulationInput(config.model_name);
simIn = simIn.setModelParameter("StopTime", "10");
simIn = setInitialState(simIn, steady_state);

%Ejecutar simulacion
tic
simOut = sim(simIn);
toc

config.step_setpoint_tension = false;

%%