// ------------------------E-commerce Order Management System------------------------
// --------------------------Header Files and Macros--------------------------
 
#include <stdio.h>     // for input/output (Standard Input/Output library)
#include <string.h>    // for string manipulation functions
#include <stdlib.h>    // for general utilities (like system calls).. mostly used for Data structures
#include <unistd.h>    // for sleep and usleep functions
#include <fcntl.h>     // for file control operations
#include <termios.h>   // for terminal I/O control
#include <sys/ioctl.h> // for terminal size detection
 
// ...................... Define Maximum Orders ......................
 
#define Max_order 100 // Maximum number of orders that can be stored
 
// ---------------------- Define Colors ----------------------
 
#define RESET "\033[0m"    // Reset color
#define CYAN "\033[36m"    // Cyan color (used in loading animation)
#define MAGENTA "\033[35m" // Magenta color (used in header)
#define GREEN "\033[32m"   // Green color (used for success messages)
#define RED "\033[31m"     // Red color (used for error messages)
#define YELLOW "\033[33m"  // Yellow color (used for highlights)
#define BLUE "\033[34m"    // Blue color (used in header)
#define BOLD "\033[1m"     // Bold text
 
// ------------------------ Function priorities --------------
 
void center(char *text);     // Function to center text in terminal
void header();               // Function to display header with ASCII art
int menuSelect();            // Function to handle menu selection
void drawMenu(int selected); // Function to draw the menu
void disable_buffering();    // Function to disable input buffering
void enable_buffering();     // Function to enable input buffering
void loading_animation();    // Function to show loading animation
void addOrder();             // Function to add a new order
void displayOrders();        // Function to display all orders
void searchOrder();          // Function to search for an order
void updateOrder();          // Function to update an existing order
void saveToFile();           // Function to save orders to a file
void loadFromFile();         // Function to load orders from a file
 
// --------------------------- Safe input functions ---------------------
 
void safe_string_input(char *buffer, int max_len, const char *prompt);
int safe_int_input(const char *prompt, int min_val, int max_val);
float safe_float_input(const char *prompt, float min_val, float max_val);
int is_order_id_unique(int id);
int confirm_action(const char *message);
 
// -------------------------- Data structures -------------------
 
typedef struct // Structure to hold order details
{
    int orderID;           // Unique identifier for the order
    char customerName[50]; // Name of the customer (Under 50 characters)
    char productName[50];  // Name of the product (Under 50 characters)
    int quantity;          // the amount or number of the product ordered
    float price;           // Price of the product
    char status[20];       // Status of the order (e.g., "Pending", "Shipped", "Delivered")
} Order;                   // Order structure (Structure's name)
 
Order orders[Max_order]; // Array to store multiple orders
int orderCount = 0;      // Current number of orders stored
 
// -------------------Function to get terminal dimensions-----------------
 
void getTerminalSize(int *rows, int *cols) // Get terminal size (this specific code works only on Unix/Linux/macOS terminals)
{
    struct winsize w;                     // Structure to hold window size
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); // Get window size
    *rows = w.ws_row;                     // Number of rows
    *cols = w.ws_col;                     // Number of columns
}
 
// ----------------Function to vertically center the screen content----------------------
 
void vertical_center_dynamic(int content_lines) // content_lines: number of lines to be printed in the content area (C programs running on Linux, macOS, or any Unix-based terminal. And Console applications that support ANSI and POSIX terminal functions)
{
    int rows, cols;                           // Variables to hold terminal size
    getTerminalSize(&rows, &cols);            // Get terminal size
    int padding = (rows - content_lines) / 2; // Calculate padding needed for vertical centering
    for (int i = 0; i < padding; i++)         // Print padding lines
        printf("\n");                         // New line for vertical spacing
}
 
// ----------------Function to disable input buffering (read keys instantly)----------------------
 
void disable_buffering() // Disable input buffering (this code is POSIX-specific, meaning it only works on: Linux, macOS & Unix terminals)
{
    struct termios t;                     // Structure to hold terminal attributes
    tcgetattr(STDIN_FILENO, &t);          // Get current terminal attributes
    t.c_lflag &= ~(ICANON | ECHO);        // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &t); // Set new terminal attributes immediately
}
 
// ----------------Function to enable input buffering (normal mode)----------------------
 
void enable_buffering() // Enable input buffering (restore normal terminal behavior). It is reusable for Unix-like terminal programs.
{
    struct termios t;                     // Structure to hold terminal attributes
    tcgetattr(STDIN_FILENO, &t);          // Get current terminal attributes
    t.c_lflag |= (ICANON | ECHO);         // Enable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &t); // Set new terminal attributes immediately
}
 
// --------------------------- Center text in terminal ---------------------------
 
