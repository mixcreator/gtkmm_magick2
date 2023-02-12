#pragma once

#include <gtkmm.h>
#include <iostream>
#include <boost/regex.hpp>
#include <variant>

#include "image.h"
#include "watermark.h"

namespace MainWindow{

class MainWindow : public Gtk::ApplicationWindow {
public:
    MainWindow();
    ~MainWindow();

private:
    Gtk::Box *cont;
    Glib::RefPtr<Gtk::Label> display_label;
    Glib::RefPtr<Gtk::Button> display_btn;
    Glib::RefPtr<Gtk::Label> status_label;
    Glib::RefPtr<Gtk::Builder> ui;
    
    Glib::RefPtr<Gtk::ImageMenuItem> menu_file_load;
    Glib::RefPtr<Gtk::ImageMenuItem> menu_file_save;
    Glib::RefPtr<Gtk::ImageMenuItem> menu_file_quit;
    
    Glib::RefPtr<Gtk::MenuItem> menu_edit_rotate;
    Glib::RefPtr<Gtk::MenuItem> menu_edit_resize;
    
    Glib::RefPtr<Gtk::Dialog> rotate_dialog;
    Glib::RefPtr<Gtk::Button> rotate_ok_btn;
    Glib::RefPtr<Gtk::Button> rotate_cancel_btn;
    Glib::RefPtr<Gtk::TextView> rotate_edit;

    Glib::RefPtr<Gtk::Dialog> resize_dialog;
    Glib::RefPtr<Gtk::Button> resize_ok_btn;
    Glib::RefPtr<Gtk::Button> resize_cancel_btn;
    Glib::RefPtr<Gtk::TextView> resize_width_edit;
    Glib::RefPtr<Gtk::TextView> resize_height_edit;

    std::vector< std::variant< Glib::RefPtr<Gtk::Dialog>, 
                               Glib::RefPtr<Gtk::Button>, 
                               Glib::RefPtr<Gtk::TextView>, 
                               Glib::RefPtr<Gtk::ImageMenuItem>,
                               Glib::RefPtr<Gtk::MenuItem>,
                               Glib::RefPtr<Gtk::Label>,  
                               Gtk::Box* > > _widgets;
    
    boost::regex regex_int;
    boost::regex regex_float;

    // Watermark::Watermark _watermark;
    std::unique_ptr<Watermark::Watermark> _watermark;
    // Image::Image _image;
    std::unique_ptr<Image::Image> _image;

    //double _rotate_degree = 0.0;

private:
    template <typename T>
    Glib::RefPtr<T> add_widget(const std::string id);

    bool check_widgets() const;

    void on_openfile_dialog();
    void on_savefile_dialog();
};

} // namespace MainWindow
