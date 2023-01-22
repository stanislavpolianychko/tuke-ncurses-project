#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>


// function declaration
void initialise_array(int array_size, char array[array_size], int diff_lvl);
bool element_is_in_array(int element, int array[], int array_size);
int main_menu();
int run_game(int lvl);
void step(int rows, int columns, char field[rows][columns], int cars_column_index, int side_of_step);
void roll_down(int rows, int columns, char field[rows][columns], int cars_column_index, int lvl, int score);
void present_game_field(int rows, int columns, char field[rows][columns], int points_to_win, int total_score);
void present_result_window(int game_res);
void print_logo();
void print_fail();
void print_win();
void loading();
void initialisation_of_colors();


// function check if some int element is in array
bool element_is_in_array(int element, int array[], int array_size) {
    for (int x = 0; x < array_size; x++) if (element == array[x]) return true;
    return false;
}

// initialise one game row
void initialise_array(int array_size, char array[array_size], int diff_lvl) {
    // get random indexes of barriers
    int count_of_barriers = 0;
    int indexes_of_barriers[diff_lvl];
    while (count_of_barriers != diff_lvl) {
        indexes_of_barriers[count_of_barriers] = rand() % array_size;
        count_of_barriers += 1;
    }

    // init field row with specific symbols
    array[0] = ' ';
    for (int i = 1; i < array_size; i++)
        if (element_is_in_array(i, indexes_of_barriers, diff_lvl)) {
            array[i] = '/';
            array[i - 1] = '\\';
        }
        else
            array[i] = ' ';
}

// initialise all field
void initialize_game_field(int rows, int columns, char field[rows][columns], int cars_column_index, int lvl) {
    // set a rows in field by every item
    char row[columns];
    for (int row_index = 0; row_index < rows; row_index++) {
        if (row_index % 2 == 0)
            initialise_array(columns, row, lvl);
        for (int column_index = 0; column_index < columns; column_index++) {
            field[row_index][column_index] = row[column_index];
        }
    }

    for (int row_index = rows - 6; row_index < columns; row_index++) {
        for (int column_index = 0; column_index < columns; column_index++) {
            field[row_index][column_index] = ' ';
        }
    }

    // set a car in the last row
    field[rows-1][cars_column_index] = 'H';
    field[rows-2][cars_column_index] = 'W';
    field[rows-1][cars_column_index + 1] = 'H';
    field[rows-2][cars_column_index + 1] = 'W';

}

// move a car in row
void step(int rows, int columns, char field[rows][columns], int cars_column_index, int cars_column_index_new) {
    field[rows-1][cars_column_index] = ' ';
    field[rows-2][cars_column_index] = ' ';
    field[rows-1][cars_column_index + 1] = ' ';
    field[rows-2][cars_column_index + 1] = ' ';


    field[rows-1][cars_column_index_new] = 'H';
    field[rows-2][cars_column_index_new] = 'W';
    field[rows-1][cars_column_index_new + 1] = 'H';
    field[rows-2][cars_column_index_new + 1] = 'W';
}

// move field down
void roll_down(int rows, int columns, char field[rows][columns], int cars_column_index, int lvl, int score) {
    char copy_of_field[rows][columns];
    for (int row_index = 0; row_index < rows; row_index++)
        for (int column_index = 0; column_index < columns; column_index++)
            copy_of_field[row_index][column_index] = field[row_index][column_index];

    for (int row_index = 0; row_index < rows - 1; row_index++)
        for (int column_index = 0; column_index < columns; column_index++)
            field[row_index + 1][column_index] = copy_of_field[row_index][column_index];

    char last_row[columns];
    if (score % 2 == 0)
        initialise_array(columns, last_row, lvl);
    else
        for (int column_index = 0; column_index < columns; column_index++)
            last_row[column_index] = copy_of_field[0][column_index];

    for (int column_index = 0; column_index < columns; column_index++)
        field[0][column_index] = last_row[column_index];


    field[rows-1][cars_column_index] = 'H';
    field[rows-2][cars_column_index] = 'W';
    field[rows-1][cars_column_index + 1] = 'H';
    field[rows-2][cars_column_index + 1] = 'W';
}