void center(char *text) // Function for call in center
{
    int rows, cols;
    getTerminalSize(&rows, &cols);
    int len = strlen(text);
    int spaces = (cols - len) / 2;
    if (spaces < 0)
        spaces = 0;
    for (int i = 0; i < spaces; i++)
        printf(" ");
    printf("%s\n", text);
}
 
// --------------------------- Loading Animation ---------------------------
 
void loading_animation()
{
    system("clear");
    char *bars[] = {
        "[■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 1%",
        "[■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 2%",
        "[■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 3%",
        "[■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 4%",
        "[■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 5%",
        "[■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 6%",
        "[■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 7%",
        "[■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 8%",
        "[■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 9%",
        "[■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 10%",
        "[■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 11%",
        "[■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 12%",
        "[■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 13%",
        "[■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 14%",
        "[■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 15%",
        "[■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 16%",
        "[■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 17%",
        "[■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 18%",
        "[■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 19%",
        "[■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 20%",
        "[■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 21%",
        "[■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 22%",
        "[■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 23%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 24%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 25%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 26%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 27%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 28%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 29%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 30%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 31%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 32%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 33%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 34%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 35%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 36%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 37%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 38%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 39%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 40%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 41%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 42%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 43%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 44%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 45%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 46%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 47%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 48%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 49%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 50%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 51%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 52%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 53%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 54%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 55%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 56%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 57%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 58%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 59%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 60%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 61%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 62%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 63%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 64%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 65%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 66%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 67%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 68%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 69%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 70%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 71%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□□] 72%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□□] 73%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□□] 74%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□□] 75%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□□] 76%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□□] 77%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□□] 78%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□□] 79%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□□] 80%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□□] 81%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□□] 82%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□□] 83%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□□] 84%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□□] 85%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□□] 86%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□□] 87%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□□] 88%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□□] 89%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□□] 90%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□□] 91%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□□] 92%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□□] 93%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□□] 94%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□□] 95%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□□] 96%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□□] 97%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□□] 98%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■□] 99%",
        "[■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■] 100%"};
    char line[1000];
    int total_bars = sizeof(bars) / sizeof(bars[0]);
    for (int i = 0; i < total_bars; i++)
    {
        system("clear");
        center(BOLD YELLOW "Loading System..." RESET);
        snprintf(line, sizeof(line), BOLD CYAN "%s" RESET, bars[i]);
        center(line);
        usleep(50000); //0.05 sec
    }
    sleep(1);
}
 
// --------------------- Draws the menu interface and highlights the currently selected option ---------------------
 
void drawMenu(int selected)
{
    header();     
    printf("\n");
 
    center(YELLOW "( Choose one of the options )" RESET);
 
    printf("\n\n"); 
 
    char *menu[] = {
        // Menu options
        "Add Order",      // Option 1
        "Display Orders", // Option 2
        "Search Order",   // Option 3
        "Update Order",   // Option 4
        "Save & Exit"     // Option 5
    };
 
    printf("\n\n"); 
 
    for (int i = 0; i < 5; i++) 
    {
        char line[100];
 
        if (i == selected) 
            sprintf(line, GREEN BOLD "→ %s" RESET, menu[i]);
        else
            sprintf(line, "  %s", menu[i]);
 
        center(line);
    }
}
 
// --------------------- Menu selection logic using arrow keys ---------------------
 
int menuSelect() //
{
    disable_buffering();
 
    int choice = 0;
    int ch;
 
    drawMenu(choice);
 
    while (1)
    {
        ch = getchar();
        if (ch == 27)
        {
            getchar();
            ch = getchar();
            if (ch == 'A' && choice > 0)
                choice--;
            else if (ch == 'B' && choice < 4)
                choice++;
        }
        else if (ch == '\n')
        {
            enable_buffering();
            return choice;
        }
        drawMenu(choice);
    }
}
 
 
//-------------------- Header with ASCII Art and Title --------------------
 
void header() //
{
    system("clear");
 
    // The ASCII art + title = about 10 printed lines
    vertical_center_dynamic(10);
 
    center(BLUE "⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣶⣶⣤⣀⠀⠀⠀⠀⠀⠀⠀⠀" RESET);
    center(BLUE "⠀⠀⠀⢀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣦⡀⠀⠀⠀⠀⠀⠀" RESET);
    center(BLUE "⠀⣠⣾⣿⣿⡿⠟⠋⠀⠀⠉⠛⢿⣿⣿⣷⡄⠀⠀⠀⠀" RESET);
    center(BLUE "⣰⣿⣿⡿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⣆⠀" RESET);
 
    center(MAGENTA BOLD "🛒  E - C O M M E R C E  O R D E R  S Y S T E M  🛒" RESET);
 
    center(BLUE "⢿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣿⡿" RESET);
    center(BLUE "⠀⠻⣿⣦⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⠟⠀" RESET);
    center(BLUE "⠀⠀⠀⠙⠻⠿⣿⣿⣿⠿⠟⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀" RESET);
 
    printf("\n\n");
}
 
