#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;

// Initialize database connection
void initDB() {
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }

    if (mysql_real_connect(conn, "localhost", "root", "", "library_db", 0, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }
}

typedef struct {
    int book_id;
    char title[100];
    int author_id;
    int publisher_id;
    char isbn[20];
    char genre[50];
    int year_published;
    int copies_available;
    char shelf_location[50];
} Book;

typedef struct {
    int author_id;
    char name[100];
    char bio[500];
} Author;

typedef struct {
    int publisher_id;
    char name[100];
    char address[150];
    char contact_info[100];
} Publisher;

typedef struct {
    int member_id;
    char name[100];
    char address[150];
    char phone[20];
    char email[100];
    char date_joined[20];
    char membership_status[20];
} Member;

typedef struct {
    int staff_id;
    char name[100];
    char role[50];
    char email[100];
    char phone[20];
} Staff;

typedef struct {
    int borrowing_id;
    int book_id;
    int member_id;
    char borrow_date[20];
    char due_date[20];
    char return_date[20];
    int staff_id;
} Borrowing;

typedef struct {
    int fine_id;
    int borrowing_id;
    float amount;
    int paid;
    char date_paid[20];
} Fine;

// Function prototypes
void addBook();
void viewBooks();
void updateBooks();
void deleteBooks();
void searchBookByID(int id);
void addAuthor();
void viewAuthors();
void updateAuthor();
void deleteAuthor();
void addPublisher();
void viewPublishers();
void updatePublisher();
void deletePublisher();
void addMember();
void viewMembers();
void updateMember();
void deleteMember();
void addStaff();
void viewStaff();
void updateStaff();
void deleteStaff();
void addBorrowing();
void viewBorrowings();
void updateBorrowing();
void deleteBorrowing();
void addFine();
void viewFines();
void updateFine();
void deleteFine();
void bookMenu();
void authorMenu();
void publisherMenu();
void memberMenu();
void staffMenu();
void borrowingMenu();
void fineMenu();
void mainMenu();

// Book Functions
void addBook() {
    Book b;
    char query[1000];
    
    printf("Enter book ID: ");
    scanf("%d", &b.book_id);
    getchar();
    printf("Enter title: ");
    scanf(" %[^\n]", b.title);
    printf("Enter author ID: ");
    scanf("%d", &b.author_id);
    printf("Enter publisher ID: ");
    scanf("%d", &b.publisher_id);
    getchar();
    printf("Enter the ISBN: ");
    scanf(" %[^\n]", b.isbn);
    printf("Enter genre: ");
    scanf(" %[^\n]", b.genre);
    printf("Enter year published: ");
    scanf("%d", &b.year_published);
    printf("Enter the copies available: ");
    scanf("%d", &b.copies_available);
    getchar();
    printf("Enter shelf location: ");
    scanf(" %[^\n]", b.shelf_location);
    
    sprintf(query, "INSERT INTO books VALUES(%d, '%s', %d, %d, '%s', '%s', %d, %d, '%s')",
            b.book_id, b.title, b.author_id, b.publisher_id, b.isbn, b.genre, 
            b.year_published, b.copies_available, b.shelf_location);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    printf("Book added successfully!\n");
}

void viewBooks() {
    printf("\n--- Book List ---\n");
    if (mysql_query(conn, "SELECT * FROM books")) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    res = mysql_use_result(conn);
    
    printf("%-10s %-30s %-10s %-10s %-15s %-20s %-10s %-10s %-15s\n", 
           "ID", "Title", "Author ID", "Pub ID", "ISBN", "Genre", "Year", "Copies", "Shelf Loc");
    
    while ((row = mysql_fetch_row(res)) != NULL) {
        printf("%-10s %-30s %-10s %-10s %-15s %-20s %-10s %-10s %-15s\n", 
               row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7], row[8]);
    }
    
    mysql_free_result(res);
}

