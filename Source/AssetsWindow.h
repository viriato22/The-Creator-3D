#pragma once
#include "Window.h"

class Texture;

class AssetsWindow :
	public Window
{

public:

	AssetsWindow();
	virtual ~AssetsWindow();

	void DrawWindow();

private:
	void DrawChilds(std::string path);
	void DeleteWindow(std::string path);
	void CreateDirectortWindow();

private:
	uint node;
	char node_name[150];
	bool show_new_folder_window;
	bool file_options_open;
	bool show_delete_window;

	Texture* texture_icon;
	Texture* mesh_icon;
	Texture* font_icon;
	Texture* folder_icon;

	std::string selected_file_path;
	std::string selected_folder;
	std::string assets_folder_path;
	std::string delete_path;
};

