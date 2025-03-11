%%
vgd = vd_id_step_response.signals(1).values;
id = vd_id_step_response.signals(2).values;
t = vd_id_step_response.time;

figure(2)
hold on;
input = -vgd;
output = id;
output = output - output(1);
output = -output;
subplot(2,1,1), plot(t, input,'LineWidth',1.5), grid on;
ylabel("V_G_D"), xlabel("t[s]");
subplot(2,1,2), plot(t, output,'LineWidth',1.5), grid on;
ylabel("I_D"), xlabel("t[s]");
xline(step_time,'r--','LineWidth',1.25)

%%
% Define los datos
data = iddata(id, vgd, t(2) - t(1)); % Crea un objeto iddata con el paso de tiempo

% Estima un modelo de transferencia de primer orden
sys = tfest(data, 1, 0); % Numerador de grado 1, denominador de grado 1

s = tf('s');
Hp = 1 / (L*s + (R + ron)); % Hp = Vgdq / Idq

% Muestra el modelo
ltiview(sys, Hp)











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