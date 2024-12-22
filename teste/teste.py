import serial
import time

# Configuração da porta serial
ser = serial.Serial('COM3', 9600, timeout=1)  # Substitua 'COM3' pela sua porta correta

def enviar_dados(dados):
    ser.write(dados.encode())
    print(f"Enviado: {dados}")

try:
    while True:
        enviar_dados("Olá")
        time.sleep(2)

except KeyboardInterrupt:
    ser.close()
    print("Comunicação encerrada.")