void updateBooks() {
    int id, author_id, publisher_id, year_published, copies_available;
    char title[100], isbn[20], genre[50], shelf_location[50];
    char query[1000];
    
    printf("Enter book ID to update: ");
    scanf("%d", &id);
    getchar();
    printf("Enter new title: ");
    scanf(" %[^\n]", title);
    printf("Enter author ID: ");
    scanf("%d", &author_id);
    printf("Enter publisher ID: ");
    scanf("%d", &publisher_id);
    getchar();
    printf("Enter the ISBN: ");
    scanf(" %[^\n]", isbn);
    printf("Enter genre: ");
    scanf(" %[^\n]", genre);
    printf("Enter year published: ");
    scanf("%d", &year_published);
    printf("Enter the copies available: ");
    scanf("%d", &copies_available);
    getchar();
    printf("Enter shelf location: ");
    scanf(" %[^\n]", shelf_location);
    
    sprintf(query, "UPDATE books SET title='%s', author_id=%d, publisher_id=%d, isbn='%s', genre='%s', year_published=%d, copies_available=%d, shelf_location='%s' WHERE book_id=%d",
            title, author_id, publisher_id, isbn, genre, year_published, copies_available, shelf_location, id);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    printf("Book updated successfully!\n");
}

void deleteBooks() {
    int id;
    char query[100];
    
    printf("Enter book ID to delete: ");
    scanf("%d", &id);
    
    sprintf(query, "DELETE FROM books WHERE book_id=%d", id);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    printf("Book deleted successfully!\n");
}

void searchBookByID(int id) {
    char query[100];
    sprintf(query, "SELECT * FROM books WHERE book_id=%d", id);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    res = mysql_use_result(conn);
    
    if ((row = mysql_fetch_row(res)) != NULL) {
        printf("\nBook Details:\n");
        printf("ID: %s\nTitle: %s\nAuthor ID: %s\nPublisher ID: %s\n", row[0], row[1], row[2], row[3]);
        printf("ISBN: %s\nGenre: %s\nYear Published: %s\n", row[4], row[5], row[6]);
        printf("Copies Available: %s\nShelf Location: %s\n", row[7], row[8]);
    } else {
        printf("Book not found!\n");
    }
    
    mysql_free_result(res);
}

// Author Functions
void addAuthor() {
    Author a;
    char query[1000];
    
    printf("Enter Author ID: ");
    scanf("%d", &a.author_id);
    getchar();
    printf("Enter Name: ");
    scanf(" %[^\n]", a.name);
    printf("Enter Bio: ");
    scanf(" %[^\n]", a.bio);
    
    sprintf(query, "INSERT INTO authors VALUES(%d, '%s', '%s')", a.author_id, a.name, a.bio);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    printf("Author added successfully!\n");
}

void viewAuthors() {
    printf("\n--- Author List ---\n");
    if (mysql_query(conn, "SELECT * FROM authors")) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    res = mysql_use_result(conn);
    
    printf("%-10s %-30s %-50s\n", "ID", "Name", "Bio");
    
    while ((row = mysql_fetch_row(res)) != NULL) {
        printf("%-10s %-30s %-50s\n", row[0], row[1], row[2]);
    }
    
    mysql_free_result(res);
}

void updateAuthor() {
    int id;
    char name[100], bio[500];
    char query[1000];
    
    printf("Enter Author ID to update: ");
    scanf("%d", &id);
    getchar();
    printf("Enter New Name: ");
    scanf(" %[^\n]", name);
    printf("Enter New Bio: ");
    scanf(" %[^\n]", bio);
    
    sprintf(query, "UPDATE authors SET name='%s', bio='%s' WHERE author_id=%d", name, bio, id);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    printf("Author updated successfully!\n");
}

void deleteAuthor() {
    int id;
    char query[100];
    
    printf("Enter Author ID to delete: ");
    scanf("%d", &id);
    
    sprintf(query, "DELETE FROM authors WHERE author_id=%d", id);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    printf("Author deleted successfully!\n");
}

// Publisher Functions
void addPublisher() {
    Publisher p;
    char query[1000];
    
    printf("Enter Publisher ID: ");
    scanf("%d", &p.publisher_id);
    getchar();
    printf("Enter Name: ");
    scanf(" %[^\n]", p.name);
    printf("Enter Address: ");
    scanf(" %[^\n]", p.address);
    printf("Enter Contact Info: ");
    scanf(" %[^\n]", p.contact_info);
    
    sprintf(query, "INSERT INTO publishers VALUES(%d, '%s', '%s', '%s')", 
            p.publisher_id, p.name, p.address, p.contact_info);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    printf("Publisher added successfully!\n");
}

void viewPublishers() {
    printf("\n--- Publisher List ---\n");
    if (mysql_query(conn, "SELECT * FROM publishers")) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    res = mysql_use_result(conn);
    
    printf("%-10s %-30s %-50s %-30s\n", "ID", "Name", "Address", "Contact Info");
    
    while ((row = mysql_fetch_row(res)) != NULL) {
        printf("%-10s %-30s %-50s %-30s\n", row[0], row[1], row[2], row[3]);
    }
    
    mysql_free_result(res);
}

