#include <gtkmm.h>
#include <iostream>
#include <any>
#include <boost/regex.hpp>
#include <algorithm>
#include <variant>
//#include <boost/algorithm/cxx11/any_of.hpp>


/*
// original code !!!!
class HelloWindow : public Gtk::ApplicationWindow {
    Gtk::Box *cont;
    Glib::RefPtr<Gtk::Label> display_label;
    Glib::RefPtr<Gtk::Button> display_btn;
    Glib::RefPtr<Gtk::Builder> ui;
public:
    HelloWindow()
    : ui{Gtk::Builder::create_from_file("simple.glade")} {
        if(ui) {
            ui->get_widget<Gtk::Box>("id_cont", cont);
            display_label = Glib::RefPtr<Gtk::Label>::cast_dynamic(
                ui->get_object("display_label")
            );
            display_btn = Glib::RefPtr<Gtk::Button>::cast_dynamic(
                ui->get_object("display_button")
            );
            if(cont && display_label && display_btn) {
                display_btn->signal_clicked().connect(
                [this]() {
                    display_label->set_text("Hello World");
                });
                add(*cont);
            }
        }
        set_title("Simple Gtk::Builder demo");
        set_default_size(400, 400);
        show_all();
    }
};
*/

class HelloWindow : public Gtk::ApplicationWindow {
    Gtk::Box *cont;
    Glib::RefPtr<Gtk::Label> display_label;
    Glib::RefPtr<Gtk::Button> display_btn;
    Glib::RefPtr<Gtk::Label> status_label;
    Glib::RefPtr<Gtk::Builder> ui;
    
    Glib::RefPtr<Gtk::ImageMenuItem> menu_file_load;
    Glib::RefPtr<Gtk::ImageMenuItem> menu_file_save;
    Glib::RefPtr<Gtk::ImageMenuItem> menu_file_quit;

    
    Glib::RefPtr<Gtk::ImageMenuItem> menu_edit_rotate;   

    
    // Rotate dialog
    Glib::RefPtr<Gtk::Dialog> rotate_dialog;
    Glib::RefPtr<Gtk::Button> rotate_ok_btn;
    Glib::RefPtr<Gtk::Button> rotate_cancel_btn;
    Glib::RefPtr<Gtk::TextView> rotate_edit;
    //////////////////////

    // Resize dialog
    Glib::RefPtr<Gtk::Dialog> resize_dialog;
    Glib::RefPtr<Gtk::Button> resize_ok_btn;
    Glib::RefPtr<Gtk::Button> resize_cancel_btn;
    Glib::RefPtr<Gtk::TextView> resize_width_edit;
    Glib::RefPtr<Gtk::TextView> resize_height_edit;

    //std::vector<std::any> widgets;
    std::vector< std::variant< Glib::RefPtr<Gtk::Dialog>, 
                               Glib::RefPtr<Gtk::Button>, 
                               Glib::RefPtr<Gtk::TextView>, 
                               Glib::RefPtr<Gtk::ImageMenuItem>, 
                               Glib::RefPtr<Gtk::Label>,  
                               Gtk::Box* > > widgets;
    
    template <typename T>
    Glib::RefPtr<T> getWidgetAndAdd(const std::string id){
        auto w = Glib::RefPtr<T>::cast_dynamic(ui->get_object(id));
        widgets.push_back(w);
        return w;        
    }

