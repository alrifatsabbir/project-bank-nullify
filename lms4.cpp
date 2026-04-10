// Include all standard C++ libraries for comprehensive functionality
#include <bits/stdc++.h>
// ifdef block to include Windows-specific headers for enabling ANSI color support
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

// Defines maximum storage limits for books, members, borrow records, and staff accounts. #limit
const int MAX_BOOKS = 100000;
const int MAX_MEMBERS = 10000;
const int MAX_BORROWED = 10;
const int MAX_STAFF = 10;

// Declares ANSI escape codes used for styled terminal output. #color-code
const string C_RESET = "\033[0m";
const string C_BOLD = "\033[1m";
const string C_RED = "\033[31m";
const string C_GREEN = "\033[32m";
const string C_YELLOW = "\033[33m";
const string C_BLUE = "\033[34m";
const string C_MAGENTA = "\033[35m";
const string C_CYAN = "\033[36m";

// Enables virtual terminal processing on Windows so ANSI colors render correctly in legacy terminals. #special-function
void enableVirtualTerminalColors() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) {
        return;
    }
    DWORD mode = 0;
    if (!GetConsoleMode(hOut, &mode)) {
        return;
    }
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, mode);
#endif
}

/*
    Library Management System (LMS) - C++ Implementation (OOP Based)
    Class Design: Book, Member, Staff, Admin, Library
    Features: Add/List Books, Add/List Members, Issue/Return Books, Authentication, Membership Packages
    Class: LibraryEntity (Base), Book, Member, Staff, Admin, Library (Controller)
    Inheritance: Single (Book/Member/Staff/Admin -> LibraryEntity), Hierarchical (LibraryEntity -> Book/Member/Staff/Admin), Multilevel (Admin -> Staff -> Member)
    Polymorphism: Virtual functions for display and role-based behavior
    Encapsulation: Private data members with public getters/setters, and protected members for inheritance
    Code Organization: Clear separation of concerns, modular functions, and consistent naming conventions
*/

// Defines the base entity abstraction shared by all domain objects. #BaseClass
class LibraryEntity {
    // Access specifier for derived classes to access common attributes. #encapsulation
protected:
    int id;
    string name;
    // Access specifier for public interface of all entities. #encapsulation
public:
    // Default constructor initializes neutral values for base attributes. #constructor
    LibraryEntity() {
        id = 0;
        name = "";
    }
    // Sets base attributes for derived classes to reuse. #class
    void setBase(int newId, const string &newName) {
        id = newId;
        name = newName;
    }
    int getId() const { 
        return id; 
    }
    string getName() const { 
        return name; 
    }

    // Provides a virtual display contract for runtime polymorphism. #polymorphism
    virtual void displayInfo() const {
        cout << "ID: " << id << " | Name: " << name;
    }

    // Ensures safe object destruction through base-class pointers. #polymorphism
    virtual ~LibraryEntity() = default;
};

// Models a library book and its circulation state. #inheritance-single #inheritance-hierarchical #derived-class(1)
class Book : public LibraryEntity {
    // Access specifier for book-specific attributes to maintain encapsulation. #encapsulation
private:
    string author;
    bool issued;
    int issuedToMemberId;
    // Access specifier for public interface of book operations. #encapsulation
public:
    // Default constructor sets up a book with no title, author, and not issued. #constructor 
    Book() {
        setBase(0, "");
        author = "";
        issued = false;
        issuedToMemberId = -1;
    }

    // Assigns complete book metadata and resets issue tracking. #AddBook-function
    void setBook(int newId, const string &newTitle, const string &newAuthor) {
        setBase(newId, newTitle);
        author = newAuthor;
        issued = false;
        issuedToMemberId = -1;
    }

    string getTitle() const { return name; }
    string getAuthor() const { return author; }
    bool isIssued() const { return issued; }
    int getIssuedToMemberId() const { return issuedToMemberId; }

    // Marks the book as issued to a specific member. #IssueBook-function
    void issueTo(int memberId) {
        issued = true;
        issuedToMemberId = memberId;
    }

