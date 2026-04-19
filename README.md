===================================================================
     MULTI-BRANCH BANKING MANAGEMENT SYSTEM - USER GUIDE
===================================================================

                    STANDARD BANK PROTOTYPE SYSTEM
                    Developed for IT Staff Training
===================================================================

TABLE OF CONTENTS
-----------------
1. SYSTEM REQUIREMENTS
2. HOW TO COMPILE AND RUN
3. FIRST TIME SETUP
4. DEFAULT TELLER LOGIN (FOR TESTING)
5. HOW TELLERS USE THE SYSTEM
6. HOW CUSTOMERS USE THE SYSTEM
7. FILE STRUCTURE
8. TROUBLESHOOTING

===================================================================
1. SYSTEM REQUIREMENTS
===================================================================

- Operating System: Windows 10/11, Linux, or macOS
- Compiler: Any C++17 compatible compiler (g++, clang++, MSVC)
- Disk Space: Approximately 5MB for program and data files
- RAM: 256MB minimum

===================================================================
2. HOW TO COMPILE AND RUN
===================================================================

2.1 USING GCC/LINUX/MAC (Terminal):

    Step 1: Open terminal in the folder containing bank.cpp
    
    Step 2: Compile the program:
    
        g++ bank.cpp -o banking_system
        
        (If you get errors about random_device, try:)
        g++ bank.cpp -o banking_system -std=c++11
        
    Step 3: Run the program:
    
        ./banking_system
        
        (On Linux/Mac)
        
        or
        
        banking_system.exe
        
        (On Windows after compilation)

