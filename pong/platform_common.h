#ifndef PLATFORM_COMMON_H
#define PLATFORM_COMMON_H

struct {
	unsigned int isDown;
	unsigned int changed;
} typedef Button;

enum {
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_PAUSE,
	BUTTON_RESTART,
	BUTTON_COUNT
};

struct {
	int mouseX;
	int mouseY;
	Button buttons[BUTTON_COUNT];
} typedef Input;

#define Pressed(b) (input->buttons[b].isDown && input->buttons[b].changed)
#define Released(b) (!input->buttons[b].isDown && input->buttons[b].changed)
#define IsDown(b) (input->buttons[b].isDown)

#endif