    // Resets issue state when a book is returned. #ReturnBook-function
    void markReturned() {
        issued = false;
        issuedToMemberId = -1;
    }
};

// Models a library member with membership and borrowing details. #inheritance-single #inheritance-hierarchical
class Member : public LibraryEntity {
    // Access specifier for member-specific attributes to maintain encapsulation. #encapsulation
private:
    // Stores borrowed book identifiers in a bounded array. #encapsulation
    int borrowedBookIds[MAX_BORROWED];
    int borrowedCount;
    // Stores membership category and duration metadata. #encapsulation
    string membershipCategory;
    int membershipDurationDays;

public:
    // Default constructor initializes a member with no name, no borrowed books, and a default membership package. #constructor
    Member() {
        setBase(0, "");
        borrowedCount = 0;
        membershipCategory = "3 Months";
        membershipDurationDays = 90;
        for (int i = 0; i < MAX_BORROWED; i++) {
            borrowedBookIds[i] = 0;
        }
    }

    // Updates member identity and selected membership package. #SetMember-function
    void setMember(int newId, const string &newName, const string &category, int durationDays) {
        setBase(newId, newName);
        borrowedCount = 0;
        membershipCategory = category;
        membershipDurationDays = durationDays;
        for (int i = 0; i < MAX_BORROWED; i++) {
            borrowedBookIds[i] = 0;
        }
    }

    int getBorrowedCount() const { return borrowedCount; }
    int getBorrowedBookIdAt(int index) const { return borrowedBookIds[index]; }
    string getMembershipCategory() const { return membershipCategory; }
    int getMembershipDurationDays() const { return membershipDurationDays; }

    // Appends a borrowed book ID when borrowing limits allow.  #SetBorrow-function
    bool borrowBook(int bookId) {
        if (borrowedCount >= MAX_BORROWED) {
            return false;
        }
        borrowedBookIds[borrowedCount] = bookId;
        borrowedCount++;
        return true;
    }

    // Removes a returned book ID and compacts internal storage. #ReturnBook-function
    bool returnBook(int bookId) {
        int position = -1;
        for (int i = 0; i < borrowedCount; i++) {
            if (borrowedBookIds[i] == bookId) {
                position = i;
                break;
            }
        }

        if (position == -1) {
            return false;
        }


        for (int i = position; i < borrowedCount - 1; i++) {
            borrowedBookIds[i] = borrowedBookIds[i + 1];
        }
        borrowedBookIds[borrowedCount - 1] = 0;
        borrowedCount--;
        return true;
    }
};

// Models staff profile information and authentication credentials. #inheritance-single #inheritance-hierarchical
class Staff : public LibraryEntity {
    // Access specifier for staff-specific attributes to maintain encapsulation. #encapsulation
private:
    string role;
    string username;
    string password;
    // Access specifier for public interface of staff operations. #encapsulation
public:
    // Default constructor initializes a staff member with no name, a generic role, and empty credentials. #constructor
    Staff() {
        setBase(0, "");
        role = "Staff";
        username = "";
        password = "";
    }

    // Sets staff profile details and authentication credentials. #SetStaff-function
    void setStaff(int newId, const string &newName, const string &newRole,
                  const string &newUsername = "", const string &newPassword = "") {
        setBase(newId, newName);
        role = newRole;
        username = newUsername;
        password = newPassword;
    }

    string getRole() const { return role; }
    string getUsername() const { return username; }

    // Declares virtual login behavior to support runtime polymorphism. #polymorphism #auth
    virtual bool login(const string &inputUser, const string &inputPassword) const {
        return inputUser == username && inputPassword == password;
    }

    // Overrides display formatting for staff-specific output. #polymorphism
    void displayInfo() const override {
        cout << "ID: " << getId() << " | Name: " << getName()
             << " | Role: " << role
             << " | Username: " << username;
    }
};

// Defines the administrator specialization derived from Staff. #inheritance-single #inheritance-multilevel
class Admin : public Staff {
public:
    // Constructs the built-in administrator account. #auth #constructor
    Admin() {
        setStaff(1, "Main Admin", "Admin", "admin", "admin123");
    }

