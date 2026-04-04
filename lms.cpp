#include <bits/stdc++.h> // Include all standard C++ libraries
using namespace std; // Use the standard namespace

const int MAX_BOOKS = 1000; // Maximum number of books allowed in the library
const int MAX_MEMBERS = 1000; // Maximum number of members allowed in the library
const int MAX_BORROWED = 100; // Maximum number of books a member can borrow

// Book class to represent a book in the library
class Book {
private: // Private members of Book
    int id; // Book ID
    string title; // Book title
    string author; // Book author
    bool issued; // Whether the book is issued or not
    int issuedToMemberId; // ID of the member to whom the book is issued

public: // Public members of Book
    Book() { // Default constructor
        id = 0; // Initialize book ID to 0
        title = ""; // Initialize title to empty string
        author = ""; // Initialize author to empty string
        issued = false; // Book is not issued initially
        issuedToMemberId = -1; // No member has issued the book
    }

    void setBook(int newId, const string &newTitle, const string &newAuthor) { // Set book details
        id = newId; // Set book ID
        title = newTitle; // Set book title
        author = newAuthor; // Set book author
        issued = false; // Book is not issued
        issuedToMemberId = -1; // No member has issued the book
    }

    int getId() const { return id; } // Get book ID
    string getTitle() const { return title; } // Get book title
    string getAuthor() const { return author; } // Get book author
    bool isIssued() const { return issued; } // Check if book is issued
    int getIssuedToMemberId() const { return issuedToMemberId; } // Get member ID to whom book is issued

    void issueTo(int memberId) { // Issue book to a member
        issued = true; // Mark as issued
        issuedToMemberId = memberId; // Set member ID
    }

    void markReturned() { // Mark book as returned
        issued = false; // Mark as not issued
        issuedToMemberId = -1; // Reset member ID
    }
}; // End of Book class

// Member class to represent a library member
class Member {
private: // Private members of Member
    int id; // Member ID
    string name; // Member name
    int borrowedBookIds[MAX_BORROWED]; // Array of borrowed book IDs
    int borrowedCount; // Number of books currently borrowed

public: // Public members of Member
    Member() { // Default constructor
        id = 0; // Initialize member ID to 0
        name = ""; // Initialize name to empty string
        borrowedCount = 0; // No books borrowed initially
        for (int i = 0; i < MAX_BORROWED; i++) {
            borrowedBookIds[i] = 0; // Initialize borrowed book IDs to 0
        }
    }

    void setMember(int newId, const string &newName) { // Set member details
        id = newId; // Set member ID
        name = newName; // Set member name
        borrowedCount = 0; // Reset borrowed count
        for (int i = 0; i < MAX_BORROWED; i++) {
            borrowedBookIds[i] = 0; // Reset borrowed book IDs
        }
    }

    int getId() const { return id; } // Get member ID
    string getName() const { return name; } // Get member name
    int getBorrowedCount() const { return borrowedCount; } // Get number of borrowed books
    int getBorrowedBookIdAt(int index) const { return borrowedBookIds[index]; } // Get borrowed book ID at index

    bool borrowBook(int bookId) { // Borrow a book
        if (borrowedCount >= MAX_BORROWED) { // Check if borrowing limit reached
            return false; // Cannot borrow more
        }
        borrowedBookIds[borrowedCount] = bookId; // Add book ID to array
        borrowedCount++; // Increment borrowed count
        return true; // Borrow successful
    }

    bool returnBook(int bookId) { // Return a borrowed book
        int position = -1; // Position of book in array
        for (int i = 0; i < borrowedCount; i++) {
            if (borrowedBookIds[i] == bookId) { // Find the book
                position = i; // Store position
                break;
            }
        }

        if (position == -1) { // Book not found
            return false; // Return failed
        }

        // Shift left to remove one borrowed book id.
        for (int i = position; i < borrowedCount - 1; i++) {
            borrowedBookIds[i] = borrowedBookIds[i + 1]; // Shift elements left
        }
        borrowedBookIds[borrowedCount - 1] = 0; // Clear last element
        borrowedCount--; // Decrement borrowed count
        return true; // Return successful
    }
}; // End of Member class

