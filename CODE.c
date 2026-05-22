/*
 * ============================================================
 *  Student Academic Performance & CGPA Tracker  v2
 *  1st Year C Mini Project
 * ============================================================
 *  FLOW:
 *   Phase 1 — Ask N, input all N students at once
 *   Phase 2 — Post-input menu (add/edit/search/rank/backlog)
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME     50
#define MAX_ROLL     15
#define MAX_SUBJECTS  8
#define BACKLOG_MIN  4.0f

/* ── Student node (linked list) ─────────────────────────── */
typedef struct Student {
    char  name[MAX_NAME];
    char  rollNo[MAX_ROLL];
    int   numSubjects;
    float marks[MAX_SUBJECTS];
    int   credits[MAX_SUBJECTS];
    float cgpa;
    struct Student *next;
} Student;

Student *head = NULL;   /* global head pointer */

/* ════════════════════════════════════════════════════════
 *  UTILITIES
 * ════════════════════════════════════════════════════════ */

void clearBuf(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Calculate & store CGPA using pointer (call-by-reference) */
void calcCGPA(Student *s) {
    float pts = 0;
    int   cr  = 0, i;
    for (i = 0; i < s->numSubjects; i++) {
        pts += s->marks[i] * s->credits[i];
        cr  += s->credits[i];
    }
    s->cgpa = cr ? (pts / cr) : 0.0f;
}

/* Append one node to end of linked list */
void appendNode(Student *node) {
    node->next = NULL;
    if (!head) { head = node; return; }
    Student *cur = head;
    while (cur->next) cur = cur->next;
    cur->next = node;
}

/* Print a divider */
void divider(void) {
    printf("  ----------------------------------------\n");
}

/* ════════════════════════════════════════════════════════
 *  INPUT: fill one student record (shared by Phase-1 & add)
 * ════════════════════════════════════════════════════════ */
void fillStudent(Student *s) {
    printf("  Name     : ");
    fgets(s->name, MAX_NAME, stdin);
    s->name[strcspn(s->name, "\n")] = '\0';

    printf("  Roll No  : ");
    fgets(s->rollNo, MAX_ROLL, stdin);
    s->rollNo[strcspn(s->rollNo, "\n")] = '\0';

    do {
        printf("  Subjects (1-%d): ", MAX_SUBJECTS);
        scanf("%d", &s->numSubjects);
        clearBuf();
    } while (s->numSubjects < 1 || s->numSubjects > MAX_SUBJECTS);

    int i;
    for (i = 0; i < s->numSubjects; i++) {
        printf("  Subject %d  Marks(0-10): ", i + 1);
        do { scanf("%f", &s->marks[i]); clearBuf(); }
        while (s->marks[i] < 0 || s->marks[i] > 10);

        printf("  Subject %d  Credits(1-5): ", i + 1);
        do { scanf("%d", &s->credits[i]); clearBuf(); }
        while (s->credits[i] < 1 || s->credits[i] > 5);
    }

    calcCGPA(s);
    printf("  => CGPA calculated: %.2f\n", s->cgpa);
}

/* ════════════════════════════════════════════════════════
 *  PHASE 1: Input N students
 * ════════════════════════════════════════════════════════ */
void inputNStudents(void) {
    int n, i;
    printf("\n  How many students to enter? ");
    scanf("%d", &n);
    clearBuf();

    if (n <= 0) { printf("  Nothing to input.\n"); return; }

    for (i = 0; i < n; i++) {
        printf("\n  -- Student %d of %d --\n", i + 1, n);
        Student *node = (Student *)malloc(sizeof(Student));
        if (!node) { printf("  [ERROR] malloc failed.\n"); return; }
        fillStudent(node);
        appendNode(node);
    }

    printf("\n  All %d students entered successfully.\n", n);
}

/* ════════════════════════════════════════════════════════
 *  MENU OPTION 1: Add more students
 * ════════════════════════════════════════════════════════ */
void menuAdd(void) {
    int n, i;
    printf("\n  How many students to add? ");
    scanf("%d", &n);
    clearBuf();

    for (i = 0; i < n; i++) {
        printf("\n  -- New Student %d --\n", i + 1);
        Student *node = (Student *)malloc(sizeof(Student));
        if (!node) { printf("  [ERROR] malloc failed.\n"); return; }
        fillStudent(node);
        appendNode(node);
    }
    printf("  %d student(s) added.\n", n);
}

/* ════════════════════════════════════════════════════════
 *  MENU OPTION 2: Display all students
 * ════════════════════════════════════════════════════════ */
void menuDisplay(void) {
    if (!head) { printf("\n  No students in system.\n"); return; }

    printf("\n");
    divider();
    printf("  %-4s  %-18s  %-12s  %6s\n", "No.", "Name", "Roll No", "CGPA");
    divider();

    Student *cur = head;
    int n = 1;
    while (cur) {
        printf("  %-4d  %-18s  %-12s  %5.2f\n",
               n++, cur->name, cur->rollNo, cur->cgpa);
        cur = cur->next;
    }
    divider();
    printf("  Total: %d student(s)\n", n - 1);
}

/* ════════════════════════════════════════════════════════
 *  MENU OPTION 3: Search by roll number
 * ════════════════════════════════════════════════════════ */
void menuSearch(void) {
    if (!head) { printf("\n  No students in system.\n"); return; }

    char q[MAX_ROLL];
    printf("\n  Enter Roll No: ");
    fgets(q, MAX_ROLL, stdin);
    q[strcspn(q, "\n")] = '\0';

    Student *cur = head;
    while (cur) {
        if (strcmp(cur->rollNo, q) == 0) {
            printf("\n");
            divider();
            printf("  Name     : %s\n",  cur->name);
            printf("  Roll No  : %s\n",  cur->rollNo);
            printf("  CGPA     : %.2f\n", cur->cgpa);
            printf("  Status   : %s\n",
                   cur->cgpa < BACKLOG_MIN ? "WARNING: backlog" : "Clear");
            printf("  Subjects : %d\n",  cur->numSubjects);

            int i;
            for (i = 0; i < cur->numSubjects; i++)
                printf("    Sub %d  Marks: %.1f  Credits: %d\n",
                       i+1, cur->marks[i], cur->credits[i]);
            divider();
            return;
        }
        cur = cur->next;
    }
    printf("  Roll No '%s' not found.\n", q);
}

/* ════════════════════════════════════════════════════════
 *  MENU OPTION 4: Edit / update marks of a student
 * ════════════════════════════════════════════════════════ */
void menuEdit(void) {
    if (!head) { printf("\n  No students to edit.\n"); return; }

    char q[MAX_ROLL];
    printf("\n  Enter Roll No to edit: ");
    fgets(q, MAX_ROLL, stdin);
    q[strcspn(q, "\n")] = '\0';

    /* Find student via pointer (call-by-ref style traversal) */
    Student *cur = head;
    while (cur) {
        if (strcmp(cur->rollNo, q) == 0) {
            printf("  Editing: %s\n", cur->name);

            int i;
            for (i = 0; i < cur->numSubjects; i++) {
                printf("  Subject %d — Current: %.1f  New (0-10): ",
                       i+1, cur->marks[i]);
                scanf("%f", &cur->marks[i]);
                clearBuf();
                if (cur->marks[i] < 0)  cur->marks[i] = 0;
                if (cur->marks[i] > 10) cur->marks[i] = 10;
            }

            calcCGPA(cur);   /* pointer updates cgpa directly */
            printf("  Updated CGPA: %.2f\n", cur->cgpa);
            return;
        }
        cur = cur->next;
    }
    printf("  Roll No '%s' not found.\n", q);
}

/* ════════════════════════════════════════════════════════
 *  MENU OPTION 5: Show class rankings / topper
 *  Uses bubble sort on a temporary pointer array
 * ════════════════════════════════════════════════════════ */
void menuToppers(void) {
    if (!head) { printf("\n  No students.\n"); return; }

    /* Count */
    int n = 0;
    Student *cur = head;
    while (cur) { n++; cur = cur->next; }

    /* Build pointer array */
    Student **arr = (Student **)malloc(n * sizeof(Student *));
    if (!arr) { printf("  [ERROR] malloc failed.\n"); return; }

    cur = head;
    int i;
    for (i = 0; i < n; i++, cur = cur->next)
        arr[i] = cur;

    /* Bubble sort descending by cgpa */
    int j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j]->cgpa < arr[j+1]->cgpa) {
                Student *tmp = arr[j];
                arr[j]       = arr[j+1];
                arr[j+1]     = tmp;
            }
        }
    }

    printf("\n");
    divider();
    printf("  %-5s  %-18s  %-12s  %6s  %s\n",
           "Rank", "Name", "Roll No", "CGPA", "");
    divider();

    for (i = 0; i < n; i++) {
        char tag[16] = "";
        if (i == 0) strcpy(tag, "<< TOPPER");

        printf("  %-5d  %-18s  %-12s  %5.2f  %s\n",
               i+1, arr[i]->name, arr[i]->rollNo, arr[i]->cgpa, tag);
    }
    divider();

    free(arr);
}