void updatePublisher() {
    int id;
    char name[100], address[150], contact_info[100];
    char query[1000];
    
    printf("Enter Publisher ID to update: ");
    scanf("%d", &id);
    getchar();
    printf("Enter New Name: ");
    scanf(" %[^\n]", name);
    printf("Enter New Address: ");
    scanf(" %[^\n]", address);
    printf("Enter New Contact Info: ");
    scanf(" %[^\n]", contact_info);
    
    sprintf(query, "UPDATE publishers SET name='%s', address='%s', contact_info='%s' WHERE publisher_id=%d",
            name, address, contact_info, id);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    printf("Publisher updated successfully!\n");
}

void deletePublisher() {
    int id;
    char query[100];
    
    printf("Enter Publisher ID to delete: ");
    scanf("%d", &id);
    
    sprintf(query, "DELETE FROM publishers WHERE publisher_id=%d", id);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    printf("Publisher deleted successfully!\n");
}

// Member Functions
void addMember() {
    Member m;
    char query[1000];
    
    printf("Enter Member ID: ");
    scanf("%d", &m.member_id);
    getchar();
    printf("Enter Name: ");
    scanf(" %[^\n]", m.name);
    printf("Enter Address: ");
    scanf(" %[^\n]", m.address);
    printf("Enter Phone: ");
    scanf(" %[^\n]", m.phone);
    printf("Enter Email: ");
    scanf(" %[^\n]", m.email);
    printf("Enter Date Joined (YYYY-MM-DD): ");
    scanf(" %[^\n]", m.date_joined);
    printf("Enter Membership Status: ");
    scanf(" %[^\n]", m.membership_status);
    
    sprintf(query, "INSERT INTO members VALUES(%d, '%s', '%s', '%s', '%s', '%s', '%s')",
            m.member_id, m.name, m.address, m.phone, m.email, m.date_joined, m.membership_status);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    printf("Member added successfully!\n");
}

void viewMembers() {
    printf("\n--- Member List ---\n");
    if (mysql_query(conn, "SELECT * FROM members")) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    res = mysql_use_result(conn);
    
    printf("%-10s %-30s %-50s %-20s %-30s %-15s %-20s\n", 
           "ID", "Name", "Address", "Phone", "Email", "Date Joined", "Status");
    
    while ((row = mysql_fetch_row(res)) != NULL) {
        printf("%-10s %-30s %-50s %-20s %-30s %-15s %-20s\n", 
               row[0], row[1], row[2], row[3], row[4], row[5], row[6]);
    }
    
    mysql_free_result(res);
}

void updateMember() {
    int id;
    char name[100], address[150], phone[20], email[100], date_joined[20], membership_status[20];
    char query[1000];
    
    printf("Enter Member ID to update: ");
    scanf("%d", &id);
    getchar();
    printf("Enter New Name: ");
    scanf(" %[^\n]", name);
    printf("Enter New Address: ");
    scanf(" %[^\n]", address);
    printf("Enter New Phone: ");
    scanf(" %[^\n]", phone);
    printf("Enter New Email: ");
    scanf(" %[^\n]", email);
    printf("Enter New Date Joined (YYYY-MM-DD): ");
    scanf(" %[^\n]", date_joined);
    printf("Enter New Membership Status: ");
    scanf(" %[^\n]", membership_status);
    
    sprintf(query, "UPDATE members SET name='%s', address='%s', phone='%s', email='%s', date_joined='%s', membership_status='%s' WHERE member_id=%d",
            name, address, phone, email, date_joined, membership_status, id);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    printf("Member updated successfully!\n");
}

void deleteMember() {
    int id;
    char query[100];
    
    printf("Enter Member ID to delete: ");
    scanf("%d", &id);
    
    sprintf(query, "DELETE FROM members WHERE member_id=%d", id);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    printf("Member deleted successfully!\n");
}

// Staff Functions
void addStaff() {
    Staff s;
    char query[1000];
    
    printf("Enter Staff ID: ");
    scanf("%d", &s.staff_id);
    getchar();
    printf("Enter Name: ");
    scanf(" %[^\n]", s.name);
    printf("Enter Role: ");
    scanf(" %[^\n]", s.role);
    printf("Enter Email: ");
    scanf(" %[^\n]", s.email);
    printf("Enter Phone number: ");
    scanf(" %[^\n]", s.phone);
    
    sprintf(query, "INSERT INTO staff VALUES(%d, '%s', '%s', '%s', '%s')",
            s.staff_id, s.name, s.role, s.email, s.phone);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    printf("Staff added successfully!\n");
}

