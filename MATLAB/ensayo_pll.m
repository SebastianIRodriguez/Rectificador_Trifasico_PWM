clear, close all;

start_frec = 50;
step_time = 0.5;
step_size = 5; %Escalon del 5%

out = sim("ensayos_pll.slx");

t = out.tout;
y = out.salida;
for i=1:length(y)-1
    while(y(i) - y(i+1) > 1)
        y(i+1) = y(i+1) + 2*pi;
    end
end
y = y - 2*pi*50*t;
%plot(a - 2*pi*50*t)
%%
cant_pre_exct = 0;
for i=1:length(t)
    if(t(i) > step_time)
        break;
    end
    cant_pre_exct = cant_pre_exct + 1;
end
%%
z = [zeros(1,cant_pre_exct) ones(1,length(out.tout) - cant_pre_exct)] * step_size;

trim_start = 1;
%trim
y = y(trim_start:end);
z = z(trim_start:end);
t = t(trim_start:end);

[tita tao K] = cohen_coon(t,y,z,step_time)

%%
