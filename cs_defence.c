// CS Defence
// Jeffrey Liu / z5478431
// Started on 8/3/2023
// Last edited on 26/3/2023

// This program runs a tower defence game!
// Relies heavily on 2d arrays and structs

#include <stdio.h>

// Constants defining the size of the map
#define MAP_ROWS 6
#define MAP_COLUMNS 12

// Constants defining the maximum possible number of certain tile types
#define MAX_PATH 71
#define MAX_TOWER 70
#define MAX_WATER 70

// Constants defining the possible input commands
#define CMD_SPAWN_ENEMIES 'e'
#define CMD_BUILD_TOWER 't'
#define CMD_MOVE_ENEMIES 'm'
#define CMD_UPGRADE_TOWER 'u'
#define CMD_TOWER_ATTACK 'a'
#define CMD_RAIN 'r'
#define CMD_FLOOD 'f'
#define CMD_BUILD_TELEPORTER 'c'

// Constant defining the reward per enemy defeated
#define MONEY_PER_ENEMY 5

// Constants defining the cost of the towers
#define BASIC_COST 200
#define POWER_COST 300
#define FORT_COST 500

// Constants defining the range of the towers
#define BASIC_RANGE 1
#define POWER_RANGE 1
#define FORT_RANGE 2

// Constants definining the power of the towers
#define BASIC_ATTACK 1
#define POWER_ATTACK 2
#define FORT_ATACK 3

// Constants defining True/False booleans
#define TRUE 1
#define FALSE 0

// Constant defining the number of valid teleporters
#define BOTH_TP_VALID 2

///////////////////////////////////////////////////////////////////////////////
/////////////////////////// USER DEFINED TYPES  ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////

enum land_type {
    GRASS,
    WATER,
    PATH_START,
    PATH_END,
    PATH_UP,
    PATH_RIGHT,
    PATH_DOWN,
    PATH_LEFT,
    TELEPORTER
};

enum entity {
    EMPTY,
    ENEMY,
    BASIC_TOWER,
    POWER_TOWER,
    FORTIFIED_TOWER
};

struct tile {
    enum land_type land;
    enum entity entity;

    int n_enemies;
};

struct coordinate {
    int row;
    int col;
};

struct game_info {
    int lives;
    int money;
    struct coordinate start_point;
    struct coordinate end_point;
    int path_length;
};

///////////////////////////////////////////////////////////////////////////////
////////////////////////  YOUR FUNCTION PROTOTYPE  ////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int scan_starting_lives(void);

int scan_money(void);

struct coordinate scan_start_point(struct tile map[MAP_ROWS][MAP_COLUMNS]);

struct coordinate scan_end_point(struct tile map[MAP_ROWS][MAP_COLUMNS]);

void scan_starting_enemies(struct tile map[MAP_ROWS][MAP_COLUMNS],
    struct coordinate start_point,
    int lives,
    int money);

int valid_point(int row, int col);

void scan_lake(struct tile map[MAP_ROWS][MAP_COLUMNS], 
    int lives, 
    int money);

int scan_path(struct tile map[MAP_ROWS][MAP_COLUMNS], 
    struct coordinate path[MAX_PATH],
    struct game_info game_data);

int command_loop(struct tile map[MAP_ROWS][MAP_COLUMNS],
    struct coordinate path[MAX_PATH], 
    struct game_info game_data);

void spawn_enemy(struct tile map[MAP_ROWS][MAP_COLUMNS],
    struct coordinate start_point);

int build_tower(struct tile map[MAP_ROWS][MAP_COLUMNS], 
    int money);

int move_enemies(struct tile map[MAP_ROWS][MAP_COLUMNS], 
    struct coordinate path[MAX_PATH],
    int path_length,
    int lives);

int move_enemies_one_tile(struct tile map[MAP_ROWS][MAP_COLUMNS],
    struct coordinate path[MAX_PATH],
    int path_length,
    int lives,
    int i);

int enemy_reached_end(struct tile map[MAP_ROWS][MAP_COLUMNS],
    struct coordinate path[MAX_PATH],
    int path_length,
    int i);

void enemy_teleport(struct tile map[MAP_ROWS][MAP_COLUMNS],
    struct coordinate path[MAX_PATH],
    int path_length,
    int i);

void enemy_move(struct tile map[MAP_ROWS][MAP_COLUMNS],
    struct coordinate path[MAX_PATH],
    int path_length,
    int i);

int upgrade_tower(struct tile map[MAP_ROWS][MAP_COLUMNS],
    int money);

int tower_attack(struct tile map[MAP_ROWS][MAP_COLUMNS],
    int money,
    struct coordinate path[MAX_PATH],
    int path_length);

int get_range(struct tile map[MAP_ROWS][MAP_COLUMNS], 
    struct coordinate tower_location[MAX_TOWER], 
    int i);

int get_attack(struct tile map[MAP_ROWS][MAP_COLUMNS], 
    struct coordinate tower_location[MAX_TOWER], 
    int i);

int scan_tower_locations(struct tile map[MAP_ROWS][MAP_COLUMNS],
    struct coordinate tower_location[MAX_TOWER]);

int tower_attack_variable(struct tile map[MAP_ROWS][MAP_COLUMNS],
    int tower_range,
    int tower_attack,
    struct coordinate tower_location[MAX_TOWER],
    int tower_index);

int scan_enemies_in_range(struct tile map[MAP_ROWS][MAP_COLUMNS],
    int tower_range, 
    int tower_attack,
    struct coordinate tower_location[MAX_TOWER],
    int tower_index,
    int row, 
    int col);

void rain(struct tile map[MAP_ROWS][MAP_COLUMNS]);

void flood_tile(struct tile map[MAP_ROWS][MAP_COLUMNS],
    int row,
    int col);

void flood_map(struct tile map[MAP_ROWS][MAP_COLUMNS]);

