/* Defines and prototypes for cline.c, part of Lawyer Race
   Copyright (C) 2012 Andreas Andersson

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* Prototypes */
char *expandpath(char *path);

/*
 * Some command line arguments outputs a lot of text. These strings are kept
 * here because they cludge the code. Below are:
 *      version
 *      usage
 *      keyfilehelp
 *      scorefilehelp
 *      plot
 * The program use puts() to display them and they shall therefore NOT end
 * with \n!
 */
static char *version = "Lawyer race version 0.1";

static char *usage = 
"Usage: lwrace [-l <0-9>] [-f <file>] [-k <file>]\n"
"       lwrace [-h [keyword] | -p | -s [-f <file>] | -v ]\n"
"Run terminal mode game Lawyer Race, an enhanced clone of an old QBasic game.\n"
"Default game control: use arrow keys to move and space to stop.\n\n"
"  -<0-9>, or         Set difficulty of the game. 0-2 = easy, 3 = default,\n"
"  --level <0-9>      4-9 = medium to hard. High scores are level dependent.\n"
"  -f  <file>, or     Read and store high scores to and from <file>.\n"
"  --scorefile <file> Type \"lwrace --help=scorefile\" for details.\n"
"  -h, --help         Display this help and exit.\n"
"  -k <file>, or      Use keys defined in <file> for game control.\n"
"  --keyfile <file>   Type \"lwrace --help=keyfile\" for details.\n"
"  -p, --plot         Display the original game plot and exit.\n"
"  -s, --scores       Display high scores and exit.\n"
"  -v, --version      Output version information and exit.\n\n"
"Report bugs to aa@mensa.se\n"
"Github project page: <https://github.com/nissafors/lwrace>";

static char *keyfilehelp = 
"There are seven possible actions in a lawyer race game: Move up, move down,\n"
"move left, move right, stop, pause and give up. By default these actions are\n"
"mapped to arrow keys, space, p and esc respectively. To remap keys, you can\n"
"use a keyfile. This is a textfile containing key map commands with syntax:\n\n"
"   action (whitespace) key\n\n"
"\"Action\" is one of the following: up, down, left, right, stop, pause or exit"
"\n\"Whitespace\" is tabs or spaces.\n"
"\"Key\" is either a character or a curses key constant (see curses.h).\n"
"Here's a sample keyfile to make things more clear:\n\n"
"   pause KEY_BREAK"
"   up    w\n"
"   down  s\n"
"   left  a\n"
"   right d\n"
"   exit  KEY_END\n\n"
"\"lwrace --keyfile /path/to/keyfile\" tells the program to use your keyfile.\n"
"Path may be relative to home or current working directory, thus the following"
"\nexamples are valid:\n\n"
"   lwrace -k keyfile\n"
"   lwrace -k ~/keyfile\n"
"   lwrace -k ./relative/path/to/keyfile\n"
"   lwrace -k /absolute/path/to/keyfile";

static char *scorefilehelp = 
"By default lawyer race writes high scores to files in ~/.lwrace/ directory.\n"
"Filenames are prepended with .<number>, where <number> is the level chosen\n"
"with the --level option. A player who has played on default level plus level\n"
"two and level six will have these files in his/her ~/.lwrace/ directory:\n\n"
"   hiscr.2  hiscr.3  hiscr.6\n\n"
"You can tell the program to store scores in a high score file of your choice\n"
"by issuing \"lwrace --scorefile /path/to/scorefile\". Path may be relative\n"
"to home or current working directory, thus the following examples are valid:\n"
"\n   lwrace -f scorefile\n"
"   lwrace -f ~/scorefile\n"
"   lwrace -f ./relative/path/to/scorefile\n"
"   lwrace -f /absolute/path/to/scorefile\n\n"
"Note that the level extension will always be added to the filename, thus the\n"
"command \"lwrace -l 5 -f myfile \" will produce the file \"myfile.5\"";

static char *plot =
"L A W Y E R   R A C E  -  The original QB game plot:\n"
"\"Once mr O needed a lawyer for some reason, and so he hired one. When the\n"
"case was lost and all mr O:s money was gone with the wind, the lawyer\n"
"office still wanted its fee, and they set after mr O, who had fled to the\n"
"mountains, looking for dollars. Rocks are falling all around him while mr O\n"
"is struggling to escape wild lawyers and to collect money enough to\nset him\n"
"free. Play the game to find out if mr O will make it!\"";