// Library class to manage books and members
class Library {
private: // Private members of Library
    Book books[MAX_BOOKS]; // Array of books
    Member members[MAX_MEMBERS]; // Array of members
    int bookCount; // Number of books in library
    int memberCount; // Number of members in library

    int nextBookId; // Next book ID to assign
    int nextMemberId; // Next member ID to assign

    int findBookIndexById(int bookId) const { // Find index of book by ID
        for (int i = 0; i < bookCount; i++) {
            if (books[i].getId() == bookId) { // If book ID matches
                return i; // Return index
            }
        }
        return -1; // Not found
    }

    int findMemberIndexById(int memberId) const { // Find index of member by ID
        for (int i = 0; i < memberCount; i++) {
            const Member &member = members[i]; // Get member
            if (member.getId() == memberId) { // If member ID matches
                return i; // Return index
            }
        }
        return -1; // Not found
    }

public: // Public members of Library
    Library() { // Default constructor
        bookCount = 0; // No books initially
        memberCount = 0; // No members initially
        nextBookId = 1; // Start book IDs from 1
        nextMemberId = 1; // Start member IDs from 1
    }

    void addBook(const string &title, const string &author) { // Add a new book
        if (bookCount >= MAX_BOOKS) { // Check if book limit reached
            cout << "Book limit reached. Cannot add more books.\n";
            return; // Cannot add more
        }

        books[bookCount].setBook(nextBookId, title, author); // Set book details
        cout << "Book added. Book ID: " << nextBookId << "\n"; // Print confirmation
        bookCount++; // Increment book count
        nextBookId++; // Increment next book ID
    }

    void listBooks() const { // List all books
        if (bookCount == 0) { // No books in library
            cout << "No books in library.\n";
            return;
        }

        cout << "\n--- Book List ---\n"; // Print header
        for (int i = 0; i < bookCount; i++) {
            const Book &book = books[i]; // Get book
            cout << "ID: " << book.getId() << " | Title: " << book.getTitle()
                 << " | Author: " << book.getAuthor()
                 << " | Status: " << (book.isIssued() ? "Issued" : "Available"); // Print book info

            if (book.isIssued()) { // If issued, print member ID
                cout << " (Member ID: " << book.getIssuedToMemberId() << ")";
            }
            cout << "\n";
        }
    }

    void addMember(const string &name) { // Add a new member
        if (memberCount >= MAX_MEMBERS) { // Check if member limit reached
            cout << "Member limit reached. Cannot add more members.\n";
            return; // Cannot add more
        }

        members[memberCount].setMember(nextMemberId, name); // Set member details
        cout << "Member added. Member ID: " << nextMemberId << "\n"; // Print confirmation
        memberCount++; // Increment member count
        nextMemberId++; // Increment next member ID
    }

    void listMembers() const { // List all members
        if (memberCount == 0) { // No members registered
            cout << "No members registered.\n";
            return;
        }

        cout << "\n--- Member List ---\n"; // Print header
        for (int i = 0; i < memberCount; i++) {
            const Member &member = members[i]; // Get member
            cout << "ID: " << member.getId() << " | Name: " << member.getName() << " | Borrowed Books: "; // Print member info

            if (member.getBorrowedCount() == 0) { // No books borrowed
                cout << "None";
            } else {
                for (int j = 0; j < member.getBorrowedCount(); j++) {
                    cout << member.getBorrowedBookIdAt(j); // Print borrowed book ID
                    if (j + 1 != member.getBorrowedCount()) {
                        cout << ", "; // Print comma if not last
                    }
                }
            }

            cout << "\n";
        }
    }

