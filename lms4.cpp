#include <bits/stdc++.h>
using namespace std;

const int MAX_BOOKS = 100000;
const int MAX_MEMBERS = 10000;
const int MAX_BORROWED = 10;
const int MAX_STAFF = 10;


class LibraryEntity {
protected:
    int id;
    string name;

public:
    LibraryEntity() {
        id = 0;
        name = "";
    }

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
};


class Book : public LibraryEntity {
private:
    string author;
    bool issued;
    int issuedToMemberId;

public:
    Book() {
        setBase(0, "");
        author = "";
        issued = false;
        issuedToMemberId = -1;
    }

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

    void issueTo(int memberId) {
        issued = true;
        issuedToMemberId = memberId;
    }

    void markReturned() {
        issued = false;
        issuedToMemberId = -1;
    }
};


class Member : public LibraryEntity {
private:
    int borrowedBookIds[MAX_BORROWED];
    int borrowedCount;

public:
    Member() {
        setBase(0, "");
        borrowedCount = 0;
        for (int i = 0; i < MAX_BORROWED; i++) {
            borrowedBookIds[i] = 0;
        }
    }

    void setMember(int newId, const string &newName) {
        setBase(newId, newName);
        borrowedCount = 0;
        for (int i = 0; i < MAX_BORROWED; i++) {
            borrowedBookIds[i] = 0;
        }
    }

    int getBorrowedCount() const { return borrowedCount; }
    int getBorrowedBookIdAt(int index) const { return borrowedBookIds[index]; }

    bool borrowBook(int bookId) {
        if (borrowedCount >= MAX_BORROWED) {
            return false;
        }
        borrowedBookIds[borrowedCount] = bookId;
        borrowedCount++;
        return true;
    }

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


class Staff : public LibraryEntity {
private:
    string role;

public:
    Staff() {
        setBase(0, "");
        role = "Staff";
    }

    void setStaff(int newId, const string &newName, const string &newRole) {
        setBase(newId, newName);
        role = newRole;
    }

    string getRole() const { return role; }
};


class Admin : public Staff {
private:
    string username;
    string password;

public:
    Admin() {
        setStaff(1, "Main Admin", "Admin");
        username = "admin";
        password = "admin123";
    }

    bool login(const string &inputUser, const string &inputPassword) const {
        return inputUser == username && inputPassword == password;
    }
};


class Library {
private:
    Book books[MAX_BOOKS];
    Member members[MAX_MEMBERS];
    Staff staffs[MAX_STAFF];
    int bookCount;
    int memberCount;
    int staffCount;

    int nextBookId;
    int nextMemberId;
    int nextStaffId;

    Admin admin;

    int findBookIndexById(int bookId) const {
        for (int i = 0; i < bookCount; i++) {
            if (books[i].getId() == bookId) {
                return i;
            }
        }
        return -1;
    }

    int findMemberIndexById(int memberId) const {
        for (int i = 0; i < memberCount; i++) {
            const Member &member = members[i];
            if (member.getId() == memberId) {
                return i;
            }
        }
        return -1;
    }

public:
    Library() {
        bookCount = 0;
        memberCount = 0;
        staffCount = 0;
        nextBookId = 1;
        nextMemberId = 1;
        nextStaffId = 1;
    }

    bool adminLogin() const {
        for (int attempt = 1; attempt <= 3; attempt++) {
            string inputUser, inputPassword;
            cout << "\nAdmin Login (Attempt " << attempt << "/3)\n";
            cout << "Username: ";
            getline(cin, inputUser);
            cout << "Password: ";
            getline(cin, inputPassword);

            if (admin.login(inputUser, inputPassword)) {
                cout << "Login successful. Welcome, Admin.\n";
                return true;
            }

            cout << "Invalid credentials. Try again.\n";
        }

        cout << "Too many failed attempts. Access denied.\n";
        return false;
    }

    void addBook(const string &title, const string &author) {
        if (bookCount >= MAX_BOOKS) {
            cout << "Book limit reached. Cannot add more books.\n";
            return;
        }

        books[bookCount].setBook(nextBookId, title, author);
        cout << "Book added. Book ID: " << nextBookId << "\n";
        bookCount++;
        nextBookId++;
    }

    void listBooks() const {
        if (bookCount == 0) {
            cout << "No books in library.\n";
            return;
        }

        cout << "\n--- Book List ---\n";
        for (int i = 0; i < bookCount; i++) {
            const Book &book = books[i];
            cout << "ID: " << book.getId() << " | Title: " << book.getTitle()
                 << " | Author: " << book.getAuthor()
                 << " | Status: " << (book.isIssued() ? "Issued" : "Available");

            if (book.isIssued()) {
                cout << " (Member ID: " << book.getIssuedToMemberId() << ")";
            }
            cout << "\n";
        }
    }