    // Overrides login and reuses Staff authentication logic. #auth #polymorphism
    bool login(const string &inputUser, const string &inputPassword) const override {
        return Staff::login(inputUser, inputPassword);
    }

    // Overrides display output for administrator details. #code #auth #polymorphism
    void displayInfo() const override {
        cout << "ID: " << getId() << " | Name: " << getName()
             << " | Role: Admin"
             << " | Username: " << getUsername();
    }
};

// Implements the central controller for library operations and workflows. #controller
class Library {
private:
    // Object collections for books, members, and staff accounts. #object-collections
    Book books[MAX_BOOKS];
    Member members[MAX_MEMBERS];
    Staff staffs[MAX_STAFF];
    // Tracks active record counts for each entity collection. #object-collections
    int bookCount;
    int memberCount;
    int staffCount;
    // Maintains auto-increment counters for generated IDs. #object-collections
    int nextBookId;
    int nextMemberId;
    int nextStaffId;

    // Holds the embedded administrator account instance. 
    Admin admin;

    // Tracks whether the current authenticated session belongs to admin. #check
    bool currentUserIsAdmin;

    // Returns the index of a book by ID, or -1 if not found. #book-list
    int findBookIndexById(int bookId) const {
        for (int i = 0; i < bookCount; i++) {
            if (books[i].getId() == bookId) {
                return i;
            }
        }
        return -1;
    }

    // Returns the index of a member by ID, or -1 if not found. #member-list
    int findMemberIndexById(int memberId) const {
        for (int i = 0; i < memberCount; i++) {
            const Member &member = members[i];
            if (member.getId() == memberId) {
                return i;
            }
        }
        return -1;
    }

    // Finds a staff account by credentials using login dispatch. #staff-list
    int findStaffIndexByCredentials(const string &user, const string &pass) const {
        for (int i = 0; i < staffCount; i++) {
            const Staff *staffAccount = &staffs[i];
            if (staffAccount->login(user, pass)) {
                return i;
            }
        }
        return -1;
    }

    // Membership package selection logic based on user choice. #SetMemberByChoice-function
    bool getMembershipInfoByChoice(int choice, string &category, int &durationDays) const {
        if (choice == 1) {
            category = "3 Months";
            durationDays = 90;
            return true;
        }
        if (choice == 2) {
            category = "6 Months";
            durationDays = 180;
            return true;
        }
        if (choice == 3) {
            category = "1 Year";
            durationDays = 365;
            return true;
        }
        return false;
    }