2.2 USING VISUAL STUDIO CODE:

    Step 1: Open the folder containing bank.cpp
    
    Step 2: Open terminal (Ctrl + `)
    
    Step 3: Type: g++ bank.cpp -o bank
    
    Step 4: Type: ./bank (Mac/Linux) or .\bank.exe (Windows)

2.3 USING DEV-C++:

    Step 1: Open Dev-C++
    
    Step 2: File -> New -> Source File
    
    Step 3: Copy all code into the editor
    
    Step 4: File -> Save As -> bank.cpp
    
    Step 5: Execute -> Compile & Run (F11)

2.4 USING CODE::BLOCKS:

    Step 1: Create new console project
    
    Step 2: Replace main.cpp with bank.cpp code
    
    Step 3: Build -> Build and Run (F9)

===================================================================
3. FIRST TIME SETUP
===================================================================

When you run the program for the FIRST TIME:

- The system automatically creates:
  * system_config.txt (settings file)
  * branches.dat (3 branches: JHB, CPT, DBN)
  * All other .dat files as needed

- NO teller accounts exist initially. You MUST create the first teller:

    Option 1 from main menu: "Create Teller Account"
    
    Enter:
    - Teller ID: (e.g., T001)
    - Full Name: (e.g., John Smith)
    - Password: (choose something, e.g., password123)
    - Branch Code: (B001, B002, or B003)

    Available Branch Codes:
    B001 = Johannesburg Main
    B002 = Cape Town Branch  
    B003 = Durban Branch

===================================================================
4. DEFAULT TELLER LOGIN CREDENTIALS (FOR TESTING)
===================================================================

Option A - Create your own teller (recommended):
-----------------------------------------------
    Use Option 1 from main menu to create a teller account
    with your own ID and password


Option B - If you want pre-made tellers for testing:
----------------------------------------------------
    The system does NOT come with default tellers for security.
    
    But for QUICK TESTING, create these three tellers:
    
    TELLER 1 (Johannesburg):
    -------------------------
    Teller ID: T001
    Password: jhb123
    Branch Code: B001
    
    TELLER 2 (Cape Town):
    ---------------------
    Teller ID: T002
    Password: cpt456
    Branch Code: B002
    
    TELLER 3 (Durban):
    ------------------
    Teller ID: T003
    Password: dbn789
    Branch Code: B003


IMPORTANT NOTE:
---------------
The system uses HASHED passwords (encrypted). Your actual password
is NOT stored - only the hash. This means even if someone gets
the tellers.dat file, they cannot read your password.

===================================================================
5. HOW TELLERS USE THE SYSTEM
===================================================================

5.1 LOGGING IN AS TELLER:

    Main Menu -> Option 2 "Login As Teller"
    
    Enter:
    - ID Number: (e.g., T001)
    - Password: (the password you created)
    
    If successful, you'll see: "Login Successful! Branch: B001"
    
    Then you enter the TELLER MENU.

5.2 TELLER MENU OPTIONS:

    OPTION 1: Register New Customer
    ---------------------------------
    - Select account type (1-4)
    - Enter customer details:
        * Full Name
        * SA ID (13 digits)
        * Contact (10 digits)
        * Email (must have @ and .)
        * Physical Address
        * Date of Birth (DD/MM/YYYY)
        * Initial Deposit (minimum depends on account type)
    
    - System AUTO-GENERATES:
        * Account Number (format: ACC-B001-1001)
        * 5-digit PIN (e.g., 12345)
    
    - IMPORTANT: The PIN is displayed ONLY ONCE on screen!
      Write it down immediately and give to customer.
      It cannot be retrieved later (only reset by teller).

    OPTION 2: Process Customer Transaction
    ---------------------------------------
    - Enter customer's account number
    - Enter customer's PIN (for verification)
    - Choose Deposit or Withdrawal
    - Enter amount
    
    OPTION 3: Search Customer
    -------------------------
    - Search by account number OR name
    - Shows account details and balance

    OPTION 4: View Branch Details
    ------------------------------
    - Shows all branches with their:
        * Total balance
        * Number of customers
        * Average per customer

    OPTION 5: Generate Reports
    --------------------------
    Sub-options:
        1. Daily Transaction Report
        2. Customer Summary Report  
        3. Branch Performance Report

    OPTION 6: Logout
    ----------------
    Returns to main menu

===================================================================
6. HOW CUSTOMERS USE THE SYSTEM
===================================================================

6.1 CUSTOMER LOGIN:

    Main Menu -> Option 3 "Customer Login"
    
    Enter:
    - Account Number: (e.g., ACC-B001-1001)
    - PIN: (5-digit PIN given by teller)
    
    If you enter wrong PIN 3 times, account gets LOCKED!
    (Teller must unlock it - contact branch manager)

6.2 CUSTOMER MENU OPTIONS:

    OPTION 1: View Balance
    ----------------------
    Shows current account balance
    
    OPTION 2: Deposit
    -----------------
    - Enter amount to deposit
    - System updates balance
    - Transaction is logged

    OPTION 3: Withdraw
    ------------------
    - Enter amount to withdraw
    - Cannot withdraw more than balance
    - System updates balance

    OPTION 4: Transfer
    ------------------
    - Enter destination account number
    - Enter amount to transfer
    - Money moves between accounts
    - Both accounts updated

    OPTION 5: View Statement
    ------------------------
    Shows last 10 transactions with:
        * Date and time
        * Transaction type
        * Amount
        * Balance after transaction

    OPTION 6: Change PIN
    --------------------
    - Enter current PIN
    - Enter new PIN (must be 5 digits)
    - Confirm new PIN
    - PIN is encrypted before storing

    OPTION 7: Logout
    ----------------
    Returns to main menu

===================================================================
7. HOW TO VIEW GENERATED PINS (Important!)
===================================================================

FOR TELLERS:
------------
When you register a customer, the PIN appears ONCE on screen:

    "Generated PIN: 12345"
    "PLEASE SAVE THIS PIN - SHOWN ONLY ONCE!"

The PIN is NOT stored in plain text anywhere. It is ENCRYPTED
using XOR encryption before being saved to customers.dat.

If a customer forgets their PIN:
--------------------------------
- They cannot retrieve it (security feature)
- Teller must create a NEW account for them
- Or implement a "reset PIN" feature (advanced)

The system deliberately does NOT allow PIN retrieval to maintain
security standards required by banks.

===================================================================
8. FILE STRUCTURE (What gets created)
===================================================================

When you run the program, these files are created:

    customers.dat      - Binary file with customer accounts
                         (PINs are encrypted using XOR)
    
    transactions.dat   - Binary file with all transactions
                         (deposits, withdrawals, transfers)
    
    branches.dat       - Binary file with 3 branches
                         (B001=JHB, B002=CPT, B003=DBN)
    
    tellers.dat        - Binary file with teller credentials
                         (passwords are hashed, not stored plain)
    
    system_config.txt  - Text file with settings
                         (max attempts, interest rates, etc.)

BACKUP FILES (created when you use backup option):
---------------------------------------------------
    customers_backup.dat
    transactions_backup.dat
    branches_backup.dat
    tellers_backup.dat

EXPORT FILES (created when you export):
---------------------------------------
    bank_export.csv    - Customer data in CSV format
                         (can open in Excel)

===================================================================
9. INTEREST CALCULATION (For Savings and Fixed Deposit)
===================================================================

The system automatically calculates interest:

    SAVINGS ACCOUNTS:
    -----------------
    Interest Rate: 2.5% per annum
    Applied when teller selects "Apply Interest" from main menu
    
    FIXED DEPOSIT ACCOUNTS:
    -----------------------
    Interest Rate: 5.0% per annum
    Applied when teller selects "Apply Interest" from main menu
    
    CHEQUE and STUDENT ACCOUNTS:
    ----------------------------
    No interest earned

TO APPLY INTEREST:
    Main Menu -> Option 9 "Apply Interest"
    System loops through all accounts and adds interest

===================================================================
10. TROUBLESHOOTING
===================================================================

PROBLEM: "No tellers found" error
---------------------------------
SOLUTION: Create a teller account first (Main Menu -> Option 1)

PROBLEM: "Account is LOCKED" message
------------------------------------
SOLUTION: Too many failed PIN attempts.
         Contact branch manager or create new account.

PROBLEM: Cannot compile - "random_device" errors
------------------------------------------------
SOLUTION: Use -std=c++11 flag:
         g++ bank.cpp -o bank -std=c++11

PROBLEM: Cannot find .dat files
--------------------------------
SOLUTION: The system creates them automatically on first run.
         Make sure you have write permissions in the folder.

PROBLEM: Program crashes when reading files
-------------------------------------------
SOLUTION: Delete all .dat files and let the system recreate them.
         (Backup first if needed using Option 11)

PROBLEM: Customer forgot PIN
----------------------------
SOLUTION: The system cannot retrieve encrypted PINs for security.
         Create a new account for the customer.

PROBLEM: "Invalid Id or Password" even though correct
------------------------------------------------------
SOLUTION: Passwords are case-sensitive.
         Make sure Caps Lock is off.
         Try creating a new teller account.

===================================================================
11. QUICK TESTING SCENARIO (For Trainers)
===================================================================

SCENARIO 1: Complete Customer Registration
-------------------------------------------
1. Run program
2. Create teller (T001, password123, B001)
3. Login as teller
4. Register customer:
   - Savings account
   - Name: Thabo Nkosi
   - SA ID: 8001015009087 (13 digits)
   - Contact: 0821234567 (10 digits)
   - Email: thabo@email.com
   - Address: 123 Street, JHB
   - DOB: 01/01/1980
   - Deposit: R500
5. Note the generated PIN
6. Logout teller

SCENARIO 2: Customer Transactions
---------------------------------
1. Customer login with account number and PIN
2. Check balance
3. Deposit R200
4. Withdraw R50
5. View statement
6. Change PIN to new number
7. Logout

SCENARIO 3: Fund Transfer
-------------------------
1. Create SECOND customer (same process)
2. Login as FIRST customer
3. Transfer R100 to second customer
4. Verify balance decreased
5. Login as SECOND customer
6. Verify balance increased

SCENARIO 4: Lockout Test
------------------------
1. Customer login
2. Enter wrong PIN 3 times
3. Account should lock
4. Try login again - should see "Account is LOCKED"

===================================================================
12. MAIN MENU QUICK REFERENCE
===================================================================

    Option | Function
    --------|--------------------------------------------------
    1      | Create Teller Account
    2      | Login As Teller
    3      | Customer Login
    4      | View All Branches
    5      | Inter-Branch Comparison
    6      | Branch Performance Report
    7      | Daily Transaction Report
    8      | Customer Summary Report
    9      | Apply Interest
    10     | Search Customer
    11     | Backup Data
    12     | Recover Data
    13     | Export to CSV
    14     | Exit

===================================================================
13. SECURITY NOTES
===================================================================

This system implements banking-grade security features:

1. PASSWORDS: Stored as HASHES (not plain text)
   - Uses std::hash with salt "XyZ123"
   - Cannot reverse-engineer original password

2. PINS: Stored ENCRYPTED (XOR cipher with key 0x55)
   - Not stored as plain text
   - Simple encryption suitable for training

3. LOCKOUT: 3 failed attempts = temporary lock
   - Prevents brute force attacks
   - Teller must assist to unlock

4. BINARY FILES: All .dat files are binary
   - Not human-readable like text files
   - Basic protection against casual viewing

NOTE: This is a TRAINING PROTOTYPE, not production banking software.
      For production, use industry-standard encryption (AES, etc.).

===================================================================
14. CONTACT & SUPPORT
===================================================================

For training purposes only - this is a prototype system.

Developed for: Standard Bank IT Training Department

===================================================================
                                END OF GUIDE
===================================================================