    void addMember(const string &name) {
        if (memberCount >= MAX_MEMBERS) {
            cout << "Member limit reached. Cannot add more members.\n";
            return;
        }

        members[memberCount].setMember(nextMemberId, name);
        cout << "Member added. Member ID: " << nextMemberId << "\n";
        memberCount++;
        nextMemberId++;
    }

    void listMembers() const {
        if (memberCount == 0) {
            cout << "No members registered.\n";
            return;
        }

        cout << "\n--- Member List ---\n";
        for (int i = 0; i < memberCount; i++) {
            const Member &member = members[i];
            cout << "ID: " << member.getId() << " | Name: " << member.getName() << " | Borrowed Books ID: ";

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

    void addStaff(const string &staffName, const string &staffRole) {
        if (staffCount >= MAX_STAFF) {
            cout << "Staff limit reached. Cannot add more staff.\n";
            return;
        }

        staffs[staffCount].setStaff(nextStaffId, staffName, staffRole);
        cout << "Staff added. Staff ID: " << nextStaffId << "\n";
        staffCount++;
        nextStaffId++;
    }

    void listStaff() const {
        if (staffCount == 0) {
            cout << "No staff registered.\n";
            return;
        }

        cout << "\n--- Staff List ---\n";
        for (int i = 0; i < staffCount; i++) {
            cout << "ID: " << staffs[i].getId() << " | Name: " << staffs[i].getName()
                 << " | Role: " << staffs[i].getRole() << "\n";
        }
    }

    void issueBook(int bookId, int memberId) {
        int bookIndex = findBookIndexById(bookId);
        if (bookIndex == -1) {
            cout << "Invalid book ID.\n";
            return;
        }

        int memberIndex = findMemberIndexById(memberId);
        if (memberIndex == -1) {
            cout << "Invalid member ID.\n";
            return;
        }

        Book &book = books[bookIndex];
        Member &member = members[memberIndex];

        if (book.isIssued()) {
            cout << "Book already issued.\n";
            return;
        }

        if (!member.borrowBook(bookId)) {
            cout << "Member borrowing limit reached.\n";
            return;
        }

        book.issueTo(memberId);
        cout << "Book issued successfully.\n";
    }

    void returnBook(int bookId, int memberId) {
        int bookIndex = findBookIndexById(bookId);
        if (bookIndex == -1) {
            cout << "Invalid book ID.\n";
            return;
        }

        int memberIndex = findMemberIndexById(memberId);
        if (memberIndex == -1) {
            cout << "Invalid member ID.\n";
            return;
        }

        Book &book = books[bookIndex];
        Member &member = members[memberIndex];

        if (!book.isIssued() || book.getIssuedToMemberId() != memberId) {
            cout << "This book is not issued to this member.\n";
            return;
        }

        if (!member.returnBook(bookId)) {
            cout << "Member record mismatch.\n";
            return;
        }

        book.markReturned();
        cout << "Book returned successfully.\n";
    }

    void showMenu() const {
        cout << "\n===== Library Management System =====\n";
        cout << "1. Add Book\n";
        cout << "2. List Books\n";
        cout << "3. Add Member\n";
        cout << "4. List Members\n";
        cout << "5. Add Staff\n";
        cout << "6. List Staff\n";
        cout << "7. Issue Book\n";
        cout << "8. Return Book\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
    }

    void run() {
        if (!adminLogin()) {
            return;
        }

        while (true) {
            showMenu();

            int choice;
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number.\n";
                continue;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (choice == 1) {
                string title, author;
                cout << "Enter book title: ";
                getline(cin, title);
                cout << "Enter author name: ";
                getline(cin, author);
                addBook(title, author);
            } else if (choice == 2) {
                listBooks();
            } else if (choice == 3) {
                string memberName;
                cout << "Enter member name: ";
                getline(cin, memberName);
                addMember(memberName);
            } else if (choice == 4) {
                listMembers();
            } else if (choice == 5) {
                string staffName, staffRole;
                cout << "Enter staff name: ";
                getline(cin, staffName);
                cout << "Enter staff role: ";
                getline(cin, staffRole);
                addStaff(staffName, staffRole);
            } else if (choice == 6) {
                listStaff();
            } else if (choice == 7) {
                int bookId, memberId;
                cout << "Enter book ID: ";
                cin >> bookId;
                cout << "Enter member ID: ";
                cin >> memberId;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                issueBook(bookId, memberId);
            } else if (choice == 8) {
                int bookId, memberId;
                cout << "Enter book ID: ";
                cin >> bookId;
                cout << "Enter member ID: ";
                cin >> memberId;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                returnBook(bookId, memberId);
            } else if (choice == 9) {
                cout << "Exiting Library Management System. Goodbye!\n";
                break;
            } else {
                cout << "Invalid choice. Please try again.\n";
            }
        }
    }
};

int main(){
    static Library library;
    library.run();
    return 0;
}
