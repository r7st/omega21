#include "glob.h"

#define NUM_CLASSES 8
#define BASE_STAT 12

#define STRENGTH 0
#define CONSTITUTION 1
#define DEXTERITY 2
#define AGILITY 3
#define INTELLIGENCE 4
#define POWER 5

void calc_mod ARGS((int*));
void init_stats ARGS((int*));
void init_leper ARGS((void));
int show_classes ARGS((void));


void calc_mod(player_class) 
int *player_class;
{
  for (int i=0; i<NUM_CLASSES; i++) {
    player_class[i]+=BASE_STAT;
  }
}

void init_stats(player_class) 
int *player_class;
{
  calc_mod(player_class);
  Player.str=Player.maxstr=player_class[STRENGTH];
  Player.con=Player.maxcon=player_class[CONSTITUTION];
  Player.dex=Player.maxdex=player_class[DEXTERITY];
  Player.agi=Player.maxagi=player_class[AGILITY];
  Player.iq=Player.maxiq=player_class[INTELLIGENCE];
  Player.pow=Player.maxpow=player_class[POWER];
  Player.cash = random_range(100)+random_range(100)+
    random_range(100)+random_range(100)+random_range(100);
}

void init_leper(void) {
  Player.cash=random_range(30);
  Player.status[DISEASED] = 1100;
  Player.dex-=5;
}

int show_classes(void) {
  char *CLASSES[]={
    "soldier",
    "scholar",
    "burglar",
    "laborer",
    "troubadour",
    "peasant",
    "noble",
    "leper"
  };

  char response;
  printw("%s\n", "Choose your class:");
  for (int i=0; i<NUM_CLASSES; i++) {
    printw("%c: %s\n", i+'a', CLASSES[i]);
  }
  printw("> ");
  refresh();
  do response = (char) mcigetc(); while (((response < 'a') || (response >= 'a'+NUM_CLASSES)));
  return response;
}

void choose_class(void) {
  char response;
  // str, con, dex, agi, iq, pow
  int Soldier[]={4, 3, 1, 0, -4, -4};
  int Scholar[]={-4, -2, -1, -1, 4, 4};
  int Burglar[]={-1, 0, 4, 2, -3, -2};
  int Laborer[]={3, 6, 0, -3, -2, -4};
  int Troubadour[]={1, 0, 2, 5, -4, -4};
  int Peasant[]={0, 0, 0, 0, 0, 0};
  int Noble[]={-2, -2, 2, 2, 0, -2}; // -2
  int Leper[]={0, -4, 0, 0, 0, 2}; // -2

  response=show_classes(); 
  switch (response) {
    case 'a': init_stats(Soldier); break; 
    case 'b': init_stats(Scholar); break; 
    case 'c': init_stats(Burglar); break; 
    case 'd': init_stats(Laborer); break; 
    case 'e': init_stats(Troubadour); break; 
    case 'f': init_stats(Peasant); break;
    case 'g': init_stats(Noble); Player.cash+=1000; break; 
    case 'h': init_stats(Leper); init_leper(); break;
    default: fprintf(stderr, "%s\n", "ERROR: INVALID PLAYER CLASS"); exit(1);
  };
  calc_melee();
}
