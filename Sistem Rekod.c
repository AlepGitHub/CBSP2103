#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_PRODUCTS 10
#define CODE_LEN 20
#define NAME_LEN 50

// Fungsi
void restockProduct(char codes[][CODE_LEN], char names[][NAME_LEN], float prices[], int stock[], int *n);
float sellProduct(char codes[][CODE_LEN], float prices[], int stock[], float revenue[], int n);
void displayProducts(char codes[][CODE_LEN], char names[][NAME_LEN], float prices[], int stock[], int n);
void generateSummary(char names[][NAME_LEN], int stock[], float revenue[], int n);
int getValidMenuChoice(void);
void inputValidCode(char *code);
void inputValidName(char *name);
float inputValidPrice();
int inputValidQuantity();
void clearScreen();
void pauseScreen();

int main() {
    char codes[MAX_PRODUCTS][CODE_LEN];
    char names[MAX_PRODUCTS][NAME_LEN];
    float prices[MAX_PRODUCTS];
    int stock[MAX_PRODUCTS];
    float revenue[MAX_PRODUCTS];
    int n = 0;

    for (int i = 0; i < MAX_PRODUCTS; i++) {
        stock[i] = 0;
        revenue[i] = 0.0f;
    }

    int choice;
    do {
        clearScreen();

        printf("\n=== Sistem Pengurusan Inventori ===\n");
        printf("1. Tambah / Mengisi Produk\n");
        printf("2. Rekod Jualan\n");
        printf("3. Papar Senarai Produk\n");
        printf("4. Papar Ringkasan\n");
        printf("5. Keluar\n");
        printf("Pilihan: ");

        choice = getValidMenuChoice();
        if (choice == -1) continue; // skip switch, return to top of loop


        switch (choice) {
            case 1:
                restockProduct(codes, names, prices, stock, &n);
                break;
            case 2: {
                float amount = sellProduct(codes, prices, stock, revenue, n);
                printf("Jumlah hasil jualan: RM%.2f\n", amount);
                break;
            }
            case 3:
                displayProducts(codes, names, prices, stock, n);
                break;
            case 4:
                generateSummary(names, stock, revenue, n);
                break;
            case 5:
                printf("Keluar dari sistem. Terima kasih!\n");
                break;
        }

        if (choice != 5) {
            pauseScreen();
        }

    } while (choice != 5);

    return 0;
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    printf("\nTekan ENTER untuk kembali ke menu...");
    while (getchar() != '\n');
    getchar();
}

void restockProduct(char codes[][CODE_LEN], char names[][NAME_LEN], float prices[], int stock[], int *n) {
    char code[CODE_LEN];
    int quantity;
    float price;
    char choice;

    inputValidCode(code);

    int found = -1;
    for (int i = 0; i < *n; i++) {
        if (strcmp(codes[i], code) == 0) {
            found = i;
            break;
        }
    }

    if (found != -1) {
        quantity = inputValidQuantity();
        if (stock[found] + quantity > 100) {
            printf("Tidak boleh menambah Stok. Melebihi Had yang ditetapkan.\n");
        } else {
            stock[found] += quantity;
            printf("Stok produk telah dikemaskini. Stok semasa: %d\n", stock[found]);
        }
    } else {
        printf("Kod produk tidak wujud dalam sistem.\n");

        if (*n >= MAX_PRODUCTS) {
            printf("Tidak boleh menambah produk baru. Had maksimum telah dicapai.\n");
            return;
        }

        printf("Adakah anda ingin mendaftarkan produk baru ini? (Y/N): ");
        scanf(" %c", &choice);

        if (choice == 'Y' || choice == 'y') {
            strcpy(codes[*n], code);
            inputValidName(names[*n]);
            price = inputValidPrice();
            quantity = inputValidQuantity();

            prices[*n] = price;
            stock[*n] = quantity;
            (*n)++;

            printf("Produk baru telah didaftarkan dan dimasukkan ke dalam sistem.\n");
        } else {
            printf("Produk tidak didaftarkan. Kembali ke menu utama.\n");
        }
    }
}

