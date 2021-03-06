/**
 * @file widget.hpp
 * @brief Defines widgets that may be put on the display.
 *
 * Copyright (C) 2019 Clyne Sullivan
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef WIDGET_HPP_
#define WIDGET_HPP_

class Adafruit_GFX;

#include <bluefruit.h>
#include <cstring>

enum class PressAction {
	Nothing = 0,
	Destroy,
	Fullscreen
};

class Widget {
private:
	unsigned int height;
public:
	constexpr Widget(unsigned int _height = 0)
		: height(_height) {}
	
	inline unsigned int getHeight(void) const {
		return height;
	}

	virtual void renderFullscreen([[maybe_unused]] Adafruit_GFX& display,
		[[maybe_unused]] int ypos) {}
	virtual void render(Adafruit_GFX& display, int ypos) = 0;
	virtual PressAction onPress(void) = 0;

protected:
	inline void setHeight(unsigned int _height) {
		height = _height;
	}
};

class TimeWidget : public Widget {
private:
	unsigned int prevTicks;

public:
	constexpr TimeWidget(void)
		: Widget(30), prevTicks(0) {}

	void render(Adafruit_GFX& display, int ypos) final;
	void renderFullscreen(Adafruit_GFX& display, int ypos) final;
	PressAction onPress(void) final;
};

class NotificationWidget : public Widget {
private:
	char message[36];

public:
	NotificationWidget(const char *msg)
		: Widget(48) {
		unsigned int i;
		for (i = 0; msg[i] != '\0' && i < 35; i++)
			message[i] = msg[i];
		message[i] = '\0';
		setHeight(16 * (i / 12 + 1));
	}

	void render(Adafruit_GFX& display, int ypos) final;
	PressAction onPress(void) final {
		return PressAction::Destroy;
	}
};

class NoteWidget : public Widget {
private:
	char *text;

public:
	NoteWidget(const char *t, unsigned int size)
		: Widget(24) {
		text = new char[size];
		for (unsigned int i = 0; i < size; i++)
			text[i] = t[i];
	}
	~NoteWidget(void) {
		delete[] text;
	}

	void render(Adafruit_GFX& display, int ypos) final;
	void renderFullscreen(Adafruit_GFX& display, int ypos) final;
	PressAction onPress(void) final;
};

#endif // WIDGET_HPP_