void viewStaff() {
    printf("\n--- Staff List ---\n");
    if (mysql_query(conn, "SELECT * FROM staff")) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    res = mysql_use_result(conn);
    
    printf("%-10s %-30s %-20s %-30s %-15s\n", "ID", "Name", "Role", "Email", "Phone");
    
    while ((row = mysql_fetch_row(res)) != NULL) {
        printf("%-10s %-30s %-20s %-30s %-15s\n", row[0], row[1], row[2], row[3], row[4]);
    }
    
    mysql_free_result(res);
}

void updateStaff() {
    int id;
    char name[100], role[50], email[100], phone[20];
    char query[1000];
    
    printf("Enter Staff ID to update: ");
    scanf("%d", &id);
    getchar();
    printf("Enter New Name: ");
    scanf(" %[^\n]", name);
    printf("Enter New Role: ");
    scanf(" %[^\n]", role);
    printf("Enter New Email: ");
    scanf(" %[^\n]", email);
    printf("Enter New Phone number: ");
    scanf(" %[^\n]", phone);
    
    sprintf(query, "UPDATE staff SET name='%s', role='%s', email='%s', phone='%s' WHERE staff_id=%d",
            name, role, email, phone, id);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    printf("Staff updated successfully!\n");
}

void deleteStaff() {
    int id;
    char query[100];
    
    printf("Enter Staff ID to delete: ");
    scanf("%d", &id);
    
    sprintf(query, "DELETE FROM staff WHERE staff_id=%d", id);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    printf("Staff deleted successfully!\n");
}

// Borrowing Functions
void addBorrowing() {
    Borrowing b;
    char query[1000];
    
    printf("Enter Borrowing ID: ");
    scanf("%d", &b.borrowing_id);
    printf("Enter Book ID: ");
    scanf("%d", &b.book_id);
    printf("Enter Member ID: ");
    scanf("%d", &b.member_id);
    getchar();
    printf("Enter Borrow Date (YYYY-MM-DD): ");
    scanf(" %[^\n]", b.borrow_date);
    printf("Enter Due Date (YYYY-MM-DD): ");
    scanf(" %[^\n]", b.due_date);
    printf("Enter Return Date (YYYY-MM-DD, or empty): ");
    scanf(" %[^\n]", b.return_date);
    printf("Enter Staff ID: ");
    scanf("%d", &b.staff_id);
    
    if (strlen(b.return_date) == 0) {
        sprintf(query, "INSERT INTO borrowings (borrowing_id, book_id, member_id, borrow_date, due_date, staff_id) VALUES(%d, %d, %d, '%s', '%s', %d)",
                b.borrowing_id, b.book_id, b.member_id, b.borrow_date, b.due_date, b.staff_id);
    } else {
        sprintf(query, "INSERT INTO borrowings VALUES(%d, %d, %d, '%s', '%s', '%s', %d)",
                b.borrowing_id, b.book_id, b.member_id, b.borrow_date, b.due_date, b.return_date, b.staff_id);
    }
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    printf("Borrowing added successfully!\n");
}

void viewBorrowings() {
    printf("\n--- Borrowing List ---\n");
    if (mysql_query(conn, "SELECT * FROM borrowings")) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    res = mysql_use_result(conn);
    
    printf("%-10s %-10s %-10s %-15s %-15s %-15s %-10s\n", 
           "Borrow ID", "Book ID", "Member ID", "Borrow Date", "Due Date", "Return Date", "Staff ID");
    
    while ((row = mysql_fetch_row(res)) != NULL) {
        printf("%-10s %-10s %-10s %-15s %-15s %-15s %-10s\n", 
               row[0], row[1], row[2], row[3], row[4], row[5] ? row[5] : "NULL", row[6]);
    }
    
    mysql_free_result(res);
}