float sellProduct(char codes[][CODE_LEN], float prices[], int stock[], float revenue[], int n) {
    char code[CODE_LEN];
    int quantity;

    printf("Masukkan Kod Produk untuk dijual: ");
    scanf("%s", code);

    int found = -1;
    for (int i = 0; i < n; i++) {
        if (strcmp(codes[i], code) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("Produk ini tidak ada dalam senarai\n");
        return 0.0f;
    }

    printf("Masukkan Kuantiti untuk dijual: ");
    scanf("%d", &quantity);

    if (quantity > stock[found]) {
        printf("Tidak mencukupi Stok Produk untuk dijual :  %d\n", stock[found]);
        return 0.0f;
    }

    stock[found] -= quantity;
    float amount = quantity * prices[found];
    revenue[found] += amount;

    printf("Hasil Jualan berjaya direkodkan.\n");
    return amount;
}

void displayProducts(char codes[][CODE_LEN], char names[][NAME_LEN], float prices[], int stock[], int n) {
    printf("\n=== PRODUCT LIST ===\n");
    if (n == 0) {
        printf("Tiada produk dalam senarai.\n");
        return;
    }

    printf("%-10s %-20s %-10s %-10s\n", "Code", "Name", "Price", "Stock");
    for (int i = 0; i < n; i++) {
        printf("%-10s %-20s RM%-9.2f %-10d\n", codes[i], names[i], prices[i], stock[i]);
    }
}

void generateSummary(char names[][NAME_LEN], int stock[], float revenue[], int n) {
    printf("\n=== Laporan Ringkas ===\n");

    printf("\nProduk yang mempunyai stok yang rendah. (<5):\n");
    int foundLowStock = 0;
    for (int i = 0; i < n; i++) {
        if (stock[i] < 5) {
            printf("%s (Stok: %d)\n", names[i], stock[i]);
            foundLowStock = 1;
        }
    }
    if (!foundLowStock) {
        printf("Tiada Produk yang mempunyai stok rendah.\n");
    }

    printf("\nHasil jualan bagi setiap Produk yang ada:\n");
    for (int i = 0; i < n; i++) {
        printf("%s: RM%.2f\n", names[i], revenue[i]);
    }
}

int getValidMenuChoice(void) {
    char input[10];
    int choice;

    printf("Masukkan pilihan anda (1-5): ");
    scanf("%s", input);

    // Semak sama ada semua aksara adalah nombor
    for (int i = 0; i < (int)strlen(input); i++) {
        if (!isdigit(input[i])) {
            printf("Input tidak sah! Sila masukkan nombor sahaja.\n");
            pauseScreen();
            clearScreen();
            return -1; // Return to main menu
        }
    }

    choice = atoi(input);
    if (choice < 1 || choice > 5) {
        printf("Pilihan tidak sah! Sila pilih antara 1 hingga 5.\n");
        pauseScreen();
        clearScreen();
        return -1; // Return to main menu
    }

    return choice;
}

void inputValidCode(char *code) {
    int valid = 0;
    while (!valid) {
        printf("Masukkan Kod Produk (maks 10 huruf/nombor): ");
        scanf("%s", code);

        int length = strlen(code);
        valid = 1;

        if (length > 10) {
            printf("Kod melebihi 10 aksara.\n");
            valid = 0;
        } else {
            for (int i = 0; i < length; i++) {
                if (!isalnum(code[i])) {
                    printf("Kod hanya boleh mengandungi huruf dan nombor sahaja.\n");
                    valid = 0;
                    break;
                }
            }
        }
    }
}

void inputValidName(char *name) {
    int valid = 0;
    while (!valid) {
        printf("Masukkan Nama Produk (maks 50 aksara): ");
        scanf(" %[^\n]", name);
        if (strlen(name) > 50) {
            printf("Nama produk melebihi 50 aksara.\n");
        } else {
            valid = 1;
        }
    }
}

float inputValidPrice() {
    char input[20];
    float price;
    int valid = 0;
    while (!valid) {
        printf("Masukkan Harga Produk (maks RM 999,999): ");
        scanf("%s", input);

        int length = strlen(input);
        valid = 1;
        int dotCount = 0;
        for (int i = 0; i < length; i++) {
            if (!isdigit(input[i])) {
                if (input[i] == '.' && dotCount == 0) {
                    dotCount++;
                } else {
                    printf("Harga hanya boleh mengandungi nombor atau titik perpuluhan.\n");
                    valid = 0;
                    break;
                }
            }
        }
        if (valid) {
            price = atof(input);
            if (price < 0 || price > 999999) {
                printf("Harga mesti antara RM 0 hingga RM 999,999 sahaja.\n");
                valid = 0;
            }
        }
    }
    return price;
}

int inputValidQuantity() {
    int quantity;
    int valid = 0;
    while (!valid) {
        printf("Masukkan Kuantiti Produk (0 - 100): ");
        if (scanf("%d", &quantity) == 1 && quantity >= 0 && quantity <= 100) {
            valid = 1;
        } else {
            printf("Kuantiti mesti antara 0 hingga 100.\n");
            while (getchar() != '\n');
        }
    }
    return quantity;
}
