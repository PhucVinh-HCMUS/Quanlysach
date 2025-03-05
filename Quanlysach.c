#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Book: Cấu trúc lưu trữ thông tin về một cuốn sách, bao gồm tên sách (name), mã số sách (id), giá (price), số lượng (quantity), và con trỏ next để trỏ đến sách kế tiếp trong danh sách liên kết.
//BookList: Cấu trúc lưu trữ danh sách liên kết đơn của các sách, với head là con trỏ trỏ đến cuốn sách đầu tiên trong danh sách.
typedef struct Book {
    char name[30];
    char id[13];
    int price;
    int quantity;
    struct Book *next;
} Book;
typedef struct {
    Book *head;
} BookList;
//initBookList: Hàm khởi tạo danh sách sách với head trỏ đến NULL, tức là danh sách ban đầu rỗng.
void initBookList(BookList *list) {
    list->head = NULL;
}
//readBooksFromFile: Hàm đọc thông tin sách từ một tệp văn bản (filename) và thêm các sách vào đầu danh sách liên kết. Mỗi sách mới sẽ trở thành sách đầu tiên trong danh sách.
void readBooksFromFile(BookList *list, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open file %s\n", filename);
        return;
    }

    while (!feof(file)) {
        Book *newBook = (Book *)malloc(sizeof(Book));
        if (fscanf(file, "%s %s %d %d", newBook->name, newBook->id, &newBook->price, &newBook->quantity) == 4) {
            newBook->next = list->head;
            list->head = newBook;
        } else {
            free(newBook);
        }
    }
    fclose(file);
}
//writeBooksToFile: Hàm ghi toàn bộ sách từ danh sách liên kết vào tệp.
void writeBooksToFile(BookList *list, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Cannot open file %s\n", filename);
        return;
    }

    Book *current = list->head;
    while (current != NULL) {
        fprintf(file, "%s %s %d %d\n", current->name, current->id, current->price, current->quantity);
        current = current->next;
    }
    fclose(file);
}
//findBookById: Hàm tìm và trả về sách có id tương ứng. Nếu không tìm thấy, trả về NULL
Book* findBookById(BookList *list, const char *id) {
    Book *current = list->head;
    while (current != NULL) {
        if (strcmp(current->id, id) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}
//displayBook: Hàm in thông tin của một sách ra màn hình. Nếu sách không tồn tại, hiển thị thông báo "Book not found!".
void displayBook(Book *book) {
    if (book) {
        printf("Name: %s\n", book->name);
        printf("ID: %s\n", book->id);
        printf("Price: %d\n", book->price);
        printf("Quantity: %d\n", book->quantity);
    } else {
        printf("Book not found!\n");
    }
}
//updateBookById: Hàm tìm và cập nhật thông tin cho sách dựa trên ID. Nếu không tìm thấy sách, thông báo lỗi.
void updateBookById(BookList *list, const char *id) {
    Book *book = findBookById(list, id);
    if (book) {
        printf("Enter new name: ");
        scanf("%s", book->name);
        printf("Enter new ID: ");
        scanf("%s", book->id);
        printf("Enter new price: ");
        scanf("%d", &book->price);
        printf("Enter new quantity: ");
        scanf("%d", &book->quantity);
    } else {
        printf("Book not found!\n");
    }
}
//radixSortBooksById: Hàm thực hiện Radix Sort để sắp xếp danh sách sách theo ID.
//countingSortByDigit: Hàm phụ trợ để thực hiện sắp xếp theo từng chữ số của ID.
int getMaxIdLength(BookList *list) {
    int maxLength = 0;
    Book *current = list->head;
    while (current != NULL) {
        int len = strlen(current->id);
        if (len > maxLength) {
            maxLength = len;
        }
        current = current->next;
    }
    return maxLength;
}

void countingSortByDigit(BookList *list, int exp) {
    BookList output;
    initBookList(&output);
    int count[10] = {0};
    Book *current = list->head;

    while (current != NULL) {
        int index = (atoi(current->id) / exp) % 10;
        count[index]++;
        current = current->next;
    }

    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    current = list->head;
    while (current != NULL) {
        int index = (atoi(current->id) / exp) % 10;
        Book *nextBook = current->next;
        current->next = output.head;
        output.head = current;
        count[index]--;
        current = nextBook;
    }

    list->head = output.head;
}

void radixSortBooksById(BookList *list) {
    int maxLength = getMaxIdLength(list);
    for (int exp = 1; maxLength / exp > 0; exp *= 10) {
        countingSortByDigit(list, exp);
    }
}
//displayMenu: Hàm hiển thị menu để người dùng lựa chọn các chức năng.
//displayAllBooks: Hàm duyệt và hiển thị thông tin tất cả sách trong danh sách liên kết.
void displayMenu() {
    printf("Library Management System\n");
    printf("1. Display all books\n");
    printf("2. Find book by ID\n");
    printf("3. Update book by ID\n");
    printf("4. Sort books by ID\n");
    printf("5. Exit\n");
    printf("Choose an option: ");
}

void displayAllBooks(BookList *list) {
    Book *current = list->head;
    while (current != NULL) {
        displayBook(current);
        current = current->next;
    }
}
//main: Hàm chính, điều khiển luồng thực thi của chương trình. 
//Dựa trên lựa chọn của người dùng, chương trình sẽ thực hiện các chức năng khác nhau như hiển thị sách, tìm kiếm sách, cập nhật sách, sắp xếp sách, và thoát chương trình.
int main() {
    BookList list;
    initBookList(&list);
    readBooksFromFile(&list, "data.txt");

    int choice;
    char id[13];
    do {
        displayMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                displayAllBooks(&list);
                break;
            case 2:
                printf("Enter book ID: ");
                scanf("%s", id);
                displayBook(findBookById(&list, id));
                break;
            case 3:
                printf("Enter book ID: ");
                scanf("%s", id);
                updateBookById(&list, id);
                break;
            case 4:
                radixSortBooksById(&list);
                printf("Books sorted by ID.\n");
                break;
            case 5:
                writeBooksToFile(&list, "data.txt");
                printf("Data saved to file. Exiting...\n");
                break;
            default:
                printf("Invalid option. Try again.\n");
        }
    } while (choice != 5);

    return 0;
}
