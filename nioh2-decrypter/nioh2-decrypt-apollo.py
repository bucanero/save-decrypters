# Nioh 2 PS4 Save Decrypter
# https://github.com/alfizari/Nioh-2-Save-Decrypt-PS4
#
"""
This is an exercise in secure symmetric-key encryption, implemented in pure
Python (no external libraries needed).

Original AES-128 implementation by Bo Zhu (http://about.bozhu.me) at 
https://github.com/bozhu/AES-Python . PKCS#7 padding, CBC mode, PKBDF2, HMAC,
byte array and string support added by me at https://github.com/boppreh/aes. 
Other block modes contributed by @righthandabacus.


Although this is an exercise, the `encrypt` and `decrypt` functions should
provide reasonable security to encrypted messages.
"""
import gc

NIOH_AES_KEY = bytes([
    0x1D, 0x8E, 0xBB, 0x9D, 0x6A, 0x2E, 0x05, 0xBB, 0x89, 0x6B, 0xF6, 0x9C, 0x03, 0x5A, 0x65, 0x90,
])

NIOH_AES_IV = bytes([
    0xBB, 0x8B, 0xDD, 0xFB, 0x81, 0x2F, 0x8D, 0x35, 0x6A, 0xC9, 0x3A, 0xC4, 0x11, 0x2A, 0x7F, 0x82,
])


s_box = bytes([ 0x1C, 0x2F, 0x03, 0x53, 0xA3, 0x01, 0x49, 0xDA, 0xA6, 0xCD, 0xE0, 0x8A, 0x19, 0xA7, 0x04, 0xD4,
							0x06, 0x1A, 0xDA, 0x49, 0x08, 0xE2, 0xF6, 0xB2, 0x9E, 0xE1, 0x22, 0x49, 0xCE, 0x7B, 0x7E, 0x5E,
							0xA0, 0x09, 0x2A, 0x63, 0xAF, 0x49, 0xCE, 0x70, 0x7B, 0x3C, 0x23, 0x80, 0xFA, 0x17, 0x47, 0xF2,
							0x62, 0x62, 0x6C, 0x59, 0x10, 0xCC, 0x29, 0x9C, 0xB5, 0x46, 0x58, 0xC7, 0x44, 0x13, 0xE7, 0x38,
							0xD5, 0xAF, 0x27, 0x83, 0xD4, 0xD5, 0xA0, 0x9E, 0xE3, 0x76, 0x3B, 0x85, 0x04, 0xD9, 0xD6, 0x98,
							0x60, 0x66, 0xD4, 0x78, 0x53, 0xEA, 0xCA, 0x0E, 0x8D, 0x56, 0x53, 0x44, 0xE2, 0xEF, 0xBD, 0xA9,
							0x9B, 0x10, 0x0A, 0xA1, 0x13, 0x93, 0xF0, 0x43, 0x0B, 0x7C, 0x39, 0x8A, 0x47, 0xDF, 0xD3, 0xC5,
							0x0E, 0x34, 0x31, 0xA6, 0xAE, 0x5A, 0xB8, 0xE7, 0xE6, 0x31, 0x43, 0xC0, 0xAA, 0x0F, 0xE0, 0x82,
							0x12, 0x4C, 0xD1, 0xDF, 0x8B, 0xA5, 0xAC, 0x70, 0xC5, 0x3D, 0x1B, 0x8E, 0x93, 0x17, 0x4D, 0x79,
							0x4E, 0xCE, 0x63, 0xC4, 0x33, 0x0E, 0x14, 0x57, 0xF0, 0xD8, 0x19, 0x5B, 0x9B, 0x61, 0x71, 0xF2,
							0x2B, 0x33, 0x7E, 0xFD, 0x2C, 0x0B, 0xB6, 0x23, 0x20, 0xB9, 0xD4, 0x91, 0x19, 0x94, 0x04, 0xA4,
							0x30, 0x13, 0x8A, 0xF1, 0xD0, 0x05, 0xEC, 0x5E, 0xAC, 0x4A, 0xD4, 0xD6, 0xA5, 0x17, 0x7F, 0xF9,
							0xE5, 0xF6, 0x00, 0x29, 0xD7, 0x93, 0x2D, 0x5E, 0x2C, 0xF1, 0x81, 0xA3, 0xB7, 0x63, 0x39, 0x57,
							0xC2, 0x33, 0x87, 0x2D, 0xA8, 0x3F, 0x02, 0xCC, 0x08, 0x67, 0x74, 0x60, 0xD8, 0xF0, 0xDA, 0x67,
							0x40, 0x64, 0x87, 0x55, 0xBB, 0x7F, 0xF2, 0x10, 0xC9, 0x03, 0x14, 0xB5, 0x80, 0x66, 0xCB, 0x91,
							0xF6, 0x1F, 0x79, 0x58, 0x88, 0xBC, 0x95, 0xC2, 0x06, 0x5F, 0xE9, 0x09, 0x32, 0xED, 0x9B, 0x85 
])

