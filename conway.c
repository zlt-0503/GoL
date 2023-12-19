#include "conway.h"

Grid* createGrid(int _x, int _y) {
    printf("1\n");
    Grid* newGrid = (Grid*)malloc(sizeof(Grid));
    if (newGrid == NULL) {
        printf("Memory allocation failed for createGrid.\n");
        exit(EXIT_FAILURE);
    }
    newGrid->x = _x;
    newGrid->y = _y;
    newGrid->Status = None;
    return newGrid;
}

void updateStatus(Grid* grid, Conway* current, Conway* next) {
    int numAlive = 0;
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    for (int i = 0; i < 8; i++) {
        int nx = grid->x + dx[i];
        int ny = grid->y + dy[i];
        if (nx >= 0 && nx < current->row && ny >= 0 && ny < current->column) {
            if (current->grids[nx][ny].Status == Alive) {
                numAlive++;
            }
        }
    }
    switch (current->grids[grid->x][grid->y].Status) {
        case Alive:
            if (numAlive < 2 || numAlive > 3) {
                next->grids[grid->x][grid->y].Status = Dead;
            }
            else
            {
                next->grids[grid->x][grid->y].Status = Alive;
            }
            break;
        case Dead:
            if (numAlive == 3) {
                next->grids[grid->x][grid->y].Status = Alive;
            }
            else
            {
                next->grids[grid->x][grid->y].Status = Dead;
            }
            break;
        case Other:
        case None:
            break;
        default:
            printf("Error Status");
            exit(1);
    }
}

void printStatus(Grid* grid) {
    printf("|");
    switch (grid->Status) {
        case Alive:
            printf("*");
            break;
        case Dead:
        case Other:
        case None:
            printf(" ");
            break;
        default:
            printf("Error Status");
            exit(1);
    }
}

Conway* createConway(const uint16_t _row, const uint16_t _column) {
    Conway* newConway = (Conway*)malloc(sizeof(Conway));
    if (newConway == NULL) {
        printf("Error.\n\n");
        exit(1);
    }
    newConway->row = _row + 2;
    newConway->column = _column + 2;
    newConway->grids = (Grid**)malloc((unsigned int)newConway->row * sizeof(Grid*));
    if (newConway->grids == NULL) {
        printf("Error.\n\n");
        exit(1);
    }
    for (int i = 0; i < newConway->row; i++) {
        newConway->grids[i] = (Grid*)malloc((unsigned int)newConway->column * sizeof(Grid));
    }
    for (int i = 0; i < newConway->row; i++) {
        for (int j = 0; j < newConway->column; j++) {
            newConway->grids[i][j].x = i;
            newConway->grids[i][j].y = j;
            newConway->grids[i][j].Status = None;
        }
    }
    return newConway;
}

void freeConway(Conway *c) {
    if (c != NULL) {
        for (int i = 0; i < c->row; i++) {
            free(c->grids[i]);
        }
        free(c->grids);
        free(c);
    }
}

void initConway(Conway *c) {
    static const int possibility = 50;
    for (int i = 1; i < c->row - 1; i++) {
        for (int j = 1; j < c->column - 1; j++) {
            const int r = rand() % 100;
            if (r < possibility) {
                c->grids[i][j].Status = Alive;
            } else {
                c->grids[i][j].Status = Dead;
            }
        }
    } 
}

void evolveConway(Conway **c) {
    Conway* nextConway = createConway((*c)->row - 2, (*c)->column - 2);
    for (int i = 1; i < (*c)->row - 1; i++) {
        for (int j = 1; j < (*c)->column - 1; j++) {
            updateStatus(&(*c)->grids[i][j], *c, nextConway);
        }
    }

    freeConway(*c);
    *c = nextConway;
}


void printConway(Conway *c) {
    clearscreen();
    for (int i = 1; i < c->row - 1; i++) {
        for (int j = 1; j < c->column - 1; j++) {
            printStatus(&c->grids[i][j]);
        }
        printf("|\n");
    }
}

void clearscreen(void) {
    #ifdef _WIN32
        system("cls");
    #elif defined(__linux__) || defined(__APPLE__)
        system("clear");
    #endif
}

int saveConway(const Conway *c, const char *path) {
    FILE* ofstream = fopen(path, "w");
    if (ofstream == NULL) {
        printf("Error while opening the file.\n");
        return 1;
    }
    fprintf(ofstream, "%d,%d\n", c->row - 2, c->column - 2);
    for (int i = 1; i < c->row - 1; i++) {
        for (int j = 1; j < c->column - 1; j++) {
            switch (c->grids[i][j].Status) {
                case Alive:
                    fprintf(ofstream, "1,");
                    break;
                case Dead:
                    fprintf(ofstream, "0,");
                    break;
                case Other:
                case None:
                default:
                    break;
            }
        }
        fprintf(ofstream, "\n");
    }
    fclose(ofstream);
    return 0;
}

void loadConway(Conway *c, const char *path) {
    FILE* ifstream = fopen(path, "r");
    if (ifstream == NULL) {
        printf("Error while opening the file.\n");
        exit(1);
    }
    char sizeLine[100];
    if (fgets(sizeLine, sizeof(sizeLine), ifstream) == NULL) {
        printf("Error reading the first line of the file.\n");
        fclose(ifstream);
        exit(1);
    }
    char ch;
    for (int i = 1; i <= c->row - 2; i++) {
        for (int j = 1; j <= c->column - 2; j++) {
            ch = fgetc(ifstream);
            if (ch == EOF) break;
            if (ch == '1') {
                c->grids[i][j].Status = Alive;
            } else if (ch == '0') {
                c->grids[i][j].Status = Dead;
            }
            while (ch != ',' && ch != '\n' && ch != EOF) {
                ch = fgetc(ifstream);
            }
            if (ch == EOF || ch == '\n') break;
        }
        while (ch != '\n' && ch != EOF) {
            ch = fgetc(ifstream);
        }
        if (ch == EOF) break;
    }

    fclose(ifstream);
}

void printHelp() {
    printf("###############################################################################################\n");
    printf("# i <row> <col>  |   initialize an empty grid with <row> rows and <col> columns               #\n");
    printf("# r              |   randomly set alive/dead states for all grids evolve into next generation #\n");
    printf("# n              |   automatically evolve, until receiving 'b' command                        #\n");
    printf("# b              |   pause evolution                                                          #\n");
    printf("# s <path>       |   save grid states to file <path>                                          #\n");
    printf("# l <path>       |   load grid status from file <path>                                        #\n");
    printf("# q              |   quit                                                                     #\n");
    printf("# h              |   help                                                                     #\n");
    printf("###############################################################################################\n");
}