void print_logo() {
    clear();
    char logo[11][39] = {
            {"     ##    ##    ##  ####   #  ####    "},
            {"    #  #    #    #  #    #  #  #   #   "},
            {"   ######    #  #   #    #  #  #   #   "},
            {"  #      #    ##     ####   #  ####    "},
            {"                                       "},
            {"                                       "},
            {"####     #     ####  ####  # #### #### "},
            {"#   #   # #    #   # #   # # #    #   #"},
            {"####   #   #   ####  ####  # #### #### "},
            {"#   # #######  #   # #   # # #    #   #"},
            {"#### #       # #   # #   # # #### #   #"},
    };

    int y_coord = 1;
    for (int i = 0; i < 11; i++) {
        int x_coord = 1;
        for (int x = 0; x < 39; x++) {
            move(y_coord, x_coord);
            if (logo[i][x] == '#')
                attron(COLOR_PAIR(3));
            printw("%c", logo[i][x]);
            attroff(COLOR_PAIR(3));
            x_coord += 1;
        }
        refresh();
        y_coord += 1;
    }
}

void print_fail() {
    clear();
    char fail[5][32] = {
            {"  ######     #      #   #       "},
            {"  #         # #     #   #       "},
            {"  ######   #   #    #   #       "},
            {"  #       #######   #   #       "},
            {"  #      #       #  #   ######  "},
    };

    int y_coord = 1;
    for (int i = 0; i < 5; i++) {
        int x_coord = 1;
        for (int x = 0; x < 32; x++) {
            move(y_coord, x_coord);
            if (fail[i][x] == '#')
                attron(COLOR_PAIR(3));
            printw("%c", fail[i][x]);
            attroff(COLOR_PAIR(3));
            x_coord += 1;
        }
        refresh();
        usleep(150 * 1000);
        y_coord += 1;
    }

    attron(COLOR_PAIR(2));
    move(7, 2);
    printw("UPS... YOU HAVE BEEN CRASHED :(");
    move(8, 2);
    printw("PRESS ANY KEY TO PASS MAIN MENU");
    attroff(COLOR_PAIR(2));
    getch();
    clear();
}

void print_win() {
    clear();
    char fail[5][30] = {
            {"  ##         ##  #    ##    ##"},
            {"   #         #   #    # #   # "},
            {"    #   #   #    #    #  #  # "},
            {"     # # # #     #    #   # # "},
            {"      #   #      #   ##    ## "},
    };

    int y_coord = 1;
    for (int i = 0; i < 5; i++) {
        int x_coord = 1;
        for (int x = 0; x < 30; x++) {
            move(y_coord, x_coord);
            if (fail[i][x] == '#')
                attron(COLOR_PAIR(3));
            printw("%c", fail[i][x]);
            attroff(COLOR_PAIR(3));
            x_coord += 1;
        }
        refresh();
        usleep(200 * 1000);
        y_coord += 1;
    }

    attron(COLOR_PAIR(2));
    move(7, 3);
    printw("CONGRATULATIONS, YOU WIN! : )");
    move(8, 3);
    printw("PRESS ANY KEY TO CONTINUE");
    attroff(COLOR_PAIR(2));
    getch();
    clear();
}