// --------------------------- Add New Order ----------------------------------
 
void addOrder()
{
    header();
    center(BOLD YELLOW "Add New Order\n" RESET);
 
    if (orderCount >= Max_order)
    {
        center(RED "Order Storage Full!" RESET);
        printf("Press ENTER to continue...");
        getchar();
        return;
    }
 
    Order o;
 
    // Get unique order ID
    do
    {
        o.orderID = safe_int_input("Enter Order ID (1-999999): ", 1, 999999);
        if (!is_order_id_unique(o.orderID))
            printf(RED "Order ID already exists. Please choose a different ID.\n" RESET);
    } while (!is_order_id_unique(o.orderID));
 
    safe_string_input(o.customerName, sizeof(o.customerName), "Enter Customer Name: ");
    if (strlen(o.customerName) == 0)
        strcpy(o.customerName, "Unknown");
 
    safe_string_input(o.productName, sizeof(o.productName), "Enter Product Name: ");
    if (strlen(o.productName) == 0)
        strcpy(o.productName, "Unknown");
 
    o.quantity = safe_int_input("Enter Quantity (1-10000): ", 1, 10000);
 
    o.price = safe_float_input("Enter Price (0.01-999999.99): ", 0.01f, 999999.99f);
 
    safe_string_input(o.status, sizeof(o.status), "Enter Status: ");
    if (strlen(o.status) == 0)
        strcpy(o.status, "Pending");
 
    orders[orderCount++] = o;
 
    center(GREEN "\nOrder Added Successfully!" RESET);
    printf("Press ENTER to continue...");
    getchar();
}
 
// ------------------------------------ Display Orders -----------------------------
 
void displayOrders()
{
    header();
    center(BOLD YELLOW "All Orders\n" RESET);
 
    if (orderCount == 0)
    {
        center(RED "No Orders Available!" RESET);
        printf("Press ENTER to continue...");
        getchar();
        return;
    }
 
    for (int i = 0; i < orderCount; i++)
    {
        printf("\n");
        // Calculate left margin for centering the block
        int rows, cols;
        getTerminalSize(&rows, &cols);
        int block_width = 50; // Approximate width of the display block
        int left_margin = (cols - block_width) / 2;
        if (left_margin < 0)
            left_margin = 0;
 
        // Print each field with label left-aligned, value right-aligned
        printf("%*s%-12s : %d\n", left_margin, "", "Order ID", orders[i].orderID);
        printf("%*s%-12s : %s\n", left_margin, "", "Customer", orders[i].customerName);
        printf("%*s%-12s : %s\n", left_margin, "", "Product", orders[i].productName);
        printf("%*s%-12s : %d\n", left_margin, "", "Quantity", orders[i].quantity);
        printf("%*s%-12s : $%.2f\n", left_margin, "", "Price", orders[i].price);
        printf("%*s%-12s : %s\n", left_margin, "", "Status", orders[i].status);
 
        // Separator
        printf("%*s", left_margin, "");
        center(CYAN "--------------------------------------" RESET);
    }
 
    printf("\n");
    center(YELLOW "Press ENTER to go back..." RESET);
    getchar();
}
 
// ------------------------------------ Search Order -----------------------------
void searchOrder()
{
    header();
    center(BOLD YELLOW "Search Order\n" RESET);
 
    int id = safe_int_input("Enter Order ID to search: ", 1, 999999);
 
    for (int i = 0; i < orderCount; i++)
    {
        if (orders[i].orderID == id)
        {
            center(BOLD CYAN "------------------------------------------" RESET);
            center(GREEN BOLD "ORDER FOUND!\n" RESET);
 
            char temp[200]; // Increased buffer size
 
            snprintf(temp, sizeof(temp), "Customer : %s", orders[i].customerName);
            center(temp);
 
            snprintf(temp, sizeof(temp), "Product : %s", orders[i].productName);
            center(temp);
 
            snprintf(temp, sizeof(temp), "Quantity : %d", orders[i].quantity);
            center(temp);
 
            snprintf(temp, sizeof(temp), "Price : $%.2f", orders[i].price);
            center(temp);
 
            snprintf(temp, sizeof(temp), "Status : %s", orders[i].status);
            center(temp);
 
            printf("Press ENTER to continue...");
            getchar();
            return;
        }
    }
 
    center(RED "Order Not Found!" RESET);
    printf("Press ENTER to continue...");
    getchar();
}
 
// --------------------------- Update Order-------------------------------------
 
