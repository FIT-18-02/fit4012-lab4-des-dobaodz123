# Report 1 page - Lab 4 DES / TripleDES

## Mục tiêu / Objective

Đồng Đỗ Bảo: em thực hiện bài lab này để hiểu rõ cách hoạt động của thuật toán DES và TripleDES, đồng thời thực hành quá trình mã hóa dữ liệu bằng mạng Feistel, sinh round keys và xử lý nhiều block dữ liệu.

---

## Cách làm / Method

Trong bài lab này, em phân tích chương trình DES có sẵn trong file `des.cpp`.

Chương trình gồm hai phần chính:

- `KeyGenerator`: dùng để sinh 16 round keys.
- `DES`: thực hiện 16 vòng Feistel để mã hóa dữ liệu.

Quá trình sinh round keys:

1. Key 64 bit đi qua bảng PC1 để loại bỏ parity bits và giảm xuống còn 56 bit.
2. Chia key thành hai nửa trái phải, mỗi phần 28 bit.
3. Thực hiện dịch vòng trái ở từng round.
4. Ghép lại và đi qua bảng PC2 để tạo khóa vòng 48 bit.
5. Sinh tổng cộng 16 round keys.

Quá trình mã hóa DES:

1. Plaintext đi qua Initial Permutation (IP).
2. Chia thành hai phần Left và Right, mỗi phần 32 bit.
3. Thực hiện 16 vòng Feistel:
   - mở rộng Right từ 32 bit lên 48 bit;
   - XOR với round key;
   - đi qua S-box;
   - permutation;
   - XOR với Left;
   - hoán đổi hai nửa.
4. Sau vòng cuối thực hiện swap.
5. Áp dụng inverse initial permutation để tạo ciphertext.

Ngoài ra chương trình còn sử dụng:
- XOR bit;
- chuyển đổi nhị phân ↔ thập phân;
- expansion table;
- S-box substitution.

Trong Q2 em bổ sung:
- nhập plaintext từ bàn phím;
- nhập key từ bàn phím;
- kiểm tra chuỗi nhị phân hợp lệ;
- chia plaintext thành nhiều block 64 bit;
- zero padding cho block cuối;
- mã hóa nhiều block.

Trong Q3 em bổ sung hàm giải mã DES bằng cách đảo ngược thứ tự round keys.

Trong Q4 em triển khai TripleDES theo mô hình EDE:
- Encrypt với K1;
- Decrypt với K2;
- Encrypt với K3.

Đồng thời viết TripleDES decrypt để khôi phục plaintext.

---

## Kết quả / Result

Chương trình biên dịch và chạy thành công bằng:

```bash
g++ des.cpp -o des
./des.exe