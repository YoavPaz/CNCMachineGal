import subprocess
import threading
import serial
import time
import sys

# =========================
# CONFIG
# =========================

FQBN = "arduino:avr:uno"
PORT = "/dev/cu.usbmodem2017_2_251"
BAUDRATE = 115200
PROJECT_DIR = "."

# =========================
# HELPERS
# =========================

def run_command(cmd):
    process = subprocess.Popen(cmd)

    process.wait()

    if process.returncode != 0:
        print(f"Command failed: {' '.join(cmd)}")
        sys.exit(1)

# =========================
# COMPILE
# =========================

print("Compiling...")

run_command([
    "arduino-cli",
    "compile",
    "--fqbn",
    FQBN,
    PROJECT_DIR
])

# =========================
# UPLOAD
# =========================

print("Uploading...")

run_command([
    "arduino-cli",
    "upload",
    "-p",
    PORT,
    "--fqbn",
    FQBN,
    PROJECT_DIR
])

print("Upload complete.")

# Give Arduino time to reboot
time.sleep(2)

# =========================
# SERIAL MONITOR
# =========================

print(f"Opening serial monitor on {PORT} @ {BAUDRATE}")
print("Type commands and press ENTER.")
print("Press CTRL+C to quit.\n")

ser = serial.Serial(PORT, BAUDRATE, timeout=0.1)

# =========================
# SERIAL READER THREAD
# =========================

def serial_reader():
    while True:
        try:
            if ser.in_waiting:
                data = ser.readline().decode(errors="ignore").strip()

                if data:
                    print(f"\n[ARDUINO] {data}")

        except:
            break

reader_thread = threading.Thread(target=serial_reader, daemon=True)
reader_thread.start()

# =========================
# USER INPUT LOOP
# =========================

try:
    while True:
        msg = input("> ")

        ser.write((msg + "\n").encode())

except KeyboardInterrupt:
    print("\nExiting...")

finally:
    ser.close()
