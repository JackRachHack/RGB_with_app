import serial
import tkinter as tk

# Configuração da porta serial
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)

# Função para enviar dados ao microcontrolador
def enviar_dados(identifier, value):
    ser.write(b'i')
    ser.write(identifier.to_bytes(1, 'big'))
    ser.write(value.to_bytes(1, 'big'))
    print(f"Enviado: ID={identifier}, Valor={value}")

# Função para atualizar os valores quando um slider é alterado
def slider_alterado(cor):
    if cor == "red":
        valueR.set(sliderR.get())
        enviar_dados(0, valueR.get())
    elif cor == "green":
        valueG.set(sliderG.get())
        enviar_dados(1, valueG.get())
    elif cor == "blue":
        valueB.set(sliderB.get())
        enviar_dados(2, valueB.get())

    update_color()

# Função para atualizar a cor do fundo do canvas
def update_color():
    r = valueR.get()
    g = valueG.get()
    b = valueB.get()
    color = f"#{r:02x}{g:02x}{b:02x}"
    canvas.config(bg=color)
    red_label.config(text=f"Red: {r}")
    green_label.config(text=f"Green: {g}")
    blue_label.config(text=f"Blue: {b}")

# Função para receber dados do microcontrolador
def receber_dados():
    data = [0, 0]
    index = 0
    receiving = False

    while ser.in_waiting > 0:
        received_byte = ser.read()

        if received_byte == b'i':  # Início do pacote
            receiving = True
            index = 0
        elif receiving:
            data[index] = ord(received_byte)
            index += 1

            if index == 2:  # Pacote completo
                receiving = False
                processar_dados_recebidos(data)

# Função para processar os dados recebidos
def processar_dados_recebidos(data):
    identifier, value = data[0], data[1]

    if identifier == 0:  # Potenciômetro Red
        valueR.set(value)
        sliderR.set(value)  # Atualiza o valor do slider Red diretamente
    elif identifier == 1:  # Potenciômetro Green
        valueG.set(value)
        sliderG.set(value)  # Atualiza o valor do slider Green diretamente
    elif identifier == 2:  # Potenciômetro Blue
        valueB.set(value)
        sliderB.set(value)  # Atualiza o valor do slider Blue diretamente

    update_color()

# Função para verificar dados serial periodicamente
def verificar_serial():
    receber_dados()
    root.after(100, verificar_serial)

# Configuração da interface gráfica
root = tk.Tk()
root.title("RGB Controller")

# Variáveis para armazenar os valores das cores
valueR = tk.IntVar(value=0)
valueG = tk.IntVar(value=0)
valueB = tk.IntVar(value=0)

# Sliders para Red, Green e Blue
sliderR = tk.Scale(root, from_=0, to=255, orient="horizontal",
                   command=lambda _: slider_alterado("red"))
sliderR.pack()

sliderG = tk.Scale(root, from_=0, to=255, orient="horizontal",
                   command=lambda _: slider_alterado("green"))
sliderG.pack()

sliderB = tk.Scale(root, from_=0, to=255, orient="horizontal",
                   command=lambda _: slider_alterado("blue"))
sliderB.pack()

# Rótulos para mostrar os valores atuais
red_label = tk.Label(root, text="Red: 0")
red_label.pack()

green_label = tk.Label(root, text="Green: 0")
green_label.pack()

blue_label = tk.Label(root, text="Blue: 0")
blue_label.pack()

# Canvas para mostrar a cor resultante
canvas = tk.Canvas(root, width=200, height=200, bg="#000000")
canvas.pack()

# Iniciar verificação de dados serial e loop principal do Tkinter
root.after(100, verificar_serial)
root.mainloop()