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

ContI_Ideal.tau = tau_limitante * 10;
ContI_Ideal.Kp = config.vsc.L / ContI_Ideal.tau;
ContI_Ideal.Ki = config.vsc.R / ContI_Ideal.tau;
config.ContI = ContI_Ideal;

%% Parametros del controlador de tension de bus
% Utilizo Kp y Ki para cancelar el polo propio de la planta
% Luego ubico el polo de la respuesta a lazo cerrado, una decada antes
% del polo de la respuesta a lazo cerrado de la corriente id
ContDC_Ideal.tau = config.ContI.tau * 10;
ContDC_Ideal.Ki = 1 / (config.vsc.R_load * ContDC_Ideal.tau);
ContDC_Ideal.Kp = ContDC_Ideal.Ki * config.vsc.R_load * config.vsc.C;
config.ContDC = ContDC_Ideal;

%% Configuracion por default de escalones
config.step.vgd = crear_configuracion_escalon(0,0,0);
config.step.vgq = crear_configuracion_escalon(0,0,0);
config.step.id  = crear_configuracion_escalon(0,0,0);

%% Configuracion general de la simulacion
config.sample_time = 1e-7; 
% CUIDADO: 
% Si se utilizan valores mas grandes de Ts pueden aparecer artefactos en la tension generada por el convertidor

config.scope_sample_time = 1e-3;
config.mode = 0;
config.model_name = "rectificador_pwm";
config.usar_fuente_lado_DC = false;

%% Parametros del snubber de los IGBTs
config.vsc.snubber.Rs = 1e5;
config.vsc.snubber.Cs = inf;

%% Parametros nominales del convertidor
config.pu.Vnom = 40;
config.pu.Inom = 3;
config.pu.Pnom = config.pu.Vnom * config.pu.Inom / 2;