#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "patients.dat"
#define MAX_NAME_LEN 100
#define MAX_DIAGNOSIS_LEN 100
#define MAX_CONTACT_LEN 50

// ------------------ STRUCT DEFINITION ------------------
typedef struct {
    int pid;
    char name[MAX_NAME_LEN];
    int age;
    char gender[10];
    char doctor[50];
    char symptoms[200];
    char lab_tests[200];
    char diagnosis[MAX_DIAGNOSIS_LEN];
    char medication[200];
    char contact[MAX_CONTACT_LEN];
} Patient;

// ------------------ FUNCTION DECLARATIONS ------------------
void addPatient();
void viewPatients();
void searchPatient();
void updatePatient();
void deletePatient();
int pidExists(int pid);
void savePatient(Patient s);
void loadPatients(Patient Patients[], int *count);
void writeAllPatients(Patient Patients[], int count);

// ------------------ MAIN MENU ------------------
int main() {
    int choice;
    while (1) {
        printf("\n==============================\n");
        printf(" Patient MANAGEMENT SYSTEM\n");
        printf("==============================\n");
        printf("1. Add New Patient\n");
        printf("2. View All Patients\n");
        printf("3. Search Patient\n");
        printf("4. Update Patient\n");
        printf("5. Delete Patient\n");
        printf("6. Exit\n");
        printf("------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // clear newline

        switch (choice) {
            case 1: addPatient(); break;
            case 2: viewPatients(); break;
            case 3: searchPatient(); break;
            case 4: updatePatient(); break;
            case 5: deletePatient(); break;
            case 6: printf("Exiting program. Goodbye!\n"); exit(0);
            default: printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}

// ------------------ ADD Patient ------------------
void addPatient() {
    Patient s;
    printf("\n--- Add New Patient ---\n");
    printf("Enter Patient PID: ");
    scanf("%d", &s.pid);
    getchar();

    if (pidExists(s.pid)) {
        printf("Error: Patient PID already exists!\n");
        return;
    }

    printf("Enter Full Name: ");
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = '\0';

    printf("Enter Age: ");
    scanf("%d", &s.age);
    getchar();

    printf("Enter Gender: ");
    fgets(s.gender, sizeof(s.gender), stdin);
    s.gender[strcspn(s.gender, "\n")] = '\0';

    printf("Enter Doctor's Name: ");
    fgets(s.doctor, sizeof(s.doctor), stdin);
    s.doctor[strcspn(s.doctor, "\n")] = '\0';

    printf("Enter Symptoms: ");
    fgets(s.symptoms, sizeof(s.symptoms), stdin);
    s.symptoms[strcspn(s.symptoms, "\n")] = '\0';

    printf("Enter Lab Tests: ");
    fgets(s.lab_tests, sizeof(s.lab_tests), stdin);
    s.lab_tests[strcspn(s.lab_tests, "\n")] = '\0';

    printf("Enter Diagnosis: ");
    fgets(s.diagnosis, sizeof(s.diagnosis), stdin);
    s.diagnosis[strcspn(s.diagnosis, "\n")] = '\0';

    printf("Enter Medication: ");
    fgets(s.medication, sizeof(s.medication), stdin);
    s.medication[strcspn(s.medication, "\n")] = '\0';

    printf("Enter Contact Info: ");
    fgets(s.contact, sizeof(s.contact), stdin);
    s.contact[strcspn(s.contact, "\n")] = '\0';

    savePatient(s);
    printf("Patient added successfully!\n");
}

// ------------------ SAVE Patient TO FILE ------------------
void savePatient(Patient s) {
    FILE *fp = fopen(FILE_NAME, "ab");
    if (!fp) {
        perror("Error opening file");
        return;
    }
    fwrite(&s, sizeof(Patient), 1, fp);
    fclose(fp);
}

// ------------------ CHECK IF PID EXISTS ------------------
int pidExists(int pid) {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) return 0;

    Patient s;
    while (fread(&s, sizeof(Patient), 1, fp)) {
        if (s.pid == pid) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// ------------------ VIEW ALL Patients ------------------
void viewPatients() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    Patient s;
    printf("\n%-5s %-15s %-5s %-10s %-20s %-20s %-20s %-20s %-20s %-15s\n",
           "PID", "Name", "Age", "Gender", "Doctor", "Symptoms", "Lab Tests", "Diagnosis", "Medication", "Contact");
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    while (fread(&s, sizeof(Patient), 1, fp)) {
        printf("%-5d %-15s %-5d %-10s %-20s %-20s %-20s %-20s %-20s %-15s\n",
               s.pid, s.name, s.age, s.gender, s.doctor, s.symptoms, s.lab_tests, s.diagnosis, s.medication, s.contact);
    }
    fclose(fp);
}

// ------------------ SEARCH Patient ------------------
void searchPatient() {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        printf("No records found.\n");
        return;
    }

    int pid, found = 0;
    Patient s;

    printf("\nEnter Patient PID to search: ");
    scanf("%d", &pid);
    getchar();

    while (fread(&s, sizeof(Patient), 1, fp)) {
        if (s.pid == pid) {
            printf("\nPatient Found:\n");
            printf("ID: %d\nName: %s\nAge: %d\nGender: %s\nDoctor: %s\n", s.pid, s.name, s.age, s.gender, s.doctor);
            printf("Symptoms: %s\nLab Tests: %s\nDiagnosis: %s\n", s.symptoms, s.lab_tests, s.diagnosis);
            printf("Medication: %s\nContact: %s\n", s.medication, s.contact);
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found) {
        printf("No Patient found with PID %d.\n", pid);
    }
}

// ------------------ UPDATE Patient ------------------
void updatePatient() {
    Patient Patients[100];
    int count = 0, pid, found = 0;

    loadPatients(Patients, &count);

    printf("\nEnter Patient PID to update: ");
    scanf("%d", &pid);
    getchar();

    for (int i = 0; i < count; i++) {
        if (Patients[i].pid == pid) {
            found = 1;
            printf("Enter new Contact Info: ");
            fgets(Patients[i].contact, sizeof(Patients[i].contact), stdin);
            Patients[i].contact[strcspn(Patients[i].contact, "\n")] = '\0';

            printf("Enter new Doctor: ");
            fgets(Patients[i].doctor, sizeof(Patients[i].doctor), stdin);
            Patients[i].doctor[strcspn(Patients[i].doctor, "\n")] = '\0';

            printf("Enter new Symptoms: ");
            fgets(Patients[i].symptoms, sizeof(Patients[i].symptoms), stdin);
            Patients[i].symptoms[strcspn(Patients[i].symptoms, "\n")] = '\0';

            printf("Enter new Lab Tests: ");
            fgets(Patients[i].lab_tests, sizeof(Patients[i].lab_tests), stdin);
            Patients[i].lab_tests[strcspn(Patients[i].lab_tests, "\n")] = '\0';

            printf("Enter new Diagnosis: ");
            fgets(Patients[i].diagnosis, sizeof(Patients[i].diagnosis), stdin);
            Patients[i].diagnosis[strcspn(Patients[i].diagnosis, "\n")] = '\0';

            printf("Enter new Medication: ");
            fgets(Patients[i].medication, sizeof(Patients[i].medication), stdin);
            Patients[i].medication[strcspn(Patients[i].medication, "\n")] = '\0';
            break;
        }
    }

    if (found) {
        writeAllPatients(Patients, count);
        printf("Record updated successfully.\n");
    } else {
        printf("Patient PID not found.\n");
    }
}

// ------------------ DELETE Patient ------------------
void deletePatient() {
    Patient Patients[100];
    int count = 0, pid, found = 0;

    loadPatients(Patients, &count);

    printf("\nEnter Patient PID to delete: ");
    scanf("%d", &pid);
    getchar();

    for (int i = 0; i < count; i++) {
        if (Patients[i].pid == pid) {
            found = 1;
            printf("Are you sure you want to delete %s (ID %d)? (y/n): ", Patients[i].name, pid);
            char confirm = getchar();
            getchar();
            if (confirm == 'y' || confirm == 'Y') {
                for (int j = i; j < count - 1; j++)
                    Patients[j] = Patients[j + 1];
                count--;
                writeAllPatients(Patients, count);
                printf("Record deleted successfully.\n");
            } else {
                printf("Deletion cancelled.\n");
            }
            break;
        }
    }

    if (!found)
        printf("Patient PID not found.\n");
}

// ------------------ LOAD & WRITE ALL Patients ------------------
void loadPatients(Patient Patients[], int *count) {
    FILE *fp = fopen(FILE_NAME, "rb");
    if (!fp) {
        *count = 0;
        return;
    }
    *count = 0;
    while (fread(&Patients[*count], sizeof(Patient), 1, fp)) {
        (*count)++;
    }
    fclose(fp);
}

void writeAllPatients(Patient Patients[], int count) {
    FILE *fp = fopen(FILE_NAME, "wb");
    if (!fp) {
        perror("Error opening file");
        return;
    }
    fwrite(Patients, sizeof(Patient), count, fp);
    fclose(fp);
}   