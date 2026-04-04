#include <bits/stdc++.h> // Include most standard C++ headers for easy classroom use
using namespace std; // Use standard namespace to keep syntax simple

class IEntity { // Interface class for runtime polymorphism demo
public: // Public section of interface
    virtual void showInfo() const = 0; // Pure virtual function for polymorphic display
    virtual ~IEntity() = default; // Virtual destructor for safe cleanup
}; // End interface

class FinePolicy { // Extra base class to support multiple inheritance
public: // Public section of FinePolicy
    virtual double finePerDay() const { return 2.0; } // Default fine amount per day
    virtual ~FinePolicy() = default; // Virtual destructor for inheritance safety
}; // End FinePolicy class

class NotificationSupport { // Another helper class for hybrid inheritance demo
public: // Public section of NotificationSupport
    void sendNotice(const string& message) const { // Simple notification function
        cout << "Notice: " << message << "\n"; // Print a notice message
    } // End function
}; // End NotificationSupport class

class Person : public IEntity { // Base class for people in the system
private: // Private section (encapsulation)
    int id; // Person id is private
    string name; // Person name is private

protected: // Protected section for child classes
    string email; // Email is protected to show protected access

public: // Public section for accessible functions
    Person(int newId = 0, const string& newName = "", const string& newEmail = "") // Constructor with defaults
        : id(newId), name(newName), email(newEmail) {} // Initialize data members

    int getId() const { return id; } // Getter for private id
    string getName() const { return name; } // Getter for private name
    string getEmail() const { return email; } // Getter for protected email

    void setName(const string& newName) { name = newName; } // Setter for private name
    void setEmail(const string& newEmail) { email = newEmail; } // Setter for protected email

    virtual string role() const { return "Person"; } // Virtual function for polymorphism
    virtual int borrowLimit() const { return 0; } // Virtual function for limit override

    void showInfo() const override { // Polymorphic display function
        cout << "ID: " << id << " | Name: " << name << " | Email: " << email << " | Role: " << role(); // Print person info
    } // End showInfo

    virtual ~Person() = default; // Virtual destructor for base class
}; // End Person class

class Member : public Person { // Single inheritance: Member -> Person
private: // Private data for encapsulation
    vector<int> borrowedBookIds; // List of borrowed book ids

public: // Public section of Member
    Member(int newId = 0, const string& newName = "", const string& newEmail = "") // Constructor
        : Person(newId, newName, newEmail) {} // Call base constructor

    bool borrowBook(int bookId) { // Add a borrowed book
        if ((int)borrowedBookIds.size() >= borrowLimit()) { // Check limit using polymorphic method
            return false; // Borrow not allowed
        } // End if
        borrowedBookIds.push_back(bookId); // Store borrowed book id
        return true; // Borrow success
    } // End borrowBook

    bool returnBook(int bookId) { // Remove a borrowed book
        auto it = find(borrowedBookIds.begin(), borrowedBookIds.end(), bookId); // Find target id
        if (it == borrowedBookIds.end()) { // If book id not found
            return false; // Return failed
        } // End if
        borrowedBookIds.erase(it); // Remove the matched id
        return true; // Return success
    } // End returnBook

    const vector<int>& getBorrowedBooks() const { return borrowedBookIds; } // Read-only borrowed list getter

    string role() const override { return "Member"; } // Override role (polymorphism)
    int borrowLimit() const override { return 2; } // Default member can borrow 2 books

    void showInfo() const override { // Override display function
        Person::showInfo(); // Reuse base info
        cout << " | Borrowed: "; // Print borrowed heading
        if (borrowedBookIds.empty()) { // If no borrowed books
            cout << "None"; // Print none
        } else { // If there are borrowed books
            for (size_t i = 0; i < borrowedBookIds.size(); i++) { // Loop through borrowed ids
                cout << borrowedBookIds[i]; // Print one id
                if (i + 1 != borrowedBookIds.size()) { // If not last item
                    cout << ", "; // Print separator
                } // End if
            } // End loop
        } // End else
    } // End showInfo
}; // End Member class

class StudentMember : public Member { // Hierarchical inheritance child 1: StudentMember -> Member
public: // Public section of StudentMember
    StudentMember(int newId = 0, const string& newName = "", const string& newEmail = "") // Constructor
        : Member(newId, newName, newEmail) {} // Call parent constructor