inv_s_box = s_box


def sub_bytes(s):
    for i in range(4):
        for j in range(4):
            s[i][j] = s_box[s[i][j]]


def inv_sub_bytes(s):
    for i in range(4):
        for j in range(4):
            s[i][j] = inv_s_box[s[i][j]]


def shift_rows(s):
    s[0][1], s[1][1], s[2][1], s[3][1] = s[1][1], s[2][1], s[3][1], s[0][1]
    s[0][2], s[1][2], s[2][2], s[3][2] = s[2][2], s[3][2], s[0][2], s[1][2]
    s[0][3], s[1][3], s[2][3], s[3][3] = s[3][3], s[0][3], s[1][3], s[2][3]


def inv_shift_rows(s):
    s[0][1], s[1][1], s[2][1], s[3][1] = s[3][1], s[0][1], s[1][1], s[2][1]
    s[0][2], s[1][2], s[2][2], s[3][2] = s[2][2], s[3][2], s[0][2], s[1][2]
    s[0][3], s[1][3], s[2][3], s[3][3] = s[1][3], s[2][3], s[3][3], s[0][3]

def add_round_key(s, k):
    for i in range(4):
        for j in range(4):
            s[i][j] ^= k[i][j]


# learned from https://web.archive.org/web/20100626212235/http://cs.ucsb.edu/~koc/cs178/projects/JT/aes.c
xtime = lambda a: (((a << 1) ^ 0x1B) & 0xFF) if (a & 0x80) else (a << 1)


def mix_single_column(a):
    # see Sec 4.1.2 in The Design of Rijndael
    t = a[0] ^ a[1] ^ a[2] ^ a[3]
    u = a[0]
    a[0] ^= t ^ xtime(a[0] ^ a[1])
    a[1] ^= t ^ xtime(a[1] ^ a[2])
    a[2] ^= t ^ xtime(a[2] ^ a[3])
    a[3] ^= t ^ xtime(a[3] ^ u)


def mix_columns(s):
    for i in range(4):
        mix_single_column(s[i])


def inv_mix_columns(s):
    # see Sec 4.1.3 in The Design of Rijndael
    for i in range(4):
        u = xtime(xtime(s[i][0] ^ s[i][2]))
        v = xtime(xtime(s[i][1] ^ s[i][3]))
        s[i][0] ^= u
        s[i][1] ^= v
        s[i][2] ^= u
        s[i][3] ^= v

    mix_columns(s)


r_con = (
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40,
    0x80, 0x1B, 0x36, 0x6C, 0xD8, 0xAB, 0x4D, 0x9A,
    0x2F, 0x5E, 0xBC, 0x63, 0xC6, 0x97, 0x35, 0x6A,
    0xD4, 0xB3, 0x7D, 0xFA, 0xEF, 0xC5, 0x91, 0x39,
)


def bytes2matrix(text):
    """ Converts a 16-byte array into a 4x4 matrix.  """
    return [list(text[i:i+4]) for i in range(0, len(text), 4)]

def matrix2bytes(matrix):
    """ Converts a 4x4 matrix into a 16-byte array.  """
    return bytes(sum(matrix, []))

def xor_bytes(a, b):
    """ Returns a new byte array with the elements xor'ed. """
    return bytes(i^j for i, j in zip(a, b))

def inc_bytes(a):
    """ Returns a new byte array with the value increment by 1 """
    out = list(a)
    for i in reversed(range(len(out))):
        if out[i] == 0xFF:
            out[i] = 0
        else:
            out[i] += 1
            break
    return bytes(out)

def pad(plaintext):
    """
    Pads the given plaintext with PKCS#7 padding to a multiple of 16 bytes.
    Note that if the plaintext size is a multiple of 16,
    a whole block will be added.
    """
    padding_len = 16 - (len(plaintext) % 16)
    padding = bytes([padding_len] * padding_len)
    return plaintext + padding

def unpad(plaintext):
    """
    Removes a PKCS#7 padding, returning the unpadded text and ensuring the
    padding was correct.
    """
    padding_len = plaintext[-1]
    assert padding_len > 0
    message, padding = plaintext[:-padding_len], plaintext[-padding_len:]
    assert all(p == padding_len for p in padding)
    return message

def split_blocks(message, block_size=16, require_padding=True):
        assert len(message) % block_size == 0 or not require_padding
        return [message[i:i+16] for i in range(0, len(message), block_size)]