// present all field
void present_game_field(int rows, int columns, char field[rows][columns], int points_to_win, int total_score){
    // show all game using n-curses
    clear();

    WINDOW * win = newwin(43,42,0,0);
    wattron(win, COLOR_PAIR(3));
    box(win, 0, 0);
    wattroff(win, COLOR_PAIR(3));
    wattron(win, COLOR_PAIR(4));
    mvwprintw(win, 1, 15, "SCORE %d/%d", total_score, points_to_win);
    wattroff(win, COLOR_PAIR(4));
    int x_coords = 2;
    for (int row_index = 0; row_index < rows; row_index++) {
        int y_coords = 1;
        for (int column_index = 0; column_index < columns; column_index++) {
            wmove(win, x_coords, y_coords);
            if (field[row_index][column_index] == 'H' || field[row_index][column_index] == 'W')
                wattron(win, COLOR_PAIR(4));
            else if (field[row_index][column_index] != ' ')
                wattron(win, COLOR_PAIR(5));
            wprintw(win, "%c", field[row_index][column_index]);
            wattroff(win, COLOR_PAIR(5));
            wattroff(win, COLOR_PAIR(4));
            y_coords += 1;
        }
        x_coords += 1;
    }
    // refresh screen
    wrefresh(win);
}

// present start menu
int main_menu() {
    // set a box for menu
    clear();
    WINDOW * win = newwin(12,12,15,14);
    // reading moves from keyboard
    keypad(win, true);
    // waiting for user choice
    int highlight = 0;
    int choice;

    print_logo();
    while (true) {
        wrefresh(win);
        if (highlight == 0) {
            wattron(win, COLOR_PAIR(4));
            mvwprintw(win, 1, 2, ">");

            mvwprintw(win, 1, 3, "LEVEL1");

            mvwprintw(win, 1, 9, "<");
            wattroff(win, COLOR_PAIR(4));

            wattron(win, COLOR_PAIR(5));
            mvwprintw(win, 4, 2, " LEVEL2 ");
            mvwprintw(win, 7, 2, " LEVEL3 ");
            mvwprintw(win, 10, 3, " EXIT ");
            wattroff(win, COLOR_PAIR(5));
        }
        else if (highlight == 1) {
            wattron(win, COLOR_PAIR(5));
            mvwprintw(win, 1, 2, " LEVEL1 ");
            wattroff(win, COLOR_PAIR(5));

            wattron(win, COLOR_PAIR(4));
            mvwprintw(win, 4, 2, ">");

            mvwprintw(win, 4, 3, "LEVEL2");

            mvwprintw(win, 4, 9, "<");
            wattroff(win, COLOR_PAIR(4));

            wattron(win, COLOR_PAIR(5));
            mvwprintw(win, 7, 2, " LEVEL3 ");
            mvwprintw(win, 10, 3, " EXIT ");
            wattroff(win, COLOR_PAIR(5));
        }
        else if (highlight == 2) {
            wattron(win, COLOR_PAIR(5));
            mvwprintw(win, 1, 2, " LEVEL1 ");
            mvwprintw(win, 4, 2, " LEVEL2 ");
            wattroff(win, COLOR_PAIR(5));

            wattron(win, COLOR_PAIR(4));
            mvwprintw(win, 7, 2, ">");

            mvwprintw(win, 7, 3, "LEVEL3");

            mvwprintw(win, 7, 9, "<");
            wattroff(win, COLOR_PAIR(4));

            wattron(win, COLOR_PAIR(5));
            mvwprintw(win, 10, 3, " EXIT ");
            wattroff(win, COLOR_PAIR(5));
        }
        else if (highlight == 3) {
            wattron(win, COLOR_PAIR(5));
            mvwprintw(win, 1, 2, " LEVEL1 ");
            mvwprintw(win, 4, 2, " LEVEL2 ");
            mvwprintw(win, 7, 2, " LEVEL3 ");
            wattroff(win, COLOR_PAIR(5));

            wattron(win, COLOR_PAIR(4));
            mvwprintw(win, 10, 3, ">");

            mvwprintw(win, 10, 4, "EXIT");

            mvwprintw(win, 10, 8, "<");
            wattroff(win, COLOR_PAIR(4));

        }
        wrefresh(win);

        choice = wgetch(win);
        switch (choice) {
            case KEY_UP:
                highlight--;
                if (highlight == -1)
                    highlight = 0;
                break;
            case KEY_DOWN:
                highlight++;
                if (highlight == 4)
                    highlight = 3;
                break;
            default:
                break;
        }
        if (choice == 10) {
            break;
        }
    }
    refresh();
    return highlight;
}

