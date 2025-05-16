#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// Constants for word limits and max wrong guesses
#define MAX_WORDS 100
#define MAX_LEN 100
#define MAX_TRIES 6

// === Function to Clear the Screen (Cross-platform) ===
void clear_screen() {
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear");  // Linux/macOS
    #endif
}

// === Function to Draw the Hangman Figure Based on Mistakes ===
void draw_hangman(int tries) {
    const char *stages[] = {
        "  +---+\n"
        "  |   |\n"
        "      |\n"
        "      |\n"
        "      |\n"
        "      |\n"
        "=========\n",

        "  +---+\n"
        "  |   |\n"
        "  O   |\n"
        "      |\n"
        "      |\n"
        "      |\n"
        "=========\n",

        "  +---+\n"
        "  |   |\n"
        "  O   |\n"
        "  |   |\n"
        "      |\n"
        "      |\n"
        "=========\n",

        "  +---+\n"
        "  |   |\n"
        "  O   |\n"
        " /|   |\n"
        "      |\n"
        "      |\n"
        "=========\n",

        "  +---+\n"
        "  |   |\n"
        "  O   |\n"
        " /|\\  |\n"
        "      |\n"
        "      |\n"
        "=========\n",

        "  +---+\n"
        "  |   |\n"
        "  O   |\n"
        " /|\\  |\n"
        " /    |\n"
        "      |\n"
        "=========\n",

        "  +---+\n"
        "  |   |\n"
        "  O   |\n"
        " /|\\  |\n"
        " / \\  |\n"
        "      |\n"
        "=========\n"
    };

    printf("%s", stages[tries]);
}

// === Function to Load Words from File into Array ===
int load_words(char words[][MAX_LEN], const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open %s\n", filename);
        return 0;
    }

    int count = 0;
    while (fgets(words[count], MAX_LEN, file)) {
        // Remove newline character from end
        words[count][strcspn(words[count], "\n")] = '\0';
        count++;
    }

    fclose(file);
    return count;
}

// === Function to Check a Letter Guess and Update the Display ===
int guess_letter(const char *word, char *display, char letter) {
    int found = 0;
    for (int i = 0; word[i]; i++) {
        if (word[i] == letter) {
            display[i] = letter;
            found = 1;
        }
    }
    return found;
}

// === Main Function (Game Logic) ===
int main() {
    char words[MAX_WORDS][MAX_LEN];

    // Load words from file
    int total = load_words(words, "words.txt");
    if (total == 0) return 1;  // Exit if loading failed

    // Pick a random word
    srand(time(NULL));
    const char *word = words[rand() % total];
    int len = strlen(word);
    char display[MAX_LEN];

    // Initialize display with underscores or spaces
    for (int i = 0; i < len; i++)
        display[i] = (word[i] == ' ') ? ' ' : '_';
    display[len] = '\0';

    int tries = 0;  // Count of incorrect guesses

    printf("=== Hangman Game ===\n");

    // === Game Loop ===
    while (tries < MAX_TRIES && strcmp(display, word) != 0) {
        clear_screen();                 // Clear screen each turn
        draw_hangman(tries);           // Draw hangman graphic
        printf("Word: %s\n", display); // Show current word progress
        printf("Tries left: %d\n", MAX_TRIES - tries);
        printf("Guess a letter: ");

        char ch;
        scanf(" %c", &ch);
        ch = tolower(ch);  // Normalize to lowercase

        if (!guess_letter(word, display, ch)) {
            printf("Wrong!\n");
            tries++;
        } else {
            printf("Correct!\n");
        }
    }

    // === End of Game ===
    clear_screen();
    draw_hangman(tries);

    if (strcmp(display, word) == 0) {
        printf("\n You won! The word was: %s\n", word);
    } else {
        printf("\n You lost! The word was: %s\n", word);
    }

    return 0;
}
