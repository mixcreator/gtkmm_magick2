#include <iostream>
#include <variant>
#include <boost/regex.hpp>

#include "main_window.h"

namespace MainWindow{

MainWindow::MainWindow()
: ui{Gtk::Builder::create_from_file("simple_with_dlg.glade")}, 
    regex_int("^(\\+|-)?\\d+$"),
    regex_float("^(-?)(0|([1-9][0-9]*))(\\.[0-9]+)?$"),
    _watermark(std::make_unique<Watermark::Watermark>()),
    _image(std::make_unique<Image::Image>())
    
{
    set_title("Simple Gtk::Builder demo");
    set_default_size(400, 400);

    if(ui) 
    {
        ui->get_widget<Gtk::Box>("id_cont", cont);
        _widgets.push_back(cont);

        display_label = add_widget<Gtk::Label>("id_display_label");
        status_label = add_widget<Gtk::Label>("id_status_label"); 
        display_btn = add_widget<Gtk::Button>("id_display_button");
        
        menu_file_quit = add_widget<Gtk::ImageMenuItem>("id_menu_file_exit");            
        menu_file_load = add_widget<Gtk::ImageMenuItem>("id_menu_file_load");
        menu_file_save = add_widget<Gtk::ImageMenuItem>("id_menu_file_save");            

        menu_edit_rotate = add_widget<Gtk::MenuItem>("id_menu_edit_rotate");
        menu_edit_resize = add_widget<Gtk:: MenuItem>("id_menu_edit_resize");
        menu_edit_watermark = add_widget<Gtk:: MenuItem>("id_menu_edit_watermark");
        

        // --------------- Rotate Dialog ---------------------------------
        rotate_dialog = add_widget<Gtk::Dialog>("id_rotate_dialog");
        rotate_ok_btn = add_widget<Gtk::Button>("id_rotate_ok");
        rotate_cancel_btn = add_widget<Gtk::Button>("id_rotate_cancel");
        rotate_edit = add_widget<Gtk::TextView>("id_rotate_edit");
        //----------------------------------------------------------------

        // --------------- Resize Dialog ---------------------------------
        resize_dialog = add_widget<Gtk::Dialog>("id_resize_dialog");
        resize_ok_btn = add_widget<Gtk::Button>("id_resize_ok");
        resize_cancel_btn = add_widget<Gtk::Button>("id_resize_cancel");
        resize_width_edit = add_widget<Gtk::TextView>("id_resize_width_edit");
        resize_height_edit = add_widget<Gtk::TextView>("id_resize_height_edit");
        //----------------------------------------------------------------

        if(check_widgets())
        {
            display_btn->signal_clicked().connect(
            [this]() {
                display_label->set_text("Hello World");
            });         


            menu_edit_rotate->signal_activate().connect(
            [this]() {
                display_label->set_text("Rotate");
                rotate_dialog->set_transient_for(*this);
                auto buffer = rotate_edit->get_buffer();
                buffer->set_text("0.0");
                rotate_dialog->set_title("Rotate");
                
                std::cout << "Rotate dlg return: " << rotate_dialog->run() << std::endl;
            });

            menu_edit_resize->signal_activate().connect(
            [this]() {
                display_label->set_text("Resize");
                resize_dialog->set_transient_for(*this);
                resize_dialog->set_title("Resize");
                resize_dialog->run();
            });

            menu_file_quit->signal_activate().connect(
            [this]() {
                display_label->set_text("Quit");
                hide();
            });

            menu_file_load->signal_activate().connect(
                sigc::mem_fun(*this, &MainWindow::on_openfile_dialog));
            
            menu_file_save->signal_activate().connect(
                sigc::mem_fun(*this, &MainWindow::on_savefile_dialog));

            menu_edit_watermark->signal_activate().connect(
            [this]() 
            {
                std::cout << "Doing watermark" << std::endl;
                if(_watermark->Do(_image->getImage())){
                    display_label->set_text("Watermark");
                    std::cout << "Done" << std::endl;
                } else{
                    std::cout << "Watermark error" << std::endl;
                }

            });


            // For Rotate dialog !!!    
            rotate_ok_btn->signal_clicked().connect(
            [this]() 
            {
                auto content = rotate_edit->get_buffer()->get_text();
                std::string::size_type sz;
                if(_image->Rotate(std::stod(content, &sz)))
                {
                    auto image = _image->getImage();
                    std::cout << "Rotated: " << content << "\'  " << image.columns() << "x" << image.rows() << std::endl;           
                    display_label->set_text(
                        content + "\n" + 
                        std::to_string((int)(image.columns())) + "x" + 
                        std::to_string((int)(image.rows()))
                    );                    
                } 
                else
                {
                    std::cout << "Rotate error" << std::endl;
                }

                rotate_dialog->hide();
            });

            rotate_edit->get_buffer()->signal_changed().connect(
            [this]() {
                auto content = rotate_edit->get_buffer()->get_text();
                auto status = boost::regex_match(std::string(content), regex_float);
                rotate_ok_btn->set_sensitive(status);
            });

            rotate_cancel_btn->signal_clicked().connect(
            [this]() {
                rotate_dialog->hide();
            });


            // For Resize dialog !!!
            resize_ok_btn->signal_clicked().connect(
            [this]() {
                auto content_width = resize_width_edit->get_buffer()->get_text();
                auto content_height = resize_height_edit->get_buffer()->get_text();
            
                if(_image->Resize(content_width + "x" + content_height))
                {
                    auto image = _image->getImage();
                    std::cout << "Resized: " << image.columns() << "x" << image.rows() << std::endl;
                    display_label->set_text(std::to_string(image.columns()) + "x" +  std::to_string(image.rows()));
                }
                else
                {
                    std::cout << "Resize error" << std::endl;
                }                
                resize_dialog->hide();
            });

            auto resize_edit_slot =
            [this]() {
                auto content_width = resize_width_edit->get_buffer()->get_text();
                auto content_height = resize_height_edit->get_buffer()->get_text();
                auto status_width = boost::regex_match(std::string(content_width), regex_int);
                auto status_height = boost::regex_match(std::string(content_height), regex_int);
                resize_ok_btn->set_sensitive(status_width & status_height);
            };

            resize_width_edit->get_buffer()->signal_changed().connect(resize_edit_slot);

            resize_height_edit->get_buffer()->signal_changed().connect(resize_edit_slot);

            resize_cancel_btn->signal_clicked().connect(
            [this]() {
                resize_dialog->hide();
            });

            if(!_watermark->Load("wmark_image.png")){
                std::cout << "Watermark image was not loaded !" << std::endl;
            }

            add(*cont);
        }
    }
    show_all();
}

MainWindow::~MainWindow() 
{

}

template <typename T>
Glib::RefPtr<T> MainWindow::add_widget(const std::string id){
    auto w = Glib::RefPtr<T>::cast_dynamic(ui->get_object(id));
    _widgets.push_back(w);
    return w;        
}

void MainWindow::on_openfile_dialog()
{
    Gtk::FileChooserDialog dialog("Please choose a file",
            Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);

    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Open", Gtk::RESPONSE_OK);

    auto filter = Gtk::FileFilter::create();
    filter->set_name("PNG and JPEG images");
    filter->add_mime_type("image/png");
    filter->add_pattern("*.png");
    filter->add_mime_type("image/jpeg");
    filter->add_pattern("*.jpg");

    dialog.add_filter(filter);

    //Show the dialog and wait for a user response:
    int result = dialog.run();
    if(result == Gtk::RESPONSE_OK) {
        std::string filename = dialog.get_filename();
        
        std::cout << "File selected: " <<  filename << std::endl;                     
        
        display_label->set_text("Load");
        //menu_file_load->set_sensitive(false); //????
        menu_file_save->set_sensitive(true);
        menu_edit_rotate->set_sensitive(true); 
        menu_edit_resize->set_sensitive(true);
        menu_edit_watermark->set_sensitive(true);

        if(_image->Load(dialog.get_filename()))
        {
            status_label->set_text("Loaded  " + filename);
            auto image = _image->getImage();
            std::cout << "Read to Image: " << filename << "("<< image.columns() << "x" << image.rows() << ")" <<std::endl;
        } 
        else
        {
            std::cout << "Load Error: " << filename << std::endl;
        }
    }

}

void MainWindow::on_savefile_dialog()
{
    Gtk::FileChooserDialog dialog("Please choose a file",
            Gtk::FILE_CHOOSER_ACTION_SAVE);
    dialog.set_transient_for(*this);

    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("_Save", Gtk::RESPONSE_OK);


    int result = dialog.run();
    if(result == Gtk::RESPONSE_OK)
    {
        std::cout << "Save clicked." << std::endl;

        std::string filename = dialog.get_filename();
        std::cout << "File selected: " <<  filename << std::endl;  
        
        if(_image->Save( dialog.get_filename()))
        {
            status_label->set_text("Saved  " + filename);
        }
        else        
        {
            std::cout << "Save Error: " << filename << std::endl;
        }
    }
    
}


bool MainWindow::check_widgets() const
{
    // Original from exmaple!
    /*
    for (auto& el : widgets)
    {
        std::visit([](auto& arg){
            using T = std::decay_t<decltype(arg)>;
            if constexpr(std::is_same_v<T, Glib::RefPtr<Gtk::Dialog>>)
            {
                std::cout << "Glib::RefPtr<Gtk::Dialog>: " << std::endl;
            }
            else if constexpr(std::is_same_v<T, Glib::RefPtr<Gtk::Button>>)
            {
                std::cout << "Glib::RefPtr<Gtk::Button>: " << std::endl;
            }
            else if constexpr(std::is_same_v<T, Glib::RefPtr<Gtk::TextView>>)
            {
                std::cout << "Glib::RefPtr<Gtk::TextView>: " << std::endl;
                std::cout << "val: " << static_cast<bool>(arg) << "\n";
            }
            else if constexpr(std::is_same_v<T, Glib::RefPtr<Gtk::ImageMenuItem>>)
            {
                std::cout << "Glib::RefPtr<Gtk::ImageMenuItem>: " << std::endl;
            }
            else if constexpr(std::is_same_v<T, Glib::RefPtr<Gtk::Label>>)
            {
                std::cout << "Glib::RefPtr<Gtk::Label>: " << std::endl;
            }
            else if constexpr(std::is_same_v<T, Gtk::Box* >)
            {
                std::cout << "Gtk::Box*: " << std::endl;
                std::cout << "val: " << static_cast<bool>(arg) << "\n";
            }                                                                                        
        }, el);
    }
    */

    bool res = true;
    for (auto& el : _widgets)
    {
        std::visit([&res](auto& arg){
            if(!static_cast<bool>(arg))
            {
                std::cout << "arg: " << static_cast<bool>(arg) << std::endl;
                res = false;
                return;
            }
        }, el); 
    }        

    return res;
}

} // namespace MainWindow


