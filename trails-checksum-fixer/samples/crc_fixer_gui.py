"""
Trails Save File CRC32 Fixer
Standalone GUI application for checking and fixing CRC32 checksums
Based on kia_save_compress_decompress.py
"""

import tkinter as tk
from tkinter import filedialog, messagebox, scrolledtext, ttk
from pathlib import Path
import sys

# Constants for Trails series game save file format
CRC32_POLY = 0xEDB88320  # CRC32 polynomial (reversed)
HASH_OFFSET = 0x8  # CRC offset (bytes 8-11)
HASH_LEN = 4  # CRC length (4 bytes)
BODY_OFFSET = HASH_OFFSET + HASH_LEN  # Body start (byte 12)

class CRCFixerGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Trails Save CRC32 Fixer")
        self.root.geometry("700x600")
        self.root.resizable(True, True)
        
        # Configure style
        self.setup_styles()
        
        # Variables
        self.current_file = None
        self.current_data = None
        
        # Create UI
        self.create_widgets()
        
        # Center window
        self.center_window()
    
    def setup_styles(self):
        """Configure ttk styles"""
        style = ttk.Style()
        style.theme_use('clam')
        
        # Configure colors
        bg_dark = '#1a1a1a'
        bg_medium = '#2d2d2d'
        fg_light = '#e8e8e8'
        accent_green = '#00ff41'
        accent_red = '#ff0051'
        
        style.configure('TFrame', background=bg_dark)
        style.configure('TLabel', background=bg_dark, foreground=fg_light, 
                       font=('Consolas', 10))
        style.configure('Title.TLabel', font=('Consolas', 16, 'bold'), 
                       foreground=accent_green)
        style.configure('TButton', font=('Consolas', 10, 'bold'),
                       background=bg_medium, foreground=fg_light)
        style.map('TButton', background=[('active', accent_green)])
        
        self.root.configure(bg=bg_dark)
    
    def center_window(self):
        """Center the window on screen"""
        self.root.update_idletasks()
        width = self.root.winfo_width()
        height = self.root.winfo_height()
        x = (self.root.winfo_screenwidth() // 2) - (width // 2)
        y = (self.root.winfo_screenheight() // 2) - (height // 2)
        self.root.geometry(f'{width}x{height}+{x}+{y}')
    
    def create_widgets(self):
        """Create all UI widgets"""
        # Main container
        main_frame = ttk.Frame(self.root, padding="20")
        main_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        self.root.columnconfigure(0, weight=1)
        self.root.rowconfigure(0, weight=1)
        
        # Title
        title = ttk.Label(main_frame, text="CRC32 FIXER", style='Title.TLabel')
        title.grid(row=0, column=0, columnspan=2, pady=(0, 5))
        
        subtitle = ttk.Label(main_frame, text="Trails Save File CRC32 Calculator & Repair Tool")
        subtitle.grid(row=1, column=0, columnspan=2, pady=(0, 20))
        
        # File selection frame
        file_frame = ttk.LabelFrame(main_frame, text="  File Selection  ", padding="10")
        file_frame.grid(row=2, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=(0, 10))
        
        self.file_label = ttk.Label(file_frame, text="No file selected", 
                                    font=('Consolas', 9))
        self.file_label.grid(row=0, column=0, sticky=tk.W, padx=(0, 10))
        
        browse_btn = ttk.Button(file_frame, text="Browse...", command=self.browse_file)
        browse_btn.grid(row=0, column=1, sticky=tk.E)
        
        file_frame.columnconfigure(0, weight=1)
        
        # CRC Display frame
        crc_frame = ttk.LabelFrame(main_frame, text="  CRC32 Values  ", padding="10")
        crc_frame.grid(row=3, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=(0, 10))
        
        # Calculated CRC
        ttk.Label(crc_frame, text="Calculated:").grid(row=0, column=0, sticky=tk.W)
        self.calc_crc_var = tk.StringVar(value="--")
        self.calc_crc_label = ttk.Label(crc_frame, textvariable=self.calc_crc_var,
                                       font=('Consolas', 11, 'bold'))
        self.calc_crc_label.grid(row=0, column=1, sticky=tk.W, padx=(10, 0))
        
        # Embedded CRC
        ttk.Label(crc_frame, text="Embedded:").grid(row=1, column=0, sticky=tk.W, pady=(5, 0))
        self.embed_crc_var = tk.StringVar(value="--")
        self.embed_crc_label = ttk.Label(crc_frame, textvariable=self.embed_crc_var,
                                        font=('Consolas', 11, 'bold'))
        self.embed_crc_label.grid(row=1, column=1, sticky=tk.W, padx=(10, 0), pady=(5, 0))
        
        # Status
        ttk.Label(crc_frame, text="Status:").grid(row=2, column=0, sticky=tk.W, pady=(5, 0))
        self.status_var = tk.StringVar(value="Ready")
        self.status_label = ttk.Label(crc_frame, textvariable=self.status_var,
                                     font=('Consolas', 10))
        self.status_label.grid(row=2, column=1, sticky=tk.W, padx=(10, 0), pady=(5, 0))
        
        # Action buttons frame
        button_frame = ttk.Frame(main_frame)
        button_frame.grid(row=4, column=0, columnspan=2, pady=(0, 10))
        
        self.check_btn = ttk.Button(button_frame, text="Check CRC32", 
                                    command=self.check_crc, state='disabled')
        self.check_btn.grid(row=0, column=0, padx=(0, 5), ipadx=10, ipady=5)
        
        self.fix_btn = ttk.Button(button_frame, text="Fix CRC32 & Save", 
                                  command=self.fix_crc, state='disabled')
        self.fix_btn.grid(row=0, column=1, padx=(5, 0), ipadx=10, ipady=5)
        
        # Console output frame
        console_frame = ttk.LabelFrame(main_frame, text="  Console Output  ", padding="10")
        console_frame.grid(row=5, column=0, columnspan=2, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(0, 10))
        
        self.console = scrolledtext.ScrolledText(console_frame, height=15, 
                                                 bg='#0d0d0d', fg='#00ff41',
                                                 font=('Consolas', 9),
                                                 insertbackground='#00ff41')
        self.console.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        self.console.config(state='disabled')
        
        console_frame.columnconfigure(0, weight=1)
        console_frame.rowconfigure(0, weight=1)
        
        # Configure grid weights
        main_frame.columnconfigure(0, weight=1)
        main_frame.rowconfigure(5, weight=1)
        
        # Initial console message
        self.log("CRC32 Fixer ready. Load a file to begin.", 'info')
    
    def log(self, message, msg_type='info'):
        """Add message to console"""
        self.console.config(state='normal')
        
        prefix = {
            'info': '> ',
            'success': '✓ ',
            'error': '✗ ',
            'calc': '  '
        }.get(msg_type, '> ')
        
        self.console.insert(tk.END, f"{prefix}{message}\n")
        self.console.see(tk.END)
        self.console.config(state='disabled')
        self.root.update()
    
    def clear_console(self):
        """Clear console output"""
        self.console.config(state='normal')
        self.console.delete('1.0', tk.END)
        self.console.config(state='disabled')
    
    def browse_file(self):
        """Open file browser dialog"""
        filename = filedialog.askopenfilename(
            title="Select Trails Save File",
            filetypes=[
                ("All Files", "*.*"),
                ("DAT Files", "*.dat"),
                ("Save Files", "*.sav")
            ]
        )
        
        if filename:
            self.load_file(filename)
    
    def load_file(self, filename):
        """Load file and enable buttons"""
        try:
            self.current_file = Path(filename)
            
            with open(filename, 'rb') as f:
                self.current_data = bytearray(f.read())
            
            self.file_label.config(text=f"{self.current_file.name} ({len(self.current_data)} bytes)")
            self.check_btn.config(state='normal')
            self.fix_btn.config(state='normal')
            
            self.clear_console()
            self.log(f"Loaded: {self.current_file.name}", 'success')
            self.log(f"File size: {len(self.current_data)} bytes", 'info')
            
            # Reset display
            self.calc_crc_var.set("--")
            self.embed_crc_var.set("--")
            self.status_var.set("Ready")
            
        except Exception as e:
            messagebox.showerror("Error", f"Failed to load file:\n{e}")
            self.log(f"Error loading file: {e}", 'error')
    
    def calc_crc32(self, data):
        """Calculate CRC32 using the algorithm from Python script"""
        # Build CRC32 lookup table
        table = [0] * 256
        for i in range(256):
            v = i
            for _ in range(8):
                v = (v >> 1) ^ CRC32_POLY if v & 1 else v >> 1
            table[i] = v
        
        # Calculate CRC with body length as init value
        body_data = data[BODY_OFFSET:]
        crc = len(body_data)
        
        self.log(f"CRC init value: 0x{crc:08X} ({crc} bytes from offset {BODY_OFFSET})", 'calc')
        
        for b in body_data:
            crc = table[(b ^ (crc & 0xFF))] ^ (crc >> 8)
        
        # Return as 4-byte little-endian
        result = bytearray(4)
        result[0] = crc & 0xFF
        result[1] = (crc >> 8) & 0xFF
        result[2] = (crc >> 16) & 0xFF
        result[3] = (crc >> 24) & 0xFF
        
        return result
    
    def check_crc(self):
        """Check CRC32 in current file"""
        if not self.current_data:
            return
        
        try:
            self.clear_console()
            self.log("Checking CRC32...", 'info')
            
            if len(self.current_data) < BODY_OFFSET + HASH_LEN:
                self.log("Error: File too small for CRC data", 'error')
                messagebox.showerror("Error", "File is too small to contain CRC data")
                return
            
            # Calculate CRC
            calculated = self.calc_crc32(self.current_data)
            embedded = self.current_data[HASH_OFFSET:HASH_OFFSET + HASH_LEN]
            
            calc_hex = calculated.hex().upper()
            embed_hex = embedded.hex().upper()
            
            self.calc_crc_var.set(f"0x{calc_hex}")
            self.embed_crc_var.set(f"0x{embed_hex}")
            
            self.log(f"Calculated CRC32: 0x{calc_hex}", 'info')
            self.log(f"Embedded CRC32:   0x{embed_hex}", 'info')
            
            if calc_hex == embed_hex:
                self.status_var.set("✓ CRC32 Valid")
                self.log("CRC32 matches - file integrity OK", 'success')
                messagebox.showinfo("CRC32 Check", "✓ CRC32 is valid!\n\nFile integrity verified.")
            else:
                self.status_var.set("✗ CRC32 Mismatch")
                self.log("CRC32 mismatch detected", 'error')
                self.log("Use 'Fix CRC32 & Save' to repair the file", 'info')
                messagebox.showwarning("CRC32 Check", "✗ CRC32 mismatch detected!\n\nFile may be corrupted or modified.\nUse 'Fix CRC32 & Save' to repair.")
        
        except Exception as e:
            self.log(f"Error checking CRC: {e}", 'error')
            messagebox.showerror("Error", f"Failed to check CRC:\n{e}")
    
    def fix_crc(self):
        """Fix CRC32 and save file"""
        if not self.current_data:
            return
        
        try:
            self.clear_console()
            self.log("Calculating correct CRC32...", 'info')
            
            if len(self.current_data) < BODY_OFFSET + HASH_LEN:
                self.log("Error: File too small for CRC data", 'error')
                messagebox.showerror("Error", "File is too small to contain CRC data")
                return
            
            # Calculate correct CRC
            correct_crc = self.calc_crc32(self.current_data)
            
            # Write correct CRC to data
            for i in range(HASH_LEN):
                self.current_data[HASH_OFFSET + i] = correct_crc[i]
            
            crc_hex = correct_crc.hex().upper()
            self.log(f"Fixed CRC32: 0x{crc_hex}", 'success')
            self.log(f"Written to bytes {HASH_OFFSET}-{HASH_OFFSET + HASH_LEN - 1} (offset 0x{HASH_OFFSET:X})", 'info')
            
            # Ask where to save
            base_name = self.current_file.stem
            default_name = f"{base_name}.fixed"
            
            save_path = filedialog.asksaveasfilename(
                title="Save Fixed File",
                initialfile=default_name,
                defaultextension="",
                filetypes=[
                    ("All Files", "*.*"),
                    ("DAT Files", "*.dat"),
                    ("Save Files", "*.sav")
                ]
            )
            
            if save_path:
                with open(save_path, 'wb') as f:
                    f.write(self.current_data)
                
                self.log(f"File saved: {Path(save_path).name}", 'success')
                self.log("Verification: Re-load the fixed file to confirm", 'info')
                
                self.calc_crc_var.set(f"0x{crc_hex}")
                self.embed_crc_var.set(f"0x{crc_hex}")
                self.status_var.set("✓ CRC32 Fixed")
                
                messagebox.showinfo("Success", f"✓ CRC32 fixed and saved!\n\nFile: {Path(save_path).name}\n\nYou can now re-load the file to verify.")
            else:
                self.log("Save cancelled", 'info')
        
        except Exception as e:
            self.log(f"Error fixing CRC: {e}", 'error')
            messagebox.showerror("Error", f"Failed to fix CRC:\n{e}")

def main():
    """Main application entry point"""
    root = tk.Tk()
    app = CRCFixerGUI(root)
    root.mainloop()

if __name__ == '__main__':
    main()
