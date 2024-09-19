#define _CRT_SECURE_NO_WARNINGS
#include "diary.h"

int entryCount = 0;// Pravilo 7: Extern varijable

// Pravilo 8: Inline funkcije
inline void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// Pravilo 12: Zaštita parametara
void addEntry(DiaryEntry* entries, int* count) {
    if (entries == NULL || count == NULL) return;

    printf("Unesite naslov: ");
    fgets(entries[*count].title, sizeof(entries[*count].title), stdin);
    strtok(entries[*count].title, "\n");  // Ukloni novi red

    printf("Unesite sadrzaj: ");
    fgets(entries[*count].content, sizeof(entries[*count].content), stdin);
    strtok(entries[*count].content, "\n");

    printf("Unesite datum (dd-mm-yyyy): ");
    fgets(entries[*count].date, sizeof(entries[*count].date), stdin);
    strtok(entries[*count].date, "\n");

    int type;
    printf("Unesite tip (0 = PERSONAL, 1 = WORK, 2 = OTHER): ");
    scanf("%d", &type);
    clearBuffer();
    if (type < 0 || type > 2) {
        printf("Nevazeci tip. Postavljam na OTHER.\n");
        entries[*count].type = OTHER;
    }
    else {
        entries[*count].type = (EntryType)type;
    }

    (*count)++;
}

void viewEntries(DiaryEntry* entries, int count) {
    if (entries == NULL) return;

    for (int i = 0; i < count; i++) {
        printf("Unos %d:\n", i + 1);
        printf("Datum: %s\n", entries[i].date);
        printf("Naslov: %s\n", entries[i].title);
        printf("Sadrzaj: %s\n", entries[i].content);
        printf("Tip: %s\n", (entries[i].type == PERSONAL) ? "PERSONAL" : (entries[i].type == WORK) ? "WORK" : "OTHER");
        printf("\n");
    }
}

void deleteEntry(DiaryEntry* entries, int* count) {
    if (entries == NULL || count == NULL) return;

    int index;
    printf("Unesite broj unosa za brisanje: ");
    scanf("%d", &index);
    clearBuffer();

    if (index < 1 || index > *count) {
        printf("Nevazeci broj unosa.\n");
        return;
    }

    for (int i = index - 1; i < *count - 1; i++) {
        entries[i] = entries[i + 1];
    }

    (*count)--;
}

void saveEntries(DiaryEntry* entries, int count, const char* fileName) {
    if (entries == NULL || fileName == NULL) return;

    FILE* file = fopen(fileName, "w");
    if (!file) {
        perror("Greska prilikom otvaranja datoteke"); ////20. Upravljati s pogreškama, errno, perror(), strerror(), feof(), ferror()
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%s;%s;%s;%d\n", entries[i].date, entries[i].title, entries[i].content, entries[i].type);
    }

    fclose(file);
}

void loadEntries(DiaryEntry* entries, int* count, const char* fileName) {
    if (entries == NULL || count == NULL || fileName == NULL) return;

    FILE* file = fopen(fileName, "r");
    if (!file) {
        perror("Greska prilikom otvaranja datoteke");
        return;
    }

    *count = 0;
    while (fscanf(file, "%19[^;];%99[^;];%999[^;];%d\n", entries[*count].date, entries[*count].title, entries[*count].content, (int*)&entries[*count].type) != EOF) {
        // Ispis trenutne pozicije u datoteci pomoću ftell
        long position = ftell(file);
        if (position != -1L) {
            printf("Pozicija unosa %d u datoteci: %ld bajtova\n", *count + 1, position);
        }
        (*count)++;
    }

    fclose(file);

    // Sortiranje unosa po naslovu nakon učitavanja
    qsort(entries, *count, sizeof(DiaryEntry), compareEntries);
}

// Funkcija za usporedbu unosa po naslovu
int compareEntries(const void* a, const void* b) {
    const DiaryEntry* entryA = (const DiaryEntry*)a;
    const DiaryEntry* entryB = (const DiaryEntry*)b;
    return strcmp(entryA->title, entryB->title);
}

// Funkcija za pretraživanje unosa po naslovu
DiaryEntry* searchEntry(DiaryEntry* entries, int count, const char* title) {
    DiaryEntry key;
    strncpy(key.title, title, sizeof(key.title));
    return (DiaryEntry*)bsearch(&key, entries, count, sizeof(DiaryEntry), compareEntries);
}

void deleteFile(const char* fileName) {
    if (remove(fileName) == 0) {
        printf("Datoteka '%s' uspješno obrisana.\n", fileName);
    }
    else {
        perror("Greska prilikom brisanja datoteke");
    }
}

void renameFile(const char* oldName, const char* newName) {
    if (rename(oldName, newName) == 0) {
        printf("Datoteka '%s' uspješno preimenovana u '%s'.\n", oldName, newName);
    }
    else {
        perror("Greska prilikom preimenovanja datoteke");
    }
}

void copyFile(const char* sourceFileName, const char* destFileName) {
    FILE* source = fopen(sourceFileName, "rb");
    FILE* dest = fopen(destFileName, "wb");

    if (source == NULL || dest == NULL) {
        perror("Greska prilikom otvaranja datoteka");
        if (source) fclose(source);
        if (dest) fclose(dest);
        return;
    }

    char buffer[1024];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        fwrite(buffer, 1, bytesRead, dest);
    }

    fclose(source);
    fclose(dest);

    printf("Datoteka '%s' uspješno kopirana u '%s'.\n", sourceFileName, destFileName);
}