    // Executes admin/staff authentication with a maximum retry policy. #Authentication-function
    bool authenticateUser() {
        currentUserIsAdmin = false;
        // Allows up to 3 attempts for the user to provide valid login credentials. #validation
        for (int attempt = 1; attempt <= 3; attempt++) {
            int userType;
            cout << "\n" << C_BOLD << C_CYAN << "Login Type (Attempt " << attempt << "/3)" << C_RESET << "\n";
            cout << C_BLUE << "1. Admin\n";
            cout << "2. Staff\n" << C_RESET;
            cout << C_YELLOW << "Choose login type: " << C_RESET;

            // Validates login-type input before credential processing. #validation
            if (!(cin >> userType)) {
                cin.clear(); // Clear error state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
                cout << C_RED << "Invalid input. Please enter a number.\n" << C_RESET; 
                continue;
            }
            // Clears input buffer to prepare for reading credentials as text. #validation
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

            // Reads credentials as text to preserve user-provided values. #validation
            string inputUser, inputPassword;
            cout << C_YELLOW << "Username: " << C_RESET;
            getline(cin, inputUser);
            cout << C_YELLOW << "Password: " << C_RESET;
            getline(cin, inputPassword);

            // Authenticates admin by directly checking the admin account credentials. #polymorphism #pointer-dispatch
            if (userType == 1) {
                // Uses a pointer to the admin account to invoke the login method, demonstrating polymorphic dispatch. #pointer-dispatch
                const Staff *adminAccount = &admin;
                // Polymorphically calls the login method on the admin account pointer, which resolves to Admin::login at runtime. #polymorphism
                if (adminAccount->login(inputUser, inputPassword)) {
                    currentUserIsAdmin = true;
                    cout << "\n" << C_GREEN << "Login successful. Welcome, Admin.\n" << C_RESET;
                    return true;
                }
            // Authenticates staff by searching registered credential records. #polymorphism #pointer-dispatch
            } else if (userType == 2) {
                int staffIndex = findStaffIndexByCredentials(inputUser, inputPassword);
                if (staffIndex != -1) {
                    currentUserIsAdmin = false;
                    // Uses a pointer to the authenticated staff account to invoke displayInfo, demonstrating polymorphic dispatch. #pointer-dispatch
                    const LibraryEntity *staffProfile = &staffs[staffIndex];
                    cout << C_GREEN << "Login successful. Welcome, " << staffProfile->getName() << ".\n" << C_RESET;
                    return true;
                }
            } else {
                cout << C_RED << "Invalid login type.\n" << C_RESET;
                continue;
            }

            cout << C_RED << "Invalid credentials. Try again.\n" << C_RESET;
        }

        // Denies authentication after all retry attempts are exhausted. #validation
        cout << C_RED << "Too many failed attempts. Access denied.\n" << C_RESET;
        return false;
    }

public:
    // Initializes the library with default values and a built-in admin account. #default-constructor
    Library() {
        bookCount = 0;
        memberCount = 0;
        staffCount = 0;
        nextBookId = 1;
        nextMemberId = 1;
        nextStaffId = 1;
        currentUserIsAdmin = false;
        // Pre-populates the staff array with a default staff account for demonstration purposes. #PrePopulatedData
        staffs[staffCount].setStaff(nextStaffId, "Default Staff", "Librarian", "staff1", "staff123");
        staffCount++;
        nextStaffId++;
    }

    // Creates and stores a new book record. #AddBook-function
    void addBook(const string &title, const string &author) {
        if (bookCount >= MAX_BOOKS) {
            cout << C_RED << "Book limit reached. Cannot add more books.\n" << C_RESET;
            return;
        }

        books[bookCount].setBook(nextBookId, title, author);
        cout << C_GREEN << "Book added. Book ID: " << nextBookId << "\n" << C_RESET;
        bookCount++;
        nextBookId++;
    }

    // Displays all books with current availability status. #ListBooks-function
    void listBooks() const {
        if (bookCount == 0) {
            cout << C_YELLOW << "No books in library.\n" << C_RESET;
            return;
        }

        cout << "\n" << C_BOLD << C_MAGENTA << "--- Book List ---" << C_RESET << "\n";
        for (int i = 0; i < bookCount; i++) {
            const Book &book = books[i];
              cout << C_BOLD << C_CYAN << "ID: " << C_RESET << book.getId() << C_BOLD << C_CYAN << " | Title: " << C_RESET << book.getTitle()
                  << C_BOLD << C_CYAN << " | Author: " << C_RESET << book.getAuthor()
                  << C_BOLD << C_CYAN << " | Status: " << C_RESET << (book.isIssued() ? (C_RED + string("Issued") + C_RESET)
                                             : (C_GREEN + string("Available") + C_RESET));

            if (book.isIssued()) {
                cout << " (Member ID: " << book.getIssuedToMemberId() << ")";
            }
            cout << "\n";
        }
    }

    // Creates a member with the selected membership category. #AddMember-function
    void addMember(const string &name, const string &category, int durationDays) {
        if (memberCount >= MAX_MEMBERS) {
            cout << C_RED << "Member limit reached. Cannot add more members.\n" << C_RESET;
            return;
        }

        members[memberCount].setMember(nextMemberId, name, category, durationDays);
           cout << C_GREEN << "Member added. Member ID: " << nextMemberId
               << " | Membership: " << category << " (" << durationDays << " days)\n" << C_RESET;
        memberCount++;
        nextMemberId++;
    }

