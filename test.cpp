#include "RSDL/src/rsdl.hpp"
#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

int main() {
    const int length_unit = 3 * 64;
    int width, height;
    cin >> width >> height;
    vector<string> filenames {"Tank1-Normal", 
			      "Tank1-Shield", 
			      "Tank1-Laser"};
    Window win(width * length_unit, height * length_unit, "testing RSDL");
    string Assets = "Assets/";
    string ext = ".png";
    // win.draw_rect(Rectangle(0,0,200,200));
    for(int i = 0; i < filenames.size(); i++) {
	for(int j = 0; j < 100; j++) {
	win.clear();
        win.draw_img(Assets + filenames[i] + ext,
		     Rectangle(2*j,0, 128, 128));
	win.update_screen();
	delay(40);
	}

    }
    win.clear();
    win.draw_img("Assets/Tank1-Shield.png", Rectangle(500,400,64,64));
    win.update_screen();
    delay(1500);
    while(true) {
        while(win.has_pending_event()) {
	    Event event = win.poll_for_event();
	    char c;
	    if(event.get_type() == Event::QUIT) {
	        return 0;	
            }
        }
    }
}