void updateBorrowing() {
    int id, book_id, member_id, staff_id;
    char borrow_date[20], due_date[20], return_date[20];
    char query[1000];
    
    printf("Enter Borrowing ID to update: ");
    scanf("%d", &id);
    getchar();
    printf("Enter New Book ID: ");
    scanf("%d", &book_id);
    printf("Enter New Member ID: ");
    scanf("%d", &member_id);
    getchar();
    printf("Enter New Borrow Date (YYYY-MM-DD): ");
    scanf(" %[^\n]", borrow_date);
    printf("Enter New Due Date (YYYY-MM-DD): ");
    scanf(" %[^\n]", due_date);
    printf("Enter New Return Date (YYYY-MM-DD, or empty): ");
    scanf(" %[^\n]", return_date);
    printf("Enter New Staff ID: ");
    scanf("%d", &staff_id);
    
    if (strlen(return_date) == 0) {
        sprintf(query, "UPDATE borrowings SET book_id=%d, member_id=%d, borrow_date='%s', due_date='%s', return_date=NULL, staff_id=%d WHERE borrowing_id=%d",
                book_id, member_id, borrow_date, due_date, staff_id, id);
    } else {
        sprintf(query, "UPDATE borrowings SET book_id=%d, member_id=%d, borrow_date='%s', due_date='%s', return_date='%s', staff_id=%d WHERE borrowing_id=%d",
                book_id, member_id, borrow_date, due_date, return_date, staff_id, id);
    }
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    printf("Borrowing updated successfully!\n");
}

void deleteBorrowing() {
    int id;
    char query[100];
    
    printf("Enter Borrowing ID to delete: ");
    scanf("%d", &id);
    
    sprintf(query, "DELETE FROM borrowings WHERE borrowing_id=%d", id);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    printf("Borrowing deleted successfully!\n");
}

// Fine Functions
void addFine() {
    Fine f;
    char query[1000];
    
    printf("Enter Fine ID: ");
    scanf("%d", &f.fine_id);
    printf("Enter Borrowing ID: ");
    scanf("%d", &f.borrowing_id);
    printf("Enter Amount: ");
    scanf("%f", &f.amount);
    printf("Enter Paid Status (0 for unpaid, 1 for paid): ");
    scanf("%d", &f.paid);
    getchar();
    printf("Enter Date Paid (YYYY-MM-DD, or empty): ");
    scanf(" %[^\n]", f.date_paid);
    
    if (strlen(f.date_paid) == 0) {
        sprintf(query, "INSERT INTO fines (fine_id, borrowing_id, amount, paid) VALUES(%d, %d, %.2f, %d)",
                f.fine_id, f.borrowing_id, f.amount, f.paid);
    } else {
        sprintf(query, "INSERT INTO fines VALUES(%d, %d, %.2f, %d, '%s')",
                f.fine_id, f.borrowing_id, f.amount, f.paid, f.date_paid);
    }
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    printf("Fine added successfully!\n");
}

void viewFines() {
    printf("\n--- Fine List ---\n");
    if (mysql_query(conn, "SELECT * FROM fines")) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    res = mysql_use_result(conn);
    
    printf("%-10s %-15s %-10s %-10s %-15s\n", "Fine ID", "Borrowing ID", "Amount", "Paid", "Date Paid");
    
    while ((row = mysql_fetch_row(res)) != NULL) {
        printf("%-10s %-15s %-10s %-10s %-15s\n", 
               row[0], row[1], row[2], row[3], row[4] ? row[4] : "NULL");
    }
    
    mysql_free_result(res);
}

void updateFine() {
    int id, borrowing_id, paid;
    float amount;
    char date_paid[20];
    char query[1000];
    
    printf("Enter Fine ID to update: ");
    scanf("%d", &id);
    getchar();
    printf("Enter New Borrowing ID: ");
    scanf("%d", &borrowing_id);
    printf("Enter New Amount: ");
    scanf("%f", &amount);
    printf("Enter New Paid Status (0 for unpaid, 1 for paid): ");
    scanf("%d", &paid);
    getchar();
    printf("Enter New Date Paid (YYYY-MM-DD, or empty): ");
    scanf(" %[^\n]", date_paid);
    
    if (strlen(date_paid) == 0) {
        sprintf(query, "UPDATE fines SET borrowing_id=%d, amount=%.2f, paid=%d, date_paid=NULL WHERE fine_id=%d",
                borrowing_id, amount, paid, id);
    } else {
        sprintf(query, "UPDATE fines SET borrowing_id=%d, amount=%.2f, paid=%d, date_paid='%s' WHERE fine_id=%d",
                borrowing_id, amount, paid, date_paid, id);
    }
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    printf("Fine updated successfully!\n");
}

void deleteFine() {
    int id;
    char query[100];
    
    printf("Enter Fine ID to delete: ");
    scanf("%d", &id);
    
    sprintf(query, "DELETE FROM fines WHERE fine_id=%d", id);
    
    if (mysql_query(conn, query)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        return;
    }
    
    printf("Fine deleted successfully!\n");
}

