#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Attendance {
    char studentID[20];
    char date[15];
    int status;
};

void markAttendance(void) {
    FILE *fp = fopen("attendance.dat", "ab");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }
    
    int n;
    printf("Enter number of records to enter: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid input.\n");
        fclose(fp);
        return;
    }
    
    struct Attendance a;
    for (int i = 0; i < n; i++) {
        printf("Enter Student ID: ");
        scanf("%19s", a.studentID);
        printf("Enter Date (DD-MM-YYYY): ");
        scanf("%14s", a.date);
        printf("Enter Status (1 for Present, 0 for Absent): ");
        scanf("%d", &a.status);
        fwrite(&a, sizeof(struct Attendance), 1, fp);
    }
    fclose(fp);
    printf("Attendance recorded successfully.\n");
}

void viewPercentage(void) {
    FILE *fp = fopen("attendance.dat", "rb");
    if (!fp) {
        printf("No records found.\n");
        return;
    }
    
    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return;
    }
    long size = ftell(fp);
    if (size < 0) {
        fclose(fp);
        return;
    }
    rewind(fp);
    
    size_t count = (size_t)size / sizeof(struct Attendance);
    if (count == 0) {
        printf("No records found.\n");
        fclose(fp);
        return;
    }
    
    struct Attendance *records = (struct Attendance *)malloc((size_t)size);
    if (!records) {
        printf("Memory allocation failed!\n");
        fclose(fp);
        return;
    }
    
    if (fread(records, sizeof(struct Attendance), count, fp) != count) {
        free(records);
        fclose(fp);
        return;
    }
    fclose(fp);
    
    char targetID[20];
    printf("Enter Student ID to check percentage: ");
    scanf("%19s", targetID);
    
    int total = 0, present = 0;
    struct Attendance *ptr = records;
    for (size_t i = 0; i < count; i++) {
        if (strcmp(ptr->studentID, targetID) == 0) {
            total++;
            if (ptr->status == 1) {
                present++;
            }
        }
        ptr++;
    }
    
    if (total == 0) {
        printf("Student ID not found in records.\n");
    } else {
        float perc = ((float)present / (float)total) * 100.0f;
        printf("Total Lectures: %d, Attended: %d, Percentage: %.2f%%\n", total, present, perc);
    }
    
    free(records);
}

void viewDefaulters(void) {
    FILE *fp = fopen("attendance.dat", "rb");
    if (!fp) {
        printf("No records found.\n");
        return;
    }
    
    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return;
    }
    long size = ftell(fp);
    if (size < 0) {
        fclose(fp);
        return;
    }
    rewind(fp);
    
    size_t count = (size_t)size / sizeof(struct Attendance);
    if (count == 0) {
        fclose(fp);
        return;
    }
    
    struct Attendance *records = (struct Attendance *)malloc((size_t)size);
    if (!records) {
        fclose(fp);
        return;
    }
    
    if (fread(records, sizeof(struct Attendance), count, fp) != count) {
        free(records);
        fclose(fp);
        return;
    }
    fclose(fp);
    
    float threshold;
    printf("Enter attendance threshold percentage: ");
    scanf("%f", &threshold);
    
    char uniqueIDs[100][20];
    int uniqueCount = 0;
    
    for (size_t i = 0; i < count; i++) {
        int found = 0;
        for (int j = 0; j < uniqueCount; j++) {
            if (strcmp(uniqueIDs[j], records[i].studentID) == 0) {
                found = 1;
                break;
            }
        }
        if (!found && uniqueCount < 100) {
            strcpy(uniqueIDs[uniqueCount++], records[i].studentID);
        }
    }
    
    printf("\n--- Defaulters List (Below %.2f%%) ---\n", threshold);
    int defaulterFound = 0;
    
    for (int i = 0; i < uniqueCount; i++) {
        int total = 0, present = 0;
        struct Attendance *ptr = records;
        for (size_t j = 0; j < count; j++) {
            if (strcmp(ptr->studentID, uniqueIDs[i]) == 0) {
                total++;
                if (ptr->status == 1) {
                    present++;
                }
            }
            ptr++;
        }
        float perc = total > 0 ? ((float)present / (float)total) * 100.0f : 0.0f;
        if (perc < threshold) {
            printf("ID: %s | Attendance: %.2f%%\n", uniqueIDs[i], perc);
            defaulterFound = 1;
        }
    }
    
    if (!defaulterFound) {
        printf("No defaulters found.\n");
    }
    
    free(records);
}

void generateAnalytics(void) {
    FILE *fp = fopen("attendance.dat", "rb");
    if (!fp) {
        printf("No records found.\n");
        return;
    }
    
    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return;
    }
    long size = ftell(fp);
    if (size < 0) {
        fclose(fp);
        return;
    }
    rewind(fp);
    
    size_t count = (size_t)size / sizeof(struct Attendance);
    if (count == 0) {
        fclose(fp);
        return;
    }
    
    struct Attendance *records = (struct Attendance *)malloc((size_t)size);
    if (!records) {
        fclose(fp);
        return;
    }
    
    if (fread(records, sizeof(struct Attendance), count, fp) != count) {
        free(records);
        fclose(fp);
        return;
    }
    fclose(fp);
    
    char dates[100][15];
    int dateCount = 0;
    
    for (size_t i = 0; i < count; i++) {
        int found = 0;
        for (int j = 0; j < dateCount; j++) {
            if (strcmp(dates[j], records[i].date) == 0) {
                found = 1;
                break;
            }
        }
        if (!found && dateCount < 100) {
            strcpy(dates[dateCount++], records[i].date);
        }
    }
    
    int totalPresentAll = 0;
    struct Attendance *ptr = records;
    for (size_t i = 0; i < count; i++) {
        if (ptr->status == 1) {
            totalPresentAll++;
        }
        ptr++;
    }
    
    float overallAvg = ((float)totalPresentAll / (float)count) * 100.0f;
    printf("\n--- Class Analytics ---\n");
    printf("Overall Class Average Attendance: %.2f%%\n\n", overallAvg);
    printf("--- Lecture-wise Breakdown ---\n");
    
    for (int i = 0; i < dateCount; i++) {
        int lectureTotal = 0, lecturePresent = 0;
        ptr = records;
        for (size_t j = 0; j < count; j++) {
            if (strcmp(ptr->date, dates[i]) == 0) {
                lectureTotal++;
                if (ptr->status == 1) {
                    lecturePresent++;
                }
            }
            ptr++;
        }
        float lecPerc = lectureTotal > 0 ? ((float)lecturePresent / (float)lectureTotal) * 100.0f : 0.0f;
        printf("Date: %s | Total: %d | Present: %d | Attendance: %.2f%%\n", dates[i], lectureTotal, lecturePresent, lecPerc);
    }
    
    free(records);
}

int main(void) {
    int choice;
    while (1) {
        printf("\n=== Attendance & Analytics System ===\n");
        printf("1. Mark Attendance\n");
        printf("2. View Student Attendance Percentage\n");
        printf("3. Identify Defaulters\n");
        printf("4. Generate Analytics\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            continue;
        }
        
        switch (choice) {
            case 1:
                markAttendance();
                break;
            case 2:
                viewPercentage();
                break;
            case 3:
                viewDefaulters();
                break;
            case 4:
                generateAnalytics();
                break;
            case 5:
                return 0;
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}