    // Displays members with membership and borrowing details. #ListMembers-function
    void listMembers() const {
        if (memberCount == 0) {
            cout << C_YELLOW << "No members registered.\n" << C_RESET;
            return;
        }

        cout << "\n" << C_BOLD << C_MAGENTA << "--- Member List ---" << C_RESET << "\n";
        for (int i = 0; i < memberCount; i++) {
            const Member &member = members[i];
            cout << C_BOLD << C_BLUE << "ID: " << C_RESET << member.getId() << C_BOLD << C_BLUE << " | Name: " << C_RESET << member.getName()
                 << C_BOLD << C_BLUE << " | Membership: " << C_RESET << member.getMembershipCategory()
                 << C_BOLD << C_BLUE << " (" << C_RESET << member.getMembershipDurationDays() << C_BOLD << C_BLUE << " days)"
                 << C_BOLD << C_BLUE << " | Borrowed Books ID: " << C_RESET;

            if (member.getBorrowedCount() == 0) {
                cout << "None";
            } else {
                for (int j = 0; j < member.getBorrowedCount(); j++) {
                    cout << member.getBorrowedBookIdAt(j);
                    if (j + 1 != member.getBorrowedCount()) {
                        cout << ", ";
                    }
                }
            }

            cout << "\n";
        }
    }

    // Registers a new staff account with credentials. #AddStaff-function
    void addStaff(const string &staffName, const string &staffRole,
                  const string &username, const string &password) {
        if (staffCount >= MAX_STAFF) {
            cout << C_RED << "Staff limit reached. Cannot add more staff.\n" << C_RESET;
            return;
        }

        staffs[staffCount].setStaff(nextStaffId, staffName, staffRole, username, password);
        cout << C_GREEN << "Staff added. Staff ID: " << nextStaffId << "\n" << C_RESET;
        staffCount++;
        nextStaffId++;
    }

    // Displays staff records via polymorphic display dispatch. #ListStaff-function
    void listStaff() const {
        if (staffCount == 0) {
            cout << C_YELLOW << "No staff registered.\n" << C_RESET;
            return;
        }

        cout << "\n" << C_BOLD << C_MAGENTA << "--- Staff List ---" << C_RESET << "\n";
        // Uses a pointer to the base class to invoke the displayInfo method, demonstrating polymorphic dispatch for staff records. #polymorphism
        for (int i = 0; i < staffCount; i++) {
            const LibraryEntity *staffEntity = &staffs[i];
            staffEntity->displayInfo();
            cout << "\n";
        }
    }

    // Issues a book to a member after validation checks. #IssueBook-function
    void issueBook(int bookId, int memberId) {
        int bookIndex = findBookIndexById(bookId);
        if (bookIndex == -1) {
            cout << C_RED << "Invalid book ID.\n" << C_RESET;
            return;
        }

        int memberIndex = findMemberIndexById(memberId);
        if (memberIndex == -1) {
            cout << C_RED << "Invalid member ID.\n" << C_RESET;
            return;
        }

        // References to the book and member objects for direct manipulation. #object-references
        Book &book = books[bookIndex];
        Member &member = members[memberIndex];

        if (book.isIssued()) {
            cout << C_RED << "Book already issued.\n" << C_RESET;
            return;
        }

        if (!member.borrowBook(bookId)) {
            cout << C_RED << "Member borrowing limit reached.\n" << C_RESET;
            return;
        }

        book.issueTo(memberId);
        cout << C_GREEN << "Book issued successfully.\n" << C_RESET;
    }

    // Processes book return with consistency validation. #ReturnBook-function
    void returnBook(int bookId, int memberId) {
        int bookIndex = findBookIndexById(bookId);
        if (bookIndex == -1) {
            cout << C_RED << "Invalid book ID.\n" << C_RESET;
            return;
        }

        int memberIndex = findMemberIndexById(memberId);
        if (memberIndex == -1) {
            cout << C_RED << "Invalid member ID.\n" << C_RESET;
            return;
        }
        // References to the book and member objects for direct manipulation. #object-references
        Book &book = books[bookIndex];
        Member &member = members[memberIndex];

        if (!book.isIssued() || book.getIssuedToMemberId() != memberId) {
            cout << C_RED << "This book is not issued to this member.\n" << C_RESET;
            return;
        }

        if (!member.returnBook(bookId)) {
            cout << C_RED << "Member record mismatch.\n" << C_RESET;
            return;
        }

        book.markReturned();
        cout << C_GREEN << "Book returned successfully.\n" << C_RESET;
    }