    bool checkWidgets() const
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
        for (auto& el : widgets)
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

public:
    HelloWindow()
    : ui{Gtk::Builder::create_from_file("simple_with_dlg.glade")} {
        if(ui) {
            ui->get_widget<Gtk::Box>("id_cont", cont);
            widgets.push_back(cont);

            display_label = getWidgetAndAdd<Gtk::Label>("id_display_label");
            status_label = getWidgetAndAdd<Gtk::Label>("id_status_label"); 
            display_btn = getWidgetAndAdd<Gtk::Button>("id_display_button");
            menu_file_quit = getWidgetAndAdd<Gtk::ImageMenuItem>("id_menu_file_exit");            
            menu_file_load = getWidgetAndAdd<Gtk::ImageMenuItem>("id_menu_file_load");
            menu_file_save = getWidgetAndAdd<Gtk::ImageMenuItem>("id_menu_file_save");            
            menu_edit_rotate = getWidgetAndAdd<Gtk::ImageMenuItem>("id_menu_edit_rotate");
            

            // --------------- Rotate Dialog ---------------------------------
            rotate_dialog = getWidgetAndAdd<Gtk::Dialog>("id_rotate_dialog");
            rotate_ok_btn = getWidgetAndAdd<Gtk::Button>("id_rotate_ok");
            rotate_cancel_btn = getWidgetAndAdd<Gtk::Button>("id_rotate_cancel");
            rotate_edit = getWidgetAndAdd<Gtk::TextView>("id_rotate_edit");
            //----------------------------------------------------------------



            if(checkWidgets())
            {
                display_btn->signal_clicked().connect(
                [this]() {
                    display_label->set_text("Hello World");
                });
                
                // For Rotate dialog !!!
                rotate_ok_btn->signal_clicked().connect(
                [this]() {
                    std::cout << " Rotate OK" << std::endl;
                    auto content = rotate_edit->get_buffer()->get_text();
                    display_label->set_text(content);
                    // Magick++ rotate
                    rotate_dialog->hide();
                });
                rotate_edit->get_buffer()->signal_changed().connect(
                [this]() {
                    std::cout << " Rotate changed" << std::endl;
                    auto content = rotate_edit->get_buffer()->get_text();
                    std::cout << content << std::endl;

                    boost::regex regExInt("^(\\+|-)?\\d+$");
                    boost::regex regExDouble("^(-?)(0|([1-9][0-9]*))(\\.[0-9]+)?$");
                    
                    auto status = boost::regex_match(std::string(content), regExDouble);
                    rotate_ok_btn->set_sensitive(status);
                });
                rotate_cancel_btn->signal_clicked().connect(
                [this]() {
                    rotate_dialog->hide();
                    std::cout << " Rotate Cancel" << std::endl;
                });
                ////////////////////////

                menu_file_quit->signal_activate().connect(
                [this]() {
                    display_label->set_text("Quit");
                });
                
                
                menu_edit_rotate->signal_activate().connect(
                [this]() {
                    display_label->set_text("Rotate");
                    rotate_dialog->set_transient_for(*this);
                    rotate_dialog->run();
                });
                              

                menu_file_load->signal_activate().connect(
                [this]() {

                    /////////////////////////////////////////////////
                    Gtk::FileChooserDialog dialog("Please choose a file",
                            Gtk::FILE_CHOOSER_ACTION_OPEN);
                    dialog.set_transient_for(*this);

                    //Add response buttons the the dialog:
                    dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
                    dialog.add_button("_Open", Gtk::RESPONSE_OK);

                    //Add filters, so that only certain file types can be selected:
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
                        status_label->set_text("Loaded  " + filename);
                        std::cout << "File selected: " <<  filename << std::endl;                     
                        
                        display_label->set_text("Load");
                        menu_file_load->set_sensitive(false);
                        menu_file_save->set_sensitive(true);
                        menu_edit_rotate->set_sensitive(true);                       
                    }
                });
          
                menu_file_save->signal_activate().connect(
                [this]() {
                    display_label->set_text("Save");
                    menu_file_load->set_sensitive(true);
                    menu_file_save->set_sensitive(false);
                });

                add(*cont);
            }
        }
        set_title("Simple Gtk::Builder demo");
        set_default_size(400, 400);
        show_all();
    }
};
int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(
        argc, argv, 
        "org.gtkmm.example.HelloApp"
    );
    HelloWindow hw;
    return app->run(hw);
}