int correct_rain_within_range(int offset, 
    int spacing);

int scan_water_tiles(struct tile map[MAP_ROWS][MAP_COLUMNS],
    struct coordinate water_location[MAX_WATER],
    int water_count);

void scan_tiles_in_range(struct tile map[MAP_ROWS][MAP_COLUMNS],
    int water_index,
    struct coordinate water_location[MAX_WATER]);

void flood_map_linear(struct tile map[MAP_ROWS][MAP_COLUMNS],
    int water_index,
    struct coordinate water_location[MAX_WATER],
    int offset_row,
    int offset_col);

void build_teleporter(struct tile map[MAP_ROWS][MAP_COLUMNS], 
    struct coordinate path[MAX_PATH],
    int path_length);

int valid_teleporter_check(struct tile map[MAP_ROWS][MAP_COLUMNS], 
    struct coordinate path[MAX_PATH],
    int path_length,
    struct coordinate teleporter);

void spawn_teleporters_on_path(struct tile map[MAP_ROWS][MAP_COLUMNS], 
    struct coordinate path[MAX_PATH],
    int path_length,
    struct coordinate teleporter1,
    struct coordinate teleporter2);

int not_between_teleporter_check(struct tile map[MAP_ROWS][MAP_COLUMNS], 
    struct coordinate path[MAX_PATH],
    struct coordinate teleporter1,
    struct coordinate teleporter2,
    int between_teleporter,
    int i);

int between_teleporter_check(struct tile map[MAP_ROWS][MAP_COLUMNS], 
    struct coordinate path[MAX_PATH],
    struct coordinate teleporter1,
    struct coordinate teleporter2,
    int between_teleporter,
    int i);

///////////////////////////////////////////////////////////////////////////////
////////////////////// PROVIDED FUNCTION PROTOTYPE  ///////////////////////////
///////////////////////////////////////////////////////////////////////////////
void initialise_map(struct tile map[MAP_ROWS][MAP_COLUMNS]);
void print_map(struct tile map[MAP_ROWS][MAP_COLUMNS], int lives, int money);
void print_tile(struct tile tile, int entity_print);


///////////////////////////// MAIN FUNCTION ///////////////////////////////////
int main(void) {
    // Initialises a struct that stores the game data pass into command loop
    struct game_info game_data;

    // Initialise a 2d array of tiles called 'map'
    struct tile map[MAP_ROWS][MAP_COLUMNS];

    // Initialises all tiles in the map to be GRASS tiles with an EMPTY entity
    initialise_map(map);

    // Scans in the starting lives input by the player
    game_data.lives = scan_starting_lives();

    // Scans in the starting money input by the player
    game_data.money = scan_money();

    // Scans in the start / end points input by the player and updates the map
    game_data.start_point = scan_start_point(map);
    game_data.end_point = scan_end_point(map);

    // Prints out the map
    print_map(map, game_data.lives, game_data.money);

    // Scans in the number of starting enemies, then updates and prints the map
    scan_starting_enemies(map, game_data.start_point, game_data.lives, 
        game_data.money);

    // Scans in the lake input, then updates and prints the map
    scan_lake(map, game_data.lives, game_data.money);

    // Defines an array of structs to store the path tiles sequentially
    struct coordinate path[MAX_PATH];

    // Scans in the path input to the array, then updates and prints the map
    // Returns the length of the path
    game_data.path_length = scan_path(map, path, game_data);

    // Runs a command loop that returns 0 when complete
    return command_loop(map, path, game_data);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////// YOUR FUNCTIONS //////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Scans in and returns the starting lives
// The starting lives are assigned to the game_data struct for later use
int scan_starting_lives(void) {
    // Initialises the int for lives and scans in the input value
    int lives;
    printf("Starting Lives: ");
    scanf("%d", &lives);

    return lives;
}

///////////////////////////////////////////////////////////////////////////////

// Scans in and returns the starting money
// The starting money is assigned to the game_data struct for later use
int scan_money(void) {
    // Initialises the int for money and scans in the input value
    int money;
    printf("Starting Money($): ");
    scanf("%d", &money);

    return money;
}

///////////////////////////////////////////////////////////////////////////////

// Scans in and returns the start point
// The start point is assigned to the game_data struct for later use
struct coordinate scan_start_point(struct tile map[MAP_ROWS][MAP_COLUMNS]) {
    // Initialises the struct for end_point and scans in the input value
    struct coordinate start_point;
    printf("Start Point: ");
    scanf("%d %d", &start_point.row, &start_point.col);

    // Updates the map to account for the input start point
    map[start_point.row][start_point.col].land = PATH_START;

    return start_point;
}

///////////////////////////////////////////////////////////////////////////////

// Scans in and returns the end point
// The end point is assigned to the game_data struct for later use
struct coordinate scan_end_point(struct tile map[MAP_ROWS][MAP_COLUMNS]) {
    // Initialises the struct for end_point and scans in the input value
    struct coordinate end_point;
    printf("End Point: ");
    scanf("%d %d", &end_point.row, &end_point.col);

    // Updates the map to account for the input end point
    map[end_point.row][end_point.col].land = PATH_END;

    return end_point;
}

///////////////////////////////////////////////////////////////////////////////

// Scans in the starting enemies and updats the map accordingly
// No return is needed as the number of enemies can be taken from the map array
void scan_starting_enemies(struct tile map[MAP_ROWS][MAP_COLUMNS],
    struct coordinate start_point, int lives, int money) {
    // Initialises the int for starting_enemies and scans in the input value
    int starting_enemies;
    printf("Initial Enemies: ");
    scanf("%d", &starting_enemies);

    // Checks that the number of starting enemies input is valid and non-zero
    // and updates the map
    if (starting_enemies > 0 && starting_enemies <= 999) {
        map[start_point.row][start_point.col].entity = ENEMY;
        map[start_point.row][start_point.col].n_enemies = starting_enemies;
    }

    // Prints out the map
    print_map(map, lives, money);
}

///////////////////////////////////////////////////////////////////////////////

// Checks whether the input coordinate is within the map or not
// Returns 0 if the point is valid
// Returns 1 if the point is invalid
int valid_point(int row, int col) {
    if (row >= 0 && 
    col >= 0 && 
    row < MAP_ROWS && 
    col < MAP_COLUMNS) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}

///////////////////////////////////////////////////////////////////////////////

// Scans in the lake inputs and updates and prints the map accordingly
void scan_lake(struct tile map[MAP_ROWS][MAP_COLUMNS], int lives, int money) {
    // Assigns the variables for the lake's top-left corner and its size
    int lake_start_row;
    int lake_start_col;
    int lake_width;
    int lake_height;

    // Scans in the inputs to the previous variables
    printf("Enter Lake: ");
    scanf("%d %d %d %d", &lake_start_row, &lake_start_col, &lake_height, 
        &lake_width);

    // Checks if the top-left and bottom-right corners of the lake input are
    // valid points. Prints the error message if invalid.
    if (valid_point(lake_start_row + lake_height - 1, 
        lake_start_col + lake_width - 1) == TRUE &&
        valid_point(lake_start_row, lake_start_col) == TRUE) {
        // Traverses through the inputs in the 2d array and converts the 
        // land to water.
        for (int i = lake_start_row; i < lake_start_row + lake_height; i++) {
            for (int j = lake_start_col; j < lake_start_col + lake_width; j++) {
                map[i][j].land = WATER;
            }
        }
    }
    else {
        printf("Error: Lake out of bounds, ignoring...\n");
    }

    print_map(map, lives, money);
}

///////////////////////////////////////////////////////////////////////////////

// Scans in the path inputs and updates and prints the map accordingly
int scan_path(struct tile map[MAP_ROWS][MAP_COLUMNS], 
    struct coordinate path[MAX_PATH],
    struct game_info game_data) {
    // Assigns a char to be initialised to individual inputs via a loop
    char input;
    printf("Enter Path: ");

    // i and j represent the coordinates of the current tile
    // The tile starts at the start point and moves according to the inputs
    // Sets the path_length to be 0, and increments with each input
    int i = game_data.start_point.row;
    int j = game_data.start_point.col;
    game_data.path_length = 0;

    // Runs until the current tile is equal to the end point
    while (!(i == game_data.end_point.row && j == game_data.end_point.col)) {
        // Scans in the next input 
        scanf(" %c", &input);

        // Assigns the current tile coordinates to its index in the path array
        path[game_data.path_length].row = i;
        path[game_data.path_length].col = j;

        // Depending on the input, updates the current tile's lnd and the 
        // position of the current tile
        if (input == 'u') {
            map[i][j].land = PATH_UP;
            i --;
        }
        else if (input == 'r') {
            map[i][j].land = PATH_RIGHT;
        cj++;
        }
        else if (input == 'd') {
            map[i][j].land = PATH_DOWN;
        ci++;
        }
        else if (input == 'l') {
            map[i][j].land = PATH_LEFT;
            j --;
        } 

        // Increments the path_length
        game_data.path_lengch++;
    }
    print_map(map, game_data.lives, game_data.money);

    // Returns the path_length to be assigned in game_data
    return game_data.path_length;
}

///////////////////////////////////////////////////////////////////////////////

// Accepts command inputs that alter the current game state
// Runs until the player dies or they enter control-d, when it returns 0
int command_loop(struct tile map[MAP_ROWS][MAP_COLUMNS],
    struct coordinate path[MAX_PATH], 
    struct game_info game_data) {
    // Assigns a variable to be the individual command input
    printf("Enter Command: ");
    char input;
    // Runs while the command is not control-d, or the player has over 0 lives
    while ((scanf(" %c", &input) != EOF) && (game_data.lives > 0)) {
        // Runs a certain command function depending on the input command
        if (input == CMD_SPAWN_ENEMIES) {
            spawn_enemy(map, game_data.start_point);
        }
        else if (input == CMD_BUILD_TOWER) {
            game_data.money = build_tower(map, game_data.money);
        }
        else if (input == CMD_MOVE_ENEMIES) {
            game_data.lives = move_enemies(map, path, 
                game_data.path_length, game_data.lives);
        }
        else if (input == CMD_UPGRADE_TOWER) {
            game_data.money = upgrade_tower(map, game_data.money);
        }
        else if (input == CMD_TOWER_ATTACK) {
            game_data.money = tower_attack(map, game_data.money, 
                path, game_data.path_length);
        }
        else if (input == CMD_RAIN) {
            rain(map);
        }
        else if (input == CMD_FLOOD) {
            flood_map(map);
        }
        else if (input == CMD_BUILD_TELEPORTER) {
            build_teleporter(map, path, game_data.path_length);
        }
        print_map(map, game_data.lives, game_data.money);

        // If the player has no lives, prints the out of lives text
        if (!(game_data.lives > 0)) {
            printf("Oh no, you ran out of lives!");
            printf("\nGame Over!\n");
            return 0;
        }
        else {
            printf("Enter Command: ");  
        }
    }
    // If the player inputs control-d prints out the game end text
    printf("\nGame Over!\n");
    return 0;
}

///////////////////////////////////////////////////////////////////////////////

// Spawns in the input number of enemies at the start point
void spawn_enemy(struct tile map[MAP_ROWS][MAP_COLUMNS],
    struct coordinate start_point) {
    // Scans in the number of enemies input
    int enemy_input;
    scanf ("%d", &enemy_input);

    // Checks if there are enemies at the start point
    // Adds the enemy input to the start point if there are enemies there
    // Assigns the enemy input to the start point otherwise
    if ((map[start_point.row][start_point.col].n_enemies > 0) &&
        (map[start_point.row][start_point.col].n_enemies <= 999)) {
        map[start_point.row][start_point.col].n_enemies += enemy_input;
    }
    else {
        map[start_point.row][start_point.col].entity = ENEMY;
        map[start_point.row][start_point.col].n_enemies = enemy_input;
    }
}

///////////////////////////////////////////////////////////////////////////////

// Spawns in a tower at the input coordinate if there are sufficient funds
// Prints an error message otherwise
int build_tower(struct tile map[MAP_ROWS][MAP_COLUMNS], 
    int money) {
    // Scans in the coordinates entered into two variables
    int tower_row;
    int tower_col;
    scanf("%d %d", &tower_row, &tower_col);

    // Places the tower and subtracts the cost if there are sufficient funds
    // and if the input point is valid
    // Prints the error message otherwise
    if ((money >= BASIC_COST) &&
        (valid_point(tower_row, tower_col) == TRUE) &&
        (map[tower_row][tower_col].land == GRASS) &&
        (map[tower_row][tower_col].entity == EMPTY)) {
        money -= BASIC_COST;
        map[tower_row][tower_col].entity = BASIC_TOWER;
        printf("Tower successfully created!\n");
    }
    else {
        printf("Error: Tower creation unsuccessful. Make sure you have at least"
        " $%d and that the tower is placed on a grass block with no entity."
        "\n", BASIC_COST);
    }

    return money;
}

///////////////////////////////////////////////////////////////////////////////

// Moves the enemies the input number of tile
int move_enemies(struct tile map[MAP_ROWS][MAP_COLUMNS], 
    struct coordinate path[MAX_PATH],
    int path_length,
    int lives) {
    // Scans in the desired move distance
    int move_distance;
    scanf("%d", &move_distance);

    // Assigns a count for the amount of damage taken
    int damage_taken = 0;

    // Moves all enemies by one tile the number of times input
    for (int counter = 0; counter < move_distance; countcr++) {
        // Scans through the path tiles
        for (int i = 0; i < path_length;ci++) {
            // Moves enemies on that tile 1 tile forward and returns the number
            // of enemies that reached the end, to count down lives
            int reached_end = move_enemies_one_tile(map, path, 
                path_length, lives, i);

            // Deduct number of enemies that reached the end from lives and add
            // to damage_taken
            damage_taken += reached_end;
            lives -= reached_end;
        }
    }

    // Prints a message stating the number of enemies that reached the end
    printf("%d enemies reached the end!\n", damage_taken);

    return lives;
}

///////////////////////////////////////////////////////////////////////////////

// Moves all enemies on a certain path tile forward one tile
int move_enemies_one_tile(struct tile map[MAP_ROWS][MAP_COLUMNS], 
    struct coordinate path[MAX_PATH],
    int path_length,
    int lives,
    int i) {
    // Initialises a variable to count the number of enemies that reach the end
    int reached_end = 0;

    // Checks if there are enemies on the tile
    // Moves enemies if there are enemies on the tile
    if (map[path[path_length - i - 1].row]
        [path[path_length - i - 1].col].entity == ENEMY) {
        // If enemy is on second last tile, remove enemies and update 
        // the reached_end variable
        if (path_length - i > path_length - 1) {
            reached_end = enemy_reached_end(map, path, path_length, i);
        }
        // If enemy is not on path (ie passed a teleporter), teleport the enemy
        // to the next teleporter
        else if ((map[path[path_length - i].row]
            [path[path_length - i].col].land == GRASS) ||
            (map[path[path_length - i].row]
            [path[path_length - i].col].land == WATER)) {
            enemy_teleport(map, path, path_length, i);
        }
        // Otherwise, move the enemy one tile forward
        else {
            enemy_move(map, path, path_length, i);
        }
    }
    return reached_end;
}

///////////////////////////////////////////////////////////////////////////////

// Removes enemies that reach the end tile
int enemy_reached_end(struct tile map[MAP_ROWS][MAP_COLUMNS],
    struct coordinate path[MAX_PATH],
    int path_length,
    int i) {
    // Assign a variable to equal the number of enemies on the current tile
    int reached_end = map[path[path_length - i - 1].row]
        [path[path_length - i - 1].col].n_enemies;

    // Clear enemies from current tile
    map[path[path_length - i - 1].row]
        [path[path_length - i - 1].col].entity = EMPTY;

    map[path[path_length - i - 1].row]
        [path[path_length - i - 1].col].n_enemies = 0;

    // Return number of enemies that reached the end
    return reached_end;
}

///////////////////////////////////////////////////////////////////////////////

// Teleports enemies when they leave a teleporter tile
void enemy_teleport(struct tile map[MAP_ROWS][MAP_COLUMNS],
    struct coordinate path[MAX_PATH],
    int path_length,
    int i) {
    // Assign a variable to equal the distance to teleport
    int teleport_distance = 1;

    // Scan through the path from the current tile and increment the teleporter
    // distance until we reach the next teleporter
    while ((map[path[path_length - i + teleport_distance].row]
        [path[path_length - i + teleport_distance].col].land 
        != TELEPORTER)) {
        teleport_distance++;
    }

    // Move enemies on the current tile to the next teleporter
    map[path[path_length - i + teleport_distance].row]
        [path[path_length - i  + teleport_distance].col].entity = 
        ENEMY;

    map[path[path_length - i + teleport_distance].row]
        [path[path_length - i + teleport_distance].col].n_enemies = 
        map[path[path_length - i - 1].row]
        [path[path_length - i- 1].col].n_enemies;
    
    // Clear enemies from the current tile
    map[path[path_length - i - 1].row]
        [path[path_length - i - 1].col].n_enemies = 0;

    map[path[path_length - i - 1].row]
        [path[path_length - i - 1].col].entity = EMPTY;
}

///////////////////////////////////////////////////////////////////////////////

// Moves enemies forward one tile
void enemy_move(struct tile map[MAP_ROWS][MAP_COLUMNS],
    struct coordinate path[MAX_PATH],
    int path_length,
    int i) {
    // Move enemies from the current tile to the next tile
    map[path[path_length - i].row]
        [path[path_length - i].col].entity = ENEMY;

    map[path[path_length - i].row]
        [path[path_length - i].col].n_enemies = 
        map[path[path_length - i - 1].row]
        [path[path_length - i - 1].col].n_enemies;
    
    // Clear enemies from the current tile
    map[path[path_length - i - 1].row]
        [path[path_length - i - 1].col].n_enemies = 0;

    map[path[path_length - i - 1].row]
        [path[path_length - i - 1].col].entity = EMPTY;
}

///////////////////////////////////////////////////////////////////////////////

// If there is a tower on the input tile and there are sufficient funds,
// upgrade the tower on that tile.
int upgrade_tower(struct tile map[MAP_ROWS][MAP_COLUMNS],
    int money) {
    // Scans in the input tile to be upgraded
    struct coordinate upgrade;
    scanf("%d %d", &upgrade.row, &upgrade.col);

    // Checks if the input tile is valid
    if (valid_point(upgrade.row, upgrade.col) == TRUE) {
        // If the tower on the tile is a basic tower, upgrade to a power tower
        // and deduct funds if there are enough funds
        if (map[upgrade.row][upgrade.col].entity == BASIC_TOWER) {
            if (money >= POWER_COST) {
                map[upgrade.row][upgrade.col].entity = POWER_TOWER;
                money -= POWER_COST;
                printf("Upgrade Successful!\n");
            }
            else {
                printf("Error: Insufficient Funds.\n");
            }
        }
        // If the tower on the tile is a power tower, upgrade to a fortified
        // tower and deduct funds if there are enough funds
        else if (map[upgrade.row][upgrade.col].entity == POWER_TOWER) {
            if (money >= FORT_COST) {
                map[upgrade.row][upgrade.col].entity = FORTIFIED_TOWER;
                money -= FORT_COST;
                printf("Upgrade Successful!\n");
            }
            else {
                printf("Error: Insufficient Funds\n.");
            }
        }
        // If the tower on the tile is a fortified tower, print error message
        else if (map[upgrade.row][upgrade.col].entity == FORTIFIED_TOWER) {
            printf("Error: Tower cannot be upgraded further.\n");
        }
        // If there is no tower on the tile, print error message
        else {
            printf("Error: Upgrade target contains no tower entity.\n");
        }
    }
    // If input tile is invalud, print error message
    else {
        printf("Error: Upgrade target is out-of-bounds.\n");
    }
    // Return the new amount of money
    return money;
}

///////////////////////////////////////////////////////////////////////////////

// Attacks all enemies in range of towers
int tower_attack(struct tile map[MAP_ROWS][MAP_COLUMNS],
    int money,
    struct coordinate path[MAX_PATH],
    int path_length) {
    // Assign a variable to hold the tower locations
    struct coordinate tower_location[MAX_TOWER];

    // Scan in the number of times the towers should attack
    int attack_times;
    scanf("%d", &attack_times);

    // Scan tower locations into the tower location array and return the number 
    // of towers
    int tower_count = scan_tower_locations(map, tower_location);

    // Assign the number of enemies defeated as 0
    int defeated_enemy_count = 0;

    // Each tower attacks once for each attack_time input
    for (int counter = 0; counter < attack_times; countcr++) {
        // Traverses the array containing tiles with twers on them
        for (int i = 0; i < tower_count;ci++) {
            // Obtain range and attack of tower based off of tower type
            int tower_range = get_range(map, tower_location, i);
            int tower_attack = get_attack(map, tower_location, i);

            // Increment the number of enemies that were defeated
            defeated_enemy_count += tower_attack_variable(map, tower_range, 
                tower_attack, tower_location, i);
        }
    }

    // Traverses through the path and clears all tiles with 0 enemeis on them
    for (int i = 0; i < path_length;ci++) {
        if (map[path[i].row][path[i].col].n_enemies <= 0) {
            map[path[i].row][path[i].col].entity = EMPTY;
        }
    }

    // Print the number of enemies destroyed
    printf("%d enemies destroyed!\n", defeated_enemy_count);

    // Update and return the amount of money
    money += MONEY_PER_ENEMY * defeated_enemy_count;
    return money;
}

///////////////////////////////////////////////////////////////////////////////

// Obtain the range value for a tower on an input tile
int get_range(struct tile map[MAP_ROWS][MAP_COLUMNS], 
    struct coordinate tower_location[MAX_TOWER], 
    int i) {
    // Assigns the tower range to be 0
    int tower_range = 0;

    // Scans for the type of tower at that tile and update range accordingly
    if (map[tower_location[i].row]
        [tower_location[i].col].entity == BASIC_TOWER) {
        tower_range = BASIC_RANGE;
    }
    else if (map[tower_location[i].row]
        [tower_location[i].col].entity == POWER_TOWER) {
        tower_range = POWER_RANGE;
    }
    else if (map[tower_location[i].row]
        [tower_location[i].col].entity == FORTIFIED_TOWER) {
        tower_range = FORT_RANGE;
    }

    // Return the range
    return tower_range;
}

///////////////////////////////////////////////////////////////////////////////

// Obtain the attack value for a toweron an input tile
int get_attack(struct tile map[MAP_ROWS][MAP_COLUMNS], 
    struct coordinate tower_location[MAX_TOWER], 
    int i) {
    // Assigns the tower attack to be 0
    int tower_attack = 0;

    // Scans for the type of tower at that tile and update attack accordingly
    if (map[tower_location[i].row]
        [tower_location[i].col].entity == BASIC_TOWER) {
        tower_attack = BASIC_ATTACK;
    }
    else if (map[tower_location[i].row]
        [tower_location[i].col].entity == POWER_TOWER) {
        tower_attack = POWER_ATTACK;
    }
    else if (map[tower_location[i].row]
        [tower_location[i].col].entity == FORTIFIED_TOWER) {
        tower_attack = FORT_ATACK;
    }

    // Return the attack
    return tower_attack;
}

///////////////////////////////////////////////////////////////////////////////

// Obtain the locations of all towers on the map and store them in an array
int scan_tower_locations(struct tile map[MAP_ROWS][MAP_COLUMNS],
    struct coordinate tower_location[MAX_TOWER]) {
    // Assign the number of towers to be 0
    int tower_count = 0;

    // Traverse through the map
    for (int i = 0; i < MAP_ROWS;ci++) {
        for (int j = 0; j < MAP_COLUMNS;cj++) {
            // If there is a tower on a tile, store the tower location in the
            // tower_location array and increment tower_count
            if (map[i][j].entity == BASIC_TOWER ||
                map[i][j].entity == POWER_TOWER ||
                map[i][j].entity == FORTIFIED_TOWER) {
                tower_location[tower_count].row = i;
                tower_location[tower_count].col = j;

                tower_couct++;
            }
        }
    }
    // Return the tower_count
    return tower_count;
}

///////////////////////////////////////////////////////////////////////////////

// Using the range and attack, scan all tiles in range of a tower and attack
// any enemies in range
int tower_attack_variable(struct tile map[MAP_ROWS][MAP_COLUMNS],
    int tower_range,
    int tower_attack,
    struct coordinate tower_location[MAX_TOWER],
    int tower_index) {
    // Assign the number of enemeis defeated by the tower to be 0
    int defeated_enemy_count = 0;

    // Traverses through the area in range of the tower
    for (int i = -tower_range; i <= tower_range;ci++) {
        for (int j = -tower_range; j <= tower_range;cj++) {
            // Checks if the point being scanned is on the map
            if (valid_point(tower_location[tower_index].row + i,
                tower_location[tower_index].col + j) == TRUE) {
                // Increments the number of enemies defeated
                defeated_enemy_count += scan_enemies_in_range(map, tower_range,
                    tower_attack, tower_location, tower_index, i, j);
            }
        }
    }
    // Returns the number of enemies defeated
    return defeated_enemy_count;
}

///////////////////////////////////////////////////////////////////////////////

// Checks for enemies on a specific tile being scanned in range of a tower
int scan_enemies_in_range(struct tile map[MAP_ROWS][MAP_COLUMNS],
    int tower_range,
    int tower_attack,
    struct coordinate tower_location[MAX_TOWER],
    int tower_index,
    int row,
    int col) {
    // Assign the number of defeated enemies to be 0
    int defeated_enemy_count = 0;

    // Checks if there is an enemy on the tile input
    if (map[tower_location[tower_index].row + row]
        [tower_location[tower_index].col + col].entity == ENEMY) {
        // Checks whether the tower overkills the enemies on the tile and 
        // clears all enemies on the tile without overkilling
        if (tower_attack > map[tower_location[tower_index].row + row]
            [tower_location[tower_index].col + col].n_enemies) {
            defeated_enemy_count += map[tower_location[tower_index].row + row]
                [tower_location[tower_index].col + col].n_enemies;
            
            map[tower_location[tower_index].row + row]
                [tower_location[tower_index].col + col].n_enemies = 0;
        }
        // Otherwise, damages enemies on that tile using the attack of the
        // tower
        else {
            defeated_enemy_count += tower_attack;

            map[tower_location[tower_index].row + row]
                [tower_location[tower_index].col + col].n_enemies -= tower_attack;
        }
    }
    // Return the number of enemies destroyed
    return defeated_enemy_count;
}

///////////////////////////////////////////////////////////////////////////////

// Spawns in rain at the input offset at the input intervals
void rain(struct tile map[MAP_ROWS][MAP_COLUMNS]) {
    // Scan in the inputs for row/col spacing/offset
    int rain_spacing_row;
    int rain_spacing_col;
    int rain_offset_row;
    int rain_offset_col;
    scanf("%d %d %d %d", &rain_spacing_row, &rain_spacing_col,
        &rain_offset_row, &rain_offset_col);

    // Correct the offset so that it is the most upper-left water tile spawned
    rain_offset_row = correct_rain_within_range(rain_offset_row, 
        rain_spacing_row);
    rain_offset_col = correct_rain_within_range(rain_offset_col, 
        rain_spacing_col);

    // Traverses through the map, starting at the offset and incremending by 
    // the spacing
    for (int i = rain_offset_row; i < MAP_ROWS; i += rain_spacing_row) {
        for (int j = rain_offset_col; j < MAP_COLUMNS; j += rain_spacing_col) {
            // Replaces the tile with water
            flood_tile(map, i, j); 
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

// Corrects the offset so it is between 0 and the spacing value
int correct_rain_within_range(int offset, int spacing) {

    while (offset < 0) {
        offset += spacing;
    }

    while (offset >= spacing) {
        offset -= spacing;
    }

    return offset;
}

///////////////////////////////////////////////////////////////////////////////

// Replaces the tile with water, if able
void flood_tile(struct tile map[MAP_ROWS][MAP_COLUMNS], 
    int row, 
    int col) {
    // Checks if the land type is grass
    if (map[row][col].land == GRASS) {
        // Checks if the entity type is a basic or power tower
        if (map[row][col].entity == BASIC_TOWER ||
            map[row][col].entity == POWER_TOWER) {
            // Removes the entity
            map[row][col].entity = EMPTY;
        }
        // Replaces the grass with water
        map[row][col].land = WATER;
    }
}

///////////////////////////////////////////////////////////////////////////////

// Scans in all water tiles on the map and expands the water out 1 tile in row
// and cols
void flood_map(struct tile map[MAP_ROWS][MAP_COLUMNS]) {
    // Scan in the amount to flood by
    int flood_counter;
    scanf("%d", &flood_counter);

    for (int counter = 0; counter < flood_counter; countcr++) {
        // Assign an array to store the locations of water tiles on the map
        // to 
        struct coordinate water_location[MAX_WATER];

        // Scan water tiles into the array, returning the number of water
        // tiles on the map
        int water_count = 0;
        water_count = scan_water_tiles(map, water_location, water_count);

        // Increment through the array of water tiles and scan for valie tiles
        // to be flooded
        for (int i = 0; i < water_count;ci++) {
            scan_tiles_in_range(map, i, water_location);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

// Scans the map for water tiles and stores them in the water_location array
int scan_water_tiles(struct tile map[MAP_ROWS][MAP_COLUMNS],
    struct coordinate water_location[MAX_WATER],
    int water_count) {
    // Traverses through the map
    for (int i = 0; i < MAP_ROWS;ci++) {
        for (int j = 0; j < MAP_COLUMNS;cj++) {
            // If the currently scanned tile is water, store it in the array
            if (map[i][j].land == WATER) {
                water_location[water_count].row = i;
                water_location[water_count].col = j;

                // Increment the number of water tiles
                water_couct++;
            }
        }
    }
    // Return the number of water tiles
    return water_count;
}

///////////////////////////////////////////////////////////////////////////////

// Scan water tiles within 1 tile of other water tiles to be flooded
void scan_tiles_in_range(struct tile map[MAP_ROWS][MAP_COLUMNS],
    int water_index,
    struct coordinate water_location[MAX_WATER]) {
    // For the rows, scan tiles within 1 tile of water
    for (int i = -1; i <= 1;ci++) {
        int j = 0;
        // Flood the tile if valid
        flood_map_linear(map, water_index, water_location, i, j);
    }
    // For the columns, scan tiles within 1 tile of water
    for (int j = -1; j <= 1;cj++) {
        int i = 0;
        // Flood the tile if valid
        flood_map_linear(map, water_index, water_location, i, j);
    }
}

///////////////////////////////////////////////////////////////////////////////

// Flood tiles after error checking for validity
void flood_map_linear(struct tile map[MAP_ROWS][MAP_COLUMNS],
    int water_index,
    struct coordinate water_location[MAX_WATER],
    int offset_row,
    int offset_col) {
    // Checks that the point to be flooded is on the map
    if (valid_point(water_location[water_index].row + offset_row,
        water_location[water_index].col + offset_col) == TRUE) {
        // Floods the specific tile
        flood_tile(map,
            water_location[water_index].row + offset_row,
            water_location[water_index].col + offset_col);
    }
}

///////////////////////////////////////////////////////////////////////////////

// Builds a teleporter at two input coordinates, removing path tiles between
// them
void build_teleporter(struct tile map[MAP_ROWS][MAP_COLUMNS], 
    struct coordinate path[MAX_PATH],
    int path_length) {
    // Scan in the input teleporter coordinates
    struct coordinate teleporter1;
    struct coordinate teleporter2;
    scanf("%d %d %d %d", &teleporter1.row, &teleporter1.col, &teleporter2.row,
        &teleporter2.col);

    // valid_teleporter defines the number of teleporter coordinate inputs that
    // are valid. Assigns this to equal 0
    int valid_teleporter = 0;

    // Checks that the input teleporters are not the same tile
    if (!((teleporter1.row == teleporter2.row) &&
        (teleporter1.col == teleporter2.col))) {
        // Increments valid_teleporter if the teleporters are on valid tiles
        valid_teleporter += valid_teleporter_check(map, path, path_length,
            teleporter1);
        valid_teleporter += valid_teleporter_check(map, path, path_length,
            teleporter2);
    }
    // If both tps are valid, spawn in teleporters and remove path tiles
    // between them
    if (valid_teleporter == BOTH_TP_VALID) {
        spawn_teleporters_on_path(map, path, path_length, teleporter1, 
            teleporter2);
    }
    // Else, print error message
    else {
        printf("Error: Teleporters can only be created on path tiles.\n");
    }
}

///////////////////////////////////////////////////////////////////////////////

// Checks if the input coordinate is valid for a teleporter
int valid_teleporter_check(struct tile map[MAP_ROWS][MAP_COLUMNS], 
    struct coordinate path[MAX_PATH],
    int path_length,
    struct coordinate teleporter) {
    // Incremenets along the path to ensure the tp is on a path tile
    for (int i = 0; i < path_length;ci++) {
        // If the tp is on a path tile and replaces a path tile, return true
        if ((path[i].row == teleporter.row) && 
            (path[i].col == teleporter.col) &&
            ((map[path[i].row][path[i].col].land == PATH_UP) ||
            (map[path[i].row][path[i].col].land == PATH_DOWN) ||
            (map[path[i].row][path[i].col].land == PATH_LEFT) ||
            (map[path[i].row][path[i].col].land == PATH_RIGHT))) {
            return TRUE;  
        }
    }
    // Else, return false
    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////

// Spawn the teleporters on the path, removing any path tiles between them
void spawn_teleporters_on_path(struct tile map[MAP_ROWS][MAP_COLUMNS], 
    struct coordinate path[MAX_PATH],
    int path_length,
    struct coordinate teleporter1,
    struct coordinate teleporter2) {
    // When between_teleporter = 0, we are not between the two teleporters
    // being spawned and do not need to remove path tiles
    // When between_teleporter = 1, we are between the two teleporters being
    // spawned and need to remove path tiles
    int between_teleporter = 0;

    // Increment along the path
    for (int i = 0; i < path_length;ci++) {
        // If we are not between the teleporters, check if the tile is to be
        // replaced with a tp
        if (between_teleporter == 0) {
            between_teleporter = not_between_teleporter_check(map, 
                path, teleporter1, teleporter2, between_teleporter, i);
        }
        // If we are between the teleporters, check if the tile is to be
        // replaced with a tp and replace path tiles with grass
        else if (between_teleporter == 1) {
            between_teleporter = between_teleporter_check(map, 
                path, teleporter1, teleporter2, between_teleporter, i);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////

// Checks if the tile is to be replaced with a teleporter and replaces it with
// a teleporter if necessary
int not_between_teleporter_check(struct tile map[MAP_ROWS][MAP_COLUMNS], 
    struct coordinate path[MAX_PATH],
    struct coordinate teleporter1,
    struct coordinate teleporter2,
    int between_teleporter,
    int i) {
    // If the current path tile equals to either of the tp coordinate inputs,
    // set the land type to teleporter and set between_teleporter to 1
    // to indicate that we are now between the teleporters
    if (((path[i].row == teleporter1.row) && 
        (path[i].col == teleporter1.col)) ||
        ((path[i].row == teleporter2.row) && 
        (path[i].col == teleporter2.col))) {
        map[path[i].row][path[i].col].land = TELEPORTER;

        between_teleporter = TRUE;
    }
    // Return the between_teleporter sentinel
    return between_teleporter;
}

///////////////////////////////////////////////////////////////////////////////

// Checks if the tile is to be replaced with a teleporter and replaces it with 
// a teleporter if necessary. Otherwise, replaces path tiles with grass tiles
int between_teleporter_check(struct tile map[MAP_ROWS][MAP_COLUMNS], 
    struct coordinate path[MAX_PATH],
    struct coordinate teleporter1,
    struct coordinate teleporter2,
    int between_teleporter,
    int i) {
    // If the current path tile equals to either of the tp coordinate inputs,
    // set the land type to teleporter and set between_teleporter to 0
    // to indicate that we are no longer between the teleporters
    if (((path[i].row == teleporter1.row) && 
        (path[i].col == teleporter1.col)) ||
        ((path[i].row == teleporter2.row) && 
        (path[i].col == teleporter2.col))) {
        map[path[i].row][path[i].col].land = TELEPORTER;
        
        between_teleporter = FALSE;
    }
    // Else, replace any land or teleporter tiles with grass tiles and clear
    // any enemies on the tile
    else if ((map[path[i].row][path[i].col].land == PATH_DOWN) ||
        (map[path[i].row][path[i].col].land == PATH_UP) ||
        (map[path[i].row][path[i].col].land == PATH_LEFT) ||
        (map[path[i].row][path[i].col].land == PATH_RIGHT) ||
        (map[path[i].row][path[i].col].land == TELEPORTER)) {
        map[path[i].row][path[i].col].land = GRASS;
        map[path[i].row][path[i].col].entity = EMPTY;
        map[path[i].row][path[i].col].n_enemies = 0;
    }
    // Return the between_teleporter sentinel
    return between_teleporter;
}

///////////////////////////////////////////////////////////////////////////////
/////////////////////////// PROVIDED FUNCTIONS  ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/**
 * Initialises map tiles to contain GRASS land and EMPTY entity.
 * 
 * Parameters:
 *     map - The map to initialise.
 * Returns:
 *     Nothing.
 */
void initialise_map(struct tile map[MAP_ROWS][MAP_COLUMNS]) {
    for (int row = 0; row < MAP_ROWc;++row) {
        for (int col = 0; col < MAP_COLUMNc;++col) {
            map[row][col].land = GRASS;
            map[row][col].entity = EMPTY;
            map[row][col].n_enemies = 0;
        }
    }
}

/**
 * Prints all map tiles based on their value, with a header displaying lives
 * and money.
 * 
 * Parameters:
 *     map   - The map to print tiles from.
 *     lives - The number of lives to print with the map.
 *     money - The amount of money to print with the map.
 * Returns:
 *     Nothing.
 */
void print_map(struct tile map[MAP_ROWS][MAP_COLUMNS], int lives, int money) {
    printf("\nLives: %d Money: $%d\n", lives, money);
    for (int row = 0; row < MAP_ROWS * c;++row) {
        for (int col = 0; col < MAP_COLUMNc;++col) {
            print_tile(map[row / 2][col], row % 2);
        }
        printf("\n");
    }
}

/**
 * Prints either the land or entity component of a single tile, based on
 * the `land_print` parameter;
 * 
 * Parameters:
 *     tile         - The tile to print the land/entity from
 *     land_print - Whether to print the land part of the tile or the entity
 *         part of the tile. If this value is 0, it prints the land, otherwise
 *         it prints the entity.
 * Returns:
 *     Nothing.
 */
void print_tile(struct tile tile, int land_print) {
    if (land_print) {
        if (tile.land == GRASS) {
            printf(" . ");
        }
        else if (tile.land == WATER) {
            printf(" ~ ");
        } 
        else if (tile.land == PATH_START) {
            printf(" S ");
        } 
        else if (tile.land == PATH_END) {
            printf(" E ");
        } 
        else if (tile.land == PATH_UP) {
            printf(" ^ ");
        } 
        else if (tile.land == PATH_RIGHT) {
            printf(" > ");
        } 
        else if (tile.land == PATH_DOWN) {
            printf(" v ");
        } 
        else if (tile.land == PATH_LEFT) {
            printf(" < ");
        } 
        else if (tile.land == TELEPORTER) {
            printf("( )");
        } 
        else {
            printf(" ? ");
        }
    } 
    else {
        if (tile.entity == EMPTY) {
            printf("   ");
        } 
        else if (tile.entity == ENEMY) {
            printf("%03d", tile.n_enemies);
        } 
        else if (tile.entity == BASIC_TOWER) {
            printf("[B]");
        } 
        else if (tile.entity == POWER_TOWER) {
            printf("[P]");
        } 
        else if (tile.entity == FORTIFIED_TOWER) {
            printf("[F]");
        } 
        else {
            printf(" ? ");
        }
    }
}
