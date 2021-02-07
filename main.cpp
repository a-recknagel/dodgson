#include <iostream>
#include "Baseline.h"
#include "Util.h"
#include "Menu.h"

#include <list>


void change(bool& a) {
	a = false;
}


int main() {
	

	// some initializiation, 8x5 is a good size
	Util::preferenceProfile pp(8, 5);
	Util::scramble(pp);


	Menu menu(pp);
	menu.mainMenu();
}


