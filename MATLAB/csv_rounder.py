import csv
import re

def redondear_valor(valor):
    """
    Redondea un valor numérico a 2 decimales, manteniendo notación científica si aplica.
    """
    valor = valor.strip()
    
    # Si está vacío, devolver vacío
    if not valor:
        return valor
    
    try:
        # Intentar convertir a float
        num = float(valor)
        
        # Verificar si el valor original tenía notación científica
        if 'e' in valor.lower():
            # Redondear y formatear en notación científica con 2 decimales
            return f"{num:.3e}"
        else:
            # Redondear a 2 decimales en formato normal
            return f"{num:.3f}"
    except ValueError:
        # Si no es un número, devolver el valor original
        return valor

def procesar_csv(archivo_entrada, archivo_salida):
    """
    Lee un CSV, redondea los valores numéricos y guarda el resultado.
    """
    with open(archivo_entrada, 'r', encoding='utf-8') as f_in:
        lector = csv.reader(f_in)
        
        # Leer todas las filas
        filas = list(lector)
    
    # Procesar cada fila
    filas_procesadas = []
    for fila in filas:
        fila_procesada = [redondear_valor(valor) for valor in fila]
        filas_procesadas.append(fila_procesada)
    
    # Escribir el archivo de salida
    with open(archivo_salida, 'w', encoding='utf-8', newline='') as f_out:
        escritor = csv.writer(f_out)
        escritor.writerows(filas_procesadas)
    
    print(f"Archivo procesado exitosamente: {archivo_salida}")

# Uso del script
if __name__ == "__main__":
    import os
    
    print(f"Directorio actual: {os.getcwd()}")
    print("\nArchivos en este directorio:")
    for archivo in os.listdir('.'):
        if archivo.endswith('.csv'):
            print(f"  - {archivo}")
    
    # Solicitar el archivo de entrada
    archivo_entrada = input("\nIngresá la ruta del archivo CSV a procesar: ").strip()
    
    # Verificar que existe
    if not os.path.exists(archivo_entrada):
        print(f"ERROR: No se encuentra el archivo '{archivo_entrada}'")
        exit(1)
    
    # Generar nombre de salida
    nombre_base = os.path.splitext(archivo_entrada)[0]
    archivo_salida = f"{nombre_base}_redondeado.csv"
    
    procesar_csv(archivo_entrada, archivo_salida)