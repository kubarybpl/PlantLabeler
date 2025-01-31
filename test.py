import sys
import time

print("Rozpoczynam pracę...")
sys.stdout.flush()  # Wymuszenie natychmiastowego wysłania danych

for i in range(5):
    print(f"Postęp: {i+1}/5")
    sys.stdout.flush()
    time.sleep(0.5)

print("Zadanie zakończone pomyślnie!")
sys.stdout.flush()