#include <iostream>
#include <string>
#include <bitset>
#include <vector>

using namespace std;

// Convert decimal to 4-bit binary
string convert_decimal_to_binary(int decimal) {
    return bitset<4>(decimal).to_string();
}

// Convert binary to decimal
int convert_binary_to_decimal(const string& binary) {
    return stoi(binary, nullptr, 2);
}

// XOR function
string Xor(const string& a, const string& b) {
    string result = "";

    for (size_t i = 0; i < a.size(); i++) {
        result += (a[i] != b[i]) ? '1' : '0';
    }

    return result;
}

// Check binary string
bool is_binary(const string& s) {
    for (char c : s) {
        if (c != '0' && c != '1') {
            return false;
        }
    }
    return true;
}

// Initial Permutation
string initial_permutation(const string& input) {

    const int ip[64] = {
        58,50,42,34,26,18,10,2,
        60,52,44,36,28,20,12,4,
        62,54,46,38,30,22,14,6,
        64,56,48,40,32,24,16,8,
        57,49,41,33,25,17,9,1,
        59,51,43,35,27,19,11,3,
        61,53,45,37,29,21,13,5,
        63,55,47,39,31,23,15,7
    };

    string output = "";

    for (int i = 0; i < 64; i++) {
        output += input[ip[i] - 1];
    }

    return output;
}

// Inverse Initial Permutation
string inverse_initial_permutation(const string& input) {

    const int ip_inverse[64] = {
        40,8,48,16,56,24,64,32,
        39,7,47,15,55,23,63,31,
        38,6,46,14,54,22,62,30,
        37,5,45,13,53,21,61,29,
        36,4,44,12,52,20,60,28,
        35,3,43,11,51,19,59,27,
        34,2,42,10,50,18,58,26,
        33,1,41,9,49,17,57,25
    };

    string output = "";

    for (int i = 0; i < 64; i++) {
        output += input[ip_inverse[i] - 1];
    }

    return output;
}

// Split plaintext into 64-bit blocks
vector<string> split_blocks(string plaintext) {

    vector<string> blocks;

    for (int i = 0; i < plaintext.size(); i += 64) {

        string block = plaintext.substr(i, 64);

        if (block.size() < 64) {
            block.append(64 - block.size(), '0');
        }

        blocks.push_back(block);
    }

    return blocks;
}

class KeyGenerator {

private:

    string key;
    vector<string> roundKeys;

    const int pc1[56] = {
        57,49,41,33,25,17,9,
        1,58,50,42,34,26,18,
        10,2,59,51,43,35,27,
        19,11,3,60,52,44,36,
        63,55,47,39,31,23,15,
        7,62,54,46,38,30,22,
        14,6,61,53,45,37,29,
        21,13,5,28,20,12,4
    };

    const int pc2[48] = {
        14,17,11,24,1,5,
        3,28,15,6,21,10,
        23,19,12,4,26,8,
        16,7,27,20,13,2,
        41,52,31,37,47,55,
        30,40,51,45,33,48,
        44,49,39,56,34,53,
        46,42,50,36,29,32
    };

    string shift_left_once(const string& s) {
        return s.substr(1) + s[0];
    }

    string shift_left_twice(string s) {
        s = shift_left_once(s);
        return shift_left_once(s);
    }

public:

    KeyGenerator(const string& input_key) {
        key = input_key;
    }

    void generateRoundKeys() {

        roundKeys.clear();

        string permuted_key = "";

        for (int i = 0; i < 56; i++) {
            permuted_key += key[pc1[i] - 1];
        }

        string left = permuted_key.substr(0, 28);
        string right = permuted_key.substr(28, 28);

        for (int i = 0; i < 16; i++) {

            if (i == 0 || i == 1 || i == 8 || i == 15) {
                left = shift_left_once(left);
                right = shift_left_once(right);
            }
            else {
                left = shift_left_twice(left);
                right = shift_left_twice(right);
            }

            string combined = left + right;

            string roundKey = "";

            for (int j = 0; j < 48; j++) {
                roundKey += combined[pc2[j] - 1];
            }

            roundKeys.push_back(roundKey);
        }
    }

