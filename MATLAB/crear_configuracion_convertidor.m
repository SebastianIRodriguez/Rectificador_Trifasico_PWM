function [vsc] = crear_configuracion_convertidor( ...
    f_red, ...
    f_sw, ...
    R_ac, ...
    L_grid, ...
    L_inv, ...
    Cf, ...
    Rd, ...
    R_igbt, ...
    C_dc, ...
    R_load, ...
    V_dc, ...
    V_grid ...
)
% Crea una estructura con todos los parametros relevantes del convertidor
% necesarios para la simulacion
%   f_red:  Frecuencia fundamental de la red a la que se conecta el
%   convertidor, en general 50Hz.
%   f_sw:       Frecuencia de conmutacion del convertidor
%   R_ac:       Resistencia de cada inductor
%   R_igbt:     Resistencia de conduccion de cada IGBT
%   L_grid:     Inductancia de desacople
%   L_inv:      Inductancia del Filtro LC
%   Rd:         Resistencia de amortiguamiento del Filtro LC
%   Cf:         Capacitor del Filtro LC
%   C_dc:       Capacidad del bus de continua
%   R_load:     Carga resistiva a alimentar
%   Vdc:        Tension a generar en el bus de continua
vsc.f = f_red;
vsc.f_sw = f_sw;
vsc.w = 2 * pi * f_red;
vsc.R_ac = R_ac;
vsc.R_igbt = R_igbt;
vsc.Lgrid = L_grid;
vsc.Linv = L_inv;
vsc.Rd = Rd;
vsc.Cf = Cf;
vsc.C = C_dc;
vsc.Vdc = V_dc;
vsc.Rgrid = R_ac + R_igbt;
vsc.R_load = R_load;
vsc.tau_switching = 1 / (2 * pi * f_sw);
vsc.Vgrid = V_grid;
end