#pragma once
#define _CRT_SECURE_NO_WARNINGS
#ifndef DIARY_H
#define DIARY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Pravilo 2 i 3: Složeni tipovi podataka i typedef
// Ovdje definiramo enum i struct koji predstavljaju složene tipove podataka.
// Također koristimo typedef kako bismo pojednostavili korištenje ovih tipova u kodu.
typedef enum {
    PERSONAL,
    WORK,
    OTHER
} EntryType;

typedef struct {
    char title[100];  // Pravilo 13: Statički alocirana polja
    char content[1000];
    char date[20];
    EntryType type;   // Pravilo 2: Konkretni primitivni tipovi i enum
} DiaryEntry;

// Pravilo 5: Static varijable
static const char* diaryFileName = "diary.txt";

// Pravilo 7: Extern varijable
extern int entryCount;

// Pravilo 6: Organizacija izvornog kôda - Header file deklarira sve funkcije koje će biti definirane u `diary.c`
void addEntry(DiaryEntry* entries, int* count);
void viewEntries(DiaryEntry* entries, int count);
void deleteEntry(DiaryEntry* entries, int* count);
void saveEntries(DiaryEntry* entries, int count, const char* fileName);
void loadEntries(DiaryEntry* entries, int* count, const char* fileName);
int compareEntries(const void* a, const void* b);
DiaryEntry* searchEntry(DiaryEntry* entries, int count, const char* title);
void deleteFile(const char* fileName);
void renameFile(const char* oldName, const char* newName);
void copyFile(const char* sourceFileName, const char* destFileName);

#endif // DIARY_H
