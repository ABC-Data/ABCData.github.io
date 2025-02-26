/*!*************************************************************************
\file   Keycodes.h
\author Anderson Phua
\email: a.phua@digipen.edu
\date   27-09-22
\brief  This header file contains the Keycodes used in the Input system, as
		well as the input object used for input handling.
		
Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*****************************************************************************/
#pragma once
#include <map>
typedef uint16_t KeyCode;
typedef KeyCode Keycode;

namespace Key
{
	enum : uint16_t
	{
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		// Lowercase A to Z
		A = 65, B = 66, C = 67, D = 68, E = 69, F = 70,
		G = 71, H = 72, I = 73, J = 74, K = 75, L = 76,
		M = 77, N = 78, O = 79, P = 80, Q = 81, R = 82,
		S = 83, T = 84, U = 85, V = 86, W = 87, X = 88,
		Y = 89, Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		Tilde = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		// Arrow keys
		Right = 262, Left = 263, Down = 264, Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		
		F1 = 290, F2 = 291, F3 = 292, F4 = 293, F5 = 294,
		F6 = 295, F7 = 296, F8 = 297, F9 = 298, F10 = 299,
		F11 = 300, F12 = 301, F13 = 302, F14 = 303, F15 = 304,
		F16 = 305, F17 = 306, F18 = 307, F19 = 308, F20 = 309,
		F21 = 310, F22 = 311, F23 = 312, F24 = 313, F25 = 314,

		/* Keypad */
		KP0 = 320, KP1 = 321, KP2 = 322, KP3 = 323, KP4 = 324,
		KP5 = 325, KP6 = 326, KP7 = 327, KP8 = 328, KP9 = 329,
		
		KPDecimal = 330, KPDivide = 331, KPMultiply = 332,
		KPSubtract = 333, KPAdd = 334, KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340, LeftControl = 341, LeftAlt = 342, LeftSuper = 343,
		RightShift = 344, RightControl = 345, RightAlt = 346, RightSuper = 347,
		Menu = 348,

		Unknown = 349
	};

	static std::map<Keycode, const char*> Keymap
	{
		{Key::Space, "Space"}, {Key::Apostrophe, "'"}, {Key::Comma, ","}, 
		{Key::Minus, "-"}, {Key::Period, "."}, {Key::Slash, "/"},
	
		// Numbers
		{Key::D0, "0"}, {Key::D1, "1"}, {Key::D2, "2"}, {Key::D3, "3"}, {Key::D4, "4"},
		{Key::D5, "5"}, {Key::D6, "6"}, {Key::D7, "7"}, {Key::D8, "8"}, {Key::D9, "9"},
	
		{Key::Semicolon, ";"},
		{Key::Equal, "="},
		// A to Z
		{Key::A, "A"}, {Key::B, "B"}, {Key::C, "C"}, {Key::D, "D"}, {Key::E, "E"},
		{Key::F, "F"}, {Key::G, "G"}, {Key::H, "H"}, {Key::I, "I"}, {Key::J, "J"},
		{Key::K, "K"}, {Key::L, "L"}, {Key::M, "M"}, {Key::N, "N"}, {Key::O, "O"},
		{Key::P, "P"}, {Key::Q, "Q"}, {Key::R, "R"}, {Key::S, "S"}, {Key::T, "T"},
		{Key::U, "U"}, {Key::V, "V"}, {Key::W, "W"}, {Key::X, "X"}, {Key::Y, "Y"},
		{Key::Z, "Z"},

		// Function keys 0
		{Key::LeftBracket, "["}, {Key::Backslash, "\\"}, {Key::RightBracket, "]"}, {Key::Tilde, "`"},
		{Key::World1, "World1"}, {Key::World2, "World2"}, // non-US #1 and #2
		{Key::Escape, "Escape"}, {Key::Enter, "Enter"},
		{Key::Tab, "Tab"}, {Key::Backspace, "Backspace"},
		{Key::Insert, "Insert"}, {Key::Delete, "Delete"},

		// Arrow keys
		{Key::Right, "Right"}, {Key::Left, "Left"}, {Key::Down, "Down"}, {Key::Up, "Up"},
	
		// Function keys 1
		{Key::PageUp, "Page Up"}, {Key::PageDown, "Page Down"}, {Key::Home, "Home"}, {Key::End, "End"},
		{Key::CapsLock, "Caps Lock"}, {Key::ScrollLock, "Scroll Lock"}, {Key::NumLock, "NumLock"},
		{Key::PrintScreen, "Print Screen"}, {Key::Pause, "Pause"},
	
		// Function keys 2
		{Key::F1, "F1"}, {Key::F2, "F2"}, {Key::F3, "F3"}, {Key::F4, "F4"}, {Key::F5, "F5"},
		{Key::F6, "F6"}, {Key::F7, "F7"}, {Key::F8, "F8"}, {Key::F9, "F9"}, {Key::F10, "F10"},
		{Key::F11, "F11"}, {Key::F12, "F12"}, {Key::F13, "F13"}, {Key::F14, "F14"}, {Key::F15, "F15"},
		{Key::F16, "F16"}, {Key::F17, "F17"}, {Key::F18, "F18"}, {Key::F19, "F19"}, {Key::F20, "F20"},
		{Key::F21, "F21"}, {Key::F22, "F22"}, {Key::F23, "F23"}, {Key::F24, "F24"}, {Key::F25, "F25"},

		// Keypad
		{Key::KP0, "KP0"}, {Key::KP1, "KP1"}, {Key::KP2, "KP2"}, {Key::KP3, "KP3"}, {Key::KP4, "KP4"},
		{Key::KP5, "KP5"}, {Key::KP6, "KP6"}, {Key::KP7, "KP7"}, {Key::KP8, "KP8"}, {Key::KP9, "KP9"},
		{Key::KPDecimal, "KP Decimal"}, {Key::KPDivide, "KP Divide"}, {Key::KPMultiply, "KP Multiply"},
		{Key::KPSubtract, "KP Subtract"}, {Key::KPAdd, "KP Add"}, {Key::KPEnter, "KP Enter"}, {Key::KPEqual, "KP Equal"},
	
		// Modifiers - may be utilized in comination with other keys
		{Key::LeftShift, "Left Shift"}, {Key::LeftControl, "Left Control"}, {Key::LeftAlt, "Left Alt"}, {Key::LeftSuper, "Left Super"},
		{Key::RightShift, "Right Shift"}, {Key::RightControl, "Right Control"}, {Key::RightAlt, "Right Alt"}, {Key::RightSuper, "Right Super"},
		{Key::Menu, "Menu"},

		{Key::Unknown, "Unknown"}
	};
}

struct KeyInput
{
	unsigned int key {Key::Unknown}, lastKey{key}; // current and last key pressed
	unsigned int action{ 3 }; // pressed / released
	
	unsigned int mods{ 0 }; // checks if the SHIFT / CTRL / ALT / WINDOWS / CAPSLOC / NUMLOCK key is entered
	short inputMode{ -1 }; // input mode
	int scanCode{ 0 }; // platform-specific
	bool triggered{ false };

	bool operator==(const KeyInput& rhs) const {return key == rhs.key || action == rhs.action || mods == rhs.mods || scanCode == rhs.scanCode;}
	//const char* GetKeymapName(Keycode kc) { return Key::Keymap[kc];}
};