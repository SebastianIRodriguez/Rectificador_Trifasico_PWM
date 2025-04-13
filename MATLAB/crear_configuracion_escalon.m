function escalon = crear_configuracion_escalon(v_inicial, porcentaje_escalon, t_escalon)
escalon.start_value = v_inicial;
escalon.end_value  = v_inicial * (1 + porcentaje_escalon / 100);
escalon.time = t_escalon;
end