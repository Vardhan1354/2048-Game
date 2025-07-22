
 #define VERSION "1.0.3"

 #include <stdio.h>        // defines: printf, puts, getchar
 #include <stdlib.h>       // defines: EXIT_SUCCESS
 #include <string.h>       // defines: strcmp
 #include <stdbool.h>      // defines: true, false
 #include <stdint.h>       // defines: uint8_t, uint32_t
 #include <time.h>         // defines: time
 #include <conio.h>        // Windows console I/O
 #include <windows.h>      // Windows API for console handling
 
 #define SIZE 4
 #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
 // Windows console color codes
 #define WIN_COLOR_BLACK 0
 #define WIN_COLOR_BLUE 1
 #define WIN_COLOR_GREEN 2
 #define WIN_COLOR_CYAN 3
 #define WIN_COLOR_RED 4
 #define WIN_COLOR_MAGENTA 5
 #define WIN_COLOR_YELLOW 6
 #define WIN_COLOR_WHITE 7
 #define WIN_COLOR_GRAY 8
 #define WIN_COLOR_BRIGHT_BLUE 9
 #define WIN_COLOR_BRIGHT_GREEN 10
 #define WIN_COLOR_BRIGHT_CYAN 11
 #define WIN_COLOR_BRIGHT_RED 12
 #define WIN_COLOR_BRIGHT_MAGENTA 13
 #define WIN_COLOR_BRIGHT_YELLOW 14
 #define WIN_COLOR_BRIGHT_WHITE 15
 
 void setConsoleColor(int foreground, int background) {
	 HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	 SetConsoleTextAttribute(hConsole, (background << 4) | foreground);
 }
 
 void resetConsoleColor() {
	 setConsoleColor(WIN_COLOR_WHITE, WIN_COLOR_BLACK);
 }
 
 void getColors(uint8_t value, uint8_t scheme, uint8_t *foreground, uint8_t *background) {
	 // Windows color scheme mapping
	 uint8_t original[] = {WIN_COLOR_GRAY, WIN_COLOR_WHITE, 
						  WIN_COLOR_BLUE, WIN_COLOR_WHITE, 
						  WIN_COLOR_GREEN, WIN_COLOR_WHITE, 
						  WIN_COLOR_CYAN, WIN_COLOR_WHITE, 
						  WIN_COLOR_RED, WIN_COLOR_WHITE, 
						  WIN_COLOR_MAGENTA, WIN_COLOR_WHITE, 
						  WIN_COLOR_YELLOW, WIN_COLOR_WHITE, 
						  WIN_COLOR_BRIGHT_WHITE, WIN_COLOR_WHITE, 
						  WIN_COLOR_BRIGHT_BLUE, WIN_COLOR_BLACK, 
						  WIN_COLOR_BRIGHT_GREEN, WIN_COLOR_BLACK, 
						  WIN_COLOR_BRIGHT_CYAN, WIN_COLOR_BLACK, 
						  WIN_COLOR_BRIGHT_RED, WIN_COLOR_BLACK, 
						  WIN_COLOR_BRIGHT_MAGENTA, WIN_COLOR_BLACK, 
						  WIN_COLOR_BRIGHT_YELLOW, WIN_COLOR_BLACK, 
						  WIN_COLOR_WHITE, WIN_COLOR_BLACK, 
						  WIN_COLOR_WHITE, WIN_COLOR_BLACK};
	 
	 uint8_t blackwhite[] = {WIN_COLOR_BLACK, WIN_COLOR_WHITE, 
							WIN_COLOR_GRAY, WIN_COLOR_WHITE, 
							WIN_COLOR_GRAY, WIN_COLOR_WHITE, 
							WIN_COLOR_GRAY, WIN_COLOR_WHITE, 
							WIN_COLOR_GRAY, WIN_COLOR_WHITE, 
							WIN_COLOR_GRAY, WIN_COLOR_WHITE, 
							WIN_COLOR_GRAY, WIN_COLOR_WHITE, 
							WIN_COLOR_GRAY, WIN_COLOR_BLACK, 
							WIN_COLOR_GRAY, WIN_COLOR_BLACK, 
							WIN_COLOR_GRAY, WIN_COLOR_BLACK, 
							WIN_COLOR_GRAY, WIN_COLOR_BLACK, 
							WIN_COLOR_GRAY, WIN_COLOR_BLACK, 
							WIN_COLOR_WHITE, WIN_COLOR_BLACK, 
							WIN_COLOR_WHITE, WIN_COLOR_BLACK, 
							WIN_COLOR_WHITE, WIN_COLOR_BLACK, 
							WIN_COLOR_WHITE, WIN_COLOR_BLACK};
	 
	 uint8_t bluered[] = {WIN_COLOR_BLACK, WIN_COLOR_WHITE, 
						 WIN_COLOR_BRIGHT_CYAN, WIN_COLOR_WHITE, 
						 WIN_COLOR_CYAN, WIN_COLOR_WHITE, 
						 WIN_COLOR_BRIGHT_BLUE, WIN_COLOR_WHITE, 
						 WIN_COLOR_BLUE, WIN_COLOR_WHITE, 
						 WIN_COLOR_BRIGHT_MAGENTA, WIN_COLOR_WHITE, 
						 WIN_COLOR_MAGENTA, WIN_COLOR_WHITE, 
						 WIN_COLOR_MAGENTA, WIN_COLOR_WHITE, 
						 WIN_COLOR_BRIGHT_RED, WIN_COLOR_WHITE, 
						 WIN_COLOR_BRIGHT_RED, WIN_COLOR_WHITE, 
						 WIN_COLOR_RED, WIN_COLOR_WHITE, 
						 WIN_COLOR_RED, WIN_COLOR_WHITE, 
						 WIN_COLOR_RED, WIN_COLOR_WHITE, 
						 WIN_COLOR_RED, WIN_COLOR_WHITE, 
						 WIN_COLOR_RED, WIN_COLOR_WHITE, 
						 WIN_COLOR_RED, WIN_COLOR_WHITE};
	 
	 uint8_t *schemes[] = {original, blackwhite, bluered};
	 *foreground = *(schemes[scheme] + (1 + value * 2) % sizeof(original));
	 *background = *(schemes[scheme] + (0 + value * 2) % sizeof(original));
 }
 
 uint8_t getDigitCount(uint32_t number) {
	 uint8_t count = 0;
	 do {
		 number /= 10;
		 count += 1;
	 } while (number);
	 return count;
 }
 
 void clearScreen() {
	 system("cls");
 }
 
 void drawBoard(uint8_t board[SIZE][SIZE], uint8_t scheme, uint32_t score) {
	 clearScreen();
	 printf("2048.c %17u pts\n\n", score);
	 
	 uint8_t x, y, fg, bg;
	 for (y = 0; y < SIZE; y++) {
		 for (x = 0; x < SIZE; x++) {
			 getColors(board[x][y], scheme, &fg, &bg);
			 setConsoleColor(fg, bg);
			 printf("       ");
			 resetConsoleColor();
		 }
		 printf("\n");
		 for (x = 0; x < SIZE; x++) {
			 getColors(board[x][y], scheme, &fg, &bg);
			 setConsoleColor(fg, bg);
			 if (board[x][y] != 0) {
				 uint32_t number = 1 << board[x][y];
				 uint8_t t = 7 - getDigitCount(number);
				 printf("%*s%u%*s", t - t / 2, "", number, t / 2, "");
			 } else {
				 printf("   ·   ");
			 }
			 resetConsoleColor();
		 }
		 printf("\n");
		 for (x = 0; x < SIZE; x++) {
			 getColors(board[x][y], scheme, &fg, &bg);
			 setConsoleColor(fg, bg);
			 printf("       ");
			 resetConsoleColor();
		 }
		 printf("\n");
	 }
	 printf("\n");
	 printf("           a , s , w , d    keys      \n");
 }
 
 uint8_t findTarget(uint8_t array[SIZE], uint8_t x, uint8_t stop) {
	 uint8_t t;
	 if (x == 0) {
		 return x;
	 }
	 for (t = x - 1;; t--) {
		 if (array[t] != 0) {
			 if (array[t] != array[x]) {
				 return t + 1;
			 }
			 return t;
		 } else {
			 if (t == stop) {
				 return t;
			 }
		 }
	 }
	 return x;
 }
 
 bool slideArray(uint8_t array[SIZE], uint32_t *score) {
	 bool success = false;
	 uint8_t x, t, stop = 0;
 
	 for (x = 0; x < SIZE; x++) {
		 if (array[x] != 0) {
			 t = findTarget(array, x, stop);
			 if (t != x) {
				 if (array[t] == 0) {
					 array[t] = array[x];
				 } else if (array[t] == array[x]) {
					 array[t]++;
					 *score += 1 << array[t];
					 stop = t + 1;
				 }
				 array[x] = 0;
				 success = true;
			 }
		 }
	 }
	 return success;
 }
 
 void rotateBoard(uint8_t board[SIZE][SIZE]) {
	 uint8_t i, j, n = SIZE;
	 uint8_t tmp;
	 for (i = 0; i < n / 2; i++) {
		 for (j = i; j < n - i - 1; j++) {
			 tmp = board[i][j];
			 board[i][j] = board[j][n - i - 1];
			 board[j][n - i - 1] = board[n - i - 1][n - j - 1];
			 board[n - i - 1][n - j - 1] = board[n - j - 1][i];
			 board[n - j - 1][i] = tmp;
		 }
	 }
 }
 
 bool moveUp(uint8_t board[SIZE][SIZE], uint32_t *score) {
	 bool success = false;
	 uint8_t x;
	 for (x = 0; x < SIZE; x++) {
		 success |= slideArray(board[x], score);
	 }
	 return success;
 }
 
 bool moveLeft(uint8_t board[SIZE][SIZE], uint32_t *score) {
	 bool success;
	 rotateBoard(board);
	 success = moveUp(board, score);
	 rotateBoard(board);
	 rotateBoard(board);
	 rotateBoard(board);
	 return success;
 }
 
 bool moveDown(uint8_t board[SIZE][SIZE], uint32_t *score) {
	 bool success;
	 rotateBoard(board);
	 rotateBoard(board);
	 success = moveUp(board, score);
	 rotateBoard(board);
	 rotateBoard(board);
	 return success;
 }
 
 bool moveRight(uint8_t board[SIZE][SIZE], uint32_t *score) {
	 bool success;
	 rotateBoard(board);
	 rotateBoard(board);
	 rotateBoard(board);
	 success = moveUp(board, score);
	 rotateBoard(board);
	 return success;
 }
 
 bool findPairDown(uint8_t board[SIZE][SIZE]) {
	 bool success = false;
	 uint8_t x, y;
	 for (x = 0; x < SIZE; x++) {
		 for (y = 0; y < SIZE - 1; y++) {
			 if (board[x][y] == board[x][y + 1])
				 return true;
		 }
	 }
	 return success;
 }
 
 uint8_t countEmpty(uint8_t board[SIZE][SIZE]) {
	 uint8_t x, y;
	 uint8_t count = 0;
	 for (x = 0; x < SIZE; x++) {
		 for (y = 0; y < SIZE; y++) {
			 if (board[x][y] == 0) {
				 count++;
			 }
		 }
	 }
	 return count;
 }
 
 bool gameEnded(uint8_t board[SIZE][SIZE]) {
	 bool ended = true;
	 if (countEmpty(board) > 0)
		 return false;
	 if (findPairDown(board))
		 return false;
	 rotateBoard(board);
	 if (findPairDown(board))
		 ended = false;
	 rotateBoard(board);
	 rotateBoard(board);
	 rotateBoard(board);
	 return ended;
 }
 
 void addRandom(uint8_t board[SIZE][SIZE]) {
	 static bool initialized = false;
	 uint8_t x, y;
	 uint8_t r, len = 0;
	 uint8_t n, list[SIZE * SIZE][2];
 
	 if (!initialized) {
		 srand((unsigned int)time(NULL));
		 initialized = true;
	 }
 
	 for (x = 0; x < SIZE; x++) {
		 for (y = 0; y < SIZE; y++) {
			 if (board[x][y] == 0) {
				 list[len][0] = x;
				 list[len][1] = y;
				 len++;
			 }
		 }
	 }
 
	 if (len > 0) {
		 r = rand() % len;
		 x = list[r][0];
		 y = list[r][1];
		 n = (rand() % 10) / 9 + 1;
		 board[x][y] = n;
	 }
 }
 
 void initBoard(uint8_t board[SIZE][SIZE]) {
	 uint8_t x, y;
	 for (x = 0; x < SIZE; x++) {
		 for (y = 0; y < SIZE; y++) {
			 board[x][y] = 0;
		 }
	 }
	 addRandom(board);
	 addRandom(board);
 }
 
 bool testSucceed() {
	 uint8_t array[SIZE];
	 uint8_t data[] = {
		 0, 0, 0, 1, 1, 0, 0, 0, 0,
		 0, 0, 1, 1, 2, 0, 0, 0, 4,
		 0, 1, 0, 1, 2, 0, 0, 0, 4,
		 1, 0, 0, 1, 2, 0, 0, 0, 4,
		 1, 0, 1, 0, 2, 0, 0, 0, 4,
		 1, 1, 1, 0, 2, 1, 0, 0, 4,
		 1, 0, 1, 1, 2, 1, 0, 0, 4,
		 1, 1, 0, 1, 2, 1, 0, 0, 4,
		 1, 1, 1, 1, 2, 2, 0, 0, 8,
		 2, 2, 1, 1, 3, 2, 0, 0, 12,
		 1, 1, 2, 2, 2, 3, 0, 0, 12,
		 3, 0, 1, 1, 3, 2, 0, 0, 4,
		 2, 0, 1, 1, 2, 2, 0, 0, 4};
	 uint8_t *in, *out, *points;
	 uint8_t t, tests;
	 uint8_t i;
	 bool success = true;
	 uint32_t score;
 
	 tests = (sizeof(data) / sizeof(data[0])) / (2 * SIZE + 1);
	 for (t = 0; t < tests; t++) {
		 in = data + t * (2 * SIZE + 1);
		 out = in + SIZE;
		 points = in + 2 * SIZE;
		 for (i = 0; i < SIZE; i++) {
			 array[i] = in[i];
		 }
		 score = 0;
		 slideArray(array, &score);
		 for (i = 0; i < SIZE; i++) {
			 if (array[i] != out[i]) {
				 success = false;
			 }
		 }
		 if (score != *points) {
			 success = false;
		 }
		 if (success == false) {
			 for (i = 0; i < SIZE; i++) {
				 printf("%u ", in[i]);
			 }
			 printf("=> ");
			 for (i = 0; i < SIZE; i++) {
				 printf("%u ", array[i]);
			 }
			 printf("(%u points) expected ", score);
			 for (i = 0; i < SIZE; i++) {
				 printf("%u ", in[i]);
			 }
			 printf("=> ");
			 for (i = 0; i < SIZE; i++) {
				 printf("%u ", out[i]);
			 }
			 printf("(%u points)\n", *points);
			 break;
		 }
	 }
	 if (success) {
		 printf("All %u tests executed successfully\n", tests);
	 }
	 return success;
 }
 
 BOOL WINAPI consoleHandler(DWORD signal) {
	 if (signal == CTRL_C_EVENT) {
		 printf("         TERMINATED         \n");
		 resetConsoleColor();
		 exit(0);
	 }
	 return TRUE;
 }
 
 int main(int argc, char *argv[]) {
	 uint8_t board[SIZE][SIZE];
	 uint8_t scheme = 0;
	 uint32_t score = 0;
	 int c;
	 bool success;
 
	 if (argc > 1) {
		 if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
			 printf("Usage: 2048 [OPTION] | [MODE]\n");
			 printf("Play the game 2048 in the console\n\n");
			 printf("Options:\n");
			 printf("  -h,  --help       Show this help message.\n");
			 printf("  -v,  --version    Show version number.\n\n");
			 printf("Modes:\n");
			 printf("  bluered      Use a blue-to-red color scheme.\n");
			 printf("  blackwhite   The black-to-white color scheme.\n");
			 return EXIT_SUCCESS;
		 }
		 else if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
			 printf("2048.c version %s\n", VERSION);
			 return EXIT_SUCCESS;
		 }
		 else if (strcmp(argv[1], "blackwhite") == 0) {
			 scheme = 1;
		 }
		 else if (strcmp(argv[1], "bluered") == 0) {
			 scheme = 2;
		 }
		 else if (strcmp(argv[1], "test") == 0) {
			 return testSucceed() ? EXIT_SUCCESS : EXIT_FAILURE;
		 }
		 else {
			 printf("Invalid option: %s\n\nTry '%s --help' for more options.\n", argv[1], argv[0]);
			 return EXIT_FAILURE;
		 }
	 }
 
	 // Set console handler for CTRL+C
	 SetConsoleCtrlHandler(consoleHandler, TRUE);
 
	 // Enable ANSI escape codes on Windows 10+
	 HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	 DWORD dwMode = 0;
	 GetConsoleMode(hOut, &dwMode);
	 dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	 SetConsoleMode(hOut, dwMode);
 
	 initBoard(board);
	 drawBoard(board, scheme, score);
	 
	 while (true) {
		 if (_kbhit()) {
			 c = _getch();
			 // Handle arrow keys (Windows returns two codes for arrows)
			 if (c == 0xE0 || c == 0) {
				 c = _getch(); // Get the actual key code
				 switch (c) {
				 case 75: // left arrow
					 success = moveLeft(board, &score);
					 break;
				 case 77: // right arrow
					 success = moveRight(board, &score);
					 break;
				 case 72: // up arrow
					 success = moveUp(board, &score);
					 break;
				 case 80: // down arrow
					 success = moveDown(board, &score);
					 break;
				 default:
					 success = false;
				 }
			 } else {
				 switch (c) {
				 case 'a':
				 case 'h':
				 case 'A':
				 case 'H':
					 success = moveLeft(board, &score);
					 break;
				 case 'd':
				 case 'l':
				 case 'D':
				 case 'L':
					 success = moveRight(board, &score);
					 break;
				 case 'w':
				 case 'k':
				 case 'W':
				 case 'K':
					 success = moveUp(board, &score);
					 break;
				 case 's':
				 case 'j':
				 case 'S':
				 case 'J':
					 success = moveDown(board, &score);
					 break;
				 case 'q':
				 case 'Q':
					 printf("        QUIT? (y/n)         \n");
					 c = _getch();
					 if (c == 'y' || c == 'Y') {
						 resetConsoleColor();
						 return EXIT_SUCCESS;
					 }
					 drawBoard(board, scheme, score);
					 success = false;
					 break;
				 case 'r':
				 case 'R':
					 printf("       RESTART? (y/n)       \n");
					 c = _getch();
					 if (c == 'y' || c == 'Y') {
						 initBoard(board);
						 score = 0;
					 }
					 drawBoard(board, scheme, score);
					 success = false;
					 break;
				 default:
					 success = false;
				 }
			 }
 
			 if (success) {
				 drawBoard(board, scheme, score);
				 Sleep(150); // 150 ms delay
				 addRandom(board);
				 drawBoard(board, scheme, score);
				 if (gameEnded(board)) {
					 printf("         GAME OVER          \n");
					 break;
				 }
			 }
		 }
	 }
 
	 resetConsoleColor();
	 return EXIT_SUCCESS;
 }