// Menu Functions
void bookMenu() {
    int choice, id;
    while (1) {
        printf("\n--- Book Menu ---\n");
        printf("1. Add Book\n2. View Books\n3. Update Book\n4. Delete Book\n5. Search Book by ID\n6. Back\nChoose: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: addBook(); break;
            case 2: viewBooks(); break;
            case 3: updateBooks(); break;
            case 4: deleteBooks(); break;
            case 5:
                printf("Enter Book ID to search: ");
                scanf("%d", &id);
                searchBookByID(id);
                break;
            case 6: return;
            default: printf("Invalid choice.\n");
        }
    }
}

void authorMenu() {
    int choice;
    while (1) {
        printf("\n--- Author Menu ---\n");
        printf("1. Add Author\n2. View Authors\n3. Update Author\n4. Delete Author\n5. Back\nChoose: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: addAuthor(); break;
            case 2: viewAuthors(); break;
            case 3: updateAuthor(); break;
            case 4: deleteAuthor(); break;
            case 5: return;
            default: printf("Invalid choice.\n");
        }
    }
}

void publisherMenu() {
    int choice;
    while (1) {
        printf("\n--- Publisher Menu ---\n");
        printf("1. Add Publisher\n2. View Publishers\n3. Update Publisher\n4. Delete Publisher\n5. Back\nChoose: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: addPublisher(); break;
            case 2: viewPublishers(); break;
            case 3: updatePublisher(); break;
            case 4: deletePublisher(); break;
            case 5: return;
            default: printf("Invalid choice.\n");
        }
    }
}

void memberMenu() {
    int choice;
    while (1) {
        printf("\n--- Member Menu ---\n");
        printf("1. Add Member\n2. View Members\n3. Update Member\n4. Delete Member\n5. Back\nChoose: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: addMember(); break;
            case 2: viewMembers(); break;
            case 3: updateMember(); break;
            case 4: deleteMember(); break;
            case 5: return;
            default: printf("Invalid choice.\n");
        }
    }
}

void staffMenu() {
    int choice;
    while (1) {
        printf("\n--- Staff Menu ---\n");
        printf("1. Add Staff\n2. View Staff\n3. Update Staff\n4. Delete Staff\n5. Back\nChoose: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: addStaff(); break;
            case 2: viewStaff(); break;
            case 3: updateStaff(); break;
            case 4: deleteStaff(); break;
            case 5: return;
            default: printf("Invalid choice.\n");
        }
    }
}

void borrowingMenu() {
    int choice;
    while (1) {
        printf("\n--- Borrowing Menu ---\n");
        printf("1. Add Borrowing\n2. View Borrowings\n3. Update Borrowing\n4. Delete Borrowing\n5. Back\nChoose: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: addBorrowing(); break;
            case 2: viewBorrowings(); break;
            case 3: updateBorrowing(); break;
            case 4: deleteBorrowing(); break;
            case 5: return;
            default: printf("Invalid choice.\n");
        }
    }
}

void fineMenu() {
    int choice;
    while (1) {
        printf("\n--- Fine Menu ---\n");
        printf("1. Add Fine\n2. View Fines\n3. Update Fine\n4. Delete Fine\n5. Back\nChoose: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: addFine(); break;
            case 2: viewFines(); break;
            case 3: updateFine(); break;
            case 4: deleteFine(); break;
            case 5: return;
            default: printf("Invalid choice.\n");
        }
    }
}

void mainMenu() {
    int choice;
    while (1) {
        printf("\n===== LIBRARY MANAGEMENT SYSTEM =====\n");
        printf("1. Manage Books\n");
        printf("2. Manage Authors\n");
        printf("3. Manage Publishers\n");
        printf("4. Manage Members\n");
        printf("5. Manage Staff\n");
        printf("6. Manage Borrowings\n");
        printf("7. Manage Fines\n");
        printf("8. Exit\n");
        printf("Choose: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: bookMenu(); break;
            case 2: authorMenu(); break;
            case 3: publisherMenu(); break;
            case 4: memberMenu(); break;
            case 5: staffMenu(); break;
            case 6: borrowingMenu(); break;
            case 7: fineMenu(); break;
            case 8: exit(0);
            default: printf("Invalid choice.\n");
        }
    }
}

int main() {
    initDB();
    mainMenu();
    mysql_close(conn);
    return 0;
}