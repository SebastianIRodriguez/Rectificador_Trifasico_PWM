Lg = 2.694791315001111E-4;
Li = 3.655669577556767E-4;
Cf = 1.855238094029786E-5;
Rd = 0.9638785874259721;

s = tf('s');

G = 1 / (1 + (s^2 * Cf * Lg) / (s * Cf * Rd + 1));
G1 = 1 / (s*Lg*( 1 + Li * (Lg + Rd + 1 / (s*Cf)) /  (Rd + 1 / (s*Cf) / Lg)) );

num = Rd * Cf * s + 1;
den = s^3*Li*Lg*Cf + s^2*(Li*Rd*Cf+Lg*Rd*Cf)+s*(Lg+Li);
G2 = num / den;

Lg = 0.0024;
num = Rd * Cf * s + 1;
den = s^3*Li*Lg*Cf + s^2*(Li*Rd*Cf+Lg*Rd*Cf)+s*(Lg+Li);
G3 = num / den;

bode(G2, G3);

%% Diseño de referencia
Lg = 10.3e-6;
Li = 360e-6;
Cf = 4.7e-6;
Rd = 0.58;

s = tf('s');

num = Rd * Cf * s + 1;
den = s^3*Li*Lg*Cf + s^2*(Li*Rd*Cf+Lg*Rd*Cf)+s*(Lg+Li);
G = num / den;

bode(G);


%%
fname = 'data_filtro_emi.json'; 
fid = fopen(fname); 
raw = fread(fid,inf); 
str = char(raw'); 
fclose(fid); 
val = jsondecode(str);

Lg = str2double(val.Lgrid) * 1e-6;
Li = str2double(val.Linv) * 1e-6;
Cf = str2double(val.Cf) * 1e-6;
Rd = str2double(val.Rd);

s = tf('s');

num = Rd * Cf * s + 1;
den = s^3*Li*Lg*Cf + s^2*(Li*Rd*Cf+Lg*Rd*Cf)+s*(Lg+Li);
G = num / den;

bode(G);

%%
w = logspace(0, 5, 100) * 2 * pi;
[mag, w_out] = freqresp(G, w);

frecs = w_out / 2 / pi;
mags = 20 * log10(abs(squeeze(mag(1,1,:))));
T = table(frecs, mags, 'VariableNames', {'f', 'mag'});
writetable(T, 'bode_table.csv');
