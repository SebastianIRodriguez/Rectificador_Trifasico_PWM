s = tf('s');
rendimiento = 1;
V_bus = 12;
V_gd = 39.6;
Z_load = 1;
Z_cap = 1 / (s * C_filtro_CC);
Z_dc = Z_cap * Z_load / (Z_cap + Z_load);
G = 3 / 2 * rendimiento * V_gd / V_bus * Z_dc;

G_lc = G / (1 + G);

C = 1 + 1/s;

H = C * G;
H_lc = H / (1 + H);

bode(G, G_lc, H, H_lc), grid on;
legend("G", "G LC", "H", "H LC")