class AES:
    """
    Class for AES-128 encryption with CBC mode and PKCS#7.

    This is a raw implementation of AES, without key stretching or IV
    management. Unless you need that, please use `encrypt` and `decrypt`.
    """
    rounds_by_key_size = {16: 10, 24: 12, 32: 14}
    def __init__(self, master_key):
        """
        Initializes the object with a given key.
        """
        assert len(master_key) in AES.rounds_by_key_size
        self.n_rounds = AES.rounds_by_key_size[len(master_key)]
        self._key_matrices = self._expand_key(master_key)

    def _expand_key(self, master_key):
        """
        Expands and returns a list of key matrices for the given master_key.
        """
        # CUSTOM MODIFICATION: Reverse each 4-byte word to match C++ implementation
        key_bytes = bytearray(master_key)
        for i in range(0, len(key_bytes), 4):
            # Reverse each 4-byte word: [0,1,2,3] becomes [3,2,1,0]
            tmp = key_bytes[i:i+4]
            key_bytes[i]   = tmp[3]
            key_bytes[i+1] = tmp[2]
            key_bytes[i+2] = tmp[1]
            key_bytes[i+3] = tmp[0]
        
        # Initialize round keys with raw key material.
        key_columns = bytes2matrix(bytes(key_bytes))
        iteration_size = len(master_key) // 4

        i = 1
        while len(key_columns) < (self.n_rounds + 1) * 4:
            # Copy previous word.
            word = list(key_columns[-1])

            # Perform schedule_core once every "row".
            if len(key_columns) % iteration_size == 0:
                # Circular shift.
                word.append(word.pop(0))
                # Map to S-BOX.
                word = [s_box[b] for b in word]
                # XOR with first byte of R-CON, since the others bytes of R-CON are 0.
                word[0] ^= r_con[i]
                i += 1
            elif len(master_key) == 32 and len(key_columns) % iteration_size == 4:
                # Run word through S-box in the fourth iteration when using a
                # 256-bit key.
                word = [s_box[b] for b in word]

            # XOR with equivalent word from previous iteration.
            word = xor_bytes(word, key_columns[-iteration_size])
            key_columns.append(word)

        # Group key words in 4x4 byte matrices.
        return [key_columns[4*i : 4*(i+1)] for i in range(len(key_columns) // 4)]

    def encrypt_block(self, plaintext):
        """
        Encrypts a single block of 16 byte long plaintext.
        """
        assert len(plaintext) == 16

        plain_state = bytes2matrix(plaintext)

        add_round_key(plain_state, self._key_matrices[0])

        for i in range(1, self.n_rounds):
            sub_bytes(plain_state)
            shift_rows(plain_state)
            mix_columns(plain_state)
            add_round_key(plain_state, self._key_matrices[i])

        sub_bytes(plain_state)
        shift_rows(plain_state)
        add_round_key(plain_state, self._key_matrices[-1])

        return matrix2bytes(plain_state)

    def decrypt_block(self, ciphertext):
        """
        Decrypts a single block of 16 byte long ciphertext.
        """
        assert len(ciphertext) == 16

        cipher_state = bytes2matrix(ciphertext)

        add_round_key(cipher_state, self._key_matrices[-1])
        inv_shift_rows(cipher_state)
        inv_sub_bytes(cipher_state)

        for i in range(self.n_rounds - 1, 0, -1):
            add_round_key(cipher_state, self._key_matrices[i])
            inv_mix_columns(cipher_state)
            inv_shift_rows(cipher_state)
            inv_sub_bytes(cipher_state)

        add_round_key(cipher_state, self._key_matrices[0])

        return matrix2bytes(cipher_state)

    def crypt_ctr(self, ciphertext, iv):
        """
        Decrypts `ciphertext` using CTR mode with the given nounce/IV.
        """
        assert len(iv) == 16
        assert len(ciphertext) % 16 == 0

        nonce = iv
        for i in range(0, len(ciphertext), 16):
            data_block = ciphertext[i:i+16]
            # CTR mode decrypt: ciphertext XOR encrypt(nonce)
            data_block = xor_bytes(data_block, self.encrypt_block(nonce))
            ciphertext[i:i+16] = data_block
            nonce = inc_bytes(nonce)


def nioh2_crypt(data: bytearray):
    # Nioh 2 PS4 Save Decrypter - Custom AES implementation
    # https://github.com/alfizari/Nioh-2-Save-Decrypt-PS4

    AES(NIOH_AES_KEY).crypt_ctr(data, NIOH_AES_IV)

    # Clean up
    gc.collect()

    return 0
