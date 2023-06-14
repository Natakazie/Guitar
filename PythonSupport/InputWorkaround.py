import serial
from pynput.keyboard import Controller, Key

# Erstellen Sie ein Keyboard-Controller-Objekt
keyboard = Controller()

# Stellen Sie eine Verbindung zum seriellen Port her
ser = serial.Serial('COM8', 9600)

while True:
    # Liest eine Zeile von der seriellen Schnittstelle
    line = ser.readline().decode('utf-8').strip()
    print(line)
    commands = line.split(";")

    for command in commands:
        if command == 'A':
           keyboard.press('a')
        elif command == 'a':
            keyboard.release('a')
        elif command == 'F':
            keyboard.press('f')
        elif command == 'f':
            keyboard.release('f')    
        elif command == 'S':
            keyboard.press('s')
        elif command == 's':
            keyboard.release('s')
        elif command == 'D':
            keyboard.press('d')
        elif command == 'd':
            keyboard.release('d')
        elif command == 'G':
            keyboard.press('g')
        elif command == 'g':
            keyboard.release('g')

        elif command == 'up':
            keyboard.press(Key.up)
            keyboard.release(Key.down)
        elif command == 'down':
            keyboard.press(Key.down)
            keyboard.release(Key.up)
        elif command == 'left':
            keyboard.press(Key.left)
            keyboard.release(Key.right)  
        elif command == 'right':
            keyboard.press(Key.right)
            keyboard.release(Key.left)
        elif command == 'neutral':
            keyboard.release(Key.up)
            keyboard.release(Key.down)
            keyboard.release(Key.left)
            keyboard.release(Key.right)                     
    # Entsprechende Tastendrücke ausführen