void updateOrder()
{
    header();
    center(BOLD YELLOW "Update Order\n" RESET);
 
    int id = safe_int_input("Enter Order ID to update: ", 1, 999999);
 
    for (int i = 0; i < orderCount; i++)
    {
        if (orders[i].orderID == id)
        {
            if (!confirm_action("Are you sure you want to update this order"))
                return;
 
            safe_string_input(orders[i].customerName, sizeof(orders[i].customerName), "New Customer Name: ");
            if (strlen(orders[i].customerName) == 0)
                strcpy(orders[i].customerName, "Unknown");
 
            safe_string_input(orders[i].productName, sizeof(orders[i].productName), "New Product Name: ");
            if (strlen(orders[i].productName) == 0)
                strcpy(orders[i].productName, "Unknown");
 
            orders[i].quantity = safe_int_input("New Quantity (1-10000): ", 1, 10000);
 
            orders[i].price = safe_float_input("New Price (0.01-999999.99): ", 0.01f, 999999.99f);
 
            safe_string_input(orders[i].status, sizeof(orders[i].status), "New Status: ");
            if (strlen(orders[i].status) == 0)
                strcpy(orders[i].status, "Pending");
 
            center(GREEN "\nOrder Updated Successfully!" RESET);
            printf("Press ENTER to continue...");
            getchar();
            return;
        }
    }
 
    center(RED "Order Not Found!" RESET);
    printf("Press ENTER to continue...");
    getchar();
}
 
//------------------------------- Save + Load ----------------------------
 
void saveToFile()
{
    FILE *f = fopen("orders.txt", "w");
    if (!f)
    {
        center(RED "Error: Cannot open file for saving!" RESET);
        printf("Press ENTER to continue...");
        getchar();
        return;
    }
 
    for (int i = 0; i < orderCount; i++)
    {
        fprintf(f, "%d|%s|%s|%d|%.2f|%s\n",
                orders[i].orderID, orders[i].customerName,
                orders[i].productName, orders[i].quantity,
                orders[i].price, orders[i].status);
    }
    fclose(f);
}
 
void loadFromFile()
{
    FILE *f = fopen("orders.txt", "r");
    if (!f)
        return;
 
    while (fscanf(f, "%d|%[^|]|%[^|]|%d|%f|%[^\n]\n",
                  &orders[orderCount].orderID,
                  orders[orderCount].customerName,
                  orders[orderCount].productName,
                  &orders[orderCount].quantity,
                  &orders[orderCount].price,
                  orders[orderCount].status) == 6)
    {
        orderCount++;
        if (orderCount >= Max_order)
            break;
    }
    fclose(f);
}
 
// --------------------------- Safe Input Functions ---------------------------
 
void safe_string_input(char *buffer, int max_len, const char *prompt)
{
    printf("%s", prompt);
    fgets(buffer, max_len, stdin);
    buffer[strcspn(buffer, "\n")] = 0; // Remove newline
}
 
int safe_int_input(const char *prompt, int min_val, int max_val)
{
    int value;
    char input[100];
    while (1)
    {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL)
            return min_val;
        if (sscanf(input, "%d", &value) == 1 && value >= min_val && value <= max_val)
            return value;
        printf(RED "Invalid input. Please enter a number between %d and %d.\n" RESET, min_val, max_val);
    }
}
 
float safe_float_input(const char *prompt, float min_val, float max_val)
{
    float value;
    char input[100];
    while (1)
    {
        printf("%s", prompt);
        if (fgets(input, sizeof(input), stdin) == NULL)
            return min_val;
        if (sscanf(input, "%f", &value) == 1 && value >= min_val && value <= max_val)
            return value;
        printf(RED "Invalid input. Please enter a number between %.2f and %.2f.\n" RESET, min_val, max_val);
    }
}
 
int is_order_id_unique(int id)
{
    for (int i = 0; i < orderCount; i++)
    {
        if (orders[i].orderID == id)
            return 0; // Not unique
    }
    return 1; // Unique
}
 
int confirm_action(const char *message)
{
    char input[10];
    printf("%s (y/n): ", message);
    fgets(input, sizeof(input), stdin);
    return (input[0] == 'y' || input[0] == 'Y');
}
 
// --------------------------- Main Function ---------------------------
 
int main()
{
    loading_animation();
    loadFromFile();
 
    int choice;
 
    while (1)
    {
 
        choice = menuSelect();
        switch (choice)
        {
        case 0:
            addOrder();
            break;
        case 1:
            displayOrders();
            break;
        case 2:
            searchOrder();
            break;
        case 3:
            updateOrder();
            break;
        case 4:
            if (confirm_action("Save all orders and exit"))
            {
                saveToFile();
                header();
                center(GREEN "Saved Successfully. Exiting..." RESET);
                sleep(2);
                return 0;
            }
            break;
        }
    }
}