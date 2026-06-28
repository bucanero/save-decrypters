import hashlib
import hmac
from tkinter import filedialog


HMAC_KEY      = b"1FB00CC8D8D94CD0A94C847C2F04A921"

prefix = bytes.fromhex('00 00 00 00 14 00 00 00')

def fix_save(data):
    search_range = data[-0x1000:]
    
    first_off = search_range.find(prefix)

    if first_off == -1:
        print("Prefix not found")
        return

    absolute_offset = len(data) - 0x1000 + first_off
    print(f"Absolute offset: {absolute_offset:#x}")

    return absolute_offset

def patch_save(data):

    cleaning_offset=fix_save(data)

    cleaning_offset+=4

    cleaned_data= data[:cleaning_offset]


    computed = hmac.new(HMAC_KEY, cleaned_data, hashlib.sha1).digest()

    final_data= cleaned_data + bytearray.fromhex('14 00 00 00') + bytearray(computed)
    
    print(f"Computed: {computed.hex().upper()}")

    return final_data



if __name__ == "__main__":
    import tkinter as tk
    from tkinter import filedialog

    root = tk.Tk()
    root.title("Save Fixer")

    def run_fix():
        path = filedialog.askopenfilename(title="Select save file")
        if not path:
            return

        with open(path, 'rb') as f:
            data = bytearray(f.read())

        fixed = patch_save(data)

        with open(path, 'wb') as f:
            f.write(fixed)

        print("[+] Save fixed successfully")

    fix_button = tk.Button(root, text="Fix Save", command=run_fix)
    fix_button.pack(padx=20, pady=20)

    root.mainloop()