/* ════════════════════════════════════════════════════════
 *  MENU OPTION 6: Show backlog students (cgpa < threshold)
 * ════════════════════════════════════════════════════════ */
void menuBacklogs(void) {
    if (!head) { printf("\n  No students.\n"); return; }

    printf("\n  Students with CGPA < %.1f:\n", BACKLOG_MIN);
    divider();

    Student *cur = head;
    int found = 0;
    while (cur) {
        if (cur->cgpa < BACKLOG_MIN) {
            printf("  %-18s  Roll: %-12s  CGPA: %.2f  [BACKLOG]\n",
                   cur->name, cur->rollNo, cur->cgpa);
            found++;
        }
        cur = cur->next;
    }

    if (!found)
        printf("  No students in backlog. Everyone is clear!\n");
    else
        printf("\n  Total backlog: %d\n", found);

    divider();
}

/* ════════════════════════════════════════════════════════
 *  Free entire linked list
 * ════════════════════════════════════════════════════════ */
void freeAll(void) {
    Student *cur = head;
    while (cur) {
        Student *tmp = cur->next;
        free(cur);
        cur = tmp;
    }
    head = NULL;
}

/* ════════════════════════════════════════════════════════
 *  MAIN
 * ════════════════════════════════════════════════════════ */
int main(void) {
    printf("\n  =========================================\n");
    printf("   Student CGPA Tracker  |  C Mini Project\n");
    printf("  =========================================\n");

    /* ── PHASE 1: Bulk input ────────────────────────────── */
    inputNStudents();

    /* ── PHASE 2: Post-input menu ───────────────────────── */
    int choice;
    do {
        printf("\n  ========= MENU ===========================\n");
        printf("  1. Add more students\n");
        printf("  2. Display all students\n");
        printf("  3. Search student by roll number\n");
        printf("  4. Edit / update marks\n");
        printf("  5. Show class rankings & topper\n");
        printf("  6. Show backlog students\n");
        printf("  0. Exit\n");
        printf("  ==========================================\n");
        printf("  Choice: ");
        scanf("%d", &choice);
        clearBuf();

        switch (choice) {
            case 1: menuAdd();      break;
            case 2: menuDisplay();  break;
            case 3: menuSearch();   break;
            case 4: menuEdit();     break;
            case 5: menuToppers();  break;
            case 6: menuBacklogs(); break;
            case 0:
                printf("\n  Freeing memory. Goodbye!\n");
                freeAll();
                break;
            default:
                printf("  Invalid choice. Enter 0-6.\n");
        }

    } while (choice != 0);

    return 0;
}