    void issueBook(int bookId, int memberId) { // Issue a book to a member
        int bookIndex = findBookIndexById(bookId); // Find book index
        if (bookIndex == -1) { // Invalid book ID
            cout << "Invalid book ID.\n";
            return;
        }

        int memberIndex = findMemberIndexById(memberId); // Find member index
        if (memberIndex == -1) { // Invalid member ID
            cout << "Invalid member ID.\n";
            return;
        }

        Book &book = books[bookIndex]; // Reference to book
        Member &member = members[memberIndex]; // Reference to member

        if (book.isIssued()) { // Book already issued
            cout << "Book already issued.\n";
            return;
        }

        if (!member.borrowBook(bookId)) { // Member borrowing limit reached
            cout << "Member borrowing limit reached.\n";
            return;
        }

        book.issueTo(memberId); // Issue book to member
        cout << "Book issued successfully.\n";
    }

    void returnBook(int bookId, int memberId) { // Return a book from a member
        int bookIndex = findBookIndexById(bookId); // Find book index
        if (bookIndex == -1) { // Invalid book ID
            cout << "Invalid book ID.\n";
            return;
        }

        int memberIndex = findMemberIndexById(memberId); // Find member index
        if (memberIndex == -1) { // Invalid member ID
            cout << "Invalid member ID.\n";
            return;
        }

        Book &book = books[bookIndex]; // Reference to book
        Member &member = members[memberIndex]; // Reference to member

        if (!book.isIssued() || book.getIssuedToMemberId() != memberId) { // Book not issued to this member
            cout << "This book is not issued to this member.\n";
            return;
        }

        if (!member.returnBook(bookId)) { // Member record mismatch
            cout << "Member record mismatch.\n";
            return;
        }

        book.markReturned(); // Mark book as returned
        cout << "Book returned successfully.\n";
    }

    void showMenu() const { // Display the main menu
        cout << "\n===== Library Management System =====\n"; // Print title
        cout << "1. Add Book\n"; // Option 1
        cout << "2. List Books\n"; // Option 2
        cout << "3. Add Member\n"; // Option 3
        cout << "4. List Members\n"; // Option 4
        cout << "5. Issue Book\n"; // Option 5
        cout << "6. Return Book\n"; // Option 6
        cout << "7. Exit\n"; // Option 7
        cout << "Enter your choice: "; // Prompt user
    }

    void run() { // Main loop to run the system
        while (true) { // Infinite loop for menu
            showMenu(); // Show menu

            int choice; // User's choice
            if (!(cin >> choice)) { // If input is not a number
                cin.clear(); // Clear error state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
                cout << "Invalid input. Please enter a number.\n";
                continue; // Continue to next iteration
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore leftover input

            if (choice == 1) { // Add book
                string title, author; // Book details
                cout << "Enter book title: ";
                getline(cin, title); // Get book title
                cout << "Enter author name: ";
                getline(cin, author); // Get author name
                addBook(title, author); // Add book
            } else if (choice == 2) { // List books
                listBooks();
            } else if (choice == 3) { // Add member
                string memberName; // Member name
                cout << "Enter member name: ";
                getline(cin, memberName); // Get member name
                addMember(memberName); // Add member
            } else if (choice == 4) { // List members
                listMembers();
            } else if (choice == 5) { // Issue book
                int bookId, memberId; // IDs
                cout << "Enter book ID: ";
                cin >> bookId; // Get book ID
                cout << "Enter member ID: ";
                cin >> memberId; // Get member ID
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore leftover input
                issueBook(bookId, memberId); // Issue book
            } else if (choice == 6) { // Return book
                int bookId, memberId; // IDs
                cout << "Enter book ID: ";
                cin >> bookId; // Get book ID
                cout << "Enter member ID: ";
                cin >> memberId; // Get member ID
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore leftover input
                returnBook(bookId, memberId); // Return book
            } else if (choice == 7) { // Exit
                cout << "Exiting Library Management System. Goodbye!\n";
                break; // Exit loop
            } else { // Invalid choice
                cout << "Invalid choice. Please try again.\n";
            }
        }
    }
}; // End of Library class

int main(){ // Main function
    Library library; // Create library object
    library.run(); // Run the library system
    return 0; // Return success
}
