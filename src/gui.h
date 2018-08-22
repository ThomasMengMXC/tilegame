#ifndef GUI_H
#define GUI_H

#include <theatre/layer.h>

class Gui {
	public:
		Gui(void);
		~Gui(void);

		void draw(Layer *layer);
		void draw(void);
};

#endif
