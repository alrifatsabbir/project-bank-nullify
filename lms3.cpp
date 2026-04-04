#include <bits/stdc++.h>
using namespace std;

const int MAX_BOOKS = 100000;
const int MAX_MEMBERS = 10000;
const int MAX_BORROWED = 10;

class Book {
private:
    int ISBN;
    string title;
    string author;
    bool issued;
    int issuedToMemberId;

public:
    Book(){
        ISBN = 0;
        title = "";
        author = "";
        issued = false;
        issuedToMemberId = -1;
    }

    void setBook(int id, const string &newTitle, const string &newAuthor){
        ISBN = id;
        title = newTitle;
        author = newAuthor;
        issued = false;
        issuedToMemberId = -1;
    }

    int getISBN() const {
        return ISBN;
    }

    string getTitle() const {
        return title;
    }

    string getAuthor() const {
        return author;
    }

    bool getIssued() const {
        return issued;
    }

    int getIssuedToMemberId() const{
        return issuedToMemberId;
    }

    void issueTo(int memberId){
        issued = true;
        issuedToMemberId = memberId;
    }

    void returnFrom(int memberId){
        issued = false;
        issuedToMemberId = -1;
    }
};

class Member{
private:
    int MemberId;
    string name;
    int borrowedBookISBN[MAX_BORROWED];
    int borrowedCount;

public:
    Member(){
        MemberId = 0;
        name = "";
        for(int i = 0; i < MAX_BORROWED; i++){
            borrowedBookISBN[i] = 0;
        }
        borrowedCount = 0;
    }

    void setMember(int newId, const string &newName){
        MemberId = newId;
        name = newName;
        for(int i = 0; i < MAX_BORROWED; i++){
            borrowedBookISBN[i] = 0;
        }
        borrowedCount = 0;
    }

    int getMemberId() const {
        return MemberId;
    }

    string getName() const {
        return name;
    }

    int getBorrowedBookISBN(int index) const {
        return borrowedBookISBN[index];
    }

    int getBorrowedCount() const {
        return borrowedCount;
    }

    bool borrowBook(int bookISBN){
        if(borrowedCount >= MAX_BORROWED){
            return false;
        }
        borrowedBookISBN[borrowedCount] = bookISBN;
        borrowedCount++;
        return true;
    }

    bool returnBook(int bookISBN){
        int position = -1;
        for(int i = 0; i < borrowedBookISBN; i++){
            if(borrowedBookISBN[i] == bookISBN){
                position = i;
                break;
            }
        }
        if(position == -1){
            return false;
        }
        for(int i = position; i < borrowedCount - 1; i++){
            borrowedBookISBN[i] = borrowedBookISBN[i+1];
        }
        borrowedBookISBN[borrowedCount - 1] = 0;
        borrowCount--;
        return true;
    }
};
class Library{
private:
    Book books[MAX_BOOKS];
    Member members[MAX_MEMBERS];
    int bookCount;
    int memberCount;

    int nextBookISBN;
    int nextMemberID;

    int findBookIndexByISBN(int bookISBN) const {
        for(int i = 0; i < bookCount; i++){
            if(books)
        }
    }
} 