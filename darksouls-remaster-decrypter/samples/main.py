from Crypto.Cipher import AES
from tkinter import ttk, filedialog, messagebox
import tkinter as tk
import os
import hashlib

window = tk.Tk()
window.title("DRS Save File Encryptor/Decryptor")
window.geometry("300x200")
window.resizable(True, True)
key = bytes.fromhex('20EC4B7519C2BD15E70C1EE4B204B8CB')


def decryptor():

    file_path = filedialog.askopenfilename(title="Select Encrypted Save File")

    if not file_path:
        print("No file selected.")
        exit()
    with open(file_path, 'rb') as f:
        data = f.read()
    iv = data[:16]
    ciphertext = data[16:]
    cipher = AES.new(key, AES.MODE_CBC, iv)
    decrypted = cipher.decrypt(ciphertext)


    file_name = os.path.basename(file_path)
    output= filedialog.asksaveasfilename(initialfile=file_name, title="Save Decrypted File")
    output_path = output
    with open(output_path, 'wb') as f:
        f.write(decrypted)
    messagebox.showinfo("Decryption Complete", f"Decrypted file saved as: {output_path}")

def encryptor():


    file_path = filedialog.askopenfilename(title="Select Save File to Encrypt")

    if not file_path:
        print("No file selected.")
        exit()
    with open(file_path, 'rb') as f:
        data = f.read()

    ##Crypto
    iv= os.urandom(16)  # Generate a random IV

    ciphertext = data  # Original data to encrypt
    cipher = AES.new(key, AES.MODE_CBC, iv)
    encrypted = cipher.encrypt(ciphertext)

    # Prepend IV
    with_iv = iv + encrypted[:-16]

    # Calculate checksum over [IV + Encrypted]
    checksum = hashlib.md5(with_iv).digest()

    # Final output: [IV][Encrypted Data][Checksum]
    final_output = with_iv+ checksum
    

    file_name = os.path.basename(file_path)
    output= filedialog.asksaveasfilename(initialfile=file_name ,title="Save Encrypted File",
                                          )
    if not output:
        print("No output file selected.")
        return
    
    output_path = output
    with open(output_path, 'wb') as f:
        f.write(final_output)
    messagebox.showinfo("Encryption Complete", f"Encrypted file saved as: {output_path}")


def recalc_checksum(data):
    checksum=hashlib.md5(data).digest()
    return checksum


#UI
window.columnconfigure(0, weight=1)
window.rowconfigure(0, weight=1)

# Create main frame with padding
main_frame = ttk.Frame(window, padding=10)
main_frame.grid(row=0, column=0, sticky="nsew")

# Allow content in main_frame to expand
main_frame.columnconfigure(0, weight=1)
main_frame.rowconfigure(0, weight=1)

# Styling constants
button_width = 20
button_padding_y = 15
button_padding_x = 20

# Add buttons using grid
decrypt_button = ttk.Button(main_frame, text="Decrypt File", width=button_width, command=decryptor)
decrypt_button.grid(row=0, column=0, pady=button_padding_y, padx=button_padding_x)

encrypt_button = ttk.Button(main_frame, text="Encrypt Files", width=button_width, command=encryptor)
encrypt_button.grid(row=1, column=0, pady=button_padding_y, padx=button_padding_x)
window.mainloop()