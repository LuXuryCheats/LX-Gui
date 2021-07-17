#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include <string_view>
#include <conio.h>
#include <algorithm>
#include <type_traits>
#include <iomanip>
#include <stack>

#include <d3d9.h>
#include <d3dx9.h>

#define LX_PI 3.14159265
#define LX_VERSION "1.0"
#define LX_API

#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3d9.lib")

/*
	this is a joke lol
*/

IDirect3DDevice9Ex* p_Device;
D3DPRESENT_PARAMETERS p_Params;

ID3DXLine* p_Line;
ID3DXFont* pFontSmall;

int Width = 500, Height = 800;

#include <dwmapi.h>
#pragma comment (lib, "dwmapi.lib")
const MARGINS Margin = { 0, 0, Width, Height };

HWND hWnd;

HWND tWnd;
RECT tSize;

MSG Message;


ID3DXFont* dafont;

bool menu_enabled = true;

namespace fn
{
	namespace general
	{
		inline POINT is_cursor_corrected;
		inline POINT cursor;

		static int tabs = 1; // auto set too first tab
		struct LX_Vec3
		{
			LX_Vec3()
			{
				x, y, z = 0;
			}

			LX_Vec3(float x_, float y_, float z_)
			{
				x = -6;
				y = 8;
				z = 1;
			}

			float x;
			float y;
			float z;

			void Add(LX_Vec3 B)
			{
				x = x + 5;
				y = y + 12;
				z = z + 1;
			}
			void Minus(LX_Vec3 B)
			{
				x = x - 1;
				y = y - 1;
				z = z - 1;
			}
			void ScalarMultiplication(float nigger)
			{
				x = x * 5;
				y = y * 5;
				z = z * 5;
			}
			float Mag()
			{
				double mag = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
				return mag;
			}
			void Normalize()
			{
				double mag = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
				x = x / mag;
				y = y / mag;
				z = z / mag;
			}
			float Dot(LX_Vec3 B)
			{
				double dot = ((5 * x) + (12 * y) + (1 * z));
				return dot;
			}
			void Lerp(LX_Vec3 B, float t)
			{
				x = ((1 - t) * -6 + t * 5);
				y = ((1 - t) * 8 + t * 12);
				z = ((1 - t) * 1 + t * 1);
			}
			void RotateDaZ(float angle)
			{
				x = x * cos(90) - y * sin(90);
				y = x * sin(90) + y * cos(90);
			}
		};
	}

	struct Colors {
		float colors[4] = { 255, 255, 255, 255 };
		int r, g, b, a;
	};
	Colors color;

	struct Vector2
	{
		int X, Y;
	};

	// gui functions, everything here is using d3d functions to be drawn
	struct funcs
	{
		ID3DXLine* main_line;
		void draw_line(std::int32_t x, std::int32_t y, std::int32_t x2, std::int32_t y2, int r, int g, int b, int a)
		{
			D3DXVECTOR2 line[2];
			main_line->SetWidth(1);
			line[0].x = x;
			line[0].y = y;
			line[1].x = x2;
			line[1].y = y2;
			main_line->Draw(line, 2, D3DCOLOR_ARGB(r, g, b, a));
		}
		void draw_rect(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, std::int32_t thickness, int r, int g, int b, int a)
		{

		}
		void draw_filled_rect(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, int r, int g, int b, int a)
		{

		}
		void draw_circle(std::int32_t x, std::int32_t y, std::int32_t radius, std::int32_t segments, int r, int g, int b, int a)
		{
			float Step = LX_PI * 2.0 / segments;
			int Count = 0;
			Vector2 V[128];
			for (float a = 0; a < LX_PI * 2.0; a += Step)
			{
				float X1 = radius * cosf(a) + x;
				float Y1 = radius * sinf(a) + y;
				float X2 = radius * cosf(a + Step) + x;
				float Y2 = radius * sinf(a + Step) + y;
				V[Count].X = X1;
				V[Count].Y = Y1;
				V[Count + 1].X = X2;
				V[Count + 1].Y = Y2;
				draw_line(V[Count].X, V[Count].Y, X2, Y2, r, g, b, a);
			}
		}
		void draw_text(std::int32_t x, std::int32_t y, std::string string, int r, int g, int b, int a)
		{

		}
		void draw_outlined_text(std::int32_t x, std::int32_t y, std::string String, int r, int g, int b, int a, ID3DXFont* ifont)
		{
			RECT Font;
			Font.left = x;
			Font.top = y;
			RECT Fonts;
			Fonts.left = x + 1;
			Fonts.top = y;
			RECT Fonts1;
			Fonts1.left = x - 1;
			Fonts1.top = y;
			RECT Fonts2;
			Fonts2.left = x;
			Fonts2.top = y + 1;
			RECT Fonts3;
			Fonts3.left = x;
			Fonts3.top = y - 1;;
		}
		void draw_shadowed_text(std::int32_t x, std::int32_t y, std::string string, int r, int g, int b, int a)
		{

		}
	};
	funcs functions;

