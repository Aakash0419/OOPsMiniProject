#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Box.H>
#include <iostream>
#include <fstream>
#include <string>

class TextEditor {
public:
    TextEditor();
    void show();

private:
    static void cb_open(Fl_Widget* widget, void* data);
    static void cb_save(Fl_Widget* widget, void* data);
    static void cb_find(Fl_Widget* widget, void* data);
    static void cb_find_dialog(Fl_Widget* widget, void* data);
    static void cb_replace_dialog(Fl_Widget* widget, void* data);
    static void cb_close_dialog(Fl_Widget* widget, void* data);

    void open_file(const std::string& filename);
    void save_file(const std::string& filename);
    void search_text(const std::string& search_term);
    void replace_text(const std::string& search_term, const std::string& replace_term);

    Fl_Window* window;
    Fl_Text_Editor* editor;
    Fl_Menu_Bar* menubar;
    Fl_Window* find_window;
    Fl_Input* find_input;
    Fl_Input* replace_input;
};

TextEditor::TextEditor() {
    window = new Fl_Window(600, 400, "Simple Text Editor");
    menubar = new Fl_Menu_Bar(0, 0, 600, 30);

    menubar->add("File/Open", 0, cb_open, this);
    menubar->add("File/Save", 0, cb_save, this);
    menubar->add("Edit/Find", 0, cb_find, this);

    editor = new Fl_Text_Editor(10, 40, 580, 300);
    editor->buffer(new Fl_Text_Buffer());

    Fl_Button* find_button = new Fl_Button(10, 350, 80, 30, "Find");
    find_button->callback(cb_find_dialog, this);

    find_window = nullptr;

    window->end();
}

void TextEditor::cb_open(Fl_Widget* widget, void* data) {
    TextEditor* editor_instance = static_cast<TextEditor*>(data);
    const char* filename = fl_file_chooser("Open File", "*", 0);
    if (filename) {
        editor_instance->open_file(filename);
    }
}

void TextEditor::cb_save(Fl_Widget* widget, void* data) {
    TextEditor* editor_instance = static_cast<TextEditor*>(data);
    const char* filename = fl_file_chooser("Save File", "*.txt", 0);
    if (filename) {
        editor_instance->save_file(filename);
    }
}

void TextEditor::cb_find(Fl_Widget* widget, void* data) {
    TextEditor* editor_instance = static_cast<TextEditor*>(data);
    const char* search_term = editor_instance->find_input->value();
    editor_instance->search_text(search_term);
}

void TextEditor::cb_find_dialog(Fl_Widget* widget, void* data) {
    TextEditor* editor_instance = static_cast<TextEditor*>(data);

    if (!editor_instance->find_window) {
        editor_instance->find_window = new Fl_Window(300, 150, "Find and Replace");

        Fl_Box* find_label = new Fl_Box(10, 10, 280, 25, "Find:");
        editor_instance->find_input = new Fl_Input(10, 40, 280, 25);

        Fl_Box* replace_label = new Fl_Box(10, 70, 280, 25, "Replace with:");
        editor_instance->replace_input = new Fl_Input(10, 100, 280, 25);

        Fl_Button* find_button = new Fl_Button(10, 130, 80, 30, "Find");
        find_button->callback(cb_find, editor_instance);

        Fl_Button* replace_button = new Fl_Button(100, 130, 80, 30, "Replace");
        replace_button->callback(cb_replace_dialog, editor_instance);

        Fl_Button* close_button = new Fl_Button(190, 130, 80, 30, "Close");
        close_button->callback(cb_close_dialog, editor_instance);

        editor_instance->find_window->end();
    }
    editor_instance->find_window->show();
}

void TextEditor::cb_replace_dialog(Fl_Widget* widget, void* data) {
    TextEditor* editor_instance = static_cast<TextEditor*>(data);
    const char* search_term = editor_instance->find_input->value();
    const char* replace_term = editor_instance->replace_input->value();
    editor_instance->replace_text(search_term, replace_term);
}

void TextEditor::cb_close_dialog(Fl_Widget* widget, void* data) {
    TextEditor* editor_instance = static_cast<TextEditor*>(data);
    editor_instance->find_window->hide();
}

void TextEditor::search_text(const std::string& search_term) {
    Fl_Text_Buffer* buffer = editor->buffer();
    std::string text = buffer->text();
    size_t pos = text.find(search_term);

    if (pos != std::string::npos) {
        fl_message("Text found at position: %zu", pos);
    } else {
        fl_message("Text not found.");
    }
}

void TextEditor::replace_text(const std::string& search_term, const std::string& replace_term) {
    Fl_Text_Buffer* buffer = editor->buffer();
    std::string text = buffer->text();

    size_t pos = text.find(search_term);
    while (pos != std::string::npos) {
        text.replace(pos, search_term.length(), replace_term);
        pos = text.find(search_term, pos + replace_term.length());
    }

    buffer->text(text.c_str());
}

void TextEditor::open_file(const std::string& filename) {
    std::ifstream file(filename);
    if (file) {
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        editor->buffer()->text(content.c_str());
    } else {
        std::cerr << "Error opening file: " << filename << std::endl;
    }
}

void TextEditor::save_file(const std::string& filename) {
    std::ofstream file(filename);
    if (file) {
        file << editor->buffer()->text();
    } else {
        std::cerr << "Error saving file: " << filename << std::endl;
    }
}

void TextEditor::show() {
    window->show();
}

int main(int argc, char** argv) {
    Fl::scheme("gtk+");
    TextEditor editor;
    editor.show();
    return Fl::run();
}