    // Renders the primary console menu interface. #menu
    void showMenu() const {
        cout << C_BOLD << C_CYAN << R"(
    ============================================================
       _      _____ ____  _____         ________     __ 
      | |    |_   _|  _ \|  __ \    /\  |  __ \ \   / /
      | |      | | | |_) | |__) |  /  \ | |__) \ \_/ / 
      | |      | | |  _ <|  _  /  / /\ \|  _  / \   /  
      | |____ _| |_| |_) | | \ \ / ____ \ | \ \  | |   
      |______|_____|____/|_|  \_\/_/    \_\_|  \_\_|   

               _          _  __          _              
              | |        | |/ /         | |             
            __| | ___    | ' / _ __ __ _| | _____  __ _  
           / _` |/ _ \   |  < | '__/ _` | |/ / _ \| '_ \. 
          | (_| |  __/   | . \  | | (_| |   <  __/| | | |
           \__,_|\___|   |_|\_\_|  \__,_|_|\_\____|_| |_|
                                                       
    ============================================================
    )" << C_RESET << endl;
        cout << C_BLUE << "1. Add Book" << endl;
        cout << C_BLUE << "2. List Books" << endl;
        cout << C_BLUE << "3. Add Member" << endl;
        cout << C_BLUE << "4. List Members" << endl;
        cout << C_BLUE << "5. Add Staff (Admin Only)" << endl;
        cout << C_BLUE << "6. List Staff" << endl;
        cout << C_BLUE << "7. Issue Book" << endl;
        cout << C_BLUE << "8. Return Book" << endl;
        cout << C_BLUE << "9. Logout" << endl;
        cout << C_BLUE << "10. Exit" << endl << C_RESET;
        cout << C_YELLOW << "Enter your choice: " << C_RESET;
    }

    // Main loop that manages user authentication and menu-driven interactions. #Run-function #MainLoop
    void run() {
        // Enforces authentication before allowing operational access. #Menu
        while (true) {
            // Enforces authentication before allowing operational access. #menu
            if (!authenticateUser()) {
                return;
            }
            // Tracks the login state to manage session flow. #check
            bool isLoggedIn = true;

            while (isLoggedIn) {
            // Displays the menu for each interaction cycle. #menu
            showMenu();

            int choice;
            // Validates numeric menu input before command dispatch. #validation
            if (!(cin >> choice)) {
                // Clears the input buffer to discard invalid input. #validation
                cin.clear();
                // Ignores the rest of the line to prepare for the next input. #validation
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << C_RED << "Invalid input. Please enter a number.\n" << C_RESET;
                continue;
            }

            // Ignores the rest of the line to prepare for the next input, especially important after numeric input. #validation
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            // Dispatches operations according to the selected menu command. #flow
            /*
                Choice 1-10 correspond to the following operations:
                1. Add Book - Prompts for book details and adds a new book record.
                2. List Books - Displays all books with their status.
                3. Add Member - Prompts for member details and membership package, then adds a new member record.
                4. List Members - Displays all members with their membership and borrowing details.
                5. Add Staff - (Admin Only) Prompts for staff details and credentials, then adds a new staff record.
                6. List Staff - Displays all staff records with their roles.
                7. Issue Book - Prompts for book ID and member ID, then processes the book issuance.
                8. Return Book - Prompts for book ID and member ID, then processes the book return.
                9. Logout - Logs out the current user and returns to the authentication flow.
                10. Exit - Terminates the application and exits the process.

                -- Each operation includes validation checks and feedback messages to guide the user through the workflow. --
            */
            if (choice == 1) {
                string title, author;
                cout << C_BOLD << C_BLUE << "Enter book title: " << C_RESET;
                getline(cin, title);
                cout << C_BOLD << C_BLUE << "Enter author name: " << C_RESET;
                getline(cin, author);
                addBook(title, author);
            } else if (choice == 2) {
                listBooks();
            } else if (choice == 3) {
                string memberName;
                int membershipChoice;
                string membershipCategory;
                int membershipDurationDays;

                cout << C_BOLD << C_BLUE << "Enter member name: " << C_RESET;
                getline(cin, memberName);
                cout << C_BOLD << C_BLUE << "Choose membership duration:\n" << C_RESET;
                cout << C_BOLD << C_CYAN << "1. 3 Months (90 Days)\n" << C_RESET;
                cout << C_BOLD << C_CYAN << "2. 6 Months (180 Days)\n" << C_RESET;
                cout << C_BOLD << C_CYAN << "3. 1 Year (365 Days)\n" << C_RESET;
                cout << C_BOLD << C_BLUE << "Enter membership choice: " << C_RESET;

                // Validates membership selection input before processing. #validation
                if (!(cin >> membershipChoice)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << C_RED << "Invalid input. Membership not added.\n" << C_RESET;
                    continue;
                }
                // Ignores the rest of the line to prepare for the next input, especially important after numeric input. #validation
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                // Validates membership option against supported categories. #validation
                if (!getMembershipInfoByChoice(membershipChoice, membershipCategory, membershipDurationDays)) {
                    cout << C_RED << "Invalid membership choice. Member not added.\n" << C_RESET;
                    continue;
                }
                // Proceeds to add the member with the selected membership package. #flow
                addMember(memberName, membershipCategory, membershipDurationDays);
            } else if (choice == 4) {
                listMembers();
            } else if (choice == 5) {
                // Enforces admin-only access control for staff management operations. #check
                if (!currentUserIsAdmin) {
                    cout << C_RED << "Access denied. Only admin can add staff.\n" << C_RESET;
                    continue;
                }

                string staffName, staffRole, username, password;
                cout << C_BOLD << C_BLUE << "Enter staff name: " << C_RESET;
                getline(cin, staffName);
                cout << C_BOLD << C_BLUE << "Enter staff role: " << C_RESET;
                getline(cin, staffRole);
                cout << C_BOLD << C_BLUE << "Set staff username: " << C_RESET;
                getline(cin, username);
                cout << C_BOLD << C_BLUE << "Set staff password: " << C_RESET;
                getline(cin, password);
                addStaff(staffName, staffRole, username, password);
            } else if (choice == 6) {
                listStaff();
            } else if (choice == 7) {
                int bookId, memberId;
                cout << C_BOLD << C_BLUE << "Enter book ID: " << C_RESET;
                cin >> bookId;
                cout << C_BOLD << C_BLUE << "Enter member ID: " << C_RESET  ;
                cin >> memberId;
                // Ignores the rest of the line to prepare for the next input, especially important after numeric input. #validation
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                issueBook(bookId, memberId);
            } else if (choice == 8) {
                int bookId, memberId;
                cout << C_BOLD << C_BLUE << "Enter book ID: " << C_RESET;
                cin >> bookId;
                cout << C_BOLD << C_BLUE << "Enter member ID: " << C_RESET;
                cin >> memberId;
                // Ignores the rest of the line to prepare for the next input, especially important after numeric input. #validation
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                returnBook(bookId, memberId);
            } else if (choice == 9) {
                // Logs out the current user and returns to authentication flow. #code #auth #flow
                cout << C_GREEN << "Logged out successfully.\n" << C_RESET;
                isLoggedIn = false;
            } else if (choice == 10) {
                // Terminates the application and exits the process. #code
                cout << C_GREEN << "Exiting Library Management System. Goodbye!\n" << C_RESET;
                return;
            } else {
                cout << C_RED << "Invalid choice. Please try again.\n" << C_RESET;
            }
        }
        }
    }
};

int main(){
    // Initializes color-capable terminal output before rendering UI. #color
    enableVirtualTerminalColors();
    // Creates one persistent library controller instance for runtime. #Instance
    static Library library;
    // Starts the interactive library management workflow. #code
    library.run();
    return 0;
}