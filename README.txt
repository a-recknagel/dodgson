Current Changes

Last Update: 12.08.2014
 - Reuploaded the project after fixing a couple of minor bugs
 - Fixed a major bug with multithreading
 
 Coming Updates
 - Adding Thesis in the scope of which this project was developed
 
 
 
+-----------+ 
|   Usage   |
+-----------+
The whole project can be downloaded from the DodgsonScoring folder. The
source-code can be consulted for documentation, but as this can often be 
cumbersome, I will provide an overview and guide in this textfile.

Content:
The project consists of a Util-class, which collects general-purpose and shared
structures and functions. DodgsonScorer is the base class for all Rules which
implements the Dodgson Method. The class itself contains everything neccessary
for running the scorer, safe the rule implementation itself. 
If properly added to the Menu class (instructions are added as comments at the 
relevant parts, one in Menu.h, add enum value, and 4 in Menu.cpp follow
instructions), a new scorer can be tested with the Menu functions. A test case
is given in the main.cpp file.

Interface:
A graphical representation of the menu can be found in this folder. Most
functions should be self-explanatory. Else, the documentation in Menu.h can be
checked. After creating a Menu instance, that instance's mainMenu-function 
needs to be called in order to start interaction.

Solving scores:
Random preference profiles of variable size can be generated via the menu. If a
certain profile is supposed to be solved, it can be supplied to the menu by 
naming it 'prefProfxyz.txt', where xyz is a number from 1 to 99. The finished
textfile must then be placed in a folder 'Data'. The schema which needs to be 
followed is the following, examplary for a n=3, m=4 profile:
******************** start of file
3 4
0 1 2 3
3 2 0 1
1 0 3 2
scDs: -1 -1 -1 -1
******************* end of file
The first line contains n and m, seperated by a whitespace. The following n
lines contain the m alternatives, in their prefered order, where the first
mentioned is the first choice etc. The last line contains the dodgson scores.
Starting with -1 means, that the score is currently unknown. After a solution
is found, "save" can  be called in order to write the solution to the file 
system.

Implementing a new scorer:
After correctly importing the new scorer into the Menu class, it needs to be
supplied with at least one function, which is 'int getSCD(int a)'. There is 
another true virtual function, 'void getFastSCD(int a)', which implements
multi processing aided scoring, but an empty implementation can be given here,
and m_supportsThreads should then be set to 'false' in the constructor.
If used correctly, and chosen in the menu via 'choose heuristic', the scorer
will gain access to the currently used preference profile with its pointer to
it, called m_pp. It points to an instance of Menu::preferenceProfile, which 
offers a straigh forward implementation of preference profiles. This field can
then be used to find the dodgson score.

Debugging:
After implementing at least getSCD, the scorer can be tested for correctness.
The function 'test' in the menu is supposed to do that, but it will propably
need to be altered in some way for the new scorer.

Benchmarking:
The benchmarking functions are more complete than the debugging, and should
offer ample opportunities to test all scorers on their performance both on 
their simple getSCD via normal solving, as well as their getFastSCD via 
threaded solving.

Code and Algorithm description:
The Thesis will cover this subject at length, so I wont repeat it here.




For any questions or suggestions regarding the code or this readme, please
contact me at arecknag@uos.de. I am also happy to hear about any new
implementations, feel free to drop a line if you try to or already managed to
get one to work.