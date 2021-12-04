#include "glob.h"
#include <regex.h>

#define REGNUM 11
#define LINE_COUNT 100

int read_opts_file ARGS((char[LINE_COUNT][STRING_LEN]));
void gen_regex ARGS((regex_t[REGNUM]));
int check_valid_opts ARGS((char[LINE_COUNT][STRING_LEN],int,char[LINE_COUNT][STRING_LEN],regex_t[REGNUM]));
void set_option ARGS((char,int));
void parse_opts ARGS((char[LINE_COUNT][STRING_LEN],int));


void load_omegaopts(void) 
{
  char fopts[LINE_COUNT][STRING_LEN];
  char vopts[LINE_COUNT][STRING_LEN];
  regex_t regex[REGNUM];
  int optsize, voptsize;

  optsize=read_opts_file(fopts);
  if (optsize == 0) return;
  gen_regex(regex);

  voptsize=check_valid_opts(fopts, optsize, vopts, regex);
  for (int i=0; i<REGNUM; i++) { regfree(&regex[i]); }
    
  parse_opts(vopts, voptsize);

  return 0;
}

/* read file options from ~/.omegaopts. may contain invalid entries */
int read_opts_file(fopts)
char fopts[LINE_COUNT][STRING_LEN];
{
  FILE *fp;
  char *Opt=NULL;
  size_t linecapp=0;
  ssize_t linelen;
  int i=0;

  char fname[STRING_LEN];
  sprintf(fname, "%s/.omegaopts", getenv("HOME"));
  if ((fp=fopen(fname, "r")) == NULL) { return 0; }

  while ((linelen = getline(&Opt, &linecapp, fp)) != -1) {
    if (Opt[linelen-1] == '\n') { Opt[linelen-1]='\0'; }
    strcpy(fopts[i++], Opt);
  }

  free(Opt);
  fclose(fp);
  return i;
}

/* compile regex array for parsing options */
void gen_regex(regex)
regex_t regex[REGNUM];
{
  char TF[]="((T(RUE)?)|(F(ALSE)?))$";
  char OPS[][STRING_LEN]={
    "^BELLICOSE=",
    "^JUMPMOVE=",
    "^RUNSTOP=",
    "^PICKUP=",
    "^CONFIRM=",
    "^TOPINV=",
    "^PACKADD=",
    "^COMPRESS=",
    "^COLOUR=",
    "^VERBOSITY=((T(ERSE)?)|(M(EDIUM)?)|(V(ERBOSE)?))$",
    "^SEARCHNUM=[1-9]$"
  };
  for (int i=0; i<REGNUM; i++) {
    if (i < REGNUM-2) strcat(OPS[i], TF);
    if ((regcomp(&regex[i], OPS[i], REG_EXTENDED)) != 0) {
      fprintf(stderr, "%s\n", "REGEX FAILED TO COMPILE. EXITING...");
      exit(1);
    }
  }
}

/* filter out invalid opts from .omegaopts */
int check_valid_opts(fopts, optsize, vopts, regex)
char fopts[LINE_COUNT][STRING_LEN]; // all options from .omegaopts file
int optsize;
char vopts[LINE_COUNT][STRING_LEN]; // valid options parsed from fopts
regex_t regex[];
{
  int os=optsize>LINE_COUNT? LINE_COUNT : optsize;
  int val=0;
  int k=0;

  for (int i=0; i<os; i++) {
    for (int j=0; j<REGNUM; j++) {
      val=regexec(&regex[j], fopts[i], 0, NULL, 0);
      if (val == 0) break;
    }
    if (val == 0) { strcpy(vopts[k++], fopts[i]); }
  }
  return k;
}

void set_option(tf, option) 
char tf;
int option;
{
  switch (tf) {
    case 'T': optionset(option); break;
    case 'F': optionreset(option);
  }
}

/* parse omegaopts and set options game options */
void parse_opts(opts, voptsize)
char opts[LINE_COUNT][STRING_LEN];
int voptsize;
{
  char *opt, *val, c;
  for(int i=0; i<voptsize; i++) {
    opt=strtok(opts[i], "=");
    val=strtok(NULL, "=");
    c=val[0];
    if ((strcmp(opt, "BELLICOSE")) == 0) { set_option(c, BELLICOSE); }
    else if ((strcmp(opt, "JUMPMOVE")) == 0) { set_option(c, JUMPMOVE); }
    else if ((strcmp(opt, "RUNSTOP")) == 0) { set_option(c, RUNSTOP); }
    else if ((strcmp(opt, "PICKUP")) == 0) { set_option(c, PICKUP); }
    else if ((strcmp(opt, "CONFIRM")) == 0) { set_option(c, CONFIRM); }
    else if ((strcmp(opt, "TOPINV")) == 0) { set_option(c, TOPINV); }
    else if ((strcmp(opt, "PACKADD")) == 0) { set_option(c, PACKADD); }
    else if ((strcmp(opt, "COMPRESS")) == 0) { set_option(c, COMPRESS_OPTION); }
    else if ((strcmp(opt, "COLOUR")) == 0) { set_option(c, SHOW_COLOUR); }
    else if ((strcmp(opt, "VERBOSITY")) == 0) {
      switch(c) {
        case 'T': Verbosity=TERSE; break;
        case 'M': Verbosity=MEDIUM; break;
        case 'V': Verbosity=VERBOSE; break;
      }
    }
    else if ((strcmp(opt, "SEARCHNUM")) == 0) {
      Searchnum=atoi(val);
    }
  }
}
