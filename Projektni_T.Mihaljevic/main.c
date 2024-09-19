#define _CRT_SECURE_NO_WARNINGS
#include "diary.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_ENTRIES 100  // Pravilo 13: Statički alocirano polje

int main() {
    // Pravilo 14 i 15: Dinamičko zauzimanje memorije i korištenje malloc()
    DiaryEntry* entries = malloc(MAX_ENTRIES * sizeof(DiaryEntry));
    if (entries == NULL) {
        fprintf(stderr, "Memory allocation failed\n");  // Pravilo 20: Upravljanje pogreškama s perror() ili fprintf()
        return 1;
    }

    int entryCount = 0;  // Pravilo 1: Cjelobrojni tip podataka
    loadEntries(entries, &entryCount, diaryFileName);  // Pravilo 17: Datoteke - rad s datotekama

    char choice;
    while (1) {  // Pravilo 9: Implementacija izbornika
        printf("Osobni Dnevnik\n");
        printf("a. Dodaj unos\n");
        printf("b. Pregledaj unose\n");
        printf("c. Izbrisi unos\n");
        printf("d. Spremi unose\n");
        printf("e. Ucitaj unose\n");
        printf("f. Pretrazi unose\n");
        printf("g. Izlaz\n");
        printf("h. Izbrisi datoteku\n");
        printf("i. Preimenuj datoteku\n");
        printf("j. Kopiraj datoteku\n");
        printf("Odabir: ");
        scanf(" %c", &choice);
        clearBuffer();  // Pravilo 8: Inline funkcija

        switch (choice) {
        case 'a':
            addEntry(entries, &entryCount);  // Pravilo 10 i 11: Upotreba pokazivača i struktura u funkcijama
            break;
        case 'b':
            viewEntries(entries, entryCount);
            break;
        case 'c':
            deleteEntry(entries, &entryCount);
            break;
        case 'd':
            saveEntries(entries, entryCount, diaryFileName);
            break;
        case 'e':
            loadEntries(entries, &entryCount, diaryFileName);
            break;
        case 'f': {
            char title[100];
            printf("Unesite naslov za pretragu: ");
            fgets(title, sizeof(title), stdin);
            strtok(title, "\n");  // Ukloni novi red
            DiaryEntry* entry = searchEntry(entries, entryCount, title);  // Pravilo 22: Pretraživanje s bsearch()
            if (entry) {
                printf("Unos pronađen:\n");
                printf("Datum: %s\n", entry->date);
                printf("Naslov: %s\n", entry->title);
                printf("Sadrzaj: %s\n", entry->content);
                printf("Tip: %s\n", (entry->type == PERSONAL) ? "PERSONAL" : (entry->type == WORK) ? "WORK" : "OTHER");
            }
            else {
                printf("Unos s naslovom '%s' nije pronađen.\n", title);
            }
            break;
        }
        case 'g':
            saveEntries(entries, entryCount, diaryFileName);
            free(entries);  // Pravilo 16: Oslobađanje memorije
            exit(0);
        case 'h':
            deleteFile(diaryFileName);  // Pravilo 19: Funkcija remove() za brisanje datoteka
            break;
        case 'i': {
            char newFileName[100];
            printf("Unesite novo ime datoteke (mora završiti sa .txt): ");
            fgets(newFileName, sizeof(newFileName), stdin);
            strtok(newFileName, "\n");
            renameFile(diaryFileName, newFileName);  // Pravilo 19: Funkcija rename() za preimenovanje datoteka
            break;
        }
        case 'j': {
            char destFileName[100];
            printf("Unesite ime nove datoteke (mora završiti sa .txt): ");
            fgets(destFileName, sizeof(destFileName), stdin);
            strtok(destFileName, "\n");
            copyFile(diaryFileName, destFileName);  // Pravilo 19: Funkcija za kopiranje datoteka
            break;
        }
        default:
            printf("Nevažeći odabir. Pokušajte ponovno.\n");
        }
    }

    free(entries);  // Pravilo 15: Korištenje free() za dinamički alociranu memoriju
    return 0;
}