    vector<string> getRoundKeys() {
        return roundKeys;
    }
};

class DES {

private:

    const int expansion_table[48] = {
        32,1,2,3,4,5,4,5,
        6,7,8,9,8,9,10,11,
        12,13,12,13,14,15,16,17,
        16,17,18,19,20,21,20,21,
        22,23,24,25,24,25,26,27,
        28,29,28,29,30,31,32,1
    };

    const int permutation_tab[32] = {
        16,7,20,21,
        29,12,28,17,
        1,15,23,26,
        5,18,31,10,
        2,8,24,14,
        32,27,3,9,
        19,13,30,6,
        22,11,4,25
    };

    const int sbox[8][4][16] = {

        {
            {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
            {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
            {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
            {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
        },

        {
            {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
            {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
            {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
            {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
        },

        {
            {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
            {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
            {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
            {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
        },

        {
            {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
            {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
            {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
            {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
        },

        {
            {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
            {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
            {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
            {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
        },

        {
            {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
            {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
            {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
            {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
        },

        {
            {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
            {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
            {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
            {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
        },

        {
            {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
            {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
            {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
            {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
        }
    };

    vector<string> round_keys;

public:

    DES(const vector<string>& keys) {
        round_keys = keys;
    }

    string process(string input, bool encrypt_mode) {

        string perm = initial_permutation(input);

        string left = perm.substr(0, 32);
        string right = perm.substr(32, 32);

        for (int round = 0; round < 16; round++) {

            string expanded = "";

            for (int i = 0; i < 48; i++) {
                expanded += right[expansion_table[i] - 1];
            }

            string round_key;

            if (encrypt_mode) {
                round_key = round_keys[round];
            } else {
                round_key = round_keys[15 - round];
            }

            string xored = Xor(expanded, round_key);

            string sbox_result = "";

            for (int i = 0; i < 8; i++) {

                string row_bits =
                    string(1, xored[i * 6]) +
                    string(1, xored[i * 6 + 5]);

                int row = convert_binary_to_decimal(row_bits);

                string col_bits = xored.substr(i * 6 + 1, 4);

                int col = convert_binary_to_decimal(col_bits);

                int val = sbox[i][row][col];

                sbox_result += convert_decimal_to_binary(val);
            }

            string perm2 = "";

            for (int i = 0; i < 32; i++) {
                perm2 += sbox_result[permutation_tab[i] - 1];
            }

            string new_right = Xor(left, perm2);

            left = right;
            right = new_right;
        }

        string combined = right + left;

        return inverse_initial_permutation(combined);
    }

    string encrypt(string input) {
        return process(input, true);
    }

    string decrypt(string input) {
        return process(input, false);
    }
};
int main() {

    string plaintext;
    string key;

    cout << "Enter plaintext: ";
    cin >> plaintext;

    cout << "Enter key (64 bits): ";
    cin >> key;

    // Validate binary input
    if (!is_binary(plaintext) || !is_binary(key)) {
        cout << "Invalid binary input!" << endl;
        return 1;
    }

    // Validate key length
    if (key.size() != 64) {
        cout << "Key must be exactly 64 bits!" << endl;
        return 1;
    }

    // Split plaintext into 64-bit blocks
    vector<string> blocks = split_blocks(plaintext);

    // Generate round keys
    KeyGenerator keygen(key);
    keygen.generateRoundKeys();

    vector<string> roundKeys = keygen.getRoundKeys();

    // Create DES object
    DES des(roundKeys);

    string final_cipher = "";

    // Encrypt all blocks
    for (string block : blocks) {
        final_cipher += des.encrypt(block);
    }

    cout << "Ciphertext: " << final_cipher << endl;

    // Decrypt all blocks
    string decrypted_text = "";

    for (int i = 0; i < final_cipher.size(); i += 64) {

        string cipher_block = final_cipher.substr(i, 64);

        decrypted_text += des.decrypt(cipher_block);
    }

    cout << "Decrypted text: " << decrypted_text << endl;

    return 0;
}