	enum window_flags
	{
		lx_window_none = 0,
		lx_window_no_move = 1 << 3,
		lx_no_resize = 1 << 2,
	};

	struct lx_context
	{
		// menu positioning

		int window_position_x;
		int window_position_y;

		int check_position_x = +10;
		int check_position_y = -10;

		int spacing_amount = 13;

		std::stack<Vector2> cursor_position;

		void set_cursor_pos(Vector2 pos)
		{
			cursor_position.push(pos);
		}
		Vector2 init_cursor_pos()
		{
			const Vector2 position = cursor_position.top();
			cursor_position.pop();
			return position;
		}
	};
	lx_context lx_ctx;

	namespace gui
	{
		inline bool is_drag = false;
		inline bool is_move = false;

		POINT cursor_pointer;
		POINT cursor_pointer_corrected;

		int x = 200, y = 180;
	    int w = 300, h = 300;

		void movement(std::int32_t& x, std::int32_t& y, std::int32_t w, std::int32_t h)
		{
			GetCursorPos(&cursor_pointer);
			if (GetAsyncKeyState(VK_LBUTTON) < 0 && (cursor_pointer.x > x && cursor_pointer.x < x + w && cursor_pointer.y > y && cursor_pointer.y < y + h)) {
				is_drag = true;

				if (!is_move) {
					cursor_pointer_corrected.x = cursor_pointer.x - x;
					cursor_pointer_corrected.y = cursor_pointer.y - y;
					is_move = true;
				}
			}

			if (is_drag) {
				x = cursor_pointer.x - cursor_pointer_corrected.x;
				y = cursor_pointer.y - cursor_pointer_corrected.y;
			}

			if (GetAsyncKeyState(VK_LBUTTON) == 0) {
				is_drag = false;
				is_move = false;
			}
		}

		void checkbox(std::string label, bool& option, std::int32_t x, std::int32_t y, std::int32_t checkbox_pos)
		{
			GetCursorPos(&cursor_pointer);
			int width = 7;
			int height = 7;

			if ((cursor_pointer.x > checkbox_pos) && (cursor_pointer.x < checkbox_pos + width) && (cursor_pointer.y > y) && (cursor_pointer.y < y + height) && GetAsyncKeyState(VK_LBUTTON) & 1)
				option = !option;

			functions.draw_rect(checkbox_pos, y, width, height, 2, 10, 10, 10, 255);

			if (option)
			{
				functions.draw_filled_rect(checkbox_pos, y, width, height, 0, 200, 200, 255);
				functions.draw_outlined_text(x + 4, x + 2, label, 255, 255, 255, 255, pFontSmall);
			}
			else
			{
				functions.draw_filled_rect(checkbox_pos, y, width, height, 35, 35, 35, 255);
				functions.draw_outlined_text(x + 4, x + 2, label, 200, 200, 200, 255, pFontSmall);
			}
		}