    string role() const override { return "Student Member"; } // Student role name
    int borrowLimit() const override { return 3; } // Student borrow limit
}; // End StudentMember class

class PremiumStudentMember : public StudentMember { // Multilevel inheritance: Premium -> Student -> Member -> Person
public: // Public section of PremiumStudentMember
    PremiumStudentMember(int newId = 0, const string& newName = "", const string& newEmail = "") // Constructor
        : StudentMember(newId, newName, newEmail) {} // Call parent constructor

    string role() const override { return "Premium Student Member"; } // Premium role name
    int borrowLimit() const override { return 5; } // Premium borrow limit
}; // End PremiumStudentMember class

class TeacherMember : public Member { // Hierarchical inheritance child 2: TeacherMember -> Member
public: // Public section of TeacherMember
    TeacherMember(int newId = 0, const string& newName = "", const string& newEmail = "") // Constructor
        : Member(newId, newName, newEmail) {} // Call parent constructor

    string role() const override { return "Teacher Member"; } // Teacher role name
    int borrowLimit() const override { return 6; } // Teacher borrow limit
}; // End TeacherMember class

class Staff : public Person { // Single inheritance example for staff branch
public: // Public section of Staff
    Staff(int newId = 0, const string& newName = "", const string& newEmail = "") // Constructor
        : Person(newId, newName, newEmail) {} // Call base constructor

    string role() const override { return "Staff"; } // Staff role text
}; // End Staff class

class Librarian : public Staff, public FinePolicy { // Multiple inheritance: Librarian -> Staff + FinePolicy
public: // Public section of Librarian
    Librarian(int newId = 0, const string& newName = "", const string& newEmail = "") // Constructor
        : Staff(newId, newName, newEmail) {} // Build Staff part

    string role() const override { return "Librarian"; } // Librarian role

    void showInfo() const override { // Override display for librarian
        Staff::showInfo(); // Reuse staff/person info
        cout << " | Fine/Day: " << finePerDay(); // Add fine policy data
    } // End showInfo
}; // End Librarian class

class SmartLibrarian : public Librarian, public NotificationSupport { // Hybrid inheritance (multilevel + multiple combined)
public: // Public section of SmartLibrarian
    SmartLibrarian(int newId = 0, const string& newName = "", const string& newEmail = "") // Constructor
        : Librarian(newId, newName, newEmail) {} // Call parent constructor

    string role() const override { return "Smart Librarian"; } // Hybrid class role
}; // End SmartLibrarian class

class Book { // Book entity class
private: // Private book data (encapsulation)
    int id; // Book id
    string title; // Book title
    string author; // Book author
    bool issued; // Issue state
    int issuedToMemberId; // Member id who borrowed

public: // Public section of Book
    Book(int newId = 0, const string& newTitle = "", const string& newAuthor = "") // Constructor
        : id(newId), title(newTitle), author(newAuthor), issued(false), issuedToMemberId(-1) {} // Initialize values

    int getId() const { return id; } // Getter for id
    string getTitle() const { return title; } // Getter for title
    string getAuthor() const { return author; } // Getter for author
    bool isIssued() const { return issued; } // Getter for issue state
    int getIssuedToMemberId() const { return issuedToMemberId; } // Getter for borrower id

    void issueTo(int memberId) { // Mark as issued to a member
        issued = true; // Set issue state true
        issuedToMemberId = memberId; // Save member id
    } // End issueTo

    void markReturned() { // Mark as returned
        issued = false; // Set issue state false
        issuedToMemberId = -1; // Clear member id
    } // End markReturned
}; // End Book class

class Library { // Main library manager class
private: // Private section of Library
    vector<Book> books; // Dynamic list of books
    vector<unique_ptr<Member>> members; // Polymorphic list of members
    int nextBookId; // Auto id counter for books
    int nextMemberId; // Auto id counter for members

    int findBookIndexById(int bookId) const { // Find book index helper
        for (int i = 0; i < (int)books.size(); i++) { // Loop through books
            if (books[i].getId() == bookId) { // Match by id
                return i; // Return index
            } // End if
        } // End loop
        return -1; // Not found
    } // End findBookIndexById

    Member* findMemberById(int memberId) const { // Find member pointer helper
        for (const auto& memberPtr : members) { // Loop through member pointers
            if (memberPtr->getId() == memberId) { // Match id
                return memberPtr.get(); // Return raw pointer
            } // End if
        } // End loop
        return nullptr; // Not found
    } // End findMemberById

public: // Public section of Library
    Library() : nextBookId(1), nextMemberId(1) {} // Initialize id counters

    void addBook(const string& title, const string& author) { // Add a book function
        books.emplace_back(nextBookId, title, author); // Create and store new book
        cout << "Book added. ID: " << nextBookId << "\n"; // Print success message
        nextBookId++; // Increase next book id
    } // End addBook

    void listBooks() const { // Show all books
        if (books.empty()) { // If no books exist
            cout << "No books available.\n"; // Print empty message
            return; // Exit function
        } // End if

        cout << "\n--- Book List ---\n"; // Print list heading
        for (const Book& book : books) { // Loop through books
            cout << "ID: " << book.getId() // Print id
                 << " | Title: " << book.getTitle() // Print title
                 << " | Author: " << book.getAuthor() // Print author
                 << " | Status: " << (book.isIssued() ? "Issued" : "Available"); // Print status
            if (book.isIssued()) { // If book is issued
                cout << " (Member ID: " << book.getIssuedToMemberId() << ")"; // Print borrower info
            } // End if
            cout << "\n"; // New line for next book
        } // End loop
    } // End listBooks

    void addMember(int typeChoice, const string& name, const string& email) { // Add member with chosen type
        unique_ptr<Member> newMember; // Polymorphic pointer for new member

        if (typeChoice == 1) { // Type 1 means student
            newMember = make_unique<StudentMember>(nextMemberId, name, email); // Create student member
        } else if (typeChoice == 2) { // Type 2 means teacher
            newMember = make_unique<TeacherMember>(nextMemberId, name, email); // Create teacher member
        } else if (typeChoice == 3) { // Type 3 means premium student
            newMember = make_unique<PremiumStudentMember>(nextMemberId, name, email); // Create premium student member
        } else { // Invalid type choice
            cout << "Invalid member type.\n"; // Print error
            return; // Exit function
        } // End type check

        cout << "Member added. ID: " << nextMemberId << " | Type: " << newMember->role() << "\n"; // Print success
        members.push_back(move(newMember)); // Store polymorphic member pointer
        nextMemberId++; // Increase next member id
    } // End addMember

    void listMembers() const { // Show all members
        if (members.empty()) { // If no members exist
            cout << "No members available.\n"; // Print empty message
            return; // Exit function
        } // End if

        cout << "\n--- Member List ---\n"; // Print heading
        for (const auto& memberPtr : members) { // Loop through members
            const Person* personView = memberPtr.get(); // Upcast to base pointer (polymorphism)
            personView->showInfo(); // Call virtual function
            cout << "\n"; // New line
        } // End loop
    } // End listMembers

    void issueBook(int bookId, int memberId) { // Issue book to a member
        int bookIndex = findBookIndexById(bookId); // Find book index
        if (bookIndex == -1) { // If book not found
            cout << "Invalid book ID.\n"; // Print error
            return; // Exit function
        } // End if

        Member* member = findMemberById(memberId); // Find member pointer
        if (member == nullptr) { // If member not found
            cout << "Invalid member ID.\n"; // Print error
            return; // Exit function
        } // End if

        Book& book = books[bookIndex]; // Reference selected book
        if (book.isIssued()) { // If already issued
            cout << "Book is already issued.\n"; // Print error
            return; // Exit function
        } // End if

        if (!member->borrowBook(bookId)) { // Check member borrow limit
            cout << "Borrow limit reached for this member type.\n"; // Print error
            return; // Exit function
        } // End if

        book.issueTo(memberId); // Mark book as issued
        cout << "Book issued successfully.\n"; // Print success
    } // End issueBook

