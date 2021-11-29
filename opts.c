#include "glob.h"

void load_omegaopts(void) 
{
  char fopts[100][100];
  char vopts[100][100];
  regex_t regex[REGNUM];
  int optsize, voptsize;

  optsize=read_opts_file(fopts);
  if (optsize == 0) return;
  gen_regex(regex); // free eventually?

  voptsize=check_valid_opts(fopts, optsize, vopts, regex);
  parse_opts(vopts, voptsize);

  return 0;
}

/* read file options from ~/.omegaopts. may contain invalid entries */
int read_opts_file(fopts)
char fopts[100][100];
{
  FILE *fp;
  char *Opt=NULL;
  size_t linecapp=0;
  ssize_t linelen;
  int i=0;

  char fname[80];
  sprintf(fname, "%s/.omegaopts", getenv("HOME"));
  if ((fp=fopen(fname, "r")) == NULL) { return 0; }

  while ((linelen = getline(&Opt, &linecapp, fp)) != -1) {
    Opt[linelen-1]='\0';
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
  char TF[100]="((T(RUE)?)|(F(ALSE)?))$";
  char OPS[][100]={
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
char fopts[100][100]; // all options from .omegaopts file
int optsize;
char vopts[100][100]; // valid options parsed from fopts
regex_t regex[];
{
  int os=optsize>100? 100 : optsize;
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

/* parse omegaopts and set options game options */
void parse_opts(opts, voptsize)
char opts[100][100];
int voptsize;
{
  char *opt, *val, c;
  for(int i=0; i<voptsize; i++) {
    opt=strtok(opts[i], "=");
    val=strtok(NULL, "=");
    c=val[0];
    if ((strcmp(opt, "BELLICOSE")) == 0) {
      if (c == 'T') optionset(BELLICOSE); else optionreset(BELLICOSE);
    }
    else if ((strcmp(opt, "JUMPMOVE")) == 0) {
      if (c == 'T') optionset(JUMPMOVE); else optionreset(JUMPMOVE);
    }
    else if ((strcmp(opt, "RUNSTOP")) == 0) {
      if (c == 'T') optionset(RUNSTOP); else optionreset(RUNSTOP);
    }
    else if ((strcmp(opt, "PICKUP")) == 0) {
      if (c == 'T') optionset(PICKUP); else optionreset(PICKUP);
    }
    else if ((strcmp(opt, "CONFIRM")) == 0) {
      if (c == 'T') optionset(CONFIRM); else optionreset(CONFIRM);
    }
    else if ((strcmp(opt, "TOPINV")) == 0) {
      if (c == 'T') optionset(TOPINV); else optionreset(TOPINV);
    }
    else if ((strcmp(opt, "PACKADD")) == 0) {
      if (c == 'T') optionset(PACKADD); else optionreset(PACKADD);
    }
    else if ((strcmp(opt, "COMPRESS")) == 0) {
      if (c == 'T') optionset(COMPRESS_OPTION); else optionreset(COMPRESS_OPTION);
    }
    else if ((strcmp(opt, "COLOUR")) == 0) {
      if (c == 'T') optionset(SHOW_COLOUR); else optionreset(SHOW_COLOUR);
    }
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
