#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 100
#define MAX_SUBJECTS 5

typedef struct {
    char name[50];
    int id;
    int grades[MAX_SUBJECTS];
    float gpa;
} Student;

float calculateGPA(int grades[], int numSubjects) {
    int sum = 0;
    for (int i = 0; i < numSubjects; i++) {
        sum += grades[i];
    }
    return (float)sum / numSubjects;
}

void addStudent(Student students[], int *studentCount) {
    if (*studentCount >= MAX_STUDENTS) {
        printf("Cannot add more students.\n");
        return;
    }
    
    Student *newStudent = &students[*studentCount];
    printf("Enter student name: ");
    scanf("%s", newStudent->name);
    printf("Enter student ID: ");
    scanf("%d", &newStudent->id);
    
    for (int i = 0; i < MAX_SUBJECTS; i++) {
        printf("Enter grade for subject %d (A=4, B=3, C=2, D=1, F=0): ", i + 1);
        scanf("%d", &newStudent->grades[i]);
    }
    
    newStudent->gpa = calculateGPA(newStudent->grades, MAX_SUBJECTS);
    (*studentCount)++;
}

void updateGrade(Student students[], int studentCount) {
    int id, subject;
    printf("Enter student ID to update grade: ");
    scanf("%d", &id);
    
    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id) {
            printf("Enter the subject number to update (1-%d): ", MAX_SUBJECTS);
            scanf("%d", &subject);
            
            if (subject < 1 || subject > MAX_SUBJECTS) {
                printf("Invalid subject number.\n");
                return;
            }
            
            printf("Enter new grade for subject %d (A=4, B=3, C=2, D=1, F=0): ", subject);
            scanf("%d", &students[i].grades[subject - 1]);
            
            students[i].gpa = calculateGPA(students[i].grades, MAX_SUBJECTS);
            printf("Grade updated successfully.\n");
            return;
        }
    }
    printf("Student ID not found.\n");
}

void displayStudents(Student students[], int studentCount) {
    for (int i = 0; i < studentCount; i++) {
        printf("Name: %s, ID: %d, Grades: ", students[i].name, students[i].id);
        for (int j = 0; j < MAX_SUBJECTS; j++) {
            printf("%d ", students[i].grades[j]);
        }
        printf(", GPA: %.2f\n", students[i].gpa);
    }
}

void displayGPAStats(Student students[], int studentCount) {
    if (studentCount == 0) {
        printf("No students in the list.\n");
        return;
    }

    float highestGPA = students[0].gpa;
    float lowestGPA = students[0].gpa;
    float sumGPA = 0;

    for (int i = 0; i < studentCount; i++) {
        if (students[i].gpa > highestGPA) highestGPA = students[i].gpa;
        if (students[i].gpa < lowestGPA) lowestGPA = students[i].gpa;
        sumGPA += students[i].gpa;
    }

    float avgGPA = sumGPA / studentCount;

    printf("Highest GPA: %.2f\n", highestGPA);
    printf("Lowest GPA: %.2f\n", lowestGPA);
    printf("Average GPA: %.2f\n", avgGPA);
}

void listLowGPAStudents(Student students[], int studentCount, float threshold) {
    printf("Students with GPA below %.2f:\n", threshold);
    for (int i = 0; i < studentCount; i++) {
        if (students[i].gpa < threshold) {
            printf("Name: %s, GPA: %.2f\n", students[i].name, students[i].gpa);
        }
    }
}

int main() {
    Student students[MAX_STUDENTS];
    int studentCount = 0;
    int choice;

    while (1) {
        printf("\nMenu:\n");
        printf("1. Add Student\n");
        printf("2. Update Student Grade\n");
        printf("3. Display All Students\n");
        printf("4. Display GPA Stats\n");
        printf("5. List Students with GPA Below Threshold\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addStudent(students, &studentCount);
                break;
            case 2:
                updateGrade(students, studentCount);
                break;
            case 3:
                displayStudents(students, studentCount);
                break;
            case 4:
                displayGPAStats(students, studentCount);
                break;
            case 5: {
                float threshold;
                printf("Enter GPA threshold: ");
                scanf("%f", &threshold);
                listLowGPAStudents(students, studentCount, threshold);
                break;
            }
            case 6:
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}