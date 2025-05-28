%% Parametros
config.vsc = crear_configuracion_convertidor( ...
    50, ...             %f_red
    10000, ...          %f_sw
    137e-3, ...         %R_ac
    1e-3, ...           %L_ac
    31e-3, ...          %R_igbt
    4 * 470e-6 + 0.47e-6, ...   %C_dc
    500, ...            %R_load
    300 ...             %V_dc
);

tau_pll = 10e-3;
tau_limitante = max(config.vsc.tau_switching, tau_pll);

%% Parametros del controlador de corriente
% Utilizo Kp y Ki para cancelar el polo propio de la plant
% Luego ubico el polo de la respuesta a lazo cerrado, una decada antes
% del polo del PLL
ContI_Ideal.tau = tau_limitante * 10;
ContI_Ideal.Kp = config.vsc.L / ContI_Ideal.tau;
ContI_Ideal.Ki = config.vsc.R / ContI_Ideal.tau;
config.ContI = ContI_Ideal;

%% Parametros del controlador de tension de bus
% Utilizo Kp y Ki para cancelar el polo propio de la planta
% Luego ubico el polo de la respuesta a lazo cerrado, una decada antes
% del polo de la respuesta a lazo cerrado de la corriente id
ContDC_Ideal.tau = config.ContI.tau * 10;
ContDC_Ideal.Ki = ContDC_Ideal.tau / config.vsc.R_load;
ContDC_Ideal.Kp = config.vsc.C * ContDC_Ideal.tau;
config.ContDC = ContDC_Ideal;

%% Configuracion por default de escalones
config.step.vgd = crear_configuracion_escalon(0,0,0);
config.step.vgq = crear_configuracion_escalon(0,0,0);
config.step.id  = crear_configuracion_escalon(0,0,0);

%% Configuracion por default para ensayos del controlador
config.cambio_tension_referencia = false;
config.step_setpoint_tension = 0;
config.step_setpoint_corriente = 0;

%% Configuracion general de la simulacion
config.sample_time = 1e-5; 
config.scope_sample_time = 1e-4;
config.mode = 0;
config.model_name = "rectificador_pwm";
config.usar_fuente_lado_DC = false;

%% Parametros del snubber de los IGBTs
config.vsc.snubber.Rs = 1e5;
config.vsc.snubber.Cs = inf;

%% Parametros nominales del convertidor
config.pu.Vnom = 40;
config.pu.Inom = 3;
config.pu.Pnom = 3 / 2 * config.pu.Vnom * config.pu.Inom;

%% MODOS DEL SISTEMA DE CONTROL
ModoControl.SOLO_FEEDFORWARD = 0;
ModoControl.FF_Y_PI_CORRIENTE = 1;
ModoControl.FF_Y_PI_CORRIENTE_Y_COMP_VDC = 2;
ModoControl.SISTEMA_COMPLETO = 3;