#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char hoten[100];
    int namsinh;
    char lop[50];
    char diachi[150];
    char nganh[100];
} SinhVien;

typedef struct Node {
    SinhVien data;
    struct Node *prev, *next;
} Node;

typedef struct {
    Node *head, *tail;
} DoublyList;

void initList(DoublyList *l) {
    l->head = l->tail = NULL;
}

Node* createNode(SinhVien sv) {
    Node *p = (Node*)malloc(sizeof(Node));
    if (!p) { printf("Khong du bo nho\n"); exit(1); }
    p->data = sv;
    p->prev = p->next = NULL;
    return p;
}

void addLast(DoublyList *l, SinhVien sv) {
    Node *p = createNode(sv);
    if (l->head == NULL) {
        l->head = l->tail = p;
    } else {
        l->tail->next = p;
        p->prev = l->tail;
        l->tail = p;
    }
}

void freeList(DoublyList *l) {
    Node *p = l->head;
    while (p) {
        Node *q = p->next;
        free(p);
        p = q;
    }
    l->head = l->tail = NULL;
}

/* ---------- In danh sach ---------- */
void printList(DoublyList l) {
    Node *p = l.head;
    int i = 1;
    printf("\n===== DANH SACH SINH VIEN =====\n");
    while (p) {
        printf("\nSTT: %d\n", i++);
        printf("Ho ten  : %s\n", p->data.hoten);
        printf("Nam sinh: %d\n", p->data.namsinh);
        printf("Lop     : %s\n", p->data.lop);
        printf("Dia chi : %s\n", p->data.diachi);
        printf("Nganh   : %s\n", p->data.nganh);
        p = p->next;
    }
}

int cmpHoTen(SinhVien a, SinhVien b) {
    return strcmp(a.hoten, b.hoten);
}
int cmpNamSinh(SinhVien a, SinhVien b) {
    return a.namsinh - b.namsinh;
}
int cmpLop(SinhVien a, SinhVien b) {
    return strcmp(a.lop, b.lop);
}

void trim_newline(char *s) {
    size_t n = strlen(s);
    if (n>0 && s[n-1]=='\n') s[n-1] = '\0';
    if (n>1 && s[n-2]=='\r') s[n-2] = '\0';
}

void DuLieu(DoublyList *l, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Khong the mo file %s\n", filename);
        return;
    }
    char line[512];
    while (fgets(line, sizeof(line), f)) {
        trim_newline(line);
        if (strlen(line) == 0) continue;
        SinhVien sv;
        char *p = NULL;
        p = strtok(line, "|");
        if (!p) continue;
        strncpy(sv.hoten, p, sizeof(sv.hoten)-1); sv.hoten[sizeof(sv.hoten)-1]=0;

        p = strtok(NULL, "|");
        if (!p) continue;
        sv.namsinh = atoi(p);

        p = strtok(NULL, "|");
        if (!p) continue;
        strncpy(sv.lop, p, sizeof(sv.lop)-1); sv.lop[sizeof(sv.lop)-1]=0;

        p = strtok(NULL, "|");
        if (!p) continue;
        strncpy(sv.diachi, p, sizeof(sv.diachi)-1); sv.diachi[sizeof(sv.diachi)-1]=0;

        p = strtok(NULL, "|");
        if (!p) continue;
        strncpy(sv.nganh, p, sizeof(sv.nganh)-1); sv.nganh[sizeof(sv.nganh)-1]=0;

        addLast(l, sv);
    }
    fclose(f);
    printf("Da nap du lieu tu file %s\n", filename);
}

/* ---------- BUBBLE SORT ---------- */
void bubbleSort(DoublyList *l, int (*cmp)(SinhVien, SinhVien)) {
    if (!l->head) return;
    int swapped;
    Node *last = NULL;
    do {
        swapped = 0;
        Node *p = l->head;
        while (p->next != last) {
            if (cmp(p->data, p->next->data) > 0) {
                SinhVien tmp = p->data;
                p->data = p->next->data;
                p->next->data = tmp;
                swapped = 1;
            }
            p = p->next;
        }
        last = p;
    } while (swapped);
}

/* ---------- SELECTION SORT ---------- */
void selectionSort(DoublyList *l, int (*cmp)(SinhVien, SinhVien)) {
    for (Node *p = l->head; p != NULL; p = p->next) {
        Node *minNode = p;
        for (Node *q = p->next; q != NULL; q = q->next)
            if (cmp(q->data, minNode->data) < 0)
                minNode = q;
        if (minNode != p) {
            SinhVien tmp = p->data;
            p->data = minNode->data;
            minNode->data = tmp;
        }
    }
}

/* ---------- INSERTION SORT (dung data-shift) ---------- */
void insertionSort(DoublyList *l, int (*cmp)(SinhVien, SinhVien)) {
    if (!l->head) return;
    Node *p = l->head->next;
    while (p) {
        SinhVien key = p->data;
        Node *q = p->prev;
        while (q && cmp(q->data, key) > 0) {
            q->next->data = q->data;
            q = q->prev;
        }
        if (!q) l->head->data = key;
        else q->next->data = key;
        p = p->next;
    }
}

/* ---------- MERGE SORT (tren danh sach, dung cmp) ---------- */
Node* splitList(Node *head) {
    Node *fast = head, *slow = head;
    while (fast->next && fast->next->next) {
        fast = fast->next->next;
        slow = slow->next;
    }
    Node *second = slow->next;
    slow->next = NULL;
    if (second) second->prev = NULL;
    return second;
}

Node* mergeList(Node *a, Node *b, int (*cmp)(SinhVien, SinhVien)) {
    if (!a) return b;
    if (!b) return a;
    if (cmp(a->data, b->data) <= 0) {
        a->next = mergeList(a->next, b, cmp);
        if (a->next) a->next->prev = a;
        return a;
    } else {
        b->next = mergeList(a, b->next, cmp);
        if (b->next) b->next->prev = b;
        return b;
    }
}

Node* mergeSortInternal(Node *head, int (*cmp)(SinhVien, SinhVien)) {
    if (!head || !head->next) return head;
    Node *second = splitList(head);
    head = mergeSortInternal(head, cmp);
    second = mergeSortInternal(second, cmp);
    return mergeList(head, second, cmp);
}

void mergeSort(DoublyList *l, int (*cmp)(SinhVien, SinhVien)) {
    if (!l->head) return;
    l->head = mergeSortInternal(l->head, cmp);
    Node *p = l->head;
    while (p->next) p = p->next;
    l->tail = p;
}

/* ---------- QUICK SORT (danh sach bang hoan doi du lieu) ---------- */
Node* partition(Node *low, Node *high, int (*cmp)(SinhVien, SinhVien)) {
    SinhVien pivot = high->data;
    Node *i = low->prev;
    for (Node *j = low; j != high; j = j->next) {
        if (cmp(j->data, pivot) < 0) {
            i = (i == NULL) ? low : i->next;
            SinhVien tmp = i->data;
            i->data = j->data;
            j->data = tmp;
        }
    }
    i = (i == NULL) ? low : i->next;
    SinhVien tmp = i->data;
    i->data = high->data;
    high->data = tmp;
    return i;
}

void quickSortInternal(Node *low, Node *high, int (*cmp)(SinhVien, SinhVien)) {
    if (high && low != high && low != high->next) {
        Node *p = partition(low, high, cmp);
        quickSortInternal(low, p->prev, cmp);
        quickSortInternal(p->next, high, cmp);
    }
}

void quickSort(DoublyList *l, int (*cmp)(SinhVien, SinhVien)) {
    if (!l->head) return;
    quickSortInternal(l->head, l->tail, cmp);
}

void ThuatToan() {
    printf("\n--- Chon thuat toan sap xep ---\n");
    printf("1. Bubble Sort\n");
    printf("2. Selection Sort\n");
    printf("3. Insertion Sort\n");
    printf("4. Merge Sort\n");
    printf("5. Quick Sort\n");
    printf("0. Thoat\n");
    printf("Chon: ");
}

void TieuChi() {
    printf("\n--- Chon tieu chi sap xep ---\n");
    printf("1. Theo ho ten (alphabet)\n");
    printf("2. Theo nam sinh (tang dan)\n");
    printf("3. Theo lop (alphabet)\n");
    printf("Chon: ");
}

int main() {
    DoublyList l;
    initList(&l); //Khoi tao Danh Sach rong

    DuLieu(&l, "sinhvien.txt"); //Doc du lieu
    printList(l);  //in ra

    int alg = -1;
    do {
        ThuatToan();
        if (scanf("%d", &alg) != 1)
		{ 
			printf("Nhap sai\n"); return 0; 
		}
        if (alg == 0) break;
        
		if (alg < 1 || alg > 5) 
		{
        	printf("Lua chon thuat toan KHONG hop le! Vui long chon lai.\n");
        	continue;
    	}
    	
        TieuChi();
        int crit;
        if (scanf("%d", &crit) != 1) 
		{ 
			printf("Nhap sai\n"); return 0; 
		}

        int (*cmp)(SinhVien, SinhVien) = NULL;
        if (crit == 1) cmp = cmpHoTen;
        else if (crit == 2) cmp = cmpNamSinh;
        else if (crit == 3) cmp = cmpLop;
        else { printf("Tieu chi khong hop le\n"); continue; }

        // Thuc hien sap xep theo lua chon
        switch (alg) {
            case 1:
                bubbleSort(&l, cmp);
                printf("\n>>> Da sap xep bang Bubble Sort\n");
                break;
            case 2:
                selectionSort(&l, cmp);
                printf("\n>>> Da sap xep bang Selection Sort\n");
                break;
            case 3:
                insertionSort(&l, cmp);
                printf("\n>>> Da sap xep bang Insertion Sort\n");
                break;
            case 4:
                mergeSort(&l, cmp);
                printf("\n>>> Da sap xep bang Merge Sort\n");
                break;
            case 5:
                quickSort(&l, cmp);
                printf("\n>>> Da sap xep bang Quick Sort\n");
                break;
            default:
                printf("Lua chon thuat toan khong hop le!\n");
        }

        printList(l);
    }
    while (alg != 0);

    freeList(&l);
    return 0;
}