    void returnBook(int bookId, int memberId) { // Return book from a member
        int bookIndex = findBookIndexById(bookId); // Find book index
        if (bookIndex == -1) { // If book not found
            cout << "Invalid book ID.\n"; // Print error
            return; // Exit function
        } // End if

        Member* member = findMemberById(memberId); // Find member pointer
        if (member == nullptr) { // If member not found
            cout << "Invalid member ID.\n"; // Print error
            return; // Exit function
        } // End if

        Book& book = books[bookIndex]; // Reference selected book
        if (!book.isIssued() || book.getIssuedToMemberId() != memberId) { // Validate issue relation
            cout << "This book is not issued to this member.\n"; // Print mismatch error
            return; // Exit function
        } // End if

        if (!member->returnBook(bookId)) { // Validate member borrowed list
            cout << "Member record mismatch.\n"; // Print mismatch error
            return; // Exit function
        } // End if

        book.markReturned(); // Mark book as returned
        cout << "Book returned successfully.\n"; // Print success
    } // End returnBook

    void showMenu() const { // Print menu
        cout << "\n===== Library Management System (OOP Demo) =====\n"; // Title line
        cout << "1. Add Book\n"; // Menu item 1
        cout << "2. List Books\n"; // Menu item 2
        cout << "3. Add Member\n"; // Menu item 3
        cout << "4. List Members\n"; // Menu item 4
        cout << "5. Issue Book\n"; // Menu item 5
        cout << "6. Return Book\n"; // Menu item 6
        cout << "7. Exit\n"; // Menu item 7
        cout << "Enter choice: "; // Choice prompt
    } // End showMenu

    void run() { // Main app loop
        while (true) { // Infinite loop until exit
            showMenu(); // Display options

            int choice; // Store menu choice
            if (!(cin >> choice)) { // Validate numeric input
                cin.clear(); // Clear fail state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Flush bad input
                cout << "Invalid input. Enter a number.\n"; // Print error
                continue; // Restart loop
            } // End validation

            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear trailing newline

            if (choice == 1) { // Add book block
                string title; // Book title variable
                string author; // Book author variable
                cout << "Enter title: "; // Prompt title
                getline(cin, title); // Read full title
                cout << "Enter author: "; // Prompt author
                getline(cin, author); // Read full author
                addBook(title, author); // Call add book
            } else if (choice == 2) { // List books block
                listBooks(); // Show books
            } else if (choice == 3) { // Add member block
                int typeChoice; // Member type choice
                string name; // Member name variable
                string email; // Member email variable
                cout << "Choose member type (1=Student, 2=Teacher, 3=Premium Student): "; // Prompt type
                cin >> typeChoice; // Read type
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear trailing newline
                cout << "Enter member name: "; // Prompt name
                getline(cin, name); // Read full name
                cout << "Enter member email: "; // Prompt email
                getline(cin, email); // Read full email
                addMember(typeChoice, name, email); // Call add member
            } else if (choice == 4) { // List members block
                listMembers(); // Show all members
            } else if (choice == 5) { // Issue book block
                int bookId; // Book id variable
                int memberId; // Member id variable
                cout << "Enter book ID: "; // Prompt book id
                cin >> bookId; // Read book id
                cout << "Enter member ID: "; // Prompt member id
                cin >> memberId; // Read member id
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear trailing newline
                issueBook(bookId, memberId); // Call issue function
            } else if (choice == 6) { // Return book block
                int bookId; // Book id variable
                int memberId; // Member id variable
                cout << "Enter book ID: "; // Prompt book id
                cin >> bookId; // Read book id
                cout << "Enter member ID: "; // Prompt member id
                cin >> memberId; // Read member id
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear trailing newline
                returnBook(bookId, memberId); // Call return function
            } else if (choice == 7) { // Exit block
                cout << "Goodbye!\n"; // Exit message
                break; // Stop loop
            } else { // Invalid choice block
                cout << "Invalid choice. Try again.\n"; // Print invalid choice message
            } // End choice check
        } // End while loop
    } // End run
}; // End Library class

int main() { // Program entry point
    SmartLibrarian demoLibrarian(9000, "Main Librarian", "librarian@library.com"); // Create hybrid object
    Person* baseView = &demoLibrarian; // Upcast object pointer to base class pointer
    cout << "--- Polymorphism Demo (Librarian) ---\n"; // Print demo heading
    baseView->showInfo(); // Call overridden virtual function through base pointer
    cout << "\n"; // Print newline
    demoLibrarian.sendNotice("System started."); // Call method from second base class

    Library library; // Create library object
    library.run(); // Run menu system
    return 0; // Return success to operating system
} // End main