void loading() {
    clear();
    char fail[5][54] = {
            {"  #       ####       #     #####   #  ##    #   ######"},
            {"  #      #    #     # #    #    #  #  # #   #  #      "},
            {"  #      #    #    #   #   #    #  #  #  #  #  #    ##"},
            {"  #      #    #   #######  #    #  #  #   # #  #     #"},
            {"  ######  ####   #       # #####   #  #    ##   ##### "},
    };

    int y_coord = 1;
    for (int i = 0; i < 5; i++) {
        int x_coord = 1;
        for (int x = 0; x < 54; x++) {
            move(y_coord, x_coord);
            if (fail[i][x] == '#')
                attron(COLOR_PAIR(3));
            printw("%c", fail[i][x]);
            attroff(COLOR_PAIR(3));
            x_coord += 1;
        }
        refresh();
        usleep(150 * 1000);
        y_coord += 1;
    }

    attron(COLOR_PAIR(2));
    move(7, 2);
    printw("GAME IS READY. GOOD LUCK!");
    move(8, 2);
    printw("PRESS ANY KEY TO START GAME");
    attroff(COLOR_PAIR(2));
    getch();
    clear();
}

void present_result_window(int game_res) {
    if (game_res == 1)
        print_win();
    else
        print_fail();
}

void initialisation_of_colors() {
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_WHITE);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_GREEN);
    init_pair(4, COLOR_RED, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
}

// run all game
int run_game(int lvl) {
    loading();
    // set variables
    int score = 0;
    int count_of_columns = 40;
    int count_of_rows = 40;
    int car_position = 19;
    int reader;
    char field[count_of_rows][count_of_columns];
    int score_to_win = 500 / lvl;

    // init game field and show game first time
    initialize_game_field(count_of_rows, count_of_columns, field, car_position, lvl);

    //print_logo();

    nodelay(stdscr, true);
    keypad(stdscr, true);
    while (score < score_to_win) {
        // while user have not input any value move the game field
        if ((reader = getch()) == ERR) {
            if (field[count_of_rows - 3][car_position] == '/' || field[count_of_rows - 3][car_position + 1] == '/'
                || field[count_of_rows - 3][car_position] == '\\' || field[count_of_rows - 3][car_position + 1] == '\\') {
                nodelay(stdscr, false);
                return 0;
            }
            roll_down(count_of_rows, count_of_columns, field, car_position, lvl, score);
            score += 1;
            present_game_field(count_of_rows, count_of_columns, field, score_to_win, score);
            usleep(150 * 1000);
        }

        // if some key have been pressed check if key == key left or key == key right, and move a car in correct direction
        if (reader == KEY_LEFT) {
            if ((car_position - 1) < 0 || field[count_of_rows - 1][car_position - 1] == '/' || field[count_of_rows - 2][car_position - 1] == '/')
                continue;
            step(count_of_rows, count_of_columns, field, car_position, car_position - 1);
            car_position -= 1;
        }
        else if (reader == KEY_RIGHT) {
            if ((car_position + 1) > (count_of_columns - 2) || field[count_of_rows - 1][car_position + 2] == '\\' || field[count_of_rows - 2][car_position + 2] == '\\')
                continue;
            step(count_of_rows, count_of_columns, field, car_position, car_position + 1);
            car_position += 1;
        }
        reader = ERR;
    }
    nodelay(stdscr, false);
    return 1;
}

int main() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    initialisation_of_colors();

    int res_of_menu = main_menu();
    while (res_of_menu != 3) {
        int result_of_game = run_game(res_of_menu + 1);
        present_result_window(result_of_game);
        res_of_menu = main_menu();
    }

    endwin();
    return 0;
}
