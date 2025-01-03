#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

// Structure des voitures
typedef struct car {
    int number;
    int s1;
    int s2;
    int s3;
    int bt;
    int tt;
    bool has_crashed;
} car;

typedef struct best_time {
    int car_number;
    int time;
} best_time;

// Structure des courses
struct races {
    char* name;
    char* country;
    int nb_laps;
    int km_lap;
    int km_race;
    bool sprint;
};

// Structure des coureurs
struct drivers {
    int number;
    char* nickname;
    char* first_name;
    char* last_name;
    int team_number;
    int points;
};

// Structure des écuries
struct teams {
    int number;
    char* name;
    char* full_name;
};

// Courses
struct races race1  = {"Bahrain International Circuit",      "Bahreïn",        57, 5412, 308238, false};
struct races race2  = {"Jeddah Corniche Circuit",            "Arabie saoudite",50, 6174, 308450, false};
struct races race3  = {"Albert Park Circuit",                "Australie",      58, 5278, 306124, false};
struct races race4  = {"Baku City Circuit",                  "Azerbaïdjan",    51, 6003, 306049, true};
struct races race5  = {"Miami International Autodrome",      "Etats-Unis",     57, 5412, 308326, false};
struct races race6  = {"Circuit de Monaco",                  "Monaco",         78, 3337, 260286, false};
struct races race7  = {"Circuit de Barcelona-Catalunya",     "Espagne",        66, 4675, 308550, false};
struct races race8  = {"Circuit Gilles-Villeneuve",          "Canada",         70, 4361, 305270, false};
struct races race9  = {"Red Bull Ring",                      "Autriche",       71, 4318, 306452, true};
struct races race10 = {"Silverstone Circuit",                "Grande-Bretagne",52, 5891, 306198, false};
struct races race11 = {"Hungaroring",                       "Hongrie",        70, 4381, 306630, false};
struct races race12 = {"Circuit de Spa-Francorchamps",       "Belgique",       44, 7004, 308052, true};
struct races race13 = {"Circuit Zandvoort",                  "Pays-Bas",       72, 4259, 306587, false};
struct races race14 = {"Autodromo Nazionale Monza",          "Italie",         53, 5793, 306720, false};
struct races race15 = {"Marina Bay Street Circuit",          "Singapour",      62, 5063, 313906, false};
struct races race16 = {"Suzuka International Racing Course", "Japon",          53, 5807, 307471, false};
struct races race17 = {"Lusail International Circuit",       "Qatar",          57, 5418, 308826, true};
struct races race18 = {"Circuit of The Americas",            "Etats-Unis",     56, 5513, 308405, true};
struct races race19 = {"Autódromo Hermanos Rodríguez",       "Mexique",        71, 4304, 305354, false};
struct races race20 = {"Autódromo José Carlos Pace",         "Brésil",         71, 4309, 305879, true};
struct races race21 = {"Las Vegas",                          "Etats-Unis",     50, 6201, 310050, false};
struct races race22 = {"Yas Marina Circuit",                 "Emirats arabes unis", 58, 5281, 306183, false};
struct races race23 = {"Imola",                              "Italie",         63, 4909, 308000, false};
struct races race24 = {"Shanghai International Circuit",     "Chine",          56, 5451, 305256, false};
struct races race25 = {"Sepang International Circuit",       "Malaisie",       56, 5543, 310408, false};

// Coureurs
struct drivers driver1  = { 1,  "VER", "Max",      "Verstappen", 1,  0};
struct drivers driver2  = { 11, "PER", "Sergio",   "Perez",      1,  0};
struct drivers driver3  = { 44, "HAM", "Lewis",    "Hamilton",   2,  0};
struct drivers driver4  = { 63, "RUS", "George",   "Russell",    2,  0};
struct drivers driver5  = { 16, "LEC", "Charles",  "Leclerc",    3,  0};
struct drivers driver6  = { 55, "SAI", "Carlos",   "Sainz",      3,  0};
struct drivers driver7  = { 4,  "NOR", "Lando",    "Norris",     4,  0};
struct drivers driver8  = { 81, "PIA", "Oscar",    "Piastri",    4,  0};
struct drivers driver9  = { 14, "ALO", "Fernando", "Alonso",     5,  0};
struct drivers driver10 = { 18, "STR", "Lance",    "Stroll",     5,  0};
struct drivers driver11 = { 10, "GAS", "Pierre",   "Gasly",      6,  0};
struct drivers driver12 = { 31, "OCO", "Esteban",  "Ocon",       6,  0};
struct drivers driver13 = { 23, "ALB", "Alexander","Albon",      7,  0};
struct drivers driver14 = { 2,  "SAR", "Logan",    "Sargeant",   7,  0};
struct drivers driver15 = { 22, "TSU", "Yuki",     "Tsunoda",    8,  0};
struct drivers driver16 = { 3,  "RIC", "Daniel",   "Ricciardo",  8,  0};
struct drivers driver17 = { 77, "BOT", "Valtteri", "Bottas",     9,  0};
struct drivers driver18 = { 24, "ZHO", "Guanyu",   "Zhou",       9,  0};
struct drivers driver19 = { 20, "MAG", "Kevin",    "Magnussen",  10, 0};
struct drivers driver20 = { 27, "HUL", "Nico",     "Hulkenberg", 10, 0};

// Écuries
struct teams team1  = { 1,  "Red Bull",       "Oracle Red Bull Racing"};
struct teams team2  = { 2,  "Mercedes",       "Mercedes-AMG PETRONAS F1 Team"};
struct teams team3  = { 3,  "Ferrari",        "Scuderia Ferrari"};
struct teams team4  = { 4,  "McLaren",        "McLaren Formula 1 Team"};
struct teams team5  = { 5,  "Aston Martin",   "Aston Martin Aramco Cognizant F1 Team"};
struct teams team6  = { 6,  "Alpine",         "BWT Alpine F1 Team"};
struct teams team7  = { 7,  "Williams",       "Williams Racing"};
struct teams team8  = { 8,  "Visa RB",        "Visa Cash App RB"};
struct teams team9  = { 9,  "Stake F1",       "Stake F1 Team"};
struct teams team10 = { 10, "Haas",           "MoneyGram Haas F1 Team"};