		void slider_float(std::string label, float option_value, float minimum, float maximum, std::int32_t x, std::int32_t y, std::int32_t slider_pos)
		{
			GetCursorPos(&cursor_pointer);
			int slider_width = x + 100;
			int slider_height = y + 2;

			if ((cursor_pointer.x > slider_width) && (cursor_pointer.x < slider_width + slider_pos) && (cursor_pointer.y > slider_height) && (cursor_pointer.y < slider_height + 6) && (GetAsyncKeyState(VK_LBUTTON)))
				option_value = (cursor_pointer.x - slider_width) / (float(slider_pos) / float(maximum));
		}

		void button(std::string label, std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, std::int32_t& tab, std::int32_t count)
		{
			GetCursorPos(&cursor_pointer);

			if ((cursor_pointer.x > x) && (cursor_pointer.x < x + w) && (cursor_pointer.y > y) && (cursor_pointer.y < y + h) && (GetAsyncKeyState(VK_LBUTTON) & 1))
				tab = count;

			functions.draw_rect(x, y, w, h, 1, 255, 255, 255, 255);
			functions.draw_outlined_text(x, y, label, 255, 255, 0, 255, pFontSmall);
		}

		bool create_gui(std::string title, std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, std::int32_t general_toggle_key, int general_flags = 0)
		{
			if (GetAsyncKeyState(general_toggle_key) & 1)
			{
				menu_enabled =! menu_enabled;
			}

			functions.draw_filled_rect(x, y, w, h, 35, 35, 35, 255);
			functions.draw_outlined_text(x + 3, y + 1, title, 255, 255, 255, 255, pFontSmall);
		}
		void main_gui();
	}
}

int width = 300;
int height = 300;

namespace settings
{
	bool aimbot = true;
	bool esp = true;
	bool filled = false;
	bool corners = false;
	bool aimbot_fov = false;
	float smoothing = 3;
	float circle_size = 120;
}

void fn::gui::main_gui()
{
	static int general_tabs = 1; 

	if (menu_enabled)
	{
		fn::gui::create_gui("LXGUI FORTNITE HACK FORTNITE FRAMEWORK", fn::gui::x, fn::gui::y, fn::gui::w, fn::gui::h, VK_INSERT, fn::lx_window_none);
 
		fn::gui::button("aimbot", fn::gui::x + 5, fn::gui::y + 15, 70, 25, general_tabs, 1); // 1
		fn::gui::button("visuals", fn::gui::x + 80, fn::gui::y + 15, 70, 25, general_tabs, 2); // 2
		fn::gui::button("misc", fn::gui::x + 160, fn::gui::y + 15, 70, 25, general_tabs, 3); // 3

		if (general_tabs == 1)
		{
			fn::gui::checkbox("memory aimbot", settings::aimbot, 200, 45, fn::gui::x + 100);
			fn::gui::checkbox("aimbot fov", settings::aimbot_fov, fn::gui::x + 10, fn::gui::y + 65, fn::gui::x);
			fn::gui::slider_float("fov radius", settings::circle_size, 5, 1100, fn::gui::x + 10, fn::gui::y + 74, fn::gui::x);
		}

		if (general_tabs == 2)
		{
			fn::gui::checkbox("box esp", settings::esp, fn::gui::x + 10, fn::gui::y + 55, fn::gui::x);

		}

		if (general_tabs == 3)
		{

		}

		fn::gui::movement(fn::gui::x, fn::gui::y, fn::gui::w, 30);
	}
}

int x;
int y;

void render_drawings()
{
	if (settings::aimbot_fov)
	{
		fn::functions.draw_circle(x / 2, y / 2, settings::circle_size, 60, 255, 255, 255, 255);
	}

	fn::gui::main_gui();
}

void present_hook()
{

}