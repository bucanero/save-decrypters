import tkinter as tk
from tkinter import filedialog, messagebox

def check():
    filepath = filedialog.askopenfilename(title="Select file to encrypt")
    if not filepath:
        return
    with open(filepath, "rb") as f:

        data = bytearray(f.read())
        # Zero out checksum field (offset 0x1A, 4 bytes)
    data[0x8:0x0C] = b'\x00\x00\x00\x00'

    # Calculate checksum (sum of all bytes)
    checksum = sum(data) & 0xFFFFFFFF  # Keep as 32-bit

    # Write checksum back (little-endian)
    data[0x8:0x0C] = checksum.to_bytes(4, 'little')
    print(checksum)
    data=bytes(data)
    with open(filepath, "wb") as f:
        f.write(data)

# Tkinter GUI
root = tk.Tk()
root.title("Digimon checksum")

tk.Button(root, text="Fix checksum", width=20, command=check).pack(pady=10)


root.mainloop()
