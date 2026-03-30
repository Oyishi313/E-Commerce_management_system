![Header](https://capsule-render.vercel.app/api?type=waving&color=0:0d1117,50:1a0f2e,100:2d1560&height=200&section=header&text=E-Commerce+Order+System&fontSize=48&fontColor=ffffff&fontAlignY=38&desc=A+terminal-based+e-commerce+order+manager+built+in+C&descAlignY=60&descSize=14&descColor=94a3b8)

[![C](https://img.shields.io/badge/Language-C-00599C?style=flat-square&logo=c&logoColor=white)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Platform](https://img.shields.io/badge/Platform-Linux-FCC624?style=flat-square&logo=linux&logoColor=black)](https://www.linux.org/)
[![Compiler](https://img.shields.io/badge/Compiler-GCC-f97316?style=flat-square)](https://gcc.gnu.org/)
[![Editor](https://img.shields.io/badge/Editor-VS%20Code-007ACC?style=flat-square&logo=visual-studio-code&logoColor=white)](https://code.visualstudio.com/)
[![License](https://img.shields.io/badge/License-MIT-22c55e?style=flat-square)](LICENSE)
[![File Storage](https://img.shields.io/badge/Storage-orders.txt-6b21a8?style=flat-square)](https://github.com/Oyishi313/E-Commerce_management_system/blob/main)

[![View Source](https://img.shields.io/badge/-%F0%9F%92%BB%20%20VIEW%20SOURCE%20%20%E2%86%92-1a0f2e?style=for-the-badge&logoColor=white)](https://github.com/Oyishi313/E-Commerce_management_system/blob/main/E-Commerce_management_system.c)

✦ Click above to view the full source code on GitHub ✦

---

### 🛒 Features

| ➕ Add Order | 📋 Display All | 🔍 Search | ✏️ Update | 💾 Save to File | 🎨 Animated UI |
| --- | --- | --- | --- | --- | --- |
| Add orders with ID, customer, product, quantity, price & status | Display all orders in a clean centered layout | Search any order instantly by Order ID | Update any existing order's details | Auto-saves all data to orders.txt on exit | Loading bar + arrow-key menu navigation |

---

## 🖥️ Terminal Preview

```
Loading System...
[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■] 100%

        🛒 E - C O M M E R C E   O R D E R   S Y S T E M 🛒

        ( Choose one of the options )

        → Add Order
          Display Orders
          Search Order
          Update Order
          Save & Exit
```

```
Enter Order ID (1-999999): 1001
Enter Customer Name: John Doe
Enter Product Name: Wireless Headphones
Enter Quantity (1-10000): 2
Enter Price (0.01-999999.99): 59.99
Enter Status: Pending

          ✅ Order Added Successfully!
```

```
Order ID     : 1001
Customer     : John Doe
Product      : Wireless Headphones
Quantity     : 2
Price        : $59.99
Status       : Pending
          --------------------------------------
```

---

## 🧠 How It Works

### Struct-based Order Records

```c
typedef struct {
    int   orderID;           // Unique identifier for each order
    char  customerName[50];  // Name of the customer
    char  productName[50];   // Name of the product
    int   quantity;          // Number of units ordered
    float price;             // Unit price of the product
    char  status[20];        // e.g., "Pending", "Shipped", "Delivered"
} Order;

Order orders[100];  // Array to store up to 100 orders
int   orderCount = 0;
```

### Arrow-Key Animated Menu

```c
int menuSelect() {
    disable_buffering();  // Read keys instantly (no Enter needed)
    int choice = 0;
    int ch;
    drawMenu(choice);

    while (1) {
        ch = getchar();
        if (ch == 27) {          // ESC sequence (arrow key)
            getchar();           // skip [
            ch = getchar();
            if (ch == 'A' && choice > 0) choice--;   // Up arrow
            else if (ch == 'B' && choice < 4) choice++;  // Down arrow
        } else if (ch == '\n') {
            enable_buffering();
            return choice;
        }
        drawMenu(choice);
    }
}
```

### Add Order with Validation

```c
void addOrder() {
    Order o;

    // Enforce unique Order ID
    do {
        o.orderID = safe_int_input("Enter Order ID (1-999999): ", 1, 999999);
        if (!is_order_id_unique(o.orderID))
            printf("Order ID already exists. Please choose a different ID.\n");
    } while (!is_order_id_unique(o.orderID));

    safe_string_input(o.customerName, sizeof(o.customerName), "Enter Customer Name: ");
    safe_string_input(o.productName,  sizeof(o.productName),  "Enter Product Name: ");
    o.quantity = safe_int_input("Enter Quantity (1-10000): ", 1, 10000);
    o.price    = safe_float_input("Enter Price (0.01-999999.99): ", 0.01f, 999999.99f);
    safe_string_input(o.status, sizeof(o.status), "Enter Status: ");

    orders[orderCount++] = o;
}
```

### Save to File and Load on Startup

```c
// Save all orders to orders.txt (pipe-delimited)
void saveToFile() {
    FILE *f = fopen("orders.txt", "w");
    for (int i = 0; i < orderCount; i++) {
        fprintf(f, "%d|%s|%s|%d|%.2f|%s\n",
            orders[i].orderID, orders[i].customerName,
            orders[i].productName, orders[i].quantity,
            orders[i].price, orders[i].status);
    }
    fclose(f);
}

// Auto-load existing orders on startup
void loadFromFile() {
    FILE *f = fopen("orders.txt", "r");
    if (!f) return;
    while (fscanf(f, "%d|%[^|]|%[^|]|%d|%f|%[^\n]\n",
        &orders[orderCount].orderID,
        orders[orderCount].customerName,
        orders[orderCount].productName,
        &orders[orderCount].quantity,
        &orders[orderCount].price,
        orders[orderCount].status) == 6) {
        orderCount++;
    }
    fclose(f);
}
```

---

## 🛠️ Compile and Run

### Prerequisites

- Linux OS
- GCC installed

```bash
# Check GCC
gcc --version

# Install if needed (Ubuntu/Debian)
sudo apt install gcc
```

### Compile

```bash
gcc -o ecommerce E-Commerce_management_system.c
```

### Run

```bash
./ecommerce
```

### One-liner

```bash
gcc -o ecommerce E-Commerce_management_system.c && ./ecommerce
```

> ⚠️ Orders are saved to `orders.txt` in the same directory as the executable. Load is automatic on next run.

---

## 📁 Project Structure

```
E-Commerce_management_system/
│
├── 📄 E-Commerce_management_system.c   ← Full source code (651 lines)
├── 📄 orders.txt                       ← Auto-generated on Save & Exit
└── 📄 README.md                        ← Project documentation
```

---

## 🛠️ Tech Stack

```
┌──────────────────────────────────────────────────────┐
│           C Terminal Console Application             │
├─────────────────┬────────────────────────────────────┤
│  Language       │  C (C99 standard)                  │
│  Compiler       │  GCC (Linux)                       │
│  Editor         │  VS Code                           │
│  Libraries      │  stdio.h · string.h · stdlib.h     │
│                 │  unistd.h · fcntl.h · termios.h    │
│                 │  sys/ioctl.h                       │
│  Storage        │  Text file (orders.txt)            │
│  Platform       │  Linux / macOS (Unix-based)        │
└─────────────────┴────────────────────────────────────┘
```

---

## 📚 Key Concepts Used

```
struct / typedef    → Custom Order data type
termios             → Disable/enable terminal buffering for arrow-key input
ioctl / TIOCGWINSZ  → Detect terminal size for dynamic centering
ANSI escape codes   → Colors (RED, GREEN, CYAN, MAGENTA...) & bold text
fopen/fclose        → Open and close files for read/write
fprintf/fscanf      → Pipe-delimited file I/O
usleep / sleep      → Loading animation timing
linear search       → Find order by ID in O(n)
safe input funcs    → Validated int, float, and string input
```

---

## 👤 Developer

**Oyishi**
C Developer

[![GitHub](https://img.shields.io/badge/GitHub-Oyishi313-181717?style=for-the-badge&logo=github&logoColor=white)](https://github.com/Oyishi313)
[![Source Code](https://img.shields.io/badge/Source%20Code-View%20File-1a0f2e?style=for-the-badge&logo=github&logoColor=white)](https://github.com/Oyishi313/E-Commerce_management_system/blob/main/E-Commerce_management_system.c)

*"Every order — added, searched, updated, and never lost."* 🛒

---

![Footer](https://capsule-render.vercel.app/api?type=waving&color=0:2d1560,50:1a0f2e,100:0d1117&height=120&section=footer)

MIT License · © 2025 Oyishi · ⭐ Star this repo